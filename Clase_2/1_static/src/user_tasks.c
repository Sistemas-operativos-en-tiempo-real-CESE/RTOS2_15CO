/*=============================================================================
 * Copyright (c) 2021, Franco Bucafusco <franco_bucafusco@yahoo.com.ar>
 * 					   Martin N. Menendez <mmenendez@fi.uba.ar>
 * All rights reserved.
 * License: read LICENCE file
 * Date: 2021/10/03
 * Version: v1.2
 *===========================================================================*/

/*==================[inclusiones]============================================*/
#include <stdio.h>
#include "main.h"
#include "keys.h"
#include "user_tasks.h"
/*==================[definiciones de datos internos]=========================*/
const gpioMap_t leds_t[] = {LED2,LED3};
const gpioMap_t gpio_t[] = {GPIO3,GPIO1};
/*==================[definiciones de datos externos]=========================*/
extern QueueHandle_t cola_1;

/*==================[definiciones de funciones internas]=====================*/
void gpio_init( void )
{
    gpioInit( GPIO7, GPIO_OUTPUT );
    gpioInit( GPIO5, GPIO_OUTPUT );
    gpioInit( GPIO3, GPIO_OUTPUT );
    gpioInit( GPIO1, GPIO_OUTPUT );
}
/*==================[definiciones de funciones externas]=====================*/

// Implementacion de funcion de la tarea

void tarea_A( void* taskParmPtr )
{
    // ---------- CONFIGURACIONES ------------------------------
    TickType_t xPeriodicity =  MAX_RATE;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    // ---------- REPETIR POR SIEMPRE --------------------------

    while( TRUE )
    {
        gpioWrite( LEDB, ON );
        vTaskDelay( MAX_RATE / 2 );
        xQueueSend( cola_1, "LED ON",  portMAX_DELAY );
        gpioWrite( LEDB, OFF );

        vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
    }
}

void tarea_C( void* taskParmPtr )
{
    // ---------- CONFIGURACIONES ------------------------------
    TickType_t xPeriodicity =  PRINT_RATE;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    // ---------- REPETIR POR SIEMPRE --------------------------

    char mensaje[MAX_MESSAGE_SIZE];

    while( TRUE )
    {
        xQueueReceive( cola_1, &mensaje, portMAX_DELAY );			// Esperamos dato para imprimir

        uartWriteString( USED_UART, mensaje );
        uartWriteString( USED_UART, "\r\n" );

        vTaskDelay( PRINT_RATE );
    }
}

void user_keys_event_handler_button_release( t_key_isr_signal* event_data )
{
    /* aloco localmente un array para almacenar temporalmente el string */
    char mensaje[MAX_MESSAGE_SIZE];

    /* imprimo el mensaje */
    TickType_t dif = keys_get_diff( event_data->tecla );

    snprintf ( mensaje, FSM_MS_SIZE, FSM_MSG_T, event_data->tecla, dif );

    /* envio */
    xQueueSend( cola_1, mensaje,  portMAX_DELAY );
}
/*==================[fin del archivo]========================================*/
