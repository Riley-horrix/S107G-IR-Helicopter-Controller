# S107G-IR-Helicopter-Controller

This is a short project I undertook after buying a S107G infrared helicopter from Ebay.

In essence it is a controller for a node mcu esp8266, and a connected BBC microbit 
(used for it's accelerometer) along with a few other random breadboard components.
It is designed as a one-hand tilt controller, incorporating all of the components 
in a glove, including the power supply and IR LED. </br>
</br>

## Reverse engineering IR protocol

The S107G has two modes, 38kHz, and 58kHz mode. I used 38kHz mode as it is easier
to achieve on a simple microcontroller.
as 32 'bits' along with a static header. The header consists of a 2ms pulse, then a
2ms low, finally a 380us pulse. 

Each 'bit' of the instruction corresponds to a pulse
and a variable wait, the encoding of a '1' is a 600us low, then a 380us pulse, and
the encoding of a '0' is a 220us low followed by a 380us pulse. The 32 'bit' instruction
sent consists of 4 bytes, (from MSB) a yaw byte, a pitch byte, a throttle byte and 
finally a trim byte. The encodings for each byte are roughly as follows : 

Throttle 0%    : 139 (0x8B)</br>
Throttle 100%  : 255 (0xFF)</br>
Yaw Left       : 8   (0x08)</br>
Yaw Right      : 106 (0x6A)</br>
Pitch Forward  : 1   (0x01)</br>
Pitch Backward : 128 (0x80)</br>

Trim 50% : 52 (0x34)</br>

These results have been collected by myself and 30' on an oscilloscope, but have been corroborated 
by similar results realised by Kerry D. Wong, which has become the basis for the controller.

Here is a link to their website where they have conducted a much more thorough reverse engineer than
me - http://www.kerrywong.com/2012/08/27/reverse-engineering-the-syma-s107g-ir-protocol/.


## Building the controller

When I started the controller design, most of my design choices were made on what spare
parts I had lying around my electronics box, like for example a few buttons, potentiometer and 
BBC microbit, and obviously the node mcu esp8266. The IR diode, I managed to solder off of the helicopter 
controller itself without damaging it. It is a 940nm, 5mm IR LED that seems to be pretty common in most
tv remotes and other widely used IR electronics.

I wanted to be able to continuously control the helicopter on all 3 axis but the limitations
of the microcontroller meant that I had access to only 1 analogue input pin. I figured the best
use of this pin would be to control the throttle, as to get a good resolution to easily find the
hover throttle.

That left me with the task of getting the accelerometer information from the BBC microbit 
(which I had already decided I wanted to use) to the esp, hovever I only had to send the information
about yaw and pitch. I decided on having the microbit read it's x and y axis tilt and send a
3 bit instruction over jumper wire to the digital inputs of the esp. And deciding the various 
configurations on the most common movements : 

000 - No input (hover)</br>
001 - Backwards</br>
010 - Yaw Right</br>
011 - Yaw Left</br>
100 - Forwards + Yaw Right</br>
101 - Forwards + Yaw Left</br>
110 - Forwards</br>
111 - (More) Forwards</br>
</br>

Using the microbit means that I can completely control the helicopter from one hand, after setting a 
hover throttle from the potentiometer.

I also decided that some in-flight throttle control was necessary since it proved to be a challenge
to rotate the throttle potentiometer with one hand, so I added two buttons to the design, one to
increase the throttle by some amount, and the other to take a little bit off of it.

After some breif prototyping, I decided that the following circuit layout was the best for this design.

![S107G-IR-Controller-Circuit.drawio-3.pdf](https://github.com/Riley-horrix/S107G-IR-Helicopter-Controller/files/11941120/S107G-IR-Controller-Circuit.drawio-3.pdf)
</br>

The final design looks like so (along with a swiftly made glove): 

[Full-layout](https://github.com/Riley-horrix/S107G-IR-Helicopter-Controller/assets/81515896/203e1fa6-2ee9-4b91-b18c-a90061dcc02a)
[Hand-mount1](https://github.com/Riley-horrix/S107G-IR-Helicopter-Controller/assets/81515896/dc0f3a60-6d3d-4e72-a8bb-d07755de720c)
[Hand-mount2](https://github.com/Riley-horrix/S107G-IR-Helicopter-Controller/assets/81515896/337dfbb4-ea92-43e0-bf5f-bf6328dc8d17)
</br>


## Refletions and future improvements

### 3 axis continuous input.
The 3 bit resolution for two axis of movement was definitely sub optimal and hard to use.
It could have been better to swap out the throttle and replace the analogue input in the esp as the yaw.
This would be relatively easy as the microbit can deliver a 10 bit resolution analogue output. Then you 
could swap out the throttle offset buttons, for throttle increase / decrease. 

### 


  




