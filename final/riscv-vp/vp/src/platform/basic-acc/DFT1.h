#ifndef DFT1_H_
#define DFT1_H_
#include <systemc>
#include <cmath>
#include <iomanip>
using namespace sc_core;

#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <def.h>

const int DFT1_R_ADDR = 0x00000000;
const int DFT1_RESULT_ADDR = 0x00000004;

const int DFT1_RS_R_ADDR   = 0x00000000;
const int DFT1_RS_W_WIDTH  = 0x00000004;
const int DFT1_RS_W_HEIGHT = 0x00000008;
const int DFT1_RS_W_DATA   = 0x0000000C;
const int DFT1_RS_RESULT_ADDR = 0x00800000;

struct DFT1 : public sc_module {
    tlm_utils::simple_target_socket<DFT1> tsock;

    sc_fifo<int> i_0;
    sc_fifo<int> i_1;
    sc_fifo<int> i_2;
    sc_fifo<int> i_3;

    sc_fifo<int> o_0;
    sc_fifo<int> o_1;
    sc_fifo<int> o_2;
    sc_fifo<int> o_3;

    SC_HAS_PROCESS(DFT1);

    DFT1(sc_module_name n): 
        sc_module(n), 
        tsock("t_skt"), 
        base_offset(0) 
    {
        tsock.register_b_transport(this, &DFT1::blocking_transport);
        SC_THREAD(do_DFT1);
    }

    ~DFT1() {}
    
    unsigned int base_offset;

    void do_DFT1(){
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
        
            int b4 = -val_i4 + val_i3;
            int b5 = -val_i5 + val_i2;
            int b6 = -val_i6 + val_i1;
            int b7 = -val_i7 + val_i0;

            wait(CLOCK_PERIOD, SC_NS);

            int c4 = -b4 - b5;
            int c5 =  b5 + b6;
            int c6 =  b6 + b7;
            int c7 =  b7;

            wait(CLOCK_PERIOD, SC_NS);
            
            float d4 =  c4;
            float d5 =  c5;
            float d6 =  c6;
            float d7 =  c7;
            float d8 = (c4 + c6) * a5;

            wait(CLOCK_PERIOD, SC_NS);

            float e4 = -d4 * a2 - d8;
            float e5 =  d5 * a3;
            float e6 =  d6 * a4 - d8;
            float e7 =  d7;

            wait(CLOCK_PERIOD, SC_NS);

            float f4 = e4;
            float f5 = e5 + e7;
            float f6 = e6;
            float f7 = e7 - e5;

            wait(CLOCK_PERIOD, SC_NS);

            float g4 =  f4 + f7;
            float g5 =  f5 + f6;
            float g6 = -f6 + f5;
            float g7 =  f7 - f4;

            wait(CLOCK_PERIOD, SC_NS);

            float val_o0 = g4 * s5;
            float val_o1 = g5 * s1;
            float val_o2 = g6 * s7;
            float val_o3 = g7 * s3;

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
                    case DFT1_RESULT_ADDR:
                        buffer.sint[0] = o_0.read();
                        buffer.sint[1] = o_1.read();
                        buffer.sint[2] = o_2.read();
                        buffer.sint[3] = o_3.read();
                        break;
                    default:
                        std::cerr << "READ Error! DFT1::blocking_transport: address 0x"
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
                    case DFT1_R_ADDR:
                        i_0.write((data_ptr[3] << 24) + (data_ptr[2] << 16) + (data_ptr[1] << 8) + data_ptr[0]);
                        i_1.write((data_ptr[7] << 24) + (data_ptr[6] << 16) + (data_ptr[5] << 8) + data_ptr[4]);
                        i_2.write((data_ptr[11] << 24) + (data_ptr[10] << 16) + (data_ptr[9] << 8) + data_ptr[8]);
                        i_3.write((data_ptr[15] << 24) + (data_ptr[14] << 16) + (data_ptr[13] << 8) + data_ptr[12]);
                        break;
                    default:
                        std::cerr << "WRITE Error! DFT1::blocking_transport: address 0x"
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
