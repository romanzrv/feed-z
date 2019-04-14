/*
  WRITTEN BY: Roman Zukov.
  PROJECT: Feed-Z
  DESCRIPTION: An automated fish feeding system for plants. Components used:
  
  - Arduino nano (ATMega328P)
  - 5V Servo
  - DS3231 real time clock module
  - LM7805 voltage regulator
*/

#include <Wire.h>
#include <Servo.h>
#include <LowPower.h>
#include <RTClibExtended.h>

/**
 * Real time clock module DS3231
 */
RTC_DS3231 rtc;

/**
 * Servo motor object
 */
Servo feedServo;

/**
 * Relay and wakeup pins
 */
#define WAKE_PIN 2

/** 
 * Irrigate frequency schedule
 */
#define FEED_HOUR 19
#define FEED_MINUTE 30
#define FEED_SECOND 0
#define FEED_TIME_MILLISECONDS 5000

/**
 * Days of week to irrigate
 */
int daysOfWeekToFeed[7] = {1, 2, 3, 4, 5, 6, 7};

void setup () {
  setPinModes();
  serialStart();
  checkRtcConnected();
  checkRtcPowerLost();
}

void loop () {
  attachSleepInterrupt(); 
  clearPendingAlarms();
  sqwPinOff();
  setAlarm();
  delay(1000);
  goToSleep();
  checkIfFeedNeeded();
  detachSleepInterrupt();
}

/**
 * Chek if current date/time is matching the schedule
 */
void checkIfFeedNeeded() {
  DateTime nowRtc = rtc.now();
  int arrayElements = (sizeof(daysOfWeekToFeed)/sizeof(daysOfWeekToFeed[0]));
  bool dayMatchs = false;

  for (int i = 0; i < arrayElements; i++) {
    if (daysOfWeekToFeed[i] == nowRtc.dayOfTheWeek()) {
      dayMatchs = true;
    }
  }

  if (dayMatchs) {
    startFeedProcess();
  }
}

/**
 * Setup wakeup and relay pin modes
 */
void setPinModes() {
  pinMode(WAKE_PIN, INPUT_PULLUP);
}

/**
 * Start feeding process
 */
void startFeedProcess() {
  feedServo.attach(4);
  feedServo.write(180);
  delay(200);
  feedServo.write(20);
  feedServo.detach();
}

/**
 * Check if RTC module is connected
 */
void checkRtcConnected() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
}

/**
 * Check if RTC power has been lost
 */
void checkRtcPowerLost() {
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

/**
 * Start serial output and wait for console opening
 */
void serialStart() {
  Serial.begin(9600);
  delay(1000);  
}

/**
 * Atatch interruption for sleep mode
 */
void attachSleepInterrupt() {
  attachInterrupt(digitalPinToInterrupt(WAKE_PIN), onWakeUpAlarm, FALLING);
}

/**
 * Detach interruption for sleep mode
 */
void detachSleepInterrupt() {
  detachInterrupt(0);
}

/**
 * Clear any pending alarms
 */
void clearPendingAlarms() {
  rtc.armAlarm(1, false);
  rtc.clearAlarm(1);
  rtc.alarmInterrupt(1, false);
  rtc.armAlarm(2, false);
  rtc.clearAlarm(2);
  rtc.alarmInterrupt(2, false);
}

/**
 * Set the SQW pin off
 */
void sqwPinOff() {
   rtc.writeSqwPinMode(DS3231_OFF);
}

/**
 * Set an alarm for DS3231
 * Format minutes:hours:second
 */
void setAlarm() {
  rtc.setAlarm(ALM1_MATCH_HOURS, FEED_MINUTE, FEED_HOUR, FEED_SECOND);
  rtc.alarmInterrupt(1, true);
}

/**
 * Arduino enters into sleep mode here
 */
void goToSleep() {
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
}

/**
 * Function that is executed when arduino wakes up by RTC
 */
void onWakeUpAlarm() {

}
