/*
 * main.c
 *
 *  Created on: 2010-03-31
 *       Autor: Miros³aw Kardaœ
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "LCD/lcd44780.h"
#define soft_reset()
#define UART_BAUD_RATE 57600
#include "uart/uart.h"



	int temp_o1 = 0;
	int temp_o2 = 0;
	int temp_o3 = 0;
	int temp_o4 = 0;
	int temp_o5 = 0;
	int temp_o6 = 0;
	int temp_PTT = 0;


	int kanal;
	int sec;

void zapytanie_kanal(void);

void LCD_obraz(void);
char c;


/*
*************Prze³¹cznik obrotowy*******************
PD7 -1
PD6 -2
PD5 -3
PD4 -4
PD3 -5
PD2 -6
*/
//**************DIODA RGB*************

// Dioda RED
#define C_PIN (1 << PC3)
#define C_ON PORTC |= C_PIN;;
#define C_OFF PORTC &= ~C_PIN;;
#define C_TOG PORTC ^= C_PIN;

// Dioda BLUE
#define B_PIN (1 << PC2)
#define B_ON PORTC |= B_PIN;;
#define B_OFF PORTC &= ~B_PIN;;
#define B_TOG PORTC ^= B_PIN;


// Dioda GREEN
#define G_PIN (1 << PC1)
#define G_ON PORTC |= G_PIN;;
#define G_OFF PORTC &= ~G_PIN;;
#define G_TOG PORTC ^= G_PIN;


//*******************************
//	STAN dla 1
#define O1_PIN (1<<PD7)
#define O1_DOWN !(PIND & O1_PIN)

//	STAN dla 2
#define O2_PIN (1<<PD6)
#define O2_DOWN !(PIND & O2_PIN)

//	STAN dla 3
#define O3_PIN (1<<PD5)
#define O3_DOWN !(PIND & O3_PIN)

//	STAN dla 4
#define O4_PIN (1<<PD4)
#define O4_DOWN !(PIND & O4_PIN)

//	STAN dla 5
#define O5_PIN (1<<PD3)
#define O5_DOWN !(PIND & O5_PIN)

//	STAN dla 6
#define O6_PIN (1<<PB1)
#define O6_DOWN !(PINB & O6_PIN)




//*******Przycisk PTT*****************//
#define PTT_PIN (1<<PB0)
#define PTT_DOWN !(PINB & PTT_PIN)

char buf[3];
int main(void)
{
/*
	PC3
	PC2
	PC1
	*/
	DDRC |= C_PIN;
	DDRC |= B_PIN;
	DDRC |= G_PIN;


    //init uart
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));

	//init interrupt
	sei();

//	*************Prze³¹cznik obrotowy*******************
//Przycisk 1
DDRD &= ~O1_PIN;
PORTD |= O1_PIN;

//Przycisk 2
DDRD &= ~O2_PIN;
PORTD |= O2_PIN;

//Przycisk 3
DDRD &= ~O3_PIN;
PORTD |= O3_PIN;

//Przycisk 4
DDRD &= ~O4_PIN;
PORTD |= O4_PIN;

//Przycisk 5
DDRD &= ~O5_PIN;
PORTD |= O5_PIN;

//Przycisk 6
DDRB &= ~O6_PIN;
PORTB |= O6_PIN;

//*******Przycisk PTT*****************

DDRB &= ~PTT_PIN;
PORTB |= PTT_PIN;


	zapytanie_kanal();
	lcd_init();

	LCD_obraz();

int kanal;
char odebrany_znak;
char odbior[20];

char kanal_o[20];
char tab_f[10];
char tab_o[10];
char tab_t[10];
int t;



