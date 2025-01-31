/** 
 * --------------------------------------------------------------------------------------+
 * @desc        Two Wire Interface / I2C Communication
 * --------------------------------------------------------------------------------------+
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       06.09.2020
 * @file        twi.c
 * @tested      AVR Atmega16, ATmega8, Atmega328
 *
 * @depend      twi.h
 * --------------------------------------------------------------------------------------+
 * @usage       Master Transmit Operation
 */
 
// include libraries
#include "twi.h"
#include "ti/driverlib/dl_i2c.h"
#include "ti_msp_dl_config.h"

#define TWI_TX_MAX_PACKET_SIZE 512
#define OLED_ADDRESS 0x3C

typedef enum I2cControllerStatus {
    I2C_STATUS_IDLE = 0,
    I2C_STATUS_TX_STARTED,
    I2C_STATUS_TX_INPROGRESS,
    I2C_STATUS_TX_COMPLETE,
    I2C_STATUS_ERROR,
} twi_status_t;

static struct {
    uint8_t address;
    uint8_t internal_buffer[TWI_TX_MAX_PACKET_SIZE];
    uint8_t *buffer;
    uint32_t items_in_buffer;
    uint32_t tx_count;
    twi_status_t controller_status;
} twi = {
    .address = OLED_ADDRESS,
    .internal_buffer = {},
    .buffer = NULL,
    .items_in_buffer = 0,
    .tx_count = 0,
    .controller_status = I2C_STATUS_IDLE
};

/**
 * @desc    TWI init - initialize frequency
 *
 * @param   void
 *
 * @return  void
 */
void TWI_Init (void)
{
    twi.buffer = twi.internal_buffer;
  return;
}

/**
 * @desc    TWI MT Start
 *
 * @param   void
 *
 * @return  char
 */
char TWI_MT_Start (void)
{
  return SUCCESS;
}

void TWI_set_external_buffer(uint8_t *buffer, uint32_t buffer_size) {
    twi.buffer = buffer;
    twi.items_in_buffer = buffer_size;
}

static void __send_buffer(void) {
    if (twi.items_in_buffer == 0) {
        return;
    }

    twi.controller_status = I2C_STATUS_IDLE;

    twi.tx_count = DL_I2C_fillControllerTXFIFO(I2C_0_INST, &twi.buffer[0], twi.items_in_buffer);

    if (twi.tx_count < twi.items_in_buffer) {
        DL_I2C_enableInterrupt(
            I2C_0_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
    } else {
        DL_I2C_disableInterrupt(
            I2C_0_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
    }

    twi.controller_status = I2C_STATUS_TX_STARTED;
    while (!(
        DL_I2C_getControllerStatus(I2C_0_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;
    DL_I2C_startControllerTransfer(
        I2C_0_INST, twi.address, DL_I2C_CONTROLLER_DIRECTION_TX, twi.items_in_buffer);

    while ((twi.controller_status != I2C_STATUS_TX_COMPLETE) &&
           (twi.controller_status != I2C_STATUS_ERROR)) {
        __WFE();
    }

    while (DL_I2C_getControllerStatus(I2C_0_INST) &
           DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
        ;

    if (DL_I2C_getControllerStatus(I2C_0_INST) &
        DL_I2C_CONTROLLER_STATUS_ERROR) {
        __BKPT(0);
    }

    while (!(
        DL_I2C_getControllerStatus(I2C_0_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;
    
    twi.items_in_buffer = 0;
    twi.buffer = twi.internal_buffer;
}

/**
 * @desc    TWI Send address + write
 *
 * @param   char
 *
 * @return  char
 */
char TWI_MT_Send_SLAW (char address)
{
    (void) address;
    // __send_buffer();

    return SUCCESS;
}

/**
 * @desc    TWI Send data
 *
 * @param   char
 *
 * @return  char
 */
char TWI_MT_Send_Data(char data)
{
    if (twi.items_in_buffer >= TWI_TX_MAX_PACKET_SIZE) {
        __BKPT();
    }

    twi.internal_buffer[twi.items_in_buffer] = data;
    twi.items_in_buffer += 1;

    return SUCCESS;
}

/**
 * @desc    TWI Send address + read
 *
 * @param   char
 *
 * @return  char
 */
char TWI_MR_Send_SLAR (char address)
{   
    return SUCCESS;
}

/**
 * @desc    TWI stop
 *
 * @param   void
 *
 * @return  void
 */
void TWI_Stop (void)
{
    __send_buffer();
    return;
}

void I2C_0_INST_IRQHandler(void)
{
    switch (DL_I2C_getPendingInterrupt(I2C_0_INST)) {
        case DL_I2C_IIDX_CONTROLLER_TX_DONE:
            DL_I2C_disableInterrupt(
                I2C_0_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
            twi.controller_status = I2C_STATUS_TX_COMPLETE;
            break;
        case DL_I2C_IIDX_CONTROLLER_TXFIFO_TRIGGER:
            twi.controller_status = I2C_STATUS_TX_INPROGRESS;
            /* Fill TX FIFO with next bytes to send */
            if (twi.tx_count < twi.items_in_buffer) {
                twi.tx_count += DL_I2C_fillControllerTXFIFO(
                    I2C_0_INST, &twi.buffer[twi.tx_count], twi.items_in_buffer - twi.tx_count);
            }
            break;
        case DL_I2C_IIDX_CONTROLLER_NACK:
            if (twi.controller_status == I2C_STATUS_TX_STARTED) {
                twi.controller_status = I2C_STATUS_ERROR;
            }
        default:
            break;
    }
}