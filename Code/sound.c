#ifndef _sound_rec_
#define _sound_rec_

//#pragma language = extended
#include <iom128.h>					// ����������� ���������� ���������
#include "portdefines.h"			// "����������" ������
#include "kedahm.h"

#define T1_OVF  		0x01
#define CLEARED 		0x02
#define DF_BIT_READY	7

#define MAX_BUFFER   264
#define MAX_PAGE     2048

static unsigned int BufferCounter = 0;
static unsigned int PageCounter = 0;

// ����� ������ AT45DB041B - PG1
#define DF_RESET (1<<nFRES)		// nFRES - �� portdefines.h

// ����� 1 
#define DF_BUF_1 0x00

// ����� 2
#define DF_BUF_2 0x01

// ������ ������ 1 
#define DF_BUF_1_WR 0x84

// ������ ������ 2
#define DF_BUF_2_WR 0x87

// ������ ������ 1
#define DF_BUF_1_RD 0x54

// ������ ������ 2
#define DF_BUF_2_RD 0x56

#define DF_B1_MP_PROG_ERASE 0x83

#define DF_B2_MP_PROG_ERASE 0x86

#define DF_B1_MP_PROG_NO_ERASE 0x88

#define DF_B2_MP_PROG_NO_ERASE 0x89

// ���������������� ������� �������� ������ ����������� ������ 1
#define DF_MP_PROG_B1 0x82

// ���������������� ������� �������� ������ ����������� ������ 2
#define DF_MP_PROG_B2 0x85

// �������������� ���������� �������� ����� ����� 1
#define DF_AUTO_PAGE_REWR_B1 0x58

// �������������� ���������� �������� ����� ����� 2
#define DF_AUTO_PAGE_REWR_B2 0x59

// ��������� ������� �������� ������ � ������� 1
#define DF_MP_B1_COMP 0x60

// ��������� ������� �������� ������ � ������� 2
#define DF_MP_B2_COMP 0x61

// ������� ������� �������� ������ � ����� 1
#define DF_MP_B1_XFER 0x53

// ������� ������� �������� ������ � ����� 2
#define DF_MP_B2_XFER 0x55

// ������� ������� ���������� ������
#define DF_SREG 0x57

// ������ ������� �������� ������
#define DF_MP_READ 0x52

// �������� �������� � 528 ����
#define PAGE_ERASE 0x81

// �������� 512 �������
#define BLOCK_ERASE 0x50

#define TRUE                0xff
#define FALSE               0x00

// �������� ��� �������� ������� 14.745600 ���
#define QTR_MICRO_SECOND    2
#define HALF_MICROSECOND    4
#define ONE_MICROSECOND     8
#define TWO_MICROSECONDS    16
#define THREE_MICROSECONDS  24
#define FIVE_MICROSECONDS   40
#define TEN_MICROSECONDS    80
#define TWENTY_MICROSECONDS 160

// ��������� �������
void DF_WriteSPI(unsigned char data);								// ������ ����� �� SPI
unsigned char DF_Status(void);										// ������ ����� ������� DataFlash
unsigned char DF_Ready(void);										// ����������� ������ ����� Ready/~Busy
void DF_NextPageToNextBuffer(unsigned char active_buffer, unsigned int page_counter);
void DF_Playback(void);
void DF_ActiveBufferToSpeaker(unsigned char active_buffer);

volatile unsigned char wait = 0;

void DF_WriteSPI(unsigned char data)								// ������ ����� �� SPI
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));										// ���� ��������� �������� ������
}

unsigned char DF_Status(void)											// ������ ����� ������� DataFlash
{
    clrbit(PORTG, nFCS);													// Enable DataFlash

    DF_WriteSPI(DF_SREG);
    DF_WriteSPI(0xFF);

    setbit(PORTG, nFCS);													// Disable DataFlash
    return SPDR;
}

unsigned char DF_Ready(void)											// ����������� ������ ����� Ready/~Busy
{
    return (DF_Status() & (1 << DF_BIT_READY));
}//DF_Ready

void DF_Playback(void)
{
    unsigned int PageCounter = 0;
    unsigned char ActiveBuffer = 1;									// ������������ ����� 1

    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (1 << CPOL) | (1 << CPHA);	// SPI ������� (����� � 3), Master mode, Fcl/16

    DF_NextPageToNextBuffer(ActiveBuffer, PageCounter);		//	������ �������� 0 � ����� 1

    while (DF_Ready == 0);												// ���� ������������ DF

    while (PageCounter < MAX_PAGE - 1)									// ���� �� ��������� ���
    {
        PageCounter++;														// ��������� ��������
        DF_NextPageToNextBuffer(ActiveBuffer, PageCounter);	// �������� ��������� ��������
        DF_ActiveBufferToSpeaker(ActiveBuffer);					// �������������

        if (ActiveBuffer == 1)											// ������ �������� �����
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
    while (DF_Ready == 0);												// ���� ������������ DataFlash

    clrbit(PORTG, nFCS);													// Enable DataFlash
    if (active_buffer == 1)												// ���� ������� ����� 1
        DF_WriteSPI(DF_MP_B1_XFER);
    else																		// ���� ������� ����� 2
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

    if (active_buffer == 1)												// ���� ������� ����� 1
        DF_WriteSPI(DF_BUF_1_RD);
    else																		// ���� ������� ����� 2
        DF_WriteSPI(DF_BUF_2_RD);

    DF_WriteSPI(0x00);													// ���������� ����
    DF_WriteSPI(0x00);
    DF_WriteSPI(0x00);													// ����� � �������� ������ �������� ������
    DF_WriteSPI(0x00);													// ���������� ����

    do
    {
        DF_WriteSPI(0xFF);												// ��������
        SetActiveUart(UART_0);											// !!!
        printf(">%c", SPDR);												// !!!
        SetActiveUart(UART_1);											// !!!
    } while (--buffer_counter);

    setbit(PORTG, nFCS);													// Disable DataFlash
}//DF_ActiveBufferToSpeaker

#endif