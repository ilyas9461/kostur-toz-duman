#include "kostur_kart_tanimlar.h"
#include "dfPlayer.h"
#include "kostur_anim.h"

unsigned int32 p10puanSureBasla=0;
unsigned int32 oyunSureSayBasla=0;      //
//int y1=-1,y2=-1;

int1 jetonDurum=FALSE;
int8 dfPlayer_ses=20;
int8 jtnTLMik=1;

void jetonAtildiIslem(){
    
    int i=0;
    o1_puan=0;
    o2_puan=0;
    jetonAtildi=FALSE ;
    
    mp3_stop();
    delay_ms(100);
    dfPplayerMaxSes(); 
    delay_ms(100);
    mp3_play_physical(wav_jeton);
    delay_ms(1000);

    //P10_Currentfonts=Font8x8;
//    P10_Currentfonts=Font11x12;
//    textBufSil();
//    for(i=0;i<8;i++){
//        P10_Clear_Display(1);
//        delay_ms(5);    
//        P10_Clear_Display(0);    
//        delay_ms(5);
//        
//        textBufSil();sprintf(text,"Oyun");
//        P10_DisplayShowString(1,0,text, 0);
//        delay_ms(50);
//
//    }

    //mp3_stop();
    
    rgb_beyaz_on();   
    sw_led_off();
    mp3_play(wav_getReady);
    
    if(read_eeprom(ee_adres_Msure_1)!=0xFF && read_eeprom(ee_adres_Msure_1+1)!=0xFF){
        m1_birliDelik_sure=make16(read_eeprom(ee_adres_Msure_1+1),read_eeprom(ee_adres_Msure_1));
        m1_ucluDelik_sure=(unsigned int16)(m1_birliDelik_sure/3);
        m1_ikiliDelik_sure=(unsigned int16)(m1_ucluDelik_sure*2);
    }
     if(read_eeprom(ee_adres_Msure_2)!=0xFF && read_eeprom(ee_adres_Msure_2+1)!=0xFF){
        m2_birliDelik_sure=make16(read_eeprom(ee_adres_Msure_2+1),read_eeprom(ee_adres_Msure_2));
        m2_ucluDelik_sure=(unsigned int16)(m2_birliDelik_sure/3);
        m2_ikiliDelik_sure=(unsigned int16)(m2_ucluDelik_sure*2);
    }
    if(read_eeprom(ee_adres_oyunSure)!=0xFF) ee_oyunSure=read_eeprom(ee_adres_oyunSure);
    else ee_oyunSure=2;
    
    //her durumda oyun baþlamadan önce sensortlerin max degeri oku...
    an_max_min_reset();

    //ee_sens_kalibre_oku();//an min ee de varsa min okunur yoksa min deger reset  degeridir...
    //an_max_adc_oku(); 
    printf(lcd_putc,"\fMAX:%Lu,%Lu,%Lu",an_max[0],an_max[1],an_max[2]);
    printf(lcd_putc,"\n%Lu,%Lu,%Lu:%Lu",an_max[3],an_max[4],an_max[5],an_max_ort);
    delay_ms(1500);
    printf(lcd_putc,"\fMIN:%Lu,%Lu,%Lu",an_min[0],an_min[1],an_min[2]);
    printf(lcd_putc,"\n%Lu,%Lu,%Lu",an_min[3],an_min[4],an_min[5]);
    delay_ms(1500);

    dfPplayerMaxSes();
    mp3_stop();
    delay_ms(100);
    mp3_play(mp3_remix);
   // mp3_play_physical(wav_remix);
   // delay_ms(100);

    for(i=0;i<10;i++) {
        sw_sa_algila=swSaAlgila(SENS_OKU_H_L);
    }
    
    motor3_ileri();
    
//    do{           //Çekme motoru yerine dc gear motor kullanýlýrsa...
//        if(sa_m3_2()){
//           motor3_dur();
//           break;
//        }
//    }while(1);
    
   delay_ms(500);
   motor3_dur();
    
//    for(i=0;i<10;i++){
//        P10_Clear_Display(1);
//        delay_ms(5);    
//        P10_Clear_Display(0);    
//        delay_ms(5);
//        
//        textBufSil();sprintf(text,"Basliyor");
//        P10_DisplayShowString(1,0,text, 0);
//        delay_ms(50);
//    }
//    
//    P10_Clear_Display(0);
//    textBufSil();sprintf(text,"---  ---");
//    P10_DisplayShowString(1,0,text, 0);
  
    jetonDurum=TRUE;

   // enable_interrupts(INT_TIMER3);
    
    //delay_ms(1000);
    //delay_ms(500);
    oyunSureSaySaniye=0;
    //P10_Clear_Display(0);
    
   // p10_oyun_puan_goster(0,0);
            
}

