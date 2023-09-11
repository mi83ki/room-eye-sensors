#include "ToFSensor.h"

#include <Log.h>

#define VL53L0X_REG_IDENTIFICATION_MODEL_ID 0xc0
#define VL53L0X_REG_IDENTIFICATION_REVISION_ID 0xc2
#define VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD 0x50
#define VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD 0x70
#define VL53L0X_REG_SYSRANGE_START 0x00
#define VL53L0X_REG_RESULT_INTERRUPT_STATUS 0x13
#define VL53L0X_REG_RESULT_RANGE_STATUS 0x14
#define address 0x29  // I2C address

ToFSensor::ToFSensor() {
  Wire1.begin(26, 32);  // join i2c bus (address optional for master)
}

ToFSensor::~ToFSensor() {}

uint16_t ToFSensor::bswap(byte b[]) {
  // Big Endian unsigned short to little endian unsigned short
  uint16_t val = ((b[0] << 8) & b[1]);
  return val;
}

uint16_t ToFSensor::makeuint16(int lsb, int msb) {
  return ((msb & 0xFF) << 8) | (lsb & 0xFF);
}

void ToFSensor::write_byte_data(byte data) {
  Wire1.beginTransmission(address);
  Wire1.write(data);
  Wire1.endTransmission();
}

void ToFSensor::write_byte_data_at(byte reg, byte data) {
  // write data word at address and register
  Wire1.beginTransmission(address);
  Wire1.write(reg);
  Wire1.write(data);
  Wire1.endTransmission();
}

void ToFSensor::write_word_data_at(byte reg, uint16_t data) {
  // write data word at address and register
  byte b0 = (data & 0xFF);
  byte b1 = ((data >> 8) && 0xFF);

  Wire1.beginTransmission(address);
  Wire1.write(reg);
  Wire1.write(b0);
  Wire1.write(b1);
  Wire1.endTransmission();
}

byte ToFSensor::read_byte_data() {
  Wire1.requestFrom(address, 1);
  while (Wire1.available() < 1) delay(1);
  byte b = Wire1.read();
  return b;
}

byte ToFSensor::read_byte_data_at(byte reg) {
  // write_byte_data((byte)0x00);
  write_byte_data(reg);
  Wire1.requestFrom(address, 1);
  while (Wire1.available() < 1) delay(1);
  byte b = Wire1.read();
  return b;
}

uint16_t ToFSensor::read_word_data_at(byte reg) {
  write_byte_data(reg);
  Wire1.requestFrom(address, 2);
  while (Wire1.available() < 2) delay(1);
  gbuf[0] = Wire1.read();
  gbuf[1] = Wire1.read();
  return bswap(gbuf);
}

void ToFSensor::read_block_data_at(byte reg, int sz) {
  int i = 0;
  write_byte_data(reg);
  Wire1.requestFrom(address, sz);
  for (i = 0; i < sz; i++) {
    while (Wire1.available() < 1) delay(1);
    gbuf[i] = Wire1.read();
  }
}

uint16_t ToFSensor::VL53L0X_decode_vcsel_period(short vcsel_period_reg) {
  // Converts the encoded VCSEL period register value into the real
  // period in PLL clocks
  uint16_t vcsel_period_pclks = (vcsel_period_reg + 1) << 1;
  return vcsel_period_pclks;
}

void ToFSensor::tofLoop(void) {
  write_byte_data_at(VL53L0X_REG_SYSRANGE_START, 0x01);

  byte val = 0;
  int cnt = 0;
  while (cnt < 100) {  // 1 second waiting time max
    delay(10);
    val = read_byte_data_at(VL53L0X_REG_RESULT_RANGE_STATUS);
    if (val & 0x01) break;
    cnt++;
  }
  if (val & 0x01)
    Serial.println("ready");
  else
    Serial.println("not ready");

  read_block_data_at(0x14, 12);
  uint16_t acnt = makeuint16(gbuf[7], gbuf[6]);
  uint16_t scnt = makeuint16(gbuf[9], gbuf[8]);
  uint16_t dist = makeuint16(gbuf[11], gbuf[10]);
  byte DeviceRangeStatusInternal = ((gbuf[0] & 0x78) >> 3);
  logger.info(
    String("ambient count: ") + String(acnt)
    + String(", signal count: ") + String(scnt)
    + String(", distance: ") + String(dist)
    + String(", status: ") + String(DeviceRangeStatusInternal)
    );
  delay(1000);
}