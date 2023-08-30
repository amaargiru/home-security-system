// Табуляция в тексте программы = 3 символа
#ifndef _kedahm_
#define _kedahm_

#include <iom128.h>			// Определения внутренних регистров
#include <inavr.h>			// Intrinsic-функции
#include <stdio.h>			// Стандартный ввод-вывод
#include <ctype.h>			// Операции с символами
#include <string.h>			// Операции со строками
#include "bitbyt.h"			// Определение операций с битами
#include "portdefines.h"	// "Распиновка" портов
#include "madelay.h"			// Задержки
#include "eeprom.h"			// Описание переменных в EEPROM
#include "ds1990a.h"			// Работа с ключами iButton
#include "guard.h"
#include "ibutton.h"
#include "dataflash.h"

#define ADC_VREF_TYPE 0x00	// Выбор опорного напряжения АЦП. 0x00 -> вывод VREF

#define TXDELAY		10000	// Задержка при выводе символа через USART

#define CtrlZ			0x1A	// Код Ctrl-Z для GSM-модуля

#define T1_OVF  		0x01
#define DF_BIT_READY	7

#define MAX_BUFFER   264

unsigned int temp = 0;		// Временные переменные под всякие служебные нужды
unsigned int temp1 = 0;
char LoopResult = 0;
unsigned char InboxSMS = 0;		// fw116

unsigned char RepeatedKey = 0;	// RepeatedKey = 0, если текущий ключ, который пытаются записать, еще отсутствует в памяти ключей

// UART0
#define DATA_REGISTER_EMPTY_0 (1<<UDRE0)

// UART1
#define DATA_REGISTER_EMPTY_1 (1<<UDRE1)
#define RXBUFLENGTH_1			256		// Длина приемного буфера UART1
volatile char RxBuffer[RXBUFLENGTH_1];	// Циклический буфер приемника
volatile char RxBufWritePoint = 0;		// Сюда пишем
char RxBufReadPoint = 0;					// Отсюда читаем
char ReceiveString[RXBUFLENGTH_1] = "";// Строка ответа, создается процедурой StringBuilder

// Переменные, связанные с абонентами охранной системы
char AbRecReadComplete = 0;							// 0 - записи абонентов не прочитаны или прочитаны и ошибочны; 1 - записи прочитаны и не имеют ошибок
#define MAX_AB_NUMBER		5							// Число абонентов
#define MAX_NUMBER_LENGTH	15							// Максимальная длина телефонного номера абонента
#define MAX_RECORD_LENGTH	17							// Максимальная длина записи абонента
#define MAX_NAME_LENGTH		8							// Максимальная длина имени абонента
#define MAX_PASS_LENGTH		3							// Длина пароля
#define SERVICE_NUM_LENGTH	5							// Длина сервисной записи

char AbNumber[MAX_AB_NUMBER][MAX_NUMBER_LENGTH + 1];// Массив номеров абонентов
char AbRecord[MAX_AB_NUMBER][MAX_RECORD_LENGTH + 1];// Массив записей абонентов
char AbName[MAX_AB_NUMBER][MAX_NAME_LENGTH + 1];	// Массив имен абонентов
char AbPass[MAX_AB_NUMBER][MAX_PASS_LENGTH + 1];	// Массив паролей абонентов
char GuestPass[MAX_PASS_LENGTH + 1];
char AbProfile[MAX_AB_NUMBER];							// Массив профилей абонентов
char AbCallTime[MAX_AB_NUMBER];							// Время дозвона
char AbCallNum[MAX_AB_NUMBER];							// Количество попыток дозвона
char AbExtService[MAX_AB_NUMBER];							// Включение дополнительного сервиса
char AbActive[MAX_AB_NUMBER];							// Признак наличия данного абонента в телефонной книге

unsigned char IncorrectAb = 0;								// Номер неверной телефонной записи на SIM-карте. Если все записи верны, IncorrectAb = 0

// Переменные, отвечающие за отсылку SMS "220V OFF" и "220V ON"
char Sending220VOFF = 0;	// Сообщение "220V OFF" не отсылалось
char Sending220VON = 1;		// Сообщение "220V ON" отсылалось

// Переменная, отвечающая за отсылку SMS "POWER OFF"
char SendingPOWOFF = 0;		// Сообщение "POWER OFF" не отсылалось

// Сервисные переменные, считываются с SIM-карты, при неисправности GSM-модуля/SIM-карты действуют настройки по умолчанию
char DelayGuardOnTime = 3;		// Задержка при постановке на охрану, в минутах
char DelayGuardOffTime = 30;	// Задержка при снятии с охраны, в секундах
char DelayNoActiveTime = 30;	// В течении этого времени нельзя вновь поставить систему на охрану после снятия. Это касается только некоторых режимов (fw101)
char iButtonUse = 0;				// Использование ключей iButton
char LockOpenTime = 9;			// Время удержания электромеханического замка, в секундах
char SirenSingTime = 90;		// Время работы сирены, в секундах

// Системные переменные
char GuardSource = 0;		// Источник постановки на охрану: 0 - на охрану не поставлено; 1 - постановка тумблером; 2 - постановка посредством SMS; 3 - постановка прямым звонком; 4 - постановка ключом iButton

char PreparationGuardTime = 0;
//unsigned char SMSandDTMFGuardOffCounter = 0;	// Отсчет секунд с момента отключения сигнализации SMS-сообщением или DTMF-кодом. После такого отключения необходимо в течение минуты ждать, не произошло ли нарушение шлейфа, и если не произошло - вновь перейти в режим охраны.

#define GS_SWITCHER	1
#define GS_SMS			2
#define GS_DTMF		3
#define GS_IBUTTON	4

char PowerCorrect = 1;		// Состояние внешнего питания: 1 - питание в норме; 0 - сбой внешнего питания

char GSMStatus = 0;			// Состояние GSM-модуля: 0 - выключен; 1 - не зарегистрирован в сети; 2 - зарегистрирован в сети; 3 - режим разговора
char GSMSigStrength = 0;	// Уровень сигнала
char SIMNum = 0;				// Номер текущей SIM-карты: 0 - задействована SIM-карта 1; 1 - задействована SIM-карта 2

char Loop1On = 0;				// Включение охранного шлейфа 1: 0 - не задействован; 1 - задействован и работает на замыкание; 2 - задействован и работает на размыкание
char Loop2On = 0;				// Включение охранного шлейфа 2
char Loop3On = 0;				// Включение охранного шлейфа 3
char Loop1Status = 0;		// Состояние охранного шлейфа 1: 0 - норма; 1 - КЗ; 2 - обрыв
char Loop2Status = 0;		// Состояние охранного шлейфа 2
char Loop3Status = 0;		// Состояние охранного шлейфа 3
#define HI_LOOP_LIMIT 527	// Верхняя граница рабочей зоны токовых петель. Соответствует суммарному сопротивлению линии 1 кОм
#define LO_LOOP_LIMIT 304	// Нижняя граница рабочей зоны токовых петель. Соответствует суммарному сопротивлению линии 2 кОм
char Loop1Violation = 0;	// Если Loop1Violation == 1, шлейф 1 был нарушен
char Loop2Violation = 0;	// Если Loop2Violation == 1, шлейф 2 был нарушен
char Loop3Violation = 0;	// Если Loop3Violation == 1, шлейф 3 был нарушен
char NewLoopViolation = 0;	// Если NewLoopViolation != OldLoopViolation, значит один из шлейфов был нарушен впервые
char OldLoopViolation = 0;
char OldLoop1Violation = 0;
char OldLoop2Violation = 0;
char OldLoop3Violation = 0;
char LoopViolation = 0;
char GuardSourceL1 = 1;		// Состояние 1-го охранного шлейфа позволяет перевести систему в режим охраны
char GuardSourceL2 = 1;		// Состояние 2-го охранного шлейфа позволяет перевести систему в режим охраны
char GuardSourceL3 = 1;		// Состояние 3-го охранного шлейфа позволяет перевести систему в режим охраны

char TotalHackLoops = 0;	// Всего нарушено шлейфов
char HackLoopsCount = 0;	// Счетчик нарушенных шлейфов (для индикации нарушенных шлейфов)
#define NLOOPS 3				// Общее количество шлейфов
char IndicatedLoops[NLOOPS] = { 0, 0, 0 };	// Массив номеров нарушенных шлейфов, которые нужно индицировать

#define  ACC_CORRECT  499	// При достижении AccVoltage границы ACC_CORRECT аккумулятор переводится в режим капельной зарядки
#define  ACC_FAULT    439	// При достижении AccVoltage границы ACC_FAULT в отсутствие внешнего питания система переводится в малопотребляющий режим

#define SMS_TIMEOUT 30		// Таймаут в секундах при отправке SMS
char EconoMode = 0;			// Режим энергопотребления: 0 - обычный режим; 1 - малопотребляющий режим
char SMSorDTMFEconoMode = 0;	// Принудительный малопотребляющий режим: 0 - нет принудительного перехода в малопотребляющий режим; 1 - требование принудительного перехода в малопотребляющий режим при отсутствии внешнего питания

char iButtonMode = 0;		// Режим работы с ключами iButton: 0 - обычный; 1 - запись ключей; 2 - стирание ключей

unsigned int CurrentADCRead = 0;	// Значение, считанное с АЦП

typedef enum { UART_0, UART_1 } t_uarts;				// Выбор UART'а
static t_uarts g_currentUsart = UART_1;

void BeginInit();											// Начальная инициализация периферии микроконтроллера
unsigned int ReadADC(unsigned char ADCInput);	// Чтение канала АЦП
void SetActiveUart(t_uarts u) 						// Выбор рабочего UART
{
    g_currentUsart = u;
}
int  putchar(int data);								// Вывод байта в UART (для printf())
void ClearRxBuffer(void);								// Очистка приемного буфера UART1
void SystemUpdate(void);								// Переопределение состояния системы
void GR_On(void);											// Включаем GR-47
void GR_Off(void);										// Выключаем GR-47
void StringBuilder(void);								// Делает строчку из данных в приемном буфере
char ReadAbRecords(void);								// Чтение записей абонентов. Если записи прочитаны и не имеют ошибок, возвращаемое значение == 1, иначе == 0
// Тестируем состояние охранных шлейфов. Если состояние нормальное, возвращает 1, иначе 0
int StrToInt(char* InputStr);						// Процедура конвертирует строку в число типа int. Конвертируются только цифры
//char* ToUpperString(char* UpString);				// Преобразует строку в верхний регистр
void SendMessageToExtUser(char* MessageStr);	// Отсылка сообщений пользователям, у которых включен дополнительный сервис. Если дополнительный сервис не включен ни у одного абонента, рассылки просто не будет. fw111

