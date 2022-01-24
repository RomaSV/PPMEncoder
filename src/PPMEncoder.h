#ifndef _PPMEncoder_h_
#define _PPMEncoder_h_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

template<uint8_t numChannels = 8>
class PPMEncoder {

public:
    const uint16_t minValue;
    const uint16_t maxValue;
    const uint16_t pulseLength;
    const uint16_t frameLength;

    PPMEncoder() : minValue(1000), maxValue(3000), pulseLength(500), frameLength(22500) {}

    PPMEncoder(const uint16_t minValue, const uint16_t maxValue, const uint16_t frameLength, const uint16_t pulseLength)
    : minValue(minValue), maxValue(maxValue), pulseLength(pulseLength), frameLength(frameLength) {}


    void setChannel(uint8_t channel, uint16_t value) {
        channels[channel] = constrain(value, minValue, maxValue);
    };

    void setChannelPercent(uint8_t channel, uint8_t percent) {
        percent = constrain(percent, 0, 100);
        setChannel(channel, map(percent, 0, 100, minValue, maxValue));
    };

    void begin(uint8_t pin, boolean inverted = false) {
        cli();

        // Store on/off-State in variable to avoid another if in timing-critical interrupt
        onState = (inverted) ? HIGH : LOW;
        offState = (inverted) ? LOW : HIGH;

        pinMode(pin, OUTPUT);
        digitalWrite(pin, offState);

        state = true;
        elapsedUs = 0;
        currentChannel = 0;

        outputPin = pin;

        for (uint8_t ch = 0; ch < numChannels; ch++) {
            setChannelPercent(ch, 0);
        }

        TCCR1A = 0;

        OCR1A = 100;
        TCCR1B = (1 << WGM12) | (1 << CS11);
        TIMSK1 = (1 << OCIE1A); // enable timer compare interrupt

        sei();
    };

    void interrupt() {
        TCNT1 = 0;

        if (state) {
            digitalWrite(outputPin, onState);
            OCR1A = pulseLength * 2;

        } else {
            digitalWrite(outputPin, offState);

            if (currentChannel >= numChannels) {
                currentChannel = 0;
                elapsedUs = elapsedUs + pulseLength;
                OCR1A = (frameLength - elapsedUs) * 2;
                elapsedUs = 0;
            } else {
                OCR1A = (channels[currentChannel] - pulseLength) * 2;
                elapsedUs = elapsedUs + channels[currentChannel];

                currentChannel++;
            }
        }

        state = !state;
    };

private:
    int16_t channels[numChannels]{};
    uint16_t elapsedUs{};

    uint8_t currentChannel{};
    byte outputPin{};
    boolean state{};

    uint8_t onState{};
    uint8_t offState{};
};

#endif
