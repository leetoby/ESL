#include <cstdio>
#include <cstdlib>
using namespace std;

#include "Testbench.h"

Testbench::Testbench(sc_module_name n) : sc_module(n) {
  SC_THREAD(feed_para);
  sensitive << i_clk.pos();
  dont_initialize();
  SC_THREAD(fetch_result);
  sensitive << i_clk.pos();
  dont_initialize();
}

Testbench::~Testbench() {
	//cout<< "Max txn time = " << max_txn_time << endl;
	//cout<< "Min txn time = " << min_txn_time << endl;
	//cout<< "Avg txn time = " << total_txn_time/n_txn << endl;
	cout << "Total run time = " << total_run_time << endl;
}

void Testbench::feed_para() {
	n_txn = 0;
	max_txn_time = SC_ZERO_TIME;
	min_txn_time = SC_ZERO_TIME;
	total_txn_time = SC_ZERO_TIME;

	#ifndef NATIVE_SYSTEMC
		o_para.reset();
	#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);
	total_start_time = sc_time_stamp();
	for (int a = -128; a < -120; ++a) {
		for (int b = -128; b < 128; ++b) {
			for (int c = 0; c < 128; ++c) {
				sc_dt::sc_uint<24> para;
				sc_dt::sc_int<8> i_a;
				sc_dt::sc_int<8> i_b;
				sc_dt::sc_int<8> i_c;
				i_a = a;
				i_b = b;
				i_c = c;
				para.range(23, 16) = i_a;
				para.range(15, 8) = i_b;
				para.range(7, 0) = i_c;
				#ifndef NATIVE_SYSTEMC
					o_para.put(para);
				#else
					o_para.write(para);
				#endif
			}
		}
	}
}

void Testbench::fetch_result() {
	sc_dt::sc_uint<26> result;
	sc_dt::sc_int<13> solve1, solve2;
	float tmp1, tmp2;
	float a_f, b_f, c_f;
	float check_value;
	#ifndef NATIVE_SYSTEMC
		i_result.reset();
	#endif
	wait(5);
	wait(1);
	for (int a = -128; a < -120; ++a) {
		std::cout << "Current Parameter: a=" << a << std::endl;
		for (int b = -128; b < 128; ++b) {
			for (int c = 0; c < 128; ++c) {
				#ifndef NATIVE_SYSTEMC
					result = i_result.get();
				#else
					result = i_result.read();
				#endif
				solve1 = result.range(25, 13);
				solve2 = result.range(12, 0);

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
	total_run_time = sc_time_stamp() - total_start_time;
	sc_stop();
}
