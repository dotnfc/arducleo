
NUCLEO_F103RB

    supported_form_factors: ["ARDUINO", "MORPHO"],
    core: "Cortex-M3",
    default_toolchain: "uARM",
    extra_labels: ["STM", "STM32F1", "STM32F103RB"],
    supported_toolchains: ["ARM", "uARM", "GCC_ARM", "IAR"],
    detect_code: ["0700"],
    device_has:  ["ANALOGIN", "CAN", "I2C", "I2CSLAVE", "INTERRUPTIN", "PORTIN", 
                  "PORTINOUT", "PORTOUT", "PWMOUT", "RTC", "SERIAL", "SLEEP", 
                  "SPI", "SPISLAVE", "STDIO_MESSAGES"],
    default_build: "small",

        