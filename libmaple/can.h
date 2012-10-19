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
 * @file can.h
 * @author Devan Lai <devan.lai@gmail.com>
 * @brief Controller Area Network (CAN) support.
 */

#ifndef _CAN_H_
#define _CAN_H_

#include "libmaple_types.h"
#include "bitband.h"
#include "rcc.h"
#include "nvic.h"
#include "gpio.h"
#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Register maps
 */

/** CAN register map type. */
typedef struct can_reg_map {
    __io uint32 MCR;            /**< Master control register */
    __io uint32 MSR;            /**< Master status register */
    __io uint32 TSR;            /**< Transmit status register */
    __io uint32 RF0R;           /**< Receive FIFO 0 register */
    __io uint32 RF1R;           /**< Receive FIFO 1 register */
    __io uint32 IER;            /**< Interrupt enable register */
    __io uint32 ESR;            /**< Error status register */
    __io uint32 BTR;            /**< Bit timing register */
    
    const uint8 RESERVED1[352]; /**< Reserved. */

    __io uint32 TI0R;           /**< TX mailbox identifier register */
    __io uint32 TDT0R;          /**< Mailbox data length control and time stamp register */
    __io uint32 TDL0R;          /**< Mailbox data low register */
    __io uint32 TDH0R;          /**< Mailbox data high register */

    __io uint32 TI1R;           /**< TX mailbox identifier register */
    __io uint32 TDT1R;          /**< Mailbox data length control and time stamp register */
    __io uint32 TDL1R;          /**< Mailbox data low register */
    __io uint32 TDH1R;          /**< Mailbox data high register */

    __io uint32 TI2R;           /**< TX mailbox identifier register */
    __io uint32 TDT2R;          /**< Mailbox data length control and time stamp register */
    __io uint32 TDL2R;          /**< Mailbox data low register */
    __io uint32 TDH2R;          /**< Mailbox data high register */

    __io uint32 RI0R;           /**< Receive FIFO mailbox identifier register */
    __io uint32 RDT0R;          /**< Receive FIFO mailbox data length control and time stamp register */
    __io uint32 RDL0R;          /**< Receive FIFO mailbox data low register */
    __io uint32 RDH0R;          /**< Receive FIFO mailbox data high register */

    __io uint32 RI1R;           /**< Receive FIFO mailbox identifier register */
    __io uint32 RDT1R;          /**< Receive FIFO mailbox data length control and time stamp register */
    __io uint32 RDL1R;          /**< Receive FIFO mailbox data low register */
    __io uint32 RDH1R;          /**< Receive FIFO mailbox data high register */

    const uint8 RESERVED2[48];  /**< Reserved. */

    __io uint32 FMR;            /**< Filter master register */
    __io uint32 FM1R;           /**< Filter mode register */
    __io uint32 FS1R;           /**< Filter scale register */
    __io uint32 FFA1R;          /**< Filter FIFO assignment register */
    __io uint32 FA1R;           /**< Filter activation register */

    const uint32 RESERVED3;     /**< Reserved. */
    const uint8 RESERVED4[28];  /**< Reserved. */

    __io uint32 F0R1;           /**< Filter bank 0 register 1 */
    __io uint32 F0R2;           /**< Filter bank 0 register 2 */
    __io uint32 F1R1;           /**< Filter bank 1 register 1 */
    __io uint32 F1R2;           /**< Filter bank 1 register 2 */
    __io uint32 F2R1;           /**< Filter bank 2 register 1 */
    __io uint32 F2R2;           /**< Filter bank 2 register 2 */
    __io uint32 F3R1;           /**< Filter bank 3 register 1 */
    __io uint32 F3R2;           /**< Filter bank 3 register 2 */
    __io uint32 F4R1;           /**< Filter bank 4 register 1 */
    __io uint32 F4R2;           /**< Filter bank 4 register 2 */
    __io uint32 F5R1;           /**< Filter bank 5 register 1 */
    __io uint32 F5R2;           /**< Filter bank 5 register 2 */
    __io uint32 F6R1;           /**< Filter bank 6 register 1 */
    __io uint32 F6R2;           /**< Filter bank 6 register 2 */
    __io uint32 F7R1;           /**< Filter bank 7 register 1 */
    __io uint32 F7R2;           /**< Filter bank 7 register 2 */
    __io uint32 F8R1;           /**< Filter bank 8 register 1 */
    __io uint32 F8R2;           /**< Filter bank 8 register 2 */
    __io uint32 F9R1;           /**< Filter bank 9 register 1 */
    __io uint32 F9R2;           /**< Filter bank 9 register 2 */
    __io uint32 F10R1;          /**< Filter bank 10 register 1 */
    __io uint32 F10R2;          /**< Filter bank 10 register 2 */
    __io uint32 F11R1;          /**< Filter bank 11 register 1 */
    __io uint32 F11R2;          /**< Filter bank 11 register 2 */
    __io uint32 F12R1;          /**< Filter bank 12 register 1 */
    __io uint32 F12R2;          /**< Filter bank 12 register 2 */
    __io uint32 F13R1;          /**< Filter bank 13 register 1 */
    __io uint32 F13R2;          /**< Filter bank 13 register 2 */
} can_reg_map;

