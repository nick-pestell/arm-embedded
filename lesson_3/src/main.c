#include "main.h"
#include <stdio.h>

int main(void) {
	// Enable GPIOC and GPIOB peripheral in RCC_AHB1ENR.
	// GPIOA and B are on bus AHB1 (see pg57 of refference manual)
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; //RCC is a pointer to a structure. RCC->member is specific syntac for defrencing (getting the value) of the member of the structure that RCC points to. (see k&r pg131)
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// Configure button input GPIO for input
	GPIOE->MODER &= ~(0x3 << (BUTTON_PIN_INPUT*2));// set the bits that represent button pin (2*button_pin and 2*button_pin +1) to 00

	// Configure button input GPIO with pull down resistor
	GPIOE->PUPDR &= ~(0x3 << (BUTTON_PIN_INPUT*2));
	GPIOE->PUPDR |= ~(0x1 << (BUTTON_PIN_INPUT*2));



	// Configure button power GPIO for output
	GPIOA->MODER &= ~(0x3 << (BUTTON_PIN_POWER*2));
	GPIOA->MODER |= (0x1 << (BUTTON_PIN_POWER*2));

	// Configure button power GPIO as push/pull type
	GPIOA->OTYPER &= ~(1 << (BUTTON_PIN_POWER)); //set the bit that represents led1 to 0 for push/pull type

	// Pull button power GPIO pin high by setting the relevant bit in the output data register to high (1)
	GPIOA->ODR |= (1 << BUTTON_PIN_POWER);


	
	// Configure led1 GPIO for output
	GPIOB->MODER &= ~(0x3 << (LED_PIN*2));
	GPIOB->MODER |=  (0x1 << (LED_PIN*2));// set the bits that represent the led_pin (2*led_pin and 2*led_pin+1) to 01. To represent 01, i.e. 2 bits we do ~0x2, 0x2 is 10 in binary.

	// Configure led1 GPIO as push/pull type
	GPIOB->OTYPER &= ~(1 << (LED_PIN)); //set the bit that represents led1 to 0 for push/pull type

	// Pull LED1 pin low by setting the relevant bit in the output data register to high (1)
	GPIOB->ODR |= (0 << LED_PIN);

	uint8_t button_down = 0;
	uint32_t gpioa_input;
	while(1){
		// Read status of each GPIOC pin by reading GPIOC peripherals input data register
		// gpioa_input = GPIOA->IDR;
		gpioa_input = GPIOE->IDR;
		// check if button is down
		// if ((gpioa_input | (0 << BUTTON_PIN)) == 0){
		if (gpioa_input & (1 << BUTTON_PIN_INPUT)) {
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
