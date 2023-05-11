#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "millis.h"
#include "uart.h"

#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b))))

#define BLUE_LED 0
#define GREEN_LED 1
#define YELLOW_LED 2
#define RED_LED 3

#define RED_LIGHT 1
#define YELLOW_LIGHT 2
#define GREEN_LIGHT 3

#define BUTTON_INPUT 4

int main()
{
    millis_init();
    sei();
    init_serial();

    volatile millis_t milliSecondsSinceButtonPress = 0;
    volatile millis_t milliSecondsSinceBlueLEDSwitch = millis_get();
    bool buttonHasBeenPressed = false;
    int lightColour = RED_LIGHT;
    BIT_SET(DDRB, BLUE_LED);
    BIT_SET(DDRB, GREEN_LED);
    BIT_SET(DDRB, YELLOW_LED);
    BIT_SET(DDRB, RED_LED);
    BIT_SET(PORTB, RED_LED);
    BIT_CLEAR(PORTB, BLUE_LED);
    BIT_CLEAR(PORTB, GREEN_LED);
    BIT_CLEAR(PORTB, YELLOW_LED);
    BIT_CLEAR(DDRD, BUTTON_INPUT);
    BIT_CLEAR(PIND, BUTTON_INPUT);

    while(1)
    {
        if((millis_get() - milliSecondsSinceBlueLEDSwitch > 4321) && BIT_CHECK(PORTB, BLUE_LED))
        {
            BIT_CLEAR(PORTB, BLUE_LED);
            milliSecondsSinceBlueLEDSwitch = millis_get();
        }

        if((millis_get() - milliSecondsSinceBlueLEDSwitch > 4321) && !BIT_CHECK(PORTB, BLUE_LED))
        {
            BIT_SET(PORTB, BLUE_LED);
            milliSecondsSinceBlueLEDSwitch = millis_get();
        }

        if(BIT_CHECK(PIND, BUTTON_INPUT) && !buttonHasBeenPressed)
        {
            setYELLOW();
            buttonHasBeenPressed = true;
            milliSecondsSinceButtonPress = millis_get();
            lightColour++;
        }

        if(millis_get() - milliSecondsSinceButtonPress > 3000 && lightColour == YELLOW_LIGHT)
        {
            setGREEN();
            lightColour++;
        }

        if(millis_get() - milliSecondsSinceButtonPress > 10000 && lightColour == GREEN_LIGHT)
        {
            setYELLOW();
            lightColour--;
        }

        if(millis_get() - milliSecondsSinceButtonPress > 13000 && lightColour == YELLOW_LIGHT)
        {
            setRED();
            lightColour--;
            buttonHasBeenPressed = false;
            milliSecondsSinceButtonPress = 0;
        }
    }
}

void setRED()
{
    BIT_CLEAR(PORTB, GREEN_LED);
    BIT_CLEAR(PORTB, YELLOW_LED);
    BIT_SET(PORTB, RED_LED);
}

void setYELLOW()
{
    BIT_CLEAR(PORTB, GREEN_LED);
    BIT_SET(PORTB, YELLOW_LED);
    BIT_CLEAR(PORTB, RED_LED);
}

void setGREEN()
{
    BIT_SET(PORTB, GREEN_LED);
    BIT_CLEAR(PORTB, YELLOW_LED);
    BIT_CLEAR(PORTB, RED_LED);
}