// Распознавание и запись в EEPROM новых ключей iButton
// Удаление ключей iButton из EEPROM
// Возвращает 1, если приложен один из записанных в памяти ключей или 0, если такого ключа не обнаружено

// Запись байта по SPI
// Чтение байта статуса DataFlash
// Программный аналог ножки Ready/~Busy
void DF_NextPageToNextBuffer(unsigned char active_buffer, unsigned int page_counter);

void DF_ActiveBufferToSpeaker(unsigned char active_buffer);

// Единая точка входа в режим охраны
// Единая точка выхода из режима охраны

void GuardReaxion(void);								// Реакция на срабатывание охранной системы

void BeginInit(void)
{
    //	Инициализация порта A
    DDRA = (1 << POUT2);
    PORTA = (1 << JIBP) | (1 << JIBM);	// Для включения pull-up

    //	Инициализация порта B
    DDRB = ((1 << SCK) | (1 << SI) | (1 << PWDN) | (1 << CHARG) | (1 << SPKOUT) | (POUT1));
    PORTB = 0;

    //	Инициализация порта C
    DDRC = ((1 << SIMSW) | (1 << EGRDLED) | (1 << EGSMLED) | (1 << EPOWLED) | (1 << GRDLED) | (1 << GSMLED) | (1 << POWLED));
    PORTC = 0;

    //	Инициализация порта D
    DDRD = ((1 << PON) | (1 << PDTR) | (1 << PTD) | (1 << PRTS));
    PORTD = 0;

    //	Инициализация порта E
    DDRE = (1 << CON) | (1 << TXD0);
    PORTE = (1 << CON);												// Включаем токовые петли

    //	Инициализация порта F
    DDRF = 0;
    PORTF = 0;

    //	Инициализация порта G
    DDRG = ((1 << nFCS) | (1 << nFRES) | (1 << nGSMPOW) | (1 << K1) | (1 << K2));
    PORTG = (1 << nFRES);

    // Инициализация АЦП
    ADMUX = ADC_VREF_TYPE;
    ADCSRA = 0x87;
    SFIOR &= 0xEF;

    // Инициализация USART0
    UCSR0A = 0x00;
    UCSR0B = 0x08;
    UCSR0C = 0x06;
    UBRR0H = 0x00;
    UBRR0L = 0x33;

    // Инициализация USART1
    UCSR1A = 0x00;
    UCSR1B = 0x98;
    UCSR1C = 0x06;
    UBRR1H = 0x00;
    UBRR1L = 0x33;

    // Инициализация таймера
    TCCR0 = 7;			// Тактирование от системного клока, с предделителем 1024 (время переполнения 30 мс @ 8.0 МГц)
    TIMSK = 1 << TOIE0;	// Разрешить прерывание по переполнению от таймера 0 (OCIE0!!!)

    // Инициализация внешнего прерывания - INT4 срабатывает по фронту
    EICRA = 0x00;
    EICRB = 0x03;
    EIMSK = 0x10;
    EIFR = 0x10;

    // Инициализация EEPROM. Если в EEPROM записаны 0xFF, значит программа запущена в первый раз и значение Loop1Set нужно перевести в 1 (нормально разомкнут, работает на замыкание). Такой подход позволяет обойтись без файла прошивки EEPROM, про который при серийном производстве могут и забыть.
    if (Loop1Set == 0xFF)
        Loop1Set = 1;
    if (Loop2Set == 0xFF)
        Loop2Set = 1;
    if (Loop3Set == 0xFF)
        Loop3Set = 1;
    if (CurrentKeys == 0xFF)
        CurrentKeys = 0;
}//BeginInit

unsigned int ReadADC(unsigned char ADCInput)	// Чтение канала АЦП
{
    ADMUX = ADCInput | ADC_VREF_TYPE;
    ADCSRA |= 0x40;
    while ((ADCSRA & 0x10) == 0);
    ADCSRA |= 0x10;
    return (unsigned int)(ADCL | (ADCH << 8));
}//ReadADC

int putchar(int data)	// Вывод байта в UART (для printf())
{
    if (g_currentUsart == UART_1)
    {
        while (!(UCSR1A & DATA_REGISTER_EMPTY_1));
        UDR1 = data;
    }// if UART_1
    else
    {
        while (!(UCSR0A & DATA_REGISTER_EMPTY_0));
        UDR0 = data;
    }//else

    __delay_cycles(TXDELAY);
    return data;
}//putchar

void ClearRxBuffer(void)	// Очистка приемного буфера UART1
{
    unsigned int ClearPoint = 0;

    for (; ClearPoint++ < RXBUFLENGTH_1; RxBuffer[ClearPoint] = 0);

    RxBufReadPoint = 0;
    RxBufWritePoint = 0;
}//ClearRxBuffer

//LoopTest

#define DTMF_PASS_LENGTH 4										// Например, "111#"
#define DTMF_COMMAND_LENGTH 2									// Например, "1#"
volatile char DTMFPassBuf[DTMF_PASS_LENGTH + 1] = "";	// Строка DTMF пароля или вводимой DTMF команды
volatile unsigned char DTMFPassPos = 0;					// Позиция, изменяемая в текущем прерывании
#define DTMF_PASS_TIMEOUT 10									// Таймаут в секундах при определении DTMF пароля
#define DTMF_COMMAMD_TIMEOUT 30								// Таймаут в секундах при вводе DTMF команды. Если таймаут истек, система разрывает соединение
unsigned char DTMFIncomingUser = 0;
#define DTMF_SHARP 12											// DTMF код символа #

#pragma vector = INT4_vect										// Внешнее прерывание INT4
//INT4_vect_Interrupt

#pragma vector = USART1_RXC_vect	// Прерывание по приходу байта в UART1
__interrupt void USART1_RXC_Interrupt(void)
{
    RxBuffer[RxBufWritePoint] = UDR1;
    if (RxBufWritePoint == RXBUFLENGTH_1 - 1)
        RxBufWritePoint = 0;
    else RxBufWritePoint++;
}//USART_RXC_Interrupt

volatile unsigned long Ticks = 0;							// Число переполнений таймера 0 = локальное время
volatile unsigned char TmSS = 0;								// Уровень сигнала для индикации
volatile unsigned char TmIndLoop = 0;						// Текущий индицируемый нарушенный шлейф
unsigned char AttemptInError = 0;							// Ошибка постановки на охраны. Нужна только для индикации. Еслии == 1 -> ошибка
volatile unsigned char TmErrRec = 0;						// Номер ошибочной записи в SIM-карте для индикации
volatile unsigned char TmSirenSingTime = 0;				// Оставшееся время работы сирены
volatile unsigned char TmLockOpenTime = 0;				// Оставшееся время, в течении которого замок будет оставаться открытым
volatile unsigned char TmGuardOffTimeout = 0;			// Оставшееся время ожидания при снятии системы с охраны
volatile unsigned char TmNoActiveTime = 0;				// Оставшееся время ожидания для возможности новой постановки на охрану
volatile unsigned int  TmRepeatGuardOn = 0;				// Время, через которое система повторно перейдет в режим охраны после нарушения (переход состоится только в том случае, если нарушенный шлейф восстановлен). Переход осуществляется через три минуты после восстановления. (fw104)

#pragma vector = TIMER0_OVF_vect								// Обработчик прерывания от таймера 0
//TIMER0_OVF_Interrupt

signed char ReadData(void)
{
    char data;

    if (RxBufWritePoint == RxBufReadPoint) return -1;	// Новых данных не найдено

    data = RxBuffer[RxBufReadPoint];

    if (RxBufReadPoint == RXBUFLENGTH_1 - 1) RxBufReadPoint = 0;
    else RxBufReadPoint++;

    return data;
}//ReadData

