#ifndef STM32F401XE_H
#define STM32F401XE_H

#include <stdint.h>

/* Base addresses of Memory Regions & Peripherals */
#define FLASH_BASE            (0x08000000UL)
#define SRAM_BASE             (0x20000000UL)
#define PERIPH_BASE           (0x40000000UL)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)

/* GPIO Base Addresses */
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800UL)

/* RCC Base Address */
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800UL)

/* Core Private Peripheral Bus (Cortex-M4 Internal Peripherals) */
#define SCS_BASE              (0xE000E000UL)
#define SYSTICK_BASE          (SCS_BASE + 0x0010UL)

/* GPIO Register Map Structure */
typedef struct {
    volatile uint32_t MODER;    /* Mode register, offset: 0x00 */
    volatile uint32_t OTYPER;   /* Output type register, offset: 0x04 */
    volatile uint32_t OSPEEDR;  /* Output speed register, offset: 0x08 */
    volatile uint32_t PUPDR;    /* Pull-up/pull-down register, offset: 0x0C */
    volatile uint32_t IDR;      /* Input data register, offset: 0x10 */
    volatile uint32_t ODR;      /* Output data register, offset: 0x14 */
    volatile uint32_t BSRR;     /* Bit set/reset register, offset: 0x18 */
    volatile uint32_t LCKR;     /* Configuration lock register, offset: 0x1C */
    volatile uint32_t AFR[2];   /* Alternate function registers, offset: 0x20-0x24 */
} GPIO_TypeDef;

/* RCC Register Map Structure */
typedef struct {
    volatile uint32_t CR;            /* Clock control register, offset: 0x00 */
    volatile uint32_t PLLCFGR;       /* PLL configuration register, offset: 0x04 */
    volatile uint32_t CFGR;          /* Clock configuration register, offset: 0x08 */
    volatile uint32_t CIR;           /* Clock interrupt register, offset: 0x0C */
    volatile uint32_t AHB1RSTR;      /* AHB1 peripheral reset register, offset: 0x10 */
    volatile uint32_t AHB2RSTR;      /* AHB2 peripheral reset register, offset: 0x14 */
    uint32_t RESERVED0[2];
    volatile uint32_t APB1RSTR;      /* APB1 peripheral reset register, offset: 0x20 */
    volatile uint32_t APB2RSTR;      /* APB2 peripheral reset register, offset: 0x24 */
    uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;       /* AHB1 peripheral clock enable register, offset: 0x30 */
    volatile uint32_t AHB2ENR;       /* AHB2 peripheral clock enable register, offset: 0x34 */
    uint32_t RESERVED2[2];
    volatile uint32_t APB1ENR;       /* APB1 peripheral clock enable register, offset: 0x40 */
    volatile uint32_t APB2ENR;       /* APB2 peripheral clock enable register, offset: 0x44 */
} RCC_TypeDef;

/* SysTick Register Map Structure */
typedef struct {
    volatile uint32_t CTRL;          /* SysTick Control and Status Register */
    volatile uint32_t LOAD;          /* SysTick Reload Value Register */
    volatile uint32_t VAL;           /* SysTick Current Value Register */
    volatile uint32_t CALIB;         /* SysTick Calibration Register */
} SysTick_TypeDef;

/* Peripheral Declarations */
#define GPIOA                 ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOC                 ((GPIO_TypeDef *) GPIOC_BASE)
#define RCC                   ((RCC_TypeDef *) RCC_BASE)
#define SysTick               ((SysTick_TypeDef *) SYSTICK_BASE)

/* RCC AHB1ENR Bits */
#define RCC_AHB1ENR_GPIOAEN   (1UL << 0)
#define RCC_AHB1ENR_GPIOCEN   (1UL << 2)

/* SysTick Control Bits */
#define SysTick_CTRL_ENABLE   (1UL << 0)
#define SysTick_CTRL_TICKINT  (1UL << 1)
#define SysTick_CTRL_CLKSOURCE (1UL << 2)

#endif /* STM32F401XE_H */
