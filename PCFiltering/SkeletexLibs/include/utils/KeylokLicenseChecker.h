/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once

#include <mutex>
#include <thread>
#include <condition_variable>

#include <PhoKeylok_API/Keylok.h>

#include <Utils/Threading.h>
#include <Utils/Exceptions.h>

#include <Utils/Notifier.h>

namespace utils
{
  enum class AuthenticationState
  {
    succeeded,
    failed
  };
  using AuthenticationNotifier = Notifier<const AuthenticationState, const std::string &>;


  /*!
    \brief
      A utility class to check Keylok USB Dongle presence.
    \note
      Checking is performed on a different thread every CHECK_INTERVAL milliseconds.
      A number of MAX_UNSUCCESSFUL_TRIES consequent checks must fail in order for the failure to be reported.
      The failure is reported by a Notification.
      Regardless of the Notification being caught and displayed, the KeylockLicenseChecker will throw after a while.
  */
  class KeylokLicenseChecker
    : public AuthenticationNotifier
  {
  public:
    inline static const std::string FAIL_MESSAGE = "\nUSB DONGLE LICENSE KEY NOT FOUND.\nThe application will now close...\n\n";
    static KeylokLicenseChecker &GetSingleton()
    {
      static KeylokLicenseChecker checker;
      return checker;
    }

    /*!
      \brief Start checking for USB license key dongle connection.
    */
    void StartChecking()
    {
      if (is_license_checking_thread_running_)
      {
        return;
      }

      is_license_checking_thread_running_ = true;
      unsuccessful_tries_ = 0;

      if (!CheckDongle())
      {
        FailDongleCheck();
      }


      license_checking_thread_ = std::thread([this]()
      {
        utils::SetThisThreadName("Keylock_Check");
        while (true)
        {
          std::unique_lock<std::mutex> lock(license_checking_thread_mutex_);

          if (!is_license_checking_thread_running_)
          {
            break;
          }

          license_checking_thread_condition_.wait_for(lock, std::chrono::milliseconds(CHECK_INTERVAL_MS));

          if (!CheckDongle())
          {
            if (++unsuccessful_tries_ >= MAX_UNSUCCESSFUL_TRIES)
            {
              FailDongleCheck();
            }
          }
          else
          {
            unsuccessful_tries_ = 0;
          }
        }
      });
    }

  private:
    KeylokLicenseChecker()
      : is_license_checking_thread_running_(false)
      , unsuccessful_tries_(0)
    {
    }

    ~KeylokLicenseChecker()
    {
      {
        std::lock_guard<std::mutex> lock(license_checking_thread_mutex_);
        is_license_checking_thread_running_ = false;
        license_checking_thread_condition_.notify_all();
      }
      if (license_checking_thread_.joinable())
      {
        license_checking_thread_.join();
      }
      if (fail_thread_.joinable())
      {
        fail_thread_.join();
      }
    }

    //! Use Keylok library to check the USB dongle connection.
    bool CheckDongle()
    {
      std::lock_guard lock(check_dongle_mutex_);
      return pho::Keylok::GetInstance()->CheckConnection();
    }

    //! Fail dongle check - send a notification and allow the listener to handle the situation.
    void FailDongleCheck()
    {
      fail_thread_ = std::thread([this]()
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(APPLICATION_CLOSE_TIMEOUT_MS));
        throw std_ext::AuthenticationError(FAIL_MESSAGE);
      });

      Notify(AuthenticationState::failed, FAIL_MESSAGE);
    }

    std::thread license_checking_thread_;
    std::mutex license_checking_thread_mutex_;
    std::mutex check_dongle_mutex_;
    std::condition_variable license_checking_thread_condition_;
    std::thread fail_thread_;
    bool is_license_checking_thread_running_;

    size_t unsuccessful_tries_;

    inline static const size_t MAX_UNSUCCESSFUL_TRIES = 10;
    inline static const size_t CHECK_INTERVAL_MS = 600;
    inline static const size_t APPLICATION_CLOSE_TIMEOUT_MS = 3000;
  };

}
