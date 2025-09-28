#include "TimeManager.h"
#include <Arduino.h>

//constructor
TimeManager::TimeManager(byte setSeconds, byte setMinutes, byte setHours) : seconds(setSeconds), minutes(setMinutes), hours(setHours) {}

byte* TimeManager::getSeconds() {
  return &seconds;
}
byte* TimeManager::getHours() {
  return &hours;
}
byte* TimeManager::getMinutes() {
  return &minutes;
}

void TimeManager::setSeconds(byte setSeconds) {
  seconds = setSeconds;
}
void TimeManager::setMinutes(byte setMinutes) {
  minutes = setMinutes;
}
void TimeManager::setHours(byte setHours) {
  hours = setHours;
}

// // een seconde toevoegen aan de tijd
void TimeManager::addSecond() {
  seconds++;
  if (seconds >= 60) {
    seconds = 0;
    minutes++;
    if (minutes >= 60) {
      minutes = 0;
      hours++;
      if (hours >= 24) {
        hours = 0;
      }
    }
  }
}