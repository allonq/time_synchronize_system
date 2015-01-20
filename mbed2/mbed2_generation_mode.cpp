//EE 202 hm2
//This is a program built for the mbed2(slave) in generation mode
//This code has been tested and should be function, if you has any problem,
//please mail me.
//Author: Yujing Qian
//y.qian42@gmail.com
#include "mbed.h"
#include "MKL46Z4.h"

#define buffer_size 9
DigitalOut LED(LED_RED);
DigitalOut out(PTD4);
Serial pc(USBTX,USBRX);

Serial uart(PTE0, PTE1);


int loop_num=0;
int per_num=0;
int Period=0;
int state=0;
int buffered=0;
uint32_t T2=0;
uint32_t T3=0;
int delay=0;
int per=0;
char buffer[buffer_size];
int buff=0;
int syc_f=2;
int syc_interval=1000/syc_f;
uint32_t syc_cnt=0;

uint32_t TIME(){return per_num*Period*48000+loop_num*48000+(TPM0->CNT);}

void Syc(){// calculate delay using hand shake
            //pc.printf("Syc\n");
            uart.getc();
            uart.putc(0);       
            uart.getc();
            pc.printf("launch");
            LED=1;
            TPM0->CNT=0;
            TPM0->SC=0x00000048;//0008
    }
void synchronize(){
    //pc.printf("state=%d\n",state);

            buffer[0]=uart.getc();//pc.printf("0=%d\n",buffer[0]);
            uart.putc(0);
            buffer[1]=uart.getc();//pc.printf("1=%d\n",buffer[0]);
            uart.putc(0);
            buffer[2]=uart.getc();//pc.printf("2=%d\n",buffer[0]);
            uart.putc(0);
            buffer[3]=uart.getc();//pc.printf("3=%d\n",buffer[0]);
            uart.putc(0);
            per=((int)buffer[3]<<24)+((int)buffer[2]<<16)+((int)buffer[1]<<8)+((int)buffer[0]);

            Syc();

            loop_num=0;
            Period=per;

    }
    
void TPM0_IRQHandler(void){
            loop_num++;
            //syc_cnt++;

            if(loop_num==Period){
                out=!out;LED=!LED;per_num++;loop_num=0;
            }
            TPM0->SC|= 0x000000c8;
            
            return;               
    }


void cmd_handler(){
        char tmp;
        if(uart.readable()){
        pc.printf("read\n");
        tmp=uart.getc();
        pc.printf(&tmp);
        
        if(tmp=='#')uart.putc(0);synchronize();}
    
    }

    

void Initial(){
    for(int i=0;i<4;i++)buffer[i]=255;
    loop_num=0;
    per_num=0;
    Period=5000;
    out=0;
    LED=1;
    SIM->SOPT2=0x05000000;
    SIM->SCGC6=0x01000000;//enable TPM 0
    //TPM0->SC=0x0;
    
    TPM0->CNT=0x0;
    TPM0->SC=0x00000040;
    TPM0->MOD=0x0000bb80;
    TPM0->SC=0x00000048;//0008
    

    }
int main() {
    Initial();
        
    pc.baud(9600);
    uart.baud(9600);
    
    NVIC_SetVector(TPM0_IRQn, (uint32_t)&TPM0_IRQHandler);
    NVIC_SetPriority(TPM0_IRQn, 1);
    NVIC_EnableIRQ(TPM0_IRQn);
     
    uart.attach(& cmd_handler);
}