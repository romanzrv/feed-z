# Feed-Z: An automated feeding system

System for auto-feed your fish made with arduino

## Components used:

  - **Arduino Nano** Arduino Nano (ATMega328P)
  - **5V Servo** motor 
  - **DS3231** Real time clock module
  - **LM7805** Voltage regulator
  - **5V DC** Water pump
  
## Setting feed schedule

To define the feed frequency set the following values:

```
FEED_HOUR 21 // Irrigation hour
FEED_MINUTE 48 // Irrigation minute
FEED_SECOND 0 // Irrigation second
FEED_TIME_MILLISECONDS 5000 // Irrigation duration

...

int daysOfWeekToFeed[7] = {1, 2, 3, 4, 5, 6, 7}; // Days of week when feed process should be triggered
```
