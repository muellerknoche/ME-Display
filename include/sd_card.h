#ifndef SDCARD_H
#define SDCARD_H


#include <Arduino.h>
#include <WiFi.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <config.h>

// === First get Config from SD Card ====

// Function to trim whitespace from strings
String trim(String str)
{
  	str.trim();
  	return str;
}
// Function to read and parse config from SD
bool loadConfigFromSD()
{
	if (!SD.begin(csPin))
	{
    	Serial.println("SD card mount failed");
    	return false;
  	}
  	Serial.println("SD card mounted");


  File file = SD.open("/config.txt");
  if (!file)
	 {
		Serial.println("Failed to open /config.txt");
    	return false;
  	}

  	while (file.available())
	{
    	String line = file.readStringUntil('\n');

		Serial.println(line);

    	line = trim(line);
    	if (line.length() == 0 || line.startsWith("#")) continue;  			// Skip empty or comments

    	int eqIndex = line.indexOf('=');
    	if (eqIndex == -1) continue;  // Invalid line

    	String key = trim(line.substring(0, eqIndex));
    	String value = trim(line.substring(eqIndex + 1));

	if (key == "ssid") ssid = value;
    else if (key == "password") password = value;
    else if (key == "ip") localIP.fromString(value);
    else if (key == "gateway") gateway.fromString(value);
    else if (key == "subnet") subnet.fromString(value);
    else if (key == "pin") pin = value;
    else if (key == "laenge") laenge = value;
    }
  	file.close();

  	// Validate if all were loaded
  	if (pin.isEmpty() || laenge.isEmpty() || ssid.isEmpty() || password.isEmpty() || localIP == IPAddress(0,0,0,0) ||
      gateway == IPAddress(0,0,0,0) || subnet == IPAddress(0,0,0,0)) 
	{
    	Serial.println("Incomplete config, using defaults");
    	return false;
  	}
		
		pin_len = laenge[0] - '0';

		for (int i= 0; i < pinlen)
		{
			reference_code[i] = pin[i] - '0';
			Serial.print(reference_code[i]);
		}

  	Serial.println("Config loaded: SSID=" + ssid + ", IP=" + localIP.toString());
  	Serial.println(pin);
  	Serial.println(laenge);
  	return true;
}

// === SD Card END ===

#endif // SDCARD_H