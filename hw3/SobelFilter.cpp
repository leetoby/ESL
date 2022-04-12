#include <cmath>
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "SobelFilter.h"

SobelFilter::SobelFilter( sc_module_name n ): sc_module( n )
{
	SC_THREAD( do_filter );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);
        
	#ifndef NATIVE_SYSTEMC
		i_rgb.clk_rst(i_clk, i_rst);
		o_r.clk_rst(i_clk, i_rst);
		o_g.clk_rst(i_clk, i_rst);
		o_b.clk_rst(i_clk, i_rst);
	#endif
}

SobelFilter::~SobelFilter() {}

// sobel mask
const int mask[MASK_X][MASK_Y] = {{1, 2, 1},
								  {2, 4, 2},
								  {1, 2, 1}};

void SobelFilter::do_filter() {
	{
		#ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("main_reset");
			i_rgb.reset();
			o_r.reset();
			o_g.reset();
			o_b.reset();
		#endif
		wait();
	}
	while (true) {
		int val_r_tmp = 0;
		int val_g_tmp = 0;
		int val_b_tmp = 0;
		int val_r = 0;
		int val_g = 0;
		int val_b = 0;
		sc_dt::sc_uint<24> rgb[3][3];
		for (unsigned int v = 0; v<MASK_Y; ++v) {
			for (unsigned int u = 0; u<MASK_X; ++u) {
				sc_dt::sc_uint<24> rgb_tmp;
				#ifndef NATIVE_SYSTEMC
					{
						HLS_DEFINE_PROTOCOL("input");
						rgb_tmp = i_rgb.get();
						wait();
					}
				#else
					rgb_tmp = i_rgb.read();
				#endif
				
				rgb[v][u] = rgb_tmp;
			}
		}
		for (unsigned int v = 0; v<MASK_Y; ++v) {
			for (unsigned int u = 0; u<MASK_X; ++u) {
				// HLS_PIPELINE_LOOP(SOFT_STALL, 1, "Loop");
				// HLS_UNROLL_LOOP(COMPLETE, 9, "my_loop");
				val_r_tmp = val_r_tmp + rgb[v][u].range(23, 16) * mask[v][u];
				val_g_tmp = val_g_tmp + rgb[v][u].range(15,8) * mask[v][u];
				val_b_tmp = val_b_tmp + rgb[v][u].range(7,0) * mask[v][u];
			}
		}
		val_r = val_r_tmp >> 4;
		val_g = val_g_tmp >> 4;
		val_b = val_b_tmp >> 4;
		#ifndef NATIVE_SYSTEMC
			{
				HLS_DEFINE_PROTOCOL("output");
				o_r.put(val_r);
				o_g.put(val_g);
				o_b.put(val_b);
				wait();
			}
		#else
			o_r.write(val_r);
			o_g.write(val_g);
			o_b.write(val_b);
		#endif
	}
}
