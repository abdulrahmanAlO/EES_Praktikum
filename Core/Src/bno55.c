#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include "bno055.h"

// Funktion, um den Sensor neu zu starten
void bno055_reset(IMU_Handle_t *imu_handle) {

	bno055_writeData(&(imu_handle->i2c_connect), BNO055_SYS_TRIGGER, BNO055_RESET_SYSTEM);
	HAL_Delay(700);
}

// Funktion, um den Sensor zu konfigurieren
void bno055_init(IMU_Handle_t *imu_handle){
	// ToDo: Aktivieren Sie Motion Shield Pin (Activation PIN)
	HAL_GPIO_WritePin(imu_handle->activation_pin.GPIOx, imu_handle->activation_pin.GPIO_Pin, GPIO_PIN_SET);
	//bno055_set(imu_handle);
	// Zurücksetzen des Sensors
	bno055_reset(imu_handle);
	HAL_GPIO_WritePin(imu_handle->activation_pin.GPIOx, imu_handle->activation_pin.GPIO_Pin, GPIO_PIN_SET);

//	bno055_writeData(&(imu_handle->i2c_connect), BNO055_SYS_TRIGGER, 0x0);
	// ToDo: Bringen Sie den Sensor in den Konfigurations-Modus
	bno055_setOperationMode(imu_handle,BNO055_OPERATION_MODE_CONFIG);
	HAL_Delay(20); // Wartezeit nach der Konfiguration
	// ToDo: Konfigurieren Sie die Einheiten für die Eulerwinkel und die Temperatur
	//bno055_writeData(imu_handle->i2c_connect,BNO055_UNIT_SEL,imu_handle->imu_config.euler_value);
	//bno055_writeData(imu_handle->i2c_connectBNO055_OPR_MODE,imu_handle->imu_config.mode_value);
	//bno055_writeData(imu_handle->i2c_connect,BNO055_UNIT_SEL,imu_handle->imu_config.temp_value);
	uint8_t x= imu_handle->imu_config.euler_value | imu_handle->imu_config.temp_value;
	bno055_writeData(&imu_handle->i2c_connect,BNO055_UNIT_SEL,x);

	//imu_handle->imu_config.euler_value=EULER_UNIT_DEGREES;
	//imu_handle->imu_config.temp_value=TEMP_UNIT_CELSIUS;
	//imu_handle->imu_config.mode_value=BNO055_OPERATION_MODE_CONFIG;

	HAL_Delay(20);
	// ToDo: Bringen Sie den Sensor in den im Konfigurations-Struct festgelegten Modus
	bno055_setOperationMode(imu_handle,imu_handle->imu_config.mode_value);
}

void bno055_deinit(IMU_Handle_t *imu_handle){
	bno055_reset(imu_handle);
	// Deaktivieren des Motion Shield Pins
	HAL_GPIO_WritePin(imu_handle->activation_pin.GPIOx, imu_handle->activation_pin.GPIO_Pin, GPIO_PIN_RESET);
}

// ToDo: Ergänzen Sie die Funktion, so dass der aktuelle Konfigurationsmodus zurückgegeben wird.
uint8_t bno055_getOperationMode(IMU_Handle_t *imu_handle){
	uint8_t mode=0;
	bno055_readData(&imu_handle->i2c_connect,BNO055_OPR_MODE,&mode,1);
	return mode;
//return imu_handle->imu_config.mode_value;

}

// ToDo: Ergänzen Sie die Funktion, so dass der Konfigurationsmodus auf den als mode übergebenen umgestellt wird.
void bno055_setOperationMode(IMU_Handle_t *imu_handle, uint8_t mode){
	bno055_writeData(&imu_handle->i2c_connect,BNO055_OPR_MODE,mode);

	HAL_Delay(20); // Wartezeit nach der Konfiguration
}

// ToDo: Ergänzen Sie die Funktion, so dass die Ausrichtung in der gewählten Einheit zurückgegeben wird.
float bno055_getHeading(IMU_Handle_t *imu_handle) {
	uint8_t value[2]={0};

	bno055_readData(&imu_handle->i2c_connect, BNO055_EUL_HEADING_LSB, &value,2);
	uint16_t x = ((uint16_t)value[1]<<8)|value[0];
	float y=x;
	if(imu_handle->imu_config.euler_value==EULER_UNIT_DEGREES){
	y/=16.0f;
	}else{
		y/=900.0f;
	}
	return y;

//return imu_handle->imu_config.euler_value;
}

// ToDo: Ergänzen Sie die Funktion, so dass die Temperatur in der gewählten Einheit zurückgegeben wird.
int16_t bno055_getTemp(IMU_Handle_t *imu_handle){
	int8_t tempratur=0;
	bno055_readData(&imu_handle->i2c_connect, BNO055_TEMP , &tempratur,1);
	int16_t ttempratur = tempratur;
	if(imu_handle->imu_config.temp_value==TEMP_UNIT_FAHRENHEIT){
		ttempratur = ttempratur *2;

		//return (int16_t) ttempratur;
	}
	return ttempratur;

}


// Hilfsfunktionen, um Daten an den Sensor zu senden oder zu empfangen
HAL_StatusTypeDef bno055_writeData(I2C_Handle_t *i2c_handle, uint8_t reg, uint8_t data) {
	uint8_t txdata[2] = {reg, data};
	uint8_t status;
	status = HAL_I2C_Master_Transmit(i2c_handle->i2c_handle, i2c_handle->i2c_address << 1, txdata, sizeof(txdata), 10);
	return status;
}

HAL_StatusTypeDef bno055_readData(I2C_Handle_t *i2c_handle, uint8_t reg, uint8_t *data, uint8_t len) {
	uint8_t status;
	status = HAL_I2C_Master_Transmit(i2c_handle->i2c_handle, i2c_handle->i2c_address << 1, &reg, 1, 100);
	if (status == HAL_OK){
		status = HAL_I2C_Master_Receive(i2c_handle->i2c_handle, i2c_handle->i2c_address << 1, data, len, 100);
	}
	return status;
}