void SystemUpdate(void)	// Переопределение состояния системы
{
    unsigned char MessageDecoding = 0;		// Приказ распознан, если MessageDecoding != 0
#define MAX_LENGTH (sizeof("#Y#ZZZ"))
    char PassPlusCode[MAX_LENGTH];			// Строка для алгоритма определения пришедшей SMS-команды

#define MAX_LENGTH_NEW_REC (sizeof("#12#1 +79161234567 User1*123А151*"))	// Строка, которую пользователь ADMIN присылает как приказ на изменение параметров определенной записи пользователя
    char NewRecIncom[MAX_LENGTH_NEW_REC];

    char StarCount = 0;							// Счетчик звездочек для SMS-команды #11#
    char BlankCount = 0;						// Счетчик пробелов для SMS-команды #11#

    char AbNewPos = 0;
    char Ab11Number[MAX_NUMBER_LENGTH + 1];	// Номер абонента для SMS-команды #11#
    char Ab11Record[MAX_RECORD_LENGTH + 1];	// Запись абонента для SMS-команды #11#

    char AbNumCount = 0;
    char AbRecCount = 0;

    //	char MessageLength = 0; 					// Для определения необходимости удаления SMS-сообщения

    PassPlusCode[0] = 0;
    NewRecIncom[0] = 0;

    // Постановка и снятие с охраны тумблером
    if (((PINC & (1 << EXTKEY)) != 0) && (GuardSource == 0))	// Если разомкнули ключ и система еще не находится в режиме охраны
        GuardIn(GS_SWITCHER);											// Переходим в режим охраны
    if (((PINC & (1 << EXTKEY)) == 0) && (GuardSource != 0))	// Если замкнули ключ и система находится в режиме охраны
        GuardOut(GS_SWITCHER);											// Выходим из режима охраны

    // Постановка и снятие с охраны ключом iButton
    for (temp = 0; temp <= 300; temp++)
    {
        delay_ms(1);														// Итого суммарная задержка 1 секунда

        temp1 = iButtonKeyScan();

        if (temp1 == 1)													// Если к считывателю приложен ключ, имеющийся в базе ключей
        {
            TmLockOpenTime = LockOpenTime;							// Приложили ключ - в любом случае надо открыть замок (fw103)

            SetActiveUart(UART_0);										// TODO Remove debug (fw103)
            printf("iB1!\n\r");											// TODO Remove debug (fw103)
            SetActiveUart(UART_1);										// TODO Remove debug (fw103)

            if (GuardSource != 0)										// Если система находится в режиме охраны
                GuardOut(GS_IBUTTON);									// Попытка выхода из режима охраны

            break;
        }
    }

    if (AbRecReadComplete)												// Проверка приема новых SMS-сообщений с приказом поставить на охрану или снять с охраны или сообщения с управляющими приказами (#2# - #11#)
    {																			// Параллельно проверяются входящие звонки
        printf("AT+CPMS=\"SM\",\"SM\",\"SM\"\n\r");				// Для работы с SMS-сообщениями будет использоваться SIM-карта
        delay_ms(1000);
        StringBuilder();
        if (strstr(ReceiveString, "+CPMS: 0") == NULL)			// (fw116)
        {
            InboxSMS = 1;
            SetActiveUart(UART_0);										// TODO Remove debug (fw116)
            printf("Not Empty.\n\r");									// TODO Remove debug (fw116)
            SetActiveUart(UART_1);										// TODO Remove debug (fw116)			
        }
        else
        {
            InboxSMS = 0;
            SetActiveUart(UART_0);										// TODO Remove debug (fw116)
            printf("Empty.\n\r");										// TODO Remove debug (fw116)
            SetActiveUart(UART_1);										// TODO Remove debug (fw116)			
        }

        printf("AT+CMGL=\"REC UNREAD\"\n\r");

        // Постановка и снятие с охраны ключом iButton. Просто заменяем этой процедурой временную задержку в 5 секунд, чтобы не стоять без дела и чтобы пользователь, приложивший ключ, не ждал слишком долго
        for (temp = 0; temp <= 1500; temp++)
        {
            delay_ms(1);													// Итого суммарная задержка 5 секунд

            temp1 = iButtonKeyScan();

            if (temp1 == 1)												// Если к считывателю приложен ключ, имеющийся в базе ключей
            {
                TmLockOpenTime = LockOpenTime;						// Приложили ключ - в любом случае надо открыть замок (fw103)

                //				SetActiveUart(UART_0);									// TODO Remove debug (fw103)
                //				printf("iB2!\n\r");										// TODO Remove debug (fw103)
                //				SetActiveUart(UART_1);									// TODO Remove debug (fw103)

                if (GuardSource != 0)									// Если система находится в режиме охраны
                    GuardOut(GS_IBUTTON);								// Попытка выхода из режима охраны

                break;
            }
        }

        StringBuilder();

        temp1 = 0;
        while ((strstr(ReceiveString, "OK") == NULL) && (strstr(ReceiveString, "ERROR") == NULL) && (temp1 < 10))	// Пока не придет "OK", "ERROR" или не истечет таймаут в 10 секунд.
        {
            temp1++;
            StringBuilder();
            //			SetActiveUart(UART_0);														// TODO Remove debug
            //			printf("Wait...\n\r");														// TODO Remove debug
            delay_ms(1000);
            //			SetActiveUart(UART_1);														// TODO Remove debug
        }

        SetActiveUart(UART_0);															// TODO Remove debug
        printf("Call Check\n\r");														// TODO Remove debug
        SetActiveUart(UART_1);															// TODO Remove debug

        if (strstr(ReceiveString, "RING") != NULL)
        {
            SetActiveUart(UART_0);														// TODO Remove debug
            printf("Inc Call\n\r");														// TODO Remove debug
            SetActiveUart(UART_1);														// TODO Remove debug

            for (temp = 0; temp < MAX_AB_NUMBER; temp++)							// Перебираем абонентов
                if ((AbProfile[temp] != 'D')											// Если пользователь не заблокирован
                    && (strstr(ReceiveString, AbName[temp]) != NULL))				// Если имя верно
                {
                    DTMFIncomingUser = temp;
                    SetActiveUart(UART_0);											// TODO Remove debug
                    printf("Inc User = %s \n\r", AbName[temp]);				// TODO Remove debug
                    SetActiveUart(UART_1);											// TODO Remove debug			
                }

            DTMFPassPos = 0;
            DTMFPassBuf[0] = 0;
            DTMFPassBuf[1] = 0;
            DTMFPassBuf[2] = 0;
            DTMFPassBuf[3] = 0;
            printf("ATA\n\r");															// Берем трубку
            clrbit(TIMSK, TOIE0);														// Запретить прерывание по переполнению от таймера 0 (чтобы не мешал разборчивому воспроизведению речи)
            setbit(PORTC, GSMLED);														// Горит постоянно - режим разговора

            clrbit(PORTB, PWDN);															// Включаем MT8870
            StringBuilder();

            DF_PagesToSpeaker(VVEDITE_START_DF, VVEDITE_END_DF);				// "Введите..."
            DF_PagesToSpeaker(PAROL_START_DF, PAROL_END_DF);					// "...пароль"

            temp = 0;
            while ((temp < DTMF_PASS_TIMEOUT) && (DTMFPassPos < DTMF_PASS_LENGTH))
            {
                printf("AT+CPAS=1\n\r");														// fw108
                delay_ms(1000);
                StringBuilder();
                if (strstr(ReceiveString, "129") != NULL)									// Если абонент положил трубку
                    break;
                temp++;
                //				SetActiveUart(UART_0);															// TODO Remove debug
                //				printf("seconds = %d, DTMFPassPos = %d\n\r", temp, DTMFPassPos);	// TODO Remove debug
                //				SetActiveUart(UART_1);															// TODO Remove debug
            }

            if (((DTMFPassBuf[0] == AbPass[DTMFIncomingUser][0] - 48) &&
                (DTMFPassBuf[1] == AbPass[DTMFIncomingUser][1] - 48) &&
                (DTMFPassBuf[2] == AbPass[DTMFIncomingUser][2] - 48)) ||
                ((DTMFPassBuf[0] == GuestPass[0] - 48) &&
                    (DTMFPassBuf[1] == GuestPass[1] - 48) &&
                    (DTMFPassBuf[2] == GuestPass[2] - 48)))
            {
                DTMFPassPos = 0;
                temp = 0;

                DF_PagesToSpeaker(VVEDITE_START_DF, VVEDITE_END_DF);						// "Введите..."
                DF_PagesToSpeaker(KOMANDU_START_DF, KOMANDU_END_DF);						// "...команду"

                while (temp < DTMF_COMMAMD_TIMEOUT)
                {
                    printf("AT+CPAS=1\n\r");														// fw108
                    delay_ms(1000);
                    StringBuilder();
                    if (strstr(ReceiveString, "129") != NULL)									// Если абонент положил трубку
                        break;

                    temp++;
                    //					SetActiveUart(UART_0);															// TODO Remove debug
                    //					printf("seconds = %d, DTMFPassPos = %d\n\r", temp, DTMFPassPos);	// TODO Remove debug
                    //					SetActiveUart(UART_1);															// TODO Remove debug

                    if (DTMFPassPos == DTMF_COMMAND_LENGTH)
                    {
                        DTMFPassPos = 0;																// Готовы к вводу новой команды
                        temp = 0;																		// Добавляем пользователю еще DTMF_COMMAMD_TIMEOUT секунд

                        if (((DTMFPassBuf[0] < 7) || (DTMFPassBuf[0] == 9)) && (DTMFPassBuf[1] == DTMF_SHARP))	// Валидны только команды 0...6 и 9
                        {
                            //							SetActiveUart(UART_0);													// TODO Remove debug
                            //							printf("DTMF Command accepted\n\r");								// TODO Remove debug
                            //							SetActiveUart(UART_1);													// TODO Remove debug

                            switch (DTMFPassBuf[0])
                            {
                            case 0:
                                GuardOut(GS_DTMF);												// Предпринимаем попытку выйти из режима охраны
                                DF_PagesToSpeaker(SIGNAL_START_DF, SIGNAL_END_DF);		// "Сигнализация..."
                                DF_PagesToSpeaker(VIKLA_START_DF, VIKLA_END_DF);		// "...выключена"
                                SendMessageToExtUser(" GUARD OFF");							// fw114
                                break;
                            case 1:
                                GuardIn(GS_DTMF);													// Предпринимаем попытку войти в режим охраны
                                DF_PagesToSpeaker(SIGNAL_START_DF, SIGNAL_END_DF);		// "Сигнализация..."
                                DF_PagesToSpeaker(VKLA_START_DF, VKLA_END_DF);			// "...включена"
                                SendMessageToExtUser(" GUARD ON");							// fw114
                                break;
                            case 2:
                                setbit(PORTG, K1);
                                DF_PagesToSpeaker(RELE_START_DF, RELE_END_DF);			// "Реле..."
                                DF_PagesToSpeaker(ODIN_START_DF, ODIN_END_DF);			// "...один..."
                                DF_PagesToSpeaker(VKL_START_DF, VKL_END_DF);				// "...включено"
                                SendMessageToExtUser(" RELE 1 ON");							// fw114
                                break;
                            case 3:
                                clrbit(PORTG, K1);
                                DF_PagesToSpeaker(RELE_START_DF, RELE_END_DF);			// "Реле..."
                                DF_PagesToSpeaker(ODIN_START_DF, ODIN_END_DF);			// "...один..."
                                DF_PagesToSpeaker(VIKL_START_DF, VIKL_END_DF);			// "...выключено"
                                SendMessageToExtUser(" RELE 1 OFF");						// fw114
                                break;
                            case 4:
                                setbit(PORTG, K2);
                                DF_PagesToSpeaker(RELE_START_DF, RELE_END_DF);			// "Реле..."
                                DF_PagesToSpeaker(DVA_START_DF, DVA_END_DF);				// "...два..."
                                DF_PagesToSpeaker(VKL_START_DF, VKL_END_DF);				// "...включено"
                                SendMessageToExtUser(" RELE 2 ON");							// fw114
                                break;
                            case 5:
                                clrbit(PORTG, K2);
                                DF_PagesToSpeaker(RELE_START_DF, RELE_END_DF);			// "Реле..."
                                DF_PagesToSpeaker(DVA_START_DF, DVA_END_DF);				// "...два..."
                                DF_PagesToSpeaker(VIKL_START_DF, VIKL_END_DF);			// "...выключено"
                                SendMessageToExtUser(" RELE 2 OFF");						// fw114
                                break;
                            case 6:
                                DF_PagesToSpeaker(SIGNAL_START_DF, SIGNAL_END_DF);		// "Сигнализация..."
                                if (GuardSource == 0)
                                    DF_PagesToSpeaker(VIKLA_START_DF, VIKLA_END_DF);	// "...выключена"
                                else
                                    DF_PagesToSpeaker(VKLA_START_DF, VKLA_END_DF);		// "...включена"
                                break;
                            case 9:
                                DF_PagesToSpeaker(EMODE_START_DF, EMODE_END_DF);		// "Экономичный режим..."
                                if (PowerCorrect == 0)
                                {
                                    SMSorDTMFEconoMode = 1;
                                    DF_PagesToSpeaker(VKLN_START_DF, VKLN_END_DF);		// "...включен"
                                    SendMessageToExtUser(" Forced EconoMode ON");		// fw114
                                }
                                else
                                    DF_PagesToSpeaker(VIKLN_START_DF, VIKLN_END_DF);	// "...выключен"
                                break;
                            }
                            DF_PagesToSpeaker(VVEDITE_START_DF, VVEDITE_END_DF);			// "Введите..."
                            DF_PagesToSpeaker(SLKOMANDU_START_DF, SLKOMANDU_END_DF);		// "...следующую команду"
                        }
                        else
                        {
                            //							SetActiveUart(UART_0);													// TODO Remove debug
                            //							printf("DTMF Command rejected\n\r");								// TODO Remove debug
                            //							SetActiveUart(UART_1);													// TODO Remove debug
                            DF_PagesToSpeaker(ERROR_START_DF, ERROR_END_DF);				// "Ошибка, повторите команду" !!! - В моем варианте прошивки отсутствует, д.б. просто тишина
                        }//Неверная команда
                    }//Введено два символа
                }//Таймаут
            }//Верный пароль
//			else
//			{
//				SetActiveUart(UART_0);																// TODO Remove debug
//				printf("DTMF Password is FALSE\n\r");											// TODO Remove debug
//				SetActiveUart(UART_1);																// TODO Remove debug
//			}//Неверный пароль

            printf("ATH\n\r");																		// Кладем трубку
            clrbit(PORTC, GSMLED);																	// Гасим
            setbit(TIMSK, TOIE0);																	// Разрешить прерывание по переполнению от таймера 0

            delay_ms(2000);
            StringBuilder();
        }

        //		MessageLength = strlen(ReceiveString);

        if ((strstr(ReceiveString, "#0#") != NULL)						// Пришла SMS-команда
            || (strstr(ReceiveString, "#1#") != NULL)
            || (strstr(ReceiveString, "#2#") != NULL)
            || (strstr(ReceiveString, "#3#") != NULL)
            || (strstr(ReceiveString, "#4#") != NULL)
            || (strstr(ReceiveString, "#5#") != NULL)
            || (strstr(ReceiveString, "#6#") != NULL)
            || (strstr(ReceiveString, "#7#") != NULL)
            || (strstr(ReceiveString, "#9#") != NULL)
            || (strstr(ReceiveString, "#10#") != NULL)
            || (strstr(ReceiveString, "#11#") != NULL))
        {
            for (temp = 0; temp < MAX_AB_NUMBER; temp++)					// Перебираем абонентов
            {
                if ((AbProfile[temp] != 'D')									// Если пользователь не заблокирован
                    && (strncmp(AbName[temp], "##", sizeof("##")) != 0)	// Если запись не пуста
                    && (strstr(ReceiveString, AbPass[temp]) != NULL)		// Если пароль верен
                    && (strstr(ReceiveString, AbName[temp]) != NULL))		// Если имя верно
                {
                    printf("AT+CMGS=\"%s\"\n\r", AbNumber[temp]);		// Начинаем отправку SMS
                    delay_ms(2000);

                    if ((strstr(ReceiveString, "#0#") != NULL) && (GuardSource != 0))		// Если пришел приказ снять с охраны и если система стоит на охране				
                    {
                        GuardOut(GS_SMS);
                        printf(" GUARD OFF");
                        MessageDecoding = 51;
                    }

                    if ((strstr(ReceiveString, "#1#") != NULL) && (GuardSource == 0) && (LoopTest() == 1))		// Если пришел приказ поставить на охрану и если система еще не поставлена на охрану и если охранные шлейфы исправны
                    {
                        GuardIn(GS_SMS);
                        printf(" GUARD ON");
                        MessageDecoding = 52;
                    }

                    if (strstr(ReceiveString, "#2#") != NULL)						// Если пришел приказ включить реле 1
                    {
                        setbit(PORTG, K1);
                        printf(" RELE 1 ON");
                        MessageDecoding = 53;
                    }

                    if (strstr(ReceiveString, "#3#") != NULL)						// Если пришел приказ выключить реле 1
                    {
                        clrbit(PORTG, K1);
                        printf(" RELE 1 OFF");
                        MessageDecoding = 54;
                    }

                    if (strstr(ReceiveString, "#4#") != NULL)						// Если пришел приказ включить реле 2
                    {
                        setbit(PORTG, K2);
                        printf(" RELE 2 ON");
                        MessageDecoding = 55;
                    }

                    if (strstr(ReceiveString, "#5#") != NULL)						// Если пришел приказ выключить реле 2
                    {
                        clrbit(PORTG, K2);
                        printf(" RELE 2 OFF");
                        MessageDecoding = 56;
                    }

                    if (strstr(ReceiveString, "#6#") != NULL)						// Если пришел запрос состояния изделия
                    {
                        if (GuardSource == 0)
                            printf(" GUARD OFF");
                        else
                            printf(" GUARD ON");
                        MessageDecoding = 1;
                    }

                    if (strstr(ReceiveString, "#9#") != NULL)						// Если пришел приказ на принудительный переход в малопотребляющее состояние
                    {
                        if (PowerCorrect == 0)
                        {
                            SMSorDTMFEconoMode = 1;
                            printf(" Forced EconoMode ON");							// Включаем принудительный экономичный режим
                        }
                        else
                            printf(" Forced EconoMode NOT TO HOLD");				// Внешнее питание в порядке - принудительный экономичный режим не применим
                        MessageDecoding = 57;
                    }

                    if ((strstr(ReceiveString, "#10#") != NULL) && (strstr(ReceiveString, "ADMIN") != NULL))		// Если пришел запрос записи от пользователя ADMIN
                    {
                        MessageDecoding = 1;
                        strncpy(PassPlusCode, strstr(ReceiveString, "#10#"), sizeof("#10#X") - 1);
                        temp1 = PassPlusCode[4] - 48;
                        printf("%d %s %s", temp1, AbNumber[temp1 - 1], AbRecord[temp1 - 1]);									// Нулевой записи в программном массиве соответствует 1-й номер, поэтому "-1"
                    }

                    if ((strstr(ReceiveString, "#11#") != NULL) && (strstr(ReceiveString, "ADMIN") != NULL))		// Если пришел приказ изменения записи от пользователя ADMIN
                    {
                        strncpy(NewRecIncom, strstr(ReceiveString, "#11#"), sizeof(NewRecIncom));

                        SetActiveUart(UART_0);																				// TODO Remove debug
                        printf("NewRecIncom = %s\n\r", NewRecIncom);													// TODO Remove debug
                        SetActiveUart(UART_1);																				// TODO Remove debug

                        AbNewPos = NewRecIncom[4] - 48;

                        for (temp1 = 0; temp1 <= strlen(NewRecIncom); temp1++)
                        {
                            if ((BlankCount == 1) && (isdigit(NewRecIncom[temp1])))
                            {
                                Ab11Number[AbNumCount] = NewRecIncom[temp1];
                                Ab11Number[AbNumCount + 1] = 0;
                                AbNumCount++;
                            }
                            if ((BlankCount == 2) && (StarCount < 2))
                            {
                                Ab11Record[AbRecCount] = NewRecIncom[temp1];
                                Ab11Record[AbRecCount + 1] = 0;
                                AbRecCount++;
                            }
                            if (NewRecIncom[temp1] == ' ')
                                BlankCount++;
                            if (NewRecIncom[temp1] == '*')
                                StarCount++;
                        }
                        SetActiveUart(UART_0);																				// TODO Remove debug
                        printf("AbNewPos = %d\n\r", AbNewPos);															// TODO Remove debug
                        printf("Ab11Number = %s\n\r", Ab11Number);													// TODO Remove debug
                        printf("Ab11Record = %s\n\r", Ab11Record);													// TODO Remove debug
                        SetActiveUart(UART_1);																				// TODO Remove debug

                        MessageDecoding = 11;
                        printf(" User redefined as: %d %s %s ", AbNewPos, Ab11Number, Ab11Record);
                    }

                    if (strstr(ReceiveString, "#7#") != NULL)						// Если пришел приказ на изменение режимов работы охранных шлейфов
                    {
                        MessageDecoding = 58;
                        strncpy(PassPlusCode, strstr(ReceiveString, "#7#"), sizeof(PassPlusCode) - 1);

                        if ((PassPlusCode[3] - 48 <= 2) && (PassPlusCode[4] - 48 <= 2) && (PassPlusCode[5] - 48 <= 2))
                        {
                            switch (PassPlusCode[3] - 48)
                            {
                            case 0: Loop1Set = 0; break;	// Шлейф отключен
                            case 1: Loop1Set = 2; break;	// Шлейф задействован и работает на размыкание
                            case 2: Loop1Set = 1; break;	// Шлейф задействован и работает на замыкание
                            }
                            switch (PassPlusCode[4] - 48)
                            {
                            case 0: Loop2Set = 0; break;	// Шлейф отключен
                            case 1: Loop2Set = 2; break;	// Шлейф задействован и работает на размыкание
                            case 2: Loop2Set = 1; break;	// Шлейф задействован и работает на замыкание
                            }
                            switch (PassPlusCode[5] - 48)
                            {
                            case 0: Loop3Set = 0; break;	// Шлейф отключен
                            case 1: Loop3Set = 2; break;	// Шлейф задействован и работает на размыкание
                            case 2: Loop3Set = 1; break;	// Шлейф задействован и работает на замыкание
                            }
                            printf(" Loops status redefined");
                        }
                        else
                            printf(" ERROR - loops status NOT redefined");
                    }

                    if (MessageDecoding == 0)					// Если сообщение так и не было идентифицировано
                        printf(" ERROR MESSAGE");
                    printf("%c", CtrlZ);							// Заканчиваем отправку SMS

                    delay_ms(1000);
                    StringBuilder();
                    temp1 = 0;
                    while ((strstr(ReceiveString, "OK") == NULL) && (strstr(ReceiveString, "ERROR") == NULL) && (temp1 < 10))	// Пока не придет "OK", "ERROR" или не истечет таймаут в 10 секунд.
                    {
                        temp1++;
                        StringBuilder();
                        delay_ms(1000);
                    }//Ожидание ответа от GR-47

                    StringBuilder();

                    if (MessageDecoding == 11)					// Если была произведена смена пользовательской записи
                    {
                        printf("AT+CPBW=%d,\"%s\",129,\"%s\"\n\r", AbNewPos, Ab11Number, Ab11Record);			// Команда на запись нового номера
                        delay_ms(2000);
                        StringBuilder();
                        AbRecReadComplete = 0; 					// Для обновления всех системных параметров надо по новой прочитать телефонную книгу
                    }

                    switch (MessageDecoding)
                    {
                    case 51:
                    {
                        delay_ms(3000);
                        SendMessageToExtUser(" GUARD OFF");
                        break;
                    }
                    case 52:
                    {
                        delay_ms(3000);
                        SendMessageToExtUser(" GUARD ON");
                        break;
                    }
                    case 53:
                    {
                        delay_ms(3000);
                        SendMessageToExtUser(" RELE 1 ON");
                        break;
                    }
                    case 54:
                    {
                        delay_ms(3000);
                        SendMessageToExtUser(" RELE 1 OFF");
                        break;
                    }
                    case 55:
                    {
                        delay_ms(3000);
                        SendMessageToExtUser(" RELE 2 ON");
                        break;
                    }
                    case 56:
                    {
                        delay_ms(3000);
                        SendMessageToExtUser(" RELE 2 OFF");
                        break;
                    }
                    case 57:
                    {
                        delay_ms(3000);
                        SendMessageToExtUser(" Forced EconoMode ON");
                        break;
                    }
                    case 11:
                    {
                        delay_ms(3000);
                        SendMessageToExtUser(" User redefined");
                        break;
                    }
                    case 58:
                    {
                        delay_ms(3000);
                        SendMessageToExtUser(" Loops status redefined");
                        break;
                    }
                    }//switch (MessageDecoding)
                }//Начало отправки SMS-сообщения
            }//Перебираем абонентов
        }//SMS с командами не принято

//		if (MessageLength > 70)									// Значит, было принято SMS-сообщение. Просто так, при каждом проходе удалять нельзя, часть сообщений теряется
        if (InboxSMS != 0)										// fw116
        {
            printf("AT+CMGD = 1\n\r");							// Удаляем первое SMS-сообщение на SIM-карте. Какой ответ будет - OK или ERROR, не важно, главное, что память SIM-сообщений никогда не поре полнится и GSM-модуль не зависнет
            delay_ms(1000);
            StringBuilder();
            printf("AT+CMGD = 2\n\r");							// Удаляем второе SMS-сообщение на SIM-карте. Какой ответ будет - OK или ERROR, не важно, главное, что память SIM-сообщений никогда не поре полнится и GSM-модуль не зависнет
            delay_ms(1000);
            StringBuilder();
            printf("AT+CMGD = 3\n\r");							// Удаляем третье SMS-сообщение на SIM-карте. Какой ответ будет - OK или ERROR, не важно, главное, что память SIM-сообщений никогда не поре полнится и GSM-модуль не зависнет
            delay_ms(1000);
            StringBuilder();
            //			SetActiveUart(UART_0);								// TODO Remove debug
            //			printf("Message delete\n\r");						// TODO Remove debug
            //			SetActiveUart(UART_1);								// TODO Remove debug
        }
    }//AbRecReadComplete

    Loop1On = Loop1Set;											// Параметры охранного шлейфа - в EEPROM
    Loop2On = Loop2Set;
    Loop3On = Loop3Set;

    // Контроль напряжения на батарее и переключение режимов заряда
    temp = ReadADC(VBAT);													// Проверяем напряжение на батарее
    //	if ((temp <= ACC_FAULT) && (PowerCorrect == 0))					// Если напряжение упало до 11 В и если отсутствует внешнее питание
    if (((temp <= ACC_FAULT) || (SMSorDTMFEconoMode == 1)) && (PowerCorrect == 0))	// (Если напряжение упало до 11 В или поступила команда принудительного экономичного режима) и если отсутствует внешнее питание
    {
        EconoMode = 1;															// Включаем экономичный режим
        //		SetActiveUart(UART_0);												// TODO Remove debug
        //		printf("Standby mode\n\r");										// TODO Remove debug
        //		SetActiveUart(UART_1);												// TODO Remove debug

        if (AbRecReadComplete)
        {
            if ((SendingPOWOFF == 0) && (SMSorDTMFEconoMode == 0))		// Если сообщения еще не рассылались и это не принудительный переход в малопотребляющий режим
            {
                for (temp = 0; temp < MAX_AB_NUMBER; temp++)
                {
                    if ((strncmp(AbName[temp], "GUEST", sizeof("GUEST")) != 0)
                        && (strncmp(AbName[temp], "##", sizeof("##")) != 0)
                        && (AbProfile[temp] != 'D'))							// Если пользователь задействован и он не GUEST и не заблокирован
                    {
                        printf("AT+CMGS=\"%s\"\n\r", AbNumber[temp]);
                        delay_ms(2000);
                        printf("Message to %s: POWER OFF%c", AbName[temp], CtrlZ);	// TODO Remove debug Message to - убрать, это только для отладки

                        //						delay_ms(4000);
                        StringBuilder();
                        temp1 = 0;
                        while ((strstr(ReceiveString, "OK") == NULL) && (strstr(ReceiveString, "ERROR") == NULL) && (temp1 < SMS_TIMEOUT))	// Пока не придет "OK", "ERROR" или не истечет таймаут в 60 секунд.
                        {
                            temp1++;
                            StringBuilder();
                            //							SetActiveUart(UART_0);												// TODO Remove debug
                            //							printf("Wait...\n\r");												// TODO Remove debug
                            delay_ms(1000);
                            //							SetActiveUart(UART_1);												// TODO Remove debug
                        }
                    }
                }
                SendingPOWOFF = 1;											// Больше рассылать не надо
            }
        }
        GR_Off();																// После рассылки SMS-сообщений выключаем GSM-модуль 
        GSMStatus = 0;															// И вся программа знает, что мы выключили GSM-модуль
    }
    else
    {
        EconoMode = 0;															// Включаем режим нормального электропотребления
        SMSorDTMFEconoMode = 0;												// Предыдущий приказ отменяется
        SendingPOWOFF = 0;
    }

    // Включение - выключение заряда аккумулятора
    if (PowerCorrect == 1)													// Если внешнее напряжение в норме
        if (temp <= ACC_CORRECT)
            setbit(PORTB, CHARG);											// Включаем быструю зарядку аккумулятора
        else
            clrbit(PORTB, CHARG);											// Выключаем быструю зарядку, переводим аккумулятор в режим капельного заряда
    else
        clrbit(PORTB, CHARG);												// Безусловно выключаем быструю зарядку

    SetActiveUart(UART_1);										// TODO Remove debug
    delay_ms(2000);												// TODO Remove debug

    if ((PINA & (1 << VDET)) > 0)								// Если внешнее питание в норме
    {
        PowerCorrect = 1;											// Внешнее питание в норме

        if ((AbRecReadComplete) && (GSMStatus == 2) && (Sending220VON == 0))	// Условие рассылки сообщений "220V ON"
        {
            for (temp = 0; temp < MAX_AB_NUMBER; temp++)
            {
                if ((strncmp(AbName[temp], "GUEST", sizeof("GUEST")) != 0)
                    && (strncmp(AbName[temp], "##", sizeof("##")) != 0)
                    && (AbProfile[temp] != 'D'))											// Если пользователь задействован и он не GUEST и не заблокирован
                {
                    printf("AT+CMGS=\"%s\"\n\r", AbNumber[temp]);
                    delay_ms(2000);
                    printf("Message to %s: 220V ON%c", AbName[temp], CtrlZ);	// TODO Remove debug Message to - убрать, это только для отладки
                    delay_ms(4000);

                    StringBuilder();
                }
            }
            Sending220VOFF = 0;
            Sending220VON = 1;
        }
    }
    else														// Сбой внешнего питания
    {
        PowerCorrect = 0;											// Сбой внешнего питания

        if (AbRecReadComplete)
        {
            if (Sending220VOFF == 0)				// Если сообщения еще не рассылались
            {
                for (temp = 0; temp < MAX_AB_NUMBER; temp++)
                {
                    if ((strncmp(AbName[temp], "GUEST", sizeof("GUEST")) != 0)
                        && (strncmp(AbName[temp], "##", sizeof("##")) != 0)
                        && (AbProfile[temp] != 'D'))	// Если пользователь задействован и он не GUEST и не заблокирован
                    {
                        printf("AT+CMGS=\"%s\"\n\r", AbNumber[temp]);
                        delay_ms(2000);
                        printf("Message to %s: 220V OFF%c", AbName[temp], CtrlZ);	// TODO Remove debug Message to - убрать, это только для отладки
                        delay_ms(4000);

                        StringBuilder();
                    }
                }
                Sending220VOFF = 1;
                Sending220VON = 0;
            }
        }
    }

    if (GSMStatus == 1)									// Если GSM-модуль включен, но еще не зарегистрирован в сети, пытаемся проверить регистрацию
    {
        printf("AT+CREG=1\n\r");
        delay_ms(1000);
        StringBuilder();
        if (strstr(ReceiveString, "OK") != NULL)	// Если GSM-модуль ответил "OK"...
            GSMStatus = 2;									// GSM-модуль зарегистрирован в сети
    }

    if (GSMStatus == 2)								// Если GSM-модуль зарегистрирован в сети, проверяем уровень сигнала
    {
        printf("AT+CSQ\n\r");
        delay_ms(1000);
        StringBuilder();
        strtok(ReceiveString, ",");				// Отсекаем bit error rate <ber>, который идет после запятой

        GSMSigStrength = StrToInt(ReceiveString);
        if (GSMSigStrength <= 31)					// Нормальное значение
        {
            GSMSigStrength /= 6;
            if (GSMSigStrength == 0) GSMSigStrength = 1;	// Чтобы моргнуть хотя бы раз и показать пользователю, что GSM-модуль видит сеть
        }
        else												//	Ненормальное значение, скорее всего, 99 - "Not known or not detectable", но раз уж GSM-модуль говорит, что сеть видна, нужно хотя бы раз моргнуть...
            GSMSigStrength = 1;
    }

    if ((EconoMode == 0) && (GSMStatus == 0))		// Нужно включить GSM-модуль
    {
        SetActiveUart(UART_0);							// TODO Remove debug
        printf("GSM ON attempt\n\r");					// TODO Remove debug 
        SetActiveUart(UART_1);							// TODO Remove debug

        GR_On();												// Включаем GSM-модуль
        printf("AT\n\r");
        delay_ms(1000);

        StringBuilder();

        if (strstr(ReceiveString, "OK") != NULL)	// Если GSM-модуль ответил "OK"...
        {
            //			SetActiveUart(UART_0);						// TODO Remove debug
            //			printf("OK received\n\r");					// TODO Remove debug
            //			SetActiveUart(UART_1);						// TODO Remove debug

            printf("ATE=1\n\r");							// ...выключаем эхо, чтобы не забивать приемный буфер
            delay_ms(500);
            printf("AT*E2RS232=2\n\r");				// ...выключаем аппаратный контроль UART'а GSM-модуля
            delay_ms(500);
            printf("AT+CLIP=1\n\r");					// ...включаем АОН
            delay_ms(500);
            printf("AT+CMEE=1\n\r");					// ...включаем расширенные коды ошибок (числами)
            delay_ms(500);
            printf("AT+CSDH=1\n\r");					// ...включаем возможность текстового режима
            delay_ms(500);
            printf("AT+CMGF=1\n\r");					// ...включаем текстовый режим передачи SMS
            delay_ms(500);

            printf("AT*E2SSN\n\r");
            delay_ms(2000);

            StringBuilder();
            if (strstr(ReceiveString, "ERROR") == NULL)		// Если GSM-модуль не выдал ошибки...
            {
                GSMStatus = 1;											// ...то изменяем статус на "Не зарегистрирован в сети"
                SetActiveUart(UART_0);								// TODO Remove debug
                printf("GR Init OK.\n\r");							// TODO Remove debug
                SetActiveUart(UART_1);								// TODO Remove debug
            }
            else															// А если уж ошибка произошла, пробуем сменить SIM-карту
            {
                if (SIMNum == 0)
                {
                    SIMNum = 1;
                    setbit(PORTC, SIMSW);
                }
                else
                {
                    SIMNum = 0;
                    clrbit(PORTC, SIMSW);
                }
            }
        }//OK
    }//EconoMode
}//SystemUpdate

