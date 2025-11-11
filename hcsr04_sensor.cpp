/*
 * hcsr04_sensor.cpp
 *
 * ESP-IDF component for ultrasonic distance measurement
 *
 * this work is based on Github project vgerwen/hcsr04
 * 
 * this work is licenced under the MIT licence like the original work vgerwen/hcsr04
 */

#include "hcsr04_sensor.hpp"
#include "freertos/FreeRTOS.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp_rom_sys.h"
#include "esp_log.h"

#define TRIGGER_LOW_DELAY 4
#define TRIGGER_HIGH_DELAY 10   // HC SR04 start pulse with 10 usec
#define PING_TIMEOUT 6000       // echo input high max 6 msec after trigger puls*/
#define ROUNDTRIP_CM 58.3       // this is 1 / speed of sound (0,0343 cm/us) * 2 (roundtrip)

static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

#define ESP_ERR_ULTRASONIC_PING         0x200
#define ESP_ERR_ULTRASONIC_PING2        0x203
#define ESP_ERR_ULTRASONIC_PING_TIMEOUT 0x201
#define ESP_ERR_ULTRASONIC_ECHO_TIMEOUT 0x202

Hcsr04Sensor::Hcsr04Sensor(std::string tag, gpio_num_t triggerPin, gpio_num_t echoPin, float maxDistance) {
	this->tag = tag;
    this->triggerPin = triggerPin;
    this->echoPin = echoPin;
    this->maxDistance = maxDistance;

    const gpio_config_t triggerPinConfig =
        {
           .pin_bit_mask = (1ULL << this->triggerPin),
           .mode = GPIO_MODE_OUTPUT,
           .pull_up_en = GPIO_PULLUP_DISABLE,
           .pull_down_en = GPIO_PULLDOWN_ENABLE,
           .intr_type = GPIO_INTR_DISABLE
        };
    gpio_config(&triggerPinConfig);

    const gpio_config_t echoPinConfig =
        {
           .pin_bit_mask = (1ULL << this->echoPin),
           .mode = GPIO_MODE_INPUT,
           .pull_up_en = GPIO_PULLUP_DISABLE,
           .pull_down_en = GPIO_PULLDOWN_ENABLE,
           .intr_type = GPIO_INTR_DISABLE
        };
    gpio_config(&echoPinConfig);

    gpio_dump_io_configuration(stdout, (1ULL << this->triggerPin) | (1ULL << this->echoPin));
}

Hcsr04Sensor::~Hcsr04Sensor() {
	// TODO Auto-generated destructor stub
}

float Hcsr04Sensor::GetDistance() {
    ESP_LOGI(this->tag.c_str(), "GetDistance called");

    float distance;

    esp_err_t ret = this->UltrasonicMeasure(this->maxDistance, &distance);

    if (ret != ESP_OK) {
       if (ret == ESP_ERR_INVALID_ARG) ESP_LOGI(this->tag.c_str(), "ret = ESP_ERR_INVALID_ARG");
       if (ret == ESP_ERR_ULTRASONIC_PING) ESP_LOGI(this->tag.c_str(), "ret = ESP_ERR_ULTRASONIC_PING");
       if (ret == ESP_ERR_ULTRASONIC_PING2) ESP_LOGI(this->tag.c_str(), "ret = ESP_ERR_ULTRASONIC_PING2");
       if (ret == ESP_ERR_ULTRASONIC_ECHO_TIMEOUT) ESP_LOGI(this->tag.c_str(), "ret = ESP_ERR_ULTRASONIC_ECHO_TIMEOUT");
       distance = 0;
    }
    return distance;
}

esp_err_t Hcsr04Sensor::UltrasonicMeasure(float maxDistance, float *distance)
{
    uint32_t    time_us;
    esp_err_t   return_value = ESP_OK;

    if (distance != NULL) {
        uint32_t max_distance = maxDistance * (float) ROUNDTRIP_CM;
        return_value = this->ultrasonic_measure_raw(max_distance, &time_us);
        ESP_LOGI(this->tag.c_str(), "time_us = %u", time_us);
        *distance = (float) time_us / (float) ROUNDTRIP_CM;
        ESP_LOGI(this->tag.c_str(), "distance = %5.1f", *distance);
    }
    else {
        return_value = ESP_ERR_INVALID_ARG;
    }
    return return_value;
}

esp_err_t Hcsr04Sensor::ultrasonic_measure_raw(uint32_t max_time_us, uint32_t *time_us)
{
    esp_err_t return_value = ESP_OK;
    int64_t   echo_start;
    int64_t   time;

    if (time_us != NULL) {
        taskENTER_CRITICAL(&mux);

        // Ping: Low for 4 us, then high 10 us
        gpio_set_level(this->triggerPin, 0);
        esp_rom_delay_us(TRIGGER_LOW_DELAY);
        gpio_set_level(this->triggerPin, 1);
        esp_rom_delay_us(TRIGGER_HIGH_DELAY);
        gpio_set_level(this->triggerPin, 0);

        // Previous ping isn't ended
        if (gpio_get_level(this->echoPin)) {
            return_value = ESP_ERR_ULTRASONIC_PING;
        }

        // Wait for echo
        echo_start = esp_timer_get_time();
        while (!gpio_get_level(this->echoPin) && (return_value == ESP_OK)) {
            time = esp_timer_get_time();
            if ( time - echo_start >= PING_TIMEOUT) {
                return_value = ESP_ERR_ULTRASONIC_PING2;
            }
        }

        // Echo detected wait for end of echo
        echo_start = esp_timer_get_time();
        time = echo_start;
        while ((gpio_get_level(this->echoPin)) && (return_value == ESP_OK)) {
            time = esp_timer_get_time();
            if (time - echo_start >= max_time_us) { 
                return_value = ESP_ERR_ULTRASONIC_ECHO_TIMEOUT;
            }
        }    
        taskEXIT_CRITICAL(&mux);
        *time_us = (uint32_t)(time - echo_start);
    }
    else {
        return_value = ESP_ERR_INVALID_ARG;  // Invalid argument
    }
    return return_value;
}
