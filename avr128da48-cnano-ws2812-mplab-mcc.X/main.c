/*
    (c) 2023 Microchip Technology Inc. and its subsidiaries.

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

#include "mcc_generated_files/system/system.h"

#include <util/delay.h>


#define NUMBER_OF_LEDS    ( 10 )
#define DELAY_TIME_MS     ( 100 )


#define LED_MIN_BRIGHT    ( 0x00 )
#define LED_MAX_BRIGHT    ( 0xFF )
#define POSITION_MIN      ( 0 )
#define POSITION_MAX      ( NUMBER_OF_LEDS - 1 )


typedef struct COLOR
{
    uint8_t redChannel;
    uint8_t greenChannel;
    uint8_t blueChannel;
} color_t;


inline void ClearLEDs(void);
void WalkLight(color_t const color);
void LightIntoPosition(color_t const color, uint8_t const position);
void SetLEDColor(color_t const color);


color_t const colorTable[] =
{
    { LED_MAX_BRIGHT, LED_MIN_BRIGHT, LED_MIN_BRIGHT }, // Red
    { LED_MAX_BRIGHT, LED_MAX_BRIGHT, LED_MIN_BRIGHT }, // Yellow
    { LED_MIN_BRIGHT, LED_MAX_BRIGHT, LED_MIN_BRIGHT }, // Green
    { LED_MIN_BRIGHT, LED_MAX_BRIGHT, LED_MAX_BRIGHT }, // Cyan
    { LED_MIN_BRIGHT, LED_MIN_BRIGHT, LED_MAX_BRIGHT }, // Blue
    { LED_MAX_BRIGHT, LED_MIN_BRIGHT, LED_MAX_BRIGHT }  // Purple
};

color_t const off = {LED_MIN_BRIGHT, LED_MIN_BRIGHT, LED_MIN_BRIGHT};


int main(void)
{
    SYSTEM_Initialize();

    ClearLEDs();

    _delay_ms(DELAY_TIME_MS);

    while (true)
    {
        for(uint8_t colorIdx = 0; colorIdx < 6; ++colorIdx)
        {
            color_t const currentColor = colorTable[colorIdx];
            WalkLight(currentColor);
        }
    }
}

inline void ClearLEDs(void)
{
    for (uint8_t ledIdx = 0; ledIdx < NUMBER_OF_LEDS; ++ledIdx)
    {
        SetLEDColor(off);
    }
}

void WalkLight(color_t const color)
{
    for (uint8_t position = POSITION_MIN; position < POSITION_MAX; ++position)
    {
        LightIntoPosition(color, position);
        _delay_ms(DELAY_TIME_MS);
    }

    for (uint8_t position = POSITION_MAX + 1; position > POSITION_MIN; --position)
    {
        LightIntoPosition(color, position - 1);
        _delay_ms(DELAY_TIME_MS);
    }
}

void LightIntoPosition(color_t const color, uint8_t const position)
{
    for (uint8_t idx = 0; idx < position; ++idx)
    {
       SetLEDColor(off);
    }

    SetLEDColor(color);

    for (uint8_t idx = position + 1; idx < NUMBER_OF_LEDS; ++idx)
    {
        SetLEDColor(off);
    }
}

void SetLEDColor(color_t const color)
{
    TCA0_Start();

    SPI0_ByteExchange(color.redChannel);
    SPI0_ByteExchange(color.blueChannel);
    SPI0_ByteExchange(color.greenChannel);

    TCA0_Stop();
}
