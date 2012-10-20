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
 * @file libmaple/can.c
 * @author Devan Lai <devan.lai@gmail.com>
 * @brief Controller Area Network (CAN) routines.
 */

#include <libmaple/can.h>

/**
 * @brief Initialize a CAN peripheral.
 *
 * Initializes the RCC clock line for the given peripheral.  Resets
 * CAN device registers.
 *
 * @param dev CAN peripheral to initialize
 */
void can_init(can_dev* const dev) {
    rcc_clk_enable(dev->clk_id);
    rcc_reset_dev(dev->clk_id);
}

/**
 * @brief Attaches an interrupt handler for the CAN peripheral
 *
 * Attaches an interrupt handler for the CAN peripheral.
 * Note: for the RX handler, this must be called after disabling USB
 */
void can_attach_interrupt(can_dev* const dev, can_interrupt_type interrupt_type, void (*handler)(void)) {
    switch (interrupt_type) {
        case CAN_INT_TX:
            dev->tx_handler = handler;
            nvic_irq_enable(dev->tx_irq_num);
            break;
        case CAN_INT_RX:
            dev->rx_handler = handler;
            nvic_irq_enable(dev->rx_irq_num);
            break;
        case CAN_INT_SC:
            dev->sc_handler = handler;
            nvic_irq_enable(dev->sc_irq_num);
            break;
        default:
            return;
    }
}

/**
 * @brief Detaches an interrupt handler from the CAN peripheral
 *
 * Detaches an interrupt handler from the CAN peripheral.
 */
void can_detach_interrupt(can_dev* const dev, can_interrupt_type interrupt_type) {
    switch (interrupt_type) {
        case CAN_INT_TX:
            dev->tx_handler = NULL;
            nvic_irq_disable(dev->tx_irq_num);
            break;
        case CAN_INT_RX:
            dev->rx_handler = NULL;
            nvic_irq_disable(dev->rx_irq_num);
            break;
        case CAN_INT_SC:
            dev->sc_handler = NULL;
            nvic_irq_disable(dev->sc_irq_num);
            break;
        default:
            return;
    }
}

/**
 * @brief Configure AFIO on TX/RX pins to connect GPIO pins to the CAN controller.
 * param dev      CAN peripheral to configure AFIO pins for (this is secretly ignored)
 * param comm_dev GPIO port to configure AFIO on
 * param tx_bit   bit position within the port for TX
 * param rx_bit   bit position within the port for RX
 */
void can_config_gpios(can_dev* const dev, gpio_dev* comm_dev, uint8 tx_bit, uint8 rx_bit) {
    gpio_set_mode(comm_dev, tx_bit, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(comm_dev, rx_bit, GPIO_INPUT_FLOATING);
}

/**
 * @brief Reconfigure the CAN peripheral's MCR, IER, and BTR registers
 *
 * Reconfigures the CAN peripheral's MCR, IER and BTR registers. This
 * will temporarily leave normal mode to reconfigure registers
 * @param dev CAN peripheral to reconfigure
 */
void can_reconfigure(can_dev* const dev, uint32 mcr_config, uint32 ier_config, uint32 btr_config) {
    can_enter_initialization(dev);
    can_leave_sleep(dev);
    while (!can_is_initializing(dev))
        ;
    //Preserve reserved/request bits when setting
    //configuration bits
    dev->regs->MCR &= ~CAN_MCR_CONFIG_MASK;
    dev->regs->MCR |= (CAN_MCR_CONFIG_MASK & mcr_config);
    //dev->regs->IER &= ~CAN_IER_CONFIG_MASK;
    //dev->regs->IER |= (CAN_IER_CONFIG_MASK & ier_config);

    dev->regs->BTR &= ~CAN_BTR_CONFIG_MASK;
    dev->regs->BTR |= (CAN_BTR_CONFIG_MASK & btr_config);
    //can_leave_initialization(dev);
    //while (can_is_initializing(dev))
        ;

}

uint8 can_tx_mailbox_free(can_dev* const dev, can_tx_mailbox mailbox) {
    switch (mailbox) {
        case CAN_TX_MAILBOX_1:
            return bb_peri_get_bit(&dev->regs->TSR, CAN_TSR_TME0_BIT) != 0;
        case CAN_TX_MAILBOX_2:
            return bb_peri_get_bit(&dev->regs->TSR, CAN_TSR_TME1_BIT) != 0;
        case CAN_TX_MAILBOX_3:
            return bb_peri_get_bit(&dev->regs->TSR, CAN_TSR_TME2_BIT) != 0;
        default:
            ASSERT(0);
            return 0;
    }
}

