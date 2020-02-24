# Installment of atmel VisualStudio prject with empty source files

## ISR and Servo interrupt logic


### PWM for engine

The engine shall be controlled by an 50 kHz pwm signal.

This is generated automatically by the atTiny13, if the Pin A of the capture function is used and programmed to be in fast PWM mode.
In this case every zero passing overflow of the timer will set the pin to 1, every compare match will set the pin to zero.

Requirement: just set the control bytes to start fast PWM

### Servo

To implement the slow periodicity of the servo (20 ms period),
the timer overflows are counted and after 13 events, the pin is set to one.
To create the working duty cycles of 0.6...2.2 ms one has to wait 

+ short dc: for a fraction of one overflow,
+ or, longer dc: up to one overflow plus some fraction.

this is realized by a mixture of state machine and Capture for Pin B.
+ If state tonflag is set, the capture is set immediately in the 1 overflow sequence of 
of the timer to the fitting compare value, and capture is activated.
+ If state tonflag is clear, the capture is deactivated in the first timer run, the value 
to program is reduced by 255, 
+ in second overflow run, the capture is activated, and the difference of before is 
set in the capture register.



