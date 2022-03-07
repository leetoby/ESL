#include <cmath>

#include "Filter.h"

Filter::Filter(sc_module_name n) : sc_module(n) {
	SC_THREAD(do_filter);
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);
}

// mask
const int mask[MASK_X][MASK_Y] = {{1, 2, 1},
								  {2, 4, 2},
								  {1, 2, 1}};

void Filter::do_filter() {
	height = i_height.read();
	width = i_width.read();
	for (int i = 0; i < 4; i = i + 1) {
		for (int j = 0; j < 512; j = j + 1) {
			buff_r[i][j] = 0;
			buff_g[i][j] = 0;
			buff_b[i][j] = 0;
		}
	}
	while (true) {
		x = i_x.read();
		y = i_y.read();
		if (x == 0) {
			for (int i = 0; i < 512; i = i + 1) {
				buff_r[0][i] = buff_r[1][i];
				buff_g[0][i] = buff_g[1][i];
				buff_b[0][i] = buff_b[1][i];
			}
			for (int i = 0; i < 512; i = i + 1) {
				buff_r[1][i] = buff_r[2][i];
				buff_g[1][i] = buff_g[2][i];
				buff_b[1][i] = buff_b[2][i];
			}
			for (int i = 0; i < 512; i = i + 1) {
				buff_r[2][i] = buff_r[3][i];
				buff_g[2][i] = buff_g[3][i];
				buff_b[2][i] = buff_b[3][i];
			}
		}
		buff_r[3][x] = i_r.read();
		buff_g[3][x] = i_g.read();
		buff_b[3][x] = i_b.read();

		val_r = 0;
		val_g = 0;
		val_b = 0;
		if (y > 1) {
			if (x == 0) {
				val_r += mask[0][1] * buff_r[0][0];
				val_r += mask[0][2] * buff_r[0][1];
				val_r += mask[1][1] * buff_r[1][0];
				val_r += mask[1][2] * buff_r[1][1];
				val_r += mask[2][1] * buff_r[2][0];
				val_r += mask[2][2] * buff_r[2][1];
				val_g += mask[0][1] * buff_g[0][0];
				val_g += mask[0][2] * buff_g[0][1];
				val_g += mask[1][1] * buff_g[1][0];
				val_g += mask[1][2] * buff_g[1][1];
				val_g += mask[2][1] * buff_g[2][0];
				val_g += mask[2][2] * buff_g[2][1];
				val_b += mask[0][1] * buff_b[0][0];
				val_b += mask[0][2] * buff_b[0][1];
				val_b += mask[1][1] * buff_b[1][0];
				val_b += mask[1][2] * buff_b[1][1];
				val_b += mask[2][1] * buff_b[2][0];
				val_b += mask[2][2] * buff_b[2][1];
			} else if (x == width - 1) {
				val_r += mask[0][0] * buff_r[0][width - 2];
				val_r += mask[0][1] * buff_r[0][width - 1];
				val_r += mask[1][0] * buff_r[1][width - 2];
				val_r += mask[1][1] * buff_r[1][width - 1];
				val_r += mask[2][0] * buff_r[2][width - 2];
				val_r += mask[2][1] * buff_r[2][width - 1];
				val_g += mask[0][0] * buff_g[0][width - 2];
				val_g += mask[0][1] * buff_g[0][width - 1];
				val_g += mask[1][0] * buff_g[1][width - 2];
				val_g += mask[1][1] * buff_g[1][width - 1];
				val_g += mask[2][0] * buff_g[2][width - 2];
				val_g += mask[2][1] * buff_g[2][width - 1];
				val_b += mask[0][0] * buff_b[0][width - 2];
				val_b += mask[0][1] * buff_b[0][width - 1];
				val_b += mask[1][0] * buff_b[1][width - 2];
				val_b += mask[1][1] * buff_b[1][width - 1];
				val_b += mask[2][0] * buff_b[2][width - 2];
				val_b += mask[2][1] * buff_b[2][width - 1];
			} else {	
				for (int i = 0; i < 3; i = i + 1) {
					for (int j = 0; j < 3; j = j + 1) {
						val_r += mask[i][j] * buff_r[i][x-1+j];
						val_g += mask[i][j] * buff_g[i][x-1+j];
						val_b += mask[i][j] * buff_b[i][x-1+j];
					}
				}
			}
		}
		result_r = val_r >> 4;
		result_g = val_g >> 4;
		result_b = val_b >> 4;
		o_result_r.write(result_r);
		o_result_g.write(result_g);
		o_result_b.write(result_b);
		wait(10); //emulate module delay
	}
}