#include "mbed.h"
#include <math.h>
#include "InterruptManager.h"
#include "TPM_init.h"
 
#define BAUD_RATE   921600
 
#define NVIC_ISER   0xE000E100
#define NVIC_ISPR   0xE000E200
 
#define SIM_SOPT2_ADDRESS 0x40048004
#define SIM_SCGC6_ADDRESS 0x4004803C
 
#define ARRAY_SIZE 50
#define SKIP_COUNT 50
 
//#define mbed1
 
DigitalOut out(PTD4);
DigitalOut green(LED1);
DigitalOut red(LED2);
InterruptIn in(PTD3);
Serial uart(PTE0, PTE1);
Serial pc(USBTX, USBRX);
 
Timer timer;
 
unsigned int tdf1 = 0;
unsigned int tdf2 = 0;
unsigned int tdt1 = 0;
unsigned int tdt2 = 0;
unsigned int send = 0;
unsigned int flag = 0;
unsigned int out_cnt = 0;
unsigned int cnt = 0;
unsigned int call_cnt = 0;
unsigned int timer_counter = 0;
unsigned int modulo_val = 48000;
unsigned int prescale_val = 0;
unsigned int flag_counter[ARRAY_SIZE];
unsigned int tick_counter[ARRAY_SIZE];
unsigned long origin_mbed1, origin_mbed2;
 
union {
    long v;
    char cv[8];
} u;
 
void init_target()
{
    NVIC_SetVector(TPM0_IRQn, (uint32_t) TPM0_handler_target);
    NVIC_SetPriority(TPM0_IRQn, 0);
 
    TPM0_init(modulo_val, prescale_val);
 
    NVIC_EnableIRQ(TPM0_IRQn);
    TPM0_start();
//    in.rise(func);
}
 
void TPM0_handler_target(void)
{
    unsigned int overflow = TPM0_SC_read() & 0x000000C0; 
    if(overflow == 0xC0) {
        TPM0_clear_overflow();
        timer_counter++;
        NVIC_ClearPendingIRQ(TPM0_IRQn);
    }
}
 
void digitalEventMonitor_mbed()
{
    if (call_cnt >= SKIP_COUNT) {
        call_cnt = 0;
        return;
    } else if (call_cnt >= COLLECTION_COUNT) {
        call_cnt++;
    } else {
        countArray[cnt] = event_counter;
        tickArray[cnt] = TPM0_CNT_read;
        send = 1;
        call_cnt++;
        
    }
}
 
#if !defined(mbed1)
void send_data_mbed2()
{
    long time = countArray[cnt] + tickArray[cnt]*0.0002 - origin_mbed2;
    char *ch = (char *)(long)time;
    sending = 1;
    time = time;
    uart.putc('d');
    for (i = 0; i < 8; i++) {
        uart.putc(ch[i]);
    }
    uart.putc('e');
    sending = 0;
}
 
void SignalEventHandler_mbed2()
{
    float tmpt;
    if (flag == 0) {
        flag = 1;
        tdf1 = timer_counter;
        tdt1 = TPM0_CNT_read();
        uart.putc('h');
    } else if (flag == 1) {
        tdf2 = timer_counter;
        tdt2 = TPM0_CNT_read();
        if (uart.getc() == 'h');
            tmpt = tdt2;
            td = (tdf2 - tdf1) + (tmpt - tdt1)*0.0002;
            origin_mbed2 = tdf1 + tdt1*0.0002;
        } else {
            //handshake failed
            ;
        }
        flag = 2;
        send = 1;
    } else {
        uart.getc();
    }
}
       
 
void init_mbed2()
{
    init_target();
    uart.attach(SignalEventHandler_mbed2);
    in.rise(digitalEventMonitor_mbed2);
    wait(5);
    SignalEventHandler_mbed2();
}
 
void manager_mbed2()
{
    init_mbed2(SignalEventHandler_mbed2);
    while(1) {
        while ((values != sentCount) && !sending){
            sendData_mbed2();
            wait(0.005);
        }
        wait(0.001);
    }
}
 
#else
void
SignalEventHandler_mbed1()
{
    if (flag == 0) {
        tdf1 = timer_counter;
        tdt1 = TPM0_CNT_read();
        uart.putc(10);
        c = uart.getc();
        if (c == 'h') {
            flag = 0;
            store_mbed2_origin();
        } else if (c == 'e') {
            flag = 0
            store_data();
        } else if (c == 'd') {
            flag == 1;
        } else {
            flag = 0;
        }
    } else {
        u.cv[c.cnt++] = uart.getc();
        if (c.cnt >= 8) {
            flag = 0;
            mbed2_time[cnt++] = mbed2_origin + u.v;
        }
    }
}
 
void init_mbed1(void (*func)(void))
{
    init_target(func);
}
 
/*
void handshake_bbb() {
    if (bbb_flag == 0) {
        // log  time
        bbb.putc();
        bbb_flag = 1;
    } else if (bbb_flag == 2) {
        //log time
        //log origin
        buart.getc();
        bbb_flag = 0;
    } else 
        bbb_flag = 0;
}
*/
 
void manager_mbed1()
{
    init_mbed2(SignalEventHandler_mbed1);
    wait(2);
//    handshake_bbb();
    while(1) {
        send_data(); //send the info to bbb
        
    }
}
#endif
   
int main() {
    timer_counter = 0;
    pc.baud(BAUD_RATE);
    uart.baud(BAUD_RATE);
   
#if defined(mbed1)
    buart.baud(BAUD_RATE);
    manager_mbed1();
#else
    manager_mbed2();
#endif
 
    while (true) {
        if (send) {
            #if defined(mbed1)
                pc.printf("\n%d:%d, %d:%d\n", tdf1, tdt1, tdf2, tdt2);
            #else
                pc.printf("\n%d:%d, %d:%d = %d\n", tdf1, tdt1, tdf2, tdt2, td);
            #endif
            send = 0;
        }
        wait(5);
    }
}