/** CAN register map base pointer */
#define CAN_BASE                       ((struct can_reg_map*)0x40006400)

/*
 * Register bit definitions
 */

/* Master control register */

#define CAN_MCR_DBF_BIT         16
#define CAN_MCR_RESET_BIT       15
#define CAN_MCR_TTCM_BIT        7
#define CAN_MCR_ABOM_BIT        6
#define CAN_MCR_AWUM_BIT        5
#define CAN_MCR_NART_BIT        4
#define CAN_MCR_RFLM_BIT        3
#define CAN_MCR_TXFP_BIT        2
#define CAN_MCR_SLEEP_BIT       1
#define CAN_MCR_INRQ_BIT        0

#define CAN_MCR_DBF             BIT(CAN_MCR_DBF_BIT)
#define CAN_MCR_RESET           BIT(CAN_MCR_RESET_BIT)
#define CAN_MCR_TTCM            BIT(CAN_MCR_TTCM_BIT)
#define CAN_MCR_ABOM            BIT(CAN_MCR_ABOM_BIT)
#define CAN_MCR_AWUM            BIT(CAN_MCR_AWUM_BIT)
#define CAN_MCR_NART            BIT(CAN_MCR_NART_BIT)
#define CAN_MCR_RFLM            BIT(CAN_MCR_RFLM_BIT)
#define CAN_MCR_TXFP            BIT(CAN_MCR_TXFP_BIT)
#define CAN_MCR_SLEEP           BIT(CAN_MCR_SLEEP_BIT)
#define CAN_MCR_INRQ            BIT(CAN_MCR_INRQ_BIT)

#define CAN_MSR_RX_BIT          11
#define CAN_MSR_SAMP_BIT        10
#define CAN_MSR_RXM_BIT         9
#define CAN_MSR_TXM_BIT         8
#define CAN_MSR_SLAKI_BIT       4
#define CAN_MSR_WKUI_BIT        3
#define CAN_MSR_ERRI_BIT        2
#define CAN_MSR_SLAK_BIT        1
#define CAN_MSR_INAK_BIT        0

#define CAN_MSR_RX              BIT(CAN_MSR_RX_BIT)
#define CAN_MSR_SAMP            BIT(CAN_MSR_SAMP_BIT)
#define CAN_MSR_RXM             BIT(CAN_MSR_RXM_BIT)
#define CAN_MSR_TXM             BIT(CAN_MSR_TXM_BIT)
#define CAN_MSR_SLAKI           BIT(CAN_MSR_SLAKI_BIT)
#define CAN_MSR_WKUI            BIT(CAN_MSR_WKUI_BIT)
#define CAN_MSR_ERRI            BIT(CAN_MSR_ERRI_BIT)
#define CAN_MSR_SLAK            BIT(CAN_MSR_SLAK_BIT)
#define CAN_MSR_INAK            BIT(CAN_MSR_INAK_BIT)

/* Transmit status register */

#define CAN_TSR_LOW2_BIT        31
#define CAN_TSR_LOW1_BIT        30
#define CAN_TSR_LOW0_BIT        29
#define CAN_TSR_TME2_BIT        28
#define CAN_TSR_TME1_BIT        27
#define CAN_TSR_TME0_BIT        26
#define CAN_TSR_ABRQ2_BIT       23
#define CAN_TSR_TERR2_BIT       19
#define CAN_TSR_ALST2_BIT       18
#define CAN_TSR_TXOK2_BIT       17
#define CAN_TSR_RQCP2_BIT       16
#define CAN_TSR_ABRQ1_BIT       15
#define CAN_TSR_TERR1_BIT       11
#define CAN_TSR_ALST1_BIT       10
#define CAN_TSR_TXOK1_BIT       9
#define CAN_TSR_RQCP1_BIT       8
#define CAN_TSR_ABRQ0_BIT       7
#define CAN_TSR_TERR0_BIT       3
#define CAN_TSR_ALST0_BIT       2
#define CAN_TSR_TXOK0_BIT       1
#define CAN_TSR_RQCP0_BIT       0