void GR_On(void)					// Включаем GR-47
{
    clrbit(PORTG, nGSMPOW);		// Включаем D9 - LM2576T-ADJ
    clrbit(PORTD, PON);
    delay_ms(3000);
    setbit(PORTD, PON);
    delay_ms(4000);
}//GR_On

void GR_Off(void)					// Выключаем GR-47
{
    clrbit(PORTD, PON);
    delay_ms(3000);
    setbit(PORTD, PON);
    delay_ms(4000);				// 4 секунды на закрытие соединений и "теплое" выключение
    setbit(PORTG, nGSMPOW);		// Выключаем D9 - LM2576T-ADJ; "холодное" выключение
    GSMStatus = 0;					// И вся остальная программа знает, что модуль выключен
}//GR_Off

void StringBuilder(void)		// Делает текстовую строку из данных в приемном буфере
{
    signed char IncomChar;
    int StringPoint = 0;
    ReceiveString[0] = 0;

    while ((IncomChar = ReadData()) != -1)			// Проверяем наличие символа
    {
        if (IncomChar > 20)								// Отсекаем ненужные нам служебные символы
        {
            SetActiveUart(UART_0);							// TODO Remove debug
            printf("%c", IncomChar);						// TODO Remove debug
            SetActiveUart(UART_1);							// TODO Remove debug

            ReceiveString[StringPoint] = IncomChar;
            ReceiveString[StringPoint + 1] = 0;			// Символ конца строки
            StringPoint++;
        }
    }
    SetActiveUart(UART_0);									// TODO Remove debug
    printf("\n\r");											// TODO Remove debug
    SetActiveUart(UART_1);									// TODO Remove debug
}//StringBuilder

