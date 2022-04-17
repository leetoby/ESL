#include <cmath>
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "Quad.h"

Quad::Quad( sc_module_name n ): sc_module( n )
{
	SC_THREAD( do_quad );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);
        
	#ifndef NATIVE_SYSTEMC
		i_para.clk_rst(i_clk, i_rst);
		o_result.clk_rst(i_clk, i_rst);
	#endif
}

Quad::~Quad() {}

void Quad::do_quad() {
	sc_dt::sc_int<8> a;
	sc_dt::sc_int<8> b;
	sc_dt::sc_int<8> c;

	sc_dt::sc_int<16> b_square;
	sc_dt::sc_int<19> a_c_4;
	sc_dt::sc_int<22> Delta;
	sc_dt::sc_int<22> Delta_sqrt;
	sc_dt::sc_int<24> num1, num2;
	sc_dt::sc_int<13> den;
	sc_dt::sc_int<13> solve1;
	sc_dt::sc_int<13> solve2;
	sc_dt::sc_uint<26> result;

	sc_dt::sc_int<22> d;
	{
		#ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("main_reset");
			i_para.reset();
			o_result.reset();
		#endif
		wait();
	}
	sc_dt::sc_uint<24> para;
	while (true) {
		#ifndef NATIVE_SYSTEMC
			{
				HLS_DEFINE_PROTOCOL("input");
				para = i_para.get();
				wait();
			}
		#else
			para = i_para.read();
		#endif

		a = para.range(23, 16);
		b = para.range(15, 8);
		c = para.range(7, 0);

		b_square = b * b;
		a_c_4 = 4 * a * c;
		Delta = b_square - a_c_4;
		Delta = Delta << 2;
		
		Delta_sqrt = 0;
		d = 1 << 20;

		for (unsigned int i = 0; i < 11; i = i + 1) {
			// HLS_UNROLL_LOOP(ON, "sqrt loop");
			HLS_PIPELINE_LOOP(SOFT_STALL, 1, "Loop" );
			if (d != 0) {
				if (Delta >= Delta_sqrt + d) {
					Delta = Delta - (Delta_sqrt + d);
					Delta_sqrt = (Delta_sqrt >> 1) + d;
				} else {
					Delta_sqrt = Delta_sqrt >> 1;
				}
				d = d >> 2;
			}
		}
		Delta_sqrt = (Delta_sqrt + 1) >> 1;


		num1 = -b + Delta_sqrt;
		num2 = -b - Delta_sqrt;

		num1 = num1 << 4;
		num2 = num2 << 4;

		den = a << 1;

		solve1 = num1 / den;
		solve2 = num2 / den;

		result.range(25, 13) = solve1;
		result.range(12, 0) = solve2;
		
		#ifndef NATIVE_SYSTEMC
			{
				HLS_DEFINE_PROTOCOL("output");
				o_result.put(result);
				wait();
			}
		#else
			o_result.write(result);
		#endif
	}
}
