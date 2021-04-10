/*
 By Liyanboy74
 https://github.com/liyanboy74
*/

#include <mega16.h>
#include <stdio.h>
#include <string.h>
#include "eeprom.h"

eeprom char cards[45][11];
eeprom char pass[4]={'1','2','3','4'};
eeprom int id=0;

char search(char *str)
{
char i,j,n=id;
unsigned char temp[11];
if(id==0)return 0;
for(j=0;j<=n;j++)
 {
 for(i=0;i<=10;i++)
  {
  temp[i]=cards[j][i];
  } 
 if(strcmp(temp,str)==0)return j+1; 
 }          
return 0; 
}

char save(char *str)
{
char i,n=id;
if(n>=45)return 0;
for(i=0;i<=10;i++)
 {
 cards[n][i]=str[i];
 }          
id++;
return n+1;
}

void remove(char ch)
{
int temp=id;
ch=ch-1;
id=ch;
save("          ");
id=temp;
}




