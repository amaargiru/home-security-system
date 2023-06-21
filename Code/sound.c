#ifndef _sound_rec_
#define _sound_rec_

//#pragma language = extended
#include <iom128.h>					// Определения внутренних регистров
#include "portdefines.h"			// "Распиновка" портов
#include "kedahm.h"

#define T1_OVF  		0x01
#define CLEARED 		0x02
#define DF_BIT_READY	7

#define MAX_BUFFER   264
#define MAX_PAGE     2048

static unsigned int BufferCounter = 0;
static unsigned int PageCounter = 0;

// Ножка сброса AT45DB041B - PG1
#define DF_RESET (1<<nFRES)		// nFRES - из portdefines.h

// Буфер 1 
#define DF_BUF_1 0x00

// Буфер 2
#define DF_BUF_2 0x01

// Запись буфера 1 
#define DF_BUF_1_WR 0x84

// Запись буфера 2
#define DF_BUF_2_WR 0x87

// Чтение буфера 1
#define DF_BUF_1_RD 0x54

// Чтение буфера 2
#define DF_BUF_2_RD 0x56

#define DF_B1_MP_PROG_ERASE 0x83

#define DF_B2_MP_PROG_ERASE 0x86

#define DF_B1_MP_PROG_NO_ERASE 0x88

#define DF_B2_MP_PROG_NO_ERASE 0x89

// Программирование главной страницы памяти посредством буфера 1
#define DF_MP_PROG_B1 0x82

// Программирование главной страницы памяти посредством буфера 2
#define DF_MP_PROG_B2 0x85

// Автоматическая перезапись страницы через буфер 1
#define DF_AUTO_PAGE_REWR_B1 0x58

// Автоматическая перезапись страницы через буфер 2
#define DF_AUTO_PAGE_REWR_B2 0x59

// Сравнение главной страницы памяти с буфером 1
#define DF_MP_B1_COMP 0x60

// Сравнение главной страницы памяти с буфером 2
#define DF_MP_B2_COMP 0x61

// Заливка главной страницы памяти в буфер 1
#define DF_MP_B1_XFER 0x53

// Заливка главной страницы памяти в буфер 2
#define DF_MP_B2_XFER 0x55

// Регистр статуса микросхемы памяти
#define DF_SREG 0x57

// Чтение главной страницы памяти
#define DF_MP_READ 0x52

// Очистить страницу в 528 байт
#define PAGE_ERASE 0x81

// Очистить 512 страниц
#define BLOCK_ERASE 0x50

#define TRUE                0xff
#define FALSE               0x00

// Задержки для тактовой частоты 14.745600 МГц
#define QTR_MICRO_SECOND    2
#define HALF_MICROSECOND    4
#define ONE_MICROSECOND     8
#define TWO_MICROSECONDS    16
#define THREE_MICROSECONDS  24
#define FIVE_MICROSECONDS   40
#define TEN_MICROSECONDS    80
#define TWENTY_MICROSECONDS 160

// Прототипы функций
void DF_WriteSPI(unsigned char data);								// Запись байта по SPI
unsigned char DF_Status(void);										// Чтение байта статуса DataFlash
unsigned char DF_Ready(void);										// Программный аналог ножки Ready/~Busy
void DF_NextPageToNextBuffer(unsigned char active_buffer, unsigned int page_counter);
void DF_Playback(void);
void DF_ActiveBufferToSpeaker(unsigned char active_buffer);

volatile unsigned char wait = 0;

void DF_WriteSPI(unsigned char data)								// Запись байта по SPI
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));										// Ждем окончания передачи данных
}

unsigned char DF_Status(void)											// Чтение байта статуса DataFlash
{
    clrbit(PORTG, nFCS);													// Enable DataFlash

    DF_WriteSPI(DF_SREG);
    DF_WriteSPI(0xFF);

    setbit(PORTG, nFCS);													// Disable DataFlash
    return SPDR;
}

unsigned char DF_Ready(void)											// Программный аналог ножки Ready/~Busy
{
    return (DF_Status() & (1 << DF_BIT_READY));
}//DF_Ready

void DF_Playback(void)
{
    unsigned int PageCounter = 0;
    unsigned char ActiveBuffer = 1;									// Активизируем буфер 1

    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (1 << CPOL) | (1 << CPHA);	// SPI включен (режим № 3), Master mode, Fcl/16

    DF_NextPageToNextBuffer(ActiveBuffer, PageCounter);		//	Читаем страницу 0 в буфер 1

    while (DF_Ready == 0);												// Ждем освобождения DF

    while (PageCounter < MAX_PAGE - 1)									// Пока не прочитаем все
    {
        PageCounter++;														// Следующая страница
        DF_NextPageToNextBuffer(ActiveBuffer, PageCounter);	// Копируем очередную страницу
        DF_ActiveBufferToSpeaker(ActiveBuffer);					// Проговариваем

        if (ActiveBuffer == 1)											// Меняем активный буфер
        {
            ActiveBuffer++;
        }
        else
        {
            ActiveBuffer--;
        }
    }

    SPCR = 0x00;															// Disable SPI
}//DF_Playback

void DF_NextPageToNextBuffer(unsigned char active_buffer, unsigned int page_counter)
{
    while (DF_Ready == 0);												// Ждем освобождения DataFlash

    clrbit(PORTG, nFCS);													// Enable DataFlash
    if (active_buffer == 1)												// Если активен буфер 1
        DF_WriteSPI(DF_MP_B1_XFER);
    else																		// Если активен буфер 2
        DF_WriteSPI(DF_MP_B2_XFER);

    DF_WriteSPI((char)(page_counter >> 6));
    DF_WriteSPI((char)(page_counter << 2));
    DF_WriteSPI(0x00);

    setbit(PORTG, nFCS);													// Disable DataFlash
}//DF_NextPageToNextBuffer

void DF_ActiveBufferToSpeaker(unsigned char active_buffer)
{
    unsigned int buffer_counter = MAX_BUFFER;

    clrbit(PORTG, nFCS);													// Enable DataFlash

    if (active_buffer == 1)												// Если активен буфер 1
        DF_WriteSPI(DF_BUF_1_RD);
    else																		// Если активен буфер 2
        DF_WriteSPI(DF_BUF_2_RD);

    DF_WriteSPI(0x00);													// Незначащий байт
    DF_WriteSPI(0x00);
    DF_WriteSPI(0x00);													// Старт с нулевого адреса текущего буфера
    DF_WriteSPI(0x00);													// Незначащий байт

    do
    {
        DF_WriteSPI(0xFF);												// Пустышка
        SetActiveUart(UART_0);											// !!!
        printf(">%c", SPDR);												// !!!
        SetActiveUart(UART_1);											// !!!
    } while (--buffer_counter);

    setbit(PORTG, nFCS);													// Disable DataFlash
}//DF_ActiveBufferToSpeaker

#endif