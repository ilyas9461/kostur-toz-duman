#include "kostur_kart_tanimlar.h"
#include "dfPlayer.h"
#include "main_kostur.h"


unsigned int32 atKisnemesi_ilkSure=0;
unsigned int32 atKosmasi_ilkSure=0;
unsigned int32 motorSureBasla=0;
unsigned int32 whooshSureBasla=0;

//* 60 RPM motor kullanýlýyor…

unsigned int16 m1_birliDelik_sure=2400;     //60rpm motor ve 1 delik için 5 atýþ
unsigned int16 m1_ikiliDelik_sure=1600;
unsigned int16 m1_ucluDelik_sure=800;

unsigned int16 m2_birliDelik_sure=2400;
unsigned int16 m2_ikiliDelik_sure=1600;
unsigned int16 m2_ucluDelik_sure=800;

unsigned int16 o1_puan=0;
unsigned int16 o2_puan=0;
unsigned int1 yeniPuan_o1=FALSE;
unsigned int1 yeniPuan_o2=FALSE;

int1 f_oyunBitti=FALSE;
int1 f_kisneme=0;
unsigned int32 oyunBitisSureBasla=0;    // Ayarlanan oyun bitiþ süresi için
int ee_oyunSure=5;

#define sw_11 11
#define sw_12 12
#define sw_13 13

#define sw_21 21
#define sw_22 22
#define sw_23 23

#define sa_11 31 
#define sa_12 32

#define sa_21 33 
#define sa_22 34

#define sa_31 35 
#define sa_32 36

////******//////

//#define adcBekle 20

//#define an_esik 512       //2.5 V
//#define an_esik 400//409       // 2V
//#define an_esik   358       //1.75V

 #define sa_an_esik 307       // 1.5V  //motorlar sýnýr anahtarlarý için belirlendi...

//#define an_esik 256         // 1.25 V
// #define an_esik 204       // 1 V

//200:0,976 V  ve 400:1,953 V   410:2,001 

//#define aralik_deger 75    //20*(4.882mv)=97mV
// 75:366 mV   82:400 mV
unsigned long deger=0;
unsigned long an_min[6];   
unsigned long ee_an_min=204;
unsigned long an_max[6];   //0:sw1_1   5:sw2_3
unsigned long _an_max[6];
unsigned long an_esik=307;//1,5V

unsigned long an_max_ort=0;

#define ARTTI 1
#define AZALDI 0
#define AYNI  0x
#define ADC_CHNL_SURE   40      //20
#define ESIK_MV 75  //50*4,889=~250mV
#define ADC_MAX_OLC 16
#define ADC_SIHFT_R 4       //ortalama ama iþlemi için shift right
#define ESIK_RST 0
#define DEGER_RST 0xffff

unsigned long an_max_arttti_azaldi[6];
unsigned long an_min_arttti_azaldi[6];


void ee_an_max_yaz(void){
    
    write_eeprom(ee_adres_sw11_an_max_0_h, make8(an_max[0],1));     //high deger
    write_eeprom(ee_adres_sw11_an_max_0_l, make8(an_max[0],0));     //low deger
    
    write_eeprom(ee_adres_sw12_an_max_1_h, make8(an_max[1],1));     //high deger
    write_eeprom(ee_adres_sw12_an_max_1_l, make8(an_max[1],0));     //low deger
    
    write_eeprom(ee_adres_sw13_an_max_2_h, make8(an_max[2],1));     //high deger
    write_eeprom(ee_adres_sw13_an_max_2_l, make8(an_max[2],0));     //low deger
    
    write_eeprom(ee_adres_sw21_an_max_3_h, make8(an_max[3],1));     //high deger
    write_eeprom(ee_adres_sw21_an_max_3_l, make8(an_max[3],0));     //low deger
    
    write_eeprom(ee_adres_sw22_an_max_4_h, make8(an_max[4],1));     //high deger
    write_eeprom(ee_adres_sw22_an_max_4_l, make8(an_max[4],0));     //low deger
    
    write_eeprom(ee_adres_sw23_an_max_5_h, make8(an_max[5],1));     //high deger
    write_eeprom(ee_adres_sw23_an_max_5_l, make8(an_max[5],0));     //low deger
}

