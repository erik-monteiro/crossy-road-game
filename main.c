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

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "nokia5110.h"

uint8_t flecha[] = { 0b00010000, 0b00111110, 0b01111110, 0b00111110, 0b00010000 };
uint8_t pacman[] = { 0b00001110, 0b00011011, 0b00011100, 0b00011111, 0b00001110 };
uint8_t rua[] = { 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000  };
uint8_t borracha[] = { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000  };
uint8_t minutos = 0;
uint8_t segundos = 0;
uint8_t milisegundos = 0;
uint8_t flecha_posicao = 0;
uint8_t arrow_position = 0;
int x = 34;

//uint8_t bomba[] = { 0b00000000, 0b00000000, 0b00111000, 0b00111100, 0b00111010 };

#define TIMER_CLK		F_CPU / 64
// Freq. de interrupção: 10 Hz
#define IRQ_FREQ		10

//tratamento do INT0
ISR(INT0_vect)
{
    PORTB ^= (1 << PB1);
}

//tratamento do INT1
ISR(INT1_vect)
{
    PORTB ^= (1 << PB2);
}

ISR(TIMER2_COMPA_vect){
    milisegundos++;
    if (milisegundos >= 100) {
        milisegundos = 0;
        segundos++;
    }

    if(segundos <20){                       // Velocidade 1
        if(milisegundos%4==0){
            arrow_position++;
        }
    }

    if(segundos >= 20 && segundos < 60){    //Velocidade 2(Entre 20 e 60pts)
        if(milisegundos%2==0){
            arrow_position++;
        }
    }

    if(segundos >=60){                      //Velocidade 3(60 pts ou mais)
        arrow_position++;
    }
    
    if (arrow_position > 60) {
        arrow_position = 0;
        flecha_posicao = geraFlecha();
    }
}

void tela_inicial();
void atualiza_pontuacao();
void morreu();
void atualiza_flecha();
void inicia_rua(int y);
int geraFlecha();

int main(void)
{
    DDRD &= ~(1 << PD7);                    // seta PD7 como entrada
    DDRD &= ~(1 << PD6);                    // seta PD6 como entrada
    DDRB &= ~(1 << PB0);                    // seta PB0 como entrada

    DDRB |= (1 << PB1);                     // seta PB1 como saida
    DDRB |= (1 << PB2);                     // seta PB2 como saída

    nokia_lcd_init();

    tela_inicial();

    cli();                                  // desabilita interrupções
    int pausa = 1;
    int reset = 0;

    PORTB &= ~(1 << PB0);                   // desabilita pull-up de PB0
    PORTD &= ~(1 << PD7);                   // desabilita pull-up de PD7
    PORTD &= ~(1 << PD6);                   // desabilita pull-up de PD6
    
    EICRA = (1 << ISC01) | (1 << ISC00);
    EICRA |= (1 << ISC11) | (0 << ISC10);
    EIMSK |= (1 << INT1) | (1 << INT0); 
    
    sei();                                  //habilita interrupções

    nokia_lcd_custom(3, rua);

    inicia_rua(0);
    inicia_rua(12);
    inicia_rua(24);
    inicia_rua(36);

    nokia_lcd_custom(2, pacman);
    nokia_lcd_set_cursor(34, 37);
    nokia_lcd_write_string("\002", 2);

    nokia_lcd_render();

    nokia_lcd_custom(5, flecha);
    nokia_lcd_custom(6, borracha);

    OCR2A = (TIMER_CLK / IRQ_FREQ) - 1;     // seta o registrador output compare
    // liga modo CTC
    TCCR2A |= (1 << WGM21);
    // seta CS22, CS21 e CS20 para prescaler 1024
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
    TIMSK2 |= (1 << OCIE2A);                // habilita máscara do timer2

    for (;;) {
        morreu();
        atualiza_flecha();
        nokia_lcd_render();
        morreu();

        if((PIND &(1 << PD7)) != 0 && x > 4 && x <= 64){ //se o botão da esquerda(a) foi pressionado
            morreu();
            nokia_lcd_set_cursor(x, 37);
            nokia_lcd_write_string("\006", 2);        
            nokia_lcd_render();
            x = x - 15;

            nokia_lcd_set_cursor(x, 37);
            nokia_lcd_write_string("\002", 2);        
            nokia_lcd_render();
            morreu();
            _delay_ms(100);
        }

        morreu();
        atualiza_flecha();
        nokia_lcd_render();
        morreu();
        

        if((PIND &(1 << PD6)) != 0 && x >= 4 && x < 64){ //se o botão da direita(d) foi pressionado
            morreu();
            nokia_lcd_set_cursor(x, 37);
            nokia_lcd_write_string("\006", 2);        
            nokia_lcd_render();
            x = x + 15;
            
            nokia_lcd_set_cursor(x, 37);
            nokia_lcd_write_string("\002", 2);        
            nokia_lcd_render();
            morreu();
            _delay_ms(100);
        }

        morreu();
        atualiza_flecha();
        nokia_lcd_render();
        nokia_lcd_set_cursor(x, 37);
        nokia_lcd_write_string("\002", 2);
        nokia_lcd_render();
        atualiza_pontuacao();
        morreu();
    }
}



