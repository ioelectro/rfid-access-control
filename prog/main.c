/*
 By Liyanboy74
 https://github.com/liyanboy74
*/

#include <mega16.h>
#include <alcd.h>
#include <stdio.h>
#include <string.h>
#include <delay.h>

#include "uart.h"
#include "key.h"
#include "eeprom.h"

#define relay   PORTC.0
#define buzzer  PORTC.1
#define led1    PORTD.3
#define led2    PORTD.4

void bib(char mode)
{
int i=0;
if(mode==1) // insert ok card
 {   
 for(i=0;i<100;i++)
  {
  buzzer=1;
  delay_us(100);
  buzzer=0;
  delay_us(200);
  } 
 for(i=0;i<100;i++)
  {
  buzzer=1;
  delay_us(100);
  buzzer=0;
  delay_us(100);
  }
 for(i=0;i<100;i++)
  {
  buzzer=1;
  delay_us(200);
  buzzer=0;
  delay_us(100);
  }  
 }
else if(mode==0)  //wrong
 {
 for(i=0;i<500;i++)
  {
  buzzer=1;
  delay_us(200);
  buzzer=0;
  delay_us(100);
  }    
 }
else if(mode==2)  //press key
 {
 buzzer=1;
 delay_ms(60);
 buzzer=0; 
 }   
else if(mode==3)  //ok
 {
 buzzer=1;
 delay_ms(100);
 buzzer=0; 
 delay_ms(50);
 buzzer=1;
 delay_ms(200);
 buzzer=0;  
 }
}

void main(void)
{
char ch;
char str[22];
DDRC=0XFF;
DDRD.3=1;
DDRD.4=1;
usart_init();
lcd_init(16);
delay_ms(50);
#asm("sei")
      
while (1)
      {                        
      lcd_clear();
      lcd_puts("Insert Card:");
      delay_ms(200);   
      if(rx_counter>0)
       {
       scanf("%s",str);  
       lcd_clear();
       ch=search(str);
       if(ch!=0)
        { 
        led2=1;
        lcd_puts("Welcome");
        sprintf(str,"Your ID is: %d",ch);
        lcd_gotoxy(0,1);
        lcd_puts(str); 
        bib(1);
        relay=1;
        delay_ms(500);
        relay=0;
        delay_ms(500);
        while(rx_counter>0)getchar();
        led2=0;
        }
       else
        {  
        led1=1;
        lcd_puts("Not found");
        bib(0);
        delay_ms(500);
        while(rx_counter>0)getchar(); 
        led1=0;
        } 
       }
       delay_ms(200); 
       
       if(chek_star()==1)
       {
       char ch,p1,p2,p3,p4;  
       lcd_clear();
       lcd_puts("Enter Password:"); 
       bib(2);delay_ms(100);  
       while(chek_star());
       delay_ms(100);
       lcd_gotoxy(0,1);
       lcd_puts("----");
       lcd_gotoxy(0,1); 

       p1=getkey();lcd_putchar('*'); 
       bib(2);delay_ms(100);
       p2=getkey();lcd_putchar('*');
       bib(2);delay_ms(100);
       p3=getkey();lcd_putchar('*');
       bib(2);delay_ms(100);
       p4=getkey();lcd_putchar('*');
       bib(2);delay_ms(100);
          
       delay_ms(200);  
          
       if(p1==pass[0]&&p2==pass[1]&&p3==pass[2]&&p4==pass[3]) 
       {
       lcd_clear();  
       lcd_puts("Menu:1=Add 2=Del");lcd_gotoxy(0,1);
       lcd_puts("3=Del All 4=Pass");
           
       ch=getkey();
       bib(2);delay_ms(100);
        
       while(rx_counter>0)getchar();
           
       if(ch=='4')
        {
        lcd_clear();
        lcd_puts("Enter New Pass:");
        lcd_gotoxy(0,1);
        lcd_puts("----");
        lcd_gotoxy(0,1);
        p1=getkey();lcd_putchar(p1);
        bib(2);delay_ms(100);
        p2=getkey();lcd_putchar(p2); 
        bib(2);delay_ms(100);
        p3=getkey();lcd_putchar(p3); 
        bib(2);delay_ms(100);
        p4=getkey();lcd_putchar(p4);
        bib(2);delay_ms(100); 
        delay_ms(200);
        lcd_clear();
        lcd_puts("Are you sure?");lcd_gotoxy(0,1);
        lcd_puts("*=Yes       #=No");   
        ch=getkey(); 
        if(ch=='*')
         {
         pass[0]=p1;
         pass[1]=p2;
         pass[2]=p3;
         pass[3]=p4;
         lcd_clear();
         lcd_puts("Password Changed");
         bib(3);
         delay_ms(1000);
         }
        }
        else if(ch=='3')
        {
        lcd_clear();
        lcd_puts("Are you sure?");lcd_gotoxy(0,1);
        lcd_puts("*=Yes       #=No"); 
        ch=getkey();    
        if(ch=='*')
         {
         int i=0,j=0;
         for(j=0;j<=44;j++)
          {
          for(i=0;i<=10;i++)
           {
           cards[j][i]=0xff;
           }  
          }
         id=0;
         lcd_clear();
         lcd_puts("All users Deleted!"); 
         bib(3);
         delay_ms(1000);
         }
        }
       else if(ch=='2')
        {  
        char str[22];
        lcd_clear();
        lcd_puts("Insert Card to  Remove it:");
        scanf("%s",str);  
        lcd_clear();  
        bib(1);delay_ms(100); 
        ch=search(str);
        if(ch==0)
         { 
         lcd_puts("Not fund");    
         bib(3);delay_ms(100);
         delay_ms(900);
         }
        else
         {
         lcd_clear();
         lcd_puts("Are you sure?");lcd_gotoxy(0,1);
         lcd_puts("*=Yes       #=No"); 
         if(getkey()=='*')
          {  
          remove(ch); 
          lcd_clear();
          lcd_puts("user Deleted!"); 
          bib(3);delay_ms(100);
          delay_ms(1000);
          }
         }   
            
        }
        else if(ch=='1')
        {
        char str[22];
        lcd_clear();
        lcd_puts("Insert Card to  Save it:");
        scanf("%s",str);  
        lcd_clear();   
        bib(1);delay_ms(100);
        ch=search(str);
        if(ch==0)
         { 
         char re;
         re=save(str);
         if(re!=0)
          {
          lcd_clear();
          lcd_puts("Card Saved !");
          lcd_gotoxy(0,1);
          sprintf(str,"Your ID is: %d",re);
          lcd_puts(str);  
          bib(3);delay_ms(100);
          delay_ms(1900); 
          }
         else
          {
          lcd_clear();
          lcd_puts("Memory is full!");    
          bib(0);delay_ms(100);
          delay_ms(2000);
          }
         }
        else
         {
         lcd_clear();
         lcd_puts("The Card Is Save"); 
         bib(0);delay_ms(100); 
         delay_ms(900);
         }
        }     
       }
       else
        {
        lcd_clear();
        lcd_puts("Wrong Password");
        bib(0);delay_ms(100);
        delay_ms(1000);   
        } 
       while(rx_counter>0)getchar();   
       }  
      }
}