void ee_an_min_yaz(void){
    write_eeprom(ee_adres_sw11_an_min_0_h, make8(an_min[0],1));     //high deger
    write_eeprom(ee_adres_sw11_an_min_0_l, make8(an_min[0],0));     //low deger
    
    write_eeprom(ee_adres_sw12_an_min_1_h, make8(an_min[1],1));     //high deger
    write_eeprom(ee_adres_sw12_an_min_1_l, make8(an_min[1],0));     //low deger
    
    write_eeprom(ee_adres_sw13_an_min_2_h, make8(an_min[2],1));     //high deger
    write_eeprom(ee_adres_sw13_an_min_2_l, make8(an_min[2],0));     //low deger
    
    write_eeprom(ee_adres_sw21_an_min_3_h, make8(an_min[3],1));     //high deger
    write_eeprom(ee_adres_sw21_an_min_3_l, make8(an_min[3],0));     //low deger
    
    write_eeprom(ee_adres_sw22_an_min_4_h, make8(an_min[4],1));     //high deger
    write_eeprom(ee_adres_sw22_an_min_4_l, make8(an_min[4],0));     //low deger
    
    write_eeprom(ee_adres_sw23_an_min_5_h, make8(an_min[5],1));     //high deger
    write_eeprom(ee_adres_sw23_an_min_5_l, make8(an_min[5],0));     //low deger
}


void an_max_min_reset(){
    int x=0;

    for(x=0;x<6;x++){
        an_min[x]=sens_min_kal;      // 1V  205
        an_max[x]=sens_max_kal;      // 2V  410
    }
}

BOOLEAN ee_sens_min_kontrol(){
    int i;
    
    for (i = 13; i < 25; i++) {
        if(read_eeprom(i)!=0xFF) return TRUE;// sens min-max degeri varsa
   }
    return FALSE;
        
}

void ee_sens_kalibre_oku(){
    
    int i,sens=0;

    for (i = 13; i < 25; i+=2) {
        ee_an_min=read_eeprom(i);   //high deger
        
        if(ee_an_min!=0xFF){
            an_min[sens]=make16(read_eeprom(i),read_eeprom(i+1));
        }else if(ee_an_min==0xFF){
            an_min[sens]=204;   //reset deger.
        }
         sens++;
    }
  
    printf(lcd_putc,"\fmin:%Lu,%Lu,%Lu",an_min[0],an_min[1],an_min[2]);
    printf(lcd_putc,"\n%Lu,%Lu,%Lu",an_min[3],an_min[4],an_min[5]);
    
    delay_ms(1000);
}

