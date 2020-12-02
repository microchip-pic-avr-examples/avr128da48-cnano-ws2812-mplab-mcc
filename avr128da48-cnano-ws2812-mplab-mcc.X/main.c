/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include <util/delay.h>

#define TCA0_Enable()       do {TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;} while(0)
#define TCA0_Disable()      do {TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;} while(0)

#define LED_MAX_BRIGHT      0xFF
#define LED_MIN_BRIGHT      0x00
#define NUMBER_OF_LEDS      13
#define DELAY_TIME          100  //milliseconds
#define POSITION_MIN        0
#define POSITION_MAX        (NUMBER_OF_LEDS - 1)

const uint8_t colorTable[] = 
{
    LED_MAX_BRIGHT, LED_MIN_BRIGHT, LED_MIN_BRIGHT, // red
    LED_MAX_BRIGHT, LED_MAX_BRIGHT, LED_MIN_BRIGHT, // yellow
    LED_MIN_BRIGHT, LED_MAX_BRIGHT, LED_MIN_BRIGHT, // green
    LED_MIN_BRIGHT, LED_MAX_BRIGHT, LED_MAX_BRIGHT, // cyan
    LED_MIN_BRIGHT, LED_MIN_BRIGHT, LED_MAX_BRIGHT, // blue
    LED_MAX_BRIGHT, LED_MIN_BRIGHT, LED_MAX_BRIGHT  // purple
};

typedef struct
{
    uint8_t redChannel;
    uint8_t greenChannel;
    uint8_t blueChannel;
} color_t;

void WalkLight(color_t color);
void LightIntoPosition(color_t color, uint8_t position);
void SetLEDColor(color_t color);
void ClearLEDs(void);

int main(void)
{
    color_t currentColor;
    uint8_t colorIndex = 0;
    uint8_t i;

    /* Initializes MCU, drivers and middle ware */
    SYSTEM_Initialize();
    ClearLEDs();
	
    _delay_ms(DELAY_TIME);

    while (1)
    {
        colorIndex = 0;
        for(i = 0; i < 6; i++)
        {
            currentColor.redChannel   = colorTable[colorIndex++];
            currentColor.greenChannel = colorTable[colorIndex++];
            currentColor.blueChannel  = colorTable[colorIndex++];
            WalkLight(currentColor);
        }
    }
}

void WalkLight(color_t color)
{
    uint8_t i;
    
    for(i = POSITION_MIN; i < POSITION_MAX; i++)
    {
        LightIntoPosition(color, i);
        _delay_ms(DELAY_TIME);
    }
    
    for(i = POSITION_MAX+1; i > POSITION_MIN; i--)
    {
        LightIntoPosition(color, i-1);
        _delay_ms(DELAY_TIME);
    }
}

void LightIntoPosition(color_t color, uint8_t position)
{
    uint8_t i;
    color_t off = {LED_MIN_BRIGHT, LED_MIN_BRIGHT, LED_MIN_BRIGHT};
    
    for(i = 0; i < position; i++)
    {
       SetLEDColor(off);
    }
    
    SetLEDColor(color);
    
    for(i = position + 1; i < NUMBER_OF_LEDS; i++)
    {
        SetLEDColor(off);
    }
}

void SetLEDColor(color_t color)
{
    TCA0_Enable();
    
    SPI0_ExchangeByte(color.redChannel);
    SPI0_ExchangeByte(color.blueChannel);
    SPI0_ExchangeByte(color.greenChannel);
    
    TCA0_Disable();
}

void ClearLEDs(void)
{
    int i;
    color_t off = {LED_MIN_BRIGHT, LED_MIN_BRIGHT, LED_MIN_BRIGHT};
     
    for(i = 0; i < NUMBER_OF_LEDS; i++)
    {
        SetLEDColor(off);
    }
}