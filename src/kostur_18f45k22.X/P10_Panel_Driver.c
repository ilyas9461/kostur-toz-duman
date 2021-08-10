/******************************************************************************
*  @file    P10_Panel_Driver.c
*  @brief   P10 LED Panel Kütüphnesi 
******************************************************************************/

typedef struct _tFont
{    
  rom unsigned int8 *table;
  unsigned int8 Width;
  unsigned int8 Height;
  
} sFONT;

extern sFONT P10_Currentfonts;
extern sFONT Font5x7;
extern sFONT Font8x8;
extern sFONT Font11x12;
extern sFONT Font14x16;


//int8 jetonSay=0;

#include "P10_Panel_Driver.h" 
#include "Font5x7.c"
#include "Font8x8.c"
#include "Font11x12.c"
#include "Font14x16.c"

sFONT P10_Currentfonts;



unsigned int8 display_ram[P10_Adress_Count][4];

void write_data(unsigned int8  row_adr)
{
   unsigned int8  i=0,j=0;
   unsigned int8  k=P10_Line_SR_Number-1;
   
   while(k!=255){                                   // Bu Dï¿½ngï¿½ ileriye doï¿½ru sayarsa panel saï¿½dan 
   
     j=(16*P10_Column_Num-1)-k;                     // Sola Doï¿½ru yï¿½klenir.
     
     for (i=0;i<4;i++)                             // Satï¿½rlar dolduruluyor..
     { 
         spi_write(display_ram[j][row_adr]);
         j-=P10_Line_SR_Number;
     }
     k--;
   }
   output_high(LATCH);
   delay_cycles(2);
   output_low(LATCH);
}

/*******************************************************************************
*   Parlaklï¿½k Ayar Fonksiyonu                                                  *
*   Parameter:    value (0-100)                                                *
*   Return:                                                                    *
*******************************************************************************/
void P10_Set_Brightness(int8 value){
   //set_pwm1_duty((int16)value*4);
}

/*******************************************************************************
*   P10_Clear_Display                                                          *
*   Parametreler: fill  0 iken ram bÃ¶lgesine 0xFF atar.                                                        *
*   Return:                                                                    *
*******************************************************************************/
void P10_Clear_Display(unsigned int1  fill){
unsigned int8  i=0,j=0,fdata=0;
    
  if (fill)fdata=0x00; else fdata=0xFF;

  for (i=0;i<4;i++)
  {
    for (j=0;j<P10_Adress_Count;j++)
    {
      display_ram[j][i]=fdata;
    }
  }
}

/*******************************************************************************
*   P10_PutPixel                                                               *
*   Parametreler  Xpos, Ypos, fill                                             *
*   Return:                                                                    *
*******************************************************************************/
void P10_PutPixel(unsigned int8  Xpos,unsigned int8  Ypos, unsigned int8  color)
{
  unsigned int8  y1=0,x1=0;
  unsigned int8  y2=0,x2=0;
  unsigned int8  temp=0;

  y1 = Ypos%4; 
  y2 = Ypos/4;
  
  x1 = Xpos%8;    
  x2 = Xpos/8; 
  
  if(color)  //Nokta Koy
  {
    temp=display_ram[(y2*P10_Line_SR_Number)+x2][y1]; //Ram'deki deï¿½er ï¿½n belleï¿½e alï¿½nï¿½yor.
    display_ram[(y2*P10_Line_SR_Number)+x2][y1] = (temp & (255-(128>>x1))); //x'inci bit 0 yapï¿½lï¿½r

  }
  else     //Nokta Sil
  {
      temp=display_ram[(y2*P10_Line_SR_Number)+x2][y1]; //Ram'deki deï¿½er ï¿½n belleï¿½e alï¿½nï¿½yor.
      display_ram[(y2*P10_Line_SR_Number)+x2][y1] = (temp | (128>>x1)); //x'inci Bit 1 yapï¿½lï¿½r 

  }
}

