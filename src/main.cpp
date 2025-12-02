/**
 * @brief Projekt Max-Exstein Video Spion
 * @authors Rainer Müller-Knoche, Elecrow
 * @note based on a Demo for the Cropanel HMI 5 inch 800x480 Display and the
 * ESP32-CAM_to_ESP32 Software found on the net streaming from CAM to TFT
 * @date startet in August 2025
 * @date now 20. Okt. 2025 nearly finished
 * @data 23. Oct, 2025 SD card rein timer Integration ??
 * @note neuer branch am 29.10.2025
 * @date weiter 30.10.2025 .h verstanden Umbau
 * @date 01.11.2025 Schnauze voll alles zurück nach main.cpp
 * @date 09.11.2025 Branch nach msaster
 * @date 12.11.2025 lv_conf.h + ports für Linux
 * Aktuelle Version 1.0.2
 * @date 21.11.2025 neuer Branch Display as WEbsockets Client
 * Neue Version 1.1.0
 * @date 23.11.25 sd Card zurück in Main 
 * @date 26.11.25 vor 8:00 merge back to master
 * @date 27.11.25 mk indicator for WiFi connect SD Card Stuff ausgelagert
 * @date 27.11.26 mk weitere Optik
 * @date 02.12.2025 mk Anpssungen an HW 3.0
 */

#include <Arduino.h>

#define DEBUG				// comment for final
#include <PCA9557.h>		// file copied from CowPanel github
#include "my_globals.h"		// ehemals main.h
#include <WiFi.h>       	// For WiFi 
#include <SD.h>         	// SD card library
#include <FS.h>
#include <SPI.h>        	// SPI for SD
#include <lvgl.h>
#include <keypad.h>
#include <SD_Card.h>		// SDCARD einlesen
#include <ArduinoWebsockets.h>

#include <LovyanGFX.hpp>
//#include <TAMC_GT911.h>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <ArduinoWebsockets.h>
#include "my_globals.h"

//!!!
//const char* websockets_server_host = "192.168.1.1"; //--> Use the IP address in the "local_ip" variable in the ESP32 TFT LCD (server) program code.
// Websocket server details
const uint16_t wsPort = 8888;					// Server port
const uint16_t websockets_server_port = 8888;
const char* wsPath = "/";						// Default path

 // ===== START TIMER FUNCTIONS =====
 hw_timer_t *timer;
volatile bool timerFlag = false;

// (ISR) Interrupt Service Routine
void IRAM_ATTR onTimer()
{
	// This code is executed every time the timer alarm triggers
  	timerFlag = true;
	ESP.restart();
}
/**
 * @fn startTimer()
 * @author Rainer Müller-Knoche mk@muekno.de
 * @brief  start a HW timer for 30 seconds
 * @note  needed fo backlight off if screen was touched but nothing done
 * or to to stop video after 30 seconds
 * @date 26.11.2925 mk
 * @date 27.11.2025 mk
 */
void startTimer()
{
	timer = timerBegin(0, 80, true);
	timerAttachInterrupt(timer, &onTimer, true);		// attach interrupt handler
	timerAlarmWrite(timer, 30000000,false);				// For 30 seconds: 30 * 1000000 = 30000000 us
														// false for one.shot
	timerAlarmEnable(timer);							// enable Timer
}
/**
 * @fn restartTimer()
 * @author Rainer Müller-Knoche mk@muekno.de
 * @brief restarts the timer, if a key was pressed let the timeout start again
 * @date 26.11.2025 mk
 * @date 27.11.2025 mk
 */
void reStartTimer()
{
	timerRestart(timer);
}
// === END TIMER FUNCTIONS ===

#define TFT_BL 2