#define CAN_TSR_LOW2            BIT(CAN_TSR_LOW2_BIT)
#define CAN_TSR_LOW1            BIT(CAN_TSR_LOW1_BIT)
#define CAN_TSR_LOW0            BIT(CAN_TSR_LOW0_BIT)
#define CAN_TSR_TME2            BIT(CAN_TSR_TME2_BIT)
#define CAN_TSR_TME1            BIT(CAN_TSR_TME1_BIT)
#define CAN_TSR_TME0            BIT(CAN_TSR_TME0_BIT)
#define CAN_TSR_CODE            (0x3 << 24)
#define CAN_TSR_ABRQ2           BIT(CAN_TSR_ABRQ2_BIT)
#define CAN_TSR_TERR2           BIT(CAN_TSR_TERR2_BIT)
#define CAN_TSR_ALST2           BIT(CAN_TSR_ALST2_BIT)
#define CAN_TSR_TXOK2           BIT(CAN_TSR_TXOK2_BIT)
#define CAN_TSR_RQCP2           BIT(CAN_TSR_RQCP2_BIT)
#define CAN_TSR_ABRQ1           BIT(CAN_TSR_ABRQ1_BIT)
#define CAN_TSR_TERR1           BIT(CAN_TSR_TERR1_BIT)
#define CAN_TSR_ALST1           BIT(CAN_TSR_ALST1_BIT)
#define CAN_TSR_TXOK1           BIT(CAN_TSR_TXOK1_BIT)
#define CAN_TSR_RQCP1           BIT(CAN_TSR_RQCP1_BIT)
#define CAN_TSR_ABRQ0           BIT(CAN_TSR_ABRQ0_BIT)
#define CAN_TSR_TERR0           BIT(CAN_TSR_TERR0_BIT)
#define CAN_TSR_ALST0           BIT(CAN_TSR_ALST0_BIT)
#define CAN_TSR_TXOK0           BIT(CAN_TSR_TXOK0_BIT)
#define CAN_TSR_RQCP0           BIT(CAN_TSR_RQCP0_BIT)

/* Receive FIFO registers */

#define CAN_RFxR_RFOM_BIT       5
#define CAN_RFxR_FOVR_BIT       4
#define CAN_RFxR_FULL_BIT       3

#define CAN_RFxR_RFOM           BIT(CAN_RFxR_RFOM_BIT)
#define CAN_RFxR_FOVR           BIT(CAN_RFxR_FOVR_BIT)
#define CAN_RFxR_FULL           BIT(CAN_RFxR_FULL_BIT)
#define CAN_RFxR_FMP            (0x3)

#define CAN_RF0R_RFOM_BIT       CAN_RFxR_RFOM_BIT
#define CAN_RF0R_FOVR_BIT       CAN_RFxR_FOVR_BIT
#define CAN_RF0R_FULL_BIT       CAN_RFxR_FULL_BIT

#define CAN_RF0R_RFOM           CAN_RFxR_RFOM
#define CAN_RF0R_FOVR           CAN_RFxR_FOVR
#define CAN_RF0R_FULL           CAN_RFxR_FULL
#define CAN_RF0R_FMP            CAN_RFxR_FMP

#define CAN_RF0R_RFOM0          CAN_RF0R_RFOM
#define CAN_RF0R_FOVR0          CAN_RF0R_FOVR
#define CAN_RF0R_FULL0          CAN_RF0R_FULL
#define CAN_RF0R_FMP0           CAN_RF0R_FMP

#define CAN_RF1R_RFOM_BIT       CAN_RFxR_RFOM_BIT
#define CAN_RF1R_FOVR_BIT       CAN_RFxR_FOVR_BIT
#define CAN_RF1R_FULL_BIT       CAN_RFxR_FULL_BIT

#define CAN_RF1R_RFOM           CAN_RFxR_RFOM
#define CAN_RF1R_FOVR           CAN_RFxR_FOVR
#define CAN_RF1R_FULL           CAN_RFxR_FULL
#define CAN_RF1R_FMP            CAN_RFxR_FMP

#define CAN_RF1R_RFOM1          CAN_RF1R_RFOM
#define CAN_RF1R_FOVR1          CAN_RF1R_FOVR
#define CAN_RF1R_FULL1          CAN_RF1R_FULL
#define CAN_RF1R_FMP1           CAN_RF1R_FMP

