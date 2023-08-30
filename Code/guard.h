#ifndef CODE_GUARD_H
#define CODE_GUARD_H

#include "dataflash.h"
#include "ibutton.h"
#include "ds1990a.h"            // Работа с ключами iButton
#include "eeprom.h"            // Описание переменных в EEPROM
#include "madelay.h"            // Задержки
#include "portdefines.h"    // "Распиновка" портов
#include "bitbyt.h"            // Определение операций с битами
#include "kedahm.h"
#include "guard.h"
#include <string.h>            // Операции со строками
#include <ctype.h>            // Операции с символами
#include <stdio.h>            // Стандартный ввод-вывод
#include <inavr.h>            // Intrinsic-функции
#include <iom128.h>            // Определения внутренних регистров

void GuardIn(unsigned char InGuardSource);

void GuardOut(unsigned char OutGuardSource);

void LoopsRead(void);

char LoopTest(void);

void GuardIn(unsigned char InGuardSource)															// Единая точка входа в режим охраны
{
    SetActiveUart(UART_0);																					// TODO Remove debug
    printf("Guard ON attempt ");																			// TODO Remove debug

    if ((LoopTest() == 1)	 																				// Если шлейфы подключены верно
        && (GuardSource == 0))																				// Если на охрану еще не поставлено
        switch (InGuardSource)
        {
        case GS_SWITCHER:
            if (TmNoActiveTime == 0)																	// (fw101)
            {
                printf("Switcher\n\r");																	// TODO Remove debug
                Loop1Violation = 0;																		// Очистка информации о нарушенных шлейфах
                Loop2Violation = 0;
                Loop3Violation = 0;
                TotalHackLoops = 0;
                LoopViolation = 0;																		// Очищаем переменную, говорящую о срабатывании охранной системы (fw109)
                GuardSource = 1;
                AttemptInError = 0;
                clrbit(PORTC, GRDLED);																	// Гасим, т.к. он мог остаться зажженным после прерывания таймера
                PreparationGuardTime = DelayGuardOnTime * 60;										// Задержка при постановке на охрану приравнивается задержке, считанной из сервисной записи SIM-карты
                SendMessageToExtUser(" GUARD ON");													// fw114
            }
            else
                printf("Warning: iButton timeout not finished\n\r");							// TODO Remove debug
            break;
        case GS_IBUTTON:
            printf("iButton\n\r");																		// TODO Remove debug
            Loop1Violation = 0;																			// Очистка информации о нарушенных шлейфах
            Loop2Violation = 0;
            Loop3Violation = 0;
            TotalHackLoops = 0;
            LoopViolation = 0;																			// Очищаем переменную, говорящую о срабатывании охранной системы (fw109)
            GuardSource = 4;
            AttemptInError = 0;
            clrbit(PORTC, GRDLED);																		// Гасим, т.к. он мог остаться зажженным после прерывания таймера
            PreparationGuardTime = DelayGuardOnTime * 60;
            SendMessageToExtUser(" GUARD ON");														// fw114
            break;
        case GS_SMS:
            printf("SMS\n\r");																			// TODO Remove debug
            Loop1Violation = 0;																			// Очистка информации о нарушенных шлейфах
            Loop2Violation = 0;
            Loop3Violation = 0;
            TotalHackLoops = 0;
            LoopViolation = 0;																			// Очищаем переменную, говорящую о срабатывании охранной системы (fw109)
            GuardSource = 2;
            AttemptInError = 0;
            clrbit(PORTC, GRDLED);																		// Гасим, т.к. он мог остаться зажженным после прерывания таймера
            PreparationGuardTime = DelayGuardOnTime * 60;
            break;
        case GS_DTMF:
            printf("DTMF\n\r");																			// TODO Remove debug
            Loop1Violation = 0;																			// Очистка информации о нарушенных шлейфах
            Loop2Violation = 0;
            Loop3Violation = 0;
            TotalHackLoops = 0;
            LoopViolation = 0;																			// Очищаем переменную, говорящую о срабатывании охранной системы (fw109)
            GuardSource = 3;
            AttemptInError = 0;
            clrbit(PORTC, GRDLED);																		// Гасим, т.к. он мог остаться зажженным после прерывания таймера
            PreparationGuardTime = DelayGuardOnTime * 60;
            break;
            //			default:
            //				printf("- ERROR: unknown source !!!\n\r");											// TODO Remove debug
        }
    else
    {
        printf("- Warning: attempt fail. Source = %d\n\r", InGuardSource);					// TODO Remove debug
    }

    if ((LoopTest() == 0)	 																				// Если и шлейфы подключены неверно
        && (GuardSource == 0))																				// И если на охрану еще не поставлено
        AttemptInError = 1;

    SetActiveUart(UART_1);																					// TODO Remove debug
}

