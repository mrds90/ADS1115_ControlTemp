/*=============================================================================
 * Author: Marcos Dominguez <mrds0690@gmail.com>
 * Date: 2021/08/01
 * Version: 1
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __BUTTON_H__
#define __BUTTON_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include <stdint.h>
#include <stddef.h>
#include "sapi.h"


/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Prototypes (declarations) of public functions]=======================*/
/**
 * @brief Inicia la maquina de estados de teclas
 * 
 */
void FSMButtonInit(gpioMap_t index_tecla);
/**
 * @brief Actualiza la maquina de estados de teclas
 * 
 * @param index_tecla 
 */
void FSMButtonUpdate(gpioMap_t index_tecla);
/**
 * @brief Checkea si una tecla se ha soltado
 * 
 * @param tecla 
 */
bool_t CheckRaiseState(gpioMap_t index_tecla);
/**
 * @brief Checkea si una tecla se ha presionado
 * 
 * @param tecla 
 */
bool_t CheckFallState(gpioMap_t index_tecla);
/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __BUTTON_H__ */
