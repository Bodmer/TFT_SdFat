/* Arduino SdSpi Library
 * Copyright (C) 2013 by William Greiman
 *
 * This file is part of the Arduino SdSpi Library
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Arduino SdSpi Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
/**
* \file
* \brief SdSpi class for V2 SD/SDHC cards
*/
#ifndef SdSpi_h
#define SdSpi_h
#include <Arduino.h>
#include "SdFatConfig.h"

//------------------------------------------------------------------------------
/**
 * \class SdSpiBase
 * \brief Virtual SPI class for access to SD and SDHC flash memory cards.
 */
class SdSpiBase {
 public:
  /** Initialize the SPI bus */
  virtual void begin() = 0;
  /** Set SPI options for access to SD/SDHC cards.
   *
   * \param[in] divisor SCK clock divider relative to the system clock.
   */
  virtual void init(uint8_t divisor);
  /** Receive a byte.
   *
   * \return The byte.
   */
  virtual uint8_t receive() = 0;
  /** Receive multiple bytes.
   *
   * \param[out] buf Buffer to receive the data.
   * \param[in] n Number of bytes to receive.
   *
   * \return Zero for no error or nonzero error code.
   */
  virtual uint8_t receive(uint8_t* buf, size_t n) = 0;
  /** Send a byte.
   *
   * \param[in] data Byte to send
   */
  virtual void send(uint8_t data) = 0;
  /** Send multiple bytes.
  *
  * \param[in] buf Buffer for data to be sent.
  * \param[in] n Number of bytes to send.
  */
  virtual void send(const uint8_t* buf, size_t n) = 0;
  /** \return true if hardware SPI else false */
  virtual bool useSpiTransactions() = 0;
};
//------------------------------------------------------------------------------
/**
 * \class SdSpi
 * \brief SPI class for access to SD and SDHC flash memory cards.
 */
