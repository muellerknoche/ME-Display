#ifndef KEYPAD_H
#define KEYPAD_H

#include <lvgl.h>
#include <config.h>

#define TFT_Bl 2
// Konstanten für Tastenposition

int col_1 = 400;//(480 -290)/2;
int col_2 = col_1 - 100;
int col_3 = col_2 - 100;

int row_1 = 10;
int row_2 = row_1 + 100; 
int row_3 = row_2 + 100;
int row_4 = row_3 + 100;

int sizeX = 90;
int sizeY = 90;

// prueft nach Eingabe der PIN Laenge auf Gueltigkeit
void check_pin(uint8_t number)
{
	BL_timer_start_time = millis();					// Ausschalten des Bildschirms verhindern
	bool fails = false;								// default OK

	#ifdef DEBUG
		Serial.print("START ");
		Serial.println(millis()); 
		Serial.print("Eingabezaehler: ");				// zaehlt eingegeben Zeichen
		Serial.println(eingabe_zaehler);
	#endif

	// check for max
	if (eingabe_zaehler < pin_len)
	{
		// Kennung für Eingabe anzeigen unter KeyPad
		// ist fix Y wandert
		print_msg( "*",loopPosX, loopPosY - (100 * eingabe_zaehler),1);

		#ifdef DEBUG
			Serial.print("number: ");
			Serial.println(number);
		#endif

		incode[eingabe_zaehler] = number;		// Eingabe speichern
		eingabe_zaehler ++;

		#ifdef DEBUG
			Serial.print(millis());
			Serial.println(" END");
		#endif

	}

	if (eingabe_zaehler == 4)
	{
//		Serial.println("START CHECK >>>>>>:");
//		print_msg("           ",600,100);
//		//print_msg("Start Check",600,100);
		uint8_t i = 0;
		for (uint8_t i = 0;i < pin_len; i++)
		{

			#ifdef DEBUG
				Serial.print("LOOP i: "); Serial.print(i);
				Serial.print(" INCODE: "); Serial.print(incode[i]);
				Serial.print(" code_fix: ");Serial.println(code_fix[i]);
			#endif

			if (code_fix[i] != incode[i])
			{

				#ifdef DEBUG
					Serial.print("LOOP i "); Serial.print(i); Serial.println("  Not equal Abbruch");
				#endif

				fails = true;				// fehlerhafte Eingabe
				i = pin_len + 1;			// force end, loopcount > mx
			}
		}
		if 	(fails == false)				// pin was OK
		{
			BL_timer_active = false;
			pinOk = true;
			video_timer_start_time = millis();
			lv_obj_t *scr = lv_scr_act();
			lv_obj_clean(scr);				// Clear Screen

//			lv_obj_set_style_bg_color(scr,lv_palette_main(LV_PALETTE_GREEN),LV_PART_MAIN);


		}
		else
		{
			failed = true;
			Serial.println("FAIL    ");
			pinOk = false;
			lv_obj_t * scr = lv_scr_act();
			lv_obj_clean(scr);
//			lv_obj_set_style_bg_color(scr,lv_palette_main(LV_PALETTE_RED),LV_PART_MAIN);

			//print_msg("FEHLER",430,180,1);
			
		}
	}
}  // End check PIN


// EINS CB
static void eins_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		// lv_obj_t *eins_txt;
		// eins_txt = lv_label_create(lv_scr_act());
		// lv_obj_set_pos(eins_txt, 400, 10);
    	// lv_label_set_text(eins_txt, "*1");
		// lv_obj_set_style_text_font(eins_txt, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		
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
		// lv_obj_t *zwei_txt;
		// zwei_txt = lv_label_create(lv_scr_act());
		// lv_obj_set_pos(zwei_txt, 400, 100);
		// lv_obj_set_style_text_font(zwei_txt, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		// lv_label_set_text(zwei_txt, "*2");
		Serial.println("2 pressed");
		check_pin(2);
	}
}

// DREI CB
static void drei_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		// lv_obj_t *drei_txt;
		// drei_txt = lv_label_create(lv_scr_act());
		// lv_obj_set_pos(drei_txt, 400, 190);
		// lv_obj_set_style_text_font(drei_txt, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		// lv_label_set_text(drei_txt, "*3");
		Serial.println("3 pressed");
		uint8_t eingabe = 3;
		check_pin(eingabe);
	}
}

static void vier_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		// lv_obj_t * vier_txt;
		// vier_txt = lv_label_create(lv_scr_act());
		// lv_obj_set_pos(vier_txt, 400, 280);
		// lv_obj_set_style_text_font(vier_txt, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		// lv_label_set_text(vier_txt, "*4");
		Serial.println("4 pressed");
		uint8_t eingabe = 4;
		check_pin(eingabe);
	}
}

