#include <cmath>
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "DFT0.h"

DFT0::DFT0( sc_module_name n ): sc_module( n )
{
	SC_THREAD( do_dft );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);
        
	#ifndef NATIVE_SYSTEMC
		i_f.clk_rst(i_clk, i_rst);
		o_F.clk_rst(i_clk, i_rst);
	#endif
}

DFT0::~DFT0() {}

void DFT0::do_dft() {
	sc_dt::sc_int<32> val_i0;
	sc_dt::sc_int<32> val_i1;
	sc_dt::sc_int<32> val_i2;
	sc_dt::sc_int<32> val_i3;
	sc_dt::sc_int<32> val_i4;
	sc_dt::sc_int<32> val_i5;
	sc_dt::sc_int<32> val_i6;
	sc_dt::sc_int<32> val_i7;

	int a1 = 46341;
	int s0 = 2;
	int s2 = 70936;
	int s4 = 92682;
	int s6 = 171254;
	{
		#ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("main_reset");
			i_f.reset();
			o_F.reset();
		#endif
		wait();
	}
	while (true) {
		// HLS_UNROLL_LOOP(ON, "sqrt loop");
		HLS_PIPELINE_LOOP(SOFT_STALL, 1, "main_loop");
		#ifndef NATIVE_SYSTEMC
			{
				HLS_DEFINE_PROTOCOL("input");
				f = i_f.get();
				wait();
			}
		#else
			f = i_f.read();
		#endif
		val_i0 = f.range(31, 0);
		val_i1 = f.range(63, 32);
		val_i2 = f.range(95, 64);
		val_i3 = f.range(127, 96);
		val_i4 = f.range(159, 128);
		val_i5 = f.range(191, 160);
		val_i6 = f.range(223, 192);
		val_i7 = f.range(255, 224);
	
		int b0 =  val_i0 + val_i7;
		int b1 =  val_i1 + val_i6;
		int b2 =  val_i2 + val_i5;
		int b3 =  val_i3 + val_i4;

		int c0 =  b0 + b3;
		int c1 =  b1 + b2;
		int c2 = -b2 + b1;
		int c3 = -b3 + b0;

		int d0 =  c0 + c1;
		int d1 = -c1 + c0;
		int d2 =  c2 + c3;
		int d3 =  c3;

		int e0 =  d0;
		int e1 =  d1;
		int e2 =  d2 * a1;
		int e3 =  d3;
		e2 = e2 >> 16;

		int f0 =  e0;
		int f1 =  e1;
		int f2 =  e2 + e3;
		int f3 =  e3 - e2;

		int g0 =  f0;
		int g1 =  f1;
		int g2 =  f2;
		int g3 =  f3;

		int val_o0 = g0 * s0;
		int val_o1 = g1 * s4;
		int val_o2 = g2 * s2;
		int val_o3 = g3 * s6;
		val_o1 = val_o1 >> 16;
		val_o2 = val_o2 >> 16;
		val_o3 = val_o3 >> 16;

		F.range(31, 0) = val_o0;
		F.range(63, 32) = val_o1;
		F.range(95, 64) = val_o2;
		F.range(127, 96) = val_o3;

		#ifndef NATIVE_SYSTEMC
			{
				HLS_DEFINE_PROTOCOL("output");
				o_F.put(F);
				wait();
			}
		#else
			o_F.write(F);
		#endif
	}
}