while(1)
{



	if(t != 1)
	{
	C_ON;
	G_OFF;
	B_OFF;
	}

	//uart_puts("PTT ");

		//uart_puts("PTT ");




			c = uart_getc();
			tab_o[0] = c;
			tab_f[0] = c;
			tab_t[0] = c;

	    	if(tab_o[0] == 'T'){

	    	tab_o[1]= uart_getc();
	    	if(tab_o[1]== '1')
	    	{


	    	lcd_locate(3,13);
	    	lcd_str("T");
	    	C_OFF;
	    	G_ON;
	    	B_OFF;
	    	t = 1;

	    	}
	    	if(tab_o[1]== '0')
	    		    	{


	    		    	lcd_locate(3,13);
	    		    	lcd_str(" ");
	    		    	C_OFF;
	    		    	G_OFF;
	    		    	B_OFF;
	    		    	t = 0;
	    		    	}

	    	}


	    	if(tab_t[0] == 'R'){
	    		    	tab_t[1]= uart_getc();

	    		    	if(tab_t[1] == '1')
	    		    	{


	    		    	lcd_locate(3,14);
	    		    	lcd_str("R");
	    		    	C_OFF;
	    		    	G_OFF;
	    		    	B_ON;
	    		    	t = 1;

	    		    	}

	    		    	if(tab_t[1] == '0')
	    		    		    	{


	    		    		    	lcd_locate(3,14);
	    		    		    	lcd_str(" ");
	    		    		    	C_OFF;
	    		    		    	G_OFF;
	    		    		    	B_OFF;
	    		    		    	t = 0;
	    		    		    	}

	    		    	}




	    	if(tab_f[0] == 'F')
	    	{
	    		lcd_locate(1,6);
	    		lcd_str("        ");

	    		tab_f[0]=uart_getc();
	    		if(tab_f[0] != '0')
	    		{
	    		 lcd_locate(1,6);
	    		 lcd_char(tab_f[0]);

	     	    }


	    		   tab_f[1]= uart_getc();
				   lcd_locate(1,7);
	    	       lcd_char(tab_f[1]);




	    	    tab_f[2]= uart_getc();
	    	    lcd_locate(1,8);
	    	    lcd_char(tab_f[2]);


	    	    tab_f[3]= uart_getc();
	    	    lcd_locate(1,9);
	    	    lcd_char(tab_f[3]);


	    	    tab_f[4]=uart_getc();
	    	    lcd_locate(1,10);
	    	    lcd_char(tab_f[4]);


	    	    tab_f[5]=uart_getc();
	    	    lcd_locate(1,11);
	    	    lcd_char(tab_f[5]);



	    	    tab_f[6]=uart_getc();
	    	    lcd_locate(1,12);
	    	    lcd_char(tab_f[6]);



	    	    tab_f[7]= uart_getc();
	    	    lcd_locate(1,13);
	    	    lcd_char(tab_f[7]);


	 	}




//uart_puts("PTT ");
sterowanie_PTT();
kanal = o1();
kanal = o2();
kanal = o3();
kanal = o4();
kanal = o5();
kanal = o6();



if( sec == 30000)
{
	if (kanal != 0)
	{
		uart_puts(">");
		uart_puts("CHAN ");
		uart_putint(kanal, 10);
		uart_putc('\r');
		uart_putc('\n');
		_delay_ms(300);

		if(kanal == 1)
		{
		lcd_locate(0,8);
		lcd_str("1");
		lcd_str(" ");
		}

		if(kanal == 2)
		{
		lcd_locate(0,8);
		lcd_str("2");
		lcd_str(" ");
		}

		if(kanal == 3)
		{
		lcd_locate(0,8);
		lcd_str("3");
		lcd_str(" ");
		}

		if(kanal == 4)
		{
		lcd_locate(0,8);
		lcd_str("4");
		lcd_str(" ");
		}

		if(kanal == 5)
		{
		lcd_locate(0,8);
		lcd_str("5");
		lcd_str(" ");
		}

		if(kanal == 6)
		{
		lcd_locate(0,8);
		lcd_str("6");
		lcd_str(" ");
		}


	}
	sec =0;

}
sec++;
}

}



