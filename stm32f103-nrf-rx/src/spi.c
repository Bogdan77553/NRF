#include "stm32f10x_conf.h"
#include "spi.h"


void initSpi1()
{
//------------------------------------------------------------
//	SPI1
//------------------------------------------------------------
GPIO_InitTypeDef gpio_init;
SPI_InitTypeDef spi_init;

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
GPIO_StructInit(&gpio_init);

//Configure GPIO pin
                      //MISO       SCK          MOSI         //NSS
gpio_init.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_7; //GPIO_Pin_4 |
gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &gpio_init);
         //ÑS
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &gpio_init);
GPIO_SetBits(GPIOA, GPIO_Pin_1);


spi_init.SPI_Mode = SPI_Mode_Master;
spi_init.SPI_CPHA = SPI_CPHA_2Edge;//Ôàçà òàêòîâîãî ñèãíàëà
spi_init.SPI_CPOL = SPI_CPOL_High; //Ïîëÿðíîñòü òàêòîâîãî ñèãíàëà (Â íåàêòèâíîì ñîñòîÿíèè SCK = 1,òàêòîâûé ñèãíàë èìååò îòìåòî÷íûé óðîâåíü “0)
//ñîîòâåòñòâóåò äîêå íà ADXL "clock polarity (CPOL) = 1 and clock phase (CPHA) = 1".
spi_init.SPI_DataSize = SPI_DataSize_8b;
spi_init.SPI_FirstBit = SPI_FirstBit_MSB;
spi_init.SPI_NSS = SPI_NSS_Soft;
spi_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//Baud rate = Fpclk/256
// 36MHz/256=140kHz < 400kHz
spi_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
SPI_Init(SPI1, &spi_init);

SPI_Cmd(SPI1, ENABLE);
SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);

//------------------------------------------------------------
}

uint8_t spi_write (uint8_t byte) {
      //Wait for SPIy Tx buffer empty
while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
    ;
    SPI_I2S_SendData(SPI1, byte);

while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
    ;
return SPI_I2S_ReceiveData(SPI1);
 }


uint8_t ADXL345oneByteRead(uint8_t address) {


     // uint8_t tx = (ADXL345_SPI_READ | (address & 0x3F));
    uint8_t tx = address | ADXL345_SPI_READ;
    uint8_t rx = 0;

    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    //Send address to read from.
    spi_write(tx);
    //Read back contents of address.
    rx = spi_write(0x00);
    GPIO_SetBits(GPIOA, GPIO_Pin_1);


    return rx;

}


void ADXL345oneByteWrite(uint8_t address, char data) {

    //uint8_t tx = (ADXL345_SPI_WRITE | (address & 0x3F));
    uint8_t tx = address | ADXL345_SPI_WRITE;
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    spi_write(tx);   //Send address to write to.

    spi_write(data);  //Send data to be written.
    GPIO_SetBits(GPIOA, GPIO_Pin_1);

}

void ADXL345multiByteRead(uint8_t startAddress, char* buffer, uint8_t size) {

    uint8_t tx = (ADXL345_SPI_READ | ADXL345_MULTI_BYTE | (startAddress & 0x3F));

    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    //Send address to start reading from.
    spi_write(tx);
uint8_t i;
    for (i = 0; i < size; i++) {
        buffer[i] = spi_write(0x00);
    }

    GPIO_SetBits(GPIOA, GPIO_Pin_1);

}

void ADXL345multiByteWrite(uint8_t startAddress, char* buffer, uint8_t size) {

    uint8_t tx = (ADXL345_SPI_WRITE | ADXL345_MULTI_BYTE | (startAddress & 0x3F));

     GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    //Send address to start reading from.
    spi_write(tx);
     uint8_t i ;
    for (i = 0; i < size; i++) {
        buffer[i] = spi_write(0x00);
    }

     GPIO_SetBits(GPIOA, GPIO_Pin_1);

}

void tp_GPIO_init(){
   //ports SPI (A and B config)
RCC->APB2ENR    |= RCC_APB2ENR_IOPAEN;   //Âêëþ÷àåì Port A
RCC->APB2ENR    |= RCC_APB2ENR_IOPBEN;   //Âêëþ÷àåì Port B
//in-out config
//PA5 - SCL     - Alternative func. push-pull     (A)
//PB7 - CS - GPIO - soft        (4)
//PA6       - MOSI Alternative func.  push-pull- OUT  (A)
//PA7       - MISO Input floating / Input pull-up - IN
//PB6 - Interrupt        - input
GPIOA->CRL = 0xA4A44444;        // Port A (Low)  Bit 5,6  - alt func    7 - input
GPIOB->CRL = 0x24444444;        // Port B (Low) bit 7 - out, bit 6 - interrupt in

//SPI1 CR1 configure
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;     //Enable oscil SPI1
  SPI1->CR1 |= SPI_CR1_BR;                //Baud rate = Fpclk/256
  SPI1->CR1 &= ~SPI_CR1_CPOL;             //Polarity cls signal CPOL = 0;
  SPI1->CR1 &= ~SPI_CR1_CPHA;             //Phase cls signal    CPHA = 0;
  SPI1->CR1 &= ~SPI_CR1_DFF;              //8 bit data
  SPI1->CR1 &= ~SPI_CR1_LSBFIRST;         //MSB will be first
  SPI1->CR1 |= SPI_CR1_SSM;               // Program mode NSS
  SPI1->CR1 |= SPI_CR1_SSI;               // àííàëîãè÷íî ñîñòîÿíèþ, êîãäà NSS 1
  SPI1->CR2 |= SPI_CR2_SSOE;              //âûâîä NSS -  slave select
  SPI1->CR1 |= SPI_CR1_MSTR;              //Mode Master
  SPI1->CR1 |= SPI_CR1_SPE;               //Enable SPI1

//--------------------------
SPI1->CR2 = 0x00000000;  //No interrupt enable
}

unsigned int get_touch (uint8_t command)  //communication with TSC2046
{
int d_in;                //zero data in
CS_TOUCH_ON;             //chip select TOUCH
delay (10);
SPI1->DR = command;                   //send command
while(!(SPI1->SR & SPI_SR_RXNE));       //wait
d_in = SPI1->DR;                        //ignore this enter
SPI1->DR = 0;      //send zero bytes - cont receiving data from module
while(!(SPI1->SR & SPI_SR_RXNE));  //wait
d_in = SPI1->DR;        //receive high byte
d_in <<= 8;             //move it 8
SPI1->DR = 0;           //send zero bytes - cont receiving data from module
while(!(SPI1->SR & SPI_SR_RXNE));       //wait
d_in |= SPI1->DR;        //receive low byte
CS_TOUCH_OFF;
delay (10);
return d_in;
}
