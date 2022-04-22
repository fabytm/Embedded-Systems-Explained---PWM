
#include "msp430.h"

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR = BIT2;                             // Set P1.2 (PWM) to output direction
  P1SEL = BIT2;                             // Select TA0.1 function for P1.2
  P1IE |=  BIT3;                            // P1.3 (button) interrupt enabled
  P1IES |= BIT3;                            // P1.3 Hi/lo edge
  P1REN |= BIT3;		                        // Enable Pull Up on SW2 (P1.3)
  P1IFG &= ~BIT3;                           // P1.3 IFG cleared
    	  	  	  	  	                      // BIT3 on Port 1 can be used as Switch2
  TA0CCR0 = 1000-1;                         // PWM Period --> 1ms
  TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
  TA0CCR1 = 500;                            // CCR1 PWM duty cycle (50% at the beginning)
  TA0CTL = TASSEL_2 + MC_1;                 // SMCLK (1MHz), up mode

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM4 w/interrupt
}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  if(TA0CCR1 < 900)                         // If duty cycle hasn't reached 90% yet
    TA0CCR1 += 100 ;                        // Increase it by 10%
   else
     TA0CCR1 = 100;                         // Else set it back to 10%
  P1IFG &= ~BIT3;                           // Clear P1.3 IFG flag
}
