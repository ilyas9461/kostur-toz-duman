#include "kostur_kart_tanimlar.h"

//extern int8 jtnTLMik;
extern int8 ee_oyunSure;

void sensOkumaSayisiAyar(void);
void sesAyar(void);
void jtnTLayar(void);
void oyunSureAyar(void);
void sensorTest(char sensHL);
void sensorKalibre(char sensHL);
void motorKurulum(void);
void resetIslemi(void);

void lcdMenuIslem(void){
    signed int btn=0;

    hc_595_clr_allPins();
    //P10_Clear_Display(0);
    delay_ms(100);
    disable_interrupts(INT_TIMER0);
   
    while(1){
        
        if(!input(btn1)){
            delay_ms(50);
            btn--;
            if(btn<=0)btn=9;
            //printf(lcd_putc,"\f\nbtn:%d   ",btn);
        }

        if(!input(btn3)){
            delay_ms(50);
            btn++;
            if(btn>9)btn=1;
            //printf(lcd_putc,"\f\nbtn:%d  ",btn);
        }
        
        if(btn==1){
             printf(lcd_putc,"\f ...Jeton TL... ");
             printf(lcd_putc,"\n<<<    <>    >>>");
             if(!input(btn2)){
                delay_ms(100);
                jtnTLayar();
                btn=2;
             }
        }
        if(btn==2){
            printf(lcd_putc,"\f ..Oyun Suresi..");
            printf(lcd_putc,"\n<<<    <>    >>>");
            if(!input(btn2)){
                delay_ms(100);
                oyunSureAyar();
                btn=3;
             }
        }
         if(btn==3){
             printf(lcd_putc,"\f..SENS KALIBRE..");
             printf(lcd_putc,"\n<<<    <>    >>>");
             if(!input(btn2)){
                delay_ms(100);
                sensorKalibre(SENS_OKU_H_L);
                btn=4;
             }
        }
        if(btn==4){
             printf(lcd_putc,"\f ..Sensor Test..");
             printf(lcd_putc,"\n<<<    <>    >>>");
             if(!input(btn2)){
                delay_ms(100);
                sensorTest(SENS_OKU_H_L);
                btn=5;
             }
        }
        if(btn==5){
            printf(lcd_putc,"\f .Motor Kurulum.");
            printf(lcd_putc,"\n<<<    <>    >>>");
             if(!input(btn2)){
                delay_ms(100);
                motorKurulum();
                btn=6;
             }
        }
        if(btn==6){
            printf(lcd_putc,"\f  ...Reset...  ");
            printf(lcd_putc,"\n<<<    <>    >>>");
            if(!input(btn2)){
                delay_ms(100);
                resetIslemi();
                btn=7;
             }
        }
         if(btn==7){
            printf(lcd_putc,"\f  ...SES...  ");
            printf(lcd_putc,"\n<<<    <>    >>>");
            if(!input(btn2)){
                delay_ms(100);
                sesAyar();
                btn=8;
                break;
            }
        }
         if(btn==8){
            printf(lcd_putc,"\fSENS OKU SAYISI");
            printf(lcd_putc,"\n<<<    <>    >>>");
            if(!input(btn2)){
                delay_ms(100);
                sensOkumaSayisiAyar();
                btn=9;
                break;
            }
        }
        if(btn==9){
            printf(lcd_putc,"\f  ...CIKIS...  ");
            printf(lcd_putc,"\n<<<    <>    >>>");
            if(!input(btn2)){
                delay_ms(100);
                //lcd_putc("\fKOSTUR KART V1.0\n"); 
                //lcd_putc("jeton:11111 2TL");
                enable_interrupts(INT_TIMER0);
                //output_low(lcd_led);
                break;
            }
        }

       delay_ms(200);
    }//while
    
    printf(lcd_putc,"\fKOSTUR KART V1.0");
    printf(lcd_putc,"\n    LYS-2019");
    
    delay_ms(1000);
    reset_cpu();
}

