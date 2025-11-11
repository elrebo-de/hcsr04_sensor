## Ultrasonic Distance Example

This example demonstrates how to utilize the `hcsr04_sensor` component to measure distances with HCSR04 ultrasonic module.

## Hardware

* Any two GPIOs on any development board can be used in this example.

## Build and Flash

Build the project and flash it to the board, then run the monitor tool to view the serial output:

* Run `. <directory with ESP-IDF version>/export.sh` to set IDF environment
* Run `idf.py set-target esp32xx` to set target chip
* Run `idf.py -p PORT flash monitor` to build, flash and monitor the project

(To exit the serial monitor, type `Ctrl-]` (or on a Mac `Ctrl-Option-6`).

See the Getting Started Guide for all the steps to configure and use the ESP-IDF to build projects.

## Example Output

```
I (264) main_task: Calling app_main()
================IO DUMP Start================
IO[20] -
  Pullup: 0, Pulldown: 1, DriveCap: 2
  InputEn: 0, OutputEn: 1, OpenDrain: 0
  FuncSel: 1 (GPIO)
  GPIO Matrix SigOut ID: 128 (simple GPIO output)
  SleepSelEn: 1

IO[21] -
  Pullup: 0, Pulldown: 1, DriveCap: 2
  InputEn: 1, OutputEn: 0, OpenDrain: 0
  FuncSel: 1 (GPIO)
  GPIO Matrix SigOut ID: 128 (simple GPIO output)
  GPIO Matrix SigIn ID: (simple GPIO input)
  SleepSelEn: 1

=================IO DUMP End=================
I (304) Water Level: GetDistance called
I (304) Water Level: time_us = 911
I (304) Water Level: distance =  15.6
I (314) ultrasonic_distance: The distance is  15.6 cm.
I (1314) Water Level: GetDistance called
I (1314) Water Level: time_us = 1173
I (1314) Water Level: distance =  20.1
I (1314) ultrasonic_distance: The distance is  20.1 cm.
I (2314) Water Level: GetDistance called
I (2314) Water Level: time_us = 1266
I (2314) Water Level: distance =  21.7
I (2314) ultrasonic_distance: The distance is  21.7 cm.
I (3314) Water Level: GetDistance called
I (3314) Water Level: time_us = 892
I (3314) Water Level: distance =  15.3
I (3314) ultrasonic_distance: The distance is  15.3 cm.
I (4314) Water Level: GetDistance called
I (4314) Water Level: time_us = 1286
I (4314) Water Level: distance =  22.1
I (4314) ultrasonic_distance: The distance is  22.1 cm.
I (5314) Water Level: GetDistance called
I (5314) Water Level: time_us = 5831
I (5314) Water Level: distance = 100.0
I (5314) Water Level: ret = ESP_ERR_ULTRASONIC_ECHO_TIMEOUT
I (5314) ultrasonic_distance: The distance is   0.0 cm.
I (6324) Water Level: GetDistance called
I (6324) Water Level: time_us = 853
I (6324) Water Level: distance =  14.6
I (6324) ultrasonic_distance: The distance is  14.6 cm.
I (7324) Water Level: GetDistance called
I (7324) Water Level: time_us = 1173
I (7324) Water Level: distance =  20.1
I (7324) ultrasonic_distance: The distance is  20.1 cm.
I (8324) Water Level: GetDistance called
I (8324) Water Level: time_us = 969
I (8324) Water Level: distance =  16.6
I (8324) ultrasonic_distance: The distance is  16.6 cm.
I (9324) Water Level: GetDistance called
I (9324) Water Level: time_us = 986
I (9324) Water Level: distance =  16.9
I (9324) ultrasonic_distance: The distance is  16.9 cm.
I (10324) main_task: Returned from app_main()
```