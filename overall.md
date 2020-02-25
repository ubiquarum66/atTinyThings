# atTinyThings
small projects for AtTiny Atmel microcontrollers

## Mechanical Wave Game


a small game, made as a present;

played by avoiding to touch the bent wiring with a electrode by controling speed, direction of a car and angle of a lever.
The lever carrying a tip electrode ring connected to a buzzer.

+ attiny13
+ model servo
+ model gearbox DC engine
+ L293D engine driver
+ 4017 decade counter
+ some capacitors, 
+ one transistor and 
+ 1 switch, 2 Potentiometers, a buzzer

![](product.png)

## Design 

during meeting Lunch Break September 2019

we need a present for our colleague .. he leaves our business unit
and we want to honor his work.

Brainstorming brought the idea of a small car , drivinge left to right and vice versa,
with a lever, that carries a electrode. 

Driving and angle of lever have to be controlled manually to not touch bended
wire with a ring at the top of lever.

As he likes to juggle, a juggling clown will be the background.

![And this was the Idea (quit close!)](first_design_RS.png).

This is a History and Repository of Project Mechanical Wave

![](product.png)

## Content

+ Design How all come to existence and evolved on paper
+ Hardware First checks with crude hardware, mostly circuitry
+ Prototypes more elaborated hardware studies, including mechatronics
+ Software ..and the way of the software

## Software


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


## Hardware 

### First Brainstorming physical-electrical-setup:

The electrical muC setup was very fast decided:

attiny13 as a 2 channel AD Converter, 

+ In: Engine speed of car
+ In: Lever Angle
+ Out: Servo Signal for angle servo
+ Out: PWM to control car speed,
+ Out: counter-stepper to create a LED light show 

The translation pulses to lightshow is done via the DecadeCounter 4017

![And this was the idea (quit close!)](electrical_hardware_setup_1.png)

### Layout and Autorouting :-)

The prototype pcb and a small SVG printout allowed for the mighty manual autorouting 
with artificial brain intelligence, paper and pencil:

![](pcb_and_autolayer.png)

leading to the first soldered test product: left lower: 4017 decade counter, right lower power ic for engine control, upper right atTiny13 microcontroller.
in the middle, the transistor for the non-muC direct direction reversal at the power ic.

![](first_soldering)

### the finished 'product'

after application of denoising capacitors at the engine, the final build of 
the control unit is shown in all its ugliness :-)

![](buildup.png)

### the final layout

and its professional svg layout:

![](endlayout.png)

That's it!

## Prototypes (Mechatronic components)

### Lever "Doppelschwinge"

Theorie shows, that if one wnats to transform the rotaton movement of the servo into
a long extended vertical movement, the 
constructors simplets choice could be the (german) Doppelschwinge; 

+ three branches connected by 
+ 3 passiv axles and containig 
+ one driving torque (Servo).

 test bech control board is just a piece of wood, holding the direction switch, and the two potentiometers, and for the short
 prototype board a Control Unit was made out of colored wood.
 If the passive, pulling, branch of the two side branches is a little bit longer than the one connected 
to the servo it ill straighten the lever during it's turning and thus creating a nice linear movement.

![](doppelschwinge.png)


### Backgorund Picture

To hint to the juggling hobby we decided to create a clown's picture as background and 
provide the rotating juggling objects with a running light chain of 10 LED's.


![](clown_design.png)

### Running LED Chain:

10 wires coming from decade counter, always only one
switched on, thus one backflow wire carries the 
necessary Resistor. Stabilizing concept by old CD.


![](led_lichtkreis.png)


### arduino atttiny setup to develop software

Arduino is only providing power, as programming is 
done via a mkII compatible ISP Interface to Atmel 
Studio.

![](aufbau.png)

### First Mockup:

together with a small wooden car, a bunch of drinking straws, a yellow small gearbox engine,
and glue a first prototype came into play:


![](car.png)
![](all_together_crude.png)

### Elektrodes for buzzing error sound

out of brass tubing, two rings were cut by a lathe to serve as elektrodes at the tip of the lever

![](rings_lathe.png)![](rings_done.png)

### Put it all together 

LED's and Oil painting were combined to create juggling clown background

![](clown_and_wiring.png)

and mounted on a wood shingle, the first functional prototype came into life:

![](test_with_clown.png)

remining issues were to transfer this to a nice plate of wood, replace the lever by a 3d print part, 
and hide the wiring harness in some tubing....


