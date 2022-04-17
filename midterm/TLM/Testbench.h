#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <string>
using namespace std;

#include <systemc>
using namespace sc_core;

#include "Initiator.h"
#include "filter_def.h"

class Testbench : public sc_module {
	public:
		Initiator initiator;
		SC_HAS_PROCESS(Testbench);
		Testbench(sc_module_name n);
		~Testbench() = default;
	private:
		void do_quad();
};
#endif
