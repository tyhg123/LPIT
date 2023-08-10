/* INCLUDE*/
#include "MKE16Z4.h"
/* ===================================================================================== */
/* DEFINE*/
#define DELAY_CNT               (3000000)
#define GREEN_LED_PIN           (1 << 4)
#define RED_LED_PIN             (1 << 5)
#define BLUE_LED_PIN            (1 << 1)
#define SW3                     (1 << 2)
#define SHIFT_SW3 2
#define SW2                     (1 << 3)
#define SHIFT_SW2 3
#define READ_BIT(var, n) (((var) >> (n)) & 1)
/* ===================================================================================== */
/* GLOBLE VARIABLE*/
uint8_t count=0;
/* ===================================================================================== */
/* FUNCION PROTYPE*/
void init();
void LPIT0_IRQHandler();

/* ===================================================================================== */
/* MAIN */

int main(void)
{

    init();
    while (1)
    {

    }
}
/* FUNCION DECLARATION */
/* ===================================================================================== */
void init()
{
    /* Enable Clock for PORTB */
    PCC->CLKCFG[PCC_PORTB_INDEX] |= PCC_CLKCFG_CGC(1);
    PCC->CLKCFG[PCC_PORTD_INDEX] |= PCC_CLKCFG_CGC(1);

    /* Setup PINB4, PINB5 as GPIO */
    PORTB->PCR[4] |= PORT_PCR_MUX(1);
    PORTB->PCR[5] |= PORT_PCR_MUX(1);
    PORTD->PCR[1] |= PORT_PCR_MUX(1);

    PORTD->PCR[2] &= ~ PORT_PCR_MUX(7);
    PORTD->PCR[2] |= PORT_PCR_MUX(1);
    PORTD->PCR[3] &= ~ PORT_PCR_MUX(7);
    PORTD->PCR[3] |= PORT_PCR_MUX(1);
    //External Interrrupt config;
    PORTD->PCR[2] |= PORT_PCR_IRQC(10);
    PORTD->PCR[3] |= PORT_PCR_IRQC(10);

    /* Setup PINB4 as Output Mode */
    FGPIOB->PDDR |= GREEN_LED_PIN | RED_LED_PIN;
    FGPIOD->PDDR |= BLUE_LED_PIN;
    FGPIOD->PDDR &= ~SW3;
    FGPIOD->PDDR &= ~SW2;

    /* Set default value for PINB4 = HIGH, PINB5 = LOW */
    //FGPIOB->PDOR &= ~RED_LED_PIN;
    FGPIOB->PDOR |= RED_LED_PIN;
    FGPIOB->PDOR |= GREEN_LED_PIN;
    FGPIOD->PDOR |= BLUE_LED_PIN;

    //SCG config
    SCG->FIRCDIV |= (1<<8);

////    //PCC config
    PCC->CLKCFG[PCC_LPIT0_INDEX] = (3<<24); // option clock
    PCC->CLKCFG[PCC_LPIT0_INDEX] |= (1<<30); // gate


//    // LPIT config
//    //CLOCK_EnableClock();
    LPIT0->MCR|= LPIT_MCR_SW_RST_MASK;
    LPIT0->MCR &= ~LPIT_MCR_SW_RST_MASK;
    LPIT0->MCR |= (LPIT_MCR_DBG_EN(1) | LPIT_MCR_DOZE_EN(1) | LPIT_MCR_M_CEN_MASK);
    LPIT0->CHANNEL[0].TCTRL |= LPIT_TCTRL_MODE(0);
    LPIT0->CHANNEL[1].TCTRL |= LPIT_TCTRL_MODE(0);


    LPIT0->CHANNEL[1].TCTRL |= LPIT_TCTRL_CHAIN(1);

    LPIT0->CHANNEL->TVAL = 47999999U;
    LPIT0->CHANNEL[1].TVAL = 3U;

//    LPIT0->MIER |= LPIT_MIER_TIE0_MASK;
    NVIC_EnableIRQ(LPIT0_IRQn);
    LPIT0->SETTEN |= LPIT_SETTEN_SET_T_EN_0_MASK;

    LPIT0->MIER |= LPIT_MIER_TIE1_MASK;

    LPIT0->SETTEN |= LPIT_SETTEN_SET_T_EN_1_MASK;

}
// oke oik
void LPIT0_IRQHandler()
{
	GPIOB->PTOR |= RED_LED_PIN;
	LPIT0->MSR |= LPIT_MSR_TIF0(1);
}


