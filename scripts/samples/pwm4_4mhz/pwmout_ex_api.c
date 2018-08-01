#include "pwmout_api.h"

#if DEVICE_PWMOUT

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"


static TIM_HandleTypeDef TimHandle;

static void pwmout_period_ns (pwmout_t* obj, int pres);
static void pwmout_write_ex (pwmout_t* obj, float value);

char HSE_SystemClock_Config_72Mhz(void) 
{ // STM32F103RB
    RCC_OscInitTypeDef RCC_OscInitStruct;
    
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;        // RCC_HSE_BYPASS
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return 0;
    }
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        return 0;
    }
    RCC_PeriphCLKInitTypeDef PeriphClkInit;
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInit.UsbClockSelection = RCC_USBPLLCLK_DIV1_5;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        return 0;
    }
    return 1;
}

void pwmout_init_ex(pwmout_t* obj, PinName pin, uint32_t freq)
{
    HAL_RCC_DeInit();
    if (!HSE_SystemClock_Config_72Mhz()) {
        error("PWM error: switch to 72Mhz failed.");;
    }
    
    // Update the SystemCoreClock variable
    SystemCoreClockUpdate();

    if (freq > SystemCoreClock) {
        error("PWM error: clock is too high.");
    }
    
    if ( (SystemCoreClock / 2) % freq) {
        error("PWM error: unsupported clock.");
    }
    
    // Get the peripheral name from the pin and assign it to the object
    obj->pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);

    if (obj->pwm == (PWMName)NC) {
        error("PWM error: pinout mapping failed.");
    }

    // Enable TIM clock
    if (obj->pwm == PWM_1) __TIM1_CLK_ENABLE();
    if (obj->pwm == PWM_2) __TIM2_CLK_ENABLE();
    if (obj->pwm == PWM_3) __TIM3_CLK_ENABLE();

    // Configure GPIO
    pinmap_pinout(pin, PinMap_PWM);

    obj->pin = pin;
    obj->period = 0;
    obj->pulse = 0;

    pwmout_period_ns(obj, (SystemCoreClock / 2) / freq); // 20 ms per default
}

void pwmout_period_ns (pwmout_t* obj, int pres)
{
    TimHandle.Instance = (TIM_TypeDef *)(obj->pwm);
    int ns = 2;
    float dc = 0.5; // pwmout_read(obj);

    __HAL_TIM_DISABLE(&TimHandle);

    TimHandle.Init.Period        = ns - 1;
    TimHandle.Init.Prescaler     = (uint16_t)(pres - 1); // (SystemCoreClock / 1000000) - 1; // 1 ns tick
    TimHandle.Init.ClockDivision = 0;
    TimHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;
    HAL_TIM_PWM_Init(&TimHandle);

    // Save for future use
    obj->period = ns;
    
    // Set duty cycle again
    pwmout_write_ex (obj, dc);

    __HAL_TIM_ENABLE(&TimHandle);
}

void pwmout_free_ex(pwmout_t* obj)
{
    // Configure GPIO
    pin_function(obj->pin, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
}

void pwmout_write_ex (pwmout_t* obj, float value)
{
    TIM_OC_InitTypeDef sConfig;
    int channel = 0;
    int complementary_channel = 0;

    TimHandle.Instance = (TIM_TypeDef *)(obj->pwm);

    if (value < (float)0.0) {
        value = 0.0;
    } else if (value > (float)1.0) {
        value = 1.0;
    }

    obj->pulse = (uint32_t)((float)obj->period * value);

    // Configure channels
    sConfig.OCMode       = TIM_OCMODE_PWM1;
    sConfig.Pulse        = obj->pulse;
    sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
    sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
    sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    switch (obj->pin) {

        // Channels 1
        case PA_6:
        case PA_8:
        case PA_15:
        case PB_4:
        case PC_6:
            channel = TIM_CHANNEL_1;
            break;

        // Channels 1N
        case PB_13:
            channel = TIM_CHANNEL_1;
            complementary_channel = 1;
            break;

        // Channels 2
        case PA_1:
        case PA_7:
        case PA_9:
        case PB_3:
        case PB_5:
        case PC_7:
            channel = TIM_CHANNEL_2;
            break;

        // Channels 2N
        case PB_14:
            channel = TIM_CHANNEL_2;
            complementary_channel = 1;
            break;

        // Channels 3
        case PA_2:
        case PA_10:
        case PB_0:
        case PB_10:
        case PC_8:
            channel = TIM_CHANNEL_3;
            break;

        // Channels 3N
        case PB_15:
            channel = TIM_CHANNEL_3;
            complementary_channel = 1;
            break;

        // Channels 4
        case PA_3:
        case PA_11:
        case PB_1:
        case PB_11:
        case PC_9:
            channel = TIM_CHANNEL_4;
            break;

        default:
            return;
    }

    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, channel);

    if (complementary_channel) {
        HAL_TIMEx_PWMN_Start(&TimHandle, channel);
    } else {
        HAL_TIM_PWM_Start(&TimHandle, channel);
    }
}


#endif // DEVICE_PWMOUT