#include <M5Atom.h>

#pragma once

class ToFSensor {
 public:
  typedef struct sToF {
    uint16_t ambientCnt;
    uint16_t signalCnt;
    uint16_t distance;
    byte status;
  } ToFData;
  ToFSensor();
  ~ToFSensor();
  void tofLoop(void);

 private:
  uint16_t bswap(byte b[]);
  uint16_t makeuint16(int lsb, int msb);
  void write_byte_data(byte data);
  void write_byte_data_at(byte reg, byte data);
  void write_word_data_at(byte reg, uint16_t data);
  byte read_byte_data();
  byte read_byte_data_at(byte reg);
  uint16_t read_word_data_at(byte reg);
  void read_block_data_at(byte reg, int sz);
  uint16_t VL53L0X_decode_vcsel_period(short vcsel_period_reg);

  byte gbuf[16];
};