void p10_oyun_puan_goster(unsigned int16 o1Puan,unsigned int16 o2Puan)
{
    
    char t1[8],t2[8];
    rgb_beyaz_on();
    sw_led_off();
    
    //P10_Currentfonts=Font8x8;
    //P10_Currentfonts=Font11x12;
   // P10_Clear_Display(1);
    delay_ms(5);    
    //P10_Clear_Display(0);    
    delay_ms(5);
    
    sprintf(t1,"%Lu",o1Puan);
    sprintf(t2,"%Lu",o2Puan);
//    P10_DisplayShowString(0,0,t1, 0);
//    P10_DisplayShowString(52,0,t2, 0);
     
     printf(lcd_putc,"\f_O1_    _O2_\n");
     printf(lcd_putc,"  %Lu      %Lu  ",o1Puan,o2Puan);

//    if(yeniPuan_o1){
//
//        if(y1==-1){
//            P10_Clear_Display(0);
//            y1++;
//        }
//        
//        if(millis()-p10puanSureBasla>50){
//             p10_DisplayShowString(0,y1,t1, 0);
//             p10puanSureBasla=millis();
//             y1++;
//        }
//        if(y1>3){
//            y1=-1;
//            p10puanSureBasla=millis();
//            P10_Clear_Display(0);
//            //delay_ms(10);
//
//            P10_DisplayShowString(0,4,t1, 0);
//            P10_DisplayShowString(64,4,t2, 0);
//            yeniPuan_o1=FALSE;
//        }
//
//    }else if(yeniPuan_o2){
//        
//        if(y2==-1){
//            P10_Clear_Display(0);
//            y2++;
//        }
//        if(millis()-p10puanSureBasla>50){
//            P10_DisplayShowString(64,y2,t2, 0);
//            p10puanSureBasla=millis();
//            y2++;
//        }
//        if(y2>3){
//            y2=-1;
//            p10puanSureBasla=millis();
//            P10_Clear_Display(0);
//            //delay_ms(10);
//
//            P10_DisplayShowString(0,4,t1, 0);
//            P10_DisplayShowString(64,4,t2, 0);
//            yeniPuan_o2=FALSE;
//        }
//
//    } else{
//        
//        //P10_Clear_Display(0);
//        //delay_us(500);
//        P10_DisplayShowString(0,4,t1, 0);
//        P10_DisplayShowString(64,4,t2, 0);
//    }
     
}

