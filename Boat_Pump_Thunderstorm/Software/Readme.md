# Software setup for the AtTiny13 

## ISR and Servo interrupt logic

Servo is realized via a repeated TimerOverflow.

To implement the slow periodicity of the servo (20 ms period), the timer overflows are counted and after 13 events, the pin is set to one. To create the working duty cycles of 0.6...2.2 ms one has to wait

+ for short dc: for a fraction of one overflow,
+ or, longer dc: up to one overflow plus some fraction.

this is realized by a mixture of state machine and Capture for Pin B.

+  If state tonflag is set, the capture is set immediately in the 1 overflow sequence of of the timer to the fitting compare value, and capture is activated.
+  If state tonflag is clear, the capture is deactivated in the first timer run, the value to program is reduced by 255,
+  in second overflow run, the capture is activated, and the difference of before is set in the capture register.

Due to the final number of clock tics needed to evaluate things in the ISR, there is a small forbidden zone of pwm durations around the full overflow duration.
The OCR must not be set too small, as it will trigger while the calculations are done. This leads to a visible plateau in the servo sweeps, but is assumed to be not important :-)

For a more sensible application, brain has to be invested.


## Morse signal generation

Due to the limited RAM Space the signal code is fetched from two ROM arrays. The Bytes of a apir contain

+ the bit pattern, and 
+ the number of valid bits
 
### Bit pattern to signal

+ A 1 in the bit pattern is a long morse "toot" , a 0 is a short "peep" , thus, e.g. 0101 is "peep toot peep toot", 
+ the pauses between the bits are automatic, 
+ the pauses between the letters are automatic,
+ the pauses between words and sentences are requested by pause commands (len>14)
+ the power of the muC managed to go directly to 3.3V speaker in the breadboard, but failed in the soldered circuitry,
+ thus I designed it out, bad pitty....half the fun, should have done a transistor circuit before.
+ 