void sensOkumaSayisiAyar(void){
    unsigned int8 okuma_sayisi=0;
    unsigned int okuma_sayisi_ee_deger=40;      // saniyede 25 defa okuma
     
     if(read_eeprom(ee_adr_sens_okuma_sayisi)!=0xFF)  
            okuma_sayisi=(unsigned int8)(1000/read_eeprom(ee_adr_sens_okuma_sayisi));
     else   okuma_sayisi=25;
    
    printf(lcd_putc,"\fSayi=%u\n",okuma_sayisi);
    printf(lcd_putc,"\n<<<    <>    >>>");
    delay_ms(1000);
    
    do{
        lcd_gotoxy(1,1);
        printf(lcd_putc,"\fSayi=%u\n",okuma_sayisi);
        printf(lcd_putc,"\n<<<    <>    >>>");
        if(!input(btn1)){
            delay_ms(100);
            okuma_sayisi--;
            if(okuma_sayisi<4) okuma_sayisi=4;
        }
        if(!input(btn3)){
            delay_ms(100);
            okuma_sayisi++;
            if(okuma_sayisi>=100) okuma_sayisi=100;
        }
        delay_ms(100);
    } while(input(btn2));
    
    okuma_sayisi_ee_deger=(unsigned int8)(1000/okuma_sayisi);
    
    //Kaydet ve çýk
    write_eeprom(ee_adr_sens_okuma_sayisi,okuma_sayisi_ee_deger);
    time_sens_okuma_sayisi= okuma_sayisi_ee_deger;
   
     printf(lcd_putc,"\f!! Kaydedildi !!");
     printf(lcd_putc,"\nSAYI:%u",(unsigned int8)(1000/time_sens_okuma_sayisi));
     
     delay_ms(1000);
    
}

void sesAyar(void){
    int ses=0;
    
    if(read_eeprom(ee_adres_dfSes)!=0xFF) ses=read_eeprom(ee_adres_dfSes);
    
    printf(lcd_putc,"\fSES=%u\n",ses);
    printf(lcd_putc,"\n<<<    <>    >>>");
    delay_ms(1000);
    
    do{
        lcd_gotoxy(1,1);
        printf(lcd_putc,"\fSES=%u\n",ses);
        printf(lcd_putc,"\n<<<    <>    >>>");
        if(!input(btn1)){
            delay_ms(100);
            ses--;
            if(ses<1) ses=1;
        }
        if(!input(btn3)){
            delay_ms(100);
            ses++;
            if(ses>=30) ses=1;
        }
        delay_ms(100);
    } while(input(btn2));
    
    //Kaydet ve çýk
     write_eeprom(ee_adres_dfSes,ses);
     mp3_set_volume (ses);
     printf(lcd_putc,"\f!! Kaydedildi !!");
     
     delay_ms(1000);
    
}
void jtnTLayar(void){
    int jtn=0;
    if(read_eeprom(ee_adres_jtnTL)!=0xFF) jtn=read_eeprom(ee_adres_jtnTL); 
        
    printf(lcd_putc,"\fjtn TL=%u\n",jtn);
    printf(lcd_putc,"\n<<<    <>    >>>");
    delay_ms(1000);
    do{
        lcd_gotoxy(1,1);
        printf(lcd_putc,"\fjtn TL=%u\n",jtn);
        printf(lcd_putc,"\n<<<    <>    >>>");
        if(!input(btn1)){
            delay_ms(100);
            jtn--;
            if(jtn<1) jtn=1;
        }
        if(!input(btn3)){
            delay_ms(100);
            jtn++;
            if(jtn>10) jtn=10;
        }
        delay_ms(100);
    } while(input(btn2));
    //Kaydet ve çýk
     write_eeprom(ee_adres_jtnTL,jtn);
     printf(lcd_putc,"\f!! Kaydedildi !!");
     
     delay_ms(1000);
    
}

void oyunSureAyar(void){
    
    int sure=2;
    if(read_eeprom(ee_adres_oyunSure)!=0xFF) sure=read_eeprom(ee_adres_oyunSure); 
        
    printf(lcd_putc,"\fOyun Sure=%u dk",sure);
    printf(lcd_putc,"\n<<<    <>    >>>");
    delay_ms(1000);
    do{
        lcd_gotoxy(1,1);
        printf(lcd_putc,"\fOyun Sure=%u dk",sure);
        printf(lcd_putc,"\n<<<    <>    >>>");
        if(!input(btn1)){
            delay_ms(100);
            sure--;
            if(sure<2) sure=2;
        }
        if(!input(btn3)){
            delay_ms(100);
            sure++;
            if(sure>25) sure=25;
        }
        delay_ms(100);
    } while(input(btn2));
    //Kaydet ve çýk
     write_eeprom(ee_adres_oyunSure,sure);
     printf(lcd_putc,"\f!! Kaydedildi !!");
     ee_oyunSure=sure;
     delay_ms(1000);
    
}


