#include "esphome/core/log.h"
#include "wm8978.h"

namespace esphome {
namespace wm8978 {

static const char *const TAG = "wm8978";

void WM8978Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up WM8978...");
/*  if (!this->read_gpio_()) {
    ESP_LOGE(TAG, "WM8978 not available under 0x%02X", this->address_);
    this->mark_failed();
    return;
  }

  this->write_gpio_();
  this->read_gpio_();*/
}
/*void WM8978Component::dump_config() {
  ESP_LOGCONFIG(TAG, "WM8978:");
  LOG_I2C_DEVICE(this)
  //ESP_LOGCONFIG(TAG, "  Is PCF8575: %s", YESNO(this->pcf8575_));
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with WM8978 failed!");
  }
}
bool WM8978Component::digital_read(uint8_t pin) {
  this->read_gpio_();
  return this->input_mask_ & (1 << pin);
}
void WM8978Component::digital_write(uint8_t pin, bool value) {
  if (value) {
    this->output_mask_ |= (1 << pin);
  } else {
    this->output_mask_ &= ~(1 << pin);
  }

  this->write_gpio_();
}*/

}  // namespace wm8978
}  // namespace esphome
