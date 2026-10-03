#include <stdint.h>

#define WEAK __attribute__((weak))
#define ALIAS(f) __attribute__((weak, alias(#f)))

/* Linker symbols */
extern uint32_t _sidata;  /* Start of .data in FLASH (LMA) */
extern uint32_t _sdata;   /* Start of .data in RAM (VMA) */
extern uint32_t _edata;   /* End of .data in RAM */
extern uint32_t _sbss;    /* Start of .bss in RAM */
extern uint32_t _ebss;    /* End of .bss in RAM */
extern uint32_t _estack;  /* Top of Stack */

/* Application Entry */
extern int main(void);

/* Default Exception Handler */
void Default_Handler(void) {
    while (1) {
        /* Trap */
    }
}

/* System Exception Handlers */
void Reset_Handler(void);
void NMI_Handler(void) ALIAS(Default_Handler);
void HardFault_Handler(void) ALIAS(Default_Handler);
void MemManage_Handler(void) ALIAS(Default_Handler);
void BusFault_Handler(void) ALIAS(Default_Handler);
void UsageFault_Handler(void) ALIAS(Default_Handler);
void SVC_Handler(void) ALIAS(Default_Handler);
void DebugMon_Handler(void) ALIAS(Default_Handler);
void PendSV_Handler(void) ALIAS(Default_Handler);
void SysTick_Handler(void) ALIAS(Default_Handler);

/* Vector Table */
__attribute__((section(".isr_vector"), used))
uint32_t * const g_pfnVectors[] = {
    (uint32_t *)&_estack,         /* Initial Stack Pointer */
    (uint32_t *)Reset_Handler,    /* Reset Handler */
    (uint32_t *)NMI_Handler,      /* NMI Handler */
    (uint32_t *)HardFault_Handler,/* Hard Fault Handler */
    (uint32_t *)MemManage_Handler,/* MPU Fault Handler */
    (uint32_t *)BusFault_Handler, /* Bus Fault Handler */
    (uint32_t *)UsageFault_Handler,/* Usage Fault Handler */
    0, 0, 0, 0,                   /* Reserved */
    (uint32_t *)SVC_Handler,      /* SVCall Handler */
    (uint32_t *)DebugMon_Handler, /* Debug Monitor Handler */
    0,                            /* Reserved */
    (uint32_t *)PendSV_Handler,   /* PendSV Handler */
    (uint32_t *)SysTick_Handler,  /* SysTick Handler */
};

/* Reset Handler: The first code executed upon power-on */
void Reset_Handler(void) {
    /* 1. Copy initialized .data section from Flash to SRAM */
    uint32_t *pSrc = &_sidata;
    uint32_t *pDst = &_sdata;

    while (pDst < &_edata) {
        *pDst++ = *pSrc++;
    }

    /* 2. Zero-initialize the .bss section in SRAM */
    pDst = &_sbss;
    while (pDst < &_ebss) {
        *pDst++ = 0U;
    }

    /* 3. Call application main() */
    main();

    /* 4. If main ever returns, trap in infinite loop */
    while (1) {
    }
}
