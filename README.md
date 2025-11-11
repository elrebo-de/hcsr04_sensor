# HCSR04 ultrasonic sensor component

This repository contains an ESP-IDF component for the HCSR04 ultrasonic module with 2 bit data interface.This
module contains an ultrasonic transmitter and receiver and can measure the distance. It runs on 
any ESP32 processor and is built using the ESP-IDF build system in version 6.0+.

The component is implemented as C++ class `Hcsr04Sensor`.

## Connecting the component

The constructor of class `Hcsr04Sensor` has four parameters:

| Parameter   | Type of Parameter | Usage                                          |
|:------------|:------------------|:-----------------------------------------------|
| tag         | std::string       | the tag to be used in the ESP log |
| triggerPin  | gpio_num_t        | the gpio number of the pin used to trigger the measurement |
| echoPin     | gpio_num_t        | the gpio number of the pin used to read the result of the measurement |
| maxDistance | float             | the maximum distance (in cm) to be measured by this sensor |

The component uses 2 GPIO pins of the SoC.

## hcsr04 library

The component only supports the HCSR04 module with a 2 bits control interface. It does not support the HCSR04 module 
with an I2C interface.

# Usage

## API
The API of the component is located in the include directory `include/hcsr04_sensor.hpp` and defines the 
C++ class `Hcsr04Sensor`

```C++
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
    gpio_num_t echoPin;    // the pin number for the echo pin
    float maxDistance;  // the maximum distance to be measured
};
```
To measure the distance the public method `GetDistance` is called. It returns the measured distance as a `float`.
In case of errors during measurement `distance = 0` is returned.

The class does not use interrupt handlers.

The public `GetDistance` method blocks during measurement of the distance. Depending on the measured distance 
the call blocks between 327 usec (2cm) and max. 6 msec (> 100cm).

**Measurement algorithm:**

- Generate start pulse of 10 usec on triggerPin
- Module generates 8 pulses of 40kHZ (approx 200usec)
- Measure echo pulse width. Distance (cm) = T-echopulse width (usec) * 0,034/2

# License

This component is provided under MIT license, see [LICENSE](LICENSE) file for details.

The work is based on the github repository vgerwen/hcsr04