#if SD_SPI_CONFIGURATION >= 3
class SdSpi : public SdSpiBase {
#else  // SD_SPI_CONFIGURATION >= 3
class SdSpi {
#endif  // SD_SPI_CONFIGURATION >= 3
 public:
  /** Initialize the SPI bus */
  void begin();
  /** Set SPI options for access to SD/SDHC cards.
   *
   * \param[in] divisor SCK clock divider relative to the system clock.
   */
  void init(uint8_t divisor);
  /** Receive a byte.
   *
   * \return The byte.
   */
  uint8_t receive();
  /** Receive multiple bytes.
   *
   * \param[out] buf Buffer to receive the data.
   * \param[in] n Number of bytes to receive.
   *
   * \return Zero for no error or nonzero error code.
   */
  uint8_t receive(uint8_t* buf, size_t n);
  /** Send a byte.
   *
   * \param[in] data Byte to send
   */
  void send(uint8_t data);
  /** Send multiple bytes.
   *
   * \param[in] buf Buffer for data to be sent.
   * \param[in] n Number of bytes to send.
   */
  void send(const uint8_t* buf, size_t n);
  /** \return true - uses SPI transactions */
  bool useSpiTransactions() {
    return true;
  }
};
//------------------------------------------------------------------------------
/**
 * \class SdSpiLib
 * \brief Arduino SPI library class for access to SD and SDHC flash
 *        memory cards.
 */
#if SD_SPI_CONFIGURATION >= 3 || SD_SPI_CONFIGURATION == 1 || defined(DOXYGEN)
#include <SPI.h>
#if SD_SPI_CONFIGURATION >= 3
class SdSpiLib : public SdSpiBase {
#else  // SD_SPI_CONFIGURATION >= 3
class SdSpiLib {
#endif  // SD_SPI_CONFIGURATION >= 3
 public:
  /**
   * Initialize SPI pins.
   */
  void begin() {
    SPI.begin();
  }
  /** Set SPI options for access to SD/SDHC cards.
   *
   * \param[in] divisor SCK clock divider relative to the system clock.
   */
  void init(uint8_t divisor) {
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
#ifndef SPI_CLOCK_DIV128
    SPI.setClockDivider(divisor);
#else  // SPI_CLOCK_DIV128
    int v;
    if (divisor <= 2) {
      v = SPI_CLOCK_DIV2;
    } else  if (divisor <= 4) {
      v = SPI_CLOCK_DIV4;
    } else  if (divisor <= 8) {
      v = SPI_CLOCK_DIV8;
    } else  if (divisor <= 16) {
      v = SPI_CLOCK_DIV16;
    } else  if (divisor <= 32) {
      v = SPI_CLOCK_DIV32;
    } else  if (divisor <= 64) {
      v = SPI_CLOCK_DIV64;
    } else {
      v = SPI_CLOCK_DIV128;
    }
    SPI.setClockDivider(v);
#endif  // SPI_CLOCK_DIV128
  }
  /** Receive a byte.
   *
   * \return The byte.
   */
  uint8_t receive() {
    return SPI.transfer(0XFF);
  }
  /** Receive multiple bytes.
   *
   * \param[out] buf Buffer to receive the data.
   * \param[in] n Number of bytes to receive.
   *
   * \return Zero for no error or nonzero error code.
   */
  uint8_t receive(uint8_t* buf, size_t n) {
    for (size_t i = 0; i < n; i++) {
      buf[i] = SPI.transfer(0XFF);
    }
    return 0;
  }
  /** Send a byte.
   *
   * \param[in] b Byte to send
   */
  void send(uint8_t b) {
    SPI.transfer(b);
  }
  /** Send multiple bytes.
   *
   * \param[in] buf Buffer for data to be sent.
   * \param[in] n Number of bytes to send.
   */
  void send(const uint8_t* buf , size_t n) {
    for (size_t i = 0; i < n; i++) {
      SPI.transfer(buf[i]);
    }
  }
  /** \return true - uses SPI transactions */
  bool useSpiTransactions() {
    return true;
  }
};
#endif  // SD_SPI_CONFIGURATION >= 3 || SD_SPI_CONFIGURATION == 1
//------------------------------------------------------------------------------
#if SD_SPI_CONFIGURATION > 1 || defined(DOXYGEN)
#include "utility/SoftSPI.h"
/**
 * \class SdSpiSoft
 * \brief Software SPI class for access to SD and SDHC flash memory cards.
 */
template<uint8_t MisoPin, uint8_t MosiPin, uint8_t SckPin>
class SdSpiSoft : public SdSpiBase {
 public:
  /**
   * initialize SPI pins
   */
  void begin() {
    m_spi.begin();
  }
  /**
   * Initialize hardware SPI - dummy for soft SPI
   * \param[in] divisor SCK divisor - ignored.
   */
  void init(uint8_t divisor) {}
  /** Receive a byte.
   *
   * \return The byte.
   */
  uint8_t receive() {
    return m_spi.receive();
  }
  /** Receive multiple bytes.
  *
  * \param[out] buf Buffer to receive the data.
  * \param[in] n Number of bytes to receive.
  *
  * \return Zero for no error or nonzero error code.
  */
  uint8_t receive(uint8_t* buf, size_t n) {
    for (size_t i = 0; i < n; i++) {
      buf[i] = receive();
    }
    return 0;
  }
  /** Send a byte.
   *
   * \param[in] data Byte to send
   */
  void send(uint8_t data) {
    m_spi.send(data);
  }
  /** Send multiple bytes.
   *
   * \param[in] buf Buffer for data to be sent.
   * \param[in] n Number of bytes to send.
   */
  void send(const uint8_t* buf , size_t n) {
    for (size_t i = 0; i < n; i++) {
      send(buf[i]);
    }
  }
  /** \return false - no SPI transactions */
  bool useSpiTransactions() {
    return false;
  }

