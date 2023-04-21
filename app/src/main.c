#include "stm8s.h"
#include "delay.h"
#include "milis.h"
#include "tm1637.h"

#define BTN_RST_PORT GPIOB
#define BTN_RST_PIN GPIO_PIN_3

#define SENSOR_PORT GPIOC
#define SENSOR_PIN GPIO_PIN_5

#define TM1637_PORT GPIOB
#define TM16347_PIN_DIO GPIO_PIN_4
#define TM16347_PIN_CLK GPIO_PIN_5

char szTemp[8];
int min = 0, secs = 0;
uint16_t mtime_key = 0;

////////////////////////////////////////////////////////////////////
//! Uživatelské funkce
void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // Předdělička DIV1
    delay_init();                                  // Incializace časovače TIM4
    init_milis();                                  // Iniciaizace millis TIM2
    GPIO_Init(SENSOR_PORT, SENSOR_PIN, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(BTN_RST_PORT, BTN_RST_PIN, GPIO_MODE_IN_PU_NO_IT);
    tm1637Init(TM1637_PORT, TM16347_PIN_CLK, TM1637_PORT, TM16347_PIN_DIO);
    tm1637SetBrightness(7);
}
////////////////////////////////////////////////////////////////////
//! Main program loop
int main(void)
{
    setup();

    while (1)
    {
        if (GPIO_ReadInputPin(BTN_RST_PORT, BTN_RST_PIN) == RESET)
        {
            secs = 0;
            min = 0;
        }
        if ((get_milis() - mtime_key) > 1000) // každých 1500 ms
        {
            mtime_key = get_milis();
            if (secs == 60)
            {
                secs = 0;
                min++;
            }
            if (min == 100)
            {
                min = 0;
            }
            sprintf(szTemp, "%02d:%02d", min, secs);

            tm1637ShowDigits(szTemp);
            secs++;
        }
    }
}
////////////////////////////////////////////////////////////////////