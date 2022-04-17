#ifndef QUAD_H_
#define QUAD_H_
#include <systemc>
using namespace sc_core;

#include "tlm"
#include "tlm_utils/simple_target_socket.h"

#include "filter_def.h"

class Quad : public sc_module {
	public:
		tlm_utils::simple_target_socket<Quad> t_skt;

		sc_fifo<char> i_a[4];
		sc_fifo<char> i_b[4];
		sc_fifo<char> i_c[4];
		sc_fifo<int> o_result1;
		sc_fifo<int> o_result2;

		SC_HAS_PROCESS(Quad);
		Quad(sc_module_name n);
		~Quad() = default;

	private:
		void do_quad();
		unsigned int base_offset;
		void blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay);
};
#endif
