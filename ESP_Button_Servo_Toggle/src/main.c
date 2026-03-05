#include <stdbool.h>
#include <stdint.h>

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SERVO_GPIO 23
#define BUTTON_SIG_GPIO 26
#define BUTTON_NC_GPIO 25

#define SERVO_FREQ_HZ 50
#define SERVO_MIN_PULSE_US 500
#define SERVO_MAX_PULSE_US 2500
#define SERVO_PERIOD_US 20000

#define SERVO_TIMER LEDC_TIMER_0
#define SERVO_MODE LEDC_LOW_SPEED_MODE
#define SERVO_CHANNEL LEDC_CHANNEL_0
#define SERVO_DUTY_RES LEDC_TIMER_14_BIT

#define SERVO_CLOSED_ANGLE 0
#define SERVO_OPEN_ANGLE 90
#define BUTTON_DEBOUNCE_MS 40
#define BUTTON_LOCKOUT_MS 250
#define BUTTON_PRESSED_LEVEL 1

static const char *TAG = "button_servo_toggle";

static inline bool is_button_pressed(void)
{
    return gpio_get_level(BUTTON_SIG_GPIO) == BUTTON_PRESSED_LEVEL;
}

static uint32_t angle_to_pulse_us(uint32_t angle_deg)
{
    if (angle_deg > 180) {
        angle_deg = 180;
    }

    return SERVO_MIN_PULSE_US +
           ((SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US) * angle_deg) / 180;
}

static uint32_t pulse_us_to_duty(uint32_t pulse_us)
{
    const uint32_t max_duty = (1U << SERVO_DUTY_RES) - 1U;
    return (pulse_us * max_duty) / SERVO_PERIOD_US;
}

static esp_err_t servo_set_angle(uint32_t angle_deg)
{
    uint32_t pulse_us = angle_to_pulse_us(angle_deg);
    uint32_t duty = pulse_us_to_duty(pulse_us);

    ESP_ERROR_CHECK(ledc_set_duty(SERVO_MODE, SERVO_CHANNEL, duty));
    ESP_ERROR_CHECK(ledc_update_duty(SERVO_MODE, SERVO_CHANNEL));

    ESP_LOGI(TAG, "Servo angle: %lu deg (pulse: %lu us)", angle_deg, pulse_us);
    return ESP_OK;
}

static void init_servo(void)
{
    ledc_timer_config_t timer_cfg = {
        .speed_mode = SERVO_MODE,
        .duty_resolution = SERVO_DUTY_RES,
        .timer_num = SERVO_TIMER,
        .freq_hz = SERVO_FREQ_HZ,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_cfg));

    ledc_channel_config_t channel_cfg = {
        .gpio_num = SERVO_GPIO,
        .speed_mode = SERVO_MODE,
        .channel = SERVO_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = SERVO_TIMER,
        .duty = 0,
        .hpoint = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel_cfg));
}

static void init_button(void)
{
    gpio_config_t button_nc_cfg = {
        .pin_bit_mask = 1ULL << BUTTON_NC_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&button_nc_cfg));
    ESP_ERROR_CHECK(gpio_set_level(BUTTON_NC_GPIO, 0));

    gpio_config_t button_sig_cfg = {
        .pin_bit_mask = 1ULL << BUTTON_SIG_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&button_sig_cfg));
}

void app_main(void)
{
    bool servo_open = false;
    bool raw_state = false;
    bool stable_state = false;
    TickType_t last_state_change_tick = 0;
    TickType_t last_toggle_tick = 0;

    init_servo();
    init_button();

    ESP_ERROR_CHECK(servo_set_angle(SERVO_CLOSED_ANGLE));

    ESP_LOGI(
        TAG,
        "Button SIG GPIO %d, NC GPIO %d (NC mode), Servo GPIO %d",
        BUTTON_SIG_GPIO,
        BUTTON_NC_GPIO,
        SERVO_GPIO
    );

    raw_state = is_button_pressed();
    stable_state = raw_state;
    last_state_change_tick = xTaskGetTickCount();

    while (1) {
        TickType_t now_tick = xTaskGetTickCount();
        bool sampled_state = is_button_pressed();

        if (sampled_state != raw_state) {
            raw_state = sampled_state;
            last_state_change_tick = now_tick;
        }

        if ((now_tick - last_state_change_tick) >= pdMS_TO_TICKS(BUTTON_DEBOUNCE_MS) &&
            stable_state != raw_state) {
            stable_state = raw_state;

            if (stable_state &&
                (now_tick - last_toggle_tick) >= pdMS_TO_TICKS(BUTTON_LOCKOUT_MS)) {
                servo_open = !servo_open;
                last_toggle_tick = now_tick;
                ESP_ERROR_CHECK(
                    servo_set_angle(servo_open ? SERVO_OPEN_ANGLE : SERVO_CLOSED_ANGLE)
                );
                }
        }

        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
