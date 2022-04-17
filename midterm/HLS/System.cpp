#include "System.h"

System::System( sc_module_name n ):
	sc_module( n ), tb("tb"), quad("quar"), clk("clk", CLOCK_PERIOD, SC_NS), rst("rst")
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	quad.i_clk(clk);
	quad.i_rst(rst);
	tb.o_para(para);
	tb.i_result(result);
	quad.i_para(para);
	quad.o_result(result);
}

System::~System() {}