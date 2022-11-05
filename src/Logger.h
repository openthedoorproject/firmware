#pragma once

#include <Arduino.h>

#define LOG_INIT(rate) Serial.begin(rate)
#define LOG_INFO(message) Serial.println(String("[") + millis() + " " + __FILE__ + ':' + __LINE__ + "] INFO: " + message)
#define LOG_WARN(message) Serial.println(String("[") + millis() + " " + __FILE__ + ':' + __LINE__ + "] WARN: " + message)
#define LOG_ERROR(message) Serial.println(String("[") + millis() + " " + __FILE__ + ':' + __LINE__ + "] ERROR: " + message)