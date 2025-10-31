/**
 * @brief Projekt Max-Exstein Video Spion
 * @authors Rainer Müller-Knoche, Elecrow
 * @note based on a Demo for the Cropanel HMI 5 inch 800x480 Display and the
 * ESP32-CAM_to_ESP32 Software found on the net streaming from CAM to TFT
 * @date startet in August 2025
 * @date now 20. Okt. 2025 nearly finished
 * @data 23. Oct, 2025 SD card rein timer Integration ??
 * @note neuer branch am 29.10.2025
 * @date weiter 30.10.2025
 */

#include <Arduino.h>

#define DEBUG				// comment for final

#include <main.h>
#include <config.h>
#include <WiFi.h>       	// For WiFi AP
#include <SD.h>         	// SD card library
#include <SPI.h>        	// SPI for SD
#include <sd_card.h>		// get data from SD

#include <lvgl.h>
#include <FS.h>
// #include <Adafruit_GFX.h>
// #include <stdio.h>
#include <LovyanGFX.hpp>
#include <TAMC_GT911.h>
#include <lvgl.h>
// #iclude <wire.h>
// #include "esp_psram.h"
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <ArduinoWebsockets.h>
#include <my_tools.h>
#include <config.h>

	hw_timer_t *timer = NULL;
	
	void IRAM_ATTR onTimer()
	{
		delay(100);
		ESP.restart();
	}

void startTimer()
{
	timer = timerBegin(0, 80, true);
	timerAttachInterrupt(timer, &onTimer, true);
	timerAlarmWrite(timer, 30000000,false);
	timerAlarmEnable(timer);

}

void reStartTimer()
{
	timerRestart(timer);
}


//SPIClass SD_SPI;

#define TFT_BL 2