static void fuenf_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		// lv_obj_t *fuenf_txt;
		// fuenf_txt = lv_label_create(lv_scr_act());
		// lv_obj_set_pos(fuenf_txt, 400, 370);
		// lv_obj_set_style_text_font(fuenf_txt, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		// lv_label_set_text(fuenf_txt, "*5");
		Serial.println("5 pressed");
		uint8_t eingabe = 5;
		check_pin(eingabe);
	}
}

static void sechs_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED){
		// lv_obj_t *sechs_txt;
		// sechs_txt = lv_label_create(lv_scr_act());
		// lv_obj_set_pos(sechs_txt, 600, 10);
		// lv_obj_set_style_text_font(sechs_txt, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		// lv_label_set_text(sechs_txt, "*6");
		Serial.println("6 pressed");
		uint8_t eingabe = 6;
		check_pin(eingabe);
	}
}

static void sieben_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		// lv_obj_t *sieben_txt;
		// sieben_txt = lv_label_create(lv_scr_act());
		// lv_obj_set_pos(sieben_txt, 600, 100);
		// lv_obj_set_style_text_font(sieben_txt, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		// lv_label_set_text(sieben_txt, "*7");
		Serial.println("7 pressed");
		uint8_t eingabe = 7;
		check_pin(eingabe);
	}
}

static void acht_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		// lv_obj_t *acht_txt;
		// acht_txt = lv_label_create(lv_scr_act());
		// lv_obj_set_pos(acht_txt, 600, 190);
		// lv_obj_set_style_text_font(acht_txt, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		// lv_label_set_text(acht_txt, "*8");
		Serial.println("8 pressed");
		check_pin(8);
	}
}

static void neun_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		// lv_obj_t *neun_txt;
		// neun_txt = lv_label_create(lv_scr_act());
		// lv_obj_set_pos(neun_txt, 600, 270);
		// lv_obj_set_style_text_font(neun_txt, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		// lv_label_set_text(neun_txt, "*9");
		Serial.println("9 pressed");
		check_pin(9);
	}
}

static void cl_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		// lv_obj_t *cl_txt;
		// cl_txt = lv_label_create(lv_scr_act());
		// lv_obj_set_pos(cl_txt, 600, 360);
		// lv_obj_set_style_text_font(cl_txt, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		// lv_label_set_text(cl_txt, "*CL");
		Serial.println("CL pressed");
		// clear()										// alle Eingaben vergessen
	}

}

static void nullx_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		// lv_obj_t *nullx_txt;
		// nullx_txt = lv_label_create(lv_scr_act());
		// lv_obj_set_pos(nullx_txt, 600, 450);
		// lv_obj_set_style_text_font(nullx_txt, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		// lv_label_set_text(nullx_txt, "*0");
		Serial.println("0 pressed");
		check_pin(0);
	}
}


static void ok_event(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		// lv_obj_t *ok_txt;
		// ok_txt = lv_label_create(lv_scr_act());
		// lv_obj_set_pos(ok_txt, 500, 270);
		// lv_obj_set_style_text_font(ok_txt, &lv_font_montserrat_46, 0);	/**Set the labels text*/
		// lv_label_set_text(ok_txt, "*OK");
		Serial.println("OK pressed");
		// ok_func			// Ausfuehren wenn OK
	}
}


/**
 * @brief 10er Tastatur erzuegen
 * @note erzeugt eine 10er Tastatur mit CLEAR und OK zur Eingabe einer PIN
 * Display ist in Landscape Tasten um 270 ° gedreht, damit in Portrait  Richtung eingebaut werden kann
 * Parameter farbe für Erweiterung
 * @param uint8_t farbe = 0 Option
 */
void create_buttons(uint8_t farbe = 0)
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

		// Button OK
		lv_obj_t * okx = lv_btn_create(lv_scr_act());					/*Add a button the current screen*/
		lv_obj_set_pos(okx, row_4, col_3);								/*Set its position*/
		lv_obj_set_size(okx, sizeX, sizeY);								/*Set its size*/
		lv_obj_add_event_cb(okx, ok_event, LV_EVENT_ALL, NULL);			/*Assign a callback to the button*/
		lv_obj_set_style_transform_angle(okx, 2700, 0);

		//lv_obj_set_style_local_bg_color(okx, LV_BTN_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
		lv_obj_t * l_okx = lv_label_create(okx);						/*Add a label to the button*/

		lv_obj_set_style_text_font(l_okx, &lv_font_montserrat_46, 0);	/**Set the labels text*/

		//		lv_obj_set_style_text_color(l_okx,);

		lv_label_set_text(l_okx, "OK");									/*Set the labeks text*/
		lv_obj_center(l_okx);
	}
	// ok_func()														// kamera ein

} // end create_buttons

#endif  // KEYPAD_H