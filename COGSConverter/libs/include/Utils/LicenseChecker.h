/*
  Copyright (C) Skeletex Research, s.r.o. - All Rights Reserved
  Unauthorized copying of this file, via any medium is strictly prohibited
  Proprietary and confidential
*/
#pragma once

#include <optional>

#include <Utils/API.h>
#include <Utils/Notifier.h>

namespace utils
{
  enum class LicenseState
  {
    valid,
    expired,
    not_found,
    used,
    invalid,
    keylok_invalid
  };
  using LicenseNotifier = Notifier<std::vector<LicenseState>, const std::string &>;



  /*!
    \brief
      A utility class to check software license - License key or Keylok USB Dongle presence.
    \note
      Checking is performed on a different thread every CHECK_INTERVAL milliseconds.
      A number of MAX_UNSUCCESSFUL_TRIES consequent checks must fail in order for the failure to be reported.
      The failure is reported by a Notification.
      Regardless of the Notification being caught and displayed, the LicenseChecker will throw after a short period.
  */
  class UTILS_API LicenseChecker
    : public LicenseNotifier
  {
  public:
    //! Get the singular instance of LicenseChecker that is instantiated.
    static LicenseChecker &GetSingleton();

    /*!
      \brief
        Start checking for USB license key dongle connection.
      \param serial_key
        A serial key or if specified. If not specified, only Keylok USB Dongle is checked.
    */
    void StartChecking(const std::string &serial_key = "");

  private:
    LicenseChecker();
    ~LicenseChecker();

    //! Fail check - throw & send a notification - allow the listener to handle the situation.
    void FailCheck(const std::vector<LicenseState> &states);

    struct Impl;
    Impl *m = nullptr;
  };
}