void an_max_adc_oku(){

    int i;
    for (i = 0; i < 16; i++) {
        
        an_sw1_1_sec();
        set_adc_channel(0); 
        delay_us(ADC_CHNL_SURE);
        _an_max[0]+=read_adc();

        an_sw1_2_sec();
        set_adc_channel(0); 
        delay_us(ADC_CHNL_SURE);
        _an_max[1]+=read_adc();

        an_sw1_3_sec();
        set_adc_channel(0); 
        delay_us(ADC_CHNL_SURE);
        _an_max[2]+=read_adc();

        an_sw2_1_sec();
        set_adc_channel(0); 
        delay_us(ADC_CHNL_SURE);
        _an_max[3]+=read_adc();

        an_sw2_2_sec();
        set_adc_channel(0); 
        delay_us(ADC_CHNL_SURE);
        _an_max[4]+=read_adc();

        an_sw2_3_sec();
        set_adc_channel(0); 
        delay_us(ADC_CHNL_SURE);
        _an_max[5]+=read_adc();
    }
    
    _an_max[0]=_an_max[0]>>4; _an_max[1]=_an_max[1]>>4; _an_max[2]=_an_max[2]>>4;  //16 ya bölme iþlemi ortalama alýnýyor
    _an_max[3]=_an_max[3]>>4; _an_max[4]=_an_max[4]>>4; _an_max[5]=_an_max[5]>>4;
    an_max_ort=0;
    for(i=0;i<6;i++){
//         if(_an_max[i]<an_max[i]) an_max_arttti_azaldi[i]=AZALDI;
//         if(_an_max[i]>an_max[i]) an_max_arttti_azaldi[i]=ARTTI;
//         if(_an_max[i]==an_max[i]) an_max_arttti_azaldi[i]=AYNI;
         
         if(_an_max[i]<512) //&& _an_max>225)// 1.1V> an <2.5V
         {
             an_max[i]=_an_max[i];
         }
     
         if(_an_max[i]<250) an_max[i]=409 ;  //250:1.2V   409:2V
         an_max_ort+=an_max[i];
    }
    an_max_ort=an_max_ort/6;
    
    //ee_an_max_yaz();
    
    printf(lcd_putc,"\fmax:%Lu,%Lu,%Lu",an_max[0],an_max[1],an_max[2]);
    printf(lcd_putc,"\n%Lu,%Lu,%Lu:%Lu",an_max[3],an_max[4],an_max[5],an_max_ort);
    delay_ms(2000);
}


int sw1_1(char h_l){
    int i;
    unsigned int16 deger_11=DEGER_RST;
    unsigned int16 an_esik_11=ESIK_RST;
    //an_esik=ESIK_RST;
    //deger=DEGER_RST;
    for (i = 0; i < ADC_MAX_OLC; i++) {
        an_sw1_1_sec();         //yaklaþýk 50 us 
        set_adc_channel(0); 
        delay_us(ADC_CHNL_SURE); //40 us

        deger_11+=read_adc();
    }
   
    deger_11=(deger_11>>ADC_SIHFT_R);  //ortalama alma iþlemi
    an_esik_11=(unsigned long)(an_max[0]-((an_max[0]-an_min[0])/2));
    //an_esik=an_max[0]-ESIK_MV;
    
    if(h_l==SENS_OKU_H_L){
        //  0 algýlar: düþen kenar 1-->0
        if( deger_11>=an_esik_11){        //eðer top yoksa çýkýþ gerilimi yüksek   1
            return 0;
        } 
        if( deger_11<an_esik_11){         // top varsa çýkýþ gerilimi düþtü...   0
            
//            if((deger<an_min[0] && an_max_arttti_azaldi[0]==AZALDI)   // deger de azalma var, max degerde azaldý ise yeni deger var kaydet...
//                 ||(deger>an_min[0] && an_max_arttti_azaldi[0]==ARTTI))//deger de artma var,max degerde arttý ise yeni deger oluþtu kaydet... 
//            {
//               an_min[0]=deger;
//               
//               write_eeprom(ee_adres_sw11_an_min_0_h, make8(an_min[0],1));     //high deger
//               write_eeprom(ee_adres_sw11_an_min_0_l, make8(an_min[0],0));     //low deger   
//            }
            last_adc_deger=deger_11;
            return 1;
        }
    }else{
//        //  1 algýlar: yükselen kenar 0-->1
//        if( deger<an_esik){        //eðer top yoksa çýkýþ gerilimi düþük  0
//            return 0;
//        } 
//        if( deger>=an_esik){         // top varsa çýkýþ gerilimi yüksek  1
//            
//           // bellek yetersiz kaldý gerektiðinde kodlanacak..... 
//            return 1;
//        }
    }

    return 0;
}

