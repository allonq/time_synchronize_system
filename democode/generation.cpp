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
#define SKIP_COUNT 500
 
#define mbed1
 
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
float td;
 
void SignalEventHandler()
{
    if (flag == 0) {
        out = !out;
        flag = 1;
    }
        
    if (call_cnt >= SKIP_COUNT) {
        call_cnt = 0;
        red = !red;
        pc.printf("%c\n", 'a');
        return;
    }
    if (call_cnt > 1) {
        call_cnt++;
        return;
    }
//    green = !green;
//    flag_counter[cnt] = flag;
//    tick_counter[cnt] = TPM0_CNT_read();
    cnt++;
    call_cnt++;
    if (cnt > ARRAY_SIZE)
        cnt = 0;
}
 
void generateSignal()
{
    unsigned int overflow = TPM0_SC_read() & 0x000000C0; 
    if(overflow == 0xC0) {
        out = !out;
        TPM0_clear_overflow();
        NVIC_ClearPendingIRQ(TPM0_IRQn);
    }
}
 
void
SignalEventHandler_mbed1()
{
    int i = 0;
    float tmpt;
    if (flag == 0) {
        uart.getc();
        tdf1 = timer_counter;
        tdt1 = TPM0_CNT_read();
        uart.putc(10);
        flag = 1;
    } else if (flag == 1) {
        uart.getc();
        tdf2 = timer_counter;
        tdt2 = TPM0_CNT_read();
        // Disable interrupt
        NVIC_DisableIRQ(TPM0_IRQn);
        tmpt = tdt2;
        td = ((tdf2 - tdf1) * 1000 + (tmpt - tdt1)*0.02)/2;
        // wait for td
        i = 1000 + (td *16.67) -  0;;
        // wait here in the loop and
        i = 2000;
        while(i--);
        // then start sending the signals
        NVIC_SetVector(TPM0_IRQn, (uint32_t) generateSignal);
        NVIC_SetPriority(TPM0_IRQn, 0);
 
        TPM0_init(modulo_val, prescale_val);
 
        NVIC_EnableIRQ(TPM0_IRQn);
        TPM0_start();
    }
}
 
void SignalEventHandler_mbed2()
{
    int i;
    float tmpt;
    if (flag == 0) {
        tdf1 = timer_counter;
        tdt1 = TPM0_CNT_read();
        uart.putc(10);
        flag = 1;
    } else if (flag == 1) {
        uart.getc();
        tdf2 = timer_counter;
        tdt2 = TPM0_CNT_read();
        uart.putc(10);
        tmpt = tdt2;
        // Stop the timer
        NVIC_DisableIRQ(TPM0_IRQn);
        td = ((tdf2 - tdf1) * 1000 + (tmpt - tdt1)*0.02)/2;
        // wait for td
        i = 1000;
        i = 1000 + (td *16.66666667);
        while(i--);
        NVIC_SetVector(TPM0_IRQn, (uint32_t) generateSignal);
        NVIC_SetPriority(TPM0_IRQn, 0);
 
        TPM0_init(modulo_val, prescale_val);
 
        NVIC_EnableIRQ(TPM0_IRQn);
        TPM0_start();
        send = 1;
        // start sending the signals
    }
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
 
void init_target(void (*func)(void))
{
    NVIC_SetVector(TPM0_IRQn, (uint32_t) TPM0_handler_target);
    NVIC_SetPriority(TPM0_IRQn, 0);
 
    TPM0_init(modulo_val, prescale_val);
 
    NVIC_EnableIRQ(TPM0_IRQn);
    TPM0_start();
//    in.rise(func);
    uart.attach(func);
}
 
void init_mbed1(void (*func)(void))
{
    init_target(func);
}
 
void init_mbed2(void (*func)(void))
{
    init_target(func);
    wait(5);
    func();
}
 
/*
void SignalEventHandler()
{
    if (flag == 500) {
        flag = 0;
        green = !green;
    }
    flag++;
}
*/
 
int main() {
    timer_counter = 0;
    pc.baud(BAUD_RATE);
    uart.baud(BAUD_RATE);
    out = 0;
#if defined(mbed1)
    init_mbed1(SignalEventHandler_mbed1);
#else
    init_mbed2(SignalEventHandler_mbed2);
#endif
 
    while (true) {
        if (send)
            pc.printf("%d:%d,%d:%d,td=%f\n", tdf1, tdt1, tdf2, tdt2, td);
        wait(5);
    }
}