class LGFX : public lgfx::LGFX_Device {
public:

lgfx::Bus_RGB    	_bus_instance;
lgfx::Panel_RGB  	_panel_instance;
LGFX(void)
	{
		{
			auto cfg = _bus_instance.config();
			cfg.panel = &_panel_instance;
			// blau
			cfg.pin_d0 = GPIO_NUM_8;	// B0
			cfg.pin_d1 = GPIO_NUM_3;	// B1
			cfg.pin_d2 = GPIO_NUM_46;	// B2
			cfg.pin_d3 = GPIO_NUM_9;	// B3
			cfg.pin_d4 = GPIO_NUM_1;	// B4
			// grün
			cfg.pin_d5 = GPIO_NUM_5;	// G0
			cfg.pin_d6 = GPIO_NUM_6;	// G1
			cfg.pin_d7 = GPIO_NUM_7;	// G2
			cfg.pin_d8 = GPIO_NUM_15;	// G3
			cfg.pin_d9 = GPIO_NUM_16;	// G4
			// rot
			cfg.pin_d10 = GPIO_NUM_4;	// G5
			cfg.pin_d11 = GPIO_NUM_45;	// R0
			cfg.pin_d12 = GPIO_NUM_48;	// R1
			cfg.pin_d13 = GPIO_NUM_47;	// R2
			cfg.pin_d14 = GPIO_NUM_21;	// R3
			cfg.pin_d15 = GPIO_NUM_14;	// R4
			// control
			cfg.pin_henable = GPIO_NUM_40;
			cfg.pin_vsync = GPIO_NUM_41;
			cfg.pin_hsync = GPIO_NUM_39;
			cfg.pin_pclk= GPIO_NUM_0;
			cfg.freq_write= 15000000;
			// 
			cfg.hsync_polarity= 0;
			cfg.hsync_front_porch = 8;
			cfg.hsync_pulse_width = 4;
			cfg.hsync_back_porch= 43;
			//
			cfg.vsync_polarity= 0;
			cfg.vsync_front_porch = 8;
			cfg.vsync_pulse_width = 4;
			cfg.vsync_back_porch= 12;
			//
			cfg.pclk_active_neg = 1;
			cfg.de_idle_high= 0;
			cfg.pclk_idle_high= 0;

			_bus_instance.config(cfg);
		}

		{
			auto cfg = _panel_instance.config();
			cfg.memory_width= 800;		
			cfg.memory_height = 480;
			cfg.panel_width= 800;
			cfg.panel_height = 480;
			cfg.offset_x = 0;
			cfg.offset_y = 0;
			_panel_instance.config(cfg);
		}
		_panel_instance.setBus(&_bus_instance);
		setPanel(&_panel_instance);
	}
};

LGFX lcd;

SPIClass& spi = SPI;
/*******************************************************************************
Please config the touch panel in touch.h
 ******************************************************************************/
#include "touch.h"
/* Change to your screen resolution */
static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t disp_draw_buf[800 * 480 / 10];
//static lv_color_t disp_draw_buf;
static lv_disp_drv_t disp_drv;

/**
 * @fn Callback my_display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
 * @author from Elecrow example
 * @date 26.11.2925 mk
 * @date 27.11.2025 mk Comment
 */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
	uint32_t w = (area->x2 - area->x1 + 1);
	uint32_t h = (area->y2 - area->y1 + 1);
	//display.fillScreen(TFT_WHITE) ?;
	lcd.pushImageDMA(area->x1, area->y1, w, h,(lgfx::rgb565_t*)&color_p->full);//
	lv_disp_flush_ready(disp);
} // END my_disp_flush
/**
* @fn callback my_touchpad_read
* @author Rainer Müller-Knoche based on Elegrow example
* @brief Anzeige Touch Position auf Monitor
* @note firsttouched Flag for loop rein
 * @date 26.11.2025 mk
 * @date 27.11.2025 mk Comment
*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
	if (touch_has_signal())
	{
		if (touch_touched())
		{
			firstTouch = true;					// MK indicate touched for loop
			data->state = LV_INDEV_STATE_PR;
			/*Set the coordinates*/
			data->point.x = touch_last_x;
			data->point.y = touch_last_y;
			#ifdef DEBUG
				Serial.print("Data x : ");	Serial.println(touch_last_x);
				Serial.print("Data y : ");	Serial.println(touch_last_y);
			#endif
		}
		else if (touch_released())
		{
			data->state = LV_INDEV_STATE_REL;
		}
	}
	else
	{
		data->state = LV_INDEV_STATE_REL;
	}
	delay(15); // ver3
} // END my_touchpad_read
PCA9557 Out;	// ver3 Out is PCA9557

