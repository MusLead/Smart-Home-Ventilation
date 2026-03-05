#include "iot_servo.h"
#include <stdbool.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define SERVO_GPIO 23

#define BUTTON_SIG_GPIO 26
#define BUTTON_NC_GPIO 25

static const char *TAG = "Servo_Task";

bool door_state = false;

// Servo Initialization
esp_err_t servo_init(void)
{
    servo_config_t servo_cfg = {
        .max_angle = 180,
        .min_width_us = 500,
        .max_width_us = 2500,
        .freq = 50,
        .timer_number = LEDC_TIMER_0,
        .channels = {
            .servo_pin = {SERVO_GPIO},
            .ch = {LEDC_CHANNEL_0},
        },
        .channel_number = 1,
    };

    esp_err_t res = iot_servo_init(LEDC_LOW_SPEED_MODE, &servo_cfg);
    if (res != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize servo: %s", esp_err_to_name(res));
        return res;
    }

    ESP_LOGI(TAG, "Servo initialized successfully on GPIO %d", SERVO_GPIO);
    return ESP_OK;
}

// Button Initialization (Pins vom Kollegen)
void button_init(void)
{
    gpio_config_t button_nc_cfg = {
        .pin_bit_mask = 1ULL << BUTTON_NC_GPIO,
        .mode = GPIO_MODE_OUTPUT,
    };
    gpio_config(&button_nc_cfg);
    gpio_set_level(BUTTON_NC_GPIO, 0);

    gpio_config_t button_sig_cfg = {
        .pin_bit_mask = 1ULL << BUTTON_SIG_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };
    gpio_config(&button_sig_cfg);
}

// Servo Control Task
void servo_start_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Starting servo task");

    bool last_button = 1;

    while (1)
    {
        bool button = gpio_get_level(BUTTON_SIG_GPIO);

        if (button == 0 && last_button == 1) // button pressed
        {
            door_state = !door_state;
        }

        last_button = button;

        iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, door_state ? 90 : 0);

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void app_main(void)
{
    servo_init();
    button_init();

    xTaskCreate(servo_start_task, "servo_task", 2048, NULL, 5, NULL);
}