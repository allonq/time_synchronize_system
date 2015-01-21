#include "TPM_init.h"
 
void TPM0_init(unsigned int modulo_val, unsigned int prescale_val){
 
    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
    SIM->SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
        
    SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
    
    //Set all bits to zero
    TPM0->SC = 0x0;
        
    //Reset COUNT value to zero
    TPM0->CNT = 0x0;
    
    //Set modulo value
    TPM0->MOD = modulo_val;
    
    //Set prescale value
    TPM0->SC |= prescale_val;
    
    //Enable overflow interrupt
    TPM0->SC |= TPM_SC_TOIE_MASK;
}
 
unsigned int TPM0_SC_read() {
    unsigned int SC_value = TPM0->SC;
    return SC_value;
}
unsigned int TPM0_CNT_read() {
    unsigned int CNT_value = TPM0->CNT;
    return CNT_value;
}
 
void TPM0_clear_overflow() {
    TPM0->SC |= TPM_SC_TOF_MASK;
}
 
void TPM0_start() {   
    
    TPM0->SC |= TPM_SC_CMOD(1);
}