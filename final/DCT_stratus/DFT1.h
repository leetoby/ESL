#ifndef DFT1_H_
#define DFT1_H_
#include <systemc>
using namespace sc_core;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class DFT1: public sc_module
{
public:
	sc_in_clk i_clk;
	sc_in < bool >  i_rst;
#ifndef NATIVE_SYSTEMC
    cynw_p2p< sc_dt::sc_bigint<256> >::in i_f;

    cynw_p2p< sc_dt::sc_bigint<128> >::out o_F;
#else
    sc_fifo< sc_dt::sc_bigint<256> > i_f;

    sc_fifo< sc_dt::sc_bigint<128> > o_F;
#endif

	SC_HAS_PROCESS( DFT1 );
	DFT1( sc_module_name n );
	~DFT1();
private:
	void do_dft();
	sc_dt::sc_bigint<256> f;
	sc_dt::sc_bigint<128> F;
};
#endif
