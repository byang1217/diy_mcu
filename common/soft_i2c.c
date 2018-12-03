#include "common.h"
#include "soft_i2c.h"

#define PIN_DELAY_US 2
#define CLK_HOLD_US 5
#define START_HOLD_US 4
#define STOP_HOLD_US 4

#define SCL(x) i2c->pin_set_func(i2c->scl_pin, x)
#define SCL_IN() do{if (!i2c->pin_mode_od) i2c->pin_direction_func(i2c->scl_pin, 1);}while(0)
#define SCL_OUT() do{if (!i2c->pin_mode_od) i2c->pin_direction_func(i2c->scl_pin, 0);}while(0)
#define SCL_VAL() i2c->pin_get_func(i2c->scl_pin)
#define SDA(x) i2c->pin_set_func(i2c->sda_pin, x)
#define SDA_IN() do{if (!i2c->pin_mode_od) i2c->pin_direction_func(i2c->sda_pin, 1);}while(0)
#define SDA_OUT() do{if (!i2c->pin_mode_od) i2c->pin_direction_func(i2c->sda_pin, 0);}while(0)
#define SDA_VAL() i2c->pin_get_func(i2c->sda_pin)

static void __start(struct soft_i2c *i2c)
{
	SCL_OUT();
	SDA_OUT();
	SCL(1);
	SDA(1);

	SCL_IN();
	delay_us(PIN_DELAY_US);
	while (SCL_VAL() == 0)
		;
	SCL_OUT();

	SDA(0);
	delay_us(START_HOLD_US);
	SCL(0);
	delay_us(CLK_HOLD_US);
}

static void __stop(struct soft_i2c *i2c)
{
	SDA(0);
	delay_us(CLK_HOLD_US);
	SCL(1);
	delay_us(STOP_HOLD_US);
	SDA(1);
	delay_us(CLK_HOLD_US);
}

// return 0 if got ack
static int __write(struct soft_i2c *i2c, unsigned char val)
{
	int i;
	int ret;

	for (i = 0; i < 8; i++) {
		SDA(val & (0x80 >> i));
		SCL(1);
		delay_us(CLK_HOLD_US);
		SCL(0);
		delay_us(CLK_HOLD_US);
	}

	//ack
	SDA(1);
	SDA_IN();
	SCL(1);
	delay_us(CLK_HOLD_US);
	ret = SDA_VAL();
	SDA_OUT();
	SCL(0);
	delay_us(CLK_HOLD_US);

	return ret;
}

static unsigned char __read(struct soft_i2c *i2c, int last)
{
	int i;
	int ret = 0;

	SDA(1);
	SDA_IN();
	for (i = 0; i < 8; i++) {
		ret <<= 1;
		SCL(1);
		delay_us(CLK_HOLD_US);
		if (SDA_VAL())
			ret |= 1;
		SCL(0);
		delay_us(CLK_HOLD_US);
	}
	SDA_OUT();

	if (last)
		SDA(1); //ack
	else
		SDA(0); //nack
	SCL(1);
	delay_us(CLK_HOLD_US);
	SCL(0);
	SDA(1);
	delay_us(CLK_HOLD_US);

	return ret;
}

int soft_i2c_xfer(struct soft_i2c *i2c, unsigned char slave_addr, unsigned char *wbuf, int wlen, unsigned char *rbuf, int rlen)
{
	int i;
	int ret;

	__start(i2c);

	if (wlen == 0 && rlen == 0) { //scan
		ret = __write(i2c, slave_addr << 1);
		goto exit;
	}

	if (wlen) {
		ret = __write(i2c, slave_addr);
		if (ret)
			goto exit;
		for (i = 0; i < wlen; i ++) {
			ret = __write(i2c, wbuf[i]);
			if (ret)
				goto exit;
		}
	}

	if (wlen && rlen)
		__start(i2c);

	if (rlen) {
		ret = __write(i2c, (slave_addr << 1) | 0x1);
		if (ret)
			goto exit;
		for (i = 0; i < rlen; i ++)
			rbuf[i] = __read(i2c, i == (rlen - 1));
	}

exit:
	__stop(i2c);
	return ret;
}

