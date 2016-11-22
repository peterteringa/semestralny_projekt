#include "stm32l1xx.h"

void lcdInit(void);
void sendCMD(uint8_t c);
void printChar(uint8_t c);
void printString(uint8_t *s);
void clearLCD(void);
void toLine1(void);
void toLine2(void);
void Delay(uint32_t nCount);
void cursoroff(void);
void cursorpos(int col, int row);
int num2text(uint16_t cislo);
