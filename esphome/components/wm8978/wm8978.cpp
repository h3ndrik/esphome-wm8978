#include "wm8978.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include <cinttypes>

namespace esphome {
namespace wm8978 {

static const char *const TAG = "wm8978";

void WM8978::setup() {
  ESP_LOGCONFIG(TAG, "Setting up WM8978 Audio Codec...");
  // Reset all registers
  if (this->write_register_(0, 0) != esphome::i2c::ERROR_OK) {
    ESP_LOGE(TAG, "WM8978 not found at address 0x%02X", this->address_);
    this->mark_failed();
    return;
  }
  ESP_LOGD("WM8978", "WM8978 found at address 0x%02X", codec_addr);

  //Power-up when NOT using the output 1.5x boost stage:
  //1. Turn on external power supplies. Wait for supply voltage to settle.
  //2. Mute all analogue outputs.
  this->set_mute_on(true);

  //3. Set L/RMIXEN = 1 and DACENL/R = 1 in register R3.
  this->write_register_(3, 0x6C);  // LOUT2, ROUT2 enable (speaker), RMIX, LMIX enable

  //4. Set BUFIOEN = 1 and VMIDSEL[1:0] to required value in register R1. Wait for the VMID supply
  //to settle. *Refer notes 1 and 2.
  //5. Set BIASEN = 1 in register R1.
  this->write_register_(1, 0x1B);  // MICEN, BIASEN, VMIDSEL[1:0]=11 (50k)

  //6. Set L/ROUT1EN = 1 in register R2.
  this->write_register_(2, 0x1B0); // ROUT1, LOUT1 enable (headphone), BOOSTENR, BOOSTENL

  //7. Enable other mixers as required.
  // Explicitly set ADC input mixer settings
  this->write_register_(43, 1<<4);  // Enable INVROUT2 (for speaker)

  //8. Enable other outputs as required.
  // Digital audio path
  this->write_register_(10, 1<<3);  // SOFTMUTE closed, 128x oversampling, best SNR

  // Input boost
  this->write_register_(47, 1<<8); // Left ADC = 0dB
  this->write_register_(48, 1<<8); // Right ADC = 0dB

  // ADC settings
  this->write_register_(14, 1<<3 | 1 << 8);  // ADC 128x oversampling, and enable high pass filter (3.7Hz cut-off)

  //9. Set remaining registers.
  // Basic initialization sequence
  this->write_register_(4, 16);   // 16-bit word length, I2S format
  this->write_register_(6, 0);    // MCLK as clock source

  // Additional settings
  this->write_register_(49, 1<<1);    // TSDEN (thermal shutdown enable)

  // Enable DAC, ADC
  this->write_register_(2, 0x1B0 | 3<<2 | 3);
  this->write_register_(3, 0x6C | 3);  // Enable DACR & DACL

  // Enable microphone inputs
  this->write_register_(44, 0x33); // LINPUT1 and RINPUT1 enabled, +20dB boost

  // Input mixer, Set higher microphone gain for recording
  this->set_mic_gain(24.0);

  // Configure EQ to reduce static - THIS IS KEY FOR REDUCING STATIC
  //this->write_register_(18, 0x12C | (0x3 << 5) | 8);  // EQ1 - 80Hz (bass): +2dB
  //this->write_register_(19, 0x2C | (0x0 << 5) | 8);   // EQ2 - 230Hz: -4dB (reduce mid-low noise)
  //this->write_register_(20, 0x2C | (0x0 << 5) | 8);   // EQ3 - 650Hz: -4dB (reduce mid noise)
  //this->write_register_(21, 0x2C | (0x3 << 5) | 4);   // EQ4 - 4.1kHz: +2dB (enhance clarity)
  //this->write_register_(22, 0x2C | (0x3 << 5) | 8);   // EQ5 - 11.7kHz: +2dB (enhance clarity)

  // Configure output channels - enable DAC output
  this->write_register_(50, 1);  // Left DAC to left mixer
  this->write_register_(51, 1);  // Right DAC to right mixer

  // Output limiter boost
  //this->write_register_(25, 4 & 0x0F); // Limiter Boost = +4dB

  // Set volume, disable mute
  this->set_volume(0.75);

  ESP_LOGI("wm8978_init", "WM8978 initialized");

  // Enable speaker amplifier
  //id(internal_speaker_amp).turn_on();
  //ESP_LOGD("WM8978", "Speaker amplifier enabled");

}
void WM8978::dump_config() {
  ESP_LOGCONFIG(TAG, "WM8978 Audio Codec:");
  LOG_I2C_DEVICE(this);

  if (this->is_failed()) {
    ESP_LOGCONFIG(TAG, "  Failed to initialize");
    return;
  }
}

bool WM8978::set_volume(float volume) {
  this->volume_ = clamp(volume, 0.0f, 1.0f);
  uint8_t reg = remap<uint8_t, float>(this->volume_, 0.0f, 1.0f, 0, 63);
  this->write_register_(52, reg);          // Left headphone volume
  this->write_register_(53, reg | 0x100);  // Right headphone volume + update
  this->write_register_(54, reg);          // Left speaker volume
  this->write_register_(55, reg | 0x100);  // Right speaker volume + update
  return true
}

bool WM8978::set_mute_state_(bool mute_state) {
  this->is_muted_ = mute_state;
  uint8_t reg = remap<uint8_t, float>(this->volume_, 0.0f, 1.0f, 0, 63) & 0x3F;

  if (mute_state) {
    reg |= 1<<6;  //BIT(5);
  }

  ESP_LOGV(TAG, "Setting WM8978_DAC_MUTE to %u (muted: %s)", reg, YESNO(mute_state));
  this->write_register_(52, reg); // LOUT1MUTE
  this->write_register_(53, reg); // ROUT1MUTE
  this->write_register_(54, reg); // LOUT2MUTE
  this->write_register_(55, reg); // ROUT2MUTE
  return true
}

bool WM8978::set_mic_gain(float mic_gain) {
  this->mic_gain_ = clamp<float>(mic_gain, -12.0f, 35.25f);
  uint8_t reg = remap<uint8_t, float>(this->mic_gain_, -12.0f, 35.25f, 0, 63);
  this->write_register_(45, reg); // Left input PGA
  this->write_register_(46, reg | 0x100); // Right input PGA
  return true;
}

esphome::i2c::ErrorCode WM8978::write_register_(uint8_t reg, uint16_t value) {
  uint8_t first = (reg << 1) | ((value >> 8) & 0X01);
  uint8_t second = value & 0xFF;
  return this->write_register(first, &second, 1);
}

}  // namespace wm8978
}  // namespace esphome
