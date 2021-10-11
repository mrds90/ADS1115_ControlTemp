/*=============================================================================
 * Author: Marcos Dominguez <mrds0690@gmail.com>
 * Date: 2021/10/08
 * Version: 0.0.1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "app.h"
#include "sapi.h"
#include "../inc/port.h"
#include "../inc/button.h"

/*=====[Definition macros of private constants]==============================*/
#define MAX_TEMP 50
#define TEMP_2_CELSIUS_IN_COUNTS 1300
#define TIME_TO_WAIT_FOR_CONV_IN_MS 1000
/*=====[Definition of private functions]==============================*/
/**
 * @brief Convert the raw data from the ADC to a temperature in Celsius
 * 
 * @param counts     The raw data from the ADC
 * @return uint16_t  The temperature in Celsius
 */
static uint16_t ConutsToTemp(uint16_t counts);
/**
 * @brief Convert temperature in Celsius to a raw data for the ADC
 * 
 * @param temp       Temperature in Celsius
 * @return uint16_t  The raw data for the ADC
 */
static uint16_t TempToCounts(uint16_t temp);
/**
 * @brief Set the New Set Treshold value
 * 
 * @param new_setpoint  The new setpoint value in Celsius
 */
static void SetNewSetTreshold(uint16_t new_setpoint);
/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/
static ads111x_obj_t ads1115_0; //!< instance of ADS1115
static uint16_t temperature_setpoint = 24; //!< The default temperature setpoint in Celsius
/*=====[Main function, program entry point after power on or reset]==========*/

int main(void)
{
   bool flag = true;
   boardInit();
   FSMButtonInit(TEC1);
   FSMButtonInit(TEC2);
   ads111x_i2c_t ads111x_port = PORT_Init();
   ADS111x_Init(&ads1115_0, ADS111X_ADDR_0, ADS111X_PGA_4096, ADS1115, &ads111x_port);
   ADS111x_SetDataRate(&ads1115_0, ADS111X_DATA_RATE_16SPS);
   ADS111x_SetMultiplexer(&ads1115_0, ADS111X_MUX_AN0_GND);//Read only one channel
   ADS111x_SetMode(&ads1115_0, ADS111X_MODE_SINGLE);
   ADS111x_SetComparatorQueue(&ads1115_0, ADS111X_COMP_QUE_2_CONV);
   SetNewSetTreshold(temperature_setpoint);
   ADS111x_StartConversion(&ads1115_0);
   delay_t wait_time;
   delayConfig(&wait_time, TIME_TO_WAIT_FOR_CONV_IN_MS);
   while(TRUE) {
      if(delayRead(&wait_time)) {
         ADS111x_StartConversion(&ads1115_0);
      }
      if(CheckFallState(TEC1)) { //Decrease temperature setpoint
         temperature_setpoint--;
         if(temperature_setpoint < 0) {
            temperature_setpoint = 0;
         }
         SetNewSetTreshold(temperature_setpoint);
      }
      if(CheckFallState(TEC2)) { //Increase temperature setpoint
         temperature_setpoint++;
         if(temperature_setpoint > MAX_TEMP) {
            temperature_setpoint = MAX_TEMP;
         }
         SetNewSetTreshold(temperature_setpoint);
      }
      uint16_t curret_temp = ConutsToTemp(ADS111x_Read(&ads1115_0));

      if( curret_temp > temperature_setpoint + 2) { //!< temperature is too high
         gpioWrite(LED2, ON);
      }
      else { //!< temperature is not higher than setpoint
         gpioWrite(LED2, OFF);
      }
      if(curret_temp < temperature_setpoint - 2) { //!< temperature is lower than the setpoint
         gpioWrite(LED3, ON);
      }
      else { //!< temperature is not lower than setpoint
         gpioWrite(LED3, OFF);
      }
      if(!gpioRead(GPIO1)) { //!< alarm pin is set
         gpioWrite(LED1, ON); //!< turn on the refrigirator
      }
      else {   //!< alarm pin is not set
         gpioWrite(LED1, OFF); //!< turn off the refrigirator
      }
   }
   
   return 0;
}


static uint16_t ConutsToTemp(uint16_t counts) {
   return (counts * MAX_TEMP) / 0x7FFF;
}

static uint16_t TempToCounts(uint16_t temp) {
   return (temp * 0x7FFF) / MAX_TEMP;
}

static void SetNewSetTreshold(uint16_t new_setpoint) {
   uint16_t thesh_lo = TempToCounts(new_setpoint) - TEMP_2_CELSIUS_IN_COUNTS;
   uint16_t thesh_hi = thesh_lo + 2 * TEMP_2_CELSIUS_IN_COUNTS;
   ADS111x_SetThresholdLow(&ads1115_0, thesh_lo);
   ADS111x_SetThresholdHigh(&ads1115_0, thesh_hi);
}