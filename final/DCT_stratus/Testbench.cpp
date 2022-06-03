#include <cstdio>
#include <cstdlib>
#define DFT1
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
		o_f.reset();
	#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);
	total_start_time = sc_time_stamp();
	for (int n = 0; n < 256; n++) {
		for (int i = 0; i < 8; i++) {
			f.range(31, 0) = matrix_in[i][0];
			f.range(63, 32) = matrix_in[i][1];
			f.range(95, 64) = matrix_in[i][2];
			f.range(127, 96) = matrix_in[i][3];
			f.range(159, 128) = matrix_in[i][4];
			f.range(191, 160) = matrix_in[i][5];
			f.range(223, 192) = matrix_in[i][6];
			f.range(255, 224) = matrix_in[i][7];
			#ifndef NATIVE_SYSTEMC
				o_f.put(f);
			#else
				o_f.write(f);
			#endif
		}
	}
}

void Testbench::fetch_result() {
	sc_dt::sc_uint<26> result;

	#ifndef NATIVE_SYSTEMC
		i_F.reset();
	#endif
	wait(5);
	wait(1);

	for (int n = 0; n < 256; n++) {
		for (int i = 0; i < 8; i++) {
			#ifndef NATIVE_SYSTEMC
				F = i_F.get();
			#else
				F = i_F.read();
			#endif

			#ifdef DFT0
				matrix_buff[i][0] = F.range(31, 0);
				matrix_buff[i][4] = F.range(63, 32);
				matrix_buff[i][2] = F.range(95, 64);
				matrix_buff[i][6] = F.range(127, 96);
			#endif

			#ifdef DFT1
				matrix_buff[i][5] = F.range(31, 0);
				matrix_buff[i][1] = F.range(63, 32);
				matrix_buff[i][7] = F.range(95, 64);
				matrix_buff[i][3] = F.range(127, 96);
			#endif
		}
		/*
		for (int i = 0; i < 8; i++) {
			printf("%d\t", i);
			for (int j = 0; j < 8; j++) {
				cout << matrix_buff[i][j] << " ";
			}
			cout << endl;
		}*/
	}

	total_run_time = sc_time_stamp() - total_start_time;
	sc_stop();
}
