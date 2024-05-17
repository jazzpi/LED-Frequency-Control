# LED Frequency Control

Simple Arduino Uno program for controlling the frequency of an LED (well,
anything connected to Pin 9).

This uses the ATMEGA328P's 16-bit Timer1 in Compare Match reset/toggle mode in
order to control the PWM frequency, at a fixed duty cycle of 50%.

You can supply the frequency via the serial console (running at 115200 baud) in
Hz as an ASCII string of up to 5 digits, terminated with CRLF. The read
frequency will be echoed back. Set a frequency of 0 to turn off the GPIO.
