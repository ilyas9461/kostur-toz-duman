/* 
 * File:   main_kostur.c
 * Author: ilyas YAGCIOGLU
 * Created on 05 Mayýs 2018 Cumartesi, 11:49
 * 
 * P10 yokkkkkk
 */

//#define  TOZ_DUMAN   //TOZ_DUMAN        KOSTUR      //hangi donaným için olduðu tanýmlnamalý.
#define  KOSTUR

#include "main_kostur.h"
#include "timer.c"
#include "timer.h"
#include <stdlib.h> //rand vb ,için
#include "kostur_kart_tanimlar.h"

unsigned int16 sens_max_kal=0;
unsigned int16 sens_min_kal=0;
signed int sw_sa_algila=-1;
unsigned int16 last_adc_deger=0;
unsigned int32 jtnCounter=0;
unsigned int8 time_sens_okuma_sayisi=40;

//#include "P10_Panel_Driver.c"
#include "hc595_spi.c"
#include "kostur_donanim_test.c"
#include "flex_lcd.c"
#include "dfPlayer.c"
#include "dfPlayer.h"
#include "top_sw_islemler.c"
#include "lcd_menu_islem.c"

extern unsigned int32 oyunBitisSureBasla;    // Ayarlanan oyun bitiþ süresi için
extern unsigned int32 oyunSureSayBasla;      //
extern unsigned int32 oyunSureSaySaniye;     // Oyun süresini saniye olarak sayar.  

unsigned long long ledAnimSureBasla=0;
int animSay=0;
TICK_TYPE time_sens_oku=0,previousTick=0;

char text[100];
extern int1 jetonDurum;
int1 jetonAtildi=FALSE;

extern int8 jtnTLMik;
extern unsigned int8 ee_oyunSure;
int8 btnBasSay=0;
unsigned int16 maxOyunSure=0xFFFF;

int16 delayMsSay=0,delay_mt_geri=0;

#include "kostur_anim.h"
#include "kostur_anim.c"

int anim=P10_animKosturBlink;

/// global fonksiyonlar

