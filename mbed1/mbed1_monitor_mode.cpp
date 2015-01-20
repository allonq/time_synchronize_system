//EE 202 hm2
//This is a program built for the mbed1 in monitor mode
//This code has been tested and should be function, if you has any problem,
//please mail me.
//Author: Yujing Qian
//y.qian42@gmail.com
#include "mbed.h"
//#include "MKL46Z4.h"

#define mod 0x0000bb80 
#define duty_cycle 0.5
#define high_cycle mod*duty_cycle
#define low_cycle mod-high_cycle
#define buffer_size 9
#define BBBbuff_size 30

DigitalOut LED(LED_RED);
InterruptIn dig_in(PTD4);
Serial pc(USBTX,USBRX);
Serial uart(PTE0, PTE1);
Serial uart_bbb(PTE16,PTE17);

uint32_t loop_num=0;
uint32_t per_num=0;
uint32_t Period=5000;
uint32_t T1=0;
uint32_t T2=0;
uint32_t T3=0;
uint32_t T4=0;
bool flag=false;

uint32_t Mod=mod;
char BBBbuffer[BBBbuff_size];
bool cmd_received=false;
int BBBbuff=0;
char buffer[4];
int buff=0;
int drift_cnt=0;
uint32_t TIME(){return per_num*Period*48000+loop_num*48000+(TPM0->CNT);}

void report(){
    int us=(TPM0->CNT);
    uint32_t ms=per_num*Period+loop_num;
    uint32_t s=ms/1000;//second
     ms=ms%1000;

    uart_bbb.printf("m%u:%d\n",s*1000+ms,us);
    pc.printf("m%u:%u:%d\n",s,ms,us);
    drift_cnt++;
    }
void receive_handler(){
    while(pc.readable() && BBBbuff<BBBbuff_size){
        BBBbuffer[BBBbuff] =pc.getc();
        pc.putc(BBBbuffer[BBBbuff]);
        //pc.printf(& rx_buffer[buff]);
        if (BBBbuffer[BBBbuff] == 's'){
            BBBbuffer[BBBbuff] = '\0';
            cmd_received=true;
            BBBbuff++;
            pc.printf("cmd received\n");
            break;            
            }
        BBBbuff++;    
        }
        return;
    }
void TPM1_IRQHandler(void);
uint32_t calculate_delay(){// calculate delay using hand shake
            
            
            T1=TIME();
            uart.putc('a');
            uart.getc();
            T4=TIME();
                        
            return (T4-T1)/2;           
    }
void Syc(uint32_t delay){

            TPM1->MOD=delay;
            TPM1->CNT=0x0;
            //loop_num=0;
            TPM1->SC=0x00000048;          
            NVIC_EnableIRQ(TPM1_IRQn);
            // open tpm1           
            uart.putc(0);//send signal to slave
    }
void synchronize(uint32_t Cmd_per){
       
        char* tmp=(char*)&Cmd_per;

        uart.getc();
        //pc.printf("TFS love xuemei");
        uart.putc((*tmp));//pc.printf("0=%d\n",*tmp);
        uart.getc();
        uart.putc((*(tmp+1)));//pc.printf("1=%d\n",(*(tmp+1)));
        uart.getc();   
        uart.putc((*(tmp+2)));//pc.printf("2=%d\n",(*(tmp+2)));
        uart.getc();  
        uart.putc((*(tmp+3)));//pc.printf("3=%d\n",(*(tmp+3)));
        uart.getc();//clear the buffer
            
            Period=Cmd_per;

            Syc(calculate_delay());
           
            loop_num=0;//clear clock 
            per_num=0;   
            flag=true;     
    }

void TPM0_IRQHandler(void){

            loop_num++;

            if(loop_num==Period){

                LED=!LED;
                per_num++;
                loop_num=0;
                }
            TPM0->SC|= 0x000000c8;
            
            
            return;               
    }

void TPM1_IRQHandler(void){// time to restart the system           
            LED=1;
              
            TPM0->CNT=0x0;          
            TPM1->CNT=0x0;
            TPM1->SC= 0x00000080;  
            NVIC_DisableIRQ(TPM1_IRQn);
            flag=true;
            return;               
    }
    

    
void Initial(){
    Period=5000;
    LED=1;
    //out=0;
    SIM->SOPT2=0x05000000;
    SIM->SCGC6=0x03000000;//enable TPM 0,1
    TPM0->SC=0x0;

    TPM0->CNT=0x0;
    TPM0->SC=0x00000040;
    TPM0->MOD=0x0000bb80;
    TPM0->SC=0x00000048;//0008
    
    TPM1->CNT=0x0;
    TPM1->SC=0x00000040;
    TPM1->MOD=0x0000ffff;
    TPM1->SC=0x00000008;//0008
    }
int main() {
    loop_num=0;
    cmd_received=false;
    pc.attach(&receive_handler);    
    int CMD_Per=0;
    Initial();
    
    pc.baud(9600);
    pc.attach(&receive_handler);
    
    uart.baud(9600);
      
    NVIC_SetVector(TPM0_IRQn, (uint32_t)&TPM0_IRQHandler);
    NVIC_SetPriority(TPM0_IRQn, 1);
    NVIC_EnableIRQ(TPM0_IRQn); 

    NVIC_SetVector(TPM1_IRQn, (uint32_t)&TPM1_IRQHandler);
    NVIC_SetPriority(TPM1_IRQn, 0);
    NVIC_EnableIRQ(TPM1_IRQn);

    dig_in.rise(report);

    while(1){
        if(cmd_received){
            cmd_received=false;
            BBBbuff=0;

            uart.putc('#');     
            synchronize(2000);

            }       
        }
    
}