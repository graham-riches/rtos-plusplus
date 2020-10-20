/*! \file system.c
*
*  \brief System clock setups.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "system.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"

/*********************************** Consts ********************************************/
#define SYSCLK_PLL_M      8
#define SYSCLK_PLL_N      336
#define SYSCLK_PLL_P      2
#define SYSCLK_PLL_Q      7
#define SYSCLK_CONFIGURED 168000000l

/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/


/****************************** Functions Prototype ************************************/
void SYSTEM_clockConfig(void);

/****************************** Functions Definition ***********************************/
/**
* \name     SYSTEM_init
* \brief    Initialize the system
*
* \param    None
* \retval   None
*/
void SYSTEM_init( void )
{
	/* initialize the clocks */
	SYSTEM_clockConfig();
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SYSTEM_clockConfig(void)
{
  volatile uint32_t StartUpCounter = 0;
  volatile uint32_t HSEStatus = 0;
  
  /* Enable HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }

  if (HSEStatus == (uint32_t)0x01)
  {
    /* Select regulator voltage output Scale 1 mode */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;

    /* HCLK = SYSCLK / 1*/
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
 
    /* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
    
    /* PCLK1 = HCLK / 4*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

    /* Configure the main PLL */
    RCC->PLLCFGR = SYSCLK_PLL_M | (SYSCLK_PLL_N << 6) | (((SYSCLK_PLL_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_HSE) | (SYSCLK_PLL_Q << 24);

    
    /* Enable the main PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till the main PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
   
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

    /* Select the main PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
    {
    }
  }
}