const float ADC_ADIM=(float)(5.0/1023.0);
void sensorKalibre(char sensorHL){
 
    signed int sens =0;
    unsigned int16 an_deger=0;
    int i=0;
    hc_595_clr_allPins();
    mp3_stop();
    dfPlayerNormSes();
   
    motor3_ileri();
    delay_ms(500);
    motor3_dur();
    
    an_max_min_reset();
    an_max_adc_oku();
    
   printf(lcd_putc,"\fSENS deger icin,");
   printf(lcd_putc,"\ntop atin...");
   
    do{
        
         sw_sa_algila=swSaAlgila(SENS_OKU_H_L);   //yaklaþýk (100us*16)*6=10mS iþlem süresi
         
         if(sw_sa_algila!=0xFF)
         {       
            sw_ses_oynat(sw_sa_algila);
            
            printf(lcd_putc,"\fSENS: %lu ",last_adc_deger);
            printf(lcd_putc,"\nSENS: %1.2f V",last_adc_deger*ADC_ADIM);
         }
        if(!input(btn2)){
            delay_ms(500);
            break;
         }
       
      
        delay_ms(25);
        
    }while(1);
    
    an_deger=an_max_ort;  
    printf(lcd_putc,"\fMAX ve MIN");
    printf(lcd_putc,"\nbelirleyin...");
    delay_ms(1000);
    
    do{
       
        if(!input(btn1)){
            delay_ms(100);
            an_deger--;
           
        }
        if(!input(btn3)){
            delay_ms(100);
            an_deger++;
            
        }
         if(!input(btn2)){
            delay_ms(500);
            break;
         }
        printf(lcd_putc,"\fMAX DEGER:");
        printf(lcd_putc,"\nSENS: %lu ",an_deger);
         
        delay_ms(100);
    }while(1);
    
    write_eeprom(ee_adr_an_max_kal_h,make8(an_deger,1));        //write_eeprom (address, value)
    write_eeprom(ee_adr_an_max_kal_l,make8(an_deger,0));
            
    an_deger=205;
    
    do{
        
        if(!input(btn1)){
            delay_ms(100);
            an_deger--;
           
        }
        if(!input(btn3)){
            delay_ms(100);
            an_deger++;
            
        }
         if(!input(btn2)){
            delay_ms(500);
            break;
         }
        printf(lcd_putc,"\fMIN DEGER:");
        printf(lcd_putc,"\nSENS: %lu ",an_deger);
        delay_ms(100);
    }while(1);
    
    write_eeprom(ee_adr_an_min_kal_h,make8(an_deger,1)); ////write_eeprom (address, value)
    write_eeprom(ee_adr_an_min_kal_l, make8(an_deger,0));
    
   // sens_max_kal=make16(read_eeprom(ee_adr_an_max_kal_h),read_eeprom(ee_adr_an_max_kal_l));
   // sens_min_kal=make16(read_eeprom(ee_adr_an_min_kal_h),read_eeprom(ee_adr_an_min_kal_l));
    
   // an_max_min_reset();
    
    
  //  ee_an_max_yaz();
    //ee_an_min_yaz();
    
    //ee_sens_kalibre_oku();
    
    write_eeprom(ee_adres_jtnCount,make8(jtnCounter,0));    //low
    write_eeprom(ee_adres_jtnCount+1,make8(jtnCounter,1));
    write_eeprom(ee_adres_jtnCount+2,make8(jtnCounter,2));
    write_eeprom(ee_adres_jtnCount+3,make8(jtnCounter,3));//high
     
     delay_ms(1000);
}

