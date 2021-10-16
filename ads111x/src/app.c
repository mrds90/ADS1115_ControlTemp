/*=============================================================================
 * Author: Marcos Dominguez <mrds0690@gmail.com>
 * Date: 2021/10/08
 * Version: 0.0.1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "app.h"
#include <stdlib.h>
#include "sapi.h"
#include "../inc/port.h"
#include "../inc/button.h"

/*=====[Definition macros of private constants]==============================*/
#define MAX_TEMP_D_CELSIUS 500
#define TEMP_2_D_CELSIUS 20
#define TEMP_1_D_CELSIUS 10
#define TIME_TO_WAIT_FOR_CONV_IN_MS 1000
/*=====[Definition of private functions]==============================*/
/**
 * @brief Initialize the ADC configuration for this custom application
 * 
 */
static void ADS1115_Init(void); 
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
static uint16_t temperature_setpoint = 240; //!< The default temperature setpoint in deci Celsius
/*=====[Main function, program entry point after power on or reset]==========*/

int main(void)
{
   boardInit();
   
   FSMButtonInit(TEC1); //!< Initialize the button to decrease the setpoint
   FSMButtonInit(TEC2); //!< Initialize the button to increase the setpoint
   FSMButtonInit(TEC3); //!< Initialize the button to increase the setpoint
   FSMButtonInit(TEC4); //!< Initialize the button to increase the setpoint

   ADS1115_Init();      //!< Initialize the ADC
   
   delay_t wait_time;   //!< Variable to wait for the conversion
   delayConfig(&wait_time, TIME_TO_WAIT_FOR_CONV_IN_MS); //!< Configure the delay
   uint16_t curret_temp; //!< Variable to store the current temperature
   while(TRUE) {
      if(delayRead(&wait_time)) {
         ADS111x_StartConversion(&ads1115_0); //!< Start the conversion
      }
      if(CheckFallState(TEC1)) { //Decrease temperature setpoint
         temperature_setpoint -= TEMP_1_D_CELSIUS;
         if(temperature_setpoint < 0) {
            temperature_setpoint = 0;
         }
         SetNewSetTreshold(temperature_setpoint);
      }
      if(CheckFallState(TEC2)) { //Increase temperature setpoint
         temperature_setpoint += TEMP_1_D_CELSIUS;
         if(temperature_setpoint > MAX_TEMP_D_CELSIUS) {
            temperature_setpoint = MAX_TEMP_D_CELSIUS;
         }
         SetNewSetTreshold(temperature_setpoint);
      }
      if(CheckFallState(TEC3)) { //Send the temperature to the UART
         /* routine to send the temperature by UART */
         char str[40];
         sprintf( str, "SetPoint: %i.\r\n", temperature_setpoint );
         uartWriteString(UART_USB, str);
         
      }
      if(CheckFallState(TEC4)) { //Send the temperature to the UART
         /* routine to send the temperature by UART */
         char str[40];
         sprintf( str, "Temperature: %i.\r\n", curret_temp );
         uartWriteString(UART_USB, str);
         
      }
      curret_temp = ConutsToTemp(ADS111x_Read(&ads1115_0));

      if( curret_temp > temperature_setpoint + TEMP_2_D_CELSIUS) { //!< temperature is too high
         gpioWrite(LED2, ON); //!< Turn on the LED tha indicates that the temperature is too high
      }
      else { //!< temperature is not higher than setpoint
         gpioWrite(LED2, OFF); //!< Turn off the LED tha indicates that the temperature is too high
      }
      if(curret_temp < temperature_setpoint - TEMP_2_D_CELSIUS) { //!< temperature is lower than the setpoint
         gpioWrite(LED3, ON); //!< Turn on the LED tha indicates that the temperature is ok
      }
      else { //!< temperature is not lower than setpoint
         gpioWrite(LED3, OFF); //!< Turn off the LED tha indicates that the temperature is ok
      }
      if(!gpioRead(GPIO1)) { //!< alarm pin is set
         gpioWrite(LED1, ON); //!< turn on the cooling system
      }
      else {   //!< alarm pin is not set
         gpioWrite(LED1, OFF); //!< turn off the cooling system
      }
   }
   
   return 0;
}


static void ADS1115_Init(void) {
   ads111x_i2c_t ads111x_port = PORT_Init();
   ADS111x_Init(&ads1115_0, ADS111X_ADDR_0, ADS111X_PGA_4096, ADS1115, &ads111x_port); //!< Initialize the ADS1115 with address 0x48, PGA = 4096, and the ADS1115 as the device
   ADS111x_SetDataRate(&ads1115_0, ADS111X_DATA_RATE_16SPS); //!< Set the data rate to 16 samples per second
   ADS111x_SetMultiplexer(&ads1115_0, ADS111X_MUX_AN0_GND); //!< Select the Analog input to be AN0
   ADS111x_SetMode(&ads1115_0, ADS111X_MODE_SINGLE); //!< Set the mode to single shot
   ADS111x_SetComparatorQueue(&ads1115_0, ADS111X_COMP_QUE_2_CONV); //!< Set the comparator queue to 2 conversions
   SetNewSetTreshold(temperature_setpoint); //!< Set the initial setpoint
   ADS111x_StartConversion(&ads1115_0); //!< First conversion
}

static uint16_t ConutsToTemp(uint16_t counts) {
   return (counts * MAX_TEMP_D_CELSIUS) / 0x7FFF; //!< Convert the raw data to a temperature in Celsius
}

static uint16_t TempToCounts(uint16_t temp) {
   return (temp * 0x7FFF) / MAX_TEMP_D_CELSIUS; //!< Convert the temperature in Celsius to a raw data for the ADC
}

static void SetNewSetTreshold(uint16_t new_setpoint) {
   uint16_t thesh_lo = TempToCounts(new_setpoint - TEMP_2_D_CELSIUS); //!< Set the low threshold to 2 degrees below the setpoint
   uint16_t thesh_hi = TempToCounts(new_setpoint + TEMP_2_D_CELSIUS)  ; //!< Set the high threshold to 2 degrees above the setpoints
   ADS111x_SetThresholdLow(&ads1115_0, thesh_lo);
   ADS111x_SetThresholdHigh(&ads1115_0, thesh_hi);
}