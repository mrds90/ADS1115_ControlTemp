/*=============================================================================
 * Author: Marcos Dominguez <mrds0690@gmail.com>
 * Date: 2021/10/08
 * Version: 0.0.1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "app.h"
#include "sapi.h"
#include "../inc/ads111x_driver.h"
#include "../inc/port.h"
#include "sapi_i2c.h"
/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/
static ads111x_obj_t ads1115_0; //!< instance of ADS1115
static volatile uint16_t adc_value;
/*=====[Main function, program entry point after power on or reset]==========*/

int main(void)
{
   bool flag = true;
	boardInit();
   ads111x_i2c_t ads111x_port = PORT_Init();
   ADS111x_Init(&ads1115_0, ADS111X_ADDR_0, ADS111X_PGA_4096, ADS1115, &ads111x_port);
   ADS111x_SetDataRate(&ads1115_0, ADS111X_DATA_RATE_16SPS);
   ADS111x_SetMultiplexer(&ads1115_0, ADS111X_MUX_AN0_AN1);//Read only one channel
   ADS111x_SetMode(&ads1115_0, ADS111X_MODE_CONTINUOUS);
   ADS111x_SetComparatorQueue(&ads1115_0, ADS111X_COMP_QUE_2_CONV);
   ADS111x_SetThresholdLow(&ads1115_0, 0x1FFF); 
   ADS111x_SetThresholdHigh(&ads1115_0, 0x3FFF);
   ADS111x_StartConversion(&ads1115_0);
   while(TRUE) {
      adc_value = ADS111x_Read(&ads1115_0);
      if (adc_value > 0x3FFF) {
         gpioWrite(LED2, ON);
      } else {
         gpioWrite(LED2, OFF);
      }
      if (adc_value < 0x1FFF) {
         gpioWrite(LED3, ON);
      } else {
         gpioWrite(LED3, OFF);
      }
      if (!gpioRead(GPIO1)) {
         gpioWrite(LED1, ON);
      } 
      else {
         gpioWrite(LED1, OFF);
      }
      delay(1);
      // ADS111x_StartConversion(&ads1115_0);
      // delay(2);
   }
   
   
   return 0;
}

/*
bool_t i2cWriteRead( i2cMap_t i2cNumber,
                     uint8_t  i2cSlaveAddress,
                     uint8_t* dataToReadBuffer,
                     uint16_t dataToReadBufferSize,
                     bool_t   sendWriteStop,
                     uint8_t* receiveDataBuffer,
                     uint16_t receiveDataBufferSize,
                     bool_t   sendReadStop )
*/