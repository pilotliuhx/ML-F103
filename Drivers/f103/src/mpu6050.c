#include "mpu6050.h"
#include "i2c.h"
#include "cmsis_os.h"

#define MPU_ERROR 		I2C_ERROR
#define MPU_INFO 		I2C_INFO
/**
  * @brief   д���ݵ�MPU6050�Ĵ���
  * @param   reg_add:�Ĵ�����ַ
	* @param	 reg_data:Ҫд�������
  * @retval  
  */
void MPU6050_WriteReg(uint8_t reg_add,uint8_t reg_dat)
{
	Sensors_I2C_WriteRegister(MPU6050_ADDRESS,reg_add,1,&reg_dat);
}

/**
  * @brief   ��MPU6050�Ĵ�����ȡ����
  * @param   reg_add:�Ĵ�����ַ
	* @param	 Read���洢���ݵĻ�����
	* @param	 num��Ҫ��ȡ��������
  * @retval  
  */
void MPU6050_ReadData(uint8_t reg_add,unsigned char* Read,uint8_t num)
{
	Sensors_I2C_ReadRegister(MPU6050_ADDRESS,reg_add,num,Read);
}


/**
  * @brief   ��ʼ��MPU6050оƬ
  * @param   
  * @retval  
  */
void MPU6050_Init(void)
{
	//�ڳ�ʼ��֮ǰҪ��ʱһ��ʱ�䣬��û����ʱ����ϵ�����ϵ����ݿ��ܻ����
	osDelay(100);
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	    //�������״̬
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //�����ǲ�����
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x08);	  //���ü��ٶȴ�����������4Gģʽ
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
	osDelay(200);
}

/**
  * @brief   ��ȡMPU6050��ID
  * @param   
  * @retval  ��������1���쳣����0
  */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //��������ַ
	if(Re != 0x68)
	{
		return 0;
	}
	else
	{
		return 1;
	}
		
}

/**
  * @brief   ��ȡMPU6050�ļ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadAcc(float *accData)
{
    uint8_t buf[6];
    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    accData[0] = ((int16_t)(buf[0] << 8) | buf[1]) * MPU6500_G_PER_LSB_4;
    accData[1] = ((int16_t)(buf[2] << 8) | buf[3]) * MPU6500_G_PER_LSB_4;
    accData[2] = ((int16_t)(buf[4] << 8) | buf[5]) * MPU6500_G_PER_LSB_4;
}

/**
  * @brief   ��ȡMPU6050�ĽǼ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadGyro(float *gyroData)
{
    uint8_t buf[6];
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief   ��ȡMPU6050��ԭʼ�¶�����
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
	uint8_t buf[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
    *tempData = (buf[0] << 8) | buf[1];
}

/**
  * @brief   ��ȡMPU6050���¶����ݣ�ת�������϶�
  * @param   
  * @retval  
  */
void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	uint8_t buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
    temp3= (buf[0] << 8) | buf[1];	
	*Temperature=((double) temp3/340.0)+36.53;

}
