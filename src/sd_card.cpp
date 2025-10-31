#include <Arduino.h>
#include <sd_card.h>

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

//	Serial.begin(115200);			// Start Serial
//	while(!Serial){delay(100);}
	if (!SD.begin(csPin))										// is SD card accessable
	{
    	Serial.println("SD card mount failed");
    	return false;
  	}
  	Serial.println("SD card mounted");							// OK i

  	File file = SD.open("/config.txt");							// try open config file
  	if (!file)
	{
		Serial.println("Failed to open /config.txt");			// failed to open
		return false;
	}

  	while (file.available())
	{
    	String line = file.readStringUntil('\n');				// read one line

		Serial.println(line);									// and print

    	line = trim(line);										// remove white space front and end
    	if (line.length() == 0 || line.startsWith("#")) continue;  		// Skip empty or comments

    	int eqIndex = line.indexOf('=');						// find pos of '='
    	if (eqIndex == -1) continue;  							// Invalid line

    	String key = trim(line.substring(0, eqIndex));			// read key (befor =), store string in  key
    	String value = trim(line.substring(eqIndex + 1));		// read value (after =), store in vaue
	
	// dependend of key store to final variable
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
    	Serial.println("Incomplete config, using defaults");	// somethings missing
    	return false;
  	}
	
	// convert 'laenge' and 'pin' 
	pin_len = laenge[0] - '0';

	for (int i= 0; i < pin_len;i++)
	{
		reference_code[i] = pin[i] - '0';
		Serial.print(reference_code[i]);
	}
	Serial.println();

	#ifdef DEBUG
	  	Serial.println("Config loaded: SSID=" + ssid + ", IP=" + localIP.toString());
  		Serial.println(pin);
  		Serial.println(laenge);
	#endif
	return true;
	
}

// === SD Card END ===

