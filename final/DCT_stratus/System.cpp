#include "System.h"

System::System( sc_module_name n ):
	sc_module( n ), tb("tb"), dft("dft"), clk("clk", CLOCK_PERIOD, SC_NS), rst("rst")
{
	tb.i_clk(clk);
	tb.o_rst(rst);

	dft.i_clk(clk);
	dft.i_rst(rst);

	tb.i_F(o_F);
	tb.o_f(i_f);

	dft.i_f(i_f);
	dft.o_F(o_F);
}

System::~System() {}