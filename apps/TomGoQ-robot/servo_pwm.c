#include "common.h"

enum {
	ROBOT_LEFT_HIP = 0,
	ROBOT_LEFT_LEG,

	ROBOT_RIGHT_HIP,
	ROBOT_RIGHT_LEG,

	ROBOT_LEFT_ARM,
	ROBOT_RIGHT_ARM,
};

static int robot_pwms[] = {
	PWM_4_1_PB_6,
	PWM_4_2_PB_7,

	PWM_4_3_PB_8,
	PWM_4_4_PB_9,

	PWM_3_1_PA_6, 
	PWM_3_2_PA_7,
};

/* 0 ~ 180 */
/* plus: 20ms, 0: 0.5ms, 45: 1.0ms, 90: 1.5ms, 135: 2.0, 180: 2.5 */
void robot_pwm_pos(uint8_t index, uint8_t pos)
{
	if (index >= ARRAY_SIZE(robot_pwms)) {
		pr_err("robot: invalid index %d\n", index);
		return;
	}
	pr_log("robot: %d, pos=%d, pwm_pulse: %d\n", index, pos, (2 * 1000) * pos / 180);
	bsp_pwm_set(robot_pwms[index], 20*1000, (2 * 1000) * pos / 180 + 500);
}

int shell_robot_test(int argc, char **argv)
{
	int i;
	int pos; 

	for (i = 1; i < argc; i++) {
		pos = atoi(argv[i]);
		robot_pwm_pos((uint8_t)i - 1, (uint8_t)pos);
	}

	return 0;
}