 private:
  SoftSPI<MisoPin, MosiPin, SckPin, 0> m_spi;
};
#endif  // SD_SPI_CONFIGURATION > 1 || defined(DOXYGEN)
//------------------------------------------------------------------------------
#if SD_SPI_CONFIGURATION == 0 || SD_SPI_CONFIGURATION >= 3
/** Default is custom fast SPI. */
typedef SdSpi SpiDefault_t;
#elif SD_SPI_CONFIGURATION == 1
/** Default is Arduino library SPI. */
typedef SdSpiLib SpiDefault_t;
#elif SD_SPI_CONFIGURATION == 2
/** Default is software SPI. */
typedef SdSpiSoft<SOFT_SPI_MISO_PIN, SOFT_SPI_MOSI_PIN, SOFT_SPI_SCK_PIN>
SpiDefault_t;
#else  // SD_SPI_CONFIGURATION == 0 || SD_SPI_CONFIGURATION >= 3
#error bad SD_SPI_CONFIGURATION
#endif  // SD_SPI_CONFIGURATION == 0 || SD_SPI_CONFIGURATION >= 3
//------------------------------------------------------------------------------
// Use of in-line for AVR to save flash.
#ifdef __AVR__
//------------------------------------------------------------------------------
inline void SdSpi::begin() {
#ifdef __AVR_ATmega328P__
  // Save a few bytes for 328 CPU - gcc optimizes single bit '|' to sbi.
  PORTB |= 1 << 2;  // SS high
  DDRB  |= 1 << 2;  // SS output mode
  DDRB  |= 1 << 3;  // MOSI output mode
  DDRB  |= 1 << 5;  // SCK output mode
#else  // __AVR_ATmega328P__

  // set SS high - may be chip select for another SPI device
  digitalWrite(SS, HIGH);

  // SS must be in output mode even it is not chip select
  pinMode(SS, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
#endif  // __AVR_ATmega328P__
}
//------------------------------------------------------------------------------
inline void SdSpi::init(uint8_t divisor) {
  uint8_t b = 2;
  uint8_t r = 0;

  // See AVR processor documentation.
  for (; divisor > b && r < 7; b <<= 1, r += r < 5 ? 1 : 2) {}
  SPCR = (1 << SPE) | (1 << MSTR) | (r >> 1);
  SPSR = r & 1 ? 0 : 1 << SPI2X;
}
//------------------------------------------------------------------------------
// Modified to improve speed slightly by syncing the read to the SPIF flag
inline uint8_t SdSpi::receive() {
  SPDR = 0XFF;
  asm volatile( "nop\n" ::); // Sync bit check
  while (!(SPSR & (1 << SPIF))) {}
  return SPDR;
}
//------------------------------------------------------------------------------
// Modified by Bodmer to use delays rather than SPIF flag checks
inline uint8_t SdSpi::receive(uint8_t* buf, size_t n) {
  if (n-- == 0) {
    return 0;
  }
  SPDR = 0XFF;
  while (!(SPSR & (1 << SPIF))) {}
  for (size_t i = 0; i < n; i++) {
    //asm volatile( "nop\n" ::); // Sync bit check
    //while (!(SPSR & (1 << SPIF))) {}
    uint8_t b = SPDR;
    SPDR = 0XFF;
    buf[i] = b;
    asm volatile( "adiw r24,0 \n" "adiw	r24,0 \n" "adiw	r24,0 \n" "adiw	r24,0 \n" ::); // Minimum 8 cycle nop delay
  }
  while (!(SPSR & (1 << SPIF))) {}
  buf[n] = SPDR;
  return 0;
}
//------------------------------------------------------------------------------
// Modified to improve speed slightly by syncing the read to the SPIF flag
inline void SdSpi::send(uint8_t data) {
  SPDR = data;
  asm volatile( "nop\n" ::); // Sync bit check
  while (!(SPSR & (1 << SPIF))) {}
}
//------------------------------------------------------------------------------
inline void SdSpi::send(const uint8_t* buf , size_t n) {
  if (n == 0) {
    return;
  }
  SPDR = buf[0];
  if (n > 1) {
    uint8_t b = buf[1];
    size_t i = 2;
    while (1) {
      while (!(SPSR & (1 << SPIF))) {}
      SPDR = b;
      if (i == n) {
        break;
      }
      b = buf[i++];
    }
  }
  while (!(SPSR & (1 << SPIF))) {}
}
#endif  // __AVR__
#endif  // SdSpi_h
