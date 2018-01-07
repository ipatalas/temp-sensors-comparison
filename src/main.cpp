#include <Arduino.h>

#define PIN_D0 16
#define PIN_D1 5
#define PIN_D2 4
#define PIN_D3 0
#define PIN_D4 2
#define PIN_D5 14
#define PIN_D6 12
#define PIN_D7 13
#define PIN_D8 15
#define PIN_D9 3
#define PIN_D10 1

#define PIN_MOSI 8
#define PIN_MISO 7
#define PIN_SCLK 6
#define PIN_HWCS 0

#define PIN_D11 9
#define PIN_D12 10

#define DHT11_PIN PIN_D3

#define ONE_WIRE_BUS PIN_D2

#include <OneWire.h>
#include <DallasTemperature.h>

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

// int count = 0;

#include <dht.h>

dht DHT;

void printAddress(DeviceAddress deviceAddress)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		if (deviceAddress[i] < 16)
			Serial.print("0");
		Serial.print(deviceAddress[i], HEX);
	}
}

void setup()
{
	Serial.begin(115200);
	Serial.println("DHT TEST PROGRAM ");
	Serial.print("LIBRARY VERSION: ");
	Serial.println(DHT_LIB_VERSION);
	Serial.println();

	Serial.print("Locating devices...");
	sensors.begin();
	Serial.print("Found ");
	Serial.print(sensors.getDeviceCount(), DEC);
	Serial.println(" devices.");

	// report parasite power requirements
	Serial.print("Parasite power is: ");
	if (sensors.isParasitePowerMode())
		Serial.println("ON");
	else
		Serial.println("OFF");

	if (!sensors.getAddress(insideThermometer, 0))
		Serial.println("Unable to find address for Device 0");

	// show the addresses we found on the bus
	Serial.print("Device 0 Address: ");
	printAddress(insideThermometer);
	Serial.println();

	// set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
	sensors.setResolution(insideThermometer, 12);

	Serial.print("Device 0 Resolution: ");
	Serial.print(sensors.getResolution(insideThermometer), DEC);
	Serial.println();

	delay(2000);

	Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
}

float LM35DZ_read(int pin)
{
	int odczyt = analogRead(pin);			  //odczytanie wartości z czujnika
	float voltage = (odczyt * 3.33) / 1024.0; //przeliczenie odczytanej wartości na napięcie w woltach (dla podłączenia pod 5 V)
	return voltage * 100; //konwersja z napięcia na temperaturę, rozdzielczość czujnika wynosi 10 mV na stopień
}

float DS18B20_read()
{
	sensors.requestTemperatures();
	return sensors.getTempC(insideThermometer);
}

void loop()
{
	// READ DATA
	int chk = DHT.read11(DHT11_PIN);
	float LM35DZ = LM35DZ_read(A0);
	float DS18B20 = DS18B20_read();

	Serial.print("DHT11, \t");

	switch (chk)
	{
	case DHTLIB_OK:
		Serial.print("OK,\t");
		break;
	case DHTLIB_ERROR_CHECKSUM:
		Serial.print("Checksum error,\t");
		break;
	case DHTLIB_ERROR_TIMEOUT:
		Serial.print("Time out error,\t");
		break;
	default:
		Serial.print("Unknown error,\t");
		break;
	}
	// DISPLAY DATA
	Serial.print(DHT.humidity, 1);
	Serial.print(",\t");
	Serial.print(DHT.temperature, 1);
	Serial.print(",\tDS18B20: ");
	Serial.print(DS18B20, 2);
	Serial.print(",\tLM35DZ: ");
	Serial.println(LM35DZ, 2);

	delay(2000);
}
