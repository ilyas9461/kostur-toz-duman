/* 
 * File:   kostur_anim.h
 * Author: ilyas
 *
 * Created on 21 Nisan 2018 Cumartesi, 08:31
 */

#ifndef KOSTUR_ANIM_H
#define	KOSTUR_ANIM_H

#ifdef	__cplusplus
extern "C" {
#endif

    #define led_animKarasimsek  0
    #define led_animKarinca     1
    #define led_animBlink       2
    #define led_animVumetre     3

    #define P10_animKosturBlink 4
    #define P10_SirketReklam    5
    #define P10_animTL          6
    #define P10_animMaxSurePuan 7
    

    void led_anim_bosta(int animSay);
    void p10_oyun_puan_goster(unsigned int16 o1Puan,unsigned int16 o2Puan);



#ifdef	__cplusplus
}
#endif

#endif	/* KOSTUR_ANIM_H */

