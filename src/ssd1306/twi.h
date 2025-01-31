/** 
 * --------------------------------------------------------------------------------------+
 * @desc        Two Wire Interface / I2C Communication
 * --------------------------------------------------------------------------------------+
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       06.09.2020
 * @file        twi.h
 * @tested      AVR Atmega16, ATmega8, Atmega328
 *
 * @depend      avr/io.h
 * --------------------------------------------------------------------------------------+
 * @usage       Basic Master Transmit Operation
 */

#ifndef __TWI_H__
#define __TWI_H__

#ifndef SUCCESS
#define SUCCESS             0
#endif

#ifndef ERROR
#define ERROR               1
#endif 

#include <stdint.h>
  
  /**
   * @desc    TWI init
   *
   * @param   void
   *
   * @return  void
   */
  void TWI_Init (void);

  /**
   * @desc    TWI MT Start
   *
   * @param   void
   *
   * @return  char
   */
  char TWI_MT_Start (void);

  /**
   * @desc    TWI Send SLAW
   *
   * @param   void
   *
   * @return  unsigned char
   */
  char TWI_MT_Send_SLAW (char);

void TWI_set_external_buffer(uint8_t *buffer, uint32_t buffer_size);

  /**
   * @desc    TWI Send data
   *
   * @param   char
   *
   * @return  char
   */
  char TWI_MT_Send_Data (char);

  /**
   * @desc    TWI Send SLAR
   *
   * @param   void
   *
   * @return  unsigned char
   */
  char TWI_MR_Send_SLAR (char);

  /**
   * @desc    TWI stop
   *
   * @param   void
   *
   * @return  void
   */
  void TWI_Stop (void);
  
#endif