int StrToInt(char* InputStr)								// Процедура конвертирует строку в число типа int. Конвертируются только цифры
{
    int ReturnValue = 0;
    unsigned char StrCount = 0;

    for (; StrCount <= strlen(InputStr); StrCount++)
        if (isdigit(InputStr[StrCount]))
            ReturnValue = 10 * ReturnValue + (InputStr[StrCount] - 48);

    return ReturnValue;
}//StrToInt

char ReadAbRecords(void)									// Чтение записей абонентов. Если записи прочитаны и не имеют ошибок, возвращаемое значение == 1, иначе == 0
{
    char RecCount = 0;										// Счетчик телефонных записей
    char CharCount = 0;										// Счетчик символов внутри ответа команды AT+CPBR
    char QuotesCount = 0;									// Счетчик кавычек. Между 1-й и 2-й кавычками находится телефонный номер, между 3-й и 4-й - имя телефонной записи
    char InNumCount = 0;										// Счетчик символов телефонного номера
    char InRecCount = 0;										// Счетчик символов телефонной записи
    char StarPos = 0;
    char ServiceName[MAX_NAME_LENGTH + 1];				// Имя сервисной записи
    char ServiceNum[SERVICE_NUM_LENGTH + 1];			// Номер сервисной записи

    if (GSMStatus == 0) return 0; 						// GSM-модуль не включен, нечего и пытаться
    else
    {
        for (RecCount = 0; RecCount < MAX_AB_NUMBER; RecCount++)	// Пытаемся прочитать записи из телефонной книги
        {
            printf("AT+CPBR=%d\n\r", RecCount + 1);						// Запрос очередной записи из телефонной книги
            delay_ms(2000);
            StringBuilder();
            if (strstr(ReceiveString, "OK") != NULL)					// Если GSM-модуль выдал подтверждение...
            {
                QuotesCount = 0;
                InNumCount = 0;
                InRecCount = 0;
                AbNumber[RecCount][0] = 0;									// Очищаем строку Номер
                AbRecord[RecCount][0] = 0;									// Очищаем строку Запись
                AbName[RecCount][0] = 0;									// Очищаем строку Имя
                AbPass[RecCount][0] = 0;									// Очищаем строку Пароль
                AbPass[RecCount][3] = 0;									// Завершающий 0 строки Пароль

                for (CharCount = 0; CharCount < strlen(ReceiveString); CharCount++)
                {
                    if (QuotesCount == 1)
                    {
                        if (ReceiveString[CharCount] != '"')
                        {
                            AbNumber[RecCount][InNumCount] = ReceiveString[CharCount];		// Формируем строку Номер
                            AbNumber[RecCount][InNumCount + 1] = 0;
                        }
                        //						SetActiveUart(UART_0);																						// TODO Remove debug
                        //						if (ReceiveString[CharCount] != '"')																	// TODO Remove debug
                        //							printf("N%c ", AbNumber[RecCount][InNumCount]);													// TODO Remove debug
                        //						SetActiveUart(UART_1);																						// TODO Remove debug
                        if (InNumCount == MAX_NUMBER_LENGTH)
                        {
                            //							SetActiveUart(UART_0);								// TODO Remove debug
                            //							printf("Number too long\n\r");					// TODO Remove debug
                            //							printf("Error - NTL\n\r");							// TODO Remove debug
                            SetActiveUart(UART_1);								// TODO Remove debug
                            IncorrectAb = RecCount + 1;						// Нулевой абонент в базе системы соответствует первому абоненту в SIM-карте
                            return 0;
                        }
                        InNumCount++;
                    }//QuotesCount==1
                    if (QuotesCount == 3)
                    {
                        if (ReceiveString[CharCount] != '"')
                        {
                            AbRecord[RecCount][InRecCount] = ReceiveString[CharCount];	// Формируем строку Запись
                            AbRecord[RecCount][InRecCount + 1] = 0;
                        }
                        //						SetActiveUart(UART_0);																						// TODO Remove debug
                        //						if (ReceiveString[CharCount] != '"')																	// TODO Remove debug
                        //							printf("R%c ", AbRecord[RecCount][InRecCount]);													// TODO Remove debug	
                        SetActiveUart(UART_1);																						// TODO Remove debug
                        if (InRecCount == MAX_RECORD_LENGTH)
                        {
                            //							SetActiveUart(UART_0);								// TODO Remove debug
                            //							printf("Record too long\n\r");					// TODO Remove debug
                            //							printf("Error - RTL\n\r");							// TODO Remove debug
                            //							SetActiveUart(UART_1);								// TODO Remove debug
                            IncorrectAb = RecCount + 1;						// Нулевой абонент в базе системы соответствует первому абоненту в SIM-карте
                            return 0;
                        }
                        InRecCount++;
                    }//QuotesCount==3
                    if (ReceiveString[CharCount] == '"')					// Нашли еще одну кавычку
                    {
                        QuotesCount++;
                        //						SetActiveUart(UART_0);									// TODO Remove debug
                        //						printf("QuotesCount = %d\n\r",  QuotesCount);	// TODO Remove debug
                        //						SetActiveUart(UART_1);									// TODO Remove debug
                    }//счетчик кавычек
                }//for CharCount

                SetActiveUart(UART_0);														// TODO Remove debug
                printf("%d Number is %s\n\r", RecCount + 1, AbNumber[RecCount]);// TODO Remove debug
                printf("%d Record is %s\n\r", RecCount + 1, AbRecord[RecCount]);// TODO Remove debug
                SetActiveUart(UART_1);														// TODO Remove debug

                StarPos = strcspn(AbRecord[RecCount], "*");
                strncpy(AbName[RecCount], AbRecord[RecCount], StarPos); 	// Формируем Имя абонента = часть Записи левее символа * 

                SetActiveUart(UART_0);													// TODO Remove debug
                printf("%d Name is %s\n\r", RecCount + 1, AbName[RecCount]);	// TODO Remove debug
                SetActiveUart(UART_1);													// TODO Remove debug

                if (strncmp(AbName[RecCount], "##", sizeof("##")) == 0) AbActive[RecCount] = 0;	// Пользователь не активен
                else
                {
                    AbActive[RecCount] = 1;		// Пользователь активен

                    AbPass[RecCount][0] = AbRecord[RecCount][StarPos + 1];	// Формируем пароль абонента
                    AbPass[RecCount][1] = AbRecord[RecCount][StarPos + 2];
                    AbPass[RecCount][2] = AbRecord[RecCount][StarPos + 3];

                    if ((strncmp(AbName[RecCount], "GUEST", sizeof("GUEST")) == 0)	// Для использования в дальнейшем при работе с DTMF
                        || (strncmp(AbName[RecCount], "Guest", sizeof("GUEST")) == 0)
                        || (strncmp(AbName[RecCount], "guest", sizeof("GUEST")) == 0))
                    {
                        GuestPass[0] = AbPass[RecCount][0];
                        GuestPass[1] = AbPass[RecCount][1];
                        GuestPass[2] = AbPass[RecCount][2];
                        GuestPass[3] = 0;

                        strncpy(AbName[RecCount], "GUEST", sizeof("GUEST"));
                    }

                    SetActiveUart(UART_0);													// TODO Remove debug
                    printf("%d Name is %s\n\r", RecCount + 1, AbName[RecCount]);	// TODO Remove debug
                    SetActiveUart(UART_1);													// TODO Remove debug

                    SetActiveUart(UART_0);																// TODO Remove debug
                    //					printf ("%d User password is %s\n\r", RecCount+1, AbPass[RecCount]);	// TODO Remove debug
                    printf("%d UPass is %s\n\r", RecCount + 1, AbPass[RecCount]);			// TODO Remove debug
                    SetActiveUart(UART_1);																// TODO Remove debug

                    AbProfile[RecCount] = AbRecord[RecCount][StarPos + 4];		// Записываем профиль абонента
                    AbCallTime[RecCount] = AbRecord[RecCount][StarPos + 5];		// Записываем время дозвона до абонента
                    AbCallNum[RecCount] = AbRecord[RecCount][StarPos + 6] - 48;		// Записываем количество попыток дозвона до абонента
                    AbExtService[RecCount] = AbRecord[RecCount][StarPos + 7] - 48;	// Записываем возможность дополнительного сервиса

                    SetActiveUart(UART_0);																			// TODO Remove debug
                    //					printf ("%d User profile is %c\n\r", RecCount+1, AbProfile[RecCount]);			// TODO Remove debug
                    printf("%d UProf is %c\n\r", RecCount + 1, AbProfile[RecCount]);					// TODO Remove debug
                    //					printf ("%d User one call time is %c\n\r", RecCount+1, AbCallTime[RecCount]);	// TODO Remove debug
                    printf("%d UOCT is %c\n\r", RecCount + 1, AbCallTime[RecCount]);					// TODO Remove debug
                    //					printf ("%d User calling attempt is %c\n\r", RecCount+1, AbCallNum[RecCount]);// TODO Remove debug
                    printf("%d UCA is %c\n\r", RecCount + 1, AbCallNum[RecCount]);						// TODO Remove debug
                    //					printf ("%d User ext service is %c\n\r", RecCount+1, AbExtService[RecCount]);	// TODO Remove debug
                    printf("%d UES is %d\n\r", RecCount + 1, AbExtService[RecCount]);					// TODO Remove debug
                    SetActiveUart(UART_1);																			// TODO Remove debug

                    if (AbRecord[RecCount][StarPos + 8] != '*')					// Не найден символ конца записи
                    {
                        IncorrectAb = RecCount + 1;							// Нулевой абонент в базе системы соответствует первому абоненту в SIM-карте
                        return 0;
                    }//AbRecord != '*'
                }//else ##
                SetActiveUart(UART_0);																	// TODO Remove debug
                //				printf ("%d User activity is %d\n\r", RecCount+1, AbActive[RecCount]);	// TODO Remove debug
                printf("%d UA is %d\n\r", RecCount + 1, AbActive[RecCount]);					// TODO Remove debug
                SetActiveUart(UART_1);																	// TODO Remove debug
            }// Answer OK - Запись найдена
            else
            {
                IncorrectAb = RecCount + 1;			// Нулевой абонент в базе системы соответствует первому абоненту в SIM-карте
                return 0;									// Одна из записей не найдена
            }//Answer Error
        }//for RecCount

        printf("AT+CPBR=6\n\r");						// Запрос сервисной записи
        delay_ms(2000);
        StringBuilder();
        if (strstr(ReceiveString, "OK") != NULL)	// Если GSM-модуль выдал подтверждение...
        {
            SetActiveUart(UART_0);						// TODO Remove debug
            //			printf("Service Answer OK\n\r");	  		// TODO Remove debug
            printf("SA OK\n\r");	  						// TODO Remove debug
            SetActiveUart(UART_1);						// TODO Remove debug
        }// Answer OK - Запись найдена
        else
        {
            SetActiveUart(UART_0);						// TODO Remove debug
            //			printf("Service Answer Error\n\r");		// TODO Remove debug
            printf("SA Error\n\r");						// TODO Remove debug
            SetActiveUart(UART_1);						// TODO Remove debug
            IncorrectAb = RecCount + 1;				// Нулевой абонент в базе системы соответствует первому абоненту в SIM-карте
            return 0;										// Сервисная запись не найдена
        }//Answer Error

        QuotesCount = 0;
        InNumCount = 0;
        InRecCount = 0;

        for (CharCount = 0; CharCount < strlen(ReceiveString); CharCount++)
        {
            if (QuotesCount == 1)
            {
                if (ReceiveString[CharCount] != '"')
                {
                    ServiceNum[InNumCount] = ReceiveString[CharCount];		// Формируем строку Номер
                    ServiceNum[InNumCount + 1] = 0;
                }

                if (InNumCount == SERVICE_NUM_LENGTH + 1)
                {
                    IncorrectAb = RecCount + 1;						// Нулевой абонент в базе системы соответствует первому абоненту в SIM-карте
                    return 0;												// Сервисный номер слишком длинный
                }
                InNumCount++;
            }//QuotesCount==1
            if (QuotesCount == 3)
            {
                if (ReceiveString[CharCount] != '"')
                {
                    ServiceName[InRecCount] = ReceiveString[CharCount];	// Формируем строку Запись
                    ServiceName[InRecCount + 1] = 0;
                }

                if (InRecCount == MAX_RECORD_LENGTH)
                {
                    IncorrectAb = RecCount + 1;						// Нулевой абонент в базе системы соответствует первому абоненту в SIM-карте
                    return 0;												// Сервисная запись слишком длинная
                }
                InRecCount++;
            }//QuotesCount==3
            if (ReceiveString[CharCount] == '"')					// Нашли еще одну кавычку
                QuotesCount++;
        }//for CharCount

        SetActiveUart(UART_0);											// TODO Remove debug
        printf("SN is %s\n\r", ServiceNum);						// TODO Remove debug
        printf("SR is %s\n\r", ServiceName);						// TODO Remove debug
        SetActiveUart(UART_1);											// TODO Remove debug

        if (!((strncmp(ServiceName, "Service", sizeof("Service")) == 0)
            || (strncmp(ServiceName, "SERVICE", sizeof("SERVICE")) == 0)
            || (strncmp(ServiceName, "service", sizeof("service")) == 0)))
        {
            IncorrectAb = RecCount + 1;								// Нулевой абонент в базе системы соответствует первому абоненту в SIM-карте
            return 0;														// Сервисная запись имеет имя, отличающееся от "Service"
        }

        if (isdigit(ServiceNum[0]))
        {
            DelayGuardOnTime = ServiceNum[0] - 48;					// Задержка при постановке на охрану, в минутах. 48d = '0'
            SetActiveUart(UART_0);																				// TODO Remove debug
            printf("DGOnT = %d m\n\r", DelayGuardOnTime);												// TODO Remove debug
            SetActiveUart(UART_1);																				// TODO Remove debug
        }
        else																	// DelayGuardOnTime должно быть описано цифрой
        {
            IncorrectAb = RecCount + 1;								// Нулевой абонент в базе системы соответствует первому абоненту в SIM-карте
            return 0;
        }

        if (isdigit(ServiceNum[1]))
        {
            DelayGuardOffTime = 10 * (ServiceNum[1] - 48);			// Задержка при снятии с охраны, в секундах. 48d = '0'
            DelayNoActiveTime = DelayGuardOffTime;
            SetActiveUart(UART_0);																				// TODO Remove debug
            printf("DGOT = %d s\n\r", DelayGuardOffTime);												// TODO Remove debug
            SetActiveUart(UART_1);																				// TODO Remove debug
        }
        else																	// DelayGuardOffTime должно быть описано цифрой
        {
            IncorrectAb = RecCount + 1;								// Нулевой абонент в базе системы соответствует первому абоненту в SIM-карте
            return 0;
        }

        if ((isdigit(ServiceNum[2])) && ((ServiceNum[2] == '0') || (ServiceNum[2] == '1')))
        {
            iButtonUse = ServiceNum[2] - 48;							// Использование ключей iButton. 48d = '0'
            SetActiveUart(UART_0);																					// TODO Remove debug
            printf("iBU = %d\n\r", iButtonUse);																// TODO Remove debug
            SetActiveUart(UART_1);																					// TODO Remove debug
        }
        else																	// iButtonUse должно быть описано цифрой
        {
            IncorrectAb = RecCount + 1;								// Нулевой абонент в базе системы соответствует первому абоненту в SIM-карте
            return 0;
        }

        if (isdigit(ServiceNum[3]))
        {
            LockOpenTime = ServiceNum[3] - 48;						// Время удержания электромеханического замка, в секундах. 48d = '0'
            SetActiveUart(UART_0);																					// TODO Remove debug
            printf("LOT = %d second(s)\n\r", LockOpenTime);												// TODO Remove debug
            SetActiveUart(UART_1);																					// TODO Remove debug
        }
        else																	// LockOpenTime должно быть описано цифрой
        {
            IncorrectAb = RecCount + 1;								// Нулевой абонент в базе системы соответствует первому абоненту в SIM-карте
            return 0;
        }

        if (isdigit(ServiceNum[4]))
        {
            SirenSingTime = 10 * (ServiceNum[4] - 48);				// Время работы сирены, в секундах. 48d = '0'
            SetActiveUart(UART_0);																					// TODO Remove debug
            printf("SST = %d seconds\n\r", SirenSingTime);													// TODO Remove debug
            SetActiveUart(UART_1);																					// TODO Remove debug
        }
        else																	// SirenSingTime должно быть описано цифрой
        {
            IncorrectAb = RecCount + 1;								// Нулевой абонент в базе системы соответствует первому абоненту в SIM-карте
            return 0;
        }
    }// else GSMStatus
    SetActiveUart(UART_0);							// TODO Remove debug
    printf("End of AB\n\r");	 	 				// TODO Remove debug
    SetActiveUart(UART_1);							// TODO Remove debug

    IncorrectAb = 0;
    return 1;
}//ReadAbRecords

