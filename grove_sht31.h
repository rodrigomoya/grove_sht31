/*
 * grove_sht31.h
 *
 * Copyright (c) 2016 rizoma.io
 * Website    : www.seeed.cc
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


#ifndef __GROVE_SHT31_H__
#define __GROVE_SHT31_H__

#include "suli2.h"

//GROVE_NAME        "Grove - SHT31"
//SKU               10102011239631235123
//IF_TYPE           I2C
//IMAGE_URL         http://bz.seeedstudio.com/depot/bmz_cache/c/c326ed0a4c729308111af05165ae3afd.image.530x397.jpg
//DESCRIPTION       "Driver for SHT31 de SeeedStudio"
//WIKI_URL          http://www.rizoma.io
//ADDED_AT          "2016-08-30"
//AUTHOR            "Rodrigo Moya Toro - rodrigo@rizoma.io"


#define SHT31_DEFAULT_ADDR    0x44
#define SHT31_MEAS_HIGHREP_STRETCH 0x2C06
#define SHT31_MEAS_MEDREP_STRETCH  0x2C0D
#define SHT31_MEAS_LOWREP_STRETCH  0x2C10
#define SHT31_MEAS_HIGHREP         0x2400
#define SHT31_MEAS_MEDREP          0x240B
#define SHT31_MEAS_LOWREP          0x2416
#define SHT31_READSTATUS           0xF32D
#define SHT31_CLEARSTATUS          0x3041
#define SHT31_SOFTRESET            0x30A2
#define SHT31_HEATEREN             0x306D
#define SHT31_HEATERDIS            0x3066

class GroveSHT31
{
    public:
        GroveSHT31(int pinsda, int pinscl);
        bool read_temperature(float *temperature);
        bool read_humidity(float *humidity);

        uint8_t crc8(const uint8_t *data, int len);
        uint16_t readStatus(void);
        void reset(void);
        void heater(boolean);
        
    private:
        I2C_T *i2c;
        float humi, temp;
        bool readData(void);
        bool readTempHum(void);
        void writeCommand(uint16_t cmd);        
};

#endif
