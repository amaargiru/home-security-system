#ifndef CODE_DATAFLASH_H
#define CODE_DATAFLASH_H

#include "ibutton.h"
#include "guard.h"
#include "ds1990a.h"            // Работа с ключами iButton
#include "eeprom.h"            // Описание переменных в EEPROM
#include "madelay.h"            // Задержки
#include "portdefines.h"    // "Распиновка" портов
#include "bitbyt.h"            // Определение операций с битами
#include <string.h>            // Операции со строками
#include <ctype.h>            // Операции с символами
#include <stdio.h>            // Стандартный ввод-вывод
#include <inavr.h>            // Intrinsic-функции
#include <iom128.h>            // Определения внутренних регистров

#define DF_RESET (1<<nFRES)		// nFRES - �� portdefines.h
#define DF_RESET (1<<nFRES)		// nFRES - из portdefines.h
#define DF_MM_RD 0xD2
#define DF_BUF_1_RD 0x54
#define DF_BUF_1_RD 0xD4
#define DF_BUF_2_RD 0x56
#define DF_BUF_2_RD 0xD6
#define DF_MP_B1_XFER 0x53
#define DF_MP_B1_XFER 0x53
#define DF_MP_B2_XFER 0x55
#define DF_MP_B2_XFER 0x55
#define DF_SREG 0x57
#define DF_SREG 0xD7

#define	SOUND_FACTOR		8000													// Отсчетов в секунду
#define	VVEDITE_START		5.9													// Начало слова "Введите" в прошитом звуковом файле в секундах
#define	VVEDITE_START_DF	VVEDITE_START*(SOUND_FACTOR/MAX_BUFFER)	// Страница DataFlash
#define	VVEDITE_END			7														// Конец слова "Введите" в прошитом звуковом файле в секундах
#define	VVEDITE_END_DF		VVEDITE_END*(SOUND_FACTOR/MAX_BUFFER)		// Страница DataFlash
#define	PAROL_START			7														// "Пароль"
#define	PAROL_START_DF		PAROL_START*(SOUND_FACTOR/MAX_BUFFER)
#define	PAROL_END			7.5
#define	PAROL_END_DF		PAROL_END*(SOUND_FACTOR/MAX_BUFFER)
#define	KOMANDU_START		7.5													// "Команду"
#define	KOMANDU_START_DF	KOMANDU_START*(SOUND_FACTOR/MAX_BUFFER)
#define	KOMANDU_END			8.3
#define	KOMANDU_END_DF		KOMANDU_END*(SOUND_FACTOR/MAX_BUFFER)
#define	SLKOMANDU_START		8.3												// "Следующую команду"
#define	SLKOMANDU_START_DF	SLKOMANDU_START*(SOUND_FACTOR/MAX_BUFFER)
#define	SLKOMANDU_END			9.8
#define	SLKOMANDU_END_DF		SLKOMANDU_END*(SOUND_FACTOR/MAX_BUFFER)
#define	RELE_START			12.9													// "Реле"
#define	RELE_START_DF		RELE_START*(SOUND_FACTOR/MAX_BUFFER)
#define	RELE_END				13.7
#define	RELE_END_DF			RELE_END*(SOUND_FACTOR/MAX_BUFFER)
#define	ODIN_START			4														// "Один"
#define	ODIN_START_DF		ODIN_START*(SOUND_FACTOR/MAX_BUFFER)
#define	ODIN_END				4.5
#define	ODIN_END_DF			ODIN_END*(SOUND_FACTOR/MAX_BUFFER)
#define	DVA_START			4.7													// "Два"
#define	DVA_START_DF		DVA_START*(SOUND_FACTOR/MAX_BUFFER)
#define	DVA_END				5.5
#define	DVA_END_DF			DVA_END*(SOUND_FACTOR/MAX_BUFFER)
#define	TRI_START			5.4													// "Три"
#define	TRI_START_DF		TRI_START*(SOUND_FACTOR/MAX_BUFFER)
#define	TRI_END				5.9
#define	TRI_END_DF			TRI_END*(SOUND_FACTOR/MAX_BUFFER)
#define	VKL_START			13.9													// "Включено"
#define	VKL_START_DF		VKL_START*(SOUND_FACTOR/MAX_BUFFER)
#define	VKL_END				14.6
#define	VKL_END_DF			VKL_END*(SOUND_FACTOR/MAX_BUFFER)
#define	VIKL_START			14.6													// "Выключено"
#define	VIKL_START_DF		VIKL_START*(SOUND_FACTOR/MAX_BUFFER)
#define	VIKL_END				15.5
#define	VIKL_END_DF			VIKL_END*(SOUND_FACTOR/MAX_BUFFER)
#define	VKLA_START			11.1													// "Включена"
#define	VKLA_START_DF		VKLA_START*(SOUND_FACTOR/MAX_BUFFER)
#define	VKLA_END				12
#define	VKLA_END_DF			VKLA_END*(SOUND_FACTOR/MAX_BUFFER)
#define	VIKLA_START			11.9													// "Выключена"
#define	VIKLA_START_DF		VIKLA_START*(SOUND_FACTOR/MAX_BUFFER)
#define	VIKLA_END			13.0
#define	VIKLA_END_DF		VIKLA_END*(SOUND_FACTOR/MAX_BUFFER)
#define	VKLN_START			16.8													// "Включен"
#define	VKLN_START_DF		VKLN_START*(SOUND_FACTOR/MAX_BUFFER)
#define	VKLN_END				17.6
#define	VKLN_END_DF			VKLN_END*(SOUND_FACTOR/MAX_BUFFER)
#define	VIKLN_START			17.6													// "Выключен"
#define	VIKLN_START_DF		VIKLN_START*(SOUND_FACTOR/MAX_BUFFER)
#define	VIKLN_END			18.5
#define	VIKLN_END_DF		VIKLN_END*(SOUND_FACTOR/MAX_BUFFER)
#define	SIGNAL_START		9.7													// "Сигнализация"
#define	SIGNAL_START_DF	SIGNAL_START*(SOUND_FACTOR/MAX_BUFFER)
#define	SIGNAL_END			11.1
#define	SIGNAL_END_DF		SIGNAL_END*(SOUND_FACTOR/MAX_BUFFER)
#define	EMODE_START			15.5													// "Экономичный режим"
#define	EMODE_START_DF		EMODE_START*(SOUND_FACTOR/MAX_BUFFER)
#define	EMODE_END			16.9
#define	EMODE_END_DF		EMODE_END*(SOUND_FACTOR/MAX_BUFFER)
#define	ERROR_START			18.5													// "Ошибка, повторите команду"
#define	ERROR_START_DF		ERROR_START*(SOUND_FACTOR/MAX_BUFFER)
#define	ERROR_END			20.5
#define	ERROR_END_DF		ERROR_END*(SOUND_FACTOR/MAX_BUFFER)

