#include "main.h"
#include <stdio.h>

int main(void) {
	// Enable GPIOA and GPIOB peripheral in RCC_AHB1ENR.
	// GPIOA and B are on bus AHB1 (see pg57 of refference manual)
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; //RCC is a pointer to a structure. RCC->member is specific syntac for defrencing (getting the value) of the member of the structure that RCC points to. (see k&r pg131)
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	// Configure button GPIO for input
	GPIOC->MODER &= ~(0x3 << (BUTTON_PIN*2));// set the bits that represent button pin (2*button_pin and 2*button_pin +1) to 00

	// Configure button GPIO with pull up resistor
	GPIOC->PUPDR &= ~(0x3 << (BUTTON_PIN*2));
	GPIOC->PUPDR |= ~(0x2 << (BUTTON_PIN*2));

	
	// Configure led1 GPIO for output
	GPIOB->MODER &= ~(0x3 << (LED_PIN*2));
	GPIOB->MODER |= ~(0x2 << (LED_PIN*2));// set the bits that represent the led_pin (2*led_pin and 2*led_pin+1) to 01. To represent 01, i.e. 2 bits we do ~0x2, 0x2 is 10 in binary.

	// Configure led1 GPIO as push/pull type
	GPIOB->OTYPER &= (0 << (LED_PIN*2)); //set the bit that represents led1 to 0 for push/pull type


	// Pull LED1 pin low by setting the relevant bit in the output data register to high (1)
	GPIOB->ODR |= (1 << LED_PIN);

	uint8_t button_down = 0;
	uint32_t gpioa_input;
	while(1){
		// Read status of each GPIOA pin by reading GPIOA peripherals input data register
		// gpioa_input = GPIOA->IDR;
		gpioa_input = ~GPIOC->IDR;
		// check if button is down
		// if ((gpioa_input | (0 << BUTTON_PIN)) == 0){
		if (gpioa_input & (1 << BUTTON_PIN)) {
			if (!button_down){
				GPIOB->ODR ^= (1 << LED_PIN); //turn on/off LED1
			}
			button_down = 1;
		}
		else{
			button_down = 0;
		}
	}

}