int sw1_2(char h_l){
    int i;
    unsigned int16 deger_12=DEGER_RST;
    unsigned int16 an_esik_12=ESIK_RST;
    //an_esik=ESIK_RST;
    //deger=DEGER_RST;
    for (i = 0; i < ADC_MAX_OLC; i++) {
        an_sw1_2_sec();
        set_adc_channel(0); 
        delay_us(ADC_CHNL_SURE);

        deger_12+=read_adc();
    }
   
    deger_12=(deger_12>>ADC_SIHFT_R);
    an_esik_12=(unsigned long)(an_max[1]-((an_max[1]-an_min[1])/2));
    //an_esik=an_max[1]-ESIK_MV;
    
    if(h_l==SENS_OKU_H_L){
        //  0 algýlar: düþen kenar 1-->0
        if( deger_12>=an_esik_12){        //eðer top yoksa çýkýþ gerilimi yüksek   1
            return 0;
        } 
        if( deger_12<an_esik_12){         // top varsa çýkýþ gerilimi düþtü...   0
            
//           if((deger<an_min[1] && an_max_arttti_azaldi[1]==AZALDI)   // deger de azalma var, max degerde azaldý ise yeni deger var kaydet...
//                 ||(deger>an_min[1] && an_max_arttti_azaldi[1]==ARTTI))//deger de artma var,max degerde arttý ise yeni deger oluþtu kaydet... 
//            {
//               an_min[1]=deger;
//               write_eeprom(ee_adres_sw12_an_min_1_h, make8(an_min[1],1));     //high deger
//               write_eeprom(ee_adres_sw12_an_min_1_l, make8(an_min[1],0));     //low deger   
//            }
            last_adc_deger=deger_12;
            return 1;
        }
    }else{
//        //  1 algýlar: yükselen kenar 0-->1
//        if( deger<an_esik){        //eðer top yoksa çýkýþ gerilimi düþük  0
//            return 0;
//        } 
//        if( deger>=an_esik){         // top varsa çýkýþ gerilimi yüksek  1
//            return 1;
//        }
    }

    return 0;
}

int sw1_3(char h_l){
    int i;
    unsigned int16 deger_13=DEGER_RST;
    unsigned int16 an_esik_13=ESIK_RST;
    //an_esik=ESIK_RST;
   // deger=DEGER_RST;
    for (i = 0; i < ADC_MAX_OLC; i++) {
        an_sw1_3_sec();
        set_adc_channel(0); 
        delay_us(ADC_CHNL_SURE);

        deger_13+=read_adc();
    }
    deger_13=(deger_13>>ADC_SIHFT_R);
    an_esik_13=(unsigned long)(an_max[2]-((an_max[2]-an_min[2])/2));
    //an_esik=an_max[2]-ESIK_MV;
    if(h_l==SENS_OKU_H_L){
        //  0 algýlar: düþen kenar 1-->0
        if( deger_13>=an_esik_13){        //eðer top yoksa çýkýþ gerilimi yüksek   1
            return 0;
        } 
        if( deger_13<an_esik_13){         // top varsa çýkýþ gerilimi düþtü...   0
            
//            if((deger<an_min[2] && an_max_arttti_azaldi[2]==AZALDI)   // deger de azalma var, max degerde azaldý ise yeni deger var kaydet...
//                 ||(deger>an_min[2] && an_max_arttti_azaldi[2]==ARTTI))//deger de artma var,max degerde arttý ise yeni deger oluþtu kaydet... 
//            {
//               //an_min[2]=deger;
//               write_eeprom(ee_adres_sw13_an_min_2_h, make8(an_min[2],1));     //high deger
//               write_eeprom(ee_adres_sw13_an_min_2_l, make8(an_min[2],0));     //low deger   
//            }
            last_adc_deger=deger_13;
            return 1;
        }
    }else{
//        //  1 algýlar: yükselen kenar 0-->1
//        if( deger<an_esik){        //eðer top yoksa çýkýþ gerilimi düþük  0
//            return 0;
//        } 
//        if( deger>=an_esik){         // top varsa çýkýþ gerilimi yüksek  1
//            return 1;
//        }
    }
    return 0;
}

