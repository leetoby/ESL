#include <cmath>
#include <iomanip>

#include "Quad.h"

Quad::Quad(sc_module_name n) : sc_module(n), t_skt("t_skt"), base_offset(0) {
	SC_THREAD(do_quad);

	t_skt.register_b_transport(this, &Quad::blocking_transport);
}

void Quad::do_quad() {
	int a;
	int b;
	int c;

	int b_square;
	int a_c_4;
	int Delta;
	int Delta_sqrt;
	int num1, num2;
	int den;
	int solve1;
	int solve2;

	int d;

	while (true) {
		a = (uint32_t)i_a[0].read();
		a |= (uint32_t)i_a[1].read() << 8;
		a |= (uint32_t)i_a[2].read() << 16;
		a |= (uint32_t)i_a[3].read() << 24;

		b = (uint32_t)i_b[0].read();
		b |= (uint32_t)i_b[1].read() << 8;
		b |= (uint32_t)i_b[2].read() << 16;
		b |= (uint32_t)i_b[3].read() << 24;

		c = (uint32_t)i_c[0].read();
		c |= (uint32_t)i_c[1].read() << 8;
		c |= (uint32_t)i_c[2].read() << 16;
		c |= (uint32_t)i_c[3].read() << 24;

		b_square = b * b;
		a_c_4 = 4 * a * c;
		Delta = b_square - a_c_4;
		Delta = Delta << 2;
		
		Delta_sqrt = 0;
		d = 1 << 20;

		for (unsigned int i = 0; i < 11; i = i + 1) {
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

		o_result1.write(solve1);
		o_result2.write(solve2);
	}
}

void Quad::blocking_transport(tlm::tlm_generic_payload &payload,
                                     sc_core::sc_time &delay) {
	sc_dt::uint64 addr = payload.get_address();
	addr = addr - base_offset;
	unsigned char *mask_ptr = payload.get_byte_enable_ptr();
	unsigned char *data_ptr = payload.get_data_ptr();
	word buffer;
	switch (payload.get_command()) {
		case tlm::TLM_READ_COMMAND:
			switch (addr) {
				case QUAD_RESULT_ADDR:
					buffer.si[0] = o_result1.read();
					buffer.si[1] = o_result2.read();
					buffer.si[2] = 0;
					break;
				case QUAD_CHECK_ADDR:
					buffer.si[0] = o_result1.num_available();
					buffer.si[1] = o_result2.num_available();
					buffer.si[2] = 0;
					break;
				default:
					std::cerr << "Error! Quad::blocking_transport: address 0x"
							<< std::setfill('0') << std::setw(8) << std::hex << addr
							<< std::dec << " is not valid" << std::endl;
					break;
			}
			data_ptr[0] = buffer.uc[0];
			data_ptr[1] = buffer.uc[1];
			data_ptr[2] = buffer.uc[2];
			data_ptr[3] = buffer.uc[3];
			data_ptr[4] = buffer.uc[4];
			data_ptr[5] = buffer.uc[5];
			data_ptr[6] = buffer.uc[6];
			data_ptr[7] = buffer.uc[7];
			data_ptr[8] = buffer.uc[8];
			data_ptr[9] = buffer.uc[9];
			data_ptr[10] = buffer.uc[10];
			data_ptr[11] = buffer.uc[11];
			break;

		case tlm::TLM_WRITE_COMMAND:
			switch (addr) {
				case QUAD_R_ADDR:
					if (mask_ptr[0] == 0xff && mask_ptr[1] == 0xff && mask_ptr[2] == 0xff && mask_ptr[3] == 0xff) {
						i_a[0].write(data_ptr[0]);
						i_a[1].write(data_ptr[1]);
						i_a[2].write(data_ptr[2]);
						i_a[3].write(data_ptr[3]);
					}
					if (mask_ptr[4] == 0xff && mask_ptr[5] == 0xff && mask_ptr[6] == 0xff && mask_ptr[7] == 0xff) {
						i_b[0].write(data_ptr[4]);
						i_b[1].write(data_ptr[5]);
						i_b[2].write(data_ptr[6]);
						i_b[3].write(data_ptr[7]);
					}
					if (mask_ptr[8] == 0xff && mask_ptr[9] == 0xff && mask_ptr[10] == 0xff && mask_ptr[11] == 0xff) {
						i_c[0].write(data_ptr[8]);
						i_c[1].write(data_ptr[9]);
						i_c[2].write(data_ptr[10]);
						i_c[3].write(data_ptr[11]);
					}
					break;
				default:
					std::cerr << "Error! Quad::blocking_transport: address 0x"
								<< std::setfill('0') << std::setw(8) << std::hex << addr
								<< std::dec << " is not valid" << std::endl;
					break;
			}
			break;

		case tlm::TLM_IGNORE_COMMAND:
			payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
			return;
		default:
			payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
			return;
	}
	payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
}
