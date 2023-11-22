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

//uint8_t flecha[] = { 0b00011100, 0b00011100, 0b00111110, 0b00011100, 0b00001000 };
uint8_t flecha[] = { 0b00010000, 0b00111110, 0b01111110, 0b00111110, 0b00010000 };
uint8_t pacman[] = { 0b00001110, 0b00011011, 0b00011100, 0b00011111, 0b00001110 };
//uint8_t rua[] = { 0b11111111, 0b00000000, 0b10101010, 0b10101010, 0b00000000  };//outro modelo de rua 
uint8_t rua[] = { 0b11111111, 0b00000000, 0b11001100, 0b11001100, 0b00000000  };
uint8_t borda[] = { 0b11111111, 0b00000000, 0b11001100, 0b11001100, 0b11111111  };


//uint8_t bomba[] = { 0b00000000, 0b00000000, 0b00111000, 0b00111100, 0b00111010 };

void atualiza_lcd(int y);
void inicia_rua(int y);
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
    nokia_lcd_custom(4, borda);
    nokia_lcd_custom(5, flecha);
    

    inicia_rua(0);
    inicia_rua(12);
    inicia_rua(24);
    inicia_rua(36);

    nokia_lcd_custom(2, pacman);
    nokia_lcd_set_cursor(34, 37);
    nokia_lcd_write_string("\002", 2);

    nokia_lcd_render();
    int i = 0;
    for (;;) {
        nokia_lcd_set_cursor(34, i);
        nokia_lcd_write_string("\005", 2);
        nokia_lcd_render();
        _delay_ms(500);
        i++;
    }
}

void atualiza_lcd(int y){

}

void inicia_rua(int y){
    nokia_lcd_set_cursor(1, y);
    nokia_lcd_write_string("\003", 3);
    nokia_lcd_set_cursor(16, y);
    nokia_lcd_write_string("\003", 3);
    nokia_lcd_set_cursor(31, y);
    nokia_lcd_write_string("\003", 3);
    nokia_lcd_set_cursor(46, y);
    nokia_lcd_write_string("\003", 3);
    nokia_lcd_set_cursor(61, y);
    nokia_lcd_write_string("\004", 3);

    nokia_lcd_render();
}