/* Interrupt enable register */

#define CAN_IER_SLKIE_BIT       17
#define CAN_IER_WKUIE_BIT       16
#define CAN_IER_ERRIE_BIT       15
#define CAN_IER_LECIE_BIT       11
#define CAN_IER_BOFIE_BIT       10
#define CAN_IER_EPVIE_BIT       9
#define CAN_IER_EWGIE_BIT       8
#define CAN_IER_FOVIE1_BIT      6
#define CAN_IER_FFIE1_BIT       5
#define CAN_IER_FMPIE1_BIT      4
#define CAN_IER_FOVIE0_BIT      3
#define CAN_IER_FFIE0_BIT       2
#define CAN_IER_FMPIE0_BIT      1
#define CAN_IER_TMEIE_BIT       0

#define CAN_IER_SLKIE           BIT(CAN_IER_SLKIE_BIT)
#define CAN_IER_WKUIE           BIT(CAN_IER_WKUIE_BIT)
#define CAN_IER_ERRIE           BIT(CAN_IER_ERRIE_BIT)
#define CAN_IER_LECIE           BIT(CAN_IER_LECIE_BIT)
#define CAN_IER_BOFIE           BIT(CAN_IER_BOFIE_BIT)
#define CAN_IER_EPVIE           BIT(CAN_IER_EPVIE_BIT)
#define CAN_IER_EWGIE           BIT(CAN_IER_EWGIE_BIT)
#define CAN_IER_FOVIE1          BIT(CAN_IER_FOVIE1_BIT)
#define CAN_IER_FFIE1           BIT(CAN_IER_FFIE1_BIT)
#define CAN_IER_FMPIE1          BIT(CAN_IER_FMPIE1_BIT)
#define CAN_IER_FOVIE0          BIT(CAN_IER_FOVIE0_BIT)
#define CAN_IER_FFIE0           BIT(CAN_IER_FFIE0_BIT)
#define CAN_IER_FMPIE0          BIT(CAN_IER_FMPIE0_BIT)
#define CAN_IER_TMEIE           BIT(CAN_IER_TMEIE_BIT)

/* Error status register */

#define CAN_ESR_BOFF_BIT        2
#define CAN_ESR_EPVF_BIT        1
#define CAN_ESR_EWGF_BIT        0

#define CAN_ESR_REC             (0xff << 24)
#define CAN_ESR_TEC             (0xff << 16)

#define CAN_ESR_LEC             (0x7 << 4)
#define CAN_ESR_LEC_NONE        (0x0 << 4)
#define CAN_ESR_LEC_STUFF       (0x1 << 4)
#define CAN_ESR_LEC_FORM        (0x2 << 4)
#define CAN_ESR_LEC_ACK         (0x3 << 4)
#define CAN_ESR_LEC_BITR        (0x4 << 4)
#define CAN_ESR_LEC_BITD        (0x5 << 4)
#define CAN_ESR_LEC_CRC         (0x6 << 5)
#define CAN_ESR_LEC_SOFT        (0x7 << 4)

#define CAN_ESR_BOFF            BIT(CAN_ESR_BOFF_BIT)
#define CAN_ESR_EPVF            BIT(CAN_ESR_EPVF_BIT)
#define CAN_ESR_EWGF            BIT(CAN_ESR_EWGF_BIT)

/* Bit timing register */

#define CAN_BTR_SILM_BIT        31
#define CAN_BTR_LBKM_BIT        30

#define CAN_BTR_SILM            BIT(CAN_BTR_SILM_BIT)
#define CAN_BTR_LBKM            BIT(CAN_BTR_LBKM_BIT)
#define CAN_BTR_SJW             (0x3 << 24)
#define CAN_BTR_TS2             (0x7 << 20)
#define CAN_BTR_TS1             (0x7 << 16)
#define CAN_BTR_BRP             (0x3ff)

/* TX mailbox identifier registers */

#define CAN_TIxR_IDE_BIT        2
#define CAN_TIxR_RTR_BIT        1
#define CAN_TIxR_TXRQ_BIT       0

#define CAN_TIxR_STID           (0x7ff << 21)
#define CAN_TIxR_EXID           (0xffffffff - 0x7)
#define CAN_TIxR_IDE            BIT(CAN_TIxR_IDE_BIT)
#define CAN_TIxR_RTR            BIT(CAN_TIxR_RTR_BIT)
#define CAN_TIxR_TXRQ           BIT(CAN_TIxR_TXRQ_BIT)

