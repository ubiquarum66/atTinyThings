# First Brainstorming physical-electrical-setup:

The electrical muC setup was very fast decided:

attiny13 as a 2 channel AD Converter, 

+ In: Engine speed of car
+ In: Lever Angle
+ Out: Servo Signal for angle servo
+ Out: PWM to control car speed,
+ Out: counter-stepper to create a LED light show 

The translation pulses to lightshow is done via the DecadeCounter 4017

![And this was the idea (quit close!)](electrical_hardware_setup_1.png)