void GuardOut(unsigned char OutGuardSource)															// Единая точка выхода из режима охраны
{
    SetActiveUart(UART_0);																					// TODO Remove debug
    printf("Guard OFF attempt ");																			// TODO Remove debug

    if (GuardSource != 0)
        switch (OutGuardSource)
        {
        case GS_SWITCHER:
            if (iButtonUse == 0)
            {
                if (GuardSource == GS_SWITCHER)
                {
                    printf("Switcher\n\r");																// TODO Remove debug
                    TmLockOpenTime = LockOpenTime;
                    GuardSource = 0;
                    SendMessageToExtUser(" GUARD OFF");												// fw114
                }
                else																							// Система была поставлена на охрану не тумблером, им снимать нельзя
                {
                    printf(":Warning. Switcher is not allowed\n\r");							// TODO Remove debug
                }
            }
            else																								// В случае, если iButtonUse == 1, снять систему с охраны тумблером нельзя
            {
                printf(":Warning. iButtonUse = 1\n\r");											// TODO Remove debug
            }
            break;
        case GS_IBUTTON:
            printf("iButton\n\r");																		// TODO Remove debug
            TmLockOpenTime = LockOpenTime;
            if (GuardSource == GS_SWITCHER)
                TmNoActiveTime = DelayNoActiveTime;
            GuardSource = 0;
            SendMessageToExtUser(" GUARD OFF");														// fw114
            break;
        case GS_SMS:
            if (GuardSource != GS_SWITCHER)
            {
                GuardSource = 0;
                printf("SMS\n\r");																		// TODO Remove debug
                TmLockOpenTime = LockOpenTime;
            }
            else
                printf("- SMS ERROR\n\r");
            break;
        case GS_DTMF:
            if (GuardSource != GS_SWITCHER)
            {
                GuardSource = 0;
                printf("DTMF\n\r");																		// TODO Remove debug
                TmLockOpenTime = LockOpenTime;
            }
            else
                printf("- DTMF ERROR\n\r");
            break;
            //			default:
            //				printf("- ERROR: unknown source !!!\n\r");											// TODO Remove debug
        }
    else
        printf("- Warning: attempt fail. Source = %d\n\r", OutGuardSource);					// TODO Remove debug

    SetActiveUart(UART_1);																					// TODO Remove debug
}

void LoopsRead(void)
{
    unsigned int LoopCode;

    if (Loop1On > 0)									// Если 1-й шлейф задействован
    {
        LoopCode = ReadADC(LIN1);					// Напряжение 1-го охранного шлейфа
        if (LoopCode < LO_LOOP_LIMIT)
            Loop1Status = 2;							// Обрыв
        else
            if ((LoopCode > LO_LOOP_LIMIT) && (LoopCode < HI_LOOP_LIMIT))
                Loop1Status = 0;						// Норма
            else
                Loop1Status = 1;						// КЗ
    }//Loop1On

    if (Loop2On > 0)									// Если 2-й шлейф задействован
    {
        LoopCode = ReadADC(LIN2);					// Напряжение 2-го охранного шлейфа
        if (LoopCode < LO_LOOP_LIMIT)
            Loop2Status = 2;							// Обрыв
        else
            if ((LoopCode > LO_LOOP_LIMIT) && (LoopCode < HI_LOOP_LIMIT))
                Loop2Status = 0;						// Норма
            else
                Loop2Status = 1;						// КЗ
    }//Loop2On

    if (Loop3On > 0)									// Если 3-й шлейф задействован
    {
        LoopCode = ReadADC(LIN3);					// Напряжение 3-го охранного шлейфа

        if (LoopCode < LO_LOOP_LIMIT)
            Loop3Status = 2;							// Обрыв
        else
            if ((LoopCode > LO_LOOP_LIMIT) && (LoopCode < HI_LOOP_LIMIT))
                Loop3Status = 0;						// Норма
            else
                Loop3Status = 1;						// КЗ
    }//Loop3On
}

char LoopTest(void)			// Тестируем состояние охранных шлейфов. Если состояние нормальное, возвращает 1, иначе 0
{
    if (Loop1On == 1)												// Линия работает на замыкание
        if ((Loop1Status == 0) || (Loop1Status == 2))	// Норма или обрыв
            GuardSourceL1 = 1;
        else GuardSourceL1 = 0;
    if (Loop1On == 2)												// Линия работает на размыкание
        if ((Loop1Status == 0) || (Loop1Status == 1))	// Норма или КЗ
            GuardSourceL1 = 1;
        else GuardSourceL1 = 0;

    if (Loop2On == 1)												// Линия работает на замыкание
        if ((Loop2Status == 0) || (Loop2Status == 2))	// Норма или обрыв
            GuardSourceL2 = 1;
        else GuardSourceL2 = 0;
    if (Loop2On == 2)												// Линия работает на размыкание
        if ((Loop2Status == 0) || (Loop2Status == 1))	// Норма или КЗ
            GuardSourceL2 = 1;
        else GuardSourceL2 = 0;

    if (Loop3On == 1)												// Линия работает на замыкание
        if ((Loop3Status == 0) || (Loop3Status == 2))	// Норма или обрыв
            GuardSourceL3 = 1;
        else GuardSourceL3 = 0;
    if (Loop3On == 2)												// Линия работает на размыкание
        if ((Loop3Status == 0) || (Loop3Status == 1))	// Норма или КЗ
            GuardSourceL3 = 1;
        else GuardSourceL3 = 0;

    // Если все три линии не используются или хотя бы один из используемых шлейфов неисправен - на охрану ставить нельзя
    if ((GuardSourceL1 == 1) && (GuardSourceL2 == 1) && (GuardSourceL3 == 1) &&
        ((Loop1On != 0) || (Loop2On != 0) || (Loop3On != 0))) return 1;
    else return 0;
}

#endif //CODE_GUARD_H
