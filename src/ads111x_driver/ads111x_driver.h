/*=============================================================================
 * Author: Marcos Dominguez <mrds0690@gmail.com>
 * Date: 2021/07/02
 * Version: 1
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __ADS111X_DRIVER_H__
#define __ADS111X_DRIVER_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include <stdint.h>
#include <stddef.h>
#include "sapi.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

#define ADS111x_CONFIG_ERROR      0xFF
#define ADS111x_CONFIG_SUCCESS    0x00


/*=====[Public function-like macros]=========================================*/


/*=====[Definitions of public data types]====================================*/

typedef struct {
    void (*StartWrite)(int16_t);
    void (*WriteByte)(uint8_t);
    void (*StartRead)(int16_t, uint8_t);
    uint8_t (*ReadByte)(void);
    void (*Stop)(void);
} ads111x_i2c_t;

    

} ads111x_comm_func_t;
typedef enum {
    ADS1113 = 1,
    ADS1114,
    ADS1115
} ads111x_device_t;

typedef enum {
    ADS111X_ADDR_0 = 0x48, //
    ADS111X_ADDR_1 = 0x49,
    ADS111X_ADDR_2 = 0x4A,
    ADS111X_ADDR_3 = 0x4B,
} ads111x_addr_t;

typedef enum {
    ADS111X_CONVERSION_REG = 0x00,
    ADS111X_CONFIG_REG = 0x01,
    ADS111X_LO_THRESH_REG = 0x02,
    ADS111X_HI_THRESH_REG = 0x03,
} ads111x_reg_t;
typedef enum {
    ADS111X_PGA_6144 = 0x00,
    ADS111X_PGA_4096 = 0x01,
    ADS111X_PGA_2048 = 0x02,
    ADS111X_PGA_1024 = 0x03,
    ADS111X_PGA_0512 = 0x04,
    ADS111X_PGA_0256 = 0x05,
} ads111x_pga_t;

typedef enum {
    ADS111X_MODE_CONTINUOUS = 0x00,
    ADS111X_MODE_SINGLE = 0x01,
} ads111x_mode_t;

typedef enum {
    ADS111X_DATA_RATE_8SPS = 0x00,
    ADS111X_DATA_RATE_16SPS = 0x01,
    ADS111X_DATA_RATE_32SPS = 0x02,
    ADS111X_DATA_RATE_64SPS = 0x03,
    ADS111X_DATA_RATE_128SPS = 0x04,
    ADS111X_DATA_RATE_250SPS = 0x05,
    ADS111X_DATA_RATE_475SPS = 0x06,
    ADS111X_DATA_RATE_860SPS = 0x07,
} ads111x_data_rate_t;

typedef enum {
    ADS111X_COMP_MODE_TRADITIONAL = 0x00,
    ADS111X_COMP_MODE_WINDOW = 0x01,
} ads111x_comp_mode_t;

typedef enum {
    ADS111X_COMP_POL_ACTIVE_LOW = 0x00,
    ADS111X_COMP_POL_ACTIVE_HIGH = 0x01,
} ads111x_comp_pol_t;

typedef enum {
    ADS111X_COMP_LAT_NON_LATCHING = 0x00,
    ADS111X_COMP_LAT_LATCHING = 0x01,
} ads111x_comp_lat_t;

typedef enum {
    ADS111X_COMP_QUE_1_CONV = 0x00,
    ADS111X_COMP_QUE_2_CONV = 0x01,
    ADS111X_COMP_QUE_4_CONV = 0x02,
    ADS111X_COMP_QUE_DISABLE = 0x03,
} ads111x_comp_que_t;

typedef enum {
    ADS111X_MUX_AN0_AN1 = 0x00,
    ADS111X_MUX_AN0_AN3 = 0x01,
    ADS111X_MUX_AN1_AN3 = 0x02,
    ADS111X_MUX_AN2_AN3 = 0x03,
    ADS111X_MUX_AN0_GND = 0x04,
    ADS111X_MUX_AN1_GND = 0x05,
    ADS111X_MUX_AN2_GND = 0x06,
    ADS111X_MUX_AN3_GND = 0x07,
} ads111x_mux_t;
typedef enum {
    ADS111X_DO_NOTHING = 0x00,
    ADS111X_START_CONVERSION = 0x01,
} ads111x_os_t;
typedef enum {
    ADS111X_CONFIG_FIELD_COMP_QUE,
    ADS111X_CONFIG_FIELD_COMP_LAT,
    ADS111X_CONFIG_FIELD_COMP_POL,
    ADS111X_CONFIG_FIELD_COMP_MODE,
    ADS111X_CONFIG_FIELD_DR,
    ADS111X_CONFIG_FIELD_MODE,
    ADS111X_CONFIG_FIELD_PGA,
    ADS111X_CONFIG_FIELD_MUX,
    ADS111X_CONFIG_FIELD_OS,

    ADS111X_CONFIG_FIELD_QTY,
}ads111x_config_field_t;

typedef struct {
    ads111x_device_t device;
    ads111x_addr_t i2c_address;
    uint16_t configuration;
} ads111x_obj_t;

/*=====[Prototypes (declarations) of public functions]=======================*/

uint8_t ADS111x_Init(ads111x_obj_t *ptr_asd111x, ads111x_addr_t i2c_address, ads111x_pga_t gain, ads111x_device_t device, ads111x_i2c_t *port);

void ADS111x_StartConversion(ads111x_obj_t *ptr_asd111x);

int16_t ADS111x_Read(ads111x_obj_t *ptr_asd111x);

void ADS111x_SetThresholdLow(ads111x_obj_t *ptr_asd111x, uint16_t threshold);

void ADS111x_SetThresholdHigh(ads111x_obj_t *ptr_asd111x, uint16_t threshold);

void ADS111x_SetMultiplexer(ads111x_obj_t *ptr_asd111x, ads111x_mux_t mux);

void ADS111x_SetGain(ads111x_obj_t *ptr_asd111x, ads111x_pga_t gain);

void ADS111x_SetDataRate(ads111x_obj_t *ptr_asd111x, ads111x_data_rate_t dr);

void ADS111x_SetMode(ads111x_obj_t *ptr_asd111x, ads111x_mode_t mode);

void ADS111x_SetComparatorMode(ads111x_obj_t *ptr_asd111x, ads111x_comp_mode_t mode);

void ADS111x_SetComparatorPolarity(ads111x_obj_t *ptr_asd111x, ads111x_comp_pol_t pol);

void ADS111x_SetComparatorLatching(ads111x_obj_t *ptr_asd111x, ads111x_comp_lat_t lat);

void ADS111x_SetComparatorQueue(ads111x_obj_t *ptr_asd111x, ads111x_comp_que_t que);

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __ADS111X_DRIVER_H__ */