/* Mailbox data length control and time stamp registers */

#define CAN_TDTxR_TGT_BIT       8

#define CAN_TDTxR_TIME          (0xffff << 16)
#define CAN_TDTxR_TGT           BIT(CAN_TDTxR_TGT_BIT)
#define CAN_TDTxR_DLC           (0xf)


/* Mailbox data low registers */

#define CAN_TDLxR_DATA3         (0xff << 24)
#define CAN_TDLxR_DATA2         (0xff << 16)
#define CAN_TDLxR_DATA1         (0xff << 8)
#define CAN_TDLxR_DATA0         (0xff)

/* Mailbox data high registers */

#define CAN_TDLxR_DATA7         (0xff << 24)
#define CAN_TDLxR_DATA6         (0xff << 16)
#define CAN_TDLxR_DATA5         (0xff << 8)
#define CAN_TDLxR_DATA4         (0xff)

/* Receive FIFO mailbox identifier registers */

#define CAN_RIxR_IDE_BIT        2
#define CAN_RIxR_RTR_BIT        1

#define CAN_RIxR_STID           (0x7ff << 21)
#define CAN_RIxR_EXID           (0xffffffff - 0x7)
#define CAN_RIxR_IDE            BIT(CAN_RIxR_IDE_BIT)
#define CAN_RIxR_RTR            BIT(CAN_RIxR_RTR_BIT)

/* Receive FIFO mailbox data length control and time stamp registers */

#define CAN_RDTxR_TIME          (0xffff << 16)
#define CAN_RDTxR_FMI           (0xff << 8)
#define CAN_RDTxR_DLC           (0xf)

/* Receive FIFO mailbox data low registers */

#define CAN_RDLxR_DATA3         (0xff << 24)
#define CAN_RDLxR_DATA2         (0xff << 16)
#define CAN_RDLxR_DATA1         (0xff << 8)
#define CAN_RDLxR_DATA0         (0xff)

/* Receive FIFO mailbox data high registers */

#define CAN_RDHxR_DATA7         (0xff << 24)
#define CAN_RDHxR_DATA6         (0xff << 16)
#define CAN_RDHxR_DATA5         (0xff << 8)
#define CAN_RDHxR_DATA4         (0xff)

/* Filter master register */

#define CAN_FMR_FINIT_BIT       0

#define CAN_FMR_FINIT           BIT(CAN_FMR_FINIT_BIT)

/* Filter mode register */

#define CAN_FM1R_FBM13_BIT      13
#define CAN_FM1R_FBM12_BIT      12
#define CAN_FM1R_FBM11_BIT      11
#define CAN_FM1R_FBM10_BIT      10
#define CAN_FM1R_FBM9_BIT       9
#define CAN_FM1R_FBM8_BIT       8
#define CAN_FM1R_FBM7_BIT       7
#define CAN_FM1R_FBM6_BIT       6
#define CAN_FM1R_FBM5_BIT       5
#define CAN_FM1R_FBM4_BIT       4
#define CAN_FM1R_FBM3_BIT       3
#define CAN_FM1R_FBM2_BIT       2
#define CAN_FM1R_FBM1_BIT       1
#define CAN_FM1R_FBM0_BIT       0

#define CAN_FM1R_FBM13          BIT(CAN_FM1R_FBM13_BIT)
#define CAN_FM1R_FBM12          BIT(CAN_FM1R_FBM12_BIT)
#define CAN_FM1R_FBM11          BIT(CAN_FM1R_FBM11_BIT)
#define CAN_FM1R_FBM10          BIT(CAN_FM1R_FBM10_BIT)
#define CAN_FM1R_FBM9           BIT(CAN_FM1R_FBM9_BIT)
#define CAN_FM1R_FBM8           BIT(CAN_FM1R_FBM8_BIT)
#define CAN_FM1R_FBM7           BIT(CAN_FM1R_FBM7_BIT)
#define CAN_FM1R_FBM6           BIT(CAN_FM1R_FBM6_BIT)
#define CAN_FM1R_FBM5           BIT(CAN_FM1R_FBM5_BIT)
#define CAN_FM1R_FBM4           BIT(CAN_FM1R_FBM4_BIT)
#define CAN_FM1R_FBM3           BIT(CAN_FM1R_FBM3_BIT)
#define CAN_FM1R_FBM2           BIT(CAN_FM1R_FBM2_BIT)
#define CAN_FM1R_FBM1           BIT(CAN_FM1R_FBM1_BIT)
#define CAN_FM1R_FBM0           BIT(CAN_FM1R_FBM0_BIT)

