#include "common.h"

enum {
	ROBOT_LEFT_ARM = 0,
	ROBOT_LEFT_HIP,
	ROBOT_LEFT_LEG,

	ROBOT_RIGHT_ARM,
	ROBOT_RIGHT_HIP,
	ROBOT_RIGHT_LEG,
};

static int robot_pwms[] = {
	PWM_3_1_PA_6, 
	PWM_4_1_PB_6,
	PWM_4_2_PB_7,

	PWM_3_2_PA_7,
	PWM_4_3_PB_8,
	PWM_4_4_PB_9,
};

/* 0 ~ 180 */
/* plus: 20ms, 0: 0.5ms, 45: 1.0ms, 90: 1.5ms, 135: 2.0, 180: 2.5 */
void robot_pwm_pos(uint8_t index, uint8_t pos)
{
	pr_log("robot: pos=%d\n", pos);
	bsp_pwm_set(robot_pwms[index], 20*1000, (2 * 1000) * pos / 180);
}

int shell_robot_test(int argc, char **argv)
{
	int i;
	int pos; 

	for (i = 1; i < argc; i++) {
		pos = atoi(argv[i]);
		robot_pwm_pos((uint8_t)i, (uint8_t)pos);
	}

	return 0;
}
