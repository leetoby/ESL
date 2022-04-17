#ifndef QUAD_H_
#define QUAD_H_
#include <systemc>
using namespace sc_core;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class Quad: public sc_module
{
public:
	sc_in_clk i_clk;
	sc_in < bool >  i_rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<24> >::in i_para;
	cynw_p2p< sc_dt::sc_uint<26> >::out o_result;
#else
	sc_fifo_in< sc_dt::sc_uint<24> > i_para;
	sc_fifo_out< sc_dt::sc_uint<26> > o_result;
#endif

	SC_HAS_PROCESS( Quad );
	Quad( sc_module_name n );
	~Quad();
private:
	void do_quad();
};
#endif