void sensorTest(char sensorHL)
{
    
    printf(lcd_putc,"\fSensor Sinyal???");
    printf(lcd_putc,"\n<<<    <>    >>>");
    hc_595_clr_allPins();
    mp3_stop();
    dfPlayerNormSes();
   
    hc_595_clr_allPins();
    an_max_min_reset();
    
    motor3_ileri();
    delay_ms(500);
    motor3_dur();
    
    an_max_min_reset();
    an_max_adc_oku();
    
    do{
//        if(sw1_1(sensorHL)){
//             o1_L1_on();
//             printf(lcd_putc,"\fSW1_1");
//             printf(lcd_putc,"\n<<<    <>    >>>");
//             output_toggle(PIN_A4);
//             mp3_stop();//delay_ms(100);
//             mp3_play_physical(wav_suDamlasi);
//             delay_ms(1000);
//             o1_L1_off();
//        }
//        if(sw1_2(sensorHL)){
//             o1_L2_on();
//             printf(lcd_putc,"\fSW1_2");
//             printf(lcd_putc,"\n<<<    <>    >>>");
//             output_toggle(PIN_A4);
//             mp3_stop();//delay_ms(100);
//             mp3_play_physical(wav_suDamlasi);
//             delay_ms(1000);
//             o1_L2_off();
//        }
//        if(sw1_3(sensorHL)){
//             o1_L3_on();
//             printf(lcd_putc,"\fSW1_3");
//             printf(lcd_putc,"\n<<<    <>    >>>");
//             output_toggle(PIN_A4);
//             mp3_stop();//delay_ms(100);
//             mp3_play_physical(wav_suDamlasi);
//             delay_ms(1000);
//             o1_L3_off();
//        }
//        
//        if(sw2_1(sensorHL)){
//             o2_L1_on();
//             printf(lcd_putc,"\fSW2_1");
//             printf(lcd_putc,"\n<<<    <>    >>>");
//             output_toggle(PIN_A4);
//             mp3_stop();//delay_ms(100);
//             mp3_play_physical(wav_suDamlasi);
//             delay_ms(1000);
//             o2_L1_off();
//        }
//        if(sw2_2(sensorHL)){
//             o2_L2_on();
//             printf(lcd_putc,"\fSW2_2");
//             printf(lcd_putc,"\n<<<    <>    >>>");
//             output_toggle(PIN_A4);
//             mp3_stop();//delay_ms(100);
//             mp3_play_physical(wav_suDamlasi);
//             delay_ms(1000);
//             o2_L2_off();
//        }
//        if(sw2_3(sensorHL)){
//             o2_L3_on();
//             printf(lcd_putc,"\fSW2_3");
//             printf(lcd_putc,"\n<<<    <>    >>>");
//             output_toggle(PIN_A4);
//             mp3_stop();//delay_ms(100);
//             mp3_play_physical(wav_suDamlasi);
//             delay_ms(1000);
//             o2_L3_off();
//        }
//        
//        if(sa_m1_1()){
//            rgb_kirmizi_on();
//            printf(lcd_putc,"\fSA_M1_1");
//            printf(lcd_putc,"\n<<<    <>    >>>");
//            output_toggle(PIN_A4);
//            mp3_stop();//delay_ms(100);
//            mp3_play_physical(wav_suDamlasi);
//            delay_ms(1000);
//            rgb_kirmizi_off();
//        }
//        if(sa_m1_2()){
//            rgb_yesil_on();
//            printf(lcd_putc,"\fSA_M1_2");
//            printf(lcd_putc,"\n<<<    <>    >>>");
//            output_toggle(PIN_A4);
//            mp3_stop();//delay_ms(100);
//            mp3_play_physical(wav_suDamlasi);
//            delay_ms(1000);
//            rgb_yesil_off();
//        }
//        
//        if(sa_m2_1()){
//            rgb_mavi_on();
//            printf(lcd_putc,"\fSA_M2_1");
//            printf(lcd_putc,"\n<<<    <>    >>>");
//            output_toggle(PIN_A4);
//            mp3_stop();//delay_ms(100);
//            mp3_play_physical(wav_suDamlasi);
//            delay_ms(1000);
//            rgb_mavi_off();
//        }
//        if(sa_m2_2()){
//            rgb_beyaz_on();
//            printf(lcd_putc,"\fSA_M2_2");
//            printf(lcd_putc,"\n<<<    <>    >>>");
//            output_toggle(PIN_A4);
//            mp3_stop();//delay_ms(100);
//            mp3_play_physical(wav_suDamlasi);
//            delay_ms(1000);
//            rgb_beyaz_off();
//        }
//        printf(lcd_putc,"\f   ... ??? ... ");
//        printf(lcd_putc,"\n<<<    <>    >>>");
        
         sw_sa_algila=swSaAlgila(SENS_OKU_H_L);   //yaklaþýk (100us*16)*6=10mS iþlem süresi
         
         if(sw_sa_algila!=0xFF)
         {       
            sw_ses_oynat(sw_sa_algila);
            printf(lcd_putc,"\fSENS: %lu ",last_adc_deger);
            printf(lcd_putc,"\nSENS: %1.2f V",last_adc_deger*ADC_ADIM);
         }
        if(!input(btn2)){
            delay_ms(500);
            break;
         }
       
      
        delay_ms(25);
        
    }while(1);
    
     mp3_stop();delay_ms(100);
     mp3_play(mp3_remix);
     delay_ms(2000);
}