/*******************************************************************************
*   P10_GetPixel                                                               *
*   Parameter:    Xpos, Ypos                                                   *
*   Return: Pixel value                                                        *
*******************************************************************************/
unsigned int8  P10_GetPixel(unsigned int8  Xpos,unsigned int8  Ypos)
{
  unsigned int8  y1=0,x1=0;
  unsigned int8  y2=0,x2=0;
  unsigned int8  temp=0;
  unsigned int8  value=0;
  
  y1 = Ypos%4;
  y2 = Ypos/4;
  x1 = Xpos%8;    
  x2 = Xpos/8;
  
  temp=display_ram[(y2*P10_Line_SR_Number)+x2][y1]; //Ram'deki deï¿½er ï¿½n belleï¿½e alï¿½nï¿½yor.
  value=1-(0x80 & temp<<x1)/128;   //Ram adresindeki bit geri gï¿½nderiliyor
  return value;
}
/*******************************************************************************
*   Satï¿½r Kaydï¿½rma Fonksiyonu                                                  *
*   Parameter:    Line, clstart, clend, ZeroBit                                *
*   Return:    OverFlow                                                        *
*******************************************************************************/
unsigned int8  LineScroll(unsigned int8  Line, unsigned int8  clstart, unsigned int8  clend, unsigned int8  ZeroBit)
{
  unsigned int8  i=0; 
  unsigned int8  y1=Line%4;  
  unsigned int8  y2=(Line/4)*P10_Line_SR_Number;
  unsigned int8  OverFlow, Carry;
  
  ZeroBit=1-ZeroBit;

  OverFlow=(display_ram[y2+clstart][y1]/128)&0x01; 
  
   for(i=clstart;i<clend+1;i++) 
   {
       if(i<clend) //if(i<11)
       {
         Carry=(display_ram[y2+(i+1)][y1]/128)&0x01;                      //Bir Sonraki Byte'in Tasan Bitini Carry'e yerlestir.
         display_ram[y2+i][y1] = display_ram[y2+i][y1]<<1;                //1 Bit Sola kaydir
         display_ram[y2+i][y1]=(unsigned int8)display_ram[y2+i][y1] | Carry;      //Carry'yi Bir ï¿½nceki Byte'in 0. bitine yerlestir Yerlestir
       }
       else
       {
          display_ram[y2+i][y1] = display_ram[y2+i][y1]<<1;               //1 Bit Sola kaydir
       }
        ////////
     // jetonKontrol(); if(jetonAtildi) return 0;
      ///////
    }
  
   if(ZeroBit){                                                           //Sag Taraftan Fonksiyona gï¿½nderilen bit ekleniyor..
    display_ram[((y2)+clend)][y1]=(int8)display_ram[((y2)+clend)][y1] | 0x01; 
  }else{
    display_ram[((y2)+clend)][y1]=(int8)display_ram[((y2)+clend)][y1] & 0xFE; 
  }
  return OverFlow;  
}

/*******************************************************************************
*   P10_DisplayScrollString                                                 *
*   Parametreler:    clstart, clend, ypos, speed, *textptr, color              *
*   Return:                                                                    *
*******************************************************************************/
void P10_DisplayScrollString(unsigned int8  clstart, unsigned int8  clend, unsigned int8  ypos, unsigned int8  speed, char* textptr, unsigned int8  color)
{
  unsigned int8  row=0,clm=0;
  unsigned int16  xbit;
  rom unsigned int8  *saddress;
  unsigned int8  cbit;
  unsigned int8  delay=110-(speed*10);
 
  unsigned int8  add;
  
  if(P10_Currentfonts.Width>0x08)
  add=0x02;
  else
  add=0x01;
  
 // while (*textptr != 0)
  while ((*textptr != 0) && (!jetonAtildi))
  {
    saddress =(unsigned int8  *) &P10_Currentfonts.table[(unsigned int16 )(*textptr-32) * P10_Currentfonts.Height * add];  
    
    for(clm=1;clm<=P10_Currentfonts.Width;clm++)
     {
       for(row=0;row<P10_Currentfonts.Height;row++)
       {  
          if(P10_Currentfonts.Width>0x08) 
          {
           xbit=((unsigned int16)saddress[row*2]<<8) | (saddress[row*2+1]);
           cbit=(xbit>>(16-clm)) & 0x01;
          }
          else
          {
            xbit=saddress[row];
            cbit=(xbit>>(8-clm)) & 0x01;
          }   
          if(color) cbit=1-cbit;
          LineScroll(ypos+row,clstart,clend,cbit);
       }
       ////////
      //jetonKontrol(); if(jetonAtildi) break;
      ///////
      delay_ms(delay);
     }
     textptr++;
  } 
  
}
/*******************************************************************************
*   P10_DisplayShowString                                                      *
*   Parametreler:    Xpos, Ypos, *textptr, color                               *
*   Return:                                                                    *
*******************************************************************************/
void P10_DisplayShowString(unsigned int8  Xpos, unsigned int8  Ypos, char* textptr, unsigned int8  color)
{
  unsigned int8  refcolumn = Xpos;
  unsigned int8  refrow = Ypos;
    
  while ((*textptr != '\0')&&(!jetonAtildi))
  {
    P10_DisplayChar(refcolumn, refrow, *textptr, color);
    refcolumn += P10_Currentfonts.Width;
    
    if(refcolumn>P10_Max_Width) refrow += P10_Currentfonts.Height;

    textptr++;
  } 
}

/*******************************************************************************
*   P10_DisplayChar                                                            *
*   Parameter:    Xpos, Ypos, *textptr, color                                  *
*   Return:                                                                    *
*******************************************************************************/
void P10_DisplayChar(unsigned int8  Xpos, unsigned int8  Ypos, char ascii, unsigned int8  color)
{
   unsigned int8  add;
   ascii-=32;
    
   //Eï¿½er Font boyutu 8 den bï¿½yï¿½k ise Adresi 2 ile ï¿½arpï¿½yoruz.
   if(P10_Currentfonts.Width>0x08)add=2;else add=1;
   P10_DrawChar(Xpos, Ypos,(unsigned int8 *)&P10_Currentfonts.table[(unsigned int16) ascii * P10_Currentfonts.Height* add], color);
}