void p10_oyun_bitti(unsigned int16 o1Puan,unsigned int16 o2Puan,unsigned int16 oyunSure){

    //P10_Clear_Display(0);
    //P10_Currentfonts=Font8x8;
    //P10_Currentfonts=Font11x12;
    hc_595_clr_allPins();
    mp3_stop();
    dfPplayerMaxSes();
    mp3_disbleLoopAll();
    mp3_play_physical(wav_oyunBitti);
    
    printf(lcd_putc,"\f  OYUN BITTI \n");
    printf(lcd_putc,"%Lu - %Lu - S=%LU",o1Puan,o2Puan,oyunSure);
    
   // textBufSil();
    sprintf(text,"Bitti...");
   // P10_DisplayShowString(1,0,text, 0);

    delay_ms(1000);
  
    //P10_Clear_Display(0);
    if(o1Puan>o2Puan){
         //textBufSil();
         sprintf(text,"Kazanan Oyuncu 1 Puan=%lu ",o1Puan);
         //P10_DisplayScrollString(0,11,0,10,text,0);
    } else if(o1Puan<o2Puan){
         //textBufSil();
         sprintf(text,"Kazanan Oyuncu 2 Puan=%lu ",o2Puan);
         //P10_DisplayScrollString(0,11,0,10,text,0);
    }else{
        //textBufSil();
        sprintf(text,"<<< Berabere Kaldiniz...");
        //P10_DisplayScrollString(0,11,0,10,text,0);
    }
//    P10_Clear_Display(0);
    //textBufSil();
    sprintf(text,"Sure:%Lu saniye  ",oyunSure);
    //P10_DisplayScrollString(0,11,0,10,text,0);
    //P10_Clear_Display(0);
   // textBufSil();
    sprintf(text,"%Lu Sn.",oyunSure);
    //P10_DisplayShowString(1,0,text, 0);
    
    mp3_stop();
    mp3_play_physical(wav_alkisSesi);
    delay_ms(1000);

    mp3_stop();
    mp3_play(mp3_remix);
    
}

int8 p10_kosturAnim(){
      //P10_Clear_Display(0);
    //P10_Currentfonts=Font11x12;
//    textBufSil();
    text="KOSTUR";
    for(int i=0;i<5;i++){
        hc_595_clr_allPins();
        //P10_Clear_Display(1);
        //P10_DisplayShowString(15,0,text, 1);
        hc_595_set_allPins();
        //P10_Clear_Display(0);
        //P10_DisplayShowString(15,0,text, 0);
        jetonKontrol(); if(jetonAtildi) return 0;
    }
}
int8 p10_TL_anim(){
    
      // P10_Clear_Display(0);
    int jtnTLmiktari=read_eeprom(ee_adres_jtnTL);
    if(jtnTLmiktari==255) jtnTLmiktari=1;
   
    //P10_Currentfonts=Font11x12;

    for(int i=0;i<6;i++){
        hc_595_clr_allPins();
        //P10_Clear_Display(1);
        
        //textBufSil();sprintf(text,"%u TL AT ",jtnTLmiktari);
        //P10_DisplayShowString(8,0,text, 1);
        delay_ms(150);
        hc_595_set_allPins();
        //P10_Clear_Display(0);
        //textBufSil();text="Calistir";
        //P10_DisplayShowString(4,0,text, 0);
        delay_ms(150);
        
        jetonKontrol(); if(jetonAtildi) return 0;
    }
     //P10_Clear_Display(0);
     //textBufSil();sprintf(text,"%u TL AT ",jtnTLmiktari);
     
     //P10_DisplayShowString(8,0,text, 0);
     
     int say=0;
     while(1){
         
         jetonKontrol(); if(jetonAtildi) return 0;
         delay_ms(5);
         say++;
         if(say>200) break;
     }
     
}

void P10_baslangic(){

    //P10_Clear_Display(0);     //Ekraný Temizle
    hc_595_clr_allPins();
    //Fontlarýnn büyüklükleri yüksekliði taþýnca Fonttan keserek yazýyor...
  
    //P10_Currentfonts=Font11x12;
    //textBufSil();text="YOYUNCAK";
    //P10_DisplayScrollString(0,11,0,10,text,0);
    //P10_Clear_Display(0);    
//    P10_DisplayShowString(4,0,text, 0);
    delay_ms(500);
   
//    text="KOSTUR";
//        for(i=0;i<5;i++){
//        P10_Clear_Display(0);
//        delay_ms(25);
//        P10_DisplayShowString(15,0,text, 0);
//        delay_ms(25);
//    }
//    for(i=0;i<5;i++){
//        P10_Clear_Display(1);
//        delay_ms(25);
//        P10_DisplayShowString(15,0,text, 1);
//        delay_ms(25);
//    }
    
//    P10_Clear_Display(0);
//    P10_Currentfonts=Font11x12;
//    P10_DisplayShowString(15,0,text, 0);
    //delay_ms(1000);
//    P10_Clear_Display(0);
//    P10_Currentfonts=Font8x8;
//    text="-O1-";
//    P10_DisplayShowString(0,3,text, 0);
//    text="-O2-";
//    P10_DisplayShowString(64,3,text, 0);
//    delay_ms(1000);
//    P10_Clear_Display(0);     //Ekraný Temizle
//    
}

