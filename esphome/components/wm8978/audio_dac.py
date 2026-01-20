import esphome.codegen as cg
from esphome.components import i2c
from esphome.components.audio_dac import AudioDac
from esphome import automation
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_MIC_GAIN, CONF_STATE

CODEOWNERS = ["@h3ndrik"]
DEPENDENCIES = ["i2c"]

wm8978_ns = cg.esphome_ns.namespace("wm8978")
WM8978 = wm8978_ns.class_("WM8978", AudioDac, cg.Component, i2c.I2CDevice)

StandbyAction = wm8978_ns.class_("StandbySetStateAction", automation.Action)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(WM8978),
            cv.Optional(CONF_MIC_GAIN, default="0db"): cv.All(
                cv.decibel, cv.float_range(min=-12.0, max=35.25)
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x1A))
)


@automation.register_action(
    "wm8978.standby",
    StandbyAction,
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(WM8978),
            cv.Required(CONF_STATE): cv.boolean,
        },
    ),
)
async def wm8978_standby_set_state_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    cg.add(var.set_state(config[CONF_STATE]))
    return var


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_mic_gain(config[CONF_MIC_GAIN]))