int sw2_1(char h_l){
    int i;
    unsigned int16 deger_21=DEGER_RST;
    unsigned int16 an_esik_21=ESIK_RST;
    //an_esik=ESIK_RST;
    //deger=DEGER_RST;
    for (i = 0; i < ADC_MAX_OLC; i++) {
        an_sw2_1_sec();
        set_adc_channel(0); 
        delay_us(ADC_CHNL_SURE);

        deger_21+=read_adc();
    }
    deger_21=(deger_21>>ADC_SIHFT_R);
    an_esik_21=(unsigned long)(an_max[3]-((an_max[3]-an_min[3])/2));
   // an_esik=an_max[3]-ESIK_MV;
    if(h_l==SENS_OKU_H_L){
        //  0 algýlar: düþen kenar 1-->0
        if( deger_21>=an_esik_21){        //eðer top yoksa çýkýþ gerilimi yüksek   1
            return 0;
        } 
        if( deger_21<an_esik_21){         // top varsa çýkýþ gerilimi düþtü...   0
            
//            if((deger<an_min[3] && an_max_arttti_azaldi[3]==AZALDI)   // deger de azalma var, max degerde azaldý ise yeni deger var kaydet...
//                 ||(deger>an_min[3] && an_max_arttti_azaldi[3]==ARTTI))//deger de artma var,max degerde arttý ise yeni deger oluþtu kaydet... 
//            {
//               an_min[3]=deger;
//               write_eeprom(ee_adres_sw21_an_min_3_h, make8(an_min[3],1));     //high deger
//               write_eeprom(ee_adres_sw21_an_min_3_l, make8(an_min[3],0));     //low deger   
//            }
            last_adc_deger=deger_21;
            return 1;
        }
    }else{
//        //  1 algýlar: yükselen kenar 0-->1
//        if( deger<an_esik){        //eðer top yoksa çýkýþ gerilimi düþük  0
//            return 0;
//        } 
//        if( deger>=an_esik){         // top varsa çýkýþ gerilimi yüksek  1
//            return 1;
//        }
    }
    
    return 0;
}

int sw2_2(char h_l){    
    int i;
    unsigned int16 deger_22=DEGER_RST;
    unsigned int16 an_esik_22=ESIK_RST;
    //an_esik=ESIK_RST;
    //deger=DEGER_RST;
    for (i = 0; i < ADC_MAX_OLC; i++) {
        an_sw2_2_sec();
        set_adc_channel(0); 
        delay_us(ADC_CHNL_SURE);

        deger_22+=read_adc();
    }
    deger_22=(deger_22>>ADC_SIHFT_R);
    
    an_esik_22=(unsigned long)(an_max[4]-((an_max[4]-an_min[4])/2));
    //an_esik=an_max[4]-ESIK_MV;
    if(h_l==SENS_OKU_H_L){
        //  0 algýlar: düþen kenar 1-->0
        if( deger_22>=an_esik_22){        //eðer top yoksa çýkýþ gerilimi yüksek   1
            return 0;
        } 
        if( deger_22<an_esik_22){         // top varsa çýkýþ gerilimi düþtü...   0
            
//            if((deger<an_min[4] && an_max_arttti_azaldi[4]==AZALDI)   // deger de azalma var, max degerde azaldý ise yeni deger var kaydet...
//                 ||(deger>an_min[4] && an_max_arttti_azaldi[4]==ARTTI))//deger de artma var,max degerde arttý ise yeni deger oluþtu kaydet... 
//            {
//               an_min[4]=deger;
//               write_eeprom(ee_adres_sw22_an_min_4_h, make8(an_min[4],1));     //high deger
//               write_eeprom(ee_adres_sw22_an_min_4_l, make8(an_min[4],0));     //low deger   
//            }
            last_adc_deger=deger_22;
            return 1;
        }
    }else{
//        //  1 algýlar: yükselen kenar 0-->1
//        if( deger<an_esik){        //eðer top yoksa çýkýþ gerilimi düþük  0
//            return 0;
//        } 
//        if( deger>=an_esik){         // top varsa çýkýþ gerilimi yüksek  1
//            return 1;
//        }
    }

    return 0;
}