void motorKurulum(void){
   // unsigned long long m1Sure=0,m2Sure=0;
   
    //disable_interrupt(INT_TIMER3);
//    printf(lcd_putc,"\fmotor:?");
//    printf(lcd_putc,"\n<<<    <>    >>>");
//    
//    BOOLEAN m1=0,m2=0;
//    motor1_ileri();
//    motor2_ileri();
  
//    while(1){
        
//        if(sa_m1_2()){
//            m1=1;
//            motor1_dur();
//        }else m1Sure+=10;
//        
//        if(sa_m2_2()){
//            m2=1;
//            motor2_dur();
//        }else m2Sure+=10;
//        
//        if((sa_m1_2()&&sa_m2_2() || (m1 && m2))){
//             motor1_geri();
//             motor2_geri();
//             break;
//        }
//        
//        printf(lcd_putc,"\fmotor1:%Lu",  m1Sure);
//        printf(lcd_putc,"\nmotor2:%Lu",  m2Sure);
//        
//        delay_ms(10);
//    }
//    m1=m2=0;
//    while(1){
//        if(sa_m1_1()){
//            motor1_dur(); 
//            m1=1;
//        }else m1Sure+=10;
//        if(sa_m2_1()){
//            m2=1;
//            motor2_dur(); 
//        }else m2Sure+=10;
//        
//        if((sa_m1_1()&&sa_m2_1()) || (m1 && m2)){
//            
//             break;
//        }
//        
//        printf(lcd_putc,"\fmotor1:%Lu",  m1Sure);
//        printf(lcd_putc,"\nmotor2:%Lu",  m2Sure);
//        
//        delay_ms(10);
//    }
//    motor1_dur(); motor2_dur(); 
////    m1Sure/=10LL;
////    m2Sure/=10LL;
//    
//    printf(lcd_putc,"\fm1:%04Lu  m2:%04Lu",(unsigned int16)(m1Sure>>1),(unsigned int16)(m2Sure>>1));// 2 ye bölme iþlemi
//    printf(lcd_putc,"\n<<<    <>    >>>");
//
//    while(input(btn2));
//    delay_ms(300);   
//                                                 //(m1Sure/2/5)=m1Sure/10  birli delik sure için hesap, 5 atýþta sona ulaþsýn
//    write_eeprom(ee_adres_Msure_1,make8((unsigned int16)(m1Sure/10),0));      //Low byte
//    write_eeprom((ee_adres_Msure_1+1),make8((unsigned int16)(m1Sure/10),1));  //High Byte
//    
//    write_eeprom(ee_adres_Msure_2,make8((unsigned int16)(m2Sure/10),0));      //Low byte
//    write_eeprom((ee_adres_Msure_2+1),make8((unsigned int16)(m2Sure/10),1));  //High Byte
//    delay_ms(2000);
}

