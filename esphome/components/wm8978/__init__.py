import esphome.codegen as cg
import esphome.config_validation as cv
#from esphome import pins
from esphome.components import i2c
from esphome.const import (
    CONF_ID,
#    CONF_INPUT,
#    CONF_NUMBER,
#    CONF_MODE,
#    CONF_INVERTED,
#    CONF_OUTPUT,
)

DEPENDENCIES = ["i2c"]
MULTI_CONF = True

wm8978_ns = cg.esphome_ns.namespace("wm8978")

WM8978Component = wm8978_ns.class_("WM8978Component", cg.Component, i2c.I2CDevice)
#WM8978GPIOPin = wm8978_ns.class_("WM8978GPIOPin", cg.GPIOPin)

CONF_WM8978 = "wm8978"
CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.Required(CONF_ID): cv.declare_id(WM8978Component),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x21))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)


#def validate_mode(value):
#    if not (value[CONF_INPUT] or value[CONF_OUTPUT]):
#        raise cv.Invalid("Mode must be either input or output")
#    if value[CONF_INPUT] and value[CONF_OUTPUT]:
#        raise cv.Invalid("Mode must be either input or output")
#    return value


#WM8978_PIN_SCHEMA = pins.gpio_base_schema(
#    WM8978GPIOPin,
#    cv.int_range(min=0, max=17),
#    modes=[CONF_INPUT, CONF_OUTPUT],
#    mode_validator=validate_mode,
#    invertable=True,
#).extend(
#    {
#        cv.Required(CONF_WM8978): cv.use_id(WM8978Component),
#    }
#)


#@pins.PIN_SCHEMA_REGISTRY.register(CONF_WM8978, WM8978_PIN_SCHEMA)
#async def wm8978_pin_to_code(config):
#    var = cg.new_Pvariable(config[CONF_ID])
#    parent = await cg.get_variable(config[CONF_WM8978])
#
#    cg.add(var.set_parent(parent))
#
#    num = config[CONF_NUMBER]
#    cg.add(var.set_pin(num))
#    cg.add(var.set_inverted(config[CONF_INVERTED]))
#    cg.add(var.set_flags(pins.gpio_flags_expr(config[CONF_MODE])))
#    return var
