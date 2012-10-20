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
 * @file wirish/include/wirish/HardwareCAN.h
 * @author Devan Lai <devan.lai@gmail.com>
 * @brief High-level CAN interface
 *
 */

#ifndef _WIRISH_HARDWARECAN_H_
#define _WIRISH_HARDWARECAN_H_

#include <libmaple/libmaple_types.h>
#include <libmaple/can.h>
#include <wirish/wirish.h>
#include <wirish/boards.h>

typedef struct {
    uint8 rx;
    uint8 tx;
} can_pins;

//TODO: Generalize to boards besides the Maple Mini
static const can_pins board_can_pins[] __FLASH__ = {
    {23, 24},
};

static const can_pins* dev_to_can_pins(can_dev* const dev) {
    switch (dev->clk_id) {
    case RCC_CAN: return board_can_pins;
    default:      return NULL;
    }
};



/**
 * @brief Defines the possible CAN communication speeds.
 */
typedef enum CANFrequency {
    CAN_1MHZ = 0,
    CAN_500KHZ = 1,
    CAN_250KHZ = 2,
    CAN_125KHZ = 3,
} CANFrequency;

#if CYCLES_PER_MICROSECOND != 72
#warning "Unexpected clock speed; CAN frequency calculation will be incorrect"
#endif

typedef enum {
    CAN_STANDARD_ID=0,
    CAN_EXTENDED_ID=1,
} can_id_type;

class CANMessage {
public:
    /**
     * @param id id of the packet - 11 bits for standard, 29 bits for extended
     * @param data pointer to the payload, up to 8 bytes
     * @param len the size of the payload, in bytes
     * @param type whether the packet should treat the id as a standard or extended CAN id
     */
    CANMessage(uint32 id, const uint8 *data, uint8 len, can_id_type type=CAN_STANDARD_ID);

    uint32 id;
    uint8 *data;
    uint8 len;
    can_id_type type;
};

/**
 * @brief Wirish CAN interface.
 *
 */
class HardwareCAN {
public:
    /**
     * @param canPortNumber Number of the CAN port to manage.
     */
    HardwareCAN(uint32 canPortNumber);

    /*
     * Set up/tear down
     */

    /**
     * @brief Turn on a CAN port and configure it for TX FIFO mode.
     *
     * CAN port is enabled, with TX FIFO mode enabled
     *
     * @param frequency Communication frequency
     * @param options master control options, from can_option
     * @param mode CAN debugging modes to use, from can_mode
     */
    void begin(CANFrequency frequency, uint32 options, uint32 mode);

    /**
     * @brief Equivalent to begin(CAN_1MHZ, 0, 0).
     */
    void begin(void);

    /**
     * @brief Disables the CAN port, but leaves its GPIO pin modes unchanged.
     */
    void end(void);

    boolean send(CANMessage *msg, boolean blocking=false);

    uint8 rxPin(void);
    uint8 txPin(void);

private:
    can_dev* can_d;
};

#endif
