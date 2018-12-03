#ifndef __SOFT_I2C_H__
#define __SOFT_I2C_H__

struct soft_i2c {
//	int speed; //only support std mode 100K
	int pin_mode_od;
	int scl_pin;
	int sda_pin;
	void (*pin_direction_func)(int pin, int input);
	void (*pin_set_func)(int pin, int high);
	int (*pin_get_func)(int pin);
};

int soft_i2c_xfer(struct soft_i2c *i2c, unsigned char slave_addr, unsigned char *wbuf, int wlen, unsigned char *rbuf, int rlen);
#endif /* __SOFT_I2C_H__ */