void mtr1Mtr2IlkKonum();
void motorGeriLedAnim();

 
void main(void){
    
   setup_ccp3(CCP_OFF); 
   setup_ccp4(CCP_OFF); 
   setup_ccp5(CCP_OFF); 
   
   setup_timer_1(T1_DISABLED); 
   setup_timer_2(T2_DIV_BY_16, 250, 1);   // 40MHz fosc ile 2.5KHz yaklaþýk(div_16)
   
   setup_ccp1(CCP_PWM ); 
   setup_ccp2(CCP_PWM ); 

   setup_timer_4(T4_DISABLED,0,1); 
   setup_timer_5(T5_DISABLED);
   setup_timer_6(T6_DISABLED,0,1); 
   
 
   setup_dac(DAC_OFF); 
   setup_comparator(NC_NC_NC_NC); 
   setup_vref(FALSE); 
   setup_adc(ADC_OFF); 
   
   setup_adc_ports(sAN0|sAN1);
   //setup_adc_ports(AN0_TO_AN1);   //18f4520
   setup_adc(ADC_CLOCK_DIV_8);

   //setup_spi(SPI_MASTER | SPI_L_TO_H |SPI_SS_DISABLED);//P10 veri spi gondermek için
  //setup_spi(SPI_MASTER | SPI_H_TO_L | SPI_SS_DISABLED);//P10 veri spi gondermek için
   
   set_tris_a(0b11101111);
   set_tris_b(0b00000011);
   set_tris_c(0b10000000);
   set_tris_d(0b10000000);
   set_tris_e(0b0000);
   
   hc_595_clr_allPins();
   //motor3_geri();delay_ms(100); motor3_dur();
   timer_kur();
   ext_int_edge(H_TO_L);      // init interrupt triggering for button press 
   enable_interrupts(INT_EXT);
   enable_interrupts(GLOBAL);
 
    //set_pwm1_duty(512);       //yarým peryot motoru döndürmediiii
   set_pwm1_duty(1023);
   set_pwm2_duty(1023);
   
   P10_baslangic();
   
   lcd_init();  // Always call this first. 
   
   if(read_eeprom(ee_adres_jtnTL)!=0xFF) jtnTLMik=read_eeprom(ee_adres_jtnTL);
   else jtnTLMik=1;
       
   jtnCounter=make32(read_eeprom(ee_adres_jtnCount+3),read_eeprom(ee_adres_jtnCount+2),
                          read_eeprom(ee_adres_jtnCount+1),read_eeprom(ee_adres_jtnCount));
    
   if(jtnCounter==0xffffffff) jtnCounter=0x00000000;

   if(read_eeprom(ee_adres_oyunSure)!=0xFF) ee_oyunSure=read_eeprom(ee_adres_oyunSure);
   else ee_oyunSure=2;
   
   sens_max_kal=make16(read_eeprom(ee_adr_an_max_kal_h),read_eeprom(ee_adr_an_max_kal_l));
   if(sens_max_kal==0xffff){
      sens_max_kal=410;  
   }
   
   sens_min_kal=make16(read_eeprom(ee_adr_an_min_kal_h),read_eeprom(ee_adr_an_min_kal_l));
   if(sens_min_kal==0xffff){
        sens_min_kal=205;
   } 
   
   if(read_eeprom(ee_adr_sens_okuma_sayisi)!=0xFF)  
        time_sens_okuma_sayisi=read_eeprom(ee_adr_sens_okuma_sayisi);
   else time_sens_okuma_sayisi=20;// 25mS,saniyede 40 defa ölçüm yap default deger.
   
   printf(lcd_putc,"\fSENS OKUMA SAYI");
   printf(lcd_putc,"\n%u -> t:%u mS",(unsigned int8)(1000/time_sens_okuma_sayisi),time_sens_okuma_sayisi);
   delay_ms(2000);
   
   output_high(lcd_led);
   
   printf(lcd_putc,"\fKOSTUR KART V1.0");
   printf(lcd_putc,"\n%05Lu  jtn:%uTL",jtnCounter,jtnTLMik);

   hc_595_clr_allPins();
   fprintf(softSerial,"Basla ...\r\n");
   
    mtr1Mtr2IlkKonum();
   
    if(read_eeprom(ee_adres_dfSes)!=0xFF)
          // mp3_set_volume(read_eeprom(ee_adres_dfSes));
        dfPlayer_ses=read_eeprom(ee_adres_dfSes);
    else dfPlayerNormSes();

    mp3_baslangic(dfPlayer_ses);
       
   output_high(lcd_led);
   delayMsSay=0;
   
   while(TRUE){
       
       if(jetonAtildi){
           
            hc_595_clr_allPins();
            disable_interrupts(INT_EXT);
            jtnCounter++;
            
            write_eeprom(ee_adres_jtnCount,make8(jtnCounter,0));    //low
            write_eeprom(ee_adres_jtnCount+1,make8(jtnCounter,1));
            write_eeprom(ee_adres_jtnCount+2,make8(jtnCounter,2));
            write_eeprom(ee_adres_jtnCount+3,make8(jtnCounter,3));//high
            
            printf(lcd_putc,"\fKOSTUR KART V1.0");
            printf(lcd_putc,"\n%05Lu  jtn:%uTL",jtnCounter,jtnTLMik);

            jetonAtildiIslem();
            
            
        }else{
           // output_low(ticket_out_pin) ; 
           // rgb_beyaz_off();
        }
       
        if(jetonDurum==TRUE){     // Oyun baþatýldý
            
           // P10_calisma_ani(millis()-oyunSureSayBasla); 
            jetonAtildi=FALSE ;
            time_sens_oku=millis();
            
            
            enable_interrupts(INT_TIMER0);
            
            setMillis(0);
            
            int last_sw1=0, last_sw2=0;
            int islem_durum1=0, islem_durum2=0;
            long m1_sure=0;
            long m2_sure=0;
            unsigned long long time_mp3_remix=0;   //32 bit int
            unsigned long long time_sens_hata=0;
           // rgb_beyaz_off();
            
            hc_595_clr_allPins();
            rgb_beyaz_on();
            oyunBitisSureBasla=millis();
            // oyun oynanýyor....
            do{
                
                if(millis()-time_mp3_remix>2000){
                    
                   if(input(DF_BUSY_PIN)){
                       mp3_stop();
                       delay_m_us(100);//delay_ms(100);
                       mp3_play(mp3_remix);
                   } 
                   time_mp3_remix=millis();
                }
               
                if(millis()-time_sens_oku>time_sens_okuma_sayisi)       //  50 defa olçüm yap 20mS de bir.
                {
                    sw_sa_algila=swSaAlgila(SENS_OKU_H_L);   //yaklaþýk (100us*16)*6=10mS iþlem süresi
                    time_sens_oku=millis();
                    
                }//else sw_sa_algila=0xFF;
                
                if(!sw_sa_algila)f_oyunBitti=TRUE;
                
                if(sw_sa_algila!=0xFF)
                {       
                    sw_ses_oynat(sw_sa_algila);

                    islem_durum1=islem_durum2=0;
                    
                    if(sw_sa_algila==sw_11 || sw_sa_algila==sw_12 || sw_sa_algila==sw_13){
                       m1_sure+=(long)(m_sure_hesapla(sw_sa_algila)/2); 
                       last_sw1=sw_sa_algila;
                    }
                    if(sw_sa_algila==sw_21 || sw_sa_algila==sw_22 || sw_sa_algila==sw_23){
                       m2_sure+=(long)(m_sure_hesapla(sw_sa_algila)/2); 
                       last_sw2=sw_sa_algila;
                    }
                    
                    time_mp3_remix=millis();
                    time_sens_hata=millis();
                }                

                if(last_sw1==sw_11 || last_sw1==sw_12 || last_sw1==sw_13)  //boylece motorlar için süreklilik saðlanmýþ olur.
                {
                    islem_durum1=sw_motor_oynat(last_sw1,m1_sure);
                    
                }
                if(last_sw2==sw_21 || last_sw2==sw_22 || last_sw2==sw_23)
                {
                    islem_durum2=sw_motor_oynat(last_sw2,m2_sure);
                }
    
                if(islem_durum1){
                    last_sw1=sw_sa_algila;  //motor iþlemi bitti ise yeni sensor blgisini yükle.
                    if(sw_sa_algila==0xFF) m1_sure=0;   //sensor yoksa süre 0
                }
                if(islem_durum2){
                    last_sw2=sw_sa_algila;
                    if(sw_sa_algila==0xFF) m2_sure=0;
                }

                printf(lcd_putc,"\fSENS: %lu ",last_adc_deger);
                printf(lcd_putc,"\nOyun: %Lu",(millis()-oyunBitisSureBasla));
                
                if(f_oyunBitti||(millis()-oyunBitisSureBasla) > ((unsigned long long)(_mul(ee_oyunSure,60000LL)))  // 3*60*1000 mS
                   ||(sa_m1_2())||(sa_m2_2()))
                {
                    f_oyunBitti=TRUE; 
                }
                 
            }while(!f_oyunBitti);

            //p10_oyun_puan_goster(o1_puan,o2_puan);
            //disable_interrupts(INT_TIMER0);
            // Oyun bitti...
                f_oyunBitti=FALSE;
                enable_interrupts(INT_EXT);
                
                mp3_stop();delay_ms(100);
                mp3_play(wav_heySesi);

                printf(lcd_putc,"\fOyun Bitti");
                rgb_beyaz_off();
                sw_led_on();
                
                motor1_dur();
                motor2_dur();
                motor3_geri();delay_ms(1000);motor3_dur();  //Oyun biter bitmez top haznesini kapat...Çekme motor suresi uzun olursa >5-10 saniye 
                delay_ms(1000);                             // çekme motoru yanabilir.
                
                if((unsigned int16)(oyunSureSaySaniye/1000)<maxOyunSure)// en az süre için
                    maxOyunSure=(unsigned int16)(oyunSureSaySaniye/1000);       //1000uS kesme olduðu için TIMER0
                
                //p10_oyun_bitti(o1_puan,o2_puan,(unsigned int16)(oyunSureSaySaniye/1250));
                oyunSureSaySaniye=0;
                delayMsSay=0; //motorlar baþlangýç konumuna gelirkne ledanim+max switc bekleme süresi.
                 //motorlar geri ilk konumlarýna 
                sw_led_off();
                
                //motor1_geri();
                //motor2_geri();
                
                if(!sa_m1_1()){ // motor 1 baslangýçta deðilse
                    motor1_geri();      
                }
                if(!sa_m2_1()){ // motor 2 baslangýçta deðilse
                   motor2_geri();
                }
                
                mp3_stop();delay_ms(100);
                mp3_play(wav_alkisSesi);
                
                do{ 
                    sw_led_off();  
                    if(sa_m1_1()){ // motor 1 baslangýç SA
                         motor1_dur();
                    }
                    if(sa_m2_1()){ // motor 2 baslangýç SA
                         motor2_dur();
                    }
                    // Eðer çekme motoru yerine motor kullanýýrsa...
//                    if(sa_m3_1()){ // motor 3 baslangýç SA
//                         motor3_dur();
//                    }
                    if((sa_m1_1())&&(sa_m2_1())){
                        //P10_baslangic();
                        break;
                    }
                    
                  motorGeriLedAnim();  //switch max bekleme süresi eklendi

                }while(1);
                
                delay_mt_geri=0;
                delayMsSay=0;
                sw_led_off();
                motor1_dur();
                motor2_dur();
                motor3_dur();
                
                mp3_stop(); 
                delay_ms(100);
//               // mp3_enableLoopAll();
                mp3_play(mp3_remix);
               
                jetonDurum=FALSE;
                f_oyunBitti=FALSE;
   
                
                if(read_eeprom(ee_adres_dfSes)!=0xFF) 
                    mp3_set_volume(read_eeprom(ee_adres_dfSes));
                else dfPlayerNormSes();
                
                //reset_cpu(); //motorlar geri geldfiktren sonra
            //oyun bitti
        }else if(!jetonDurum){
            //Oyun oynanmýyor Oyuncak BOÞTA....
            
            if(read_eeprom(ee_adres_jtnTL)!=0xFF) jtnTLMik=read_eeprom(ee_adres_jtnTL);
                else if(read_eeprom(ee_adres_jtnTL)==0xFF) jtnTLMik=1;
                
            printf(lcd_putc,"\fKOSTUR KART V1.0");
            printf(lcd_putc,"\n%05Lu  jtn:%uTL",jtnCounter,jtnTLMik);
                      
            ledAnimSureBasla=millis();
            enable_interrupts(INT_TIMER0);
            
            do{
              // dfSesAyarla(); 
              // jetonKontrol();
                
                if(input(DF_BUSY_PIN)){
                    mp3_rastgele();
                }
                
                if(!input(btn2)){
                  //delay_ms(250);
                   btnBasSay++;
                   output_high(lcd_led);
                   if(btnBasSay>2){
                        btnBasSay=0;
                        output_high(lcd_led);
                        printf(lcd_putc,"\f ...AYARLAR... ");
                        printf(lcd_putc,"\n<<<    <>    >>>");
                        delay_ms(1000);
                        hc_595_clr_allPins();
                        lcdMenuIslem();  
                        
                    }
                }else{
                   // output_low(lcd_led);
                    btnBasSay=0;
                }
                
                switch(anim){
                    
                    case led_animKarasimsek:
                       
                        if(millis()-ledAnimSureBasla>7210LL){
                           anim=P10_animTL;
                           ledAnimSureBasla=millis();
                           animSay=0;
                           hc_595_clr_allPins();
                          
                        }
                        ////delay_ms(175);delay_ms boyunca millis() çalýþmadý. timer interruyput delay_ms den sonra çalýþmaya devam etti
                        delay_m_us(150);// delay_ms yerine  timer0 saymasý çok etkilenmedi. 
                        led_anim_bosta(animSay++);
                        //printf(lcd_putc,"\f anim_bosta");
                        //printf(lcd_putc,"\nmS:%Lu",(millis()-ledAnimSureBasla));
                        
                        if(animSay>9)animSay=0;
                        break;
                        
                    case P10_animKosturBlink:
                         p10_kosturAnim();
                         anim=led_animKarasimsek;
                         ledAnimSureBasla=millis();
                         animSay=0;
                        break;
                    case led_animKarinca:
                       
                        if(millis()-ledAnimSureBasla>8500LL){
                           anim=P10_animMaxSurePuan;
                           ledAnimSureBasla=millis();
                           animSay=0;
                           hc_595_clr_allPins();
                          
                        }
                        //delay_ms(175);
                        delay_m_us(150);
                        ledAnimKarinca(animSay++);
                        
                        if(animSay>11)animSay=0;
                        break;
                    case P10_animTL:
                        
                        p10_TL_anim();
                        
                        anim=P10_SirketReklam;
                        ledAnimSureBasla=millis();
                        animSay=0;
                        break;
                    case P10_animMaxSurePuan:
                        
//                        if(o1_puan>o2_puan)P10_animMaxPuanMinSure(o1_puan,maxOyunSure);
//                        else if(o1_puan<o2_puan)P10_animMaxPuanMinSure(o2_puan,maxOyunSure);
//                        else if(o1_puan==o2_puan)P10_animMaxPuanMinSure(o1_puan,maxOyunSure);
                        
                        anim= P10_animKosturBlink;
                        ledAnimSureBasla=millis();
                        animSay=0;
                        break;
                    case P10_SirketReklam :
                        P10_animkosturSirket();
                        anim=led_animKarinca;
                        ledAnimSureBasla=millis();
                        animSay=0;
                        break;

                }
                
            }while(!jetonAtildi);
        }//else oyuncak boþta ...
        
    }//while(TRUE)
    
}//main

