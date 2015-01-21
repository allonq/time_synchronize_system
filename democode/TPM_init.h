#ifndef TPM_INIT_H
#define TPM_INIT_H
 
#include "mbed.h"
 
#define SIM_SOPT2_ADDR 0x40048004
#define SIM_SCGC6_ADDR 0x4004803C
#define MCG_C1_ADDR     0x40064000
#define TPM0_SC_ADDR    0x40038000
#define TPM0_C0SC_ADDR  0x4003800C
#define TPM0_CNT_ADDR  0x40038004
#define TPM0_MOD_ADDR  0x40038008
#define TPM0_CONF_ADDR  0x40038084
 
void print_stuff(char* whatever);
 
void TPM0_init(unsigned int modulo_val, unsigned int prescale_val);
 
void TPM0_clear_overflow();
unsigned int TPM0_CNT_read();
unsigned int TPM0_SC_read();
    
void TPM0_start();
#endif  