#pragma once

#include "esphome/core/component.h"
#include "esphome/components/audio_dac/audio_dac.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace wm8978 {

class WM8978 : public audio_dac::AudioDac, public Component, public i2c::I2CDevice {
 public:
  /////////////////////////
  // Component overrides //
  /////////////////////////

  void setup() override;
  void dump_config() override;

  ////////////////////////
  // AudioDac overrides //
  ////////////////////////

  /// @brief Writes the volume out to the DAC
  /// @param volume floating point between 0.0 and 1.0
  /// @return True if successful and false otherwise
  bool set_volume(float volume) override;

  /// @brief Gets the current volume out from the DAC
  /// @return floating point between 0.0 and 1.0
  float volume() override { return this->volume_; }

  /// @brief Disables mute for audio out
  /// @return True if successful and false otherwise
  bool set_mute_off() override { return this->set_mute_state_(false); }

  /// @brief Enables mute for audio out
  /// @return True if successful and false otherwise
  bool set_mute_on() override { return this->set_mute_state_(true); }

  bool is_muted() override { return this->is_muted_; }

  bool set_mic_gain(float mic_gain);
  float mic_gain() override { return this->mic_gain_; };

 protected:
  esphome::i2c::ErrorCode write_register_(uint8_t reg, uint16_t value);

  /// @brief Mutes or unmutes the DAC audio out
  /// @param mute_state True to mute, false to unmute
  /// @return True if successful and false otherwise
  bool set_mute_state_(bool mute_state);
  bool set_sleep_state_(bool sleep_state);

  float volume_{0};
  float mic_gain_{0};
};

}  // namespace wm8978
}  // namespace esphome
