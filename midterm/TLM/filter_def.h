#ifndef FILTER_DEF_H_
#define FILTER_DEF_H_

const int MASK_N = 2;
const int MASK_X = 3;
const int MASK_Y = 3;

const int QUAD_R_ADDR = 0x00000000;
const int QUAD_RESULT_ADDR = 0x00000004;
const int QUAD_CHECK_ADDR = 0x00000008;

union word {
	int si[3];
	unsigned char uc[12];
};

#endif
