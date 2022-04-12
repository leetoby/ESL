#ifndef SOBEL_FILTER_H_
#define SOBEL_FILTER_H_
#include <systemc>
using namespace sc_core;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

#include "filter_def.h"

class SobelFilter: public sc_module
{
public:
	sc_in_clk i_clk;
	sc_in < bool >  i_rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<24> >::in i_rgb;
	cynw_p2p< sc_dt::sc_uint<8> >::out o_r;
	cynw_p2p< sc_dt::sc_uint<8> >::out o_g;
	cynw_p2p< sc_dt::sc_uint<8> >::out o_b;
#else
	sc_fifo_in< sc_dt::sc_uint<24> > i_rgb;
	sc_fifo_out< sc_dt::sc_uint<8> > o_r;
	sc_fifo_out< sc_dt::sc_uint<8> > o_g;
	sc_fifo_out< sc_dt::sc_uint<8> > o_b;
#endif

	SC_HAS_PROCESS( SobelFilter );
	SobelFilter( sc_module_name n );
	~SobelFilter();
private:
	void do_filter();
};
#endif
