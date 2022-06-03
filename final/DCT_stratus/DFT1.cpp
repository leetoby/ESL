#include <cmath>
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "DFT1.h"

DFT1::DFT1( sc_module_name n ): sc_module( n )
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

DFT1::~DFT1() {}

void DFT1::do_dft() {
	sc_dt::sc_int<32> val_i0;
	sc_dt::sc_int<32> val_i1;
	sc_dt::sc_int<32> val_i2;
	sc_dt::sc_int<32> val_i3;
	sc_dt::sc_int<32> val_i4;
	sc_dt::sc_int<32> val_i5;
	sc_dt::sc_int<32> val_i6;
	sc_dt::sc_int<32> val_i7;

	int a2 = 35468;
	int a3 = 46341;
	int a4 = 85627;
	int a5 = 25080;

	int s1 = 66820;
	int s3 = 78819;
	int s5 = 117962;
	int s7 = 335926;
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
		HLS_PIPELINE_LOOP(SOFT_STALL, 3, "main_loop");
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
	
		int b4 = -val_i4 + val_i3;
		int b5 = -val_i5 + val_i2;
		int b6 = -val_i6 + val_i1;
		int b7 = -val_i7 + val_i0;

		int c4 = -b4 - b5;
		int c5 =  b5 + b6;
		int c6 =  b6 + b7;
		int c7 =  b7;

        int d4 =  c4;
        int d5 =  c5;
        int d6 =  c6;
        int d7 =  c7;
        int d8 = (c4 + c6) * a5;
		d8 = d8 >> 16;

		int e4 = -d4 * a2;
		int e5 =  d5 * a3;
		int e6 =  d6 * a4;
		int e7 =  d7;
		e4 = e4 >> 16;
		e5 = e5 >> 16;
		e6 = e6 >> 16;
		e4 = e4 - d8;
		e6 = e6 - d8;

		int f4 = e4;
		int f5 = e5 + e7;
		int f6 = e6;
		int f7 = e7 - e5;

		int g4 =  f4 + f7;
		int g5 =  f5 + f6;
		int g6 = -f6 + f5;
		int g7 =  f7 - f4;

		int val_o0 = g4 * s5;
		int val_o1 = g5 * s1;
		int val_o2 = g6 * s7;
		int val_o3 = g7 * s3;
		val_o0 = val_o0 >> 16;
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
