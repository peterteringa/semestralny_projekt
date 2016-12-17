#include "stm32l1xx.h"
#include "hd44780.h"

/************************************ Port a Piny pre LCD *******************************/
#define LCD_Port 		GPIOA
#define RS 				GPIO_Pin_1
#define EN 				GPIO_Pin_4
#define D4 				GPIO_Pin_7
#define D5 				GPIO_Pin_8
#define D6 				GPIO_Pin_9
#define D7 				GPIO_Pin_10

void strobeEN(void);
void upNib(uint8_t c);
void downNib(uint8_t c);
void Delay(uint32_t nCount);

/************************************ inicializuj LCD **********************************/
void lcdInit(void) {

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);						//GPIOA clock enable

	GPIO_InitTypeDef GPIO_InitStructure;									//GPIOA initialization
	GPIO_InitStructure.GPIO_Pin   = EN | RS | D4 | D5 | D6 | D7 | GPIO_Pin_5;
	GPIO_ResetBits(LCD_Port, EN | RS | D4 | D5 | D6 | D7);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(LCD_Port, &GPIO_InitStructure);

	GPIO_ResetBits(LCD_Port, EN | RS | D4 | D5 | D6 | D7);
	Delay(0xffff);
	sendCMD(0x02);
	Delay(0x3FFFC);  //wait 20ms
	sendCMD(0x28);  //LCD configs
	sendCMD(0x06);
	sendCMD(0x01);
	sendCMD(0x0E);
	Delay(0xffff);
}
/************************************ generuj hodinovy signal pre LCD ******************/
void strobeEN(void) {
	Delay(0x3ff);
	GPIO_SetBits(LCD_Port, EN);
	Delay(0x3ff);
	GPIO_ResetBits(LCD_Port, EN);
}
/************************************ nastav horne 4 bity znaku ************************/
void upNib(uint8_t c) {
	if(c & 0x80)
		GPIO_SetBits(LCD_Port, D7);
	else
		GPIO_ResetBits(LCD_Port, D7);
	if(c & 0x40)
		GPIO_SetBits(LCD_Port, D6);
	else
		GPIO_ResetBits(LCD_Port, D6);
	if(c & 0x20)
		GPIO_SetBits(LCD_Port, D5);
	else
		GPIO_ResetBits(LCD_Port, D5);
	if(c & 0x10)
		GPIO_SetBits(LCD_Port, D4);
	else
		GPIO_ResetBits(LCD_Port, D4);
}
/************************************ nastav dolne 4 bity znaku ************************/
void downNib(uint8_t c) {
	if(c & 0x8)
		GPIO_SetBits(LCD_Port, D7);
	else
		GPIO_ResetBits(LCD_Port, D7);
	if(c & 0x4)
		GPIO_SetBits(LCD_Port, D6);
	else
		GPIO_ResetBits(LCD_Port, D6);
	if(c & 0x2)
		GPIO_SetBits(LCD_Port, D5);
	else
		GPIO_ResetBits(LCD_Port, D5);
	if(c & 0x1)
		GPIO_SetBits(LCD_Port, D4);
	else
		GPIO_ResetBits(LCD_Port, D4);
}
/************************************ posli prikaz *************************************/
void sendCMD(uint8_t c) {
	GPIO_ResetBits(LCD_Port, RS);
	upNib(c);
	strobeEN();
	downNib(c);
	strobeEN();
}
/************************************ posli znak ***************************************/
void printChar(uint8_t c) {
	if(((c>=0x20)&&(c<=0x7F)) || ((c>=0xA0)&&(c<=0xFF))) {	//check if 'c' is within display boundry
		GPIO_SetBits(LCD_Port, RS);
		upNib(c);
		strobeEN();
		downNib(c);
		strobeEN();
		GPIO_ResetBits(LCD_Port, RS);
	}
}
/************************************ posli retazec znakov *****************************/
void printString(uint8_t *s) {
	uint8_t i=0;
	//while (s[i] == '\0') i++;
	while(s[i] != '\0') {
		printChar(s[i]);
		i++;
	}
}
/************************************ vycisti displej **********************************/
void clearLCD(void) {
	sendCMD(0x01);
}
/************************************ skoc do riadku jedna *****************************/
void toLine1(void) {
	sendCMD(0x80);
}
/************************************ skoc do riadku dva *******************************/
void toLine2(void) {
	sendCMD(0xC0);
}
/************************************ vypni kurzor *************************************/
void cursoroff(void){
	sendCMD(0x0C);
}
/************************************ nastav poziciu kurzora na LDC ********************/
void cursorpos(int col, int row){
	if (row == 1) col += 0x7f;
	if (row == 2) col += 0xbf;
	sendCMD(col);
}

/************************************ prevod cisla na text *****************************/
int num2text(uint16_t cislo) {

	static uint8_t i, j, k;
	static char text[16], pom;

	for (i=0;i<16;i++) text[i] = '\0';

	if (cislo == 0) text[0] = '0';

	i = 15;
	while (cislo > 0) {
		text[15-i] = cislo%10 + '0';
		cislo /= 10;
		i--;
	}

	i = 15;
	while (text[i] == '\0') i--;

	k = i;

	for (j=0;j<(i+1)/2;j++) {
		pom = text[j];
		text[j] = text[k];
		text[k] = pom;
		k--;
	}
	return text;
}

void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
