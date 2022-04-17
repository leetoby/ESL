#include <iostream>
#include <string>
using namespace std;

// Wall Clock Time Measurement
#include <sys/time.h>

#include "Quad.h"
#include "Testbench.h"

// TIMEVAL STRUCT IS Defined ctime
// use start_time and end_time variables to capture
// start of simulation and end of simulation
struct timeval start_time, end_time;

// int main(int argc, char *argv[])
int sc_main(int argc, char **argv) {
	Testbench tb("tb");
	Quad quad("quad");
	tb.initiator.i_skt(quad.t_skt);

	sc_start();
	std::cout << "Simulated time == " << sc_core::sc_time_stamp() << std::endl;
	return 0;
}