static unsigned int BufferCounter = 0;
static unsigned int PageCounter = 0;

volatile unsigned char Timer1OverCount = 0;

#pragma vector = TIMER1_OVF_vect

#define	INTRO_START		0														// Начало фразы "Внимание, сработала сигнализация. Шлейф номер..." в прошитом звуковом файле в секундах
#define	INTRO_START_DF	INTRO_START*(SOUND_FACTOR/MAX_BUFFER)		// Страница DataFlash
#define	INTRO_END		4														// Конец фразы "Внимание, сработала сигнализация. Шлейф номер..." в прошитом звуковом файле в секундах
#define	INTRO_END_DF	INTRO_END*(SOUND_FACTOR/MAX_BUFFER)			// Страница DataFlash

void GuardReaxion(void)									// Реакция на срабатывание охранной системы
{
    unsigned char ReaxCount = 0;
    unsigned char ReaxTemp = 0;
    char HackLoops[sizeof(" 1 2 3")] = "";
    unsigned char AbCallConfirm[MAX_AB_NUMBER];	// Абоненту, до которого дозвонились, присваивается 1, до которого не дозвонились - 0
    unsigned char ReaxCallTimeout = 0;
    unsigned char ReaxCallNum = 0;
    unsigned char ReaxVoiceAttempt = 0;
#define VOICE_ATTEMPT_NUM 7

    SetActiveUart(UART_0);												// TODO Remove debug
    printf("\n\rALARMA!!!!!\n\r");									// TODO Remove debug
    SetActiveUart(UART_1);												// TODO Remove debug

    TmSirenSingTime = SirenSingTime;					// Загружаем в TmSirenSingTime полное время работы

    // Рассылка SMS-сообщений абонентам категорий A и C и абонентам с включенным дополнительным сервисом
    if (Loop1Violation == 1)
        strncpy(HackLoops, " 1", sizeof(" 1"));
    if (Loop2Violation == 1)
        strncpy(HackLoops, " 2", sizeof(" 2"));
    if (Loop3Violation == 1)
        strncpy(HackLoops, " 3", sizeof(" 3"));

    for (ReaxCount = 0; ReaxCount < MAX_AB_NUMBER; ReaxCount++)
    {
        if ((strncmp(AbName[ReaxCount], "GUEST", sizeof("GUEST")) != 0)
            && (strncmp(AbName[ReaxCount], "##", sizeof("##")) != 0)
            && ((AbProfile[ReaxCount] == 'A') || (AbProfile[ReaxCount] == 'C') || (AbExtService[ReaxCount] == 1)))	// Если пользователь задействован и он не GUEST и не заблокирован
        {
            printf("AT+CMGS=\"%s\"\n\r", AbNumber[ReaxCount]);
            delay_ms(2000);
            printf("Message to %s: ALARM! LINE(S)%s.%c", AbName[ReaxCount], HackLoops, CtrlZ);						// TODO Remove debug Message to - убрать, это только для отладки

            StringBuilder();
            ReaxTemp = 0;
            while ((strstr(ReceiveString, "OK") == NULL) && (strstr(ReceiveString, "ERROR") == NULL) && (ReaxTemp < 10))	// Пока не придет "OK", "ERROR" или не истечет таймаут в 10 секунд.
            {
                ReaxTemp++;
                StringBuilder();
                delay_ms(1000);
            }//Ожидание ответа от GR-47
        }//Определение абонента, требующего SMS-сообщения
    }//Рассылка SMS с предупреждением о срабатывании сигнализации завершена

    // Обзвон абонентов
    for (ReaxCount = 0; ReaxCount < MAX_AB_NUMBER; ReaxCount++)
    {
        if ((strncmp(AbName[ReaxCount], "GUEST", sizeof("GUEST")) != 0)
            && (strncmp(AbName[ReaxCount], "##", sizeof("##")) != 0)
            && ((AbProfile[ReaxCount] == 'A') || (AbProfile[ReaxCount] == 'B')))	// Если пользователь задействован и он не GUEST и не заблокирован
        {
            clrbit(TIMSK, TOIE0);											// Запретить прерывание по переполнению от таймера 0 (чтобы не мешал разборчивому воспроизведению речи)
            setbit(PORTC, GSMLED);											// Горит постоянно - режим разговора

            ReaxCallNum = AbCallNum[ReaxCount];

            while (ReaxCallNum != 0)
            {
                switch (AbCallTime[ReaxCount])
                {
                case '0':
                    ReaxCallTimeout = 30; break;								// Время дозвона до данного абонента в секундах
                case '1':
                    ReaxCallTimeout = 60; break;
                case '2':
                    ReaxCallTimeout = 120; break;
                default:
                    ReaxCallTimeout = 40; break;
                }

                AbCallConfirm[ReaxCount] = 0;
                printf("ATD %s;\n\r", AbNumber[ReaxCount]);
                while (ReaxCallTimeout != 0)
                {
                    printf("AT+CPAS=1\n\r");
                    delay_ms(1000);
                    StringBuilder();

                    if (strstr(ReceiveString, "130") != NULL)					// Если GSM-модуль перешел в режим разговора
                    {
                        DTMFPassPos = 0;
                        DTMFPassBuf[0] = 0;
                        ReaxVoiceAttempt = 0;

                        while ((ReaxVoiceAttempt++ < VOICE_ATTEMPT_NUM) && (DTMFPassBuf[0] != DTMF_SHARP))	// Повторяем фразу несколько раз и ждем, пока пользователь нажмет '#'
                        {
                            DTMFPassPos = 0;
                            DTMFPassBuf[0] = 0;

                            DF_PagesToSpeaker(INTRO_START_DF, INTRO_END_DF);	// "Внимание, сработала сигнализация. Шлейф номер..."
                            if (Loop1Violation == 1)
                                DF_PagesToSpeaker(ODIN_START_DF, ODIN_END_DF);	// "...один..."
                            if (Loop2Violation == 1)
                                DF_PagesToSpeaker(DVA_START_DF, DVA_END_DF);		// "...два..."
                            if (Loop3Violation == 1)
                                DF_PagesToSpeaker(TRI_START_DF, TRI_END_DF);		// "...три..."
                            printf("AT+CPAS=1\n\r");
                            delay_ms(500);
                            StringBuilder();
                            if (strstr(ReceiveString, "129") != NULL)				// Если абонент положил трубку
                                break;
                        }

                        AbCallConfirm[ReaxCount] = 1;
                        ReaxCallNum = 0;
                        break;
                    }
                    if (strstr(ReceiveString, "129") != NULL)					// Если абонент так и не взял трубку до истечения таймаута оператора
                        break;
                    ReaxCallTimeout--;
                }
                StringBuilder();
                printf("ATH\n\r");												// Кладем трубку
                delay_ms(2000);
                StringBuilder();
                if (ReaxCallNum != 0) ReaxCallNum--;
            }

            clrbit(PORTC, GSMLED);											// Гасим
            setbit(TIMSK, TOIE0);											// Разрешить прерывание по переполнению от таймера 0
        }//Определение абонента, требующего звонка
    }//Обзвон абонентов с предупреждением о срабатывании сигнализации завершен

    // Рассылка SMS-сообщений тем абонентам категории B, до которых мы так и не смогли дозвониться
    for (ReaxCount = 0; ReaxCount < MAX_AB_NUMBER; ReaxCount++)
    {
        if ((strncmp(AbName[ReaxCount], "GUEST", sizeof("GUEST")) != 0)
            && (strncmp(AbName[ReaxCount], "##", sizeof("##")) != 0)
            && ((AbProfile[ReaxCount] == 'B') && (AbCallConfirm[ReaxCount] == 0)))
        {
            printf("AT+CMGS=\"%s\"\n\r", AbNumber[ReaxCount]);
            delay_ms(2000);
            printf("Message to %s: ALARM! LINE(S)%s.%c", AbName[ReaxCount], HackLoops, CtrlZ);										// TODO Remove debug Message to - убрать, это только для отладки

            StringBuilder();
            ReaxTemp = 0;
            while ((strstr(ReceiveString, "OK") == NULL) && (strstr(ReceiveString, "ERROR") == NULL) && (ReaxTemp < 10))	// Пока не придет "OK", "ERROR" или не истечет таймаут в 10 секунд.
            {
                ReaxTemp++;
                StringBuilder();
                delay_ms(1000);
            }//Ожидание ответа от GR-47
        }//Определение абонента, требующего SMS-сообщения
    }//Рассылка SMS "недозвоненным" абонентам завершена
}//GuardReaxion