void resetIslemi(void){
    
    int i=0;
    printf(lcd_putc,"\fEVET       HAYIR");
    printf(lcd_putc,"\n<<<    <>    >>>");
    delay_ms(100);
    while(!input(btn2));    //orta buton býrakýlýncaya kadar..
    while(1){
        if(!input(btn1)){       //  EVET resetle
            delay_ms(200);
            
            for(i=0;i<41;i++)   //37: sensonr kalibre degerleri hariç
            {
                write_eeprom(i,255);
            }
//            write_eeprom(ee_adres_Msure_1,0xFF);      //Low byte
//            write_eeprom((ee_adres_Msure_1+1),0xFF);  //High Byte
//    
//            write_eeprom(ee_adres_Msure_2,0xFF);      //Low byte
//            write_eeprom((ee_adres_Msure_2+1),0xFF);*
//            
//            write_eeprom(ee_adres_jtnTL,0xFF);
            
            printf(lcd_putc,"\f ! RESET TAMAM !");
            printf(lcd_putc,"\n<<<    <>    >>>");
            
        }
         if(!input(btn2)||!input(btn3)){
             delay_ms(100);
             break;
         }
    }
    
    delay_ms(1000);
   
}


 
        
//        if(sens==5){
//            if(!input(btn2)){
//                delay_ms(500);
//                break;
//            }
//        }
//        
//        if(sens==0){
//            o1_L1_on();
//            o1_L2_off();o1_L3_off();o2_L123_off();
//            
//            for(i=0;i<16;i++){
//                an_sw1_1_sec();
//                set_adc_channel(0); 
//                delay_us(20);
//                an_deger+=read_adc();
//            }
//            an_deger=an_deger>>4;
//            
//            if(an_deger<an_min[sens]){
//                an_min[sens]=an_deger;
//            }
//        }
//        if(sens==1){
//            o1_L2_on();
//            o1_L1_off();o1_L3_off();o2_L123_off();
//           
//            for(i=0;i<16;i++){
//                an_sw1_2_sec();
//                set_adc_channel(0); 
//                delay_us(20);
//                an_deger+=read_adc();
//            }
//            an_deger=an_deger>>4;
//            
//            if(an_deger<an_min[sens]){
//                an_min[sens]=an_deger;
//            }
//        }
//         if(sens==2){
//            o1_L3_on();
//            o1_L1_off();o1_L2_off();o2_L123_off();
//          
//            for(i=0;i<16;i++){
//                an_sw1_3_sec();
//                set_adc_channel(0); 
//                delay_us(20);
//                an_deger+=read_adc();
//            }
//            an_deger=an_deger>>4;
//            
//           if(an_deger<an_min[sens]){
//                an_min[sens]=an_deger;
//            }
//        }
//        
//         if(sens==3){
//            o2_L1_on();
//            o2_L2_off();o2_L3_off();o1_L123_off();
//            
//            for(i=0;i<16;i++){
//                an_sw2_1_sec();
//                set_adc_channel(0); 
//                delay_us(20);
//                an_deger+=read_adc();
//            }
//            an_deger=an_deger>>4;
//            if(an_deger<an_min[sens]){
//                an_min[sens]=an_deger;
//            }
//        }
//        if(sens==4){
//            o2_L2_on();
//            o2_L1_off();o2_L3_off();o1_L123_off();
//            for(i=0;i<16;i++){
//                an_sw2_2_sec();
//                set_adc_channel(0); 
//                delay_us(20);
//                an_deger+=read_adc();
//            }
//            an_deger=an_deger>>4;
//            if(an_deger<an_min[sens]){
//                an_min[sens]=an_deger;
//            }
//        }
//        if(sens==5){
//            o2_L3_on();
//            o2_L1_off();o2_L2_off();o1_L123_off();
//            for(i=0;i<16;i++){
//                an_sw2_3_sec();
//                set_adc_channel(0); 
//                delay_us(20);
//                an_deger+=read_adc();
//            }
//            an_deger=an_deger>>4;
//            
//            if(an_deger<an_min[sens]){
//                an_min[sens]=an_deger;
//            }
//        }
//        
//        esik=(unsigned long)(an_max[sens]-((an_max[sens]-an_min[sens])/2));
//        
//        if(sensorHL==SENS_OKU_H_L){
//            
//            if( an_deger<an_esik){         // top varsa çýkýþ gerilimi düþtü...   0
//                hc_595_clr_allPins();
//                mp3_stop();//delay_ms(100);
//                mp3_play_physical(wav_suDamlasi);
//                delay_ms(250);
//            }
//        }
//        
//        printf(lcd_putc,"\fSen%d: %1.2f-%1.2f",sens,an_max[sens]*ADC_ADIM,an_min[sens]*ADC_ADIM);
//        printf(lcd_putc,"\n <<<  %1.2f  >>>",an_deger*ADC_ADIM);
//        
