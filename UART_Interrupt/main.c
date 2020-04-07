#include "stm32f4xx.h"                  // Device header
void delay(int delayms);
void led_play(int value);
void UART2_Init();
void USART2_IRQHandler(void);
void USART2_write(int ch);
char c;
int main ()
{
__disable_irq();
UART2_Init();
GPIOA->MODER|= GPIO_MODER_MODER5_0; //PA5 as output
USART2->CR1|=0x0020;
NVIC_EnableIRQ(USART2_IRQn);
__enable_irq();

while(1)
{
if(c!=' '){
led_play(c);
USART2_write(c);
USART2_write('\r');
USART2_write('\n');	
c=' ';
}
}

}

void led_play(int value)
{
value %=16; 
	for (;value>0;value--){
	GPIOA->BSRR|=GPIO_BSRR_BS5; //turn on ledd
		delay(100);
		GPIOA->BSRR|=GPIO_BSRR_BR5; //turn off the led		
		delay(100);
	}
}

void delay(int delayms){
SysTick->LOAD=16000-1;
SysTick->VAL=0;
SysTick->CTRL=0x5;
for (int i=0;i<delayms;i++)
{
while(!(SysTick->CTRL &0x10000)){}
		}
	SysTick->CTRL=0;
}

void UART2_Init(){
		RCC->AHB1ENR |=1; //Enable GPIOA clock
	RCC->APB1ENR|=0x20000; //Enable USART clock
	//configure PA3 as alternative function 
	GPIOA->AFR[0]=0x07700; //ALT7 for UART2
	GPIOA->AFR[1]=0x07000;
	GPIOA->MODER|=0x0080;  //enable PA3 as alternate function
	GPIOA->MODER|=0x0020; //enable PA2 as alterate fuction
	USART2->BRR=0x008B; // set baud rate of 115200 for 16MHz
	USART2->CR1|=0x200C; //enable RX
	//USART2->CR1|=0x0008; // enable tx
	//USART2->CR1|=0x2000; //enable USART2	
	
	

}



void USART2_IRQHandler(void){

if(USART2->SR&0x0020){
c=USART2->DR;

}
}
void USART2_write(int ch){
	
while(!(USART2->SR&0x0080)){
}
USART2->DR=(ch&0xff);	
}