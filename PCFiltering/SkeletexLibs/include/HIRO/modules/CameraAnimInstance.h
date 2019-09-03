#pragma once
#include <jansson/jansson.h>
#include <COGS/Interpolator.h>
#include <HIRO/modules/CameraAnimUnit.h>



namespace hiro
{
  namespace modules
  {

    class CameraAnimInstance : public hiro::DataInstance
    {
    public:
      CameraAnimInstance(const CameraAnimUnit *unit);
      void GenerateGui(hiro::GuiGenerator &gui) override;
      void Update(float time_delta) override;
    private:
      cogs::Interpolator<hiro::Camera::Pose> interpolator_;
      gui::Checkbox *loop_playback_;
      gui::Checkbox *rotate_around_;
      gui::NumericFloat *playback_speed_;
      gui::NumericFloat *key_deltatime_;
      bool is_playing_first_frame_ = false;
      bool is_playing_ = false;
      float play_time_ = 0.0f;

      void AddKeyframe();
      void RemoveLastKeyframe();
      void RemoveAllKeyframes();

      void PlayAnimation();
      void StopAnimation();

      void SaveAnimation();
      void LoadAnimation();
    };

  }
}

