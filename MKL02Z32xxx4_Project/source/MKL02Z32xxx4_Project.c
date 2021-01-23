/**
 * @file    MKL02Z32xxx4_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL02Z4.h"
#include "fsl_debug_console.h"


#include "sdk_hal_gpio.h"
#include "sdk_hal_uart0.h"
#include "sdk_hal_i2c0.h"


#define MMA851_I2C_DEVICE_ADDRESS	0x1D
#define MMA8451_WHO_AM_I_MEMORY_ADDRESS		0x0D

#define MMA8451_OUT_X_MSB		0x01
#define MMA8451_OUT_X_LSB		0x02
#define MMA8451_OUT_Y_MSB		0x03
#define MMA8451_OUT_Y_LSB		0x04
#define MMA8451_OUT_Z_MSB		0x05
#define MMA8451_OUT_Z_LSB		0x06

#define ACCEL_CTRL_REG1 0x2A
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void) {
	uint16_t	nuevo_dato_i2c;
	uint8_t nuevo_byte_uart;
    status_t status;

 /*Variables donde se guardaran los valores de X, Y, Z */
    uint16_t toma_de_datos_xu;
   	uint16_t toma_de_datos_xl;
    uint16_t toma_de_datos_xt;

    uint16_t toma_de_datos_yu;
   	uint16_t toma_de_datos_yl;
    uint16_t toma_de_datos_yt;

    uint16_t toma_de_datos_zu;
    uint16_t toma_de_datos_zl;
    uint16_t toma_de_datos_zt;
  /*Fin de variables de toma de datos*/

  	/* Init board hardware. */

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    (void)uart0Inicializar(115200);
    (void)i2c0MasterInit(100000);

    PRINTF("Hello World\n");

    //resultado = gpioPutHigh(kPTB10);

    /* Force the counter to be placed into memory. */

    /* Enter an infinite loop, just incrementing a counter. */

    while(1) {
       	if(uart0CuantosDatosHayEnBuffer()>0){
       		status=uart0LeerByteDesdeBuffer(&nuevo_byte_uart);
       		if(status==kStatus_Success){
       			printf("dato:%c\r\n",nuevo_byte_uart);
       			switch (nuevo_byte_uart) {
   				case 'a':
   				case 'A':
   					gpioPutToggle(KPTB10);
   					break;

   				case 'v':
   					gpioPutHigh(KPTB7);
   					break;
   				case 'V':
   					gpioPutLow(KPTB7);
   					break;

   				case 'r':
   					gpioPutValue(KPTB6,1);
   					break;
   				case 'R':
   					gpioPutValue(KPTB6,0);
   					break;

   				case 'M':
   					i2c0MasterReadByte(&nuevo_dato_i2c, MMA851_I2C_DEVICE_ADDRESS, MMA8451_WHO_AM_I_MEMORY_ADDRESS);
   					if(nuevo_dato_i2c==0x1A){
   						i2c0MasterWriteByte(MMA851_I2C_DEVICE_ADDRESS, ACCEL_CTRL_REG1, 0x0D);
   						printf("MMA8451 ENCONTRADO!!");
   					}else{
   						printf("MMA8451 NO ENCONTRADO!!");
   					}
   					break;
   				case 'x':
   				case 'X':
   					i2c0MasterReadByte(&toma_de_datos_xu, MMA851_I2C_DEVICE_ADDRESS, MMA8451_OUT_X_MSB);
   					i2c0MasterReadByte(&toma_de_datos_xl, MMA851_I2C_DEVICE_ADDRESS, MMA8451_OUT_X_LSB);

   					i2c0MasterReadByte(&toma_de_datos_yu, MMA851_I2C_DEVICE_ADDRESS, MMA8451_OUT_Y_MSB);
   					i2c0MasterReadByte(&toma_de_datos_yl, MMA851_I2C_DEVICE_ADDRESS, MMA8451_OUT_Y_LSB);

   					i2c0MasterReadByte(&toma_de_datos_zu, MMA851_I2C_DEVICE_ADDRESS, MMA8451_OUT_Z_MSB);
   					i2c0MasterReadByte(&toma_de_datos_zl, MMA851_I2C_DEVICE_ADDRESS, MMA8451_OUT_Z_LSB);

   					toma_de_datos_xt = (toma_de_datos_xu<<6) | (toma_de_datos_xl >>2);
   					toma_de_datos_yt = (toma_de_datos_yu<<6) | (toma_de_datos_yl >>2);
   					toma_de_datos_zt = (toma_de_datos_zu<<6) | (toma_de_datos_zl >>2);

   					printf("X: %d Y: %d Z: %d\n\r", toma_de_datos_xt, toma_de_datos_yt, toma_de_datos_zt);

   					break;
       			}
       		}else{
       			printf("error\r\n");
       		}
       	}
       }
    return 0 ;
}
