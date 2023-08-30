#ifndef CODE_IBUTTON_H
#define CODE_IBUTTON_H

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

unsigned char iButtonKeyScan(void);

void iButtonNewKeysSearch(void);

void iButtonKeysErase(void);

void iButtonUpdate(void)							// Определяет режим работы с ключами iButton - запись, стирание, нормальный режим
{
    SetActiveUart(UART_0);							// TODO Remove debug
    if ((PINA & (1 << JIBP)) == 0)					// Режим записи ключей iButton
    {
        iButtonMode = 1;
        //		printf("IB: write keys\r");				// TODO Remove debug
    }
    else
        if ((PINA & (1 << JIBM)) == 0)				// Режим стирания ключей iButton
        {
            iButtonMode = 2;
            //			printf("IB: erase keys\n\r");			// TODO Remove debug
        }
        else												// Режим обычного функционирования iButton
        {
            iButtonMode = 0;
            //			printf("IB: normal mode \n\r");		// TODO Remove debug
        }
    SetActiveUart(UART_1);							// TODO Remove debug
}

unsigned char iButtonKeyScan(void)									// Возвращает 1, если приложен один из записанных в памяти ключей или 0, если такого ключа не обнаружено
{
    unsigned char uidCounter;

    SetActiveUart(UART_0);												// TODO Remove debug
    ds1990a_init();														// Чтение ID ключа происходит в случае отклика 1-Wire шины

    if ((CRC8(uidDS, 7) == uidDS[7]) && (uidDS[0] == DS1990A_ID))
        for (uidCounter = 0; uidCounter <= CurrentKeys; uidCounter++)
            if ((uidDS[0] == ibKeys[uidCounter][0]) && (uidDS[1] == ibKeys[uidCounter][1])	// Если такой ключ записан в памяти ключей
                && (uidDS[2] == ibKeys[uidCounter][2]) && (uidDS[3] == ibKeys[uidCounter][3])
                && (uidDS[4] == ibKeys[uidCounter][4]) && (uidDS[5] == ibKeys[uidCounter][5])
                && (uidDS[6] == ibKeys[uidCounter][6]))
            {
                printf("Key incoming ");
                return 1;
            }

    SetActiveUart(UART_1);												// TODO Remove debug
    return 0;
}

void iButtonNewKeysSearch(void)
{
    SetActiveUart(UART_0);												// TODO Remove debug
    setbit(PORTC, GRDLED);
    ds1990a_init();														// Чтение ID ключа происходит в случае отклика 1-Wire шины
    RepeatedKey = 0;

    if ((CRC8(uidDS, 7) == uidDS[7]) && (uidDS[0] == DS1990A_ID))
    {
        printf("New key incoming ");
        printf("ROM = %d %d %d %d %d %d %d %d CRC8 = %d\n\r", uidDS[0], uidDS[1], uidDS[2], uidDS[3], uidDS[4], uidDS[5], uidDS[6], uidDS[7], CRC8(uidDS, 7));

        for (temp = 0; temp <= CurrentKeys; temp++)
            if (((uidDS[0] == ibKeys[temp][0]) && (uidDS[1] == ibKeys[temp][1])	// Если такой ключ уже есть или если ключей больше 20
                && (uidDS[2] == ibKeys[temp][2]) && (uidDS[3] == ibKeys[temp][3])
                && (uidDS[4] == ibKeys[temp][4]) && (uidDS[5] == ibKeys[temp][5])
                && (uidDS[6] == ibKeys[temp][6])) || (CurrentKeys == MAX_IB_KEYS))
                RepeatedKey = 1;

        if (RepeatedKey == 0)
        {
            for (temp1 = 0; temp1 <= 7; temp1++)
                ibKeys[CurrentKeys][temp1] = uidDS[temp1];
            CurrentKeys++;
            printf("NEW KEY SAVED\n\r");														// TODO Remove debug
            clrbit(PORTC, GRDLED);																// Погасим GRDLED на три секунды, чтобы дать пользователю понять, что новый ключ записан
            delay_ms(3000);
            setbit(PORTC, GRDLED);
        }
        else
        {
            printf("Key repeated\n\r");														// TODO Remove debug
            for (temp1 = 0; temp1 <= 4 * 5; temp1++)											// Мигаем GRDLED'ом пять секунд, чтобы дать пользователю понять, что текущий ключ уже присутствует в базе
            {
                delay_ms(250);
                invbit(PORTC, GRDLED);
            }
        }
    }
    else
    {
        printf("Current Keys = %d\r", CurrentKeys);										// TODO Remove debug
    }
    clrbit(PORTC, GRDLED);
    SetActiveUart(UART_1);																		// TODO Remove debug
}

void iButtonKeysErase(void)							// Удаление ключей iButton из EEPROM
{
    SetActiveUart(UART_0);								// TODO Remove debug
    setbit(PORTC, GRDLED);
    if (CurrentKeys != 0)								// Иначе и удалять нечего
    {
        for (temp = 0; temp <= 4 * 3; temp++)			// Мигаем GRDLED'ом три секунды, чтобы дать пользователю понять, что ключи стираются
        {
            delay_ms(250);
            invbit(PORTC, GRDLED);
        }
        CurrentKeys = 0;										// На самом деле стираем все ключи
        for (temp = 0; temp < MAX_IB_KEYS; temp++)
            for (temp1 = 0; temp1 < IB_KEY_LENGTH; temp1++)
                ibKeys[temp][temp1] = 0xFF;
        //		printf("\n\rKeys deleted\n\r");					// TODO Remove debug
    }
    //	else															// TODO Remove debug
    //		printf("No keys \r");								// TODO Remove debug

    clrbit(PORTC, GRDLED);

    SetActiveUart(UART_1);									// TODO Remove debug
}

#endif //CODE_IBUTTON_H
