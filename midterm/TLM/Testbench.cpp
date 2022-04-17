#include <cassert>
#include <cstdio>
#include <cstdlib>
using namespace std;

#include "Testbench.h"

Testbench::Testbench(sc_module_name n) : sc_module(n), initiator("initiator") {
  	SC_THREAD(do_quad);
}

void Testbench::do_quad() {
	word data;
	unsigned char mask[12];
	//wait(5 * CLOCK_PERIOD, SC_NS);
	int solve1, solve2;
	float tmp1, tmp2;
	float a_f, b_f, c_f;
	float check_value;
	for (int a = -128; a < -120; ++a) {
		for (int b = -128; b < 128; ++b) {
			for (int c = 0; c < 128; ++c) {
				data.si[0] = a;
				data.si[1] = b;
				data.si[2] = c;
				mask[0] = 0xff;
				mask[1] = 0xff;
				mask[2] = 0xff;
				mask[3] = 0xff;
				mask[4] = 0xff;
				mask[5] = 0xff;
				mask[6] = 0xff;
				mask[7] = 0xff;
				mask[8] = 0xff;
				mask[9] = 0xff;
				mask[10] = 0xff;
				mask[11] = 0xff;

				initiator.write_to_socket(QUAD_R_ADDR, mask, data.uc, 12);
				wait(1 * CLOCK_PERIOD, SC_NS);
				
				bool done = false;
				int output_num = 0;
				while(!done) {
					initiator.read_from_socket(QUAD_CHECK_ADDR, mask, data.uc, 12);
					output_num = data.si[0];
					if(output_num>0) done=true;
				}
				
				wait(10 * CLOCK_PERIOD, SC_NS);
				initiator.read_from_socket(QUAD_RESULT_ADDR, mask, data.uc, 12);
				solve1 = data.si[0];
				solve2 = data.si[1];

				tmp1 = solve1 / 16.0;
				tmp2 = solve2 / 16.0;
				a_f = a / 16.0;
				b_f = b / 16.0;
				c_f = c / 16.0;
				check_value = (a_f * tmp1 * tmp1) + (b_f * tmp1) + c_f;
				if (check_value > 2 || check_value < -2) {
					std::cout << "error: check_value=" << check_value << " When a=" << a_f << ", b=" << b_f << ", c=" << c_f << ", solve1=" << solve1 << endl;
				}
				check_value = (a_f * tmp2 * tmp2) + (b_f * tmp2) + c_f;
				if (check_value > 2 || check_value < -2) {
					std::cout << "error: check_value=" << check_value << " When a=" << a_f << ", b=" << b_f << ", c=" << c_f << ", solve2=" << solve2 << endl;
				}
			}
		}
	}
	sc_stop();
}
