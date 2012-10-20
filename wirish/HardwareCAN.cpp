/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2012 Devan Lai.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file HardwareCAN.cpp
 * @author Devan Lai <devan.lai@gmail.com>
 * @brief Wirish CAN implementation.
 */

#include <wirish/HardwareCAN.h>

/*
 * Constructor
 */

HardwareCAN::HardwareCAN(uint32 canPortNumber) {
    switch (canPortNumber) {
    case 1:
        this->can_d = CAN1;
        break;
    default:
        ASSERT(0);
    }
}

/*
 * Set up/tear down
 */

void HardwareCAN::begin(CANFrequency frequency, uint32 options, uint32 mode) {
    options |= CAN_TX_FIFO;
    mode |= CAN_MODE_LOOPBACK | CAN_MODE_SILENT;
    //TODO: compute real BTR values for each frequency
    /* Using the default TS1=2, TS2=3,
       we get nominal bit timing
       = Tpclk * (BRP+1) * (1+TS1+1+TS2+1)
       = Tpclk * (BRP+1) * 8
       If the clk is running at 72MHz, then we want
       BRP = 8 to get a 1MHz baud rate.
    */
    uint32 prescaler = 8;
    uint32 timing = 0x01230000 | prescaler;
    can_init(this->can_d);
    can_reconfigure(this->can_d, options, 0, mode | timing);
}

void HardwareCAN::begin(void) {
    this->begin(CAN_1MHZ, 0, 0);
}

void HardwareCAN::end(void) {
    //TODO: actually shutdown everything
    return;
}

boolean HardwareCAN::send(CANMessage *msg, boolean blocking) {
    do {
        ;
    } while (blocking);
    return false;
};

uint8 HardwareCAN::rxPin(void) {
    return dev_to_can_pins(this->can_d)->rx;
}

uint8 HardwareCAN::txPin(void) {
    return dev_to_can_pins(this->can_d)->tx;
}
