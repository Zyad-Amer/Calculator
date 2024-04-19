/*
 * calculator.c
 *
 *  Created on: Feb 29, 2024
 *      Author: Zyad Montaser
 */
#include "lcd.h"
#include "keypad.h"
#include <util/delay.h> /* for delay function*/

#define ASCI_OF_ON_BUTTON  13
#define MAX_NUM_OF_SIGNS   7/*max number of signs to be displayed on 2x16 lcd*/
#define MAX_NUM_OF_NUMBERS 8/*max number of numbers to be displayed on 2x16 lcd(8 nums and 7 signs)*/

float64 calculate(sint64*nums,uint8*sign,uint8 signSize)
{
	float64 result=0;
	uint8 i,flag=0;

	for(i=0;i<signSize;i++)
	{
		if(sign[i]=='+' ||sign[i]=='-')
		{
			flag=1;
		}
	}

	if(flag==1)
	{
		/*loop to do the multiplication and division operations firstly and assign it in nums array*/
		for(i=0;i<signSize;i++)
		{
			if(sign[i]=='*')
			{
				nums[i]=nums[i]*nums[i+1];
				if(i==0)
				{
					result=nums[i];
				}
			}
			else if(sign[i]=='/')
			{
				nums[i]=nums[i]/nums[i+1];
				if(i==0)
				{
					result=nums[i];
				}
			}
		}
	}

	/*loop to do the addition and subtraction operations and calculate the final result*/
	for(i=0;i<signSize;i++)
	{
		if(sign[i]=='+')
		{
			if(i==0)
			{
				result=nums[i]+nums[i+1];
			}
			else
			{
				result+=nums[i+1];
			}
		}
		else if(sign[i]=='-')
		{
			if(i==0)
			{
				result=nums[i]-nums[i+1];
			}
			else
			{
				result-=nums[i+1];
			}
		}
		if(flag==0)
		{
			if(sign[i]=='*')
			{
				if(i==0)
				{
					result=nums[i]*nums[i+1];
				}
				else
				{
					result*=nums[i+1];
				}
			}
			else if(sign[i]=='/')
			{
				if(i==0)
				{
					result=nums[i]/nums[i+1];
				}
				else
				{
					result/=nums[i+1];
				}
			}
		}
	}
	return result;
}

int main()
{
	sint64 nums[MAX_NUM_OF_NUMBERS];
	uint8 i=0,j=0,firstDigit=TRUE,key,sign[MAX_NUM_OF_SIGNS];

	LCD_init();

	/*Display identification message*/
	LCD_displayStringRowColumn(0,3,"Calculator");
	_delay_ms(1000);
	LCD_clearScreen();

	while(1)
	{
    	key=KEYPAD_getPressedKey();

		if(key==ASCI_OF_ON_BUTTON)/*button for clear all*/
		{
			LCD_clearScreen();
			i=-1;
			j=0;
			firstDigit=TRUE;
		}
		else if(key=='=')
		{
			LCD_moveCursor(1,0);
			LCD_displayCharacter('=');
			LCD_intgerToString(calculate(nums,sign,i));/*calculate the result and display it on the LCD*/
		}
		else if(key =='+'||key=='-'||key=='*'||key=='/')
		{
			/*display the entered sign on the lcd*/
			LCD_moveCursor(0,i);
			LCD_displayCharacter(key);

			sign[j]=key;
			firstDigit=TRUE;
			j++;
		}
		else if(key<=9)
		{
			/*display the entered number on the lcd*/
			LCD_moveCursor(0,i);
			LCD_intgerToString(key);

			if(firstDigit==TRUE)/*if it is the first digit in the number*/
			{
				nums[j]=key;
				firstDigit=FALSE;
			}
			else
				nums[j]=nums[j]*10+key;
		}

		i++;
		_delay_ms(200);
	}
}
