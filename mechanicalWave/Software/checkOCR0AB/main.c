#define F_CPU 1200000
/*
 * checkOCR0AB.c
 *
 * Created: 22.05.2019 18:55:36
 * Author : Thomas
 
*/


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define N_1    (_BV(CS00))
#define N_8    (_BV(CS01))
#define N_64   (_BV(CS01)|_BV(CS00))
#define N_256  (_BV(CS02))
#define N_1024 (_BV(CS02)|_BV(CS00))


// delay for main lop to trigger the led chain
void Delay(unsigned int deli);

// setup ADC for two inputs: speed and servo
void initADC();

// helper: read and average 2 ADC inputs
uint16_t read_ADC10(uint8_t channel);
// helper: read and average 2 ADC inputs
void setServoDuties(uint8_t dutyServoAD, uint8_t * c, int8_t * flag);

#define NSKIP 13

ISR(TIM0_OVF_vect) /* veraltet: SIGNAL(SIG_OVERFLOW0) */
{
	//~ here a fast pwm interrupt us realized, and a slow servo base 
	//~ frequency created via a hierarchical overflow counter
	
}

ISR(TIM0_COMPB_vect) /* veraltet: SIGNAL(SIG_OVERFLOW0) */
{
	//~ here a fast pwm interrupt us realized, and a slow servo base 
	//~ frequency created via a hierarchical overflow counter
}




int main(void)
{
	unsigned char c;
	int8_t flag;
	uint8_t firstdutyServoAD;
	uint8_t firstdutyEngineAD;
	/* setup */
	// setup AD Converters...
	// setup pwm mode to create engine pwm
	// read ADSC channels for the first time
	// set PWM of engine
	// set pwm parts of servo
	
    /* loop */
	while (1) {
		//~ read ADC and change the duty elemnets of pwms and servos 
		// do mappings and sclings
		// servo requires more then one timer overflow -- non atomic,
		// thus protect setup
		// --> start interrupt save consistent transfer of remainder and timer flag
		cli();
		//~ program servo timer settings
		sei();
		// <-- end of interrupt save consistent transfer of remainder and timer flag
		// main path: the interrupts are preemptive, so this can be as slow as possible
		PORTB |= (1<<PB2);    //Blinkenlights: set LED counter trigger active
		Delay(5000);           //wait for certain ..arbitrary length ... 
		PORTB &= ~(1<<PB2);   //reset LED counter trigger and pass some time near sleeping
		Delay(2000);
	}
	pwm_stop();  // never to be reached :-)
	
}

