#ifndef DEF_H_
#define DEF_H_

#define CLOCK_PERIOD 10


#define MAX_IMAGE_BUFFER_LENTH 1024
#define THRESHOLD 90

const int DMA_TRANS = 64;

union word {
	int sint[4];
	unsigned int uint[4];
	unsigned char uc[16];
};

const float a1 = 0.7071067811865476;
const float a2 = 0.5411961001461971;
const float a3 = 0.7071067811865476;
const float a4 = 1.3065629648763766;
const float a5 = 0.38268343236508984;

const float s0 = 2.0;
const float s1 = 1.0195911582083184;
const float s2 = 1.082392200292394;
const float s3 = 1.2026897738700906;
const float s4 = 1.4142135623730951;
const float s5 = 1.7999524462728318;
const float s6 = 2.613125929752754;
const float s7 = 5.125830895483013;

#endif