/*******************************************************************************
*   P10_DrawChar                                                               *
*   Parameter:    Xpos, Ypos, *saddress, color                                 *
*   Return:                                                                    *
*******************************************************************************/
void P10_DrawChar(unsigned int8  Xpos, unsigned int8  Ypos, rom unsigned int8  *saddress, unsigned int8  color)
{
   unsigned int8  index = 0, i = 0;
   unsigned int16  shift;
   unsigned int16  xbit;
   
   for(index=0;index<P10_Currentfonts.Height;index++)
   {  
         if(P10_Currentfonts.Width>0x08)
         {  
            xbit=((unsigned int16)saddress[index*2]<<8) | (saddress[index*2+1]);
            shift=0x8000;
         }
         else
         {
            xbit=saddress[index];
            shift=0x80;
         }
         
         for(i=0;i<P10_Currentfonts.Width;i++)
         {
           if(((xbit<<i) & shift)==0x00)
           {
              P10_PutPixel(Xpos+i,Ypos+index,color);
           }
           else
           {
              P10_PutPixel(Xpos+i,Ypos+index,1-color);
           }
         }
    }
}

/*******************************************************************************
*   P10_DrawLine                                                               *
*   Parameter:    x1, x2, y1, y2, fill                                         *
*   Return:                                                                    *
*******************************************************************************/
void P10_DrawLine(unsigned int8  x1, unsigned int8  y1, unsigned int8  x2, unsigned int8  y2, unsigned int8  color)
{
  signed int addx=1, addy=1, P;
  signed int i, dy, dx, diff;
  
   if(x2>x1) 
   {
      dx = x2 - x1;
   }else{
      dx = x1 - x2;
      addx = -1;
   }
   if(y2>y1)
   {
      dy = y2 - y1;
   }else{
      dy = y1 - y2;
      addy = -1;
   }

   if(dx >= dy)
   {
      dy *= 2;
      P = dy - dx;
      diff = P - dx;

      for(i=0; i<=dx; i++)
      {
         P10_PutPixel(x1, y1, color);

         if(P < 0)
         {
            P  += dy;
            x1 += addx;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
   else
   {
      dx *= 2;
      P = dx - dy;
      diff = P - dy;

      for(i=0; i<=dy; ++i)
      {
         P10_PutPixel(x1, y1, color);

         if(P < 0)
         {
            P  += dx;
            y1 += addy;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }   
}

/*******************************************************************************
*   P10_DrawRect                                                               *
*   Parameter:    x1, x2, y1, y2, fill                                         *
*   Return:                                                                    *
*******************************************************************************/
void P10_DrawRect(unsigned int8  x1, unsigned int8  y1, unsigned int8  x2, unsigned int8  y2, unsigned int8  fill, unsigned int8  color)
{
   unsigned int16  i, xmin, xmax, ymin, ymax;

  if(fill)
   {
      if(x1 < x2)
      {
         xmin = x1;
         xmax = x2;
      }
      else
      {
         xmin = x2;
         xmax = x1;
      }

      if(y1 < y2)
      {
         ymin = y1;
         ymax = y2;
      }
      else
      {
         ymin = y2;
         ymax = y1;
      }

      for(; xmin <= xmax; ++xmin)
      {
         for(i=ymin; i<=ymax; ++i)
         {
            P10_PutPixel(xmin, i, color);
         }
      }
   }
   else
   {
      P10_DrawLine(x1, y1, x2, y1, color);
      P10_DrawLine(x2, y1, x2, y2, color);
      P10_DrawLine(x2, y2, x1, y2, color);
      P10_DrawLine(x1, y2, x1, y1, color);
   }      
}

/*******************************************************************************
*   P10_DrawCircle                                                             *
*   Parameter:    x, y, Size, fill                                             *
*   Return:                                                                    *
*******************************************************************************/
void P10_DrawCircle(unsigned int8 x, unsigned int8 y, unsigned int8 size, unsigned int8 fill, unsigned int8 color)
{
   signed int a,b,P;
   a=0;
   b=size;
   P=1-size;
   
   do
   {
      if(fill)
      {
         P10_DrawLine(x-a, y+b, x+a, y+b,color);
         P10_DrawLine(x-a, y-b, x+a, y-b,color);
         P10_DrawLine(x-b, y+a, x+b, y+a,color);
         P10_DrawLine(x-b, y-a, x+b, y-a,color);
      }
      else
      {
         P10_PutPixel(a+x, b+y,color);
         P10_PutPixel(b+x, a+y,color);
         P10_PutPixel(x-a, b+y,color);
         P10_PutPixel(x-b, a+y,color);
         P10_PutPixel(b+x, y-a,color);
         P10_PutPixel(a+x, y-b,color);
         P10_PutPixel(x-a, y-b,color);
         P10_PutPixel(x-b, y-a,color);
      }
      
      if(P < 0)
      P+= 3 + 2*a++;
    else
      P+= 5 + 2*(a++ - b--);
     
  }while(a<=b);
}

void text_sil(void){
    int x=0;
    for(x=0;x<100;x++)text[x]="\0"; //\0 null karakter 
}