class LGFX : public lgfx::LGFX_Device {
public:

lgfx::Bus_RGB    	_bus_instance;
lgfx::Panel_RGB  	_panel_instance;
lgfx::Light_PWM 	_light_instance;
lgfx::Touch_GT911	_touch_instance;
LGFX(void)
	{
		{
			auto cfg = _bus_instance.config();
			cfg.panel = &_panel_instance;

			cfg.pin_d0 = GPIO_NUM_8;	// B0
			cfg.pin_d1 = GPIO_NUM_3;	// B1
			cfg.pin_d2 = GPIO_NUM_46;	// B2
			cfg.pin_d3 = GPIO_NUM_9;	// B3
			cfg.pin_d4 = GPIO_NUM_1;	// B4

			cfg.pin_d5 = GPIO_NUM_5;	// G0
			cfg.pin_d6 = GPIO_NUM_6;	// G1
			cfg.pin_d7 = GPIO_NUM_7;	// G2
			cfg.pin_d8 = GPIO_NUM_15;	// G3
			cfg.pin_d9 = GPIO_NUM_16;	// G4
			cfg.pin_d10 = GPIO_NUM_4;	// G5

			cfg.pin_d11 = GPIO_NUM_45;	// R0
			cfg.pin_d12 = GPIO_NUM_48;	// R1
			cfg.pin_d13 = GPIO_NUM_47;	// R2
			cfg.pin_d14 = GPIO_NUM_21;	// R3
			cfg.pin_d15 = GPIO_NUM_14;	// R4

			cfg.pin_henable = GPIO_NUM_40;
			cfg.pin_vsync = GPIO_NUM_41;
			cfg.pin_hsync = GPIO_NUM_39;
			cfg.pin_pclk= GPIO_NUM_0;
			cfg.freq_write= 15000000;

			cfg.hsync_polarity= 0;
			cfg.hsync_front_porch = 8;
			cfg.hsync_pulse_width = 4;
			cfg.hsync_back_porch= 43;

			cfg.vsync_polarity= 0;
			cfg.vsync_front_porch = 8;
			cfg.vsync_pulse_width = 4;
			cfg.vsync_back_porch= 12;

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

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{

	uint32_t w = (area->x2 - area->x1 + 1);
	uint32_t h = (area->y2 - area->y1 + 1);

	//display.fillScreen(TFT_WHITE);
#if (LV_COLOR_16_SWAP != 0)
	lcd.pushImageDMA(area->x1, area->y1, w, h,(lgfx::rgb565_t*)&color_p->full);
#else
	lcd.pushImageDMA(area->x1, area->y1, w, h,(lgfx::rgb565_t*)&color_p->full);//
#endif

	lv_disp_flush_ready(disp);
	 

} // END my_disp_flush

/**
 * @brief Anzeige einer Nachricht an Pos X,Y. 0,0 ist Display rechts oben. 
 * @note Achtung Display wird Portrait eingebaut ist aber Landscape orientiert.
 * @note Optionaler Paramter "schrift" für spaetere Auswertung
 * @param String anzuzeigender Text
 * @param int posX (von rechts oben nach unten)
 * @param int posY (von rechts oben nach links) 
 * @param int schrift = 0 Size = 46 (default) schrift = 1 Size =24
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

lv_obj_t * time_msg;

// Restzeit auf TFT anzeigen
void print_time_rest(char message[], uint8_t clear_it = 0)
{
	if (clear_it == 0)
	{
		time_msg = lv_label_create(lv_scr_act());
		lv_obj_set_pos(time_msg, 410, 220);
		lv_obj_set_style_text_font(time_msg, &lv_font_montserrat_30, 0);	/**Set the labels text*/
		//lv_obj_set_style_text_color(time_msg,TFT_GREEN);
		lv_label_set_text(time_msg, message);
	}
	else
	{
		// time_msg = lv_label_create(lv_scr_act());
		// lv_obj_set_pos(time_msg, 430, 220);
		// lv_obj_set_style_text_font(time_msg, &lv_font_montserrat_36, 0);	/**Set the labels text*/
		lv_label_set_text(time_msg, "");
	}
}

// Anzeige Touch Position auf Monitor
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
	if (touch_has_signal())
	{
		if (touch_touched())
		{
			touched = true;					// indicate for loop

			data->state = LV_INDEV_STATE_PR;
			/*Set the coordinates*/
			data->point.x = touch_last_x;
			data->point.y = touch_last_y;

			//#ifdef DEBUG
				Serial.print("touched ");  Serial.println(millis());
				Serial.print( "Data x :" );
				Serial.println( touch_last_x );
				Serial.print( "Data y :" );
				Serial.println( touch_last_y );
			//#endif

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
	//delay(15);
} // END my_touchpad_read

//lv_obj_t * my_disp;
//lv_display_set_rotation(my_disp,90);

#include <keypad.h>


	// has to be outside any function
	using namespace websockets;
	WebsocketsServer server;

	WebsocketsClient client;


void setup()
{
	 Serial.begin(115200);			// Start Serial
	 while(!Serial){delay(100);}


	// Serial.println("START SERIAL");

	// Load config or use defaults
  	if (!loadConfigFromSD())
	{
		//ssid = defaultSSID;	password = defaultPassword;	localIP = defaultIP;	gateway = defaultGateway;	subnet = defaultSubnet;
		//pin = defaultPin;	laenge = defaultLaenge;
	}

  	// Start AP mode
  	WiFi.mode(WIFI_AP);
  	WiFi.softAP(ssid.c_str(), password.c_str());
  	Serial.println("AP started with SSID: " + ssid);
  	delay(100);  // Brief delay for AP init

  	// Configure IP settings
  	if (!WiFi.softAPConfig(localIP, gateway, subnet))
  	{
    	Serial.println("AP config failed");
  	} else {
    	Serial.println("AP config successful");
  	}

  	// Print AP IP
  	Serial.print("AP IP address: ");
  	Serial.println(WiFi.softAPIP());

	pinMode(TFT_BL, OUTPUT);		// Backlight Control

	// Init Display
	lcd.begin();
 	lcd.setTextSize(2);
	delay(200);
	lv_init();
	delay(100);
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
    // ... initialize disp_drv ...
    //disp_drv.rotated = LV_DISP_ROT_90;
	
	lv_timer_handler();

// lv_gui_button(char btnt[], char labelt[], uint32_t posX, Uint32_t posY, uint32_t sX, int sY)

	#ifdef DEBUG
		Serial.println();
		Serial.println("Create ESP32 as Access Point and start the server.");
		Serial.println("WIFI mode : AP");
	#endif


	server.listen(8888);

	#ifdef DEBUG
		Serial.println();
		Serial.print("Is server live ? ");
		if (server.available())
		{
			Serial.println("yes");	/* code */
		}
		else
		{
			Serial.println("NO");
		}
	
		Serial.println(server.available());
		Serial.println("-------------");
		//----------------------------------------
		Serial.println("Waiting for connection from ESP32-CAM (Client).");
	#endif

}	// End Setup

void loop()
{
	if (touched)														// touch dedected BL on make keypad
	{
		Serial.println(" loop  touchrd");
		touched = false;
		digitalWrite(2, HIGH); 										// BL ein
		create_buttons(1);											// keypad activ
		no_buttons = false;											// set flag keypad on
		startTimer();
	}

	// 	// Start the Viodo and the Timer only once
if (server.poll())
  {
    client = server.accept();
  }

  if (client.available())
  {
    client.poll();
    WebsocketsMessage msg = client.readBlocking();
	if (pinOkSet)
	{
		timerRestart(timer);									// restart timeout		if (!video)													// video läuft noch nicht
		{
			video = true;										// flag video runs
		}
		lcd.drawJpg(( uint8_t*)msg.c_str(), msg.length(),0,150);  // it is from the LovyanGFX library  and works  fine
	}
}
 
	lv_timer_handler(); /* let the GUI do its work */
//	delay(10);
	// now2 = millis();
}
