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
			cynw_p2p< sc_dt::sc_uint<24> >::base_out o_para;
			cynw_p2p< sc_dt::sc_uint<26> >::base_in i_result;
		#else
			sc_fifo_out< sc_dt::sc_uint<24> > o_para;
			sc_fifo_in< sc_dt::sc_uint<26> > i_result;
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

		void feed_para();
		void fetch_result();
};
#endif
