WM8978 dac;

class MyCustomWM8978Component : public Component {
 public:
  void setup() override {
    //PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
    //REG_WRITE(PIN_CTRL, 0xFFFFFFF0);
    dac.begin(19, 18);
    dac.cfgInput(1, 0, 0);
    dac.setMICgain(20);
    //dac.setALC(1, 7, 0);
    dac.setNoise(1, 3);
    dac.setHPF(1);
    dac.setSPKvol(50); // 0-63
    dac.setHPvol(32, 32);
    ESP_LOGD("custom", "WM8978 setup.");
    spk_vol = 50;
    hp_vol = 32;
  }
//  void loop() override {
//  }
  void on_mute() {
    dac.setSPKvol(0);
    dac.setHPvol(0, 0);
    ESP_LOGD("custom", "WM8978 muted");
  }
  void on_unmute() {
    dac.setSPKvol(50);
    dac.setHPvol(32, 32);
    ESP_LOGD("custom", "WM8978 unmuted");
  }
  void on_vol_up() {
    if (hp_vol < 63) hp_vol++;
    if (spk_vol < 63) spk_vol++;
    dac.setHPvol(hp_vol, hp_vol);
    dac.setSPKvol(spk_vol);
  }
  void on_vol_down() {
    if (hp_vol > 0) hp_vol--;
    if (spk_vol > 0) spk_vol--;
    dac.setHPvol(hp_vol, hp_vol);
    dac.setSPKvol(spk_vol);
  }
  private:
    uint8_t hp_vol;
    uint8_t spk_vol;
};
