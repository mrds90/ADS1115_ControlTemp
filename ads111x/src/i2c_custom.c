/*=============================================================================
 * Author: Marcos Dominguez <mrds0690@gmail.com>
 * Date: 2021/10/08
 * Version: 1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "../inc/i2c_custom.h"
#include "../../../../libs/lpc_open/lpc_chip_43xx/inc/i2cm_18xx_43xx.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of private functions]====================================*/



/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Implementation of public functions]==================================*/
bool i2c_Init(I2C_ID_T i2cNumber, uint32_t clockRateHz){
    if( i2cNumber != I2C0 ) {
        return false;
    }
   // Configuracion de las lineas de SDA y SCL de la placa
    Chip_SCU_I2C0PinConfig( I2C0_STANDARD_FAST_MODE ); // Equal for CIAA-NXP and EDU-CIAA-NXP on I2C0
   // Inicializacion del periferico
    Chip_I2C_Init( i2cNumber );
   // Seleccion de velocidad del bus
    Chip_I2C_SetClockRate( i2cNumber, clockRateHz );
    Chip_I2C_SetMasterEventHandler(I2C0, Chip_I2C_EventHandler);
    NVIC_EnableIRQ(I2C0_IRQn);
}

int i2c_MasterSend(uint8_t slaveAddr, const uint8_t *buff, uint8_t len) {
    return Chip_I2C_MasterSend( I2C0, slaveAddr, buff, len );
}

int i2c_MasterReceive(uint8_t slaveAddr, uint8_t reg, uint8_t *buff, uint8_t len) {
    return Chip_I2C_MasterCmdRead( I2C0, slaveAddr, reg, buff, len );
}


/*=====[Implementations of private functions]================================*/

/*=====[Implementations of interrupt functions]==============================*/

void I2C0_IRQHandler(void) {
    Chip_I2C_MasterStateHandler(I2C0);
}
