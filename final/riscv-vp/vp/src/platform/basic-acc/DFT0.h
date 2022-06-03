#ifndef DFT0_H_
#define DFT0_H_
#include <systemc>
#include <cmath>
#include <iomanip>
using namespace sc_core;

#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <def.h>

const int DFT0_R_ADDR = 0x00000000;
const int DFT0_RESULT_ADDR = 0x00000004;

const int DFT0_RS_R_ADDR   = 0x00000000;
const int DFT0_RS_W_WIDTH  = 0x00000004;
const int DFT0_RS_W_HEIGHT = 0x00000008;
const int DFT0_RS_W_DATA   = 0x0000000C;
const int DFT0_RS_RESULT_ADDR = 0x00800000;

struct DFT0 : public sc_module {
    tlm_utils::simple_target_socket<DFT0> tsock;

    sc_fifo<int> i_0;
    sc_fifo<int> i_1;
    sc_fifo<int> i_2;
    sc_fifo<int> i_3;

    sc_fifo<int> o_0;
    sc_fifo<int> o_1;
    sc_fifo<int> o_2;
    sc_fifo<int> o_3;

    SC_HAS_PROCESS(DFT0);

    DFT0(sc_module_name n): 
        sc_module(n), 
        tsock("t_skt"), 
        base_offset(0) 
    {
        tsock.register_b_transport(this, &DFT0::blocking_transport);
        SC_THREAD(do_DFT0);
    }

    ~DFT0() {}
    
    unsigned int base_offset;

    void do_DFT0(){
        { wait(CLOCK_PERIOD, SC_NS); }
        while (true) {
            int val_i0 = i_0.read();
            int val_i1 = i_1.read();
            int val_i2 = i_2.read();
            int val_i3 = i_3.read();
            int val_i4 = i_0.read();
            int val_i5 = i_1.read();
            int val_i6 = i_2.read();
            int val_i7 = i_3.read();

            wait(CLOCK_PERIOD, SC_NS);
        
            int b0 =  val_i0 + val_i7;
            int b1 =  val_i1 + val_i6;
            int b2 =  val_i2 + val_i5;
            int b3 =  val_i3 + val_i4;

            wait(CLOCK_PERIOD, SC_NS);

            int c0 =  b0 + b3;
            int c1 =  b1 + b2;
            int c2 = -b2 + b1;
            int c3 = -b3 + b0;

            wait(CLOCK_PERIOD, SC_NS);

            int d0 =  c0 + c1;
            int d1 = -c1 + c0;
            int d2 =  c2 + c3;
            int d3 =  c3;

            wait(CLOCK_PERIOD, SC_NS);

            float e0 =  d0;
            float e1 =  d1;
            float e2 =  d2 * a1;
            float e3 =  d3;

            wait(CLOCK_PERIOD, SC_NS);

            float f0 = e0;
            float f1 = e1;
            float f2 = e2 + e3;
            float f3 = e3 - e2;

            wait(CLOCK_PERIOD, SC_NS);
            
            float g0 =  f0;
            float g1 =  f1;
            float g2 =  f2;
            float g3 =  f3;

            wait(CLOCK_PERIOD, SC_NS);

            float val_o0 = g0 * s0;
            float val_o1 = g1 * s4;
            float val_o2 = g2 * s2;
            float val_o3 = g3 * s6;

            wait(CLOCK_PERIOD, SC_NS);

            o_0.write((int)round(val_o0));
            o_1.write((int)round(val_o1));
            o_2.write((int)round(val_o2));
            o_3.write((int)round(val_o3));
        }
    }

    void blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay){
        wait(delay);
        // unsigned char *mask_ptr = payload.get_byte_enable_ptr();
        // auto len = payload.get_data_length();
        tlm::tlm_command cmd = payload.get_command();
        sc_dt::uint64 addr = payload.get_address();
        unsigned char *data_ptr = payload.get_data_ptr();

        addr -= base_offset;

        // cout << (int)data_ptr[0] << endl;
        // cout << (int)data_ptr[1] << endl;
        // cout << (int)data_ptr[2] << endl;
        word buffer;

        switch (cmd) {
            case tlm::TLM_READ_COMMAND:
                // cout << "READ" << endl;
                switch (addr) {
                    case DFT0_RESULT_ADDR:
                        buffer.sint[0] = o_0.read();
                        buffer.sint[1] = o_1.read();
                        buffer.sint[2] = o_2.read();
                        buffer.sint[3] = o_3.read();
                        break;
                    default:
                        std::cerr << "READ Error! DFT0::blocking_transport: address 0x"
                                << std::setfill('0') << std::setw(8) << std::hex << addr
                                << std::dec << " is not valid" << std::endl;
                }
                for (int i = 0; i < 16; i++) {
                    data_ptr[i] = buffer.uc[i];
                }
                break;
            case tlm::TLM_WRITE_COMMAND:
                // cout << "WRITE" << endl;
                switch (addr) {
                    case DFT0_R_ADDR:
                        i_0.write((data_ptr[3] << 24) + (data_ptr[2] << 16) + (data_ptr[1] << 8) + data_ptr[0]);
                        i_1.write((data_ptr[7] << 24) + (data_ptr[6] << 16) + (data_ptr[5] << 8) + data_ptr[4]);
                        i_2.write((data_ptr[11] << 24) + (data_ptr[10] << 16) + (data_ptr[9] << 8) + data_ptr[8]);
                        i_3.write((data_ptr[15] << 24) + (data_ptr[14] << 16) + (data_ptr[13] << 8) + data_ptr[12]);
                        break;
                    default:
                        std::cerr << "WRITE Error! DFT0::blocking_transport: address 0x"
                                << std::setfill('0') << std::setw(8) << std::hex << addr
                                << std::dec << " is not valid" << std::endl;
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
};
#endif
