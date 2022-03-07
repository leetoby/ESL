#ifndef FILTER_H_
#define FILTER_H_
#include <systemc>
using namespace sc_core;

#include "filter_def.h"

class Filter : public sc_module {
	public:
		sc_in_clk i_clk;
		sc_in<bool> i_rst;
		sc_fifo_in<unsigned char> i_r;
		sc_fifo_in<unsigned char> i_g;
		sc_fifo_in<unsigned char> i_b;
		sc_fifo_out<int> o_result_r;
		sc_fifo_out<int> o_result_g;
		sc_fifo_out<int> o_result_b;

		SC_HAS_PROCESS(Filter);
		Filter(sc_module_name n);
		~Filter() = default;

	private:
		void do_filter();
		int val_r;
		int val_g;
		int val_b;
		int result_r;
		int result_g;
		int result_b;
};
#endif
