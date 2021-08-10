
//       _____
// PWM__|     \
// OE___| AND  }---- Disp_Enable 
//      |_____/      
//
//  P10 Led Board
//      1     2
// OE  --|oo|-- A
// GND --|oo|-- B
// GND --|oo|-- 
// GND --|oo|-- SCK
// GND --|oo|-- LATCH
// GND --|oo|-- DATA
// GND --|oo|--
// GND --|oo|--
//     15    16 

#define P10_Column_Num 3//3//1         yatay seri panel sayısı
#define P10_Line_Num 1
#define P10_Max_Width 32*P10_Column_Num
#define P10_Max_Height 16*P10_Line_Num
#define P10_Adress_Count 16*P10_Column_Num*P10_Line_Num
#define P10_Line_SR_Number P10_Max_Width/8      //toplam panellerdeki Shfit Reg (SR)sayısından 4 eksik

#define PinA     pin_b3 //pin_c0
#define PinB     pin_b4
#define PinOE    pin_b5     //pin_c4                    //OE=0 Display OFF, OE=1 Display ON 
#define PinPWM   pin_b3     //CCP2 
#define LATCH    pin_c4 
//18F4550   ;       SDO --> RC7     SCK --> RB1

extern unsigned int8  display_ram[P10_Adress_Count][4];

void write_data(unsigned int8 row_adr);
void P10_Set_Brightness(int8 value);
void P10_Clear_Display(unsigned int1 fill);
void P10_PutPixel(unsigned int8 Xpos,unsigned int8 Ypos, unsigned int8 color);
unsigned int8  P10_GetPixel(unsigned int8 Xpos,unsigned int8 Ypos);
unsigned int8  LineScroll(unsigned int8 Line, unsigned int8 clstart, unsigned int8 clend, unsigned int8 ZeroBit);
void P10_DisplayScrollString(unsigned int8 clstart, unsigned int8 clend, unsigned int8  ypos, unsigned int8 speed, char* textptr, unsigned int8 color);
void P10_DisplayShowString(unsigned int8 Xpos, unsigned int8 Ypos, char* textptr, unsigned int8 color);
void P10_DisplayChar(unsigned int8 Xpos, unsigned int8 Ypos, char ascii, unsigned int8 color);
void P10_DisplayChar(unsigned int8 Xpos, unsigned int8 Ypos, char ascii, unsigned int8 color);
void P10_DrawChar(unsigned int8  Xpos, unsigned int8  Ypos, rom unsigned int8  *saddress, unsigned int8  color);
void P10_DrawLine(unsigned int8 x1, unsigned int8 y1, unsigned int8 x2, unsigned int8 y2, unsigned int8 color);
void P10_DrawRect(unsigned int8 x1, unsigned int8 y1, unsigned int8 x2, unsigned int8 y2, unsigned int8 fill, unsigned int8 color);
void P10_DrawCircle(unsigned int8 x, unsigned int8 y, unsigned int8 size, unsigned int8 fill, unsigned int8 color);
void textBuf_sil(void);
