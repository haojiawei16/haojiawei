#include "LPC11XX.H"


#define LED1_ON() (LPC_GPIO2->DATA&=~(1<<0))
#define LED1_OFF() (LPC_GPIO2->DATA|=(1<<0))
#define LED2_ON() (LPC_GPIO2->DATA&=~(1<<1))
#define LED2_OFF() (LPC_GPIO2->DATA|=(1<<1))

#define KEY1_DOWN() ((LPC_GPIO3->DATA&(1<<0))!=(1<<0))
#define KEY2_DOWN() ((LPC_GPIO3->DATA&(1<<1))!=(1<<1))

void LedInit(void)
{
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<16);
	LPC_IOCON->PIO2_0 &=~0x07;
	LPC_IOCON->PIO2_0 |=0x00;
	LPC_IOCON->PIO2_1 &=~0x07;
	LPC_IOCON->PIO2_1 |=0x00;
	LPC_SYSCON->SYSAHBCLKCTRL &=(1<<16);
	LPC_SYSCON->SYSAHBCLKCTRL |=(1<<6);
	LPC_GPIO2->DIR |=(1<<0);
	LPC_GPIO2->DATA |=(1<<0);
	LPC_GPIO2->DIR |=(1<<1);
	LPC_GPIO2->DATA |=(1<<1);
	
	LPC_IOCON->PIO3_0&=~(0x07);
	LPC_GPIO3->DIR  &=~(1<<0);
	LPC_IOCON->PIO3_1&=~(0x07);
	LPC_GPIO3->DIR &=~(1<<1);
}

void CT32B1_Init(uint32_t interval)
{
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<16);
	LPC_IOCON->R_PIO1_2&=~(0x07);
	LPC_IOCON->R_PIO1_2|=0x03;
	LPC_SYSCON->SYSAHBCLKCTRL&=~(1<<16);
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<10);
	LPC_TMR32B1->TCR=0x02;
	LPC_TMR32B1->PR=0;
	LPC_TMR32B1->MCR=0x02<<9;
	LPC_TMR32B1->PWMC=0x02;
	LPC_TMR32B1->MR1=interval/2;
	LPC_TMR32B1->MR3=interval;
	LPC_TMR32B1->TCR=0x01;
}
int main()
{
	CT32B1_Init(SystemCoreClock/1000);
	while(1);
}
void PIOINT3_IRQHandler()
{
	if((LPC_GPIO3->MIS&(1<<0))==(1<<0))
	{
		LPC_TMR32B1->MR1=LPC_TMR32B1->MR1*1.1;
		LPC_GPIO3->IC=(1<<0);
	}
	if((LPC_GPIO3->MIS&(1<<1))==(1<<1))
	{
		LPC_TMR32B1->MR1=LPC_TMR32B1->MR1*0.9;
		LPC_GPIO3->IC=(1<<1);
	}
}