int sw2_3(char h_l){
    int i;
    unsigned int16 deger_23=DEGER_RST;
    unsigned int16 an_esik_23=ESIK_RST;
    //an_esik=ESIK_RST;
    //deger=DEGER_RST;
    
    for (i = 0; i < ADC_MAX_OLC; i++) {
        an_sw2_3_sec();
        set_adc_channel(0); 
        delay_us(ADC_CHNL_SURE);

        deger_23+=read_adc();
    }
    deger_23=(deger_23>>ADC_SIHFT_R);
    an_esik_23=(unsigned long)(an_max[5]-((an_max[5]-an_min[5])/2));
    //an_esik=an_max[5]-ESIK_MV;
    if(h_l==SENS_OKU_H_L){
        //  0 algýlar: düþen kenar 1-->0
        if( deger_23>=an_esik_23){        //eðer top yoksa çýkýþ gerilimi yüksek   1
            return 0;
        } 
        if( deger_23<an_esik_23){         // top varsa çýkýþ gerilimi düþtü...   0
            
//            if((deger<an_min[5] && an_max_arttti_azaldi[5]==AZALDI)   // deger de azalma var, max degerde azaldý ise yeni deger var kaydet...
//                 ||(deger>an_min[5] && an_max_arttti_azaldi[5]==ARTTI))//deger de artma var,max degerde arttý ise yeni deger oluþtu kaydet... 
//            {
//               an_min[5]=deger;
//               write_eeprom(ee_adres_sw23_an_min_5_h, make8(an_min[5],1));     //high deger
//               write_eeprom(ee_adres_sw23_an_min_5_l, make8(an_min[5],0));     //low deger   
//            }
            last_adc_deger=deger_23;
            return 1;
        }
    }else{
//        //  1 algýlar: yükselen kenar 0-->1
//        if( deger<an_esik){        //eðer top yoksa çýkýþ gerilimi düþük  0
//            return 0;
//        } 
//        if( deger>=an_esik){         // top varsa çýkýþ gerilimi yüksek  1
//            return 1;
//        }
    }

    return 0;
}


////// MOTORLAR   /////
int sa_m1_1(){
    an_sa_m1_1_sec();
    set_adc_channel(1); 
    delay_us(20);
    deger=read_adc();
    
    //if(read_adc()>=an_esik) return 1;// 1 algýlar
    //else if(read_adc()<an_esik) return 0;
    
    if(deger>=sa_an_esik) return 0;// 0 algýlar
    else if(deger<sa_an_esik) return 1;
}

int sa_m1_2(){
    an_sa_m1_2_sec();
    set_adc_channel(1); 
    delay_us(ADC_CHNL_SURE);
    deger=read_adc();
    
   //if(read_adc()>=an_esik) return 1;// 1 algýlar
    //else if(read_adc()<an_esik) return 0;
    
    if(deger>=sa_an_esik) return 0;// 0 algýlar
    else if(deger<sa_an_esik) return 1;
}

int sa_m2_1(){
    an_sa_m2_1_sec();
    set_adc_channel(1); 
    delay_us(20);
    deger=read_adc();
    
    //if(read_adc()>=an_esik) return 1;// 1 algýlar
    //else if(read_adc()<an_esik) return 0;
    
    if(deger>=sa_an_esik) return 0;// 0 algýlar
    else if(deger<sa_an_esik) return 1;
}

int sa_m2_2(){
    an_sa_m2_2_sec();
    set_adc_channel(1); 
    delay_us(ADC_CHNL_SURE);
    deger=read_adc();

   //if(read_adc()>=an_esik) return 1;// 1 algýlar
    //else if(read_adc()<an_esik) return 0;
    
    if(deger>=sa_an_esik) return 0;// 0 algýlar
    else if(deger<sa_an_esik) return 1;
}

