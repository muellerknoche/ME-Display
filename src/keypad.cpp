/**
 * @author Rainer Müller-Knoche mk@muekno-de
 * @brief this mdule creates a nummeric keypad using LVGL 8.3.6 number 1-9
 * and a clear key which resets the the Display.
 * Entering a Key will show an Asterisk below the keypad. If the max length
 * of the entered PIN is reached, the entered PIN will be comparedto the given
 * correct PIN. If equal a flag will be set for loop, otherwise the Display will ne reseted
 * @date 26.11.2025 afternoon mk
 */

#include "my_globals.h"
#include "main.h"
#include <Arduino.h>
#include <lvgl.h>

#define  DEBUG
// Konstanten für Tastenposition
// x,y 0,0 = rechts oben 
// X von oben nach unten
// Y Rechts nach links
//int col_1 = 400;//(480 -290)/2;
int col_1 = 455;			// Y Abstand von rechts
int col_2 = col_1 - 150;	// 
int col_3 = col_2 - 150;
int row_1 = 10;				// X Abstand = 10 von oben
int row_2 = row_1 + 150; 	// X 10 + 130(key) + 10 Abstand
int row_3 = row_2 + 150;
int row_4 = row_3 + 150;
int sizeX = 130;
int sizeY = 130;
bool failed = false;
int loopPosX = 700;
int loopPosY = 350;

/**
 * @author Rainer Müller-Knoche
 * @date 09.11.2025 comments
 * @brief stores pressed key,  counts pressed keys, if the last key (PIN length) is reached
 * enter PIN is compared  to the correct PIN if Ok an OK flagis set for loop.
 * Else the display is reseted. Ever key resets th 30 second timeout timer.
 * @param number the value of pressed key.
 * @date 26.11.2026
 */
void check_pin(uint8_t number)
{
	reStartTimer();				// timeout verlängern Wenn eingabe
	bool fails = false;			// default OK
	#ifdef DEBUG
		Serial.print("Eingabezaehler: ");				// zaehlt eingegeben Zeichen
		Serial.println(eingabe_zaehler);
		Serial.print("pin_len: ");				// zaehlt eingegeben Zeichen
		Serial.println(pin_len);
	#endif
	// check for max PIN length
	if (eingabe_zaehler < pin_len)
	{
		// Timeout verlaengern wenn Taste gedrückt
		reStartTimer();
		// Kennung für Eingabe anzeigen unter KeyPad
		// X ist fix Y wandert
	 	char buffer[] = "*";	// Astrisk as a feedbak
		char* star = buffer;
		print_msg(star,loopPosX, loopPosY - (100 * eingabe_zaehler),1);
		#ifdef DEBUG
			Serial.print("number: ");	Serial.println(number);
		#endif

		in_code[eingabe_zaehler] = number;		// tore entered key
		eingabe_zaehler++;
	}
	if (eingabe_zaehler == pin_len)				// pin length reached
	{
		for (uint8_t i = 0;i < pin_len; i++)
		{
			#ifdef DEBUG
				Serial.print("LOOP i: "); Serial.print(i); 	Serial.print(" IN_CODE: "); Serial.print(in_code[i]);
				Serial.print(" REFERENCE_CODE: "); Serial.println(reference_code[i]);
			#endif
			if (reference_code[i] != in_code[i])
			{
// !!!!
				#ifdef DEBUG
					Serial.print("LOOP i "); Serial.print(i); Serial.println("  Not equal Abbruch");
				#endif
				fails = true;				// fehlerhafte Eingabe
				i = pin_len + 1;			// force end, loopcount > mx
			}
		}
		if 	(fails == false)					// pin was OK, fails == false means no fail
		{
			#ifdef DEBUG
				Serial.println("Correct");
			#endif
			pin_ok = true;
			lv_obj_t *scr = lv_scr_act();
			lv_obj_clean(scr);					// Clear Screen
//			lv_obj_set_style_bg_color(scr,lv_palette_main(LV_PALETTE_GREEN),LV_PART_MAIN);
		}
		else									// fails was true PIN not correct
		{
			ESP.restart();						// reset the display
		}
	}
}  // End check PIN

