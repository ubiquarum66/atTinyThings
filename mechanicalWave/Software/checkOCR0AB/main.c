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
// helper variables to keep track of ISR variables in realtime context
volatile int state =0;
volatile int s =0;
volatile unsigned char ton1flag =0;
volatile unsigned char dutyservo =60;



//13 periods of timer overflowca 20 ms = base period of servo control
#define NSKIP 13

ISR(TIM0_OVF_vect) /* veraltet: SIGNAL(SIG_OVERFLOW0) */
{
	//~ here a fast pwm interrupt us realized, and a slow servo base 
	//~ frequency created via a hierarchical overflow counter
	//~ fast pwm is done via OCA function on PinA , no need to do more 
	//~ TOVFL set the pin to ON at each timer overflow, the OCA iinterrupt internally 
	//~ toggles the pin and waits for next OVFL and next register compare match. (FAST-PWM mode)
	//~ slow servo base frequency and duty cycle is created by counting TOVFL events.
	//~	every NSKIP runs, state goes to zero and is incremented then (one base period of servo around 20 ms)
	//~ with each Periode PINB is switched on.
	//~ the allowed on time of servo is either smaller than one TOVL period or
	//~ one TOVL period plus remaining mus. 
	//~ this is realized via the ton1flag.	
	switch(state)
	{
	case 0:
		PORTB|=(1<<PB1);
		if (!ton1flag)
		{
		    TIFR0 = (1<< OCF0B);
		    TIMSK0 |= (1<<OCIE0B);
		    s = TCNT0;
		}
  
	 break;
	case 1:
		if (ton1flag)
		{
		    TIFR0 = (1<< OCF0B);
		    TIMSK0 |= (1<<OCIE0B);
			s = TCNT0;
		}
  	 break;
	case NSKIP:
	    state =-1;
	 break;
	default:
	break;
	}
    state++;	 
	
}

ISR(TIM0_COMPB_vect) /* veraltet: SIGNAL(SIG_OVERFLOW0) */
{
	//~ when this interrupt fires, the remaining part of the servo on cycle has passed.
	//~ 13 rounds of timer overflow took place and OCB was setup to create the remaining ON mus delay.
	//~ the subordination of NSKIP runs and reamining OCB establishes the hierarchical overflow counter
	{
		//switch of portpin B 
		PORTB &=~(1<<PB1);
		//disable OCB
		TIMSK0 &= ~(1<<OCIE0B);
	}
}

void setServoDuties(uint8_t dutyServoAD, uint8_t * c, int8_t * flag)
{
			// now handle Servo: linear transmission of ad value to duty
			// set flag whether more than one TOVFLW is needed:
			*flag = (dutyServoAD > 156)? 1: 0; //signal longer then 1 TOV of TI0
			// series development of multiplications to shifts...1.2 times AD = approx 1 AD + 1/4 AD ...
			*c = 60 + dutyServoAD + (dutyServoAD >> 2);
			//partial or complete remainder for Compare Interrupt OCR0B by uint8_t overflow
			//safety limits.... min 0.4 ms
			if (flag ) {if (*c < 10 ) *c =10;} else {if  (*c<60) *c=60; }
}



int main(void)
{
	unsigned char c;
	int8_t flag;
	uint8_t firstdutyServoAD;
	uint8_t firstdutyEngineAD;
	/* setup */
	// setup pwm mode to create engine pwm
	// setup AD Converters...
	initADC();
	// read ADSC channels for the first time
	// set PWM of engine
	// set pwm parts of servo
	
    /* loop */
	while (1) {
		//~ read ADC and change the duty elemnets of pwms and servos 
		uint8_t dutyEngineAD = (uint8_t)read_ADC10(3);
		uint8_t dutyServoAD = (uint8_t)read_ADC10(2);
		// do mappings and scalings
		// servo requires more then one timer overflow -- non atomic,
		// now handle Servo: linear transmission ad to duty
		setServoDuties(dutyServoAD,&c, &flag);

		// thus protect setup
		// --> start interrupt save consistent transfer of remainder and timer flag
		cli();
		//~ program servo timer settings
		ton1flag = flag;
		OCR0B=dutyservo = c;
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

void initADC()
{
	// we only use internal reference voltage of 1.1V
	//ADMUX |= (1 << REFS0);   
	DIDR0 |= (1<< ADC2D);
	DIDR0 |= (1<< ADC3D);

	//select ADC Channels ;
	ADMUX |= (1 << MUX0); 
	ADMUX |= (1 << MUX1); 
	//only 8 bit needed.... Bit for LeftAdjust ADCH/ ADCL
	ADMUX |= (1 << ADLAR);  
	//enable adc
	ADCSRA |= (1<<ADEN);
	ADCSRA |= (1 << ADPS1);   //with next line…
	ADCSRA |= (1 << ADPS0);   //set division factor-8 for 125kHz ADC clock
	//ADCSRA |= (1 << ADSC);   //start conversion ... not yet
}
uint16_t result = 0;

uint16_t read_ADC10(uint8_t channel)
{
	uint8_t i;
	result = 0;

	//ADCSRA = (1<<ADEN) | (ADPS2) | (ADPS1) | (ADPS0) ;
	ADCSRA |= (1<<ADEN);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADMUX  &= ~3;			//ADC Kanal löschen
	ADMUX  |= (1 << ADLAR) | channel;			//ADC Kanal wählen
	//ADMUX |= REFS0 ;			// Referenzspannung Intern nutzen!
	// erste wandlung dauert laenger, ergebnis ncht so zuverlaessig
	ADCSRA |= (1<<ADSC);		// eine ADC-Wandlung starten
	while ( ADCSRA & (1<<ADSC) ) { ; }
	// fertig und verwerfen...
	// Messen - Mittelwert aus 2^x Wandlungen bilden
	// Anzahl der Messungen kann an eigene Bedürfnisse angepasst werden, hier x = 2 , 4 messungen
	result = 0;				// zuvor noch Inhalt von result löschen
	for(i=0;i<4;i++)
	{
		ADCSRA |= (1<<ADSC);	// eine ADC-Wandlung starten "single conversion"
		while ( ADCSRA & (1<<ADSC) ) { ; }	// auf Abschluss der Konvertierung warten
		result += (uint16_t)ADCH;			// Wandlungsergebnisse dazuaddieren
	}							// ADCW ist eine Kombination aus ADCL und ADCH -> 16Bit
	ADCSRA &= ~(1<<ADEN);		// ADC ausschalten
	result >>= 2;				// Mittelwert bilden , 2^x, deshalb einfaches schieben
	
	return result;  // und Ergebnis zurueck...
}

