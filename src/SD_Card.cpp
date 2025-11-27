// SD CARD START
/**
 * @author someone on the net, Rainer Müller-Knoche mk@muekno.de
 * @brief removes white space before and after a string
 * @date 26.11.2025
 * @date 27.11.2025 mk wieder augelagert
 */
 #include <Arduino.h>
 #include <SD.h>
 #include <FS.h>
 #include <SPI.h>
#include <my_globals.h>

String trim(String str)
{
  	str.trim();
  	return str;
}
/**
 * @fn bool loadConfigFromSD()
 * @author someone on the net, Rainer Müller-Knoche mk@muekno.de
 * @brief reads configuration from SD Card
 * @brief SSID, Passwort, Websockserver IPAddress, PIN, Pinlength
 * @date 26.11.2025
 * @date 27.11 2025 mk wieder ausgelagert
 */
bool loadConfigFromSD()
{
	if (!SD.begin(csPin))										// is SD card accessable
	{
    	Serial.println("SD card mount failed");
    	return false;
  	}
  	Serial.println("SD card mounted");							// OK 
	File myfile = SD.open("/config.txt");
  	if (!myfile)
	{
		Serial.println("Failed to open /config.txt");			// failed to open
		return false;
	}
	Serial.println("read SD Card now");
  	while (myfile.available())
	{
    	String line = myfile.readStringUntil('\n');				// read one line
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
		else if (key == "ip")	localIP.fromString(value);
		else if (key == "gateway") gateway = value;				// gateway has the  same value
																// as Websocket Server IP Addree
																// we use it for wsHost IP address
		else if (key == "subnet") subnet.fromString(value);
		else if (key == "pin") pin = value;
		else if (key == "laenge") laenge = value;;
    }
  	myfile.close();
	Serial.println("got all closed file");
	#ifdef DEBUG
		Serial.println("SD Card readings");
		Serial.print("D_ssid: ");Serial.println(ssid);
		Serial.print("D_pasword: ");Serial.println(password);
		Serial.print("D_localIP: ");Serial.println(localIP);
		Serial.print("D_gateway: ");Serial.println(gateway);		// wsHost
		Serial.print("D_subnet: ");Serial.println(subnet);
		Serial.print("D_pin: ");Serial.println(pin);
		Serial.print("D_laenge: ");Serial.println(laenge);
	#endif
    return true;
} // END loadConfigFRomSD
// SD CARD END
