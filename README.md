# PPMEncoder

This library generates a Pule-Position-Signal using interrupts, so you don't have to care about timings in your software! For more details what an PPM signal is, please consult the [english](https://en.wikipedia.org/wiki/Pulse-position_modulation) or the [german](https://de.wikipedia.org/wiki/Puls-Pausen-Modulation) wikipedia article. Take a look at the examples how to use this library - I hope you find it easy to use :)

## Introduction

A simple visual on how a PPM signal works:

![PPM Signal](https://upload.wikimedia.org/wikipedia/commons/6/65/Fernsteuerungsmodulation.gif)<br>
CC-BY-SA 3.0 Christian Wolff

Your value range is from 0,5ms to 1.5ms. This results in timer1 overflow values from 500 to 2500 - which is your range you can use.

## Setup

By default there are *8 Channels* use can use. You can set this value using the template parameter.

```c++
PPMEncoder<> ppmEncoder{}; // default 8 channels
PPMEncoder<4> ppmEncoder{}; // 4 channels
```
For more than 8 channels, the frame length has to increase - which you can set manually with the following constructor.

```c++
PPMEncoder(const uint16_t minValue, const uint16_t maxValue, const uint16_t frameLength, const uint16_t pulseLength)
```
