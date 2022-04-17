#ifndef SYSTEM_H_
#define SYSTEM_H_
#include <systemc>
using namespace sc_core;

#include "Testbench.h"
#ifndef NATIVE_SYSTEMC
#include "Quad_wrap.h"
#else
#include "Quad.h"
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
			Quad_wrapper quad;
		#else
			Quad quad;
		#endif
			sc_clock clk;
			sc_signal<bool> rst;
		#ifndef NATIVE_SYSTEMC
			cynw_p2p< sc_dt::sc_uint<24> > para;
			cynw_p2p< sc_dt::sc_uint<26> > result;
		#else
			sc_fifo< sc_dt::sc_uint<24> > para;
			sc_fifo< sc_dt::sc_uint<26> > result;
		#endif
};
#endif