/**
 * @author Rainer Müller-Knoche mk@muekno.de
 * @brief the following functions are callback for each possible key
 * @brief they are called when the corresponding key is pressed
 * @brief and call the check_pin function
 * @param the correspondig  object * e
 */
// EINS CB
static void eins_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		Serial.println("1 pressed");
		check_pin(1);
	}
}
// ZWEI CB
static void zwei_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		check_pin(2);
	}
}
// DREI CB
static void drei_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		check_pin(3);
	}
}
static void vier_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		check_pin(4);
	}
}
static void fuenf_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		check_pin(5);
	}
}
static void sechs_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED){
		check_pin(6);
	}
}
static void sieben_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		check_pin(7);
	}
}
static void acht_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		Serial.println("8 pressed");
		check_pin(8);
	}
}
static void neun_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		Serial.println("9 pressed");
		check_pin(9);
	}
}
static void cl_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		Serial.println("CL pressed");
		delay(50);
		ESP.restart();										// alle Eingaben vergessen
	}
}
static void nullx_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		Serial.println("0 pressed");
		check_pin(0);
	}
}
// not used yet
static void ok_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		Serial.println("OK pressed");
	}
}
/**
 * @author Rainer Müller-Knoche mk@muekno.de
 * @brief 10er Tastatur erzuegen
 * @note erzeugt eine 10er Tastatur mit CLEAR zur Eingabe einer PIN
 * Display ist in Landscape Tasten um 270 ° gedreht, damit in Portrait Richtung eingebaut werden kann
 * Parameter farbe für Erweiterung
 * @param uint8_t farbe = 0 Option
 * @date 26.11.2025 mk überarbeitet kommentiertes raus,kommentare aktualisiert
 */