int sa_m3_1(){
    an_sa_m3_1_sec();
    set_adc_channel(1); 
    delay_us(ADC_CHNL_SURE);
    deger=read_adc();
    
    //if(read_adc()>=an_esik) return 1;// 1 algýlar
    //else if(read_adc()<an_esik) return 0;
    
    if(deger>=sa_an_esik) return 0;// 0 algýlar
    else if(deger<sa_an_esik) return 1;
}

int sa_m3_2(){
    an_sa_m3_2_sec();
    set_adc_channel(1); 
    delay_us(ADC_CHNL_SURE);
    deger=read_adc();
    
   //if(read_adc()>=an_esik) return 1;// 1 algýlar
    //else if(read_adc()<an_esik) return 0;
    
    if(deger>=sa_an_esik) return 0;// 0 algýlar
    else if(deger<sa_an_esik) return 1;
}

////  *****    //////

int swSaAlgila(char sensorHL){

    rgb_beyaz_on(); 
    spiDataL=0;
    
    oyunbittiM12dur_doBrk();  
   
    if(sw1_1(sensorHL)){
        //mp3_stop();
       // mp3_play(wav_whoosh);
       // delay_ms(wav_whoosh_sure);
       // delay_m_us(wav_whoosh_sure);
        
        o1_L1_on();
        return sw_11;
    }
    if(sw1_2(sensorHL)){
       // mp3_stop();
        //mp3_play(wav_whoosh);
        //delay_ms(wav_whoosh_sure);
        //delay_m_us(wav_whoosh_sure);
        
        o1_L2_on();
        return sw_12;
    }
    if(sw1_3(sensorHL)){
        //mp3_stop();
        //mp3_play(wav_whoosh);
       // delay_ms(wav_whoosh_sure);
       // delay_m_us(wav_whoosh_sure);
        
        o1_L3_on();
        return sw_13;
    }

    if(sw2_1(sensorHL)){
       // mp3_stop();
        //mp3_play(wav_whoosh);
        //delay_ms(wav_whoosh_sure);
        //delay_m_us(wav_whoosh_sure);
        
        o2_L1_on();
        return sw_21;
    }
    if(sw2_2(sensorHL)){
        //mp3_stop();
        //mp3_play(wav_whoosh);
        //delay_ms(wav_whoosh_sure);
       // delay_m_us(wav_whoosh_sure);
        
        o2_L2_on();
        return sw_22;
    }
     if(sw2_3(sensorHL)){
        //mp3_stop();
       // mp3_play(wav_whoosh);
        //delay_ms(wav_whoosh_sure);
       //delay_m_us(wav_whoosh_sure);
        o2_L3_on();
        
        return sw_23;
     }  

        oyunbittiM12dur_doBrk();
        
//        if(f_oyunBitti||(millis()-oyunBitisSureBasla>((unsigned long long)(_mul(_mul(ee_oyunSure,60LL),1000LL))))
//                ||(sa_m1_2())||(sa_m2_2()))
//        { 
//            f_oyunBitti=TRUE;  
//            return 0;
//        }
        
    if(sa_m1_2()||sa_m2_2()||f_oyunBitti)
        return 0; 

    if(f_oyunBitti) return 0;
    else return 255;  //sensorden bilgi gelmiyorsa
    
} 

int varSW(char sensorHL){
    
    if(sw1_1(sensorHL)){
        return sw_11;
    }
    if(sw1_2(sensorHL)){
        return sw_12;
    }
    if(sw1_3(sensorHL)){
        return sw_13;
    }
    if(sw2_1(sensorHL)) {
        return sw_21;
    }
    if(sw2_2(sensorHL)){
        return sw_22;
    }
    if(sw2_3(sensorHL)) {
        return sw_23;
    }
    
    return 0; 
}


