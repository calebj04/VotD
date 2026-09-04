#include <Arduino.h>

struct tm setClock()
{
    configTzTime("EST5EDT,M3.2.0/2,M11.1.0/2", "pool.ntp.org");

    struct tm timeinfo;

    while (!getLocalTime(&timeinfo))
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Time synchronized!");

    return timeinfo;
}

long sleepTime(struct tm currentTime)
{
    int currentSeconds =
        currentTime.tm_hour * 3600 +
        currentTime.tm_min * 60 +
        currentTime.tm_sec;

    int targetSeconds = 6 * 3600 + 30 * 60;

    return targetSeconds - currentSeconds;
}
