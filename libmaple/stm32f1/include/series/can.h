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
 * @file libmaple/stm32f1/include/series/can.h
 * @author Devan Lai <devan.lai@gmail.com>
 * @brief STM32F1 Controller Area Network (CAN) header.
 */

#ifndef _LIBMAPLE_STM32F1_CAN_H_
#define _LIBMAPLE_STM32F1_CAN_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Devices
 */

extern struct can_dev* const CAN1;

/*
 * Register map base pointers
 */

/** STM32F1 CAN1 register map base pointer. */
#define CAN_BASE                       ((struct can_reg_map*)0x40006400)

/*
 * Interrupt handlers
 */
void __irq_can_sce(void);
void __irq_usb_hp_can_tx(void);
void can_rx_irq_handler(void);

#ifdef __cplusplus
}
#endif

#endif
