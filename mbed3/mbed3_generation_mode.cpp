//EE 202 hm2
//This is a program built for the mbed3 in generation mode
//This code has been tested and should be function, if you has any problem,
//please mail me.
//Author: Yujing Qian
//y.qian42@gmail.com
#include "mbed.h"
#include "MKL46Z4.h"

#define buffer_size 9
DigitalOut LED(LED_RED);
InterruptIn mbed1(PTD2);
InterruptIn mbed2(PTD4);
AnalogIn analoginput(PTB0);
Serial pc(USBTX,USBRX);



int loop_num=0;
int per_num=0;
int Period=0;
int t1=0;
int t2=0;
int s;
int ms;
float us;
//Timer timer;
uint32_t TIME(){return per_num*Period*48000+loop_num*48000+(TPM0->CNT);}

void mbed1_event(){
    t1=TIME();   
    pc.printf("%d:%d\n", per_num*Period+loop_num,TPM0->CNT);
    }
void mbed2_event(){
    t2=TIME();
    pc.printf("%d:%d\n", per_num*Period+loop_num,TPM0->CNT);
    }
    
void TPM0_IRQHandler(void){
            loop_num++;
            if(loop_num==Period){
                LED=!LED;per_num++;loop_num=0;
                }
            TPM0->SC|= 0x000000c8;
            return;               
    }

void Initial(){
    NVIC_SetVector(TPM0_IRQn, (uint32_t)&TPM0_IRQHandler);
    NVIC_SetPriority(TPM0_IRQn, 0);
    loop_num=0;
    per_num=0;
    Period=1000;
    LED=1;
    SIM->SOPT2=0x05000000;
    SIM->SCGC6=0x01000000;//enable TPM 0
    
    TPM0->CNT=0x0;
    TPM0->SC=0x00000040;
    TPM0->MOD=0x0000bb80;
    NVIC_EnableIRQ(TPM0_IRQn);
    TPM0->SC=0x00000048;//0008
    }
    
int main() {
    Initial();
        //timer.start();
    pc.baud(9600);

     pc.printf("wait for event\n");
   mbed1.rise(mbed1_event);
   mbed2.rise(mbed2_event);

}