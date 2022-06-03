#ifndef SYSTEM_H_
#define SYSTEM_H_
#include <systemc>
using namespace sc_core;

#include "Testbench.h"
#ifndef NATIVE_SYSTEMC
#include "DFT1_wrap.h"
#else
#include "DFT1.h"
#endif

class System: public sc_module
{
	public:
		SC_HAS_PROCESS( System );
		System( sc_module_name n);
		~System();
	private:
		Testbench tb;
		#ifndef NATIVE_SYSTEMC
			DFT1_wrapper dft;
		#else
			DFT1 dft;
		#endif
			sc_clock clk;
			sc_signal<bool> rst;
		#ifndef NATIVE_SYSTEMC
			cynw_p2p< sc_dt::sc_bigint<256> > i_f;

			cynw_p2p< sc_dt::sc_bigint<128> > o_F;
		#else
			sc_fifo< sc_dt::sc_bigint<256> > i_f;

			sc_fifo< sc_dt::sc_bigint<128> > o_F;
		#endif
};
#endif
