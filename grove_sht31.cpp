/*
 * grove_sht31.cpp
 *
 * Copyright (c) 2016 Rizoma.io .
 * Website    : www.rizoma.io
 * Author     : Rodrigo Moya
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
  heater(true);
  delay(3000);
  
  _i2caddr = SHT31_ADDR;
  reset();  
}

bool GroveSHT31::read_temperature(float *temperature) {
  if (! getTempHum()) return NAN;
  *temperature = temp;

  return true;  
}


bool GroveSHT31::read_humidity(float *humidity) {  
  if (! getTempHum()) return NAN;
  *humidity = humi;
  return true;  
}

uint16_t GroveSHT31::readStatus(void) {  
}

void GroveSHT31::reset(void) {
  writeCommand(SHT31_SOFTRESET);
  delay(500);
}

void GroveSHT31::heater(boolean h) {
  if (h)
    writeCommand(SHT31_HEATEREN);
  else
    writeCommand(SHT31_HEATERDIS);
}

uint8_t GroveSHT31::crc8(const uint8_t *data, int len) {
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


boolean GroveSHT31::getTempHum(void) {
  uint8_t readbuffer[6];
  writeCommand(SHT31_MEAS_HIGHREP);
  
  delay(500);
  Wire.requestFrom(_i2caddr, (uint8_t)6);
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
  
  /*          //Cálculo de huemdad del grano a partir de los modelos de Henderson y Chung-Pfost
  
  double c1, c2, c3;
  double d1, d2, d3;
  
  switch (grano){     //Determinación de parámetros según tipo de grano
  
    case arrozcascara:
      c1 = 1.9187;	
      c2 = 51.1610;	
      c3 = 2.4451;
      d1 = 0.2939;	
      d2 = 0.0460;	
      d3 = 35.7030;
      break;
    case maiz:
      c1 = 8.6541;
      c2 = 49.81;	
      c3 = 1.8634;
      d1 = 0.3387;
      d2 = 0.0590;
      d3 = 30.2050;
      break;
    case soja:
      c1 = 30.5327;
      c2 = 134.1360;
      c3 = 1.2164;
      d1 = 0.4163;
      d2 = 0.0719;	
      d3 = 100.2880;
      break;
    case sorgo:
      c1 = 
      c2 =
      c3 = 
      d1 =
      d2 =
      d3 = 
      break;
    case arrozcascara:
      c1 = 
      c2 =
      c3 = 
      d1 =
      d2 =
      d3 = 
      break;
    case VALUE_1:
      c1 = 
      c2 =
      c3 = 
      d1 =
      d2 =
      d3 = 
      break;
    case VALUE_2.value_:
      c1 = 
      c2 =
      c3 = 
      d1 =
      d2 =
      d3 = 
      break;
    case 3:
      printf("Value is 3\n");
      break;
  }
  
  double humiH = (1/100) * pow( (log(1-(shum/100))/(-c1*(stemp+c2)), (1/c3));
  double humiCP = d1 - d2*(log(-(stemp+d3)*log(shum/100));
  
  double meanhumi= (humiH + humiCP)/2;
  
  */
  
  humi = shum;
   
  return true;
}

void GroveSHT31::writeCommand(uint16_t cmd) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(cmd >> 8);
  Wire.write(cmd & 0xFF);
  Wire.endTransmission();      
}
