#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <string>
using namespace std;

#include <systemc>
using namespace sc_core;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class Testbench : public sc_module {
	public:
		sc_in_clk i_clk;
		sc_out < bool >  o_rst;
		#ifndef NATIVE_SYSTEMC
			cynw_p2p< sc_dt::sc_bigint<128> >::base_in i_F;

			cynw_p2p< sc_dt::sc_bigint<256> >::base_out o_f;
		#else
			sc_fifo_in< sc_dt::sc_bigint<128> > i_F;

			sc_fifo_out< sc_dt::sc_bigint<256> > o_f;
		#endif

		SC_HAS_PROCESS(Testbench);

		Testbench(sc_module_name n);
		~Testbench();

	private:
		unsigned int n_txn;
		sc_time max_txn_time;
		sc_time min_txn_time;
		sc_time total_txn_time;
		sc_time total_start_time;
		sc_time total_run_time;

		sc_dt::sc_bigint<256> f;
		sc_dt::sc_bigint<128> F;
		void feed_para();
		void fetch_result();
		
		int const matrix_in[8][8] = {{166, 113, 228, 205,   7,  48, 166,  57},
									 {255, 170,  28, 203, 213, 122,  57, 218},
									 {246, 128,  19, 179,  46,  35, 136, 201},
									 {138,  75, 200,  92, 107, 103, 253,   3},
									 { 59,  42, 186,  72, 191, 121,  80, 173},
									 { 17,   4, 206, 140, 247, 109,  66, 130},
									 { 82, 118, 117,  72,  62,  17, 140, 132},
									 {131, 187,  68,  31, 131, 126, 215,  89}};

		sc_dt::sc_int<32> matrix_buff[8][8] = {{0, 0, 0, 0, 0, 0, 0, 0},
											   {0, 0, 0, 0, 0, 0, 0, 0},
											   {0, 0, 0, 0, 0, 0, 0, 0},
											   {0, 0, 0, 0, 0, 0, 0, 0},
											   {0, 0, 0, 0, 0, 0, 0, 0},
											   {0, 0, 0, 0, 0, 0, 0, 0},
											   {0, 0, 0, 0, 0, 0, 0, 0},
											   {0, 0, 0, 0, 0, 0, 0, 0}};
};
#endif