void DF_Playback(void);

void DF_Playback(void);

// ��������� �������
void DF_WriteSPI(unsigned char data);

void DF_WriteSPI(unsigned char data);

unsigned char DF_Status(void);

unsigned char DF_Status(void);

unsigned char DF_Ready(void);

unsigned char DF_Ready(void);

void DF_MainMemoryToSpeaker(unsigned int Page);

void DF_PagesToSpeaker(unsigned int StartPage, unsigned int EndPage);

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
}

void DF_WriteSPI(unsigned char data)								// ������ ����� �� SPI
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));										// ���� ��������� �������� ������
}

void DF_WriteSPI(unsigned char data)								// Запись байта по SPI
{
    SPDR = data;
    while (!(SPSR & 0x80));												// Ждем окончания передачи данных
}

unsigned char DF_Status(void)											// ������ ����� ������� DataFlash
{
    clrbit(PORTG, nFCS);													// Enable DataFlash

    DF_WriteSPI(DF_SREG);
    DF_WriteSPI(0xFF);

    setbit(PORTG, nFCS);													// Disable DataFlash
    return SPDR;
}

unsigned char DF_Status(void)											// Чтение байта статуса DataFlash
{
    //	clrbit(PORTG, nFCS);													// Enable DataFlash

    DF_WriteSPI(DF_SREG);
    DF_WriteSPI(0xFF);

    //	setbit(PORTG, nFCS);													// Disable DataFlash
    return SPDR;
}

unsigned char DF_Ready(void)											// ����������� ������ ����� Ready/~Busy
{
    return (DF_Status() & (1 << DF_BIT_READY));
}

unsigned char DF_Ready(void)											// Программный аналог ножки Ready/~Busy
{
    return (DF_Status() & (1 << DF_BIT_READY));
}

void DF_MainMemoryToSpeaker(unsigned int Page)
{
    unsigned long IntroAddress;
    unsigned char dummy = 0;
    unsigned int StartAddress = 0;

    clrbit(PORTG, nFCS);													// Enable DataFlash
    //	while(DF_Ready == 0);												// Ждем освобождения DataFlash

    DF_WriteSPI(DF_MM_RD);
    DF_WriteSPI((char)(Page >> 7));
    DF_WriteSPI((char)(Page << 1) | (StartAddress >> 8));
    DF_WriteSPI((char)(StartAddress));
    DF_WriteSPI(0x00);
    DF_WriteSPI(0x00);
    DF_WriteSPI(0x00);
    DF_WriteSPI(0x00);

    for (IntroAddress = 0; IntroAddress < MAX_BUFFER; IntroAddress++)
    {
        while (!(ACSR & T1_OVF));											// Ждем переполнения таймера
        DF_WriteSPI(0xFF);												// Пустышка
        dummy = SPDR;

        OCR1B = dummy;														// Передаёт данные на "проговаривание"
        ACSR &= (~T1_OVF);												// Сбрасываем флажок
    }

    setbit(PORTG, nFCS);													// Disable DataFlash
}

void DF_PagesToSpeaker(unsigned int StartPage, unsigned int EndPage)
{
    unsigned int CurrentPage;

    SPCR = 0x5C;															// Инициализация SPI

    TCCR1A = 0x21;															// 8 бит ШИМ, используем COM1B
    TCNT1 = 0x00;
    TIFR = 0x04;															// Очистка флага переполнения
    setbit(TIMSK, TOIE1);												// Разрешить прерывание по переполнению от таймера 1
    TCCR1B = 0x01;															// Предделитель счетчика 1 = 1
    OCR1B = 0x00;

    for (CurrentPage = StartPage; CurrentPage <= EndPage; CurrentPage++)
        DF_MainMemoryToSpeaker(CurrentPage);

    clrbit(TIMSK, TOIE1);												// Запретить прерывание по переполнению от таймера 1
    TCCR1B = 0x00;															// Остановить счетчик 1

    SPCR = 0x00;															// Disable SPI
}

#endif //CODE_DATAFLASH_H
