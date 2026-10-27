/**
 ******************************************************************************
 * @file    adc.c
 * @author  Silke Behn
 *          HAW-Hamburg
 *          Labor f�r technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    01.12.2020
 * @brief   Funktionen f�r den Zugriff auf den ADC
 *****
*/

#include "stm32f4xx_hal.h"

//Defines f�r CCR ADC common control register
#define CR_CLEAR_MASK                     ((uint32_t)0xFFFC30E0)   
#define ADC_Mode_Independent              ((uint32_t)0x00000000) 
#define ADC_TwoSamplingDelay_8Cycles      ((uint32_t)0x00000300)
#define ADC_DMAAccessMode_Disabled        ((uint32_t)0x00000000)     /* DMA mode disabled */
#define ADC_Prescaler_Div4                ((uint32_t)0x00010000)

//defines f�r CR1 ADC control register 1
#define CR1_CLEAR_MASK                    ((uint32_t)0xF73F0000)         //((uint32_t)0xFCFFEFF)
#define ADC_CR1_SCANMODE_ENABLE           ((uint32_t)0x00000100)
#define ADC_CR1_RESOLUTION_12BIT          ((uint32_t)0x00000000)   //f�r 12Bit Aufl�sung

//defines f�r CR2 ADC control register 2
#define CR2_CLEAR_MASK                    ((uint32_t)0x8080F0FC)     //    ((uint32_t)0xC0FFF7FD)
#define ADC_CR2_CONT_SINGLE               ((uint32_t)0x00000000)
#define ADC_CR2_DATA_ALIGN_RIGHT          ((uint32_t)0x00000000)
#define ADC_CR2_JEXT_DISABLE              ((uint32_t)0x00000000)

//defines f�r SMPR ADC sample time register
#define SMPR_CLEAR_MASK                   ((uint32_t)0x00000007) 
#define ADC_SampleTime_15Cycles           ((uint8_t)0x01)
#define ADC_SampleTime_56Cycles           ((uint8_t)0x11)

//defines f�r SQR ADC regular sequence register
#define SQR_CLEAR_MASK                    ((uint32_t)0x0000001F)  
#define RANK                              ((uint8_t) 0x1 )




/*----------------------------------------------------------------
 * Initialisieren und starten  einem ADC
 * ---------------------------------------------------------------
 * adc  1 oder 2 oder 3 je nach ADC 
 *----------------------------------------------------------------*/
