
#include "kostur_kart_tanimlar.h"
#include "dfPlayer.h"

/*                .................. */
void hc595_ledler_yak(int sure){
        o1_L1_on();
        delay_ms(sure);
        o1_L2_on();
        delay_ms(sure);
        o1_L3_on();
        delay_ms(sure);
        
        o2_L1_on();
        delay_ms(sure);
        o2_L2_on();
        delay_ms(sure);
        o2_L3_on();
        delay_ms(sure);
        
        rgb_kirmizi_on();
        delay_ms(sure);
        
        rgb_yesil_on();
        delay_ms(sure);
        
        rgb_mavi_on();
        delay_ms(sure);
//        
//        uLed1_on();
//        delay_ms(sure);
//        
//        uLed2_on();
//        delay_ms(sure);
}
void hc595_ledler_sondur(int sure){
        o1_L1_off();
        delay_ms(sure);
        o1_L2_off();
        delay_ms(sure);
        o1_L3_off();
        delay_ms(sure);
        
        o2_L1_off();
        delay_ms(sure);
        o2_L2_off();
        delay_ms(sure);
        o2_L3_off();
        delay_ms(sure);
        
        rgb_kirmizi_off();
        delay_ms(sure);
        
        rgb_yesil_off();
        delay_ms(sure);
        
        rgb_mavi_off();
        delay_ms(sure);
        
//        uLed1_off();
//        delay_ms(sure);
//        
//        uLed2_off();
//        delay_ms(sure);
}
void motorPwmTest(unsigned int16 duty, char yon){
    
    set_pwm1_duty(duty);        //motorlar yaklaþýk olarak 400 den sonra dönmeye baþladýlar. 
    set_pwm2_duty(duty);
    if(yon==1){
        motor1_ileri();
        motor2_ileri();
    }else{
       motor1_geri();
       motor2_geri(); 
    }
    
}