void sterowanie_PTT(void)
{
	if(PTT_DOWN != temp_PTT)
	{
		temp_PTT = PTT_DOWN;
		if(temp_PTT == 1)
		{
			uart_puts(">");
			uart_puts("PTT ");
			uart_puts(" 1");
			uart_putc('\r');
			uart_putc('\n');
			_delay_ms(300);

		}
		if(temp_PTT == 0)
				{
			        uart_puts(">");
					uart_puts("PTT ");
					uart_puts(" 0");
					uart_putc('\r');
					uart_putc('\n');
					_delay_ms(300);

				}
	}
}



void zapytanie_kanal(void)
	{
		uart_puts("LIST");
		uart_putc('\r');
		uart_putc('\n');
	}

void LCD_obraz(void)
{
	lcd_locate(0,0);
	lcd_str("CHANEL:");				// napis z pamiêci FLASH

	lcd_locate(1,0);
	lcd_str("FREQ: ");

	lcd_locate(3,0);
	lcd_str("STATUS");
	lcd_locate(3,15);
	lcd_str("X");

}






int o1 (void)
{

	if(O1_DOWN != temp_o1)
			{
				temp_o1 = O1_DOWN;
				if(temp_o1 == 1)
				{
			        uart_puts(">");
					uart_puts("CHAN 1");
					uart_putc('\r');
					uart_putc('\n');
					kanal = 1;
					_delay_ms(300);

				}

				lcd_locate(0,8);
				lcd_str("1");
				lcd_str(" ");
			}
return kanal;
}
int o2(void)
{

	if(O2_DOWN != temp_o2)
	{
		temp_o2 = O2_DOWN;
		if(temp_o2 == 1)
		{
	        uart_puts(">");
			uart_puts("CHAN 2");
			uart_putc('\r');
			uart_putc('\n');
			_delay_ms(300);
			kanal = 2;
		}
		lcd_locate(0,8);
		lcd_str(" ");
		lcd_str(" ");
	}
	return kanal;
}
int o3(void)
{

	if(O3_DOWN != temp_o3)
				{
				temp_o3 = O3_DOWN;
				if(temp_o3 == 1)
				{
			        uart_puts(">");
					uart_puts("CHAN 3");
					uart_putc('\r');
					uart_putc('\n');
					_delay_ms(500);
					kanal = 3;
				}

					lcd_locate(0,8);
					lcd_str("3");
					lcd_str(" ");
				}
	return kanal;
}
int o4(void)
{

	if(O4_DOWN != temp_o4)
				{
				temp_o4 = O4_DOWN;
				if(temp_o4 == 1)
				{
			        uart_puts(">");
					uart_puts("CHAN 4");
					uart_putc('\r');
					uart_putc('\n');
					kanal = 4;

					_delay_ms(300);
				}

					lcd_locate(0,8);
					lcd_str("4");
					lcd_str(" ");
				}
	return kanal;
}

int o5(void)
{

	if(O5_DOWN != temp_o5)
				{
				temp_o5 = O5_DOWN;
				if(temp_o5 == 1)
				{
			        uart_puts(">");
					uart_puts("CHAN 5");
					uart_putc('\r');
					uart_putc('\n');
					kanal = 5;
					_delay_ms(300);

				}
					lcd_locate(0,8);
					lcd_str("5");
					lcd_str(" ");

				}
	return kanal;
}
int o6(void)
{

	if(O6_DOWN != temp_o6)
				{
				temp_o6 = O6_DOWN;
				if(temp_o6 == 1)
				{
			        uart_puts(">");
					uart_puts("CHAN 6");
					uart_putc('\r');
					uart_putc('\n');
					kanal = 6;
					_delay_ms(300);

				}
					lcd_locate(0,8);
					lcd_str("6");
					lcd_str(" ");

				}
	return kanal;
}
