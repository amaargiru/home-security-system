//
// Created by emelyanov.my on 30.08.2023.
//

#ifndef CODE_INTERRUPTS_H
#define CODE_INTERRUPTS_H

#include "kedahm.h"
#include "guard.h"

__interrupt void TIMER0_OVF_Interrupt(void)
{
#define TmQuartSeconds 7									// 1/4 секунды при частоте переполнения таймера 30 Гц
#define TmHalfSeconds 15									// 1/2 секунды
#define TmFullSeconds 28									// 1 секунда. Не равно 30, т.к. д.б. равно N*TmQuartSeconds
#define TmTwoFullSeconds 56								// 2 секунды. Не равно 60, т.к. д.б. равно N*TmQuartSeconds, где N - целое число. Иначе условие if (Ticks % TmFullSSPeriod == 0) {if (Ticks % TmTwoFullSeconds == 0)...} (смотри ниже) будет выполняться не так часто, как хотелось бы :)
#define TmFullSSPeriod 160									// Один раз в столько тиков обновляется информация об уровне сигнала

    ++Ticks;

    if (Ticks % TmHalfSeconds == 0)							// Проверяем внешнее питание раз в полсекунды
        if (PowerCorrect == 0) invbit(PORTC, POWLED);	// При сбое внешнего питания - мигает с частотой 1 Гц
        else setbit(PORTC, POWLED);							// Если внешнее питание в норме - горит постоянно

    if (Ticks % TmFullSSPeriod == 0)							// Загрузка значения уровня сигнала и номера неверной записи на SIM-карте в подпрограмму индикации
    {
        TmSS = 2 * GSMSigStrength;
        clrbit(PORTC, GSMLED);
        TmErrRec = 2 * IncorrectAb;
        clrbit(PORTC, GRDLED);

        if (TotalHackLoops != 0)
        {
            TmIndLoop = 2 * IndicatedLoops[HackLoopsCount];

            if (HackLoopsCount == (TotalHackLoops - 1))
                HackLoopsCount = 0;
            else
                HackLoopsCount++;
        }
    }
    if (Ticks % TmQuartSeconds == 0)							// Проверяем состояние GSM-модуля раз в четверть секунды
    {
        if ((TotalHackLoops != 0) && (GuardSource == 0) && (TmIndLoop > 0))
        {
            invbit(PORTC, GRDLED); 															// Индикатор мигает
            TmIndLoop--;
        }
        else

            if ((AttemptInError == 1) && (Ticks % TmFullSeconds == 0))
                invbit(PORTC, GRDLED);														// Мигает с частотой 2 Гц после попытки поставить на охрану с неисправными шлейфами
            else

                if ((GuardSource != 0)														// Если систему уже поставили на охрану...
                    && (Ticks % TmTwoFullSeconds == 0))
                {
                    setbit(PORTC, GRDLED);												// ...то мигаем раз в две секунды
                    //						SetActiveUart(UART_0);												// TODO Remove debug
                    //						printf(">\n\r", PreparationGuardTime);							// TODO Remove debug
                    //						SetActiveUart(UART_1);												// TODO Remove debug
                }
                else

                    if ((GuardSource != 0)													// Если систему уже поставили на охрану...
                        && (Ticks % TmFullSeconds == 0)										// fw110
                        && ((PreparationGuardTime > 0) || (TmGuardOffTimeout > 0)))	// fw110
                    {
                        setbit(PORTC, GRDLED);											// ...то мигаем раз в секунду
                        //							SetActiveUart(UART_0);											// TODO Remove debug
                        //							printf("^\n\r", PreparationGuardTime);						// TODO Remove debug
                        //							SetActiveUart(UART_1);											// TODO Remove debug
                    }
                    else

                        if (TmErrRec > 0)														// ...или индицируем номер неверной записи в SIM-карте, если таковая имеется
                        {
                            invbit(PORTC, GRDLED);
                            TmErrRec--;
                        }
                        else	clrbit(PORTC, GRDLED);										// Индикатор потушен

        if (GSMStatus == 0) clrbit(PORTC, GSMLED);		// Не вставлена SIM-карта или GSM-модуль не может ее прочитать - гасим светодиод
        if (GSMStatus == 1) invbit(PORTC, GSMLED);		// Мигает с частотой 2 Гц
        if (GSMStatus == 2)										// Если GSM-модуль зарегистрирован в сети - показываем уровень сигнала
        {
            if (TmSS > 0)											// Индикатор мигает
            {
                invbit(PORTC, GSMLED);
                TmSS--;
            }
            else clrbit(PORTC, GSMLED);						// Индикатор потушен
        }
        if (GSMStatus == 3)										// Режим разговора - горит постоянно
            setbit(PORTC, GSMLED);
    }

    if ((GuardSource != 0) && (Ticks % TmFullSeconds == 0))	// Если система стоит на охране - проверяем истечение задержки при постановке на охрану
        if (PreparationGuardTime != 0)
        {
            PreparationGuardTime--;										// Если время задержки еще не истекло, мы его уменьшаем
            SetActiveUart(UART_0);										// TODO Remove debug
            printf("PGT=%d\n\r", PreparationGuardTime);			// TODO Remove debug
            SetActiveUart(UART_1);										// TODO Remove debug
        }

    if (Ticks % TmFullSeconds == 0)									// Проверяем состояние шлейфов в режиме охраны раз в секунду
    {
        if (TmRepeatGuardOn > 0)										// (fw104)
        {
            SetActiveUart(UART_0);										// TODO Remove debug
            printf("TRGO=%d\n\r", TmRepeatGuardOn);				// TODO Remove debug
            SetActiveUart(UART_1);										// TODO Remove debug

            TmRepeatGuardOn--;
        }
        else
        {
            Loop1Violation = 0;											// Вновь начинаем опрашивать нарушенные шлейфы (fw104)
            Loop2Violation = 0;
            Loop3Violation = 0;
            NewLoopViolation = 0;
            OldLoopViolation = 0;

            //			SetActiveUart(UART_0);										// TODO Remove debug
            //			printf("Loops recovered\n\r");							// TODO Remove debug
            //			SetActiveUart(UART_1);										// TODO Remove debug
        }

        if (TmSirenSingTime > 0)										// Это означает, что управление передано процедуре GuardReaxion и нужно включить сирену
        {
            setbit(PORTB, POUT1);
            TmSirenSingTime--;											// Сирене осталось работать на секунду меньше
        }
        else
            clrbit(PORTB, POUT1);

        if (TmLockOpenTime > 0)											// Это означает, что управление передано процедуре GuardOut и нужно открыть замок
        {
            setbit(PORTA, POUT2);
            TmLockOpenTime--;												// Замок будет открыт на секунду меньше

            //			SetActiveUart(UART_0);										// TODO Remove debug
            //			printf("O!\n\r", TmGuardOffTimeout);					// TODO Remove debug
            //			SetActiveUart(UART_1);										// TODO Remove debug
        }
        else
            clrbit(PORTA, POUT2);

        if (TmGuardOffTimeout > 0)										// Это означает, что шлейфы нарушены и система ожидает или снятия с охраны или времени начала комплекса охранных мероприятий
        {
            TmGuardOffTimeout--;											// Еще на секунду меньше

            SetActiveUart(UART_0);										// TODO Remove debug
            printf("TmGOT=%d\n\r", TmGuardOffTimeout);			// TODO Remove debug
            SetActiveUart(UART_1);										// TODO Remove debug
        }

        if (TmNoActiveTime > 0)											// Это означает, что систему сняли с охраны и некоторое время вновь ввести в режим охраны ее нельзя (fw101)
            TmNoActiveTime--;												// Еще на секунду меньше

        LoopsRead();														// Определение состояния охранных шлейфов (fw104)
        LoopResult = LoopTest();										// (fw104)
        if (LoopResult == 0)
            TmRepeatGuardOn = 3 * 60;										// Три минуты. Тестируем шлейфы даже при невключенной охране (fw104)

        if ((GuardSource != 0) && (PreparationGuardTime == 0))// Если система поставлена на охрану и задержка при постановке на охрану истекла
        {
            if (LoopResult == 1)											// Со шлейфами все в порядке
            {
                SetActiveUart(UART_0);									// TODO Remove debug
                printf("*\n\r");											// TODO Remove debug
                SetActiveUart(UART_1);									// TODO Remove debug
            }
            else																// Нарушение охранных шлейфов
            {
                SetActiveUart(UART_0);									// TODO Remove debug
                printf("Alarm!\n\r");									// TODO Remove debug
                SetActiveUart(UART_1);									// TODO Remove debug

                TmRepeatGuardOn = 3 * 60;									// Три минуты

                OldLoopViolation = NewLoopViolation;

                OldLoop1Violation = Loop1Violation;
                OldLoop2Violation = Loop2Violation;
                OldLoop3Violation = Loop3Violation;

                if (GuardSourceL1 == 0) Loop1Violation = 1;		// Определяем номера нарушенных шлейфов
                if (GuardSourceL2 == 0) Loop2Violation = 1;
                if (GuardSourceL3 == 0) Loop3Violation = 1;

                NewLoopViolation = Loop1Violation + Loop2Violation + Loop3Violation;

                //				SetActiveUart(UART_0);									// TODO Remove debug
                //				printf("NLV=%d\n\r", NewLoopViolation);			// TODO Remove debug
                //				printf("OLV=%d\n\r", OldLoopViolation);			// TODO Remove debug
                //				SetActiveUart(UART_1);									// TODO Remove debug

                if (NewLoopViolation > OldLoopViolation)
                {
                    LoopViolation = 1;									// Приключилось новенькое нарушение. LoopViolation = 0 только после отработки GuardReaxion() [или в процедуре снятия с охраны (fw109)]
                    if ((GuardSource == GS_IBUTTON) || ((GuardSource == GS_SWITCHER) && (iButtonUse == 1)))
                    {
                        TmGuardOffTimeout = 0;							// Немедленная реакция (fw101)
                        SetActiveUart(UART_0);							// TODO Remove debug
                        printf("IMMEDIATE-ACTION ALARM!!!\n\r");	// TODO Remove debug fw115
                        SetActiveUart(UART_1);							// TODO Remove debug
                    }
                    else
                        TmGuardOffTimeout = DelayGuardOffTime;			// Определяем время задержки до выполнения комплекса охранных мероприятий (на случай, если шлейф нарушен человеком, который пришел снимать сигнализацию с охраны)
                }

                if (OldLoop1Violation != Loop1Violation)
                {
                    IndicatedLoops[TotalHackLoops] = 1;
                    TotalHackLoops++;										// Нарушен еще один шлейф (в данном случае - первый)
                }
                if (OldLoop2Violation != Loop2Violation)
                {
                    IndicatedLoops[TotalHackLoops] = 2;
                    TotalHackLoops++;										// Нарушен еще один шлейф (в данном случае - второй)
                }
                if (OldLoop3Violation != Loop3Violation)
                {
                    IndicatedLoops[TotalHackLoops] = 3;
                    TotalHackLoops++;										// Нарушен еще один шлейф (в данном случае - третий)
                }

                //				SetActiveUart(UART_0);										// TODO Remove debug
                //				printf("THL = %d\n\r", TotalHackLoops);				// TODO Remove debug
                //				SetActiveUart(UART_1);										// TODO Remove debug

                SetActiveUart(UART_0);									// TODO Remove debug
                printf("Viol = %d %d %d = %d\n\r", Loop1Violation, Loop2Violation, Loop3Violation, LoopViolation);	// TODO Remove debug
                SetActiveUart(UART_1);									// TODO Remove debug
            }
        }//Контроль шлейфов в режиме охраны
    }
}