void tela_inicial(){
    nokia_lcd_set_cursor(10, 17);
    nokia_lcd_write_string("Bem vindo!",1);
    nokia_lcd_set_cursor(5, 25);
    nokia_lcd_write_string("Aperte start",1);
    nokia_lcd_render();
    while ((PINB &(1 << PB0)) == 0)         // Enquanto o botão de start(s) não for pressionado
    {
        if((PINB &(1 << PB0)) == 1){
            return;
        }
    }
    return;
}

void morreu(){
    if(flecha_posicao == x && arrow_position < 40  && arrow_position > 25){
        nokia_lcd_clear();
        nokia_lcd_set_cursor(10, 17);
        nokia_lcd_write_string("GAME OVER!",1);
        nokia_lcd_set_cursor(5, 25);
        nokia_lcd_write_string("Aperte start",1);
        nokia_lcd_set_cursor(10, 35);
        nokia_lcd_write_string("Pontos: ",1);
        char tempoTela[9];
        sprintf(tempoTela, "%03d", segundos);
        nokia_lcd_set_cursor(52, 35);
        nokia_lcd_write_string(tempoTela, 1);
        nokia_lcd_render();
        while ((PINB &(1 << PB0)) == 0)     // Enquanto o botão de start não for pressionado
        {
        }
        segundos = 0;
        arrow_position = 0;
        flecha_posicao = geraFlecha();
        nokia_lcd_clear();
        inicia_rua(0);
        inicia_rua(12);
        inicia_rua(24);
        inicia_rua(36);
    }
    return;
}

void atualiza_flecha(){
    if(flecha_posicao >= 4 && flecha_posicao <= 64){ 
        nokia_lcd_set_cursor(flecha_posicao, arrow_position);
        nokia_lcd_write_string("\005", 2);
        if(arrow_position >= 12){
            int b = arrow_position-12;
            nokia_lcd_set_cursor(flecha_posicao, b);
            nokia_lcd_write_string("\006", 2);    
        }
    }
}

void atualiza_pontuacao() 
{
    char tempoTela[9];

    sprintf(tempoTela, "%03d", segundos);

    nokia_lcd_set_cursor(1, 0);
    nokia_lcd_write_string(tempoTela, 1);
    nokia_lcd_render();
}

void inicia_rua(int y){
    nokia_lcd_set_cursor(0, y);
    nokia_lcd_write_string("\003", 3);
    nokia_lcd_set_cursor(15, y);
    nokia_lcd_write_string("\003", 3);
    nokia_lcd_set_cursor(30, y);
    nokia_lcd_write_string("\003", 3);
    nokia_lcd_set_cursor(45, y);
    nokia_lcd_write_string("\003", 3);
    nokia_lcd_set_cursor(60, y);
    nokia_lcd_write_string("\003", 3);
    nokia_lcd_set_cursor(75, y);
    nokia_lcd_write_string("\003", 2);

    nokia_lcd_render();
}

int geraFlecha() {
    return ((rand() % 5) * 15) + 4;
}