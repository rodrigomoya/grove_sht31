/*
 * grove_sht31.cpp
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Lambor, Jack
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "suli2.h"
#include "grove_sht31.h" 

GroveSHT31::GroveSHT31(int pinsda, int pinscl) {
    this->i2c = (I2C_T *)malloc(sizeof(I2C_T));
    suli_i2c_init(i2c, pinsda, pinscl);
    Wire.begin();
    reset();
}

uint16_t GroveSHT31::readStatus(void) {
  writeCommand(SHT31_READSTATUS);
  Wire.requestFrom(SHT31_DEFAULT_ADDR, (uint8_t)3);
  uint16_t stat = Wire.read();
  stat <<= 8;
  stat |= Wire.read();
  return stat;
}

void GroveSHT31::reset(void) {
  writeCommand(SHT31_SOFTRESET);
  delay(10);
}

void GroveSHT31::heater(boolean h) {
  if (h)
    writeCommand(SHT31_HEATEREN);
  else
    writeCommand(SHT31_HEATERDIS);
}


bool GroveSHT31::read_temperature(float *temperature) {
  if (! readTempHum()) return NAN;
    *temperature = temp;
    return true;  
}
  

bool GroveSHT31::read_humidity(float *humidity) {
  if (! readTempHum()) return NAN;
    *humidity = humi;
    return true;  
}


bool GroveSHT31::readTempHum(void) {
  uint8_t readbuffer[6];

  writeCommand(SHT31_MEAS_HIGHREP);  
  delay(500);
  Wire.requestFrom(SHT31_DEFAULT_ADDR, (uint8_t)6);
  if (Wire.available() != 6) 
    return false;
  for (uint8_t i=0; i<6; i++) {
    readbuffer[i] = Wire.read();
  }
  uint16_t ST, SRH;
  ST = readbuffer[0];
  ST <<= 8;
  ST |= readbuffer[1];

  if (readbuffer[2] != crc8(readbuffer, 2)) return false;

  SRH = readbuffer[3];
  SRH <<= 8;
  SRH |= readbuffer[4];

  if (readbuffer[5] != crc8(readbuffer+3, 2)) return false;

  double stemp = ST;
  stemp *= 175;
  stemp /= 0xffff;
  stemp = -45 + stemp;
  temp = stemp;
  
  double shum = SRH;
  shum *= 100;
  shum /= 0xFFFF;
  
  humi = shum;
  
  return true;
}

void GroveSHT31::writeCommand(uint16_t cmd) {
  Wire.beginTransmission(SHT31_DEFAULT_ADDR);
  Wire.write(cmd >> 8);
  Wire.write(cmd & 0xFF);
  Wire.endTransmission();  
}

uint8_t GroveSHT31::crc8(const uint8_t *data, int len)
{
  const uint8_t POLYNOMIAL(0x31);
  uint8_t crc(0xFF);
  
  for ( int j = len; j; --j ) {
      crc ^= *data++;

      for ( int i = 8; i; --i ) {
    crc = ( crc & 0x80 )
      ? (crc << 1) ^ POLYNOMIAL
      : (crc << 1);
      }
  }
  return crc;
}