void initADC(int adc){
		
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;        //GPIOF einschalten
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;        //GPIOF einschalten
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;        //GPIOF einschalten
	
	if ( adc == 1 ){
	   RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;        
	}
	else if ( adc == 2 ){
	   RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
	}
	else {	
	   RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;        
  }
	
	
	//GPIO auf Analog stellen
	uint32_t gpioModer = GPIOF->MODER;
	
	//Channel 3_6  GPIOF 8
	gpioModer = ( gpioModer & ~(3 << (2 * 8))) | (GPIO_MODE_ANALOG  << 2 * 8); 
  //Channel 3_7  GPIOF 9
	gpioModer = ( gpioModer & ~(3 << (2 * 9))) | (GPIO_MODE_ANALOG  << 2 * 9); 		
	//Channel 3_8  GPIOF 10
	gpioModer = ( gpioModer & ~(3 << (2 * 10))) | (GPIO_MODE_ANALOG  << 2 * 10); 
	
	GPIOF->MODER = gpioModer;
	
	gpioModer = GPIOB->MODER;
	//Channel 12_9  GPIOB 1
	gpioModer = ( gpioModer & ~(3 << (2 * 1))) | (GPIO_MODE_ANALOG  << 2 * 1); 
	GPIOB->MODER = gpioModer;
	
	
	gpioModer = GPIOC->MODER;
	//Channel 123_10  GPIOC 0
	gpioModer = ( gpioModer & ~(3 << (2 * 0))) | (GPIO_MODE_ANALOG  << 2 * 0); 
	
	//Channel 123_12  GPIOC 2
	gpioModer = ( gpioModer & ~(3 << (2 * 2))) | (GPIO_MODE_ANALOG  << 2 * 2); 
	
	//Channel 123_13  GPIOC 3
	gpioModer = ( gpioModer & ~(3 << (2 * 3))) | (GPIO_MODE_ANALOG  << 2 * 3); 
	
	GPIOC->MODER = gpioModer;
	
	
	
	
	/*---------------------------- ADC CCR Configuration -----------------*/
  uint32_t ccr = 0;
	ccr = ADC->CCR;
  
  ccr &= CR_CLEAR_MASK;
  
  /* Bit  0- 4: 00000  Independent mode */
  /* Bit  8-11:  0011  DELAY zwischen zwei Sampling Raten = 8* T_ADCCLK  */    
	/* Bit 14-15:    00  DMA mode disabled */
	/* Bit 16-17:    01  ADC Prescaler / PCLK2 div 4*/
  ccr |= (uint32_t)(ADC_Mode_Independent |  ADC_TwoSamplingDelay_8Cycles | ADC_DMAAccessMode_Disabled | ADC_Prescaler_Div4   );
                        
  ADC->CCR = ccr;

	
  /*---------------------------- ADCx CR1 Configuration -----------------*/
  uint32_t cr1 = 0;
  if ( adc == 1 )
			cr1 = ADC1->CR1;
  else if ( adc == 2)
			cr1 = ADC2->CR1;
  else 
		  cr1 = ADC3->CR1;
	
	
  cr1 &= CR1_CLEAR_MASK;
  
  /* Configure ADCx: scan conversion mode and resolution */
  /* Bit 8:  1  SCAN mode enable*/
  /* Bit 24-25:  00 12Bit Resolution */ 
  cr1 |= ADC_CR1_SCANMODE_ENABLE  | ADC_CR1_RESOLUTION_12BIT ;
	
	if ( adc == 1 )
			ADC1->CR1 = cr1;
  else if ( adc == 2)
			ADC2->CR1 = cr1;
  else 
		  ADC3->CR1 = cr1;
	
	
  /*---------------------------- ADCx CR2 Configuration -----------------*/
  uint32_t cr2 = 0;
  
	if ( adc == 1 )
			cr2 = ADC1->CR2;
  else if ( adc == 2)
			cr2 = ADC2->CR2;
  else 
		  cr2 = ADC3->CR2;
	
  cr2 &= CR2_CLEAR_MASK;
  
	/* Bit 1:  0  Single conversion mode */
  /* Bit 11: 0  Data Align right  */
  /* Bit 20-21: 00  Trigger detection disabled */ 
  cr2 |= ADC_CR2_CONT_SINGLE | ADC_CR2_DATA_ALIGN_RIGHT | ADC_CR2_JEXT_DISABLE |ADC_CR2_ADON;
                  
  if ( adc == 1 )
			ADC1->CR2 = cr2;
  else if ( adc == 2)
			ADC2->CR2 = cr2;
  else 
		  ADC3->CR2 = cr2;
  

  /*---------------------------- ADCx SQR1 Configuration -----------------*/
  /*Erstmal kein Channel einstellen, 
	  muss noch mit ADC_RegChannelConfig() eingestellt werden */
	if ( adc == 1 ){
		 ADC1->SQR1 = 0x00000000;
	   ADC1->SQR2 = 0x00000000;
	   ADC1->SQR3 = 0x00000000;
	}
  else if ( adc == 2) {
		 ADC2->SQR1 = 0x00000000;
	   ADC2->SQR2 = 0x00000000;
	   ADC2->SQR3 = 0x00000000;
	}	
	else {
		 ADC3->SQR1 = 0x00000000;
	   ADC3->SQR2 = 0x00000000;
	   ADC3->SQR3 = 0x00000000;
	}

}

/*----------------------------------------------------------------------
 * Einstellen, an welchen Channel der Analog Wert anliegt
 * ---------------------------------------------------------------------
 * adc     in Welcher ADC soll genutzt werden
 * channel in  An welchem Channel liegt das Analog Signal an
 *---------------------------------------------------------------------*/