__interrupt void OutVoiceSample(void)
{
    Timer1OverCount++;

    if (Timer1OverCount == 2)
    {
        Timer1OverCount = 0;
        ACSR |= T1_OVF;													// Устанавливаем флаг "Байт воспроизведен"
    }
}

__interrupt void INT4_vect_Interrupt(void)
{
    //	SetActiveUart(UART_0);										// TODO Remove debug
    //	printf("NEXT KEY\n\r");										// TODO Remove debug
    //	SetActiveUart(UART_1);										// TODO Remove debug

    if (DTMFPassPos < DTMF_PASS_LENGTH)						// На всякий случай проверяем диапазон
    {
        DTMFPassBuf[DTMFPassPos] = ((PINE >> DTQ1) & 0x01) | ((PINE >> (DTQ2 - 1)) & 0x02) | ((PINE >> (DTQ3 - 2)) & 0x04) | ((PINE >> (DTQ4 - 3)) & 0x08);	// "Сплющиваем" информацию от MT8870 в число
        if (DTMFPassBuf[DTMFPassPos] == 10) DTMFPassBuf[DTMFPassPos] = 0;	// В DTMF 0 кодируется как 1010b = 10d
        DTMFPassPos++;

        //		SetActiveUart(UART_0);									// TODO Remove debug
        //		printf("DTMFPassBuf = %d %d %d %d \n\r", DTMFPassBuf[0], DTMFPassBuf[1], DTMFPassBuf[2], DTMFPassBuf[3]);	// TODO Remove debug
        //		SetActiveUart(UART_1);									// TODO Remove debug
    }
}

#endif //CODE_INTERRUPTS_H