void create_buttons(uint8_t farbe)
{
	if (farbe < 10)
	{
		// Button EINS
		lv_obj_t * eins = lv_btn_create(lv_scr_act());					/*Add a button the current screen*/
		lv_obj_set_pos(eins, row_1, col_1);								/*Set its position*/
		lv_obj_set_size(eins, sizeX, sizeY);							/*Set its size*/
		lv_obj_set_style_transform_angle(eins, 2700, 0);
		lv_obj_add_event_cb(eins, eins_event, LV_EVENT_ALL, NULL);		/*Assign a callback to the button*/
		lv_obj_t * l_eins = lv_label_create(eins);						/*Add a label to the button*/
		lv_label_set_text(l_eins, "1");									/*Set the labeks text*/
		lv_obj_set_style_text_font(l_eins, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		lv_obj_center(l_eins);
		// Button ZWEI
		lv_obj_t * zwei = lv_btn_create(lv_scr_act());					/*Add a button the current screen*/
		lv_obj_set_pos(zwei, row_1, col_2);								/*Set its position*/
		lv_obj_set_size(zwei, sizeX, sizeY);							/*Set its size*/
		lv_obj_add_event_cb(zwei, zwei_event, LV_EVENT_ALL, NULL);		/*Assign a callback to the button*/
		lv_obj_set_style_transform_angle(zwei, 2700, 0);	
		lv_obj_t * l_zwei = lv_label_create(zwei);						/*Add a label to the button*/
		lv_obj_set_style_text_font(l_zwei, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		lv_label_set_text(l_zwei, "2");									/*Set the labeks text*/
		lv_obj_center(l_zwei);
		// Button DREI
		lv_obj_t * drei = lv_btn_create(lv_scr_act());					/*Add a button the current screen*/
		lv_obj_set_pos(drei, row_1,col_3);								/*Set its position*/
		lv_obj_set_size(drei, sizeX, sizeY);							/*Set its size*/
		lv_obj_add_event_cb(drei, drei_event, LV_EVENT_ALL, NULL);		/*Assign a callback to the button*/
		lv_obj_set_style_transform_angle(drei, 2700, 0);
		lv_obj_t * l_drei = lv_label_create(drei);						/*Add a label to the button*/
		lv_obj_set_style_text_font(l_drei, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		lv_label_set_text(l_drei, "3");									/*Set the labeks text*/
		lv_obj_center(l_drei);
		// Button VIER
		lv_obj_t * vier = lv_btn_create(lv_scr_act());					/*Add a button the current screen*/
		lv_obj_set_pos(vier,row_2, col_1);								/*Set its position*/
		lv_obj_set_size(vier, sizeX, sizeY);							/*Set its size*/
		lv_obj_add_event_cb(vier, vier_event, LV_EVENT_ALL, NULL);		/*Assign a callback to the button*/
		lv_obj_set_style_transform_angle(vier, 2700, 0);	
		lv_obj_t * l_vier = lv_label_create(vier);						/*Add a label to the button*/
		lv_obj_set_style_text_font(l_vier, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		lv_label_set_text(l_vier, "4");									/*Set the labeks text*/
		lv_obj_center(l_vier);
		// Button FUENF
		lv_obj_t * fuenf = lv_btn_create(lv_scr_act());					/*Add a button the current screen*/
		lv_obj_set_pos(fuenf, row_2, col_2);							/*Set its position*/
		lv_obj_set_size(fuenf, sizeX, sizeY);							/*Set its size*/
		lv_obj_add_event_cb(fuenf, fuenf_event, LV_EVENT_ALL, NULL);	/*Assign a callback to the button*/
		lv_obj_set_style_transform_angle(fuenf, 2700, 0);	
		lv_obj_t * l_fuenf = lv_label_create(fuenf);					/*Add a label to the button*/
		lv_obj_set_style_text_font(l_fuenf, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		lv_label_set_text(l_fuenf, "5");								/*Set the labeks text*/
		lv_obj_center(l_fuenf);
		// Button SECHS
		lv_obj_t * sechs = lv_btn_create(lv_scr_act());					/*Add a button the current screen*/
		lv_obj_set_pos(sechs, row_2, col_3);							/*Set its position*/
		lv_obj_set_size(sechs, sizeX, sizeY);							/*Set its size*/
		lv_obj_add_event_cb(sechs, sechs_event, LV_EVENT_ALL, NULL);	/*Assign a callback to the button*/
		lv_obj_set_style_transform_angle(sechs, 2700, 0);	
		lv_obj_t * l_sechs = lv_label_create(sechs);					/*Add a label to the button*/
		lv_obj_set_style_text_font(l_sechs, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		lv_label_set_text(l_sechs, "6");								/*Set the labeks text*/
		lv_obj_center(l_sechs);
		// Button SIEBEN
		lv_obj_t * sieben = lv_btn_create(lv_scr_act());				/*Add a button the current screen*/
		lv_obj_set_pos(sieben, row_3, col_1);							/*Set its position*/
		lv_obj_set_size(sieben, sizeX, sizeY);							/*Set its size*/
		lv_obj_add_event_cb(sieben, sieben_event, LV_EVENT_ALL, NULL);	/*Assign a callback to the button*/
		lv_obj_set_style_transform_angle(sieben, 2700, 0);	
		lv_obj_t * l_sieben = lv_label_create(sieben);					/*Add a label to the button*/
		lv_obj_set_style_text_font(l_sieben, &lv_font_montserrat_46, 0);/**Set the labels text*/
		lv_label_set_text(l_sieben, "7");								/*Set the labeks text*/
		lv_obj_center(l_sieben);
		// Button ACHT
		lv_obj_t * acht = lv_btn_create(lv_scr_act());					/*Add a button the current screen*/
		lv_obj_set_pos(acht, row_3, col_2);								/*Set its position*/
		lv_obj_set_size(acht, sizeX, sizeY);							/*Set its size*/
		lv_obj_add_event_cb(acht, acht_event, LV_EVENT_ALL, NULL);		/*Assign a callback to the button*/
		lv_obj_set_style_transform_angle(acht, 2700, 0);	
		lv_obj_t * l_acht = lv_label_create(acht);						/*Add a label to the button*/
		lv_obj_set_style_text_font(l_acht, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		lv_label_set_text(l_acht, "8");									/*Set the labeks text*/
		lv_obj_center(l_acht);
		// Button NEUN
		lv_obj_t * neun = lv_btn_create(lv_scr_act());					/*Add a button the current screen*/
		lv_obj_set_pos(neun, row_3, col_3);								/*Set its position*/
		lv_obj_set_size(neun, sizeX, sizeY);							/*Set its size*/
		lv_obj_add_event_cb(neun, neun_event, LV_EVENT_ALL, NULL);		/*Assign a callback to the button*/
		lv_obj_set_style_transform_angle(neun, 2700, 0);	
		lv_obj_t * l_neun = lv_label_create(neun);						/*Add a label to the button*/
		lv_obj_set_style_text_font(l_neun, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		lv_label_set_text(l_neun, "9");									/*Set the labeks text*/
		lv_obj_center(l_neun);
		// Button CLEAR
		lv_obj_t * clear = lv_btn_create(lv_scr_act());					/*Add a button the current screen*/
		lv_obj_set_pos(clear, row_4, col_1);							/*Set its position*/
		lv_obj_set_size(clear, sizeX, sizeY);							/*Set its size*/
		lv_obj_add_event_cb(clear, cl_event, LV_EVENT_ALL, NULL);		/*Assign a callback to the button*/
		lv_obj_set_style_transform_angle(clear, 2700, 0);	
		lv_obj_t * l_clear = lv_label_create(clear);					/*Add a label to the button*/
		lv_obj_set_style_text_font(l_clear, &lv_font_montserrat_36, 0);	/**Set the labels text*/
		lv_label_set_text(l_clear, "Clear");							/*Set the labeks text*/
		lv_obj_center(l_clear);
		// Button NULL
		lv_obj_t * nullx = lv_btn_create(lv_scr_act());					/*Add a button the current screen*/
		lv_obj_set_pos(nullx, row_4, col_2);							/*Set its position*/
		lv_obj_set_size(nullx, sizeX, sizeY);							/*Set its size*/
		lv_obj_add_event_cb(nullx, nullx_event, LV_EVENT_ALL, NULL);	/*Assign a callback to the button*/
		lv_obj_set_style_transform_angle(nullx, 2700, 0);	
		lv_obj_t * l_nullx = lv_label_create(nullx);					/*Add a label to the button*/
		lv_obj_set_style_text_font(l_nullx, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		lv_label_set_text(l_nullx, "0");								/*Set the labeks text*/
		lv_obj_center(l_nullx);
		// Button OK not used yet
		// lv_obj_t * okx = lv_btn_create(lv_scr_act());					/*Add a button the current screen*/
		// lv_obj_set_pos(okx, row_4, col_3);								/*Set its position*/
		// lv_obj_set_size(okx, sizeX, sizeY);								/*Set its size*/
		// lv_obj_add_event_cb(okx, ok_event, LV_EVENT_ALL, NULL);			/*Assign a callback to the button*/
		// lv_obj_set_style_transform_angle(okx, 2700, 0);
		// //lv_obj_set_style_local_bg_color(okx, LV_BTN_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
		// lv_obj_t * l_okx = lv_label_create(okx);							/*Add a label to the button*/
		// lv_obj_set_style_text_font(l_okx, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		// lv_label_set_text(l_okx, "OK");									/*Set the labeks text*/
		// lv_obj_center(l_okx);
	}
	// ok_func()															// kamera ein
} // end create_buttons