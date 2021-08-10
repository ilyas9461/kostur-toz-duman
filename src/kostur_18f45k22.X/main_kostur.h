/* 
 * File:   main_kostur.h
 * Author: ilyas
 *
 * Created on 05 Mayýs 2018 Cumartesi, 11:49
 */

#ifndef MAIN_KOSTUR_H
#define	MAIN_KOSTUR_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    #include <18F45K22.h>
    //#include <18F4520.h>
    #device adc=10
    #device HIGH_INTS=TRUE 
    #device PASS_STRINGS=IN_RAM  

//    #fuses HSM, NOWDT, BROWNOUT, PUT, NOPBADEN, NOHFOFST 
//    #fuses NOPLLEN        //HW PLL disabled, PLL enabled in software 
//    #fuses MCLR           //Master Clear pin enabled 
//    #fuses PROTECT        //Code protected from reads 
//    #use delay(clock=10MHz,crystal=10MHz)
    
    #FUSES NOWDT                    //No Watch Dog Timer
    #FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
    #FUSES NOFCMEN                  //Fail-safe clock monitor disabled
    #FUSES NOIESO                   //Internal External Switch Over mode disabled
    #FUSES NOBROWNOUT               //No brownout reset
    #FUSES NOPBADEN                 //PORTB pins are configured as digital I/O on RESET
    #FUSES NOHFOFST                 //High Frequency INTRC waits until stable before clocking CPU //1845k22 de AKTÝF
    #FUSES NOSTVREN                 //Stack full/underflow will not cause reset
    #FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
    #FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
    #FUSES PROTECT                  //Code protected from reads
    #FUSES PLLEN
    #FUSES PRIMARY_ON  
    #FUSES MCLR 

    #use delay(clock=40000000,crystal=10000000)   

//    #FUSES NOWDT 
//    #FUSES WDT1                     //Watch Dog Timer uses 1:1 Postscale 
//    #FUSES HSH                      //High speed with 4x PLL 
//    #FUSES PROTECT                //Code  protected from reading 
//    #FUSES NOBROWNOUT               //No brownout reset 
//    #FUSES NOPUT                    //No Power Up Timer 
//    #FUSES NOCPD                    //No EE protection 
//    #FUSES NOSTVREN                   //Stack full/underflow will cause reset 
//    #FUSES NODEBUG                  //No Debug mode for ICD 
//    #FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O 
//    #FUSES WRT                    //Program memory not write protected 
//    #FUSES WRTD                   //Data EEPROM not write protected 
//    #FUSES NOIESO                     //Internal External Switch Over mode enabled 
//    #FUSES NOFCMEN                    //Fail-safe clock monitor enabled 
//    #FUSES NOPBADEN                 //PORTB pins are configured as digital I/O on RESET 
//    #FUSES NOWRTC                   //configuration not registers write protected 
//    #FUSES NOWRTB                   //Boot block not write protected 
//    #FUSES NOEBTR                   //Memory not protected from table reads 
//    #FUSES NOEBTRB                  //Boot block not protected from table reads 
//    #FUSES NOCPB                    //No Boot Block code protection 
//    #FUSES MCLR                     //Master Clear pin enabled 
//    #FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode) 
//    #FUSES PLLEN                    //Enable PLL 4x Clock 
//    #FUSES PRIMARY_ON    
//    #FUSES NOHFOFST                 //High Frequency INTRC waits until stable before clocking CPU 
//
//    #use delay(clock=40Mhz, crystal=10MHz) 
    
    #use fast_io(A)
    #use fast_io(B)
    #use fast_io(C)
    #use fast_io(D)
    #use fast_io(E)

    #use rs232(baud=9600,parity=N, xmit=PIN_D6, rcv=PIN_D7,bits=8,DISABLE_INTS,STREAM=softSerial)   //seri port pin tanýmlama
    #use rs232(baud=9600,parity=N, xmit=PIN_C6, rcv=PIN_C7,bits=8,DISABLE_INTS,STREAM=dfPlayerSerial)   //seri port pin tanýmlama
    //#use rs232(uart1,baud =9600,DISABLE_INTS,STREAM=dfPlayerSerial) 
    //#use rs232(uart2,baud =9600,DISABLE_INTS,STREAM=softSerial)
    
    //#use timer(timer=1,tick=100us,bits=32,NOISR)  
    //#USE TIMER(TIMER=1,TICK=1ms,BITS=32,ISR)  
    #define TICK_TYPE unsigned int32 

    extern unsigned int32 timerIntSay;
    int32 oyunSureSaySaniye=0;
    
    void delay_m_us(unsigned int16 n) ;
    long map(long x, long in_min, long in_max, long out_min, long out_max);
    void ISR_RB0();

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_KOSTUR_H */