void led_anim_bosta(int animSay){
    
  if(animSay==0){
      rgb_kirmizi_on();
      rgb_yesil_off();
      rgb_mavi_off(); 
  }
  if(animSay==1){
      rgb_yesil_on(); 
      rgb_mavi_off();
      rgb_kirmizi_off();
  }
  if(animSay==2){
      rgb_mavi_on(); 
      rgb_yesil_off();
      rgb_kirmizi_off();
  }
  
  if(animSay==3){
      rgb_beyaz_off();
      o2_L1_on();
  }
  if(animSay==4)o2_L2_on();
  if(animSay==5)o2_L3_on();
  
  if(animSay==6)o1_L1_on();
  if(animSay==7)o1_L2_on();
  if(animSay==8)o1_L3_on();
  
  if(animSay==9){
      
        o1_L1_off();
        o1_L2_off();
        o1_L3_off();
        
        o2_L1_off();
        o2_L2_off();
        o2_L3_off();
        
        rgb_kirmizi_off();
        rgb_yesil_off();
        rgb_mavi_off();

  }
 
}
void ledAnimKarinca(int animSay){
    
  if(animSay==0){
      o2_L1_on();
      rgb_yesil_on();
  }
  if(animSay==1)o2_L1_off();
  
  if(animSay==2)o2_L2_on();
  if(animSay==3){
      o2_L2_off();
      rgb_kirmizi_on();
  }
  
  if(animSay==4)o2_L3_on();
  if(animSay==5)o2_L3_off();
  
  if(animSay==6)o1_L1_on();
  if(animSay==7){
      o1_L1_off();
      rgb_mavi_on();
  }
  
  if(animSay==8)o1_L2_on();
  if(animSay==9)o1_L2_off();
  
  if(animSay==10)o1_L3_on();
  if(animSay==11){
      o1_L3_off();
      rgb_beyaz_off();
  }
    
}
int8 P10_animkosturSirket()
{
    for(int i=0;i<6;i++){
        hc_595_clr_allPins();
       // P10_Clear_Display(1);
       // P10_Currentfonts=Font11x12;
//        textBufSil();
//        text="AK";
//        P10_DisplayShowString(4,0,text, 1);
        delay_ms(150);
        hc_595_set_allPins();
       // P10_Clear_Display(0);
       // textBufSil();
        text="YOYUNCAK";
       // P10_DisplayShowString(4,0,text, 1);
        delay_ms(150);
//        P10_Currentfonts=Font8x8;
//        text="Eskisehir";
//        P10_DisplayShowString(12,4,text, 0);
       // delay_ms(100);
        jetonKontrol(); if(jetonAtildi) return 0;
    }
     hc_595_clr_allPins();
//     P10_Currentfonts=Font11x12;
//     text="YOYUNCAK";;
//     P10_DisplayShowString(4,0,text, 0);
}
int8 P10_animMaxPuanMinSure( unsigned int16 puan, unsigned int16 sure)
{
    if(sure==0xFFFF) sure=0;
    //P10_Clear_Display(0);
    hc_595_clr_allPins();
    //P10_Currentfonts=Font8x8;
    //P10_Currentfonts=Font11x12;
    
    //textBufSil();sprintf(text,"Rekor: %Lu saniye ",sure);
    //P10_DisplayScrollString(0,11,0,10,text,0);
    //P10_Clear_Display(0); 
    //textBufSil();sprintf(text,"%Lu Sn.",sure);
    //P10_DisplayShowString(1,0,text, 0);
    jetonKontrol(); if(jetonAtildi) return 0;
     int say=0;
     while(1){
         jetonKontrol(); if(jetonAtildi) return 0;
         delay_ms(10);
         say++;
         if(say>200) break;
     }
}

