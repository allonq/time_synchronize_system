//EE 202 hm2
//This is a program built for the mbed3 in monitoring mode
//This code has been tested and should be function, if you has any problem,
//please mail me.
//Author: Yujing Qian
//y.qian42@gmail.com
#include "mbed.h"
#include "MKL46Z4.h"

DigitalOut LED(LED_RED);
DigitalOut mbed1(PTD2);
DigitalOut mbed2(PTD4);
Serial pc(USBTX,USBRX);

int loop_num=0;
int per_num=0;
int Period=0;
int t1=0;
int t2=0;

void TPM0_IRQHandler(void){
            loop_num++;
            if(loop_num==Period){
                mbed1=!mbed1;
                mbed2=!mbed2;
                LED=!LED;
                per_num++;
                loop_num=0;
                }
            TPM0->SC|= 0x000000c8;
            return;               
    }

void Initial(){
    NVIC_SetVector(TPM0_IRQn, (uint32_t)&TPM0_IRQHandler);
    NVIC_SetPriority(TPM0_IRQn, 0);
    loop_num=0;
    per_num=0;
    Period=500;
    LED=1;
    mbed1=0;
    mbed2=0;
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
    pc.baud(9600);

}