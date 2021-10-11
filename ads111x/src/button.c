/*=============================================================================
 * Author: Marcos Dominguez <mrds0690@gmail.com>
 * Date: 2021/08/01
 * Version: 1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "button.h"
#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/
#define PERIODO_1S              1000 
#define PERIODO_500MS           500 
#define PERIODO_2S              2000 
#define PERIODO_3S              3000
#define LED_OFF                 0

/*=====[Definition of private data type]==============================*/
typedef enum {
    UP,
    FALLING,
    DOWN,
    RISING,

    DEBOUNCE_QTY,
} debounce_state_t;
    
typedef struct {
   gpioMap_t tecla;
   delay_t delay;
   debounce_state_t state;
   bool_t flag_rise;
   bool_t flag_fall;
} dbn_t; 	

/*=====[Definitions of private global variables]=============================*/
static dbn_t teclas[TEC4-TEC1+1];

/*=====[Delaration of private functions]==================================*/
static void UpState(dbn_t *dataTecla);

static void FallingState(dbn_t *dataTecla);

static void DownState(dbn_t *dataTecla);

static void RisingState(dbn_t *dataTecla);

static void buttonReleased(dbn_t *tecla);

static void buttonPressed(dbn_t *tecla);

/*=====[Implematation of public functions]==================================*/
void FSMButtonInit(gpioMap_t index_tecla) {
   uint8_t array_index = index_tecla - TEC1;
   teclas[array_index].tecla = index_tecla;
   teclas[array_index].state = UP;
   teclas[array_index].flag_rise = FALSE;
   teclas[array_index].flag_fall = FALSE;
}

void FSMButtonUpdate(gpioMap_t index_tecla) {
   uint8_t array_index = index_tecla - TEC1;
   teclas[array_index].tecla = index_tecla;
   void (*AntiReboteFunc[DEBOUNCE_QTY])(dbn_t *dataTecla) = {UpState, FallingState ,DownState, RisingState};
   AntiReboteFunc[teclas[array_index].state](&teclas[array_index]);
}

bool_t CheckRaiseState(gpioMap_t index_tecla) { 
   uint8_t array_index = index_tecla - TEC1;
   FSMButtonUpdate(index_tecla);
   bool_t flag = teclas[array_index].flag_rise;   
   teclas[array_index].flag_rise = 0;
   return flag;
}

bool_t CheckFallState(gpioMap_t index_tecla) { 
   uint8_t array_index = index_tecla - TEC1;
   FSMButtonUpdate(index_tecla);
   bool_t flag = teclas[array_index].flag_fall;
   teclas[array_index].flag_fall = 0;
   return flag;
}
/*=====[Implematation of private functions]==================================*/
static void UpState(dbn_t *dataTecla) {
   if (!gpioRead(dataTecla->tecla)) {
            dataTecla->state = FALLING;
   }
}

static void FallingState(dbn_t *dataTecla) {
   if (!dataTecla->delay.running) {
      delayConfig(&dataTecla->delay,50);
   }
   if (delayRead(&dataTecla->delay)) {
      if (!gpioRead(dataTecla->tecla)) {
         dataTecla->state = DOWN;
         buttonPressed(dataTecla);
      }
      else {
         dataTecla->state = UP;
      }
   }
}

static void DownState(dbn_t *dataTecla) {
   if (gpioRead(dataTecla->tecla)) {
      dataTecla->state = RISING;
   }
}

static void RisingState(dbn_t *dataTecla) {
   if (!dataTecla->delay.running) {
      delayConfig(&dataTecla->delay,50);
   }
   if (delayRead(&dataTecla->delay)) {
      if (gpioRead(dataTecla->tecla)) {
         dataTecla->state = UP;
         buttonReleased(dataTecla);
      }
      else {
         dataTecla->state = DOWN;
      }
   }
}

static void buttonReleased(dbn_t *tecla) {
   tecla->flag_rise = 0xFF;
}

static void buttonPressed(dbn_t *tecla) {
   tecla->flag_fall = 0xFF;
}