/********************************Kesme Alt Program****************************/
#INT_EXT //HIGH      //Set external interrupt on pin RB0 
void ISR_RB0() 
{ 
  // mp3_stop();
  // mp3_play_physical(wav_jeton);
   printf(lcd_putc,"\fJETON");

   jetonKontrol();
   //if(jetonAtildi) jetonAtildiIslem();
}

// **!!! delay_ms boyunca millis() çalýþmadý. timer interrupt delay_ms den sonra çalýþmaya devam etti
// delay_ms yerine  kullanýulýnca timer0 kesmesi çok etkilenmedi. 
void delay_m_us(unsigned int16 n) {
    
    for (;n!=0; n--)
        delay_us(1000);
}

 

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
 	    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void mtr1Mtr2IlkKonum(){
    
   delayMsSay=0;
   int1 motor3_dur=FALSE;
   if(!sa_m1_1())       //baþlangýc sýnýr anahtarlarý düþen kenarý algýlar düþen kenarda 1 degeri verir
   {
       motor1_geri();
      // motor3_geri(); //çekme motoru
   }
   if(!sa_m2_1()){
       motor2_geri();
      // motor3_geri(); //çekme motoru
   }
   motor3_geri(); //çekme motoru
   
    do{ 
        if(sa_m2_1()){
            motor2_dur();
        }
        if(sa_m1_1()) {
            motor1_dur();  // makro ifadelere dikkat... {} parantez kullanýlmalý...
        } 
        
        if((sa_m1_1())&&(sa_m2_1())) 
            break;

        if(delay_mt_geri>100) //100 ün katlarýnda
        {
            sw_led_on();
        }
        
        if(delay_mt_geri>200){
            spiDataL=0;
            delay_mt_geri=0;
            sw_led_off();
        }
        if(delayMsSay>200 && !motor3_dur) {
            //delayMsSay=0;
            motor3_dur=TRUE;
            motor3_dur();       //çekme motoru
        }
        
        if(delayMsSay>15000)
        {
            delayMsSay=0;
            break;
        }
        
        delayMsSay++;
        delay_m_us(1);
           
    }while(1);
    delay_mt_geri=0;
    delayMsSay=0;
    motor1_dur();
    motor2_dur();
    motor3_dur();
}

void motorGeriLedAnim(){
                
    if(delay_mt_geri>100) //100 un katlarýnda
    {
            sw_led_on();
    }

    if(delay_mt_geri>200){
        spiDataL=0;
        delay_mt_geri=0;
        sw_led_off();
    }
//    if(delayMsSay>200) {
//        //delayMsSay=0;
//    }
    if(delayMsSay==3000){
        motor3_geri();
    }
    if(delayMsSay==3500){
        motor3_dur();
    }
    if(delayMsSay>15000)
    {
          delayMsSay=0;
          break;
    }
    delayMsSay++;
    delay_mt_geri++;
    delay_m_us(1);
    
}
