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
 * @file libmaple/stm32f1/can.c
 * @author Devan Lai <devan.lai@gmail.com>
 * @brief Controller Area Network (CAN) support.
 */

#include <libmaple/can.h>


/*
 * CAN devices
 */

static can_dev can1 = {
    .regs = CAN_BASE,
    .clk_id = RCC_CAN,
    .tx_irq_num = NVIC_USB_HP_CAN_TX,
    .rx_irq_num = NVIC_USB_LP_CAN_RX0,
    .sc_irq_num = NVIC_CAN_SCE,
    .tx_handler = NULL,
    .rx_handler = NULL,
    .sc_handler = NULL,
    .enabled = 0
};
/** CAN1 device. */
can_dev* const CAN1 = &can1;

/*
 * Interrupt handlers.
 */

void __irq_can_sce(void) {
    if (can1.enabled && can1.sc_handler != NULL) {
        (can1.sc_handler)();
    }
}

void __irq_usb_hp_can_tx(void) {
    if (can1.enabled && can1.tx_handler != NULL) {
        (can1.tx_handler)();
    }
}

void can_rx_irq_handler(void) {
    if (can1.rx_handler != NULL) {
        (can1.rx_handler)();
    }
}
