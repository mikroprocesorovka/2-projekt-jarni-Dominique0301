#include "stm8s.h"
#include <stdio.h>
#include "assert.h"
#include "milis.h"
#include "delay.h"
#include "stm8_hd44780.h"

#define LED_PORT GPIOC
#define LED_PIN  GPIO_PIN_5
#define LED_HIGH   GPIO_WriteHigh(LED_PORT, LED_PIN)
#define LED_LOW  GPIO_WriteLow(LED_PORT, LED_PIN)
#define LED_REVERSE GPIO_WriteReverse(LED_PORT, LED_PIN)

#define BTN_PORT GPIOE
#define BTN_PIN  GPIO_PIN_4
#define BTN_PUSH (GPIO_ReadInputPin(BTN_PORT, BTN_PIN)==RESET) 

#define NCODER_CLK_PORT GPIOB
#define NCODER_CLK_PIN GPIO_PIN_5
#define NCODER_DATA_PORT GPIOB
#define NCODER_DATA_PIN GPIO_PIN_4
#define NCODER_SW_PORT GPIOB
#define NCODER_SW_PIN GPIO_PIN_3
#define NCODER_GET_CLK (GPIO_ReadInputPin(NCODER_CLK_PORT, NCODER_CLK_PIN) != RESET)
#define NCODER_GET_DATA ( GPIO_ReadInputPin(NCODER_DATA_PORT, NCODER_DATA_PIN)!=RESET)
#define NCODER_GET_SW ( GPIO_ReadInputPin(NCODER_SW_PORT, NCODER_SW_PIN )==RESET)


void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovat MCU na 16MHz

    init_milis(); //Initializace milis
		
		GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(BTN_PORT, BTN_PIN, GPIO_MODE_IN_FL_NO_IT);

    GPIO_Init(NCODER_CLK_PORT, NCODER_CLK_PIN, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(NCODER_DATA_PORT, NCODER_DATA_PIN, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(NCODER_SW_PORT, NCODER_SW_PIN, GPIO_MODE_IN_PU_NO_IT);
}

int minule = 0;
char text[10];

int8_t check_ncoder(void)
{
		
    if (minule == 0 && NCODER_GET_CLK == 1) {
        // vzestupn? hrana 
        minule = 1;
        if (NCODER_GET_DATA == 0) {
            return -1;
        } else {
            return 1;
        }
    } 
    else if (minule == 1 && NCODER_GET_CLK == 0) {
        // sestupn? hrana 
        minule = 0;
        if (NCODER_GET_DATA == 0) {
            return 1;
        } else {
            return -1;
        }
    }
    return 0;
}

int main(void)
{
    uint32_t time = 0;
		uint32_t time_btn = 0;
		int8_t minule_led = 0;
    int16_t luxus = 0;
    setup();
		lcd_init();
		//lcd_gotoxy(0,0);

    while (1) {
			
        if (milis() - time > 33) { 
            time = milis();
						luxus += check_ncoder();
						lcd_gotoxy(0,0);
						sprintf(text, "cislo=%6d",luxus);
						lcd_puts(text);
        }
				
				if (milis() - time_btn > 2 && NCODER_GET_SW) {
					if (minule_led == 0) {
						LED_HIGH;
						minule_led = 1;
					}
					else {
						LED_LOW;
						minule_led = 0;
					}
				}
				luxus += check_ncoder();

    }
}









void assert_failed(uint8_t* file, uint32_t line)// stvd házelo nìjakou chybu s assert a tohle pomohlo
 

{
 

  /* User can add his own implementation to report the file name and line number,
 

     ex: printf(''Wrong parameters value: file %s on line %d\r\n'', file, line) */
 

 

  /* Infinite loop */
 

  while (1)
 

  {
 

  }
 

}
