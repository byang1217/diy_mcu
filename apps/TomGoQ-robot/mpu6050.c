#include "common.h"
#include "soft_i2c.h"

#define SCL_PIN	PB_6
#define SDA_PIN	PB_7

static struct soft_i2c s_i2c = {
	.pin_mode_od = 1,
	.scl_pin = PB_6,
	.sda_pin = PB_7,
	.pin_set_func = bsp_gpio_set,
	.pin_get_func = bsp_gpio_get,
};

int shell_soft_i2c_scan(int argc, char **argv)
{
	int addr;

	bsp_gpio_mode(s_i2c.scl_pin, GPIO_Mode_Out_OD);
	bsp_gpio_mode(s_i2c.sda_pin, GPIO_Mode_Out_OD);
	pr_log("start i2c scan ...\n");
	for (addr = 0x3; addr < 0x7f; addr ++) {
		if (soft_i2c_xfer(&s_i2c, addr, NULL, 0, NULL, 0) == 0)
			pr_log("Found dev, slave addr = 0x%x\n", addr);
	}
	pr_log("i2c scan end\n");
	return 0;
}

//****************************************
#define SMPLRT_DIV      0x19    //陀螺仪采样率，典型值：0x07(125Hz)
#define CONFIG          0x1A    //低通滤波频率，典型值：0x06(5Hz)
#define GYRO_CONFIG     0x1B    //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define ACCEL_CONFIG    0x1C    //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define ACCEL_XOUT_H    0x3B
#define ACCEL_XOUT_L    0x3C
#define ACCEL_YOUT_H    0x3D
#define ACCEL_YOUT_L    0x3E
#define ACCEL_ZOUT_H    0x3F
#define ACCEL_ZOUT_L    0x40
#define TEMP_OUT_H      0x41
#define TEMP_OUT_L      0x42
#define GYRO_XOUT_H     0x43
#define GYRO_XOUT_L     0x44   
#define GYRO_YOUT_H     0x45
#define GYRO_YOUT_L     0x46
#define GYRO_ZOUT_H     0x47
#define GYRO_ZOUT_L     0x48
#define PWR_MGMT_1      0x6B    //电源管理，典型值：0x00(正常启用)
#define WHO_AM_I            0x75    //IIC地址寄存器(默认数值0x68，只读)

int shell_mpu6050_test(int argc, char **argv)
{
#define MPU6050_SLAVE_ADDR 0x68
	int i;
	char rx_buf[16];
	char tx_buf[16];
	unsigned long start_us, end_us;

	bsp_gpio_mode(s_i2c.scl_pin, GPIO_Mode_Out_OD);
	bsp_gpio_mode(s_i2c.sda_pin, GPIO_Mode_Out_OD);

	tx_buf[0] = PWR_MGMT_1;
	tx_buf[1] = 0;
	soft_i2c_xfer(&s_i2c, MPU6050_SLAVE_ADDR, tx_buf, 2, NULL, 0);

	tx_buf[0] = SMPLRT_DIV;
	tx_buf[1] = 0x7;
	soft_i2c_xfer(&s_i2c, MPU6050_SLAVE_ADDR, tx_buf, 2, NULL, 0);

	tx_buf[0] = CONFIG;
	tx_buf[1] = 0x6;
	soft_i2c_xfer(&s_i2c, MPU6050_SLAVE_ADDR, tx_buf, 2, NULL, 0);

	tx_buf[0] = GYRO_CONFIG;
	tx_buf[1] = 0x18;
	soft_i2c_xfer(&s_i2c, MPU6050_SLAVE_ADDR, tx_buf, 2, NULL, 0);

	tx_buf[0] = ACCEL_CONFIG;
	tx_buf[1] = 0x01;
	soft_i2c_xfer(&s_i2c, MPU6050_SLAVE_ADDR, tx_buf, 2, NULL, 0);

	delay_ms(100);

/*
	tx_buf[0] = WHO_AM_I;
	memset(rx_buf, 0, sizeof(rx_buf));
	if (soft_i2c_xfer(&s_i2c, MPU6050_SLAVE_ADDR, tx_buf, 1, rx_buf, 1) == 0)
		pr_log("WHO_AM_I: %x\n", rx_buf[0]);
*/

	tx_buf[0] = ACCEL_XOUT_H;
	memset(rx_buf, 0, sizeof(rx_buf));
	if (soft_i2c_xfer(&s_i2c, MPU6050_SLAVE_ADDR, tx_buf, 1, rx_buf, 14) == 0) {
			pr_log("%010d, %010d, %010d, %010d, %010d, %010d, %010d\n",
				(rx_buf[0] << 8) + rx_buf[1],
				(rx_buf[2] << 8) + rx_buf[3],
				(rx_buf[4] << 8) + rx_buf[5],
				(rx_buf[6] << 8) + rx_buf[7],
				(rx_buf[8] << 8) + rx_buf[9],
				(rx_buf[10] << 8) + rx_buf[11],
				(rx_buf[12] << 8) + rx_buf[13]);
	}

	start_us = bsp_get_uptime_us();
	for (i = 0; i < 1000; i++)
		soft_i2c_xfer(&s_i2c, MPU6050_SLAVE_ADDR, tx_buf, 1, rx_buf, 14);
	end_us = bsp_get_uptime_us();
	pr_log("read 14 bytes for 1000 times: start=%dus, end=%dus, average=%dus\n", start_us, end_us, (end_us - start_us)/1000);

	return 0;
}
