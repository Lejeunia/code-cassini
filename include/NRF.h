#ifndef NRF_H
#define NRF_H

#include "config.h"
#include <Arduino.h>
#include <RF24.h>
#include <SPI.h>

#define NRF_PA_LEVEL        RF24_PA_MAX
#define NRF_CMD_BUFFER_SIZE 256

class NRF_Comm {
public:
    NRF_Comm(uint8_t cePin, uint8_t csnPin);

    bool begin();
    bool update();
    bool hasCommand() const;
    String readCommand();

    void printDetails()    { _radio.printDetails(); }
    bool radioAvailable()  { return _radio.available(); }

private:
    RF24 _radio;

    char    _rawBuf[NRF_CMD_BUFFER_SIZE];
    int     _rawLen;

    String  _cmdQueue[16];
    uint8_t _cmdHead;
    uint8_t _cmdTail;
    uint8_t _cmdCount;

    void _parseBuffer();
    void _enqueueCommand(const char* start, int len);
};

#endif