/* Filter scale register */

#define CAN_FS1R_FSC13_BIT      13
#define CAN_FS1R_FSC12_BIT      12
#define CAN_FS1R_FSC11_BIT      11
#define CAN_FS1R_FSC10_BIT      10
#define CAN_FS1R_FSC9_BIT       9
#define CAN_FS1R_FSC8_BIT       8
#define CAN_FS1R_FSC7_BIT       7
#define CAN_FS1R_FSC6_BIT       6
#define CAN_FS1R_FSC5_BIT       5
#define CAN_FS1R_FSC4_BIT       4
#define CAN_FS1R_FSC3_BIT       3
#define CAN_FS1R_FSC2_BIT       2
#define CAN_FS1R_FSC1_BIT       1
#define CAN_FS1R_FSC0_BIT       0

#define CAN_FS1R_FSC13          BIT(CAN_FS1R_FSC13_BIT)
#define CAN_FS1R_FSC12          BIT(CAN_FS1R_FSC12_BIT)
#define CAN_FS1R_FSC11          BIT(CAN_FS1R_FSC11_BIT)
#define CAN_FS1R_FSC10          BIT(CAN_FS1R_FSC10_BIT)
#define CAN_FS1R_FSC9           BIT(CAN_FS1R_FSC9_BIT)
#define CAN_FS1R_FSC8           BIT(CAN_FS1R_FSC8_BIT)
#define CAN_FS1R_FSC7           BIT(CAN_FS1R_FSC7_BIT)
#define CAN_FS1R_FSC6           BIT(CAN_FS1R_FSC6_BIT)
#define CAN_FS1R_FSC5           BIT(CAN_FS1R_FSC5_BIT)
#define CAN_FS1R_FSC4           BIT(CAN_FS1R_FSC4_BIT)
#define CAN_FS1R_FSC3           BIT(CAN_FS1R_FSC3_BIT)
#define CAN_FS1R_FSC2           BIT(CAN_FS1R_FSC2_BIT)
#define CAN_FS1R_FSC1           BIT(CAN_FS1R_FSC1_BIT)
#define CAN_FS1R_FSC0           BIT(CAN_FS1R_FSC0_BIT)

/* Filter FIFO assignment register */

#define CAN_FFA1R_FFA13_BIT     13
#define CAN_FFA1R_FFA12_BIT     12
#define CAN_FFA1R_FFA11_BIT     11
#define CAN_FFA1R_FFA10_BIT     10
#define CAN_FFA1R_FFA9_BIT      9
#define CAN_FFA1R_FFA8_BIT      8
#define CAN_FFA1R_FFA7_BIT      7
#define CAN_FFA1R_FFA6_BIT      6
#define CAN_FFA1R_FFA5_BIT      5
#define CAN_FFA1R_FFA4_BIT      4
#define CAN_FFA1R_FFA3_BIT      3
#define CAN_FFA1R_FFA2_BIT      2
#define CAN_FFA1R_FFA1_BIT      1
#define CAN_FFA1R_FFA0_BIT      0

#define CAN_FFA1R_FFA13         BIT(CAN_FFA1R_FFA13_BIT)
#define CAN_FFA1R_FFA12         BIT(CAN_FFA1R_FFA12_BIT)
#define CAN_FFA1R_FFA11         BIT(CAN_FFA1R_FFA11_BIT)
#define CAN_FFA1R_FFA10         BIT(CAN_FFA1R_FFA10_BIT)
#define CAN_FFA1R_FFA9          BIT(CAN_FFA1R_FFA9_BIT)
#define CAN_FFA1R_FFA8          BIT(CAN_FFA1R_FFA8_BIT)
#define CAN_FFA1R_FFA7          BIT(CAN_FFA1R_FFA7_BIT)
#define CAN_FFA1R_FFA6          BIT(CAN_FFA1R_FFA6_BIT)
#define CAN_FFA1R_FFA5          BIT(CAN_FFA1R_FFA5_BIT)
#define CAN_FFA1R_FFA4          BIT(CAN_FFA1R_FFA4_BIT)
#define CAN_FFA1R_FFA3          BIT(CAN_FFA1R_FFA3_BIT)
#define CAN_FFA1R_FFA2          BIT(CAN_FFA1R_FFA2_BIT)
#define CAN_FFA1R_FFA1          BIT(CAN_FFA1R_FFA1_BIT)
#define CAN_FFA1R_FFA0          BIT(CAN_FFA1R_FFA0_BIT)

