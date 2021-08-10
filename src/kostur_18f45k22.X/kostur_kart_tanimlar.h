/* 
 * File:   kostur_kart_tanimlar.h
 * Author: ilyas
 *
 * Created on 09 Mart 2018 Cuma, 15:36
 */

#ifndef KOSTUR_KART_TANIMLAR_H
#define	KOSTUR_KART_TANIMLAR_H

#ifdef	__cplusplus
extern "C" {
#endif
    //unsigned int16 spiData=0;  //16 bit veri gönderileceði zaman
    unsigned int8 spiDataH=0,spiDataM=0,spiDataL=0;
    int8 jetonSay=0;
    void hc595_spi_yaz(unsigned int16 veri,unsigned char yaz_bit);
    void hc595_spi_24bit_yaz(unsigned int8 veriH,unsigned int8 veriM,unsigned int8 veriL);
    
    #define spi_yaz16() hc595_spi_yaz(spiData,16)
    #define spi_yaz24() hc595_spi_24bit_yaz(spiDataH,spiDataM,spiDataL)
    #define hc_595_clr_allPins() hc595_spi_24bit_yaz(0x00,0x00,0x00);//delay_ms(100)
    #define hc_595_set_allPins() hc595_spi_24bit_yaz(0xFF,0xFF,0xFF);delay_ms(100)
    
    // spiDataL  ilk 8bit
    #define l2_1    0
    #define l2_2    1
    #define l2_3    2
    #define l1_1    3
    #define l1_2    4
    #define l1_3    5
    #define led_k   6
    #define led_y   7
    // spiDataM ikinci 8 bit
    #define led_m   0
    #define c2      1
    #define b2      2
    #define a2      3
    #define c1      4
    #define b1      5
    #define a1      6
    #define u_led   7
    // spiDataH üçüncü 8 bit
    #define in1     0
    #define in2     1
    #define in3     2
    #define in4     3
    #define in5     4
    #define in6     5
    #define in7     6
    #define in8     7

    //74HC4051_1 tanýmlamalarý

    #define an_sw1_1_sec() bit_clear(spiDataM,a1); bit_clear(spiDataM,b1);bit_clear(spiDataM,c1);spi_yaz24()                    
    #define an_sw1_2_sec() bit_set(spiDataM,a1); bit_clear(spiDataM,b1);bit_clear(spiDataM,c1);spi_yaz24()                 
    #define an_sw1_3_sec() bit_clear(spiDataM,a1); bit_set(spiDataM,b1);bit_clear(spiDataM,c1); spi_yaz24()     
    
    #define an_sw2_1_sec() bit_set(spiDataM,a1); bit_set(spiDataM,b1);bit_clear(spiDataM,c1);spi_yaz24()                    
    #define an_sw2_2_sec() bit_clear(spiDataM,a1); bit_clear(spiDataM,b1);bit_set(spiDataM,c1);spi_yaz24()                 
    #define an_sw2_3_sec() bit_set(spiDataM,a1); bit_clear(spiDataM,b1);bit_set(spiDataM,c1); spi_yaz24()  
                                                       
    #define an_dFSesSec() bit_clear(spiDataM,a1);bit_set(spiDataM,b1);bit_set(spiDataM,c1);spi_yaz24() 
    
                        
    #define dfSesAyarla() an_dFSesSec();set_adc_channel(0); delay_us(20); \
                           mp3_set_volume(map(read_adc(),0,1023,0,30))     \

    #define an_sa_m1_1_sec() bit_clear(spiDataM,a2); bit_clear(spiDataM,b2);bit_clear(spiDataM,c2);spi_yaz24() 
    #define an_sa_m1_2_sec() bit_set(spiDataM,a2); bit_clear(spiDataM,b2);bit_clear(spiDataM,c2);spi_yaz24() 
    #define an_sa_m2_1_sec() bit_clear(spiDataM,a2); bit_set(spiDataM,b2);bit_clear(spiDataM,c2);spi_yaz24() 
    #define an_sa_m2_2_sec() bit_set(spiDataM,a2); bit_set(spiDataM,b2);bit_clear(spiDataM,c2);spi_yaz24()
    #define an_sa_m3_1_sec() bit_clear(spiDataM,a2); bit_clear(spiDataM,b2);bit_set(spiDataM,c2);spi_yaz24() 
    #define an_sa_m3_2_sec() bit_set(spiDataM,a2); bit_clear(spiDataM,b2);bit_set(spiDataM,c2);spi_yaz24() 

    #define o1_L1_on()  bit_set(spiDataL,l1_1);spi_yaz24()
    #define o1_L2_on()  bit_set(spiDataL,l1_2);spi_yaz24()
    #define o1_L3_on()  bit_set(spiDataL,l1_3);spi_yaz24()
    #define o1_L123_on() o1_L1_on();o1_L2_on();o1_L3_on()

    #define o1_L1_off()  bit_clear(spiDataL,l1_1);spi_yaz24()
    #define o1_L2_off()  bit_clear(spiDataL,l1_2);spi_yaz24()
    #define o1_L3_off()  bit_clear(spiDataL,l1_3);spi_yaz24()
    #define o1_L123_off()  o1_L1_off();o1_L2_off();o1_L3_off()

    #define o2_L1_on()  bit_set(spiDataL,l2_1);spi_yaz24()
    #define o2_L2_on()  bit_set(spiDataL,l2_2);spi_yaz24()
    #define o2_L3_on()  bit_set(spiDataL,l2_3);spi_yaz24()
     #define o2_L123_on() o2_L1_on();o2_L2_on();o2_L3_on()

    #define o2_L1_off()  bit_clear(spiDataL,l2_1);spi_yaz24()
    #define o2_L2_off()  bit_clear(spiDataL,l2_2);spi_yaz24()
    #define o2_L3_off()  bit_clear(spiDataL,l2_3);spi_yaz24()
    #define o2_L123_off()  o2_L1_off();o2_L2_off();o1_L3_off()

    #define sw_led_off() o1_L123_off(); o2_L123_off()
    #define sw_led_on() o1_L123_on();o2_L123_on()
    
    #define rgb_kirmizi_on()  bit_set(spiDataL,led_k);spi_yaz24()
    #define rgb_kirmizi_off() bit_clear(spiDataL,led_k);spi_yaz24()
    
    #define rgb_yesil_on() bit_set(spiDataL,led_y);spi_yaz24()
    #define rgb_yesil_off() bit_clear(spiDataL,led_y);spi_yaz24()
    
    #define rgb_mavi_on() bit_set(spiDataM,led_m);spi_yaz24()
    #define rgb_mavi_off() bit_clear(spiDataM,led_m);spi_yaz24()

    //#define rgb_beyaz_on() rgb_kirmizi_on();rgb_yesil_on();rgb_mavi_on()
    #define rgb_beyaz_on()   bit_set(spiDataL,led_k); bit_set(spiDataL,led_y);  \
                             bit_set(spiDataM,led_m); spi_yaz24()              \

    //#define rgb_beyaz_off() rgb_kirmizi_off();rgb_yesil_off();rgb_mavi_off()
    #define rgb_beyaz_off() bit_clear(spiDataL,led_k);bit_clear(spiDataL,led_y);    \
                            bit_clear(spiDataM,led_m);spi_yaz24()                   \

    #define motor1_ileri() bit_set(spiDataH,in1);    \
                           bit_clear(spiDataH,in2);  \
                           spi_yaz24()              \                      
                           //output_high(PIN_C2)      \
                          // Rc2 : ccp1                      
    #define motor1_geri()  bit_set(spiDataH,in2);    \
                           bit_clear(spiDataH,in1);  \
                           spi_yaz24()              \                      
                          // output_high(PIN_C2)    \       //PWM yoksa 
    #define motor1_dur() bit_clear(spiDataH,in1);    \
                         bit_clear(spiDataH,in2);    \                         
                         spi_yaz24()                \
                                 
    #define motor2_ileri() bit_set(spiDataH,in3);    \
                           bit_clear(spiDataH,in4);  \
                           spi_yaz24()              \                       
                          // output_high(PIN_C11)    \       //PWM yoksa 
                          // Rc1 : ccp2
    #define motor2_geri()  bit_set(spiDataH,in4);    \
                           bit_clear(spiDataH,in3);  \
                           spi_yaz24()              \
                                   
    #define motor2_dur()   bit_clear(spiDataH,in3);    \
                           bit_clear(spiDataH,in4);  \
                           spi_yaz24()               \         
                           //output_high(PIN_C1)    \       //PWM yoksa   
    #define motor3_ileri() bit_set(spiDataH,in5);    \
                           bit_clear(spiDataH,in6);  \
                           spi_yaz24()               \      //En pin +5V'ta donanýmda  
                                  
    #define motor3_geri()  bit_set(spiDataH,in6);    \
                           bit_clear(spiDataH,in5);  \
                           spi_yaz24()              \
                                 
    #define motor3_dur()   bit_clear(spiDataH,in5);  \
                           bit_clear(spiDataH,in6);  \
                           spi_yaz24()               \
                           
    #define oyunbittiM12dur_doBrk() if(sa_m1_2()||sa_m2_2()||f_oyunBitti)\
                                    {                                   \
                                        motor1_dur();                   \
                                        motor2_dur();                   \
                                        f_oyunBitti=TRUE;               \ 
                                        break;                          \
                                    }                                   \
                           

    
    #define jtn_pin PIN_B0                  //Pin ler  büyük harfle olmalý
    #define ticket_out_pin PIN_C0
     //delay_ms(300);;                              
    #define jetonKontrol() if(!input(jtn_pin)){                         \
                                                                        \
                            jetonSay++;                                 \
                           }                                            \
                           if(jetonSay>=jtnTLMik){                      \
                                jetonAtildi=TRUE;                       \
                                jetonSay=0;                             \
                           }                                            \
    
    #define btn1 PIN_A5
    #define btn2 PIN_A3                       
    #define btn3 PIN_A2

   /// wav dosyasý tanýmlamlarý
    /// mp3-wav index
	#define wav_atKisnemesi     1
	#define wav_atKosmasi       2
	#define wav_suDamlasi       3
	#define wav_heySesi         4
	#define wav_alkisSesi       5
	#define wav_oyunBitti       6
	#define wav_getReady        7
	#define wav_whoosh          8
	#define wav_jeton           9
	#define mp3_remix           10
	#define mp3_tavsan          11
	#define wav_waterDrop       12
	#define mp3_adele_Rolling   14
	#define wav_duduk_basla     15
	#define wav_ding            16
	#define mp3_mars            17
	#define mp3_getlowve_araba	18
    #define mp3_beep_uyari      19

     //// bosta rastgele mp3 müzik
    
    #define mp3_adele_Rolling2  20
    #define mp3_anitta          21
    #define mp3_love_lo         22
    #define mp3_agen            23
    #define mp3_dejavu          24
    #define mp3_rain            25
    #define mp3_aweli           26

    #define wav_remix_sure 200000 //(3*60+20)*1000  //3:20 mS degeri....
    #define wav_atKisnemesi_sure  1500
    #define wav_atKosmasi_sure    2000
    #define wav_whoosh_sure  20 //300//350

    #define oyunSure_1dk    (unsigned int32)(60*1250)//60000UL
    #define oyunSure_3dk    (unsigned int32)(180*1250)//180000UL
    #define oyunSure_5dk    (unsigned int32)(300*1250)//300000UL
    #define oyunSure_7dk    (unsigned int32)(420*1250)//420000UL
    #define oyunSure_10dk   (unsigned int32)(600*1250)//600000UL

    #define millisMaxSayi 0xFFFFFFFF

    #define birliDelikPuan 30
    #define ikiliDelikPuan 20
    #define ucluDelikPuan  10

    #define dfPplayerMaxSes() mp3_set_volume (29);delay_ms(100)//48
    #define dfPlayerNormSes() mp3_set_volume(20);delay_ms(100)
    #define dfPlayerMinSes() mp3_set_volume(1);delay_ms(100)

    #define textBufSil() text_sil()

    #define ee_adres_jtnCount   0   //LSB Byte, high 3
    #define ee_adres_jtnTL      4
    #define ee_adres_Msure_1    5   //LSB Byte, high 6
    #define ee_adres_Msure_2    7   //LSB Byte, high 8
    #define ee_adres_Msure_3    9   //LSB Byte, high 10
    #define ee_adres_oyunSure   11
    #define ee_adres_dfSes      12


    #define ee_adres_sw11_an_min_0_h      13
    #define ee_adres_sw11_an_min_0_l      14
    #define ee_adres_sw12_an_min_1_h      15
    #define ee_adres_sw12_an_min_1_l      16
    #define ee_adres_sw13_an_min_2_h      17
    #define ee_adres_sw13_an_min_2_l      18
    #define ee_adres_sw21_an_min_3_h      19
    #define ee_adres_sw21_an_min_3_l      20
    #define ee_adres_sw22_an_min_4_h      21
    #define ee_adres_sw22_an_min_4_l      22
    #define ee_adres_sw23_an_min_5_h      23
    #define ee_adres_sw23_an_min_5_l      24

    #define ee_adres_sw11_an_max_0_h      25
    #define ee_adres_sw11_an_max_0_l      26
    #define ee_adres_sw12_an_max_1_h      27
    #define ee_adres_sw12_an_max_1_l      28
    #define ee_adres_sw13_an_max_2_h      29
    #define ee_adres_sw13_an_max_2_l      30
    #define ee_adres_sw21_an_max_3_h      31
    #define ee_adres_sw21_an_max_3_l      32
    #define ee_adres_sw22_an_max_4_h      33
    #define ee_adres_sw22_an_max_4_l      34
    #define ee_adres_sw23_an_max_5_h      35
    #define ee_adres_sw23_an_max_5_l      36

    #define ee_adr_an_max_kal_h 37
    #define ee_adr_an_max_kal_l 38
    #define ee_adr_an_min_kal_h 39
    #define ee_adr_an_min_kal_l 40

    #define ee_adr_sens_okuma_sayisi    41



    #define birDelikAdimSys   5

    #define SENS_OKU_L    'L'     // Sensor normade low
    #define SENS_OKU_H    'H'     // Sensor normade High 

   // #define SENS_OKU_H_L    SENS_OKU_H  
    #define SENS_OKU_H_L    SENS_OKU_L
    

    #define lcd_led PIN_B2

    //  ****    ////
#ifdef	__cplusplus
}
#endif

#endif	/* KOSTUR_KART_TANIMLAR_H */

