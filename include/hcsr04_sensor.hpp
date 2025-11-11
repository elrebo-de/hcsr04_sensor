/*
 * hcsr04-sensor.hpp
 *
 * ESP-IDF component for ultrasonic distance measurement
 *
 * this work is based on Github project vgerwen/hcsr04
 *
 * this work is licenced under the MIT licence like the original work vgerwen/hcsr04
 */

#ifndef HCSR04_SENSOR_HPP_
#define HCSR04_SENSOR_HPP_

#include <stdio.h>
#include <cstdint>
#include <string>
#include "esp_err.h"
#include "driver/gpio.h"

class Hcsr04Sensor {
public:
	Hcsr04Sensor(std::string tag, gpio_num_t triggerPin, gpio_num_t echoPin, float maxDistance);
	virtual ~Hcsr04Sensor();
    float GetDistance();

private:
    esp_err_t UltrasonicMeasure(float maxDistance, float *distance);
    esp_err_t ultrasonic_measure_raw(uint32_t max_time_us, uint32_t *time_us);

    std::string tag = "Hcsr04Sensor";
	gpio_num_t triggerPin; // the pin number for the trigger pin
	gpio_num_t echoPin; // the pin number for the echo pin
	float maxDistance;
};

#endif /* HCSR04_SENSOR_HPP_ */
