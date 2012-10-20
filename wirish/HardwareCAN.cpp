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
#include <wirish/boards.h>
#include <libmaple/stm32.h>

/*
 * General utility functions
 */

//Borrowed from HardwareSPI.cpp
static void disable_pwm(const stm32_pin_info *i) {
    if (i->timer_device) {
        timer_set_mode(i->timer_device, i->timer_channel, TIMER_DISABLED);
    }
}

static void configure_gpios(can_dev *dev) {
    const can_pins *pins = dev_to_can_pins(dev);
    if (!pins) {
        return;
    }
    const stm32_pin_info *rx = &PIN_MAP[pins->rx];
    const stm32_pin_info *tx = &PIN_MAP[pins->tx];

    ASSERT(rx->gpio_device == tx->gpio_device);

    disable_pwm(rx);
    disable_pwm(tx);

    can_config_gpios(dev, rx->gpio_device, tx->gpio_bit, rx->gpio_bit);
}

static void enable_device(can_dev *dev, CANFrequency freq, uint32 options, uint32 ier_options, uint32 mode) {
    /* Using TBS1=4, TBS2=3,
       we get nominal bit timing
       = Tpclk * (BRP+1) * (1+4+3)
       = Tpclk * (BRP+1) * 6
       If the clk is running at 36MHz, then we want
       BRP+1 = 6 to get a 1MHz baud rate.
    */
    
    uint32 target = 1;
    switch (freq) {
        case CAN_1MHZ:
            target = 1000000;
            break;
        case CAN_500KHZ:
            target = 500000;
            break;
        case CAN_250KHZ:
            target = 250000;
            break;
        case CAN_125KHZ:
            target = 125000;
            break;
        default:
            ASSERT(0);
    }

    uint8 SJW = 4 - 1;
    uint8 TS2 = 3 - 1;
    uint8 TS1 = 4 - 1;
    uint16 BRP = ((STM32_PCLK1 / 6) / target) - 1;
    uint32 timing = BRP;

    timing |= TS1 << 16;
    timing |= TS2 << 20;
    timing |= SJW << 24;

    can_init(dev);
    configure_gpios(dev);
    can_reconfigure(dev, options, ier_options, mode | timing);

}

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
    uint32 ier_options = 0;
    enable_device(this->can_d, frequency, options, ier_options, mode);
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
