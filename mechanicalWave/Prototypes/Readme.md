# Mechatronic components

## Lever "Doppelschwinge"

Theorie shows, that if one wnats to transform the rotaton movement of the servo into
a long extended vertical movement, the 
constructors simplets choice could be the (german) Doppelschwinge; 

+ three branches connected by 
+ 3 passiv axles and containig 
+ one driving torque (Servo).

If the passive, pulling, branch of the two side branches is a little bit longer than the one connected 
to the servo it ill straighten the lever during it's turning and thus creating a nice linear movement.

![](doppelschwinge.png)


## Backgorund Picture

To hint to the juggling hobby we decided to create a clown's picture as background and 
provide the rotating juggling objects with a running light chain of 10 LED's.


![](clown_design.png)

## Running LED Chain:

10 wires coming from decade counter, always only one
switched on, thus one backflow wire carries the 
necessary Resistor. Stabilizing concept by old CD.


![](led_lichtkreis.png)


## arduino atttiny setup to develop software

Arduina is only providing power, as programming is 
done via a mkII compatible ISP Interface to Atmel 
Studio.

![](aufbau.png)