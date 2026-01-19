import esphome.codegen as cg
from esphome.components import i2c
from esphome.components.audio_dac import AudioDac
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_MIC_GAIN

CODEOWNERS = ["@h3ndrik"]
DEPENDENCIES = ["i2c"]

wm8978_ns = cg.esphome_ns.namespace("wm8978")
WM8978 = wm8978_ns.class_("WM8978", AudioDac, cg.Component, i2c.I2CDevice)

WM8978_MIC_GAINS = [-12, -11.25, -10.5, -9.75, -9, -8.25, -7.5, -6.75, -6, -5.25, -4.5, -3.75, -3, -2.25, -1.5, -0.75, 0, 0.75, 1.5, 2.25, 3, 3.75, 4.5, 5.25, 6, 6,75, 7.5, 8.25, 9, 9.75, 10.5, 11.25, 12, 12,75, 13.5, 14.25, 15, 15.75, 16.5, 17.25, 18, 18.75, 19.5, 20.25, 21, 21.75, 22.5, 23.25, 24, 24.75, 25.5, 26.25, 27, 27.75, 28.5, 29.25, 30, 30.75, 31.5, 32.25, 33, 33.75, 34.5, 35.25]

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(WM8978),
            cv.Optional(CONF_MIC_GAIN, default="0db"): cv.All(
                cv.decibel, cv.one_of(*WM8978_MIC_GAINS)
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x1A))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_mic_gain(config[CONF_MIC_GAIN]))