/* Filter activation register */

#define CAN_FA1R_FACT13_BIT     13
#define CAN_FA1R_FACT12_BIT     12
#define CAN_FA1R_FACT11_BIT     11
#define CAN_FA1R_FACT10_BIT     10
#define CAN_FA1R_FACT9_BIT      9
#define CAN_FA1R_FACT8_BIT      8
#define CAN_FA1R_FACT7_BIT      7
#define CAN_FA1R_FACT6_BIT      6
#define CAN_FA1R_FACT5_BIT      5
#define CAN_FA1R_FACT4_BIT      4
#define CAN_FA1R_FACT3_BIT      3
#define CAN_FA1R_FACT2_BIT      2
#define CAN_FA1R_FACT1_BIT      1
#define CAN_FA1R_FACT0_BIT      0

#define CAN_FA1R_FACT13         BIT(CAN_FA1R_FACT13_BIT)
#define CAN_FA1R_FACT12         BIT(CAN_FA1R_FACT12_BIT)
#define CAN_FA1R_FACT11         BIT(CAN_FA1R_FACT11_BIT)
#define CAN_FA1R_FACT10         BIT(CAN_FA1R_FACT10_BIT)
#define CAN_FA1R_FACT9          BIT(CAN_FA1R_FACT9_BIT)
#define CAN_FA1R_FACT8          BIT(CAN_FA1R_FACT8_BIT)
#define CAN_FA1R_FACT7          BIT(CAN_FA1R_FACT7_BIT)
#define CAN_FA1R_FACT6          BIT(CAN_FA1R_FACT6_BIT)
#define CAN_FA1R_FACT5          BIT(CAN_FA1R_FACT5_BIT)
#define CAN_FA1R_FACT4          BIT(CAN_FA1R_FACT4_BIT)
#define CAN_FA1R_FACT3          BIT(CAN_FA1R_FACT3_BIT)
#define CAN_FA1R_FACT2          BIT(CAN_FA1R_FACT2_BIT)
#define CAN_FA1R_FACT1          BIT(CAN_FA1R_FACT1_BIT)
#define CAN_FA1R_FACT0          BIT(CAN_FA1R_FACT0_BIT)

/*
 * Enumerations
 */

typedef enum {
    CAN_TX_TIMESTAMP =  CAN_MCR_TTCM,
    CAN_AUTO_RECOVER =  CAN_MCR_ABOM,
    CAN_AUTO_WAKE =     CAN_MCR_AWUM,
    CAN_NO_RETRANSMIT = CAN_MCR_NART,
    CAN_OVERRUN_LOCK =  CAN_MCR_RFLM,
    CAN_TX_FIFO =       CAN_MCR_TXFP
} can_options;

typedef enum {
    CAN_MODE_SILENT = CAN_BTR_SILM,
    CAN_MODE_LOOPBACK = CAN_BTR_LBKM
} can_modes;

typedef enum {
    CAN_TX_MAILBOX_1 = 1,
    CAN_TX_MAILBOX_2 = 2,
    CAN_TX_MAILBOX_3 = 3
} can_tx_mailbox;

typedef enum {
    CAN_RX_MAILBOX_1 = 0,
    CAN_RX_MAILBOX_2 = 1,
} can_rx_mailbox;

/*
 * Devices
 */

/** CAN device type */
typedef struct can_dev {
    can_reg_map *regs;          /**< Register map */
    rcc_clk_id clk_id;          /**< RCC clock information */
    nvic_irq_num tx_irq_num;    /**< NVIC interrupt number for CAN TX events */
    nvic_irq_num rx_irq_num;    /**< NVIC interrupt number for CAN RX events */
    nvic_irq_num sc_irq_num;    /**< NVIC interrupt number for CAN Status Change Events */
    void (*tx_handler)(void);   /**< User-specified TX interrupt handler */
    void (*rx_handler)(void);   /**< User-specified RX interrupt handler */
    void (*sc_handler)(void);   /**< User-specified Status Change interrupt handler */
    uint8 enabled;              /**< Flag indicating if CAN interrupts should be used */
} can_dev;

#define CAN_MAILBOX_NREGS 4
typedef struct can_mailbox_reg_map {
    __io uint32 IR;            /**< Mailbox identifier register */
    __io uint32 DTR;           /**< Mailbox data length control and time stamp register */
    __io uint32 DLR;           /**< Mailbox data low register */
    __io uint32 DHR;           /**< Mailbox data high register */
} can_mailbox_reg_map;