/**
 * @fn print_msg(char message[], int pos_X, int pos_Y, int schrift = 0)
 * @author Rainer Müller-Knoche mk@muekno.de
 * @brief Anzeige einer Nachricht an Pos X,Y. 0,0 ist Display rechts oben. 
 * @note Achtung Display wird Portrait eingebaut ist aber Landscape orientiert.
 * @note Optionaler Paramter "schrift" für spaetere Auswertung
 * @param String anzuzeigender Text
 * @param int posX (von rechts oben nach unten)
 * @param int posY (von rechts oben nach links) 
 * @param int schrift = 0 Size = 46 (default) schrift = 1 Size =24
 * @date 26.11.2025 mk
 * @date 27.11.2025 mk Optik
 */
void print_msg(char message[], int pos_X, int pos_Y, int schrift = 0)
{
	lv_obj_t * msg_text;
	msg_text = lv_label_create(lv_scr_act());
	lv_obj_set_pos(msg_text, pos_X, pos_Y);
	if (schrift > 0)
	{
		lv_obj_set_style_text_font(msg_text, &lv_font_montserrat_46, 0);	/**Set the labels text*/
	}
	else
	{
		lv_obj_set_style_text_font(msg_text, &lv_font_montserrat_24, 0);	/**Set the labels text*/
	}
	lv_label_set_text(msg_text, message);
}
#include <keypad.h>

	using namespace websockets;
	WebsocketsClient client;

	unsigned long lastPing = 0;  				// Für periodische Pings
	unsigned long lastDisconnect = 0; 			// Für Reconnect-Delay
	const unsigned long reconnectDelay = 5000;  // 5 Sekunden warten vor Reconnect
	const unsigned long pingInterval = 30000;   // Ping alle 30 Sekunden

	/**
	 * @author Gil Maimon rainer Mülleer-Knoche mk@muekno.de
	 * @brief callback from lib readme
	 * @date 26.11.2025
	 * @date 02.12.2025 JPEG Anzeige rein
 	 */
	void onMessageCallback(WebsocketsMessage msg)
	{
    	Serial.print("Got Message: ");
		if (msg.isBinary())
		{
		    const uint8_t* jpgData = (const uint8_t*)msg.c_str();  // Access binary data
    		size_t jpgLen = msg.length();
		    //lcd.startWrite();  					// Begin transaction for faster drawing
    		lcd.drawJpg(jpgData, jpgLen, 0, 0);  	// Draw at (0,0) - adjust position/size as needed
    		//lcd.endWrite();    					// End transaction
  		}
  		else
		{
		    Serial.println("Received text: " + msg.data());
		}
	}
	/**
	 * @author Gil Maimon
	 * @brief callback from lib readme
	 * @date 26.11.2025
 	 */
	void onEventsCallback(WebsocketsEvent event, String data)
	{
	    if(event == WebsocketsEvent::ConnectionOpened) {
    	    Serial.println("On EventCb Connnection Opened");
    	} else if(event == WebsocketsEvent::ConnectionClosed) {
        	Serial.println("On EventCb Connnection Closed");
    	} else if(event == WebsocketsEvent::GotPing) {
        	Serial.println("On EventCb Got a Ping!");
    	} else if(event == WebsocketsEvent::GotPong) {
        	Serial.println("On EventCb Got a Pong!");
    	}
	}
/**
 * @fn setup()
 * @author Rainer Müller-Knoche mk@muekno.de
 * @brief setup functions
 * @date 26.11.2025 mk aufgeräumt
 * @date 27.11.2025 mk indicator for WiFi connect or not 
 * @brief display flash 3 times if no connect, 2 times if connect
 * @date 28.11.2025 mk
 * @date 02.12.2025 Anpassung  an Cropanal HW Ver 3.0
 *  */
