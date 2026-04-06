#ifndef BNO055_H_
#define BNO055_H_

#include "stm32f4xx_hal.h"

// ToDo: Ergänzen Sie die Defines mit den Werten für die Einheiten und das Reset des Sensors

#define EULER_UNIT_DEGREES      0x00
#define EULER_UNIT_RAD          0x04
#define TEMP_UNIT_CELSIUS       0x00
#define TEMP_UNIT_FAHRENHEIT    0x10
#define BNO055_RESET_SYSTEM     0x20

// ToDo: Ergänzen Sie die Defines mit den Registeradressen
// Register definitions
#define BNO055_UNIT_SEL    			0x3B
#define BNO055_OPR_MODE   			0x3D
#define BNO055_EUL_HEADING_LSB    	 0x1A
#define BNO055_EUL_HEADING_MSB   	0x1B
#define BNO055_EUL_ROLL_LSB   		0x1C
#define BNO055_EUL_ROLL_MSB   		 0x1D
#define BNO055_EUL_PITCH_LSB  		 0x1E
#define BNO055_EUL_PITCH_MSB  		 0x1F
#define BNO055_TEMP          		 0x34
#define BNO055_SYS_TRIGGER  		 0x3F

// ToDo: Ergänzen Sie die Defines für alle Operationsmodi
// Operating Modes
#define BNO055_OPERATION_MODE_CONFIG		0x00

// Sensor Modes
#define BNO055_Sensor_ACCONLY_MODE   0x01
#define BNO055_Sensor_MAGONLY_MODE   0x02
#define BNO055_Sensor_GYROONLY_MODE   0x03
#define BNO055_Sensor_ACCMAG_MODE   0x04
#define BNO055_Sensor_ACCGYRO_MODE   0x05
#define BNO055_Sensor_MAGGYRO_MODE   0x06
#define BNO055_Sensor_AMG_MODE   0x07
// Fusion Modes
#define BNO055_FUSION_IMU_MODE      0x08
#define BNO055_FUSION_COMPASS_MODE      0x09
#define BNO055_FUSION_M4G_MODE      0x0A
#define BNO055_FUSION_NDOF_FMC_OFF_MODE   0x0B
#define BNO055_FUSION_NDOF_MODE   0x0C
/* Defines die gebraucht werden + enums für P6
 */

enum ZielWinkel{

	zweihunder=0,
	hundertachtiger=1,
	neuziger=2,
	NULL_GERAD=3

};
/*
 * Handle structs for sensor interaction
 */
// ToDo: Ergänzen Sie das struct, so dass es
//			- den Modus
//			- die Einheit der Eulerwinkel
// 			- die Einheit der Temperatur enthält
typedef struct
{
	uint8_t euler_value;
	uint8_t temp_value;
	uint8_t mode_value;


}IMU_Config_t;

typedef struct
{
	I2C_HandleTypeDef *i2c_handle;
	uint8_t i2c_address;
}I2C_Handle_t;

typedef struct
{
	GPIO_TypeDef *GPIOx;
	uint32_t GPIO_Pin;
}GPIO_Handle_t;

typedef struct
{
	I2C_Handle_t i2c_connect;
	GPIO_Handle_t activation_pin;
	IMU_Config_t imu_config;
}IMU_Handle_t;


void bno055_init(IMU_Handle_t *imu_handle);
void bno055_reset(IMU_Handle_t *imu_handle);
void bno055_deinit(IMU_Handle_t *imu_handle);

uint8_t bno055_getOperationMode(IMU_Handle_t *imu_handle);
void bno055_setOperationMode(IMU_Handle_t *imu_handle, uint8_t mode);

float bno055_getHeading(IMU_Handle_t *imu_handle);
int16_t bno055_getTemp(IMU_Handle_t *imu_handle);


HAL_StatusTypeDef bno055_writeData(I2C_Handle_t *i2c_handle, uint8_t reg, uint8_t data);
HAL_StatusTypeDef bno055_readData(I2C_Handle_t *i2c_handle, uint8_t reg, uint8_t *data, uint8_t len);


#endif  // BNO055_H_