void ADC_RegChannelConfig(int adc, int channel)
{
  
    /*---------------------------- ADCx SMPR Configuration -----------------*/
	  uint32_t smpr = 0;  
		if ( adc == 1 ){		
					//Einstellungen f�r Channel 0-9
					if ( channel < 10){
						 smpr = ADC1->SMPR2;
						 smpr &= ~(SMPR_CLEAR_MASK << (3 * channel));
					
						 //Sampling time f�r den Channel einstellen
						 smpr |= (uint32_t)ADC_SampleTime_56Cycles << (3 * channel);
					
						 ADC1->SMPR2 = smpr;
						
					//Einstellungen f�r Channel 10-18	
					} else {
						 smpr = ADC1->SMPR1;
							
						 smpr &= ~(SMPR_CLEAR_MASK << (3 * (channel-10)));
					
						 //Sampling time f�r den Channel einstellen
						 smpr |= (uint32_t)ADC_SampleTime_56Cycles << (3 * (channel-10));
					
					
						 ADC1->SMPR1 = smpr;
					}	
		 }			
     else if (adc == 2) {					
				  //Einstellungen f�r Channel 0-9
					if ( channel < 10){
						 smpr = ADC2->SMPR2;
						 smpr &= ~(SMPR_CLEAR_MASK << (3 * channel));
					
						 //Sampling time f�r den Channel einstellen
						 smpr |= (uint32_t)ADC_SampleTime_56Cycles << (3 * channel);
					
						 ADC2->SMPR2 = smpr;
						
					//Einstellungen f�r Channel 10-18	
					} else {
						 smpr = ADC2->SMPR1;
							
						 smpr &= ~(SMPR_CLEAR_MASK << (3 * (channel-10)));
					
						 //Sampling time f�r den Channel einstellen
						 smpr |= (uint32_t)ADC_SampleTime_56Cycles << (3 * (channel-10));
					
					
						 ADC2->SMPR1 = smpr;
					}	
		 } else {
          //Einstellungen f�r Channel 0-9
					if ( channel < 10){
						 smpr = ADC3->SMPR2;
						 smpr &= ~(SMPR_CLEAR_MASK << (3 * channel));
					
						 //Sampling time f�r den Channel einstellen
						 smpr |= (uint32_t)ADC_SampleTime_56Cycles << (3 * channel);
					
						 ADC3->SMPR2 = smpr;
						
					//Einstellungen f�r Channel 10-18	
					} else {
						 smpr = ADC3->SMPR1;
							
						 smpr &= ~(SMPR_CLEAR_MASK << (3 * (channel-10)));
					
						 //Sampling time f�r den Channel einstellen
						 smpr |= (uint32_t)ADC_SampleTime_56Cycles << (3 * (channel-10));
					
					
						 ADC3->SMPR1 = smpr;
					}	
		 }			 
		
					
					
					
	  /*---------------------------- ADCx SQR Configuration -----------------*/
 
		uint32_t sqr3 = 0;
		if ( adc == 1) {	  	
				sqr3 = ADC1->SQR3;
				
				sqr3 &= ~(SQR_CLEAR_MASK << (5 * (RANK - 1)));
						
				sqr3 = (uint32_t)channel << (5 * (RANK - 1));
				
				ADC1->SQR3 = sqr3;
		}	else if ( adc == 2) {
			  sqr3 = ADC2->SQR3;
				
				sqr3 &= ~(SQR_CLEAR_MASK << (5 * (RANK - 1)));
						
				sqr3 = (uint32_t)channel << (5 * (RANK - 1));
				
				ADC2->SQR3 = sqr3;
			
		} else {	
    /*---------------------------------------------------*/
		    sqr3 = ADC3->SQR3;
				
				sqr3 &= ~(SQR_CLEAR_MASK << (5 * (RANK - 1)));
						
				sqr3 = (uint32_t)channel << (5 * (RANK - 1));
				
				ADC3->SQR3 = sqr3;
		}	
			
}  

/*----------------------------------------------------------------------
 * Die Umwandlung wird gestartet und der digitale Wert gelesen und 
 * zur�ck geliefert.
 * ---------------------------------------------------------------------
 * adc     in  welcher ADC soll genutzt werden
 *         out ADC Wert
 *---------------------------------------------------------------------*/
int readADCValue(int adc ){
  uint32_t timeout = 0xFFF;
	int result=0;
	
	if ( adc == 1 ){
	   /* Start software conversion */
	   ADC1->CR2 |= (uint32_t)ADC_CR2_SWSTART;
		
	   /* Wait till done */
	   while (!(ADC1->SR & ADC_SR_EOC)) {
		    if (timeout-- == 0x00) {
			    return 0;
		    }
	   }
	   result = ADC1->DR;	 
  } else if ( adc == 2){
     /* Start software conversion */
	   ADC2->CR2 |= (uint32_t)ADC_CR2_SWSTART;
		
	   /* Wait till done */
	   while (!(ADC2->SR & ADC_SR_EOC)) {
		    if (timeout-- == 0x00) {
			     return 0;
		    }
	   }

    	/* Return result */
	    result =  ADC2->DR;
	 } else {
	
	    ADC3->CR2 |= (uint32_t)ADC_CR2_SWSTART;
		
	    /* Wait till done */
	    while (!(ADC3->SR & ADC_SR_EOC)) {
		     if (timeout-- == 0x00) {
			     return 0;
		     }
	    }

	    /* Return result */
	    result = ADC3->DR;
		}
	  return result;
}