void SendMessageToExtUser(char* MessageStr)	// Отсылка сообщений пользователям, у которых включен дополнительный сервис. Если дополнительный сервис не включен ни у одного абонента, рассылки просто не будет. fw111
{
    unsigned char ExtUserCount = 0;
    unsigned char ExtUserTemp = 0;

    SetActiveUart(UART_0);												// TODO Remove debug
    printf("Message =%s\n\r", MessageStr);							// TODO Remove debug
    SetActiveUart(UART_1);												// TODO Remove debug

    clrbit(TIMSK, TOIE0);												// Запретить прерывание по переполнению от таймера 0

    for (ExtUserCount = 0; ExtUserCount < MAX_AB_NUMBER; ExtUserCount++)
    {
        if ((strncmp(AbName[ExtUserCount], "GUEST", sizeof("GUEST")) != 0)
            && (strncmp(AbName[ExtUserCount], "##", sizeof("##")) != 0)
            && (AbExtService[ExtUserCount] == 1))	// Если пользователь задействован и он не GUEST и имеет право на дополнительный сервис
        {
            printf("AT+CMGS=\"%s\"\n\r", AbNumber[ExtUserCount]);
            delay_ms(2000);
            printf("Message to %s:%s.%c", AbName[ExtUserCount], MessageStr, CtrlZ);										// TODO Remove debug Message to - убрать, это только для отладки

            StringBuilder();
            ExtUserTemp = 0;
            while ((strstr(ReceiveString, "OK") == NULL) && (strstr(ReceiveString, "ERROR") == NULL) && (ExtUserTemp < 10))	// Пока не придет "OK", "ERROR" или не истечет таймаут в 10 секунд.
            {
                ExtUserTemp++;
                StringBuilder();
                delay_ms(1000);
            }//Ожидание ответа от GR-47
        }//Определение абонента, требующего SMS-сообщения
    }//Рассылка SMS завершена

    setbit(TIMSK, TOIE0);											// Разрешить прерывание по переполнению от таймера 0
}//SendMessageToExtUser

#endif
