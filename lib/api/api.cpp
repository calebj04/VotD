#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "certificates.h"
#include "books.h"
#include "secrets.h"

String getReference(WiFiClientSecure &client, int day)
{
    client.setCACert(youVersionRootCA);

    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");
    if (https.begin(client, "https://api.youversion.com/v1/verse_of_the_days/" + String(day)))
    {
        https.addHeader("x-yvp-app-key", youVersionKey);

        Serial.print("[HTTPS] GET...\n");
        int httpCode = https.GET();

        if (httpCode > 0)
        {
            Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK)
            {
                String payload = https.getString();

                int start = payload.indexOf("\"passage_id\":\"") + 14;
                int end = payload.indexOf("\"", start);

                String reference = payload.substring(start, end);

                https.end();
                return unabbreviate(reference);
            }
            https.end();
            return "";
        }
        else
        {
            Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
            https.end();
            return "";
        }
    }
    else
    {
        Serial.printf("[HTTPS] Unable to connect\n");
        return "";
    }
}

String getVerse(int day)
{
    WiFiClientSecure client;

    String reference = getReference(client, day);

    client.setCACert(esvRootCA);

    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");
    if (https.begin(client, "https://api.esv.org/v3/passage/text/?q=" + reference))
    {
        https.addHeader("Authorization", esvKey);

        Serial.print("[HTTPS] GET...\n");
        int httpCode = https.GET();

        if (httpCode > 0)
        {
            Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK)
            {
                String response = https.getString();

                int passageStart = response.indexOf("\"passages\"");
                int quoteStart = response.indexOf("\"", passageStart + 11);
                int quoteEnd = response.indexOf("\"", quoteStart + 1);

                String passage = response.substring(quoteStart + 1, quoteEnd);

                int verseStart = passage.indexOf("[");
                int verseEnd = passage.lastIndexOf("(");

                String verse = passage.substring(verseStart, verseEnd);
                verse.trim();

                https.end();
                return verse;
            }
            https.end();
            return "";
        }
        else
        {
            Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
            https.end();
            return "";
        }
    }
    else
    {
        Serial.printf("[HTTPS] Unable to connect\n");
        return "";
    }
}
