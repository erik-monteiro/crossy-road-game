/* Nokia 5110 LCD AVR Library example
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by SkewPL, http://skew.tk
 * Custom char code by Marcelo Cohen - 2021
 */

#include <avr/io.h>
#include <util/delay.h>

#include "nokia5110.h"

uint8_t flecha[] = { 0b00011100, 0b00011100, 0b00111110, 0b00011100, 0b00001000 };
uint8_t pacman[] = { 0b00111000, 0b01101100, 0b01110000, 0b01111100, 0b00111000 };
uint8_t rua[] = { 0b10000000, 0b00000000,0b00000000 ,0b00000000 ,0b00000000  };
 

//uint8_t bomba[] = { 0b00000000, 0b00000000, 0b00111000, 0b00111100, 0b00111010 };

int main(void)
{
    nokia_lcd_init();
    nokia_lcd_clear();
    //nokia_lcd_custom(1, flecha);
    //nokia_lcd_write_string("IT'S WORKING!",1);
    //nokia_lcd_set_cursor(0, 12);
    //nokia_lcd_write_string("\001", 2);

    nokia_lcd_custom(2, pacman);
    nokia_lcd_set_cursor(34, 34);
    nokia_lcd_write_string("\002", 2);

    nokia_lcd_custom(3, rua);
    nokia_lcd_set_cursor(0, 6);
    nokia_lcd_write_string("\003", 2);
     nokia_lcd_set_cursor(0, 12);
    nokia_lcd_write_string("\003", 2);

    nokia_lcd_render();

    for (;;) {
        _delay_ms(1000);
    }
}