typedef enum {
    CAN_INT_TX,
    CAN_INT_RX,
    CAN_INT_SC
} can_interrupt_type;

/*
 * CAN Convenience functions
 */

void can_init(can_dev *dev);
void can_tx_irq_handler(void);
void can_rx_irq_handler(void);
void can_attach_interrupt(can_dev *dev, can_interrupt_type interrupt_type, void (*handler)(void));
void can_detach_interrupt(can_dev *dev, can_interrupt_type interrupt_type);

static inline can_mailbox_reg_map* can_tx_mailbox_regs(can_dev *dev, can_tx_mailbox mailbox) {
    __io uint32 *ti0r = &dev->regs->TI0R;
    return (can_mailbox_reg_map*)(ti0r + CAN_MAILBOX_NREGS * (mailbox - 1));
}

static inline can_mailbox_reg_map* can_rx_mailbox_regs(can_dev *dev, can_rx_mailbox mailbox) {
    __io uint32 *ri0r = &dev->regs->RI0R;
    return (can_mailbox_reg_map*)(ri0r + CAN_MAILBOX_NREGS * (mailbox - 1));
}

//static inline can_tx_mess

void can_reconfigure(can_dev *dev, uint32 mcr_config, uint32 ier_config, uint32 btr_config);
uint8 can_tx_mailbox_free(can_dev *dev, can_tx_mailbox mailbox);

/**
 * @brief Returns true if the CAN peripheral is in sleep mode
 * 
 * Returns true if the CAN peripheral is currently in sleep mode.
 */
static inline uint8 can_is_asleep(can_dev *dev) {
    return bb_peri_get_bit(&dev->regs->MSR, CAN_MSR_SLAK_BIT) != 0;
}

/**
 * @brief Returns true if the CAN peripheral is in initialization mode
 * 
 * Returns true if the CAN peripheral is currently in initialization mode.
 */
static inline uint8 can_is_initializing(can_dev *dev) {
    return bb_peri_get_bit(&dev->regs->MSR, CAN_MSR_INAK_BIT) != 0;
}

/**
 * @brief Returns true if the CAN peripheral is in normal mode
 * 
 * Returns true if the CAN peripheral is currently in normal mode
 */
static inline uint8 can_is_normal(can_dev *dev) {
    return (dev->regs->MSR & (CAN_MSR_SLAK | CAN_MSR_INAK)) == 0;
}

/**
 * @brief Wake the CAN peripheral from sleep mode
 *
 * Wakes up the CAN peripheral from sleep mode by clearing the sleep
 * bit in the master control register.  On reset, the CAN peripheral
 * starts in sleep mode.
 *
 * @param dev CAN peripheral to wake
 */
static inline void can_leave_sleep(can_dev *dev) {
    bb_peri_set_bit(&dev->regs->MCR, CAN_MCR_SLEEP_BIT, 0);
}

/**
 * @brief Signal the CAN peripheral to enter sleep mode
 *
 * Signals the CAN peripheral to enter sleep mode by setting the sleep
 * bit in the master control register.  Does not wait for the peripheral
 * to actually go to sleep.
 *
 * @param dev CAN peripheral to put to sleep
 */
static inline void can_enter_sleep(can_dev *dev) {
    bb_peri_set_bit(&dev->regs->MCR, CAN_MCR_SLEEP_BIT, 1);
}

/**
 * @brief Put the CAN peripheral into initialization mode
 *
 * Signals the CAN peripheral to enter initialization mode.  Note
 * that if the peripheral is in sleep mode, it must be woken before
 * it can enter initialization mode.  Certain filter and general
 * configuration options can only be set during initialization.
 *
 * @param dev CAN peripheral to prepare for configuration
 */
static inline void can_enter_initialization(can_dev *dev) {
    bb_peri_set_bit(&dev->regs->MCR, CAN_MCR_INRQ_BIT, 1);
    return;
}

/**
 * @brief Leave initialization mode and begin normal operation
 *
 * Signals the CAN peripheral to leave initialization mode.
 * Note that messages can only be sent and received in normal mode.
 * @param dev CAN peripheral to leave initialization mode
 */
static inline void can_leave_initialization(can_dev *dev) {
    bb_peri_set_bit(&dev->regs->MCR, CAN_MCR_INRQ_BIT, 0);
    return;
}

extern can_dev *CAN;

#ifdef __cplusplus
}
#endif

#endif