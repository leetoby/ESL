#include "string"
#include "string.h"
#include "cassert"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
// cd $EE6470/riscv-vp/vp/build
// make install

// cd $EE6470/riscv-vp/sw/basic-DCT
// make
// make sim

// DCT ACC
static char* const DFT_START_ADDR0 = reinterpret_cast<char* const>(0x80000000);
static char* const DFT_READ_ADDR0  = reinterpret_cast<char* const>(0x80000004);
static char* const DFT_START_ADDR1 = reinterpret_cast<char* const>(0x90000000);
static char* const DFT_READ_ADDR1  = reinterpret_cast<char* const>(0x90000004);

// DMA 
static volatile uint32_t * const DMA_SRC_ADDR  = (uint32_t * const)0x70000000;
static volatile uint32_t * const DMA_DST_ADDR  = (uint32_t * const)0x70000004;
static volatile uint32_t * const DMA_LEN_ADDR  = (uint32_t * const)0x70000008;
static volatile uint32_t * const DMA_OP_ADDR   = (uint32_t * const)0x7000000C;
static volatile uint32_t * const DMA_STAT_ADDR = (uint32_t * const)0x70000010;
static const uint32_t DMA_OP_MEMCPY = 1;

bool _is_using_dma = true;

union word {
  int sint[4];
  unsigned int uint[4];
  unsigned char uc[16];
};

int matrix_in[8][8] = {{166, 113, 228, 205,   7,  48, 166,  57},
					   {255, 170,  28, 203, 213, 122,  57, 218},
					   {246, 128,  19, 179,  46,  35, 136, 201},
					   {138,  75, 200,  92, 107, 103, 253,   3},
					   { 59,  42, 186,  72, 191, 121,  80, 173},
					   { 17,   4, 206, 140, 247, 109,  66, 130},
					   { 82, 118, 117,  72,  62,  17, 140, 132},
					   {131, 187,  68,  31, 131, 126, 215,  89}};

int matrix_buff[8][8];
int matrix_out[8][8];

bool hart_0_done_first = false;
bool hart_1_done_first = false;

bool hart_0_done_second = false;
bool hart_1_done_second = false;

void write_data_to_ACC(char* ADDR, unsigned char* buffer, int len) {
	if (_is_using_dma) {  
		// Using DMA 
		*DMA_SRC_ADDR = (uint32_t)(buffer);
		*DMA_DST_ADDR = (uint32_t)(ADDR);
		*DMA_LEN_ADDR = len;
		*DMA_OP_ADDR  = DMA_OP_MEMCPY;
	} else {
		// Directly Send
		memcpy(ADDR, buffer, sizeof(unsigned char)*len);
	}
}
void read_data_from_ACC(char* ADDR, unsigned char* buffer, int len){
	if (_is_using_dma) {
		// Using DMA 
		*DMA_SRC_ADDR = (uint32_t)(ADDR);
		*DMA_DST_ADDR = (uint32_t)(buffer);
		*DMA_LEN_ADDR = len;
		*DMA_OP_ADDR  = DMA_OP_MEMCPY;
	} else {
		// Directly Read
		memcpy(buffer, ADDR, sizeof(unsigned char)*len);
	}
}