void puanBelirle(int sw){
    
    switch(sw){
        case sw_11: 
            o1_puan+=birliDelikPuan;        
            break;
        case sw_12:
            o1_puan+=ikiliDelikPuan;
            break;
        case sw_13:
            o1_puan+=ucluDelikPuan;
            break;
        case sw_21:
            o2_puan+=birliDelikPuan;
            break;
        case sw_22:
            o2_puan+=ikiliDelikPuan;
            break;
        case sw_23:
            o2_puan+=ucluDelikPuan;
            break;
    }
}///
///////

void sw_ses_oynat(int sw)
{
    mp3_stop();
    delay_m_us(20);//ses kacirmadi

    if(sw==sw_11 || sw==sw_21) //tekli delik
    {
        #ifdef  KOSTUR
            mp3_play(wav_atKisnemesi);
        #endif
        #ifdef  TOZ_DUMAN
            mp3_play(mp3_tavsan);
        #endif

    }
    if(sw==sw_12 || sw==sw_13 || sw==sw_22 || sw==sw_23)
    {
         #ifdef  KOSTUR
           // mp3_play(wav_atKisnemesi);
            mp3_play(wav_atKosmasi); 
        #endif
        #ifdef  TOZ_DUMAN
           // mp3_play(mp3_tavsan);
            mp3_play(wav_whoosh);
        #endif
       
    }
}

BOOLEAN m1_islem_durum=TRUE;
BOOLEAN m1_islem_ileri=FALSE;
BOOLEAN m2_islem_durum=TRUE;
BOOLEAN m2_islem_ileri=FALSE;


long m_sure_hesapla(int sens_sw)
{
    int _sw=sens_sw;
    
    if(_sw==sw_11){
        o1_L1_on();
        return m1_birliDelik_sure;
    }
     if(_sw==sw_12){
        o1_L2_on();
        return m1_ikiliDelik_sure;
    }
    if(_sw==sw_13){
         o1_L3_on();
        return m1_ucluDelik_sure;
    }
    
    if(_sw==sw_21){
         o2_L1_on();
        return m2_birliDelik_sure;
    }
     if(_sw==sw_22){
        o2_L2_on();
        return m2_ikiliDelik_sure;
    }
     if(_sw==sw_23){
        o2_L3_on();
        return m2_ucluDelik_sure;
    }
}

int sw_motor_oynat(int sens_sw, long sure)
{
    int _sw=sens_sw;

    if(_sw==sw_11 || _sw==sw_12 || _sw==sw_13)
    {
        if(!m1_islem_ileri)  //motor timer sýfýrlanýyor. ilk çalýþma zamaný
        {
            motor1_ileri();
            m1_islem_ileri=TRUE;
            m1_islem_durum=FALSE;
            //m1_sure+=m1_birliDelik_sure;
            timer_m1_say=0; //timer kesmesinde 1ms artarak içinde sayýyor.
        }
       
        if(timer_m1_say> sure && m1_islem_ileri) // motor sure bitti...
        {
            motor1_dur();
            m1_islem_durum=TRUE;
            m1_islem_ileri=FALSE;
           // m1_sure=0;
            return 1;
        }
        
        printf(lcd_putc,"\ftimer:%Lu", timer_m1_say);
        printf(lcd_putc,"\ntM1:%Lu", sure);
 
    }
    
    if(_sw==sw_21 || _sw==sw_22 || _sw==sw_23)
    {
        if(!m2_islem_ileri)  //motor timer sýfýrlanýyor. ilk çalýþma zamaný
        {
            motor2_ileri();
            m2_islem_ileri=TRUE;
            m2_islem_durum=FALSE;
            //m1_sure+=m1_birliDelik_sure;
            timer_m2_say=0;
        }
       
        if(timer_m2_say> sure && m2_islem_ileri) // motor sure bitti...
        {
            motor2_dur();
            m2_islem_durum=TRUE;
            m2_islem_ileri=FALSE;
           // m1_sure=0;
            return 1;
        }
        
        printf(lcd_putc,"\ftimer:%Lu", timer_m2_say);
        printf(lcd_putc,"\ntM2:%Lu", sure);
 
    }
    return 0;
        
    
}

