#include "max6675.h"

int thermoDO = 4;
int thermoCS = 5;
int thermoCLK = 6;
int coilRelay = 7;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup()
{
    Serial.begin(9600);

    Serial.println("MAX6675 test");
    // wait for MAX chip to stabilize
    delay(500);
    pinMode(coilRelay, OUTPUT);
}

void loop()
{
    // basic readout test, just print the current temp

    Serial.print("C = ");
    Serial.print(thermocouple.readCelsius());
    if (thermocouple.readCelsius() > 50)
    {
        Serial.println("Relay on");
        digitalWrite(coilRelay, HIGH);
    }
    else if (thermocouple.readCelsius() < 50)
    {
        Serial.println("relay off");
        digitalWrite(coilRelay, LOW);
        delay(100);
        digitalWrite(coilRelay, HIGH);
    }

    // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
    delay(250);
}