void setup()
{
	 Serial.begin(115200);				// Start Serial
//	 while(!Serial){delay(100);}		// while loop blocks if no serial Monitor
	delay(200);
	 Wire.begin(19,20);					// brauchen wir wohl nicht
	pinMode(TFT_BL, OUTPUT);			// Backlight Control
	digitalWrite(TFT_BL, LOW);			// BL OUT

	// for Ver 3.0
	Out.reset();
	Out.setMode(IO_OUTPUT);
	Out.setState(IO0, IO_LOW);
  	Out.setState(IO1, IO_LOW);
  	delay(20);
  	Out.setState(IO0, IO_HIGH);
  	delay(100);
  	Out.setMode(IO1, IO_INPUT);
	// end for Ver 3.0

	Serial.println("get SD Card Values now"); 	// SD card is in SD_card.h and SD_Card.cppp  now
  	if (!loadConfigFromSD())					// should mever occur, but in case of as a backup
	{
		Serial.println("SD CARD PPOBLEM");
		ssid = defaultSsid;	password = defaultPassword;	localIP = defaultIP;	subnet = defaultSubnet;
		pin = defaultPin;	laenge = defaultLaenge;
	}
	Serial.println("Back from SD Card");
	#ifdef DEBUG
		Serial.print("D_ssid: ");Serial.println(ssid);
		Serial.print("D_password: ");Serial.println(password);
		Serial.print("D_localIP: ");Serial.println(localIP);
		Serial.print("D_gateway: ");Serial.println(gateway);
		Serial.print("D_subnet: ");Serial.println(subnet);
		Serial.print("D_pin: ");Serial.println(pin);
		Serial.print("D_laenge: ");Serial.println(laenge);
	#endif
	// convert 'laenge' and 'pin' from ASCII (String) to binary 
	pin_len = laenge[0] - '0';
	// PIN now
	for (int i= 0; i < pin_len;i++)
	{
		reference_code[i] = pin[i] - '0';
		Serial.print(reference_code[i]);
	}
	#ifdef DEBUG
		Serial.println("\rSet Wifi to STA mode");
		Serial.print("ssid: ");	Serial.println(ssid.c_str()); 
		Serial.print("password: ");	Serial.println(password.c_str());
		Serial.println("WiFi.begin(ssid.c_str(),password.c_str());");
	#endif
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid.c_str(),password.c_str());				
	// Wait mx 15 secondsome time to connect to wifi
	for(int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++)
	{
		Serial.print(".");
		delay(300);					// wait a little bit
	}
	// indicate connect failed
	if (WiFi.status() == WL_CONNECT_FAILED)
	{
		ESP.restart();
	}
	else 		// connected
	{
		Serial.print("WiFi Connected: myIP: ");	Serial.println(WiFi.localIP());
	}

	// Init Display
	lcd.begin();
	lcd.fillScreen(TFT_BLACK);	//E
// 	lcd.setTextSize(2);			//E
	delay(200);					//E
	lv_init();
	delay(100);					//E
	touch_init();
	screenWidth = lcd.width();
	screenHeight = lcd.height();
	lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * screenHeight / 10);
	//lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, 480 * 272 / 10);
	/* Initialize the display */
	lv_disp_drv_init(&disp_drv);
	/* Change the following line to your display resolution */
	disp_drv.hor_res = screenWidth;
	disp_drv.ver_res = screenHeight;
	disp_drv.flush_cb = my_disp_flush;						// touch callback
	disp_drv.draw_buf = &draw_buf;
	lv_disp_drv_register(&disp_drv);
	/* Initialize the (dummy) input device driver */
	static lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	indev_drv.read_cb = my_touchpad_read;
	lv_indev_drv_register(&indev_drv);
	lv_disp_drv_t disp_drv;
#ifdef TFT_BL									//E
	pinMode(TFT_BL, OUTPUT);					//E
	digitalWrite(TFT_BL, LOW);					//E
#endif											//E
    // ... initialize disp_drv ...
	lv_timer_handler();
// lv_gui_button(char btnt[], char labelt[], uint32_t posX, Uint32_t posY, uint32_t sX, int sY)

}	// End Setup

void loop()
{
	client.poll(); // as of ArduinoWebsockets lib example
	if (firstTouch) // set if a touch is regitered
	{
		if (!firstTouchSeen) // it is the first Touch
	{
		firstTouchSeen = true; // set first Touch flag
		digitalWrite(TFT_BL, HIGH); // switch on backlight
		create_buttons(1); // init and show keypad

		startTimer(); // start a 30 second timer, to reset the
		// display, if nothing mor happens
		Serial.println("TOUCHED"); // just notice
	}
	if(pin_ok) // the entered PIN was correct
	{
		Serial.println("PIN OK"); // just notice
		pin_ok = false; // reset flag, to enter only once
//		client.connect(gateway, 8888, "/");
		// try connect Websocket Server
		// can we get a status?
		client.send("stream"); // send something
		//client.ping(); // send a ping
	}

}
// Receive jpg now and display it
// lcd.drawJpg(( uint8_t*)msg.c_str(), msg.length(),0,0); // it is from the LovyanGFX library and works fine

//Serial.println( end - start);

lv_timer_handler(); /* let the GUI do its work */
// delay(2);
// now2 = millis();
}