int main(int hart_id, char *argv[]) {
	for (int n = 0; n < 256; n++) {
		int out;
		unsigned char buffer[8];
		word data;

		for (int i = 0; i < 100 * hart_id; i++);	// delay of different id
		
		if (hart_id == 0) {
			hart_0_done_first = false;
			hart_0_done_second = false;
		} else if (hart_id == 1) {
			hart_1_done_first = false;
			hart_1_done_second = false;
		}

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					buffer[4*j+k] = (matrix_in[i][j] >> (k*8)) % 256;
				}
			}
			if (hart_id == 0) {
				write_data_to_ACC(DFT_START_ADDR0, buffer, 16);
			} else if (hart_id == 1) {
				write_data_to_ACC(DFT_START_ADDR1, buffer, 16);
			}
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					buffer[4*j+k] = (matrix_in[i][j+4] >> (k*8)) % 256;
				}
			}
			if (hart_id == 0) {
				write_data_to_ACC(DFT_START_ADDR0, buffer, 16);
				read_data_from_ACC(DFT_READ_ADDR0, buffer, 16);
			} else if (hart_id == 1) {
				write_data_to_ACC(DFT_START_ADDR1, buffer, 16);
				read_data_from_ACC(DFT_READ_ADDR1, buffer, 16);
			}
			memcpy(data.uc, buffer, 16);
			if (hart_id == 0) {
				matrix_buff[i][0] = data.sint[0];
				matrix_buff[i][4] = data.sint[1];
				matrix_buff[i][2] = data.sint[2];
				matrix_buff[i][6] = data.sint[3];
			} else if (hart_id == 1) {
				matrix_buff[i][5] = data.sint[0];
				matrix_buff[i][1] = data.sint[1];
				matrix_buff[i][7] = data.sint[2];
				matrix_buff[i][3] = data.sint[3];
			}
		}

		if (hart_id == 0) {
			hart_0_done_first = true;
		} else if (hart_id == 1) {
			hart_1_done_first = true;
		}

		while (!hart_1_done_first || !hart_1_done_first);

		for (int i = 0; i < 100 * hart_id; i++);	// delay of different id
		
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					buffer[4*j+k] = (matrix_buff[j][i] >> (k*8)) % 256;
				}
			}
			if (hart_id == 0) {
				write_data_to_ACC(DFT_START_ADDR0, buffer, 16);
			} else if (hart_id == 1) {
				write_data_to_ACC(DFT_START_ADDR1, buffer, 16);
			}
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					buffer[4*j+k] = (matrix_buff[j+4][i] >> (k*8)) % 256;
				}
			}
			if (hart_id == 0) {
				write_data_to_ACC(DFT_START_ADDR0, buffer, 16);
				read_data_from_ACC(DFT_READ_ADDR0, buffer, 16);
			} else if (hart_id == 1) {
				write_data_to_ACC(DFT_START_ADDR1, buffer, 16);
				read_data_from_ACC(DFT_READ_ADDR1, buffer, 16);
			}
			memcpy(data.uc, buffer, 16);
			if (hart_id == 0) {
				matrix_out[0][i] = data.sint[0];
				matrix_out[4][i] = data.sint[1];
				matrix_out[2][i] = data.sint[2];
				matrix_out[6][i] = data.sint[3];
			} else if (hart_id == 1) {
				matrix_out[5][i] = data.sint[0];
				matrix_out[1][i] = data.sint[1];
				matrix_out[7][i] = data.sint[2];
				matrix_out[3][i] = data.sint[3];
			}
		}

		if (hart_id == 0) {
			hart_0_done_second = true;
		} else if (hart_id == 1) {
			hart_1_done_second = true;
		}

		while (!hart_0_done_second || !hart_1_done_second);
		
		/*
		if (hart_id == 0) {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					printf("%d ", matrix_out[i][j]);
				}
				printf("\n");
			}
		}*/
	}
	return 0;
}
/*
[[ 3.11120000e+04  2.55726003e+02  6.98025514e+02 -9.15227892e+02
   1.16531198e+03  1.54126977e+03 -6.50384795e+02  1.95749473e+03]
 [ 1.99094868e+03  1.74687836e+03  7.70118943e+02 -1.73013605e+02
   1.69347994e+03  1.69495834e+03  8.68709396e+01 -1.61904993e+03]
 [ 4.18253995e+02  1.18708027e+03  1.42156768e+03  1.27411782e+02
  -3.22714453e+02  2.11199210e+02 -1.28618499e+03 -1.32415818e+03]
 [-7.53549720e+02 -3.74601771e+02 -2.85549589e+03 -2.02692945e+03
  -9.34356328e+01 -3.59615246e+02  1.95272790e+03  1.10718985e+03]
 [-1.47078210e+02  2.29337569e+02 -4.87699607e+02  3.21397536e+02
  -3.06000000e+03  1.24261477e+03  1.50056966e+02  1.27459501e+03]
 [-1.82518292e+03  8.81102793e+02  5.36367814e+02 -1.16294916e+03
 [-2.46721696e+02 -1.19066149e+02 -4.22184992e+02 -3.90247478e+02
  -2.65213423e+01  7.95799256e+02  1.54432324e+02 -4.80872486e+01]
 [-1.10780570e+03 -4.63838399e+01  1.77336942e+03 -1.05637970e+03
   6.50631736e+02 -1.53306245e+02  4.84380210e+01 -9.01339255e+02]]
*/