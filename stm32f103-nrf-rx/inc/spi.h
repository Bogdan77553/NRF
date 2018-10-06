#ifndef __SPI_H
#define __SPI_H

#define ADXL345_DEVID_REG          0x00
#define ADXL345_THRESH_TAP_REG     0x1D
#define ADXL345_OFSX_REG           0x1E
#define ADXL345_OFSY_REG           0x1F
#define ADXL345_OFSZ_REG           0x20
#define ADXL345_DUR_REG            0x21
#define ADXL345_LATENT_REG         0x22
#define ADXL345_WINDOW_REG         0x23
#define ADXL345_THRESH_ACT_REG     0x24
#define ADXL345_THRESH_INACT_REG   0x25
#define ADXL345_TIME_INACT_REG     0x26
#define ADXL345_ACT_INACT_CTL_REG  0x27
#define ADXL345_THRESH_FF_REG      0x28
#define ADXL345_TIME_FF_REG        0x29
#define ADXL345_TAP_AXES_REG       0x2A
#define ADXL345_ACT_TAP_STATUS_REG 0x2B
#define ADXL345_BW_RATE_REG        0x2C
#define ADXL345_POWER_CTL_REG      0x2D
#define ADXL345_INT_ENABLE_REG     0x2E
#define ADXL345_INT_MAP_REG        0x2F
#define ADXL345_INT_SOURCE_REG     0x30
#define ADXL345_DATA_FORMAT_REG    0x31
#define ADXL345_DATAX0_REG         0x32
#define ADXL345_DATAX1_REG         0x33
#define ADXL345_DATAY0_REG         0x34
#define ADXL345_DATAY1_REG         0x35
#define ADXL345_DATAZ0_REG         0x36
#define ADXL345_DATAZ1_REG         0x37
#define ADXL345_FIFO_CTL           0x38
#define ADXL345_FIFO_STATUS        0x39

//Data rate codes.
#define ADXL345_3200HZ      0x0F
#define ADXL345_1600HZ      0x0E
#define ADXL345_800HZ       0x0D
#define ADXL345_400HZ       0x0C
#define ADXL345_200HZ       0x0B
#define ADXL345_100HZ       0x0A
#define ADXL345_50HZ        0x09
#define ADXL345_25HZ        0x08
#define ADXL345_12HZ5       0x07
#define ADXL345_6HZ25       0x06

#define ADXL345_SPI_READ    0x80
#define ADXL345_SPI_WRITE   0x00
#define ADXL345_MULTI_BYTE  0x60

#define ADXL345_X           0x00
#define ADXL345_Y           0x01
#define ADXL345_Z           0x02

#define CS_TOUCH_ON   (GPIOB->BSRR =GPIO_BSRR_BR7)    //negative logic
#define CS_TOUCH_OFF  (GPIOB->BSRR =GPIO_BSRR_BS7)    //negative logic

void initSpi1();

#endif /* __SPI_H */
