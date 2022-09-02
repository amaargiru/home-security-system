// ��������� � ������ ��������� = 3 �������
#ifndef _kedahm_
#define _kedahm_

#include <iom128.h>			// ����������� ���������� ���������
#include <inavr.h>			// Intrinsic-�������
#include <stdio.h>			// ����������� ����-�����
#include <ctype.h>			// �������� � ���������
#include <string.h>			// �������� �� ��������
#include "bitbyt.h"			// ����������� �������� � ������
#include "portdefines.h"	// "����������" ������
#include "madelay.h"			// ��������
#include "eeprom.h"			// �������� ���������� � EEPROM
#include "ds1990a.h"			// ������ � ������� iButton

#define ADC_VREF_TYPE 0x00	// ����� �������� ���������� ���. 0x00 -> ����� VREF

#define TXDELAY		10000	// �������� ��� ������ ������� ����� USART

#define CtrlZ			0x1A	// ��� Ctrl-Z ��� GSM-������

#define T1_OVF  		0x01
#define DF_BIT_READY	7

#define MAX_BUFFER   264
#define MAX_PAGE     2048

unsigned int temp  = 0;		// ��������� ���������� ��� ������ ��������� �����
unsigned int temp1 = 0;
char LoopResult = 0;
unsigned char InboxSMS = 0;		// fw116

unsigned char RepeatedKey = 0;	// RepeatedKey = 0, ���� ������� ����, ������� �������� ��������, ��� ����������� � ������ ������

// UART0
#define DATA_REGISTER_EMPTY_0 (1<<UDRE0)

// UART1
#define FRAMING_ERROR_1	(1<<FE1)
#define PARITY_ERROR_1	(1<<UPE1)
#define DATA_OVERRUN_1	(1<<DOR1)
#define DATA_REGISTER_EMPTY_1 (1<<UDRE1)
#define RXBUFLENGTH_1			256		// ����� ��������� ������ UART1
volatile char RxBuffer[RXBUFLENGTH_1];	// ����������� ����� ���������
volatile char RxBufWritePoint = 0;		// ���� �����
char RxBufReadPoint = 0;					// ������ ������
char ReceiveString[RXBUFLENGTH_1] = "";// ������ ������, ��������� ���������� StringBuilder

// ����������, ��������� � ���������� �������� �������
char AbRecReadComplete = 0;							// 0 - ������ ��������� �� ��������� ��� ��������� � ��������; 1 - ������ ��������� � �� ����� ������
#define MAX_AB_NUMBER		5							// ����� ���������
#define MAX_NUMBER_LENGTH	15							// ������������ ����� ����������� ������ ��������
#define MAX_RECORD_LENGTH	17							// ������������ ����� ������ ��������
#define MAX_NAME_LENGTH		8							// ������������ ����� ����� ��������
#define MAX_PASS_LENGTH		3							// ����� ������
#define SERVICE_NUM_LENGTH	5							// ����� ��������� ������

char AbNumber    [MAX_AB_NUMBER][MAX_NUMBER_LENGTH+1];// ������ ������� ���������
char AbRecord    [MAX_AB_NUMBER][MAX_RECORD_LENGTH+1];// ������ ������� ���������
char AbName      [MAX_AB_NUMBER][MAX_NAME_LENGTH+1];	// ������ ���� ���������
char AbPass      [MAX_AB_NUMBER][MAX_PASS_LENGTH+1];	// ������ ������� ���������
char GuestPass   [MAX_PASS_LENGTH+1];
char AbProfile   [MAX_AB_NUMBER];							// ������ �������� ���������
char AbCallTime  [MAX_AB_NUMBER];							// ����� �������
char AbCallNum   [MAX_AB_NUMBER];							// ���������� ������� �������
char AbExtService[MAX_AB_NUMBER];							// ��������� ��������������� �������
char AbActive    [MAX_AB_NUMBER];							// ������� ������� ������� �������� � ���������� �����

unsigned char IncorrectAb = 0;								// ����� �������� ���������� ������ �� SIM-�����. ���� ��� ������ �����, IncorrectAb = 0

// ����������, ���������� �� ������� SMS "220V OFF" � "220V ON"
char Sending220VOFF = 0;	// ��������� "220V OFF" �� ����������
char Sending220VON = 1;		// ��������� "220V ON" ����������

// ����������, ���������� �� ������� SMS "POWER OFF"
char SendingPOWOFF = 0;		// ��������� "POWER OFF" �� ����������

// ��������� ����������, ����������� � SIM-�����, ��� ������������� GSM-������/SIM-����� ��������� ��������� �� ���������
char DelayGuardOnTime = 3;		// �������� ��� ���������� �� ������, � �������
char DelayGuardOffTime = 30;	// �������� ��� ������ � ������, � ��������
char DelayNoActiveTime = 30;	// � ������� ����� ������� ������ ����� ��������� ������� �� ������ ����� ������. ��� �������� ������ ��������� ������� (fw101)
char iButtonUse = 0;				// ������������� ������ iButton
char LockOpenTime = 9;			// ����� ��������� �������������������� �����, � ��������
char SirenSingTime = 90;		// ����� ������ ������, � ��������

// ��������� ����������
char GuardSource = 0;		// �������� ���������� �� ������: 0 - �� ������ �� ����������; 1 - ���������� ���������; 2 - ���������� ����������� SMS; 3 - ���������� ������ �������; 4 - ���������� ������ iButton

char PreparationGuardTime = 0;
//unsigned char SMSandDTMFGuardOffCounter = 0;	// ������ ������ � ������� ���������� ������������ SMS-���������� ��� DTMF-�����. ����� ������ ���������� ���������� � ������� ������ �����, �� ��������� �� ��������� ������, � ���� �� ��������� - ����� ������� � ����� ������.

#define REPEAT_GUARD_TIME 180	// fw082

#define GS_SWITCHER	1
#define GS_SMS			2
#define GS_DTMF		3
#define GS_IBUTTON	4

char PowerCorrect = 1;		// ��������� �������� �������: 1 - ������� � �����; 0 - ���� �������� �������

char GSMStatus = 0;			// ��������� GSM-������: 0 - �������� ; 1 - �� ��������������� � ����; 2 - ��������������� � ����; 3 - ����� ���������
char GSMSigStrength = 0;	// ������� �������
char SIMNum = 0;				// ����� ������� SIM-�����: 0 - ������������� SIM-����� 1; 1 - ������������� SIM-����� 2
char SIMFault = 0;			// 0 - �������� � SIM-������; 1 - SIM-����� �������� ���������

char Loop1On = 0;				// ��������� ��������� ������ 1: 0 - �� ������������; 1 - ������������ � �������� �� ���������; 2 - ������������ � �������� �� ����������
char Loop2On = 0;				// ��������� ��������� ������ 2
char Loop3On = 0;				// ��������� ��������� ������ 3
#define HI_J_LIMIT 750		// ������� ������� ���������� �� ��������� 1 - 3
#define LO_J_LIMIT 250		// ������ ������� ���������� �� ��������� 1 - 3
char Loop1Status = 0;		// ��������� ��������� ������ 1: 0 - �����; 1 - ��; 2 - �����
char Loop2Status = 0;		// ��������� ��������� ������ 2
char Loop3Status = 0;		// ��������� ��������� ������ 3
#define HI_LOOP_LIMIT 527	// ������� ������� ������� ���� ������� ������. ������������� ���������� ������������� ����� 1 ���
#define LO_LOOP_LIMIT 304	// ������ ������� ������� ���� ������� ������. ������������� ���������� ������������� ����� 2 ���
char Loop1Violation = 0;	// ���� Loop1Violation == 1, ����� 1 ��� �������
char Loop2Violation = 0;	// ���� Loop2Violation == 1, ����� 2 ��� �������
char Loop3Violation = 0;	// ���� Loop3Violation == 1, ����� 3 ��� �������
char NewLoopViolation = 0;	// ���� NewLoopViolation != OldLoopViolation, ������ ���� �� ������� ��� ������� �������
char OldLoopViolation = 0;
char OldLoop1Violation = 0;
char OldLoop2Violation = 0;
char OldLoop3Violation = 0;
char LoopViolation = 0;
char GuardSourceL1 = 1;		// ��������� 1-�� ��������� ������ ��������� ��������� ������� � ����� ������
char GuardSourceL2 = 1;		// ��������� 2-�� ��������� ������ ��������� ��������� ������� � ����� ������
char GuardSourceL3 = 1;		// ��������� 3-�� ��������� ������ ��������� ��������� ������� � ����� ������

char TotalHackLoops = 0;	// ����� �������� �������
char HackLoopsCount = 0;	// ������� ���������� ������� (��� ��������� ���������� �������)
#define NLOOPS 3				// ����� ���������� �������
char IndicatedLoops[NLOOPS] = {0, 0, 0};	// ������ ������� ���������� �������, ������� ����� ������������

char AccStatus = 0;			// ��������� �������: 0 - �� ����������; 1 - ����������
#define  ACC_CORRECT  499	// ��� ���������� AccVoltage ������� ACC_CORRECT ����������� ����������� � ����� ��������� �������
#define  ACC_FAULT    439	// ��� ���������� AccVoltage ������� ACC_FAULT � ���������� �������� ������� ������� ����������� � ���������������� �����

char Relay1On = 0;			// ��������� ���� 1: 0 - ���������; 1 - ��������
char Relay2On = 0;			// ��������� ���� 2

char Out1On = 0;				// ��������� ����� 1:  0 - ��������; 1 - �������
char Out2On = 0;				// ��������� ����� 2

#define SMS_TIMEOUT 30		// ������� � �������� ��� �������� SMS
char EconoMode = 0;			// ����� �����������������: 0 - ������� �����; 1 - ���������������� �����
char SMSorDTMFEconoMode = 0;	// �������������� ���������������� �����: 0 - ��� ��������������� �������� � ���������������� �����; 1 - ���������� ��������������� �������� � ���������������� ����� ��� ���������� �������� �������

char iButtonMode = 0;		// ����� ������ � ������� iButton: 0 - �������; 1 - ������ ������; 2 - �������� ������

unsigned int CurrentADCRead = 0;	// ��������, ��������� � ���

typedef enum {UART_0, UART_1} t_uarts;				// ����� UART'�
static t_uarts g_currentUsart = UART_1;

void BeginInit();											// ��������� ������������� ��������� ����������������
unsigned int ReadADC(unsigned char ADCInput);	// ������ ������ ���
void SetActiveUart (t_uarts u) 						// ����� �������� UART
{
	g_currentUsart = u;
}
int  putchar( int data );								// ����� ����� � UART (��� printf())
void ClearRxBuffer(void);								// ������� ��������� ������ UART1
void SystemUpdate(void);								// ��������������� ��������� �������
void GR_On(void);											// �������� GR-47
void GR_Off(void);										// ��������� GR-47
void StringBuilder(void);								// ������ ������� �� ������ � �������� ������
char ReadAbRecords(void);								// ������ ������� ���������. ���� ������ ��������� � �� ����� ������, ������������ �������� == 1, ����� == 0
char LoopTest(void);										// ��������� ��������� �������� �������. ���� ��������� ����������, ���������� 1, ����� 0
int StrToInt (char* InputStr);						// ��������� ������������ ������ � ����� ���� int. �������������� ������ �����
//char* ToUpperString(char* UpString);				// ����������� ������ � ������� �������
void SendMessageToExtUser (char* MessageStr);	// ������� ��������� �������������, � ������� ������� �������������� ������. ���� �������������� ������ �� ������� �� � ������ ��������, �������� ������ �� �����. fw111

void iButtonNewKeysSearch(void);						// ������������� � ������ � EEPROM ����� ������ iButton
void iButtonKeysErase(void);							// �������� ������ iButton �� EEPROM
unsigned char iButtonKeyScan(void);					// ���������� 1, ���� �������� ���� �� ���������� � ������ ������ ��� 0, ���� ������ ����� �� ����������

void DF_WriteSPI(unsigned char data);				// ������ ����� �� SPI
unsigned char DF_Status( void );						// ������ ����� ������� DataFlash
unsigned char DF_Ready( void );						// ����������� ������ ����� Ready/~Busy
void DF_NextPageToNextBuffer (unsigned char active_buffer, unsigned int page_counter);
void DF_Playback (void);
void DF_ActiveBufferToSpeaker (unsigned char active_buffer);
void DF_MainMemoryToSpeaker(unsigned int Page);
void DF_PagesToSpeaker(unsigned int StartPage, unsigned int EndPage);

void GuardIn (unsigned char InGuardSource);		// ������ ����� ����� � ����� ������
void GuardOut (unsigned char OutGuardSource);	// ������ ����� ������ �� ������ ������

void LoopsRead (void);

void GuardReaxion(void);								// ������� �� ������������ �������� �������

void BeginInit(void)
{
	//	������������� ����� A
	DDRA = (1<<POUT2);
	PORTA = (1<<JIBP) | (1<<JIBM);	// ��� ��������� pull-up

	//	������������� ����� B
	DDRB = ((1<<SCK) | (1<<SI) | (1<<PWDN) | (1<<CHARG) | (1<<SPKOUT) | (POUT1));
	PORTB = 0;

	//	������������� ����� C
	DDRC = ((1<<SIMSW) | (1<<EGRDLED) | (1<<EGSMLED) | (1<<EPOWLED) | (1<<GRDLED) | (1<<GSMLED) | (1<<POWLED));
	PORTC = 0;

	//	������������� ����� D
	DDRD = ((1<<PON) | (1<<PDTR) | (1<<PTD) | (1<<PRTS));
	PORTD = 0;

	//	������������� ����� E
	DDRE = (1<<CON) | (1<<TXD0);
	PORTE = (1<<CON);												// �������� ������� �����

	//	������������� ����� F
	DDRF = 0;
	PORTF = 0;

	//	������������� ����� G
	DDRG = ((1<<nFCS) | (1<<nFRES) | (1<<nGSMPOW) | (1<<K1) | (1<<K2));
	PORTG = (1<<nFRES);

	// ������������� ���
	ADMUX = ADC_VREF_TYPE;
	ADCSRA = 0x87;
	SFIOR &= 0xEF;

	// ������������� USART0
	UCSR0A = 0x00;
	UCSR0B = 0x08;
	UCSR0C = 0x06;
	UBRR0H = 0x00;
	UBRR0L = 0x33;

	// ������������� USART1
	UCSR1A = 0x00;
	UCSR1B = 0x98;
	UCSR1C = 0x06;
	UBRR1H = 0x00;
	UBRR1L = 0x33;

	// ������������� �������
	TCCR0 = 7;			// ������������ �� ���������� �����, � ������������� 1024 (����� ������������ 30 �� @ 8.0 ���)
	TIMSK = 1<<TOIE0;	// ��������� ���������� �� ������������ �� ������� 0 (OCIE0!!!)

	// ������������� �������� ���������� - INT4 ����������� �� �����
	EICRA = 0x00;
	EICRB = 0x03;
	EIMSK = 0x10;
	EIFR  = 0x10;

	// ������������� EEPROM. ���� � EEPROM �������� 0xFF, ������ ��������� �������� � ������ ��� � �������� Loop1Set ����� ��������� � 1 (��������� ���������, �������� �� ���������). ����� ������ ��������� �������� ��� ����� �������� EEPROM, ��� ������� ��� �������� ������������ ����� � ������.
	if (Loop1Set == 0xFF)
		Loop1Set = 1;
	if (Loop2Set == 0xFF)
		Loop2Set = 1;
	if (Loop3Set == 0xFF)
		Loop3Set = 1;
	if (CurrentKeys == 0xFF)
		CurrentKeys = 0;
}//BeginInit

unsigned int ReadADC(unsigned char ADCInput)	// ������ ������ ���
{
	ADMUX = ADCInput | ADC_VREF_TYPE;
	ADCSRA |= 0x40;
	while ((ADCSRA & 0x10) == 0);
	ADCSRA|=0x10;
	return (unsigned int)(ADCL | (ADCH<<8));
}//ReadADC

int putchar( int data )	// ����� ����� � UART (��� printf())
{
	if (g_currentUsart == UART_1)
	{
		while ( !( UCSR1A & DATA_REGISTER_EMPTY_1) );
		UDR1 = data;
	}// if UART_1
	else 
	{
		while ( !( UCSR0A & DATA_REGISTER_EMPTY_0) );
		UDR0 = data;
	}//else

	__delay_cycles(TXDELAY);
	return data;
}//putchar

void ClearRxBuffer(void)	// ������� ��������� ������ UART1
{
	unsigned int ClearPoint = 0;

	for (; ClearPoint++ < RXBUFLENGTH_1; RxBuffer[ClearPoint] = 0);

	RxBufReadPoint = 0;
	RxBufWritePoint = 0;
}//ClearRxBuffer

char LoopTest(void)			// ��������� ��������� �������� �������. ���� ��������� ����������, ���������� 1, ����� 0
{
	if (Loop1On == 1)												// ����� �������� �� ���������
		if ((Loop1Status == 0) || (Loop1Status == 2))	// ����� ��� �����
			GuardSourceL1 = 1;
		else GuardSourceL1 = 0;
	if (Loop1On == 2)												// ����� �������� �� ����������
		if ((Loop1Status == 0) || (Loop1Status == 1))	// ����� ��� ��
			GuardSourceL1 = 1;
		else GuardSourceL1 = 0;

	if (Loop2On == 1)												// ����� �������� �� ���������
		if ((Loop2Status == 0) || (Loop2Status == 2))	// ����� ��� �����
			GuardSourceL2 = 1;
		else GuardSourceL2 = 0;
	if (Loop2On == 2)												// ����� �������� �� ����������
		if ((Loop2Status == 0) || (Loop2Status == 1))	// ����� ��� ��
			GuardSourceL2 = 1;
		else GuardSourceL2 = 0;

	if (Loop3On == 1)												// ����� �������� �� ���������
		if ((Loop3Status == 0) || (Loop3Status == 2))	// ����� ��� �����
			GuardSourceL3 = 1;
		else GuardSourceL3 = 0;
	if (Loop3On == 2)												// ����� �������� �� ����������
		if ((Loop3Status == 0) || (Loop3Status == 1))	// ����� ��� ��
			GuardSourceL3 = 1;
		else GuardSourceL3 = 0;

	// ���� ��� ��� ����� �� ������������ ��� ���� �� ���� �� ������������ ������� ���������� - �� ������ ������� ������
	if ((GuardSourceL1 == 1) && (GuardSourceL2 == 1) && (GuardSourceL3 == 1) &&
		((Loop1On != 0) || (Loop2On != 0) || (Loop3On != 0))) return 1;
	else return 0;
}//LoopTest

#define DTMF_PASS_LENGTH 4										// ��������, "111#"
#define DTMF_COMMAND_LENGTH 2									// ��������, "1#"
volatile char DTMFPassBuf[DTMF_PASS_LENGTH+1] = "";	// ������ DTMF ������ ��� �������� DTMF �������
volatile unsigned char DTMFPassPos = 0;					// �������, ���������� � ������� ����������
#define DTMF_PASS_TIMEOUT 10									// ������� � �������� ��� ����������� DTMF ������
#define DTMF_COMMAMD_TIMEOUT 30								// ������� � �������� ��� ����� DTMF �������. ���� ������� �����, ������� ��������� ����������
unsigned char DTMFIncomingUser = 0;
#define DTMF_SHARP 12											// DTMF ��� ������� #

#pragma vector = INT4_vect										// ������� ���������� INT4
__interrupt void INT4_vect_Interrupt(void)
{
//	SetActiveUart(UART_0);										// !!!
//	printf("NEXT KEY\n\r");										// !!!
//	SetActiveUart(UART_1);										// !!!		

	if (DTMFPassPos < DTMF_PASS_LENGTH)						// �� ������ ������ ��������� ��������
	{
		DTMFPassBuf[DTMFPassPos] = ((PINE >> DTQ1) & 0x01) | ((PINE >> (DTQ2 - 1)) & 0x02) | ((PINE >> (DTQ3 - 2)) & 0x04) | ((PINE >> (DTQ4 - 3)) & 0x08);	// "����������" ���������� �� MT8870 � �����
		if (DTMFPassBuf[DTMFPassPos] == 10) DTMFPassBuf[DTMFPassPos] = 0;	// � DTMF 0 ���������� ��� 1010b = 10d
		DTMFPassPos++;

//		SetActiveUart(UART_0);									// !!!
//		printf("DTMFPassBuf = %d %d %d %d \n\r", DTMFPassBuf[0], DTMFPassBuf[1], DTMFPassBuf[2], DTMFPassBuf[3]);	// !!!
//		SetActiveUart(UART_1);									// !!!		
	}
}//INT4_vect_Interrupt

#pragma vector = USART1_RXC_vect	// ���������� �� ������� ����� � UART1
__interrupt void USART1_RXC_Interrupt(void)
{
	RxBuffer[RxBufWritePoint] = UDR1;
	if (RxBufWritePoint == RXBUFLENGTH_1-1)
		RxBufWritePoint = 0;
	else RxBufWritePoint++;
}//USART_RXC_Interrupt

volatile unsigned long Ticks = 0;							// ����� ������������ ������� 0 = ��������� �����
volatile unsigned char TmSS = 0;								// ������� ������� ��� ���������
volatile unsigned char TmIndLoop = 0;						// ������� ������������ ���������� �����
unsigned char AttemptInError = 0;							// ������ ���������� �� ������. ����� ������ ��� ���������. ����� == 1 -> ������
volatile unsigned char TmErrRec = 0;						// ����� ��������� ������ � SIM-����� ��� ���������
volatile unsigned char TmSirenSingTime = 0;				// ���������� ����� ������ ������
volatile unsigned char TmLockOpenTime = 0;				// ���������� �����, � ������� �������� ����� ����� ���������� ��������
volatile unsigned char TmGuardOffTimeout = 0;			// ���������� ����� �������� ��� ������ ������� � ������
volatile unsigned char TmNoActiveTime = 0;				// ���������� ����� �������� ��� ����������� ����� ���������� �� ������
volatile unsigned int  TmRepeatGuardOn = 0;				// �����, ����� ������� ������� �������� �������� � ����� ������ ����� ��������� (������� ��������� ������ � ��� ������, ���� ���������� ����� ������������). ������� �������������� ����� ��� ������ ����� ��������������. (fw104)

#pragma vector = TIMER0_OVF_vect								// ���������� ���������� �� ������� 0
__interrupt void TIMER0_OVF_Interrupt (void) 
{
	#define TmQuartSeconds 7									// 1/4 ������� ��� ������� ������������ ������� 30 ��
	#define TmHalfSeconds 15									// 1/2 �������
	#define TmFullSeconds 28									// 1 �������. �� ����� 30, �.�. �.�. ����� N*TmQuartSeconds
	#define TmTwoFullSeconds 56								// 2 �������. �� ����� 60, �.�. �.�. ����� N*TmQuartSeconds, ��� N - ����� �����. ����� ������� if (Ticks % TmFullSSPeriod == 0) {if (Ticks % TmTwoFullSeconds == 0)...} (������ ����) ����� ����������� �� ��� �����, ��� �������� �� :)
	#define TmFullSSPeriod 160									// ���� ��� � ������� ����� ����������� ���������� � ������ �������

	++Ticks;

	if	(Ticks % TmHalfSeconds == 0)							// ��������� ������� ������� ��� � ����������
		if (PowerCorrect == 0) invbit(PORTC, POWLED);	// ��� ���� �������� ������� - ������ � �������� 1 ��
		else setbit(PORTC, POWLED);							// ���� ������� ������� � ����� - ����� ���������

	if (Ticks % TmFullSSPeriod == 0)							// �������� �������� ������ ������� � ������ �������� ������ �� SIM-����� � ������������ ���������
	{
		TmSS = 2*GSMSigStrength;
		clrbit(PORTC, GSMLED);
		TmErrRec = 2*IncorrectAb;
		clrbit(PORTC, GRDLED);

		if (TotalHackLoops != 0)
		{
			TmIndLoop = 2*IndicatedLoops[HackLoopsCount];

			if (HackLoopsCount == (TotalHackLoops - 1))
				HackLoopsCount	= 0;
			else
				HackLoopsCount++;
		}
	}
	if	(Ticks % TmQuartSeconds == 0)							// ��������� ��������� GSM-������ ��� � �������� �������
	{
		if ((TotalHackLoops != 0) && (GuardSource == 0) && (TmIndLoop > 0))
		{
			invbit(PORTC, GRDLED); 															// ��������� ������
			TmIndLoop--;
		}			
		else

			if ((AttemptInError == 1) && (Ticks % TmFullSeconds == 0))
				invbit(PORTC, GRDLED);														// ������ � �������� 2 �� ����� ������� ��������� �� ������ � ������������ ��������
			else

				if ((GuardSource != 0)														// ���� ������� ��� ��������� �� ������...
				&& (Ticks % TmTwoFullSeconds == 0))
					{
						setbit(PORTC, GRDLED);												// ...�� ������ ��� � ��� �������
//						SetActiveUart(UART_0);												// !!!
//						printf(">\n\r", PreparationGuardTime);							// !!!
//						SetActiveUart(UART_1);												// !!!	
					}
				else

					if ((GuardSource != 0)													// ���� ������� ��� ��������� �� ������...
					&& (Ticks % TmFullSeconds == 0)										// fw110
					&& ((PreparationGuardTime > 0) || (TmGuardOffTimeout > 0)))	// fw110
						{
							setbit(PORTC, GRDLED);											// ...�� ������ ��� � �������
//							SetActiveUart(UART_0);											// !!!
//							printf("^\n\r", PreparationGuardTime);						// !!!
//							SetActiveUart(UART_1);											// !!!	
						}
					else

						if (TmErrRec > 0)														// ...��� ���������� ����� �������� ������ � SIM-�����, ���� ������� �������
						{
							invbit(PORTC, GRDLED);
							TmErrRec--;
						}
						else	clrbit(PORTC, GRDLED);										// ��������� �������

		if (GSMStatus == 0) clrbit(PORTC, GSMLED);		// �� ��������� SIM-����� ��� GSM-������ �� ����� �� ��������� - ����� ���������
		if (GSMStatus == 1) invbit(PORTC, GSMLED);		// ������ � �������� 2 ��
		if (GSMStatus == 2)										// ���� GSM-������ ��������������� � ���� - ���������� ������� �������
		{
			if (TmSS > 0)											// ��������� ������
			{
				invbit(PORTC, GSMLED);
				TmSS--;
			}
			else clrbit(PORTC, GSMLED);						// ��������� �������
		}
		if (GSMStatus == 3)										// ����� ��������� - ����� ���������
			setbit(PORTC, GSMLED);
	}
	
	if	((GuardSource != 0) && (Ticks % TmFullSeconds == 0))	// ���� ������� ����� �� ������ - ��������� ��������� �������� ��� ���������� �� ������
		if (PreparationGuardTime != 0)
		{
			PreparationGuardTime--;										// ���� ����� �������� ��� �� �������, �� ��� ���������
			SetActiveUart(UART_0);										// !!!
			printf("PGT=%d\n\r", PreparationGuardTime);			// !!!
			SetActiveUart(UART_1);										// !!!			
		}

	if	(Ticks % TmFullSeconds == 0)									// ��������� ��������� ������� � ������ ������ ��� � �������
	{
		if (TmRepeatGuardOn > 0)										// (fw104)
		{
			SetActiveUart(UART_0);										// !!!
			printf("TRGO=%d\n\r", TmRepeatGuardOn);				// !!!
			SetActiveUart(UART_1);										// !!!

			TmRepeatGuardOn--;
		}
		else
		{
			Loop1Violation = 0;											// ����� �������� ���������� ���������� ������ (fw104)
			Loop2Violation = 0;
			Loop3Violation = 0;
			NewLoopViolation = 0;
			OldLoopViolation = 0;

//			SetActiveUart(UART_0);										// !!!
//			printf("Loops recovered\n\r");							// !!!
//			SetActiveUart(UART_1);										// !!!
		}

		if (TmSirenSingTime > 0)										// ��� ��������, ��� ���������� �������� ��������� GuardReaxion � ����� �������� ������
		{
			setbit(PORTB, POUT1);
			TmSirenSingTime--;											// ������ �������� �������� �� ������� ������
		}
		else
			clrbit(PORTB, POUT1);

		if (TmLockOpenTime > 0)											// ��� ��������, ��� ���������� �������� ��������� GuardOut � ����� ������� �����
		{
			setbit(PORTA, POUT2);
			TmLockOpenTime--;												// ����� ����� ������ �� ������� ������

//			SetActiveUart(UART_0);										// !!!
//			printf("O!\n\r", TmGuardOffTimeout);					// !!!
//			SetActiveUart(UART_1);										// !!!
		}
		else
			clrbit(PORTA, POUT2);

		if (TmGuardOffTimeout > 0)										// ��� ��������, ��� ������ �������� � ������� ������� ��� ������ � ������ ��� ������� ������ ��������� �������� �����������
		{
			TmGuardOffTimeout--;											// ��� �� ������� ������

			SetActiveUart(UART_0);										// !!!
			printf("TmGOT=%d\n\r", TmGuardOffTimeout);			// !!!
			SetActiveUart(UART_1);										// !!!
		}

		if (TmNoActiveTime > 0)											// ��� ��������, ��� ������� ����� � ������ � ��������� ����� ����� ������ � ����� ������ �� ������ (fw101)
			TmNoActiveTime--;												// ��� �� ������� ������

		LoopsRead();														// ����������� ��������� �������� ������� (fw104)
		LoopResult = LoopTest();										// (fw104)
		if (LoopResult == 0)
			TmRepeatGuardOn = 3*60;										// ��� ������. ��������� ������ ���� ��� ������������ ������ (fw104)

		if ((GuardSource != 0) && (PreparationGuardTime == 0))// ���� ������� ���������� �� ������ � �������� ��� ���������� �� ������ �������
		{
			if (LoopResult == 1)											// �� �������� ��� � �������
			{
				SetActiveUart(UART_0);									// !!!
				printf("*\n\r");											// !!!
				SetActiveUart(UART_1);									// !!!
			}
			else																// ��������� �������� �������
			{
				SetActiveUart(UART_0);									// !!!
				printf("Alarm!\n\r");									// !!!
				SetActiveUart(UART_1);									// !!!

				TmRepeatGuardOn = 3*60;									// ��� ������

				OldLoopViolation = NewLoopViolation;

				OldLoop1Violation = Loop1Violation;
				OldLoop2Violation = Loop2Violation;
				OldLoop3Violation = Loop3Violation;

				if (GuardSourceL1 == 0) Loop1Violation = 1;		// ���������� ������ ���������� �������
				if (GuardSourceL2 == 0) Loop2Violation = 1;
				if (GuardSourceL3 == 0) Loop3Violation = 1;

				NewLoopViolation = Loop1Violation + Loop2Violation + Loop3Violation;

//				SetActiveUart(UART_0);									// !!!
//				printf("NLV=%d\n\r", NewLoopViolation);			// !!!
//				printf("OLV=%d\n\r", OldLoopViolation);			// !!!
//				SetActiveUart(UART_1);									// !!!

				if (NewLoopViolation > OldLoopViolation)	
				{
					LoopViolation = 1;									// ������������ ��������� ���������. LoopViolation = 0 ������ ����� ��������� GuardReaxion() [��� � ��������� ������ � ������ (fw109)]
					if ((GuardSource == GS_IBUTTON) || ((GuardSource == GS_SWITCHER) && (iButtonUse == 1)))
					{
						TmGuardOffTimeout = 0;							// ����������� ������� (fw101)
						SetActiveUart(UART_0);							// !!!
						printf("IMMEDIATE-ACTION ALARM!!!\n\r");	// !!! fw115
						SetActiveUart(UART_1);							// !!!
					}
					else
					TmGuardOffTimeout = DelayGuardOffTime;			// ���������� ����� �������� �� ���������� ��������� �������� ������������ (�� ������, ���� ����� ������� ���������, ������� ������ ������� ������������ � ������)
				}

				if (OldLoop1Violation != Loop1Violation)
				{
					IndicatedLoops[TotalHackLoops] = 1;
					TotalHackLoops++;										// ������� ��� ���� ����� (� ������ ������ - ������)
				}
				if (OldLoop2Violation != Loop2Violation)
				{
					IndicatedLoops[TotalHackLoops] = 2;
					TotalHackLoops++;										// ������� ��� ���� ����� (� ������ ������ - ������)
				}
				if (OldLoop3Violation != Loop3Violation)
				{
					IndicatedLoops[TotalHackLoops] = 3;
					TotalHackLoops++;										// ������� ��� ���� ����� (� ������ ������ - ������)
				}

//				SetActiveUart(UART_0);										// !!!
//				printf("THL = %d\n\r", TotalHackLoops);				// !!!
//				SetActiveUart(UART_1);										// !!!

				SetActiveUart(UART_0);									// !!!
				printf("Viol = %d %d %d = %d\n\r", Loop1Violation, Loop2Violation, Loop3Violation, LoopViolation);	// !!!
				SetActiveUart(UART_1);									// !!!
			}
		}//�������� ������� � ������ ������
	}
}//TIMER0_OVF_Interrupt

signed char ReadData(void)
{
	char data;

	if (RxBufWritePoint == RxBufReadPoint) return -1;	// ����� ������ �� �������

	data = RxBuffer[RxBufReadPoint];

	if(RxBufReadPoint == RXBUFLENGTH_1-1) RxBufReadPoint = 0;
	else RxBufReadPoint++;
	
	return data;
}//ReadData

void SystemUpdate(void)	// ��������������� ��������� �������
{
	unsigned char MessageDecoding = 0;		// ������ ���������, ���� MessageDecoding != 0
	#define MAX_LENGTH (sizeof("#Y#ZZZ"))
	char PassPlusCode[MAX_LENGTH];			// ������ ��� ��������� ����������� ��������� SMS-�������

	#define MAX_LENGTH_NEW_REC (sizeof("#12#1 +79161234567 User1*123�151*"))	// ������, ������� ������������ ADMIN ��������� ��� ������ �� ��������� ���������� ������������ ������ ������������
	char NewRecIncom[MAX_LENGTH_NEW_REC];

	char StarCount = 0;							// ������� ��������� ��� SMS-������� #11#
	char BlankCount  = 0;						// ������� �������� ��� SMS-������� #11#

	char AbNewPos = 0;
	char Ab11Number [MAX_NUMBER_LENGTH+1];	// ����� �������� ��� SMS-������� #11#
	char Ab11Record [MAX_RECORD_LENGTH+1];	// ������ �������� ��� SMS-������� #11#

	char AbNumCount = 0;
	char AbRecCount = 0;

//	char MessageLength = 0; 					// ��� ����������� ������������� �������� SMS-���������

	PassPlusCode[0] = 0;
	NewRecIncom[0] = 0;

	// ���������� � ������ � ������ ���������
	if (((PINC & (1<<EXTKEY)) != 0) && (GuardSource == 0))	// ���� ���������� ���� � ������� ��� �� ��������� � ������ ������
		GuardIn(GS_SWITCHER);											// ��������� � ����� ������
	if (((PINC & (1<<EXTKEY)) == 0) && (GuardSource != 0))	// ���� �������� ���� � ������� ��������� � ������ ������
		GuardOut(GS_SWITCHER);											// ������� �� ������ ������

	// ���������� � ������ � ������ ������ iButton
	for (temp = 0; temp <= 300; temp++)
	{
		delay_ms(1);														// ����� ��������� �������� 1 �������

		temp1 = iButtonKeyScan();

		if (temp1 == 1)													// ���� � ����������� �������� ����, ��������� � ���� ������
		{
			TmLockOpenTime = LockOpenTime;							// ��������� ���� - � ����� ������ ���� ������� ����� (fw103)

			SetActiveUart(UART_0);										// !!! (fw103)
			printf("iB1!\n\r");											// !!! (fw103)
			SetActiveUart(UART_1);										// !!! (fw103)

			if (GuardSource != 0)										// ���� ������� ��������� � ������ ������
				GuardOut(GS_IBUTTON);									// ������� ������ �� ������ ������

			break;
		}
	}

	#define	SOUND_FACTOR		8000													// �������� � �������
	#define	VVEDITE_START		5.9													// ������ ����� "�������" � �������� �������� ����� � ��������
	#define	VVEDITE_START_DF	VVEDITE_START*(SOUND_FACTOR/MAX_BUFFER)	// �������� DataFlash
	#define	VVEDITE_END			7														// ����� ����� "�������" � �������� �������� ����� � ��������
	#define	VVEDITE_END_DF		VVEDITE_END*(SOUND_FACTOR/MAX_BUFFER)		// �������� DataFlash

	#define	PAROL_START			7														// "������"
	#define	PAROL_START_DF		PAROL_START*(SOUND_FACTOR/MAX_BUFFER)
	#define	PAROL_END			7.5
	#define	PAROL_END_DF		PAROL_END*(SOUND_FACTOR/MAX_BUFFER)

	#define	KOMANDU_START		7.5													// "�������"
	#define	KOMANDU_START_DF	KOMANDU_START*(SOUND_FACTOR/MAX_BUFFER)
	#define	KOMANDU_END			8.3
	#define	KOMANDU_END_DF		KOMANDU_END*(SOUND_FACTOR/MAX_BUFFER)

	#define	SLKOMANDU_START		8.3												// "��������� �������"
	#define	SLKOMANDU_START_DF	SLKOMANDU_START*(SOUND_FACTOR/MAX_BUFFER)
	#define	SLKOMANDU_END			9.8
	#define	SLKOMANDU_END_DF		SLKOMANDU_END*(SOUND_FACTOR/MAX_BUFFER)

	#define	RELE_START			12.9													// "����"
	#define	RELE_START_DF		RELE_START*(SOUND_FACTOR/MAX_BUFFER)
	#define	RELE_END				13.7
	#define	RELE_END_DF			RELE_END*(SOUND_FACTOR/MAX_BUFFER)

	#define	ODIN_START			4														// "����"
	#define	ODIN_START_DF		ODIN_START*(SOUND_FACTOR/MAX_BUFFER)
	#define	ODIN_END				4.5
	#define	ODIN_END_DF			ODIN_END*(SOUND_FACTOR/MAX_BUFFER)

	#define	DVA_START			4.7													// "���"
	#define	DVA_START_DF		DVA_START*(SOUND_FACTOR/MAX_BUFFER)
	#define	DVA_END				5.5
	#define	DVA_END_DF			DVA_END*(SOUND_FACTOR/MAX_BUFFER)

	#define	TRI_START			5.4													// "���"
	#define	TRI_START_DF		TRI_START*(SOUND_FACTOR/MAX_BUFFER)
	#define	TRI_END				5.9
	#define	TRI_END_DF			TRI_END*(SOUND_FACTOR/MAX_BUFFER)

	#define	VKL_START			13.9													// "��������"
	#define	VKL_START_DF		VKL_START*(SOUND_FACTOR/MAX_BUFFER)
	#define	VKL_END				14.6
	#define	VKL_END_DF			VKL_END*(SOUND_FACTOR/MAX_BUFFER)

	#define	VIKL_START			14.6													// "���������"
	#define	VIKL_START_DF		VIKL_START*(SOUND_FACTOR/MAX_BUFFER)
	#define	VIKL_END				15.5
	#define	VIKL_END_DF			VIKL_END*(SOUND_FACTOR/MAX_BUFFER)

	#define	VKLA_START			11.1													// "��������"
	#define	VKLA_START_DF		VKLA_START*(SOUND_FACTOR/MAX_BUFFER)
	#define	VKLA_END				12
	#define	VKLA_END_DF			VKLA_END*(SOUND_FACTOR/MAX_BUFFER)

	#define	VIKLA_START			11.9													// "���������"
	#define	VIKLA_START_DF		VIKLA_START*(SOUND_FACTOR/MAX_BUFFER)
	#define	VIKLA_END			13.0
	#define	VIKLA_END_DF		VIKLA_END*(SOUND_FACTOR/MAX_BUFFER)

	#define	VKLN_START			16.8													// "�������"
	#define	VKLN_START_DF		VKLN_START*(SOUND_FACTOR/MAX_BUFFER)
	#define	VKLN_END				17.6
	#define	VKLN_END_DF			VKLN_END*(SOUND_FACTOR/MAX_BUFFER)

	#define	VIKLN_START			17.6													// "��������"
	#define	VIKLN_START_DF		VIKLN_START*(SOUND_FACTOR/MAX_BUFFER)
	#define	VIKLN_END			18.5
	#define	VIKLN_END_DF		VIKLN_END*(SOUND_FACTOR/MAX_BUFFER)

	#define	SIGNAL_START		9.7													// "������������"
	#define	SIGNAL_START_DF	SIGNAL_START*(SOUND_FACTOR/MAX_BUFFER)
	#define	SIGNAL_END			11.1
	#define	SIGNAL_END_DF		SIGNAL_END*(SOUND_FACTOR/MAX_BUFFER)

	#define	EMODE_START			15.5													// "����������� �����"
	#define	EMODE_START_DF		EMODE_START*(SOUND_FACTOR/MAX_BUFFER)
	#define	EMODE_END			16.9
	#define	EMODE_END_DF		EMODE_END*(SOUND_FACTOR/MAX_BUFFER)

	#define	ERROR_START			18.5													// "������, ��������� �������"
	#define	ERROR_START_DF		ERROR_START*(SOUND_FACTOR/MAX_BUFFER)
	#define	ERROR_END			20.5
	#define	ERROR_END_DF		ERROR_END*(SOUND_FACTOR/MAX_BUFFER)

	if (AbRecReadComplete)												// �������� ������ ����� SMS-��������� � �������� ��������� �� ������ ��� ����� � ������ ��� ��������� � ������������ ��������� (#2# - #11#)
	{																			// ����������� ����������� �������� ������
		printf("AT+CPMS=\"SM\",\"SM\",\"SM\"\n\r");				// ��� ������ � SMS-����������� ����� �������������� SIM-�����
		delay_ms(1000);
		StringBuilder();
		if (strstr(ReceiveString, "+CPMS: 0") == NULL)			// (fw116)
		{
			InboxSMS = 1;
			SetActiveUart(UART_0);										// !!! (fw116)
			printf("Not Empty.\n\r");									// !!! (fw116)
			SetActiveUart(UART_1);										// !!! (fw116)			
		}
		else
		{
			InboxSMS = 0;
			SetActiveUart(UART_0);										// !!! (fw116)
			printf("Empty.\n\r");										// !!! (fw116)
			SetActiveUart(UART_1);										// !!! (fw116)			
		}

		printf("AT+CMGL=\"REC UNREAD\"\n\r");

		// ���������� � ������ � ������ ������ iButton. ������ �������� ���� ���������� ��������� �������� � 5 ������, ����� �� ������ ��� ���� � ����� ������������, ����������� ����, �� ���� ������� �����
		for (temp = 0; temp <= 1500; temp++)
		{
			delay_ms(1);													// ����� ��������� �������� 5 ������

			temp1 = iButtonKeyScan();

			if (temp1 == 1)												// ���� � ����������� �������� ����, ��������� � ���� ������
			{
				TmLockOpenTime = LockOpenTime;						// ��������� ���� - � ����� ������ ���� ������� ����� (fw103)

//				SetActiveUart(UART_0);									// !!! (fw103)
//				printf("iB2!\n\r");										// !!! (fw103)
//				SetActiveUart(UART_1);									// !!! (fw103)

				if (GuardSource != 0)									// ���� ������� ��������� � ������ ������
					GuardOut(GS_IBUTTON);								// ������� ������ �� ������ ������

				break;
			}
		}

		StringBuilder();

		temp1 = 0;
		while ((strstr(ReceiveString, "OK") == NULL) && (strstr(ReceiveString, "ERROR") == NULL) && (temp1 < 10))	// ���� �� ������ "OK", "ERROR" ��� �� ������� ������� � 10 ������.
		{
			temp1++;
			StringBuilder();
//			SetActiveUart(UART_0);														// !!!
//			printf("Wait...\n\r");														// !!!
			delay_ms(1000);
//			SetActiveUart(UART_1);														// !!!
		}

		SetActiveUart(UART_0);															// !!!
		printf("Call Check\n\r");														// !!!
		SetActiveUart(UART_1);															// !!!

		if (strstr(ReceiveString, "RING") != NULL)
		{
			SetActiveUart(UART_0);														// !!!
			printf("Inc Call\n\r");														// !!!
			SetActiveUart(UART_1);														// !!!

			for (temp = 0; temp < MAX_AB_NUMBER; temp++)							// ���������� ���������
				if ((AbProfile[temp] != 'D')											// ���� ������������ �� ������������
				&& (strstr(ReceiveString, AbName[temp]) != NULL))				// ���� ��� �����
					{
						DTMFIncomingUser = temp;
						SetActiveUart(UART_0);											// !!!
						printf("Inc User = %s \n\r", AbName[temp]);				// !!!
						SetActiveUart(UART_1);											// !!!			
					}

			DTMFPassPos = 0;
			DTMFPassBuf[0] = 0;
			DTMFPassBuf[1] = 0;
			DTMFPassBuf[2] = 0;
			DTMFPassBuf[3] = 0;
			printf("ATA\n\r");															// ����� ������
			clrbit(TIMSK, TOIE0);														// ��������� ���������� �� ������������ �� ������� 0 (����� �� ����� ������������ ��������������� ����)
			setbit(PORTC, GSMLED);														// ����� ��������� - ����� ���������

			clrbit(PORTB, PWDN);															// �������� MT8870
			StringBuilder();

			DF_PagesToSpeaker(VVEDITE_START_DF, VVEDITE_END_DF);				// "�������..."
			DF_PagesToSpeaker(PAROL_START_DF, PAROL_END_DF);					// "...������"

			temp = 0;			
			while((temp < DTMF_PASS_TIMEOUT) && (DTMFPassPos < DTMF_PASS_LENGTH))
			{
				printf("AT+CPAS=1\n\r");														// fw108
				delay_ms(1000);
				StringBuilder();
				if (strstr(ReceiveString, "129") != NULL)									// ���� ������� ������� ������
					break;
				temp++;
//				SetActiveUart(UART_0);															// !!!
//				printf("seconds = %d, DTMFPassPos = %d\n\r", temp, DTMFPassPos);	// !!!
//				SetActiveUart(UART_1);															// !!!
			}

			if (((DTMFPassBuf[0] == AbPass[DTMFIncomingUser][0] - 48) &&
				  (DTMFPassBuf[1] == AbPass[DTMFIncomingUser][1] - 48) &&
				  (DTMFPassBuf[2] == AbPass[DTMFIncomingUser][2] - 48)) ||
				 ((DTMFPassBuf[0] == GuestPass[0] - 48) &&
				  (DTMFPassBuf[1] == GuestPass[1] - 48) &&
				  (DTMFPassBuf[2] == GuestPass[2] - 48)))
			{
//				SetActiveUart(UART_0);									// !!!
//				printf("DTMF Password is TRUE\n\r");				// !!!
//				SetActiveUart(UART_1);									// !!!

				DTMFPassPos = 0;
				temp = 0;

				DF_PagesToSpeaker(VVEDITE_START_DF, VVEDITE_END_DF);						// "�������..."
				DF_PagesToSpeaker(KOMANDU_START_DF, KOMANDU_END_DF);						// "...�������"

				while(temp < DTMF_COMMAMD_TIMEOUT)
				{
					printf("AT+CPAS=1\n\r");														// fw108
					delay_ms(1000);
					StringBuilder();
					if (strstr(ReceiveString, "129") != NULL)									// ���� ������� ������� ������
						break;

					temp++;
//					SetActiveUart(UART_0);															// !!!
//					printf("seconds = %d, DTMFPassPos = %d\n\r", temp, DTMFPassPos);	// !!!
//					SetActiveUart(UART_1);															// !!!
				
					if (DTMFPassPos == DTMF_COMMAND_LENGTH)
					{
						DTMFPassPos = 0;																// ������ � ����� ����� �������
						temp = 0;																		// ��������� ������������ ��� DTMF_COMMAMD_TIMEOUT ������

						if (((DTMFPassBuf[0] < 7) || (DTMFPassBuf[0] == 9)) && (DTMFPassBuf[1] == DTMF_SHARP))	// ������� ������ ������� 0...6 � 9
						{
//							SetActiveUart(UART_0);													// !!!
//							printf("DTMF Command accepted\n\r");								// !!!
//							SetActiveUart(UART_1);													// !!!

							switch (DTMFPassBuf[0])
							{
								case 0:
									GuardOut(GS_DTMF);												// ������������� ������� ����� �� ������ ������
									DF_PagesToSpeaker(SIGNAL_START_DF, SIGNAL_END_DF);		// "������������..."
									DF_PagesToSpeaker(VIKLA_START_DF, VIKLA_END_DF);		// "...���������"
									SendMessageToExtUser(" GUARD OFF");							// fw114
									break;
								case 1:
									GuardIn(GS_DTMF);													// ������������� ������� ����� � ����� ������
									DF_PagesToSpeaker(SIGNAL_START_DF, SIGNAL_END_DF);		// "������������..."
									DF_PagesToSpeaker(VKLA_START_DF, VKLA_END_DF);			// "...��������"
									SendMessageToExtUser(" GUARD ON");							// fw114
									break;
								case 2:
									setbit(PORTG, K1);
									DF_PagesToSpeaker(RELE_START_DF, RELE_END_DF);			// "����..."
									DF_PagesToSpeaker(ODIN_START_DF, ODIN_END_DF);			// "...����..."
									DF_PagesToSpeaker(VKL_START_DF, VKL_END_DF);				// "...��������"
									SendMessageToExtUser(" RELE 1 ON");							// fw114
									break;
								case 3:
									clrbit(PORTG, K1);
									DF_PagesToSpeaker(RELE_START_DF, RELE_END_DF);			// "����..."
									DF_PagesToSpeaker(ODIN_START_DF, ODIN_END_DF);			// "...����..."
									DF_PagesToSpeaker(VIKL_START_DF, VIKL_END_DF);			// "...���������"
									SendMessageToExtUser(" RELE 1 OFF");						// fw114
									break;
								case 4:
									setbit(PORTG, K2);
									DF_PagesToSpeaker(RELE_START_DF, RELE_END_DF);			// "����..."
									DF_PagesToSpeaker(DVA_START_DF, DVA_END_DF);				// "...���..."
									DF_PagesToSpeaker(VKL_START_DF, VKL_END_DF);				// "...��������"
									SendMessageToExtUser(" RELE 2 ON");							// fw114
									break;
								case 5:
									clrbit(PORTG, K2);
									DF_PagesToSpeaker(RELE_START_DF, RELE_END_DF);			// "����..."
									DF_PagesToSpeaker(DVA_START_DF, DVA_END_DF);				// "...���..."
									DF_PagesToSpeaker(VIKL_START_DF, VIKL_END_DF);			// "...���������"
									SendMessageToExtUser(" RELE 2 OFF");						// fw114
									break;
								case 6:
									DF_PagesToSpeaker(SIGNAL_START_DF, SIGNAL_END_DF);		// "������������..."
									if (GuardSource == 0)
										DF_PagesToSpeaker(VIKLA_START_DF, VIKLA_END_DF);	// "...���������"
									else
										DF_PagesToSpeaker(VKLA_START_DF, VKLA_END_DF);		// "...��������"
									break;
								case 9:
									DF_PagesToSpeaker(EMODE_START_DF, EMODE_END_DF);		// "����������� �����..."
									if (PowerCorrect == 0)
									{
										SMSorDTMFEconoMode = 1;
										DF_PagesToSpeaker(VKLN_START_DF, VKLN_END_DF);		// "...�������"
										SendMessageToExtUser(" Forced EconoMode ON");		// fw114
									}
									else
										DF_PagesToSpeaker(VIKLN_START_DF, VIKLN_END_DF);	// "...��������"
									break;
							}
							DF_PagesToSpeaker(VVEDITE_START_DF, VVEDITE_END_DF);			// "�������..."
							DF_PagesToSpeaker(SLKOMANDU_START_DF, SLKOMANDU_END_DF);		// "...��������� �������"
						}
						else
						{
//							SetActiveUart(UART_0);													// !!!
//							printf("DTMF Command rejected\n\r");								// !!!
//							SetActiveUart(UART_1);													// !!!
							DF_PagesToSpeaker(ERROR_START_DF, ERROR_END_DF);				// "������, ��������� �������" !!! - � ���� �������� �������� �����������, �.�. ������ ������
						}//�������� �������
					}//������� ��� �������
				}//�������
			}//������ ������
//			else
//			{
//				SetActiveUart(UART_0);																// !!!
//				printf("DTMF Password is FALSE\n\r");											// !!!
//				SetActiveUart(UART_1);																// !!!
//			}//�������� ������

			printf("ATH\n\r");																		// ������ ������
			clrbit(PORTC, GSMLED);																	// �����
			setbit(TIMSK, TOIE0);																	// ��������� ���������� �� ������������ �� ������� 0

			delay_ms(2000);
			StringBuilder();
		}

//		MessageLength = strlen(ReceiveString);

		if  ((strstr(ReceiveString, "#0#") != NULL)						// ������ SMS-�������
			||(strstr(ReceiveString, "#1#") != NULL)
			||(strstr(ReceiveString, "#2#") != NULL)
			||(strstr(ReceiveString, "#3#") != NULL)
			||(strstr(ReceiveString, "#4#") != NULL)
			||(strstr(ReceiveString, "#5#") != NULL)
			||(strstr(ReceiveString, "#6#") != NULL)
			||(strstr(ReceiveString, "#7#") != NULL)
			||(strstr(ReceiveString, "#9#") != NULL)
			||(strstr(ReceiveString, "#10#") != NULL)
			||(strstr(ReceiveString, "#11#") != NULL))
		{
			for (temp = 0; temp < MAX_AB_NUMBER; temp++)					// ���������� ���������
			{
				if ((AbProfile[temp] != 'D')									// ���� ������������ �� ������������
				&& (strncmp(AbName[temp], "##", sizeof("##")) != 0)	// ���� ������ �� �����
				&& (strstr(ReceiveString, AbPass[temp]) != NULL)		// ���� ������ �����
				&& (strstr(ReceiveString, AbName[temp]) != NULL))		// ���� ��� �����
				{
					printf("AT+CMGS=\"%s\"\n\r", AbNumber[temp]);		// �������� �������� SMS
					delay_ms(2000);

					if ((strstr(ReceiveString, "#0#") != NULL) && (GuardSource != 0))		// ���� ������ ������ ����� � ������ � ���� ������� ����� �� ������				
					{
						GuardOut(GS_SMS);
						printf(" GUARD OFF");
						MessageDecoding = 51;
					}

					if ((strstr(ReceiveString, "#1#") != NULL) && (GuardSource == 0) && (LoopTest() == 1))		// ���� ������ ������ ��������� �� ������ � ���� ������� ��� �� ����������� �� ������ � ���� �������� ������ ��������
					{
						GuardIn(GS_SMS);
						printf(" GUARD ON");
						MessageDecoding = 52;
					}

					if (strstr(ReceiveString, "#2#") != NULL)						// ���� ������ ������ �������� ���� 1
					{
						setbit(PORTG, K1);
						printf(" RELE 1 ON");
						MessageDecoding = 53;
					}

					if (strstr(ReceiveString, "#3#") != NULL)						// ���� ������ ������ ��������� ���� 1
					{
						clrbit(PORTG, K1);
						printf(" RELE 1 OFF");
						MessageDecoding = 54;
					}

					if (strstr(ReceiveString, "#4#") != NULL)						// ���� ������ ������ �������� ���� 2
					{
						setbit(PORTG, K2);
						printf(" RELE 2 ON");
						MessageDecoding = 55;
					}

					if (strstr(ReceiveString, "#5#") != NULL)						// ���� ������ ������ ��������� ���� 2
					{
						clrbit(PORTG, K2);
						printf(" RELE 2 OFF");
						MessageDecoding = 56;
					}

					if (strstr(ReceiveString, "#6#") != NULL)						// ���� ������ ������ ��������� �������
					{
						if (GuardSource == 0)
							printf(" GUARD OFF");
						else
							printf(" GUARD ON");
						MessageDecoding = 1;
					}

					if (strstr(ReceiveString, "#9#") != NULL)						// ���� ������ ������ �� �������������� ������� � ���������������� ���������
					{
						if (PowerCorrect == 0)
						{
							SMSorDTMFEconoMode = 1;
							printf(" Forced EconoMode ON");							// �������� �������������� ����������� �����
						}
						else
							printf(" Forced EconoMode NOT TO HOLD");				// ������� ������� � ������� - �������������� ����������� ����� �� ��������
						MessageDecoding = 57;
					}

					if ((strstr(ReceiveString, "#10#") != NULL) && (strstr(ReceiveString, "ADMIN") != NULL))		// ���� ������ ������ ������ �� ������������ ADMIN
					{
						MessageDecoding = 1;
						strncpy(PassPlusCode, strstr(ReceiveString, "#10#"), sizeof("#10#X")-1);
						temp1 = PassPlusCode[4] - 48;
						printf("%d %s %s", temp1, AbNumber[temp1-1], AbRecord[temp1-1]);									// ������� ������ � ����������� ������� ������������� 1-�� �����, ������� "-1"
					}

					if ((strstr(ReceiveString, "#11#") != NULL) && (strstr(ReceiveString, "ADMIN") != NULL))		// ���� ������ ������ ��������� ������ �� ������������ ADMIN
					{
						strncpy(NewRecIncom, strstr(ReceiveString, "#11#"), sizeof(NewRecIncom));

						SetActiveUart(UART_0);																				// !!!
						printf("NewRecIncom = %s\n\r", NewRecIncom);													// !!!
						SetActiveUart(UART_1);																				// !!!

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
						SetActiveUart(UART_0);																				// !!!
						printf("AbNewPos = %d\n\r", AbNewPos);															// !!!
						printf("Ab11Number = %s\n\r", Ab11Number);													// !!!
						printf("Ab11Record = %s\n\r", Ab11Record);													// !!!
						SetActiveUart(UART_1);																				// !!!

						MessageDecoding = 11;
						printf(" User redefined as: %d %s %s ", AbNewPos, Ab11Number, Ab11Record);
					}

					if (strstr(ReceiveString, "#7#") != NULL)						// ���� ������ ������ �� ��������� ������� ������ �������� �������
					{
						MessageDecoding = 58;
						strncpy(PassPlusCode, strstr(ReceiveString, "#7#"), sizeof(PassPlusCode)-1);

						if ((PassPlusCode[3] - 48 <= 2) && (PassPlusCode[4] - 48 <= 2) && (PassPlusCode[5] - 48 <= 2))
						{
							switch (PassPlusCode[3] - 48)
							{
								case 0 : Loop1Set = 0; break;	// ����� ��������
								case 1 : Loop1Set = 2; break;	// ����� ������������ � �������� �� ����������
								case 2 : Loop1Set = 1; break;	// ����� ������������ � �������� �� ���������
							}
							switch (PassPlusCode[4] - 48)
							{
								case 0 : Loop2Set = 0; break;	// ����� ��������
								case 1 : Loop2Set = 2; break;	// ����� ������������ � �������� �� ����������
								case 2 : Loop2Set = 1; break;	// ����� ������������ � �������� �� ���������
							}
							switch (PassPlusCode[5] - 48)
							{
								case 0 : Loop3Set = 0; break;	// ����� ��������
								case 1 : Loop3Set = 2; break;	// ����� ������������ � �������� �� ����������
								case 2 : Loop3Set = 1; break;	// ����� ������������ � �������� �� ���������
							}
							printf(" Loops status redefined");
						}
						else
							printf(" ERROR - loops status NOT redefined");
					}

					if (MessageDecoding == 0)					// ���� ��������� ��� � �� ���� ����������������
						printf(" ERROR MESSAGE");
					printf("%c",CtrlZ);							// ����������� �������� SMS

					delay_ms(1000);
					StringBuilder();
					temp1 = 0;
					while ((strstr(ReceiveString, "OK") == NULL) && (strstr(ReceiveString, "ERROR") == NULL) && (temp1 < 10))	// ���� �� ������ "OK", "ERROR" ��� �� ������� ������� � 10 ������.
					{
						temp1++;
						StringBuilder();
						delay_ms(1000);
					}//�������� ������ �� GR-47

					StringBuilder();

					if (MessageDecoding == 11)					// ���� ���� ����������� ����� ���������������� ������
					{
						printf("AT+CPBW=%d,\"%s\",129,\"%s\"\n\r", AbNewPos, Ab11Number, Ab11Record);			// ������� �� ������ ������ ������
						delay_ms(2000);
						StringBuilder();
						AbRecReadComplete = 0; 					// ��� ���������� ���� ��������� ���������� ���� �� ����� ��������� ���������� �����
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
				}//������ �������� SMS-���������
			}//���������� ���������
		}//SMS � ��������� �� �������

//		if (MessageLength > 70)									// ������, ���� ������� SMS-���������. ������ ���, ��� ������ ������� ������� ������, ����� ��������� ��������
		if (InboxSMS != 0)										// fw116
		{
			printf("AT+CMGD = 1\n\r");							// ������� ������ SMS-��������� �� SIM-�����. ����� ����� ����� - OK ��� ERROR, �� �����, �������, ��� ������ SIM-��������� ������� �� ������������ � GSM-������ �� ��������
			delay_ms(1000);
			StringBuilder();
			printf("AT+CMGD = 2\n\r");							// ������� ������ SMS-��������� �� SIM-�����. ����� ����� ����� - OK ��� ERROR, �� �����, �������, ��� ������ SIM-��������� ������� �� ������������ � GSM-������ �� ��������
			delay_ms(1000);
			StringBuilder();
			printf("AT+CMGD = 3\n\r");							// ������� ������ SMS-��������� �� SIM-�����. ����� ����� ����� - OK ��� ERROR, �� �����, �������, ��� ������ SIM-��������� ������� �� ������������ � GSM-������ �� ��������
			delay_ms(1000);
			StringBuilder();
//			SetActiveUart(UART_0);								// !!!
//			printf("Message delete\n\r");						// !!!
//			SetActiveUart(UART_1);								// !!!
		}
	}//AbRecReadComplete

	Loop1On = Loop1Set;											// ��������� ��������� ������ - � EEPROM
	Loop2On = Loop2Set;
	Loop3On = Loop3Set;

	// �������� ���������� �� ������� � ������������ ������� ������
	temp = ReadADC(VBAT);													// ��������� ���������� �� �������
//	if ((temp <= ACC_FAULT) && (PowerCorrect == 0))					// ���� ���������� ����� �� 11 � � ���� ����������� ������� �������
	if (((temp <= ACC_FAULT) || (SMSorDTMFEconoMode == 1)) && (PowerCorrect == 0))	// (���� ���������� ����� �� 11 � ��� ��������� ������� ��������������� ������������ ������) � ���� ����������� ������� �������
	{
		EconoMode = 1;															// �������� ����������� �����
//		SetActiveUart(UART_0);												// !!!
//		printf("Standby mode\n\r");										// !!!
//		SetActiveUart(UART_1);												// !!!

		if (AbRecReadComplete)
		{
			if ((SendingPOWOFF == 0) && (SMSorDTMFEconoMode == 0))		// ���� ��������� ��� �� ����������� � ��� �� �������������� ������������ � ���������������� �����
			{
				for (temp = 0; temp < MAX_AB_NUMBER; temp++)
				{
					if ((strncmp(AbName[temp], "GUEST", sizeof("GUEST")) != 0)
					&& (strncmp(AbName[temp], "##", sizeof("##")) != 0)
					&& (AbProfile[temp] != 'D'))							// ���� ������������ ������������ � �� �� GUEST � �� ������������
					{
						printf("AT+CMGS=\"%s\"\n\r", AbNumber[temp]);
						delay_ms(2000);
						printf("Message to %s: POWER OFF%c", AbName[temp], CtrlZ);	// !!! Message to - ������, ��� ������ ��� �������

//						delay_ms(4000);
						StringBuilder();
						temp1 = 0;
						while ((strstr(ReceiveString, "OK") == NULL) && (strstr(ReceiveString, "ERROR") == NULL) && (temp1 < SMS_TIMEOUT))	// ���� �� ������ "OK", "ERROR" ��� �� ������� ������� � 60 ������.
						{
							temp1++;
							StringBuilder();
//							SetActiveUart(UART_0);												// !!!
//							printf("Wait...\n\r");												// !!!
							delay_ms(1000);
//							SetActiveUart(UART_1);												// !!!
						}
					}
				}
				SendingPOWOFF = 1;											// ������ ��������� �� ����
			}
		}
		GR_Off();																// ����� �������� SMS-��������� ��������� GSM-������ 
		GSMStatus = 0;															// � ��� ��������� �����, ��� �� ��������� GSM-������
	}
	else
	{
		EconoMode = 0;															// �������� ����� ����������� ������������������
		SMSorDTMFEconoMode = 0;												// ���������� ������ ����������
		SendingPOWOFF = 0;
	}

	// ��������� - ���������� ������ ������������
	if (PowerCorrect == 1)													// ���� ������� ���������� � �����
		if (temp <= ACC_CORRECT)
			setbit(PORTB, CHARG);											// �������� ������� ������� ������������
		else
			clrbit(PORTB, CHARG);											// ��������� ������� �������, ��������� ����������� � ����� ���������� ������
	else
		clrbit(PORTB, CHARG);												// ���������� ��������� ������� �������

	SetActiveUart(UART_1);										// !!!
	delay_ms(2000);												// !!!

	if ((PINA & (1<<VDET)) > 0)								// ���� ������� ������� � �����
	{
		PowerCorrect = 1;											// ������� ������� � �����

		if ((AbRecReadComplete) && (GSMStatus == 2) && (Sending220VON == 0))	// ������� �������� ��������� "220V ON"
		{
			for (temp = 0; temp < MAX_AB_NUMBER; temp++)
			{
				if ((strncmp(AbName[temp], "GUEST", sizeof("GUEST")) != 0)
				&& (strncmp(AbName[temp], "##", sizeof("##")) != 0)
				&& (AbProfile[temp] != 'D'))											// ���� ������������ ������������ � �� �� GUEST � �� ������������
				{
					printf("AT+CMGS=\"%s\"\n\r", AbNumber[temp]);
					delay_ms(2000);
					printf("Message to %s: 220V ON%c", AbName[temp], CtrlZ);	// !!! Message to - ������, ��� ������ ��� �������
					delay_ms(4000);

					StringBuilder();
				}
			}
			Sending220VOFF = 0;
			Sending220VON = 1;			
		}
	}
	else														// ���� �������� �������
	{
		PowerCorrect = 0;											// ���� �������� �������

		if (AbRecReadComplete)
		{
			if (Sending220VOFF == 0)				// ���� ��������� ��� �� �����������
			{
				for (temp = 0; temp < MAX_AB_NUMBER; temp++)
				{
					if ((strncmp(AbName[temp], "GUEST", sizeof("GUEST")) != 0)
					&& (strncmp(AbName[temp], "##", sizeof("##")) != 0)
					&& (AbProfile[temp] != 'D'))	// ���� ������������ ������������ � �� �� GUEST � �� ������������
					{
						printf("AT+CMGS=\"%s\"\n\r", AbNumber[temp]);
						delay_ms(2000);
						printf("Message to %s: 220V OFF%c", AbName[temp], CtrlZ);	// !!! Message to - ������, ��� ������ ��� �������
						delay_ms(4000);

						StringBuilder();
					}
				}
				Sending220VOFF = 1;
				Sending220VON = 0;			
			}
		}
	}

	if (GSMStatus == 1)									// ���� GSM-������ �������, �� ��� �� ��������������� � ����, �������� ��������� �����������
	{
		printf("AT+CREG=1\n\r");
		delay_ms(1000);
		StringBuilder();
		if (strstr(ReceiveString, "OK") != NULL)	// ���� GSM-������ ������� "OK"...
			GSMStatus = 2;									// GSM-������ ��������������� � ����
	}

	if (GSMStatus == 2)								// ���� GSM-������ ��������������� � ����, ��������� ������� �������
	{
		printf("AT+CSQ\n\r");
		delay_ms(1000);
		StringBuilder();
		strtok(ReceiveString, ",");				// �������� bit error rate <ber>, ������� ���� ����� �������

		GSMSigStrength = StrToInt(ReceiveString);
		if (GSMSigStrength <= 31)					// ���������� ��������
		{
			GSMSigStrength /= 6;
			if (GSMSigStrength == 0) GSMSigStrength = 1;	// ����� �������� ���� �� ��� � �������� ������������, ��� GSM-������ ����� ����
		}
		else												//	������������ ��������, ������ �����, 99 - "Not known or not detectable", �� ��� �� GSM-������ �������, ��� ���� �����, ����� ���� �� ��� ��������...
			GSMSigStrength = 1;
	}

	if ((EconoMode == 0) && (GSMStatus == 0))		// ����� �������� GSM-������
	{
		SetActiveUart(UART_0);							// !!!
		printf("GSM ON attempt\n\r");					// !!! 
		SetActiveUart(UART_1);							// !!!

		GR_On();												// �������� GSM-������
		printf("AT\n\r");
		delay_ms(1000);

		StringBuilder();

		if (strstr(ReceiveString, "OK") != NULL)	// ���� GSM-������ ������� "OK"...
		{			
//			SetActiveUart(UART_0);						// !!!
//			printf("OK received\n\r");					// !!!
//			SetActiveUart(UART_1);						// !!!

			printf("ATE=1\n\r");							// ...��������� ���, ����� �� �������� �������� �����
			delay_ms(500);
			printf("AT*E2RS232=2\n\r");				// ...��������� ���������� �������� UART'� GSM-������
			delay_ms(500);
			printf("AT+CLIP=1\n\r");					// ...�������� ���
			delay_ms(500);
			printf("AT+CMEE=1\n\r");					// ...�������� ����������� ���� ������ (�������)
			delay_ms(500);
			printf("AT+CSDH=1\n\r");					// ...�������� ����������� ���������� ������
			delay_ms(500);
			printf("AT+CMGF=1\n\r");					// ...�������� ��������� ����� �������� SMS
			delay_ms(500);

			printf("AT*E2SSN\n\r");
			delay_ms(2000);

			StringBuilder();
			if (strstr(ReceiveString, "ERROR") == NULL)		// ���� GSM-������ �� ����� ������...
			{
				GSMStatus = 1;											// ...�� �������� ������ �� "�� ��������������� � ����"
				SetActiveUart(UART_0);								// !!!
				printf("GR Init OK.\n\r");							// !!!
				SetActiveUart(UART_1);								// !!!
			}
			else															// � ���� �� ������ ���������, ������� ������� SIM-�����
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

void GR_On(void)					// �������� GR-47
{
	clrbit(PORTG, nGSMPOW);		// �������� D9 - LM2576T-ADJ
	clrbit(PORTD,PON);
	delay_ms(3000);
	setbit(PORTD,PON);
	delay_ms(4000);
}//GR_On

void GR_Off(void)					// ��������� GR-47
{
	clrbit(PORTD,PON);
	delay_ms(3000);
	setbit(PORTD,PON);
	delay_ms(4000);				// 4 ������� �� �������� ���������� � "������" ����������
	setbit(PORTG, nGSMPOW);		// ��������� D9 - LM2576T-ADJ; "��������" ����������
	GSMStatus = 0;					// � ��� ��������� ��������� �����, ��� ������ ��������
}//GR_Off

void StringBuilder(void)		// ������ ��������� ������ �� ������ � �������� ������
{
	signed char IncomChar = 0;
	int StringPoint = 0;
	ReceiveString[0] = 0;	

	while ((IncomChar = ReadData()) != -1)			// ��������� ������� �������
	{
		if (IncomChar > 20)								// �������� �������� ��� ��������� �������
		{
			SetActiveUart(UART_0);							// !!!
			printf("%c", IncomChar);						// !!!
			SetActiveUart(UART_1);							// !!!

			ReceiveString[StringPoint] = IncomChar;
			ReceiveString[StringPoint+1] = 0;			// ������ ����� ������
			StringPoint++;
		}
	}
	SetActiveUart(UART_0);									// !!!
	printf("\n\r");											// !!!
	SetActiveUart(UART_1);									// !!!
}//StringBuilder

int StrToInt (char* InputStr)								// ��������� ������������ ������ � ����� ���� int. �������������� ������ �����
{
	int ReturnValue = 0;
	unsigned char StrCount = 0;

	for (; StrCount <= strlen(InputStr); StrCount++)
		if (isdigit(InputStr[StrCount]))
			ReturnValue = 10*ReturnValue + (InputStr[StrCount] - 48);

	return ReturnValue;
}//StrToInt

char ReadAbRecords(void)									// ������ ������� ���������. ���� ������ ��������� � �� ����� ������, ������������ �������� == 1, ����� == 0
{
	char RecCount = 0;										// ������� ���������� �������
	char CharCount = 0;										// ������� �������� ������ ������ ������� AT+CPBR
	char QuotesCount = 0;									// ������� �������. ����� 1-� � 2-� ��������� ��������� ���������� �����, ����� 3-� � 4-� - ��� ���������� ������
	char InNumCount = 0;										// ������� �������� ����������� ������
	char InRecCount = 0;										// ������� �������� ���������� ������
	char StarPos = 0;
	char ServiceName [MAX_NAME_LENGTH+1];				// ��� ��������� ������
	char ServiceNum  [SERVICE_NUM_LENGTH+1];			// ����� ��������� ������

	if (GSMStatus == 0) return 0; 						// GSM-������ �� �������, ������ � ��������
	else
	{
		for (RecCount = 0; RecCount < MAX_AB_NUMBER; RecCount++)	// �������� ��������� ������ �� ���������� �����
		{
			printf("AT+CPBR=%d\n\r", RecCount+1);						// ������ ��������� ������ �� ���������� �����
			delay_ms(2000);
			StringBuilder();
			if (strstr(ReceiveString, "OK") != NULL)					// ���� GSM-������ ����� �������������...
			{
				QuotesCount = 0;
				InNumCount = 0;
				InRecCount = 0;
				AbNumber[RecCount][0] = 0;									// ������� ������ �����
				AbRecord[RecCount][0] = 0;									// ������� ������ ������
				AbName  [RecCount][0] = 0;									// ������� ������ ���
				AbPass  [RecCount][0] = 0;									// ������� ������ ������
				AbPass  [RecCount][3] = 0;									// ����������� 0 ������ ������

				for (CharCount = 0; CharCount < strlen(ReceiveString); CharCount++)
				{
					if (QuotesCount == 1)
					{
						if (ReceiveString[CharCount] != '"')
						{
							AbNumber[RecCount][InNumCount] = ReceiveString[CharCount];		// ��������� ������ �����
							AbNumber[RecCount][InNumCount+1] = 0;
						}
//						SetActiveUart(UART_0);																						// !!!
//						if (ReceiveString[CharCount] != '"')																	// !!!
//							printf("N%c ", AbNumber[RecCount][InNumCount]);													// !!!
//						SetActiveUart(UART_1);																						// !!!
						if (InNumCount == MAX_NUMBER_LENGTH)
						{
//							SetActiveUart(UART_0);								// !!!
//							printf("Number too long\n\r");					// !!!
//							printf("Error - NTL\n\r");							// !!!
							SetActiveUart(UART_1);								// !!!
							IncorrectAb = RecCount + 1;						// ������� ������� � ���� ������� ������������� ������� �������� � SIM-�����
							return 0;							
						}
						InNumCount++;
					}//QuotesCount==1
					if (QuotesCount == 3)
					{
						if (ReceiveString[CharCount] != '"')
						{
							AbRecord[RecCount][InRecCount] = ReceiveString[CharCount];	// ��������� ������ ������
							AbRecord[RecCount][InRecCount+1] = 0;
						}
//						SetActiveUart(UART_0);																						// !!!
//						if (ReceiveString[CharCount] != '"')																	// !!!
//							printf("R%c ", AbRecord[RecCount][InRecCount]);													// !!!	
						SetActiveUart(UART_1);																						// !!!
						if (InRecCount == MAX_RECORD_LENGTH)
						{
//							SetActiveUart(UART_0);								// !!!
//							printf("Record too long\n\r");					// !!!
//							printf("Error - RTL\n\r");							// !!!
//							SetActiveUart(UART_1);								// !!!
							IncorrectAb = RecCount + 1;						// ������� ������� � ���� ������� ������������� ������� �������� � SIM-�����
							return 0;							
						}
						InRecCount++;
					}//QuotesCount==3
					if (ReceiveString[CharCount] == '"')					// ����� ��� ���� �������
					{
						QuotesCount++;
//						SetActiveUart(UART_0);									// !!!
//						printf("QuotesCount = %d\n\r",  QuotesCount);	// !!!
//						SetActiveUart(UART_1);									// !!!
					}//������� �������
				}//for CharCount

				SetActiveUart(UART_0);														// !!!
				printf ("%d Number is %s\n\r", RecCount+1, AbNumber[RecCount]);// !!!
				printf ("%d Record is %s\n\r", RecCount+1, AbRecord[RecCount]);// !!!
				SetActiveUart(UART_1);														// !!!

				StarPos = strcspn(AbRecord[RecCount], "*");
				strncpy( AbName[RecCount], AbRecord[RecCount], StarPos); 	// ��������� ��� �������� = ����� ������ ����� ������� * 

				SetActiveUart(UART_0);													// !!!
				printf ("%d Name is %s\n\r", RecCount+1, AbName[RecCount]);	// !!!
				SetActiveUart(UART_1);													// !!!

				if (strncmp(AbName[RecCount], "##", sizeof("##")) == 0) AbActive[RecCount] = 0;	// ������������ �� �������
				else 
				{
					AbActive[RecCount] = 1;		// ������������ �������

					AbPass[RecCount][0] = AbRecord[RecCount][StarPos+1];	// ��������� ������ ��������
					AbPass[RecCount][1] = AbRecord[RecCount][StarPos+2];
					AbPass[RecCount][2] = AbRecord[RecCount][StarPos+3];

					if	((strncmp(AbName[RecCount], "GUEST", sizeof("GUEST")) == 0)	// ��� ������������� � ���������� ��� ������ � DTMF
					||	 (strncmp(AbName[RecCount], "Guest", sizeof("GUEST")) == 0)
					||	 (strncmp(AbName[RecCount], "guest", sizeof("GUEST")) == 0))
					{
						GuestPass[0] = AbPass[RecCount][0];
						GuestPass[1] = AbPass[RecCount][1];
						GuestPass[2] = AbPass[RecCount][2];
						GuestPass[3] = 0;

						strncpy (AbName[RecCount], "GUEST", sizeof("GUEST"));
					}

					SetActiveUart(UART_0);													// !!!
					printf ("%d Name is %s\n\r", RecCount+1, AbName[RecCount]);	// !!!
					SetActiveUart(UART_1);													// !!!

					SetActiveUart(UART_0);																// !!!
//					printf ("%d User password is %s\n\r", RecCount+1, AbPass[RecCount]);	// !!!
					printf ("%d UPass is %s\n\r", RecCount+1, AbPass[RecCount]);			// !!!
					SetActiveUart(UART_1);																// !!!

					AbProfile[RecCount] = AbRecord[RecCount][StarPos+4];		// ���������� ������� ��������
					AbCallTime[RecCount] = AbRecord[RecCount][StarPos+5];		// ���������� ����� ������� �� ��������
					AbCallNum[RecCount] = AbRecord[RecCount][StarPos+6]-48;		// ���������� ���������� ������� ������� �� ��������
					AbExtService[RecCount] = AbRecord[RecCount][StarPos+7]-48;	// ���������� ����������� ��������������� �������

					SetActiveUart(UART_0);																			// !!!
//					printf ("%d User profile is %c\n\r", RecCount+1, AbProfile[RecCount]);			// !!!
					printf ("%d UProf is %c\n\r", RecCount+1, AbProfile[RecCount]);					// !!!
//					printf ("%d User one call time is %c\n\r", RecCount+1, AbCallTime[RecCount]);	// !!!
					printf ("%d UOCT is %c\n\r", RecCount+1, AbCallTime[RecCount]);					// !!!
//					printf ("%d User calling attempt is %c\n\r", RecCount+1, AbCallNum[RecCount]);// !!!
					printf ("%d UCA is %c\n\r", RecCount+1, AbCallNum[RecCount]);						// !!!
//					printf ("%d User ext service is %c\n\r", RecCount+1, AbExtService[RecCount]);	// !!!
					printf ("%d UES is %d\n\r", RecCount+1, AbExtService[RecCount]);					// !!!
					SetActiveUart(UART_1);																			// !!!

					if (AbRecord[RecCount][StarPos+8] != '*')					// �� ������ ������ ����� ������
						{
//							SetActiveUart(UART_0);																		// !!!
//							printf ("Second star is not present\n\r", RecCount+1, AbActive[RecCount]);	// !!!
//							printf ("Error - SSnP\n\r", RecCount+1, AbActive[RecCount]);					// !!!
//							SetActiveUart(UART_1);																		// !!!
							IncorrectAb = RecCount + 1;							// ������� ������� � ���� ������� ������������� ������� �������� � SIM-�����
							return 0;
						}//AbRecord != '*'
				}//else ##
				SetActiveUart(UART_0);																	// !!!
//				printf ("%d User activity is %d\n\r", RecCount+1, AbActive[RecCount]);	// !!!
				printf ("%d UA is %d\n\r", RecCount+1, AbActive[RecCount]);					// !!!
				SetActiveUart(UART_1);																	// !!!
			}// Answer OK - ������ �������
			else
			{
				IncorrectAb = RecCount + 1;			// ������� ������� � ���� ������� ������������� ������� �������� � SIM-�����
				return 0;									// ���� �� ������� �� �������
			}//Answer Error
		}//for RecCount

		printf("AT+CPBR=6\n\r");						// ������ ��������� ������
		delay_ms(2000);
		StringBuilder();
		if (strstr(ReceiveString, "OK") != NULL)	// ���� GSM-������ ����� �������������...
		{
			SetActiveUart(UART_0);						// !!!
//			printf("Service Answer OK\n\r");	  		// !!!
			printf("SA OK\n\r");	  						// !!!
			SetActiveUart(UART_1);						// !!!
		}// Answer OK - ������ �������
		else
		{
			SetActiveUart(UART_0);						// !!!
//			printf("Service Answer Error\n\r");		// !!!
			printf("SA Error\n\r");						// !!!
			SetActiveUart(UART_1);						// !!!
			IncorrectAb = RecCount + 1;				// ������� ������� � ���� ������� ������������� ������� �������� � SIM-�����
			return 0;										// ��������� ������ �� �������
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
					ServiceNum[InNumCount] = ReceiveString[CharCount];		// ��������� ������ �����
					ServiceNum[InNumCount+1] = 0;
				}
//				SetActiveUart(UART_0);																						// !!!
//				if (ReceiveString[CharCount] != '"') printf("SNum%c ", ServiceNum[InNumCount]);			// !!!
//				SetActiveUart(UART_1);																						// !!!

				if (InNumCount == SERVICE_NUM_LENGTH+1)
				{
//					SetActiveUart(UART_0);								// !!!
//					printf("Service number too long\n\r");			// !!!
//					printf("Error - SNTL\n\r");						// !!!
//					printf("InNumCount = %d\n\r", InNumCount);	// !!!
//					SetActiveUart(UART_1);								// !!!
					IncorrectAb = RecCount + 1;						// ������� ������� � ���� ������� ������������� ������� �������� � SIM-�����
					return 0;												// ��������� ����� ������� �������
				}
				InNumCount++;
			}//QuotesCount==1
			if (QuotesCount == 3)
			{
				if (ReceiveString[CharCount] != '"')
				{
					ServiceName[InRecCount] = ReceiveString[CharCount];	// ��������� ������ ������
					ServiceName[InRecCount+1] = 0;
				}
//				SetActiveUart(UART_0);																							// !!!
//				if (ReceiveString[CharCount] != '"')																		// !!!
//					printf("SRec%c ", AbRecord[RecCount][InRecCount]);													// !!!
//				SetActiveUart(UART_1);																							// !!!

				if (InRecCount == MAX_RECORD_LENGTH)
				{
//					SetActiveUart(UART_0);								// !!!
//					printf("Service record too long\n\r");			// !!!
//					printf("Error SRTL\n\r");							// !!!
//					SetActiveUart(UART_1);								// !!!
					IncorrectAb = RecCount + 1;						// ������� ������� � ���� ������� ������������� ������� �������� � SIM-�����
					return 0;												// ��������� ������ ������� �������
				}
				InRecCount++;
			}//QuotesCount==3
			if (ReceiveString[CharCount] == '"')					// ����� ��� ���� �������
				QuotesCount++;
		}//for CharCount

		SetActiveUart(UART_0);											// !!!
		printf ("SN is %s\n\r", ServiceNum);						// !!!
		printf ("SR is %s\n\r", ServiceName);						// !!!
		SetActiveUart(UART_1);											// !!!

		if   (!((strncmp(ServiceName, "Service", sizeof("Service")) == 0)
			  || (strncmp(ServiceName, "SERVICE", sizeof("SERVICE")) == 0)
			  || (strncmp(ServiceName, "service", sizeof("service")) == 0)))
		{
//			SetActiveUart(UART_0);																				// !!!
//			printf ("Error - Service Record must be 'Service', not '%s'\n\r", ServiceName);	// !!!
//			printf ("Error - SR not 'Service'\n\r");														// !!!
//			SetActiveUart(UART_1);																				// !!!
			IncorrectAb = RecCount + 1;								// ������� ������� � ���� ������� ������������� ������� �������� � SIM-�����
			return 0;														// ��������� ������ ����� ���, ������������ �� "Service"
		}

		if (isdigit(ServiceNum[0]))
		{
			DelayGuardOnTime = ServiceNum[0] - 48;					// �������� ��� ���������� �� ������, � �������. 48d = '0'
			SetActiveUart(UART_0);																				// !!!
			printf ("DGOnT = %d m\n\r", DelayGuardOnTime);												// !!!
			SetActiveUart(UART_1);																				// !!!
		}
		else																	// DelayGuardOnTime ������ ���� ������� ������
		{
//			SetActiveUart(UART_0);																				// !!!
//			printf ("Error - DelayGuardOnTime must be digit, not '%c'\n\r", ServiceName[0]);	// !!!
//			printf ("Error - DGOnTnD\n\r");																	// !!!
//			SetActiveUart(UART_1);																				// !!!
			IncorrectAb = RecCount + 1;								// ������� ������� � ���� ������� ������������� ������� �������� � SIM-�����
			return 0;														
		}

		if (isdigit(ServiceNum[1]))
		{
			DelayGuardOffTime = 10*(ServiceNum[1] - 48);			// �������� ��� ������ � ������, � ��������. 48d = '0'
			DelayNoActiveTime = DelayGuardOffTime;
			SetActiveUart(UART_0);																				// !!!
			printf ("DGOT = %d s\n\r", DelayGuardOffTime);												// !!!
			SetActiveUart(UART_1);																				// !!!
		}
		else																	// DelayGuardOffTime ������ ���� ������� ������
		{
//			SetActiveUart(UART_0);																				// !!!
//			printf ("Error - DelayGuardOffTime must be digit, not '%c'\n\r", ServiceName[1]);// !!!
//			printf ("Error - DGTnD\n\r");																		// !!!
//			SetActiveUart(UART_1);																				// !!!
			IncorrectAb = RecCount + 1;								// ������� ������� � ���� ������� ������������� ������� �������� � SIM-�����
			return 0;														
		}

		if ((isdigit(ServiceNum[2])) && ((ServiceNum[2]=='0') || (ServiceNum[2]=='1')))
		{
			iButtonUse = ServiceNum[2] - 48;							// ������������� ������ iButton. 48d = '0'
			SetActiveUart(UART_0);																					// !!!
			printf ("iBU = %d\n\r", iButtonUse);																// !!!
			SetActiveUart(UART_1);																					// !!!
		}
		else																	// iButtonUse ������ ���� ������� ������
		{
//			SetActiveUart(UART_0);																					// !!!
//			printf ("Error - iButtonUse must be digit (0 or 1), not '%c'\n\r", ServiceName[2]);	// !!!
//			printf ("Error - iBUnD\n\r");																			// !!!
//			SetActiveUart(UART_1);																					// !!!
			IncorrectAb = RecCount + 1;								// ������� ������� � ���� ������� ������������� ������� �������� � SIM-�����
			return 0;														
		}

		if (isdigit(ServiceNum[3]))
		{
			LockOpenTime = ServiceNum[3] - 48;						// ����� ��������� �������������������� �����, � ���������. 48d = '0'
			SetActiveUart(UART_0);																					// !!!
			printf ("LOT = %d second(s)\n\r", LockOpenTime);												// !!!
			SetActiveUart(UART_1);																					// !!!
		}
		else																	// LockOpenTime ������ ���� ������� ������
		{
//			SetActiveUart(UART_0);																					// !!!
//			printf ("Error - LockOpenTime must be digit, not '%c'\n\r", ServiceName[3]);			// !!!
//			printf ("Error LOTnD\n\r");																			// !!!
//			SetActiveUart(UART_1);																					// !!!
			IncorrectAb = RecCount + 1;								// ������� ������� � ���� ������� ������������� ������� �������� � SIM-�����
			return 0;														
		}

		if (isdigit(ServiceNum[4]))
		{
			SirenSingTime = 10*(ServiceNum[4] - 48);				// ����� ������ ������, � ��������. 48d = '0'
			SetActiveUart(UART_0);																					// !!!
			printf ("SST = %d seconds\n\r", SirenSingTime);													// !!!
			SetActiveUart(UART_1);																					// !!!
		}
		else																	// SirenSingTime ������ ���� ������� ������
		{
//			SetActiveUart(UART_0);																					// !!!
//			printf ("Error - SirenSingTime must be digit, not '%c'\n\r", ServiceName[4]);			// !!!
//			printf ("Error SSTnD\n\r");																			// !!!
//			SetActiveUart(UART_1);																					// !!!
			IncorrectAb = RecCount + 1;								// ������� ������� � ���� ������� ������������� ������� �������� � SIM-�����
			return 0;														
		}
	}// else GSMStatus
	SetActiveUart(UART_0);							// !!!
	printf("End of AB\n\r");	 	 				// !!!
	SetActiveUart(UART_1);							// !!!

	IncorrectAb = 0;
	return 1;
}//ReadAbRecords

void iButtonUpdate (void)							// ���������� ����� ������ � ������� iButton - ������, ��������, ���������� �����
{
	SetActiveUart(UART_0);							// !!!
	if ((PINA & (1<<JIBP)) == 0)					// ����� ������ ������ iButton
	{
		iButtonMode = 1;
//		printf("IB: write keys\r");				// !!!
	}
	else 
		if ((PINA & (1<<JIBM)) == 0)				// ����� �������� ������ iButton
		{
			iButtonMode = 2;
//			printf("IB: erase keys\n\r");			// !!!
		}
		else												// ����� �������� ���������������� iButton
		{
			iButtonMode = 0;
//			printf("IB: normal mode \n\r");		// !!!
		}
	SetActiveUart(UART_1);							// !!!
}//iButtonUpdate

unsigned char iButtonKeyScan(void)									// ���������� 1, ���� �������� ���� �� ���������� � ������ ������ ��� 0, ���� ������ ����� �� ����������
{
	unsigned char uidCounter;

	SetActiveUart(UART_0);												// !!!
	ds1990a_init();														// ������ ID ����� ���������� � ������ ������� 1-Wire ����

	if ((CRC8(uidDS, 7) == uidDS[7]) && (uidDS[0] == DS1990A_ID))
		for (uidCounter = 0; uidCounter <= CurrentKeys; uidCounter++)
			if  ((uidDS[0] ==  ibKeys[uidCounter][0]) && (uidDS[1] == ibKeys[uidCounter][1])	// ���� ����� ���� ������� � ������ ������
	   		&& (uidDS[2] == ibKeys[uidCounter][2]) && (uidDS[3] == ibKeys[uidCounter][3])
				&& (uidDS[4] == ibKeys[uidCounter][4]) && (uidDS[5] == ibKeys[uidCounter][5])
				&& (uidDS[6] == ibKeys[uidCounter][6]))
			{
				printf("Key incoming ");
				return 1;
			}

	SetActiveUart(UART_1);												// !!!
	return 0;
}

void iButtonNewKeysSearch(void)
{
	SetActiveUart(UART_0);												// !!!
	setbit(PORTC, GRDLED);
	ds1990a_init();														// ������ ID ����� ���������� � ������ ������� 1-Wire ����
	RepeatedKey = 0;
			
	if ((CRC8(uidDS, 7) == uidDS[7]) && (uidDS[0] == DS1990A_ID))
	{
		printf("New key incoming ");
		printf("ROM = %d %d %d %d %d %d %d %d CRC8 = %d\n\r", uidDS[0], uidDS[1], uidDS[2], uidDS[3], uidDS[4], uidDS[5], uidDS[6], uidDS[7], CRC8(uidDS, 7));

		for (temp = 0; temp <= CurrentKeys; temp++)
			if  (((uidDS[0] == ibKeys[temp][0]) && (uidDS[1] == ibKeys[temp][1])	// ���� ����� ���� ��� ���� ��� ���� ������ ������ 20
	   		&& (uidDS[2] == ibKeys[temp][2]) && (uidDS[3] == ibKeys[temp][3])
				&& (uidDS[4] == ibKeys[temp][4]) && (uidDS[5] == ibKeys[temp][5])
				&& (uidDS[6] == ibKeys[temp][6])) || (CurrentKeys == MAX_IB_KEYS))
					RepeatedKey = 1;

		if (RepeatedKey == 0)
		{
			for (temp1 = 0; temp1 <= 7; temp1++)
				ibKeys[CurrentKeys][temp1] = uidDS[temp1];
			CurrentKeys++;
			printf("NEW KEY SAVED\n\r");														// !!!
			clrbit(PORTC, GRDLED);																// ������� GRDLED �� ��� �������, ����� ���� ������������ ������, ��� ����� ���� �������
			delay_ms(3000);
			setbit(PORTC, GRDLED);
		}
		else
		{
			printf("Key repeated\n\r");														// !!!
			for (temp1 = 0; temp1 <= 4*5; temp1++)											// ������ GRDLED'�� ���� ������, ����� ���� ������������ ������, ��� ������� ���� ��� ������������ � ����
			{
				delay_ms(250);
				invbit(PORTC, GRDLED);
			}
		}
	}
	else
	{
		printf("Current Keys = %d\r", CurrentKeys);										// !!!
	}
	clrbit(PORTC, GRDLED);
	SetActiveUart(UART_1);																		// !!!
}//iButtonNewKeysSearch

void iButtonKeysErase(void)							// �������� ������ iButton �� EEPROM
{
	SetActiveUart(UART_0);								// !!!
	setbit(PORTC, GRDLED);
	if (CurrentKeys != 0)								// ����� � ������� ������
	{
		for (temp = 0; temp <= 4*3; temp++)			// ������ GRDLED'�� ��� �������, ����� ���� ������������ ������, ��� ����� ���������
		{
			delay_ms(250);
			invbit(PORTC, GRDLED);
		}
		CurrentKeys = 0;										// �� ����� ���� ������� ��� �����
		for (temp = 0; temp < MAX_IB_KEYS; temp++)
			for (temp1 = 0; temp1 < IB_KEY_LENGTH; temp1++)
				ibKeys[temp][temp1] = 0xFF;
//		printf("\n\rKeys deleted\n\r");					// !!!
	}
//	else															// !!!
//		printf("No keys \r");								// !!!

	clrbit(PORTC, GRDLED);

	SetActiveUart(UART_1);									// !!!
}//iButtonKeysErase

static unsigned int BufferCounter = 0;
static unsigned int PageCounter = 0;

// ����� ������ AT45DB041B - PG1
#define DF_RESET (1<<nFRES)		// nFRES - �� portdefines.h

// ������ MainPage
#define DF_MM_RD 0xD2

// ������ ������ 1
#define DF_BUF_1_RD 0xD4

// ������ ������ 2
#define DF_BUF_2_RD 0xD6
 
// ������� ������� �������� ������ � ����� 1
#define DF_MP_B1_XFER 0x53

// ������� ������� �������� ������ � ����� 2
#define DF_MP_B2_XFER 0x55

// ������� ������� ���������� ������
#define DF_SREG 0xD7

volatile unsigned char Timer1OverCount = 0;

#pragma vector = TIMER1_OVF_vect
__interrupt void OutVoiceSample(void)
{
	Timer1OverCount++;

//	if (Timer1OverCount == 1)											// ����� 0 ����� ���������
//	OCR1B = 0;

	if (Timer1OverCount == 2)
	{
		Timer1OverCount = 0;
		ACSR |= T1_OVF;													// ������������� ���� "���� �������������"
	}
}//OutVoiceSample

void DF_WriteSPI(unsigned char data)								// ������ ����� �� SPI
{
	SPDR = data;
	while (!(SPSR & 0x80));												// ���� ��������� �������� ������
}//DF_WriteSPI

unsigned char DF_Status(void)											// ������ ����� ������� DataFlash
{
//	clrbit(PORTG, nFCS);													// Enable DataFlash

	DF_WriteSPI(DF_SREG);
	DF_WriteSPI(0xFF);

//	setbit(PORTG, nFCS);													// Disable DataFlash
	return SPDR;
}//DF_Status

unsigned char DF_Ready(void)											// ����������� ������ ����� Ready/~Busy
{
	return (DF_Status() & (1<<DF_BIT_READY));
}//DF_Ready

void DF_MainMemoryToSpeaker(unsigned int Page)
{
	unsigned long IntroAddress;
	unsigned char dummy = 0;
	unsigned int StartAddress = 0;

	clrbit(PORTG, nFCS);													// Enable DataFlash
//	while(DF_Ready == 0);												// ���� ������������ DataFlash

	DF_WriteSPI(DF_MM_RD);	
	DF_WriteSPI((char)(Page >> 7));
	DF_WriteSPI((char)(Page << 1)|(StartAddress >> 8));
	DF_WriteSPI((char)(StartAddress));
	DF_WriteSPI(0x00);
	DF_WriteSPI(0x00);
	DF_WriteSPI(0x00);
	DF_WriteSPI(0x00);

	for(IntroAddress = 0; IntroAddress < MAX_BUFFER; IntroAddress++)
	{
		while(!(ACSR&T1_OVF));											// ���� ������������ �������
		DF_WriteSPI(0xFF);												// ��������
		dummy = SPDR;

		OCR1B = dummy;														// �������� ������ �� "��������������"
		ACSR &= (~T1_OVF);												// ���������� ������  
	}

	setbit(PORTG, nFCS);													// Disable DataFlash
}//DF_MainMemoryToSpeaker

void DF_PagesToSpeaker(unsigned int StartPage, unsigned int EndPage)
{
	unsigned int CurrentPage;

	SPCR = 0x5C;															// ������������� SPI

	TCCR1A = 0x21;															// 8 ��� ���, ���������� COM1B
	TCNT1 = 0x00;
	TIFR = 0x04;															// ������� ����� ������������
	setbit(TIMSK, TOIE1);												// ��������� ���������� �� ������������ �� ������� 1
	TCCR1B = 0x01;															// ������������ �������� 1 = 1
	OCR1B = 0x00;

	for (CurrentPage = StartPage; CurrentPage <= EndPage; CurrentPage++)
		DF_MainMemoryToSpeaker(CurrentPage);

	clrbit(TIMSK, TOIE1);												// ��������� ���������� �� ������������ �� ������� 1
	TCCR1B = 0x00;															// ���������� ������� 1

	SPCR = 0x00;															// Disable SPI
}

void GuardIn (unsigned char InGuardSource)															// ������ ����� ����� � ����� ������
{
	SetActiveUart(UART_0);																					// !!!
	printf("Guard ON attempt ");																			// !!!

	if ((LoopTest() == 1)	 																				// ���� ������ ���������� �����
		&& (GuardSource == 0))																				// ���� �� ������ ��� �� ����������
 		switch (InGuardSource)
		{
			case GS_SWITCHER:
				if (TmNoActiveTime == 0)																	// (fw101)
				{
					printf("Switcher\n\r");																	// !!!
					Loop1Violation = 0;																		// ������� ���������� � ���������� �������
					Loop2Violation = 0;
					Loop3Violation = 0;
					TotalHackLoops = 0;
					LoopViolation = 0;																		// ������� ����������, ��������� � ������������ �������� ������� (fw109)
					GuardSource = 1;
					AttemptInError = 0;
					clrbit(PORTC, GRDLED);																	// �����, �.�. �� ��� �������� �������� ����� ���������� �������
					PreparationGuardTime = DelayGuardOnTime*60;										// �������� ��� ���������� �� ������ �������������� ��������, ��������� �� ��������� ������ SIM-�����
					SendMessageToExtUser(" GUARD ON");													// fw114
				}
				else
					printf("Warning: iButton timeout not finished\n\r");							// !!!
				break;
			case GS_IBUTTON:
				printf("iButton\n\r");																		// !!!
				Loop1Violation = 0;																			// ������� ���������� � ���������� �������
				Loop2Violation = 0;
				Loop3Violation = 0;
				TotalHackLoops = 0;
				LoopViolation = 0;																			// ������� ����������, ��������� � ������������ �������� �������  (fw109)
				GuardSource = 4;
				AttemptInError = 0;
				clrbit(PORTC, GRDLED);																		// �����, �.�. �� ��� �������� �������� ����� ���������� �������
				PreparationGuardTime = DelayGuardOnTime*60;
				SendMessageToExtUser(" GUARD ON");														// fw114
				break;
			case GS_SMS:
				printf("SMS\n\r");																			// !!!
				Loop1Violation = 0;																			// ������� ���������� � ���������� �������
				Loop2Violation = 0;
				Loop3Violation = 0;
				TotalHackLoops = 0;
				LoopViolation = 0;																			// ������� ����������, ��������� � ������������ �������� ������� (fw109)
				GuardSource = 2;
				AttemptInError = 0;
				clrbit(PORTC, GRDLED);																		// �����, �.�. �� ��� �������� �������� ����� ���������� �������
				PreparationGuardTime = DelayGuardOnTime*60;
				break;
			case GS_DTMF:
				printf("DTMF\n\r");																			// !!!
				Loop1Violation = 0;																			// ������� ���������� � ���������� �������
				Loop2Violation = 0;
				Loop3Violation = 0;
				TotalHackLoops = 0;
				LoopViolation = 0;																			// ������� ����������, ��������� � ������������ �������� ������� (fw109)
				GuardSource = 3;
				AttemptInError = 0;
				clrbit(PORTC, GRDLED);																		// �����, �.�. �� ��� �������� �������� ����� ���������� �������
				PreparationGuardTime = DelayGuardOnTime*60;
				break;
//			default:
//				printf("- ERROR: unknown source !!!\n\r");											// !!!
		}
	else
	{
		printf("- Warning: attempt fail. Source = %d\n\r", InGuardSource);					// !!!
	}

	if ((LoopTest() == 0)	 																				// ���� � ������ ���������� �������
		&& (GuardSource == 0))																				// � ���� �� ������ ��� �� ����������
		AttemptInError = 1;

	SetActiveUart(UART_1);																					// !!!
}

void GuardOut (unsigned char OutGuardSource)															// ������ ����� ������ �� ������ ������
{
	SetActiveUart(UART_0);																					// !!!
	printf("Guard OFF attempt ");																			// !!!

	if (GuardSource != 0)
		switch (OutGuardSource)
		{
			case GS_SWITCHER:
				if (iButtonUse == 0)
				{
					if (GuardSource == GS_SWITCHER)
					{
						printf("Switcher\n\r");																// !!!
						TmLockOpenTime = LockOpenTime;
						GuardSource = 0;
						SendMessageToExtUser(" GUARD OFF");												// fw114
					}
					else																							// ������� ���� ���������� �� ������ �� ���������, �� ������� ������
					{
						printf(":Warning. Switcher is not allowed\n\r");							// !!!
					}
				}
				else																								// � ������, ���� iButtonUse == 1, ����� ������� � ������ ��������� ������
				{
					printf(":Warning. iButtonUse = 1\n\r");											// !!!
				}
				break;
			case GS_IBUTTON:
				printf("iButton\n\r");																		// !!!
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
					printf("SMS\n\r");																		// !!!
					TmLockOpenTime = LockOpenTime;
				}
				else
					printf ("- SMS ERROR\n\r");
				break;
			case GS_DTMF:
				if (GuardSource != GS_SWITCHER)
				{
					GuardSource = 0;
					printf("DTMF\n\r");																		// !!!
					TmLockOpenTime = LockOpenTime;
				}
				else
					printf ("- DTMF ERROR\n\r");
				break;
//			default:
//				printf("- ERROR: unknown source !!!\n\r");											// !!!			
		}
	else
		printf("- Warning: attempt fail. Source = %d\n\r", OutGuardSource);					// !!!

	SetActiveUart(UART_1);																					// !!!
}

void LoopsRead (void)
{
	unsigned int LoopCode = 0;

	if (Loop1On > 0)									// ���� 1-� ����� ������������
	{
		LoopCode = ReadADC(LIN1);					// ���������� 1-�� ��������� ������ 
		if (LoopCode < LO_LOOP_LIMIT)
			Loop1Status = 2;							// �����
		else
			if ((LoopCode > LO_LOOP_LIMIT) && (LoopCode < HI_LOOP_LIMIT))
				Loop1Status = 0;						// �����
			else
				Loop1Status = 1;						// ��
	}//Loop1On

	if (Loop2On > 0)									// ���� 2-� ����� ������������
	{
		LoopCode = ReadADC(LIN2);					// ���������� 2-�� ��������� ������
		if (LoopCode < LO_LOOP_LIMIT)
			Loop2Status = 2;							// �����
		else
			if ((LoopCode > LO_LOOP_LIMIT) && (LoopCode < HI_LOOP_LIMIT))
				Loop2Status = 0;						// �����
			else
				Loop2Status = 1;						// ��
	}//Loop2On

	if (Loop3On > 0)									// ���� 3-� ����� ������������
	{
		LoopCode = ReadADC(LIN3);					// ���������� 3-�� ��������� ������

		if (LoopCode < LO_LOOP_LIMIT)
			Loop3Status = 2;							// �����
		else
			if ((LoopCode > LO_LOOP_LIMIT) && (LoopCode < HI_LOOP_LIMIT))
				Loop3Status = 0;						// �����
			else
				Loop3Status = 1;						// ��
	}//Loop3On
}//LoopsRead

#define	INTRO_START		0														// ������ ����� "��������, ��������� ������������. ����� �����..." � �������� �������� ����� � ��������
#define	INTRO_START_DF	INTRO_START*(SOUND_FACTOR/MAX_BUFFER)		// �������� DataFlash
#define	INTRO_END		4														// ����� ����� "��������, ��������� ������������. ����� �����..." � �������� �������� ����� � ��������
#define	INTRO_END_DF	INTRO_END*(SOUND_FACTOR/MAX_BUFFER)			// �������� DataFlash

void GuardReaxion(void)									// ������� �� ������������ �������� �������
{
	unsigned char ReaxCount = 0;
	unsigned char ReaxTemp = 0;
	char HackLoops [sizeof(" 1 2 3")] = "";
	unsigned char AbCallConfirm[MAX_AB_NUMBER];	// ��������, �� �������� �����������, ������������� 1, �� �������� �� ����������� - 0
	unsigned char ReaxCallTimeout = 0;
	unsigned char ReaxCallNum = 0;
	unsigned char ReaxVoiceAttempt = 0;
	#define VOICE_ATTEMPT_NUM 7

	SetActiveUart(UART_0);												// !!!
	printf("\n\rALARMA!!!!!\n\r");									// !!!
//	printf("LoopViolation=%d\n\r", LoopViolation);				// !!!
//	printf("GuardSource=%d\n\r", GuardSource);					// !!!
//	printf("TmGuardOffTimeout=%d\n\r", TmGuardOffTimeout);	// !!!
	SetActiveUart(UART_1);												// !!!

	TmSirenSingTime = SirenSingTime;					// ��������� � TmSirenSingTime ������ ����� ������

	// �������� SMS-��������� ��������� ��������� A � C � ��������� � ���������� �������������� ��������
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
		&& ((AbProfile[ReaxCount] == 'A') || (AbProfile[ReaxCount] == 'C') || (AbExtService[ReaxCount] == 1)))	// ���� ������������ ������������ � �� �� GUEST � �� ������������
		{
			printf("AT+CMGS=\"%s\"\n\r", AbNumber[ReaxCount]);
			delay_ms(2000);
			printf("Message to %s: ALARM! LINE(S)%s.%c", AbName[ReaxCount], HackLoops, CtrlZ);						// !!! Message to - ������, ��� ������ ��� �������

			StringBuilder();
			ReaxTemp = 0;
			while ((strstr(ReceiveString, "OK") == NULL) && (strstr(ReceiveString, "ERROR") == NULL) && (ReaxTemp < 10))	// ���� �� ������ "OK", "ERROR" ��� �� ������� ������� � 10 ������.
			{
				ReaxTemp++;
				StringBuilder();
//				SetActiveUart(UART_0);												// !!!
//				printf("Wait...\n\r");												// !!!
//				SetActiveUart(UART_1);												// !!!
				delay_ms(1000);
			}//�������� ������ �� GR-47
		}//����������� ��������, ���������� SMS-���������
	}//�������� SMS � ��������������� � ������������ ������������ ���������

	// ������ ���������
	for (ReaxCount = 0; ReaxCount < MAX_AB_NUMBER; ReaxCount++)
	{
		if ((strncmp(AbName[ReaxCount], "GUEST", sizeof("GUEST")) != 0)
		&& (strncmp(AbName[ReaxCount], "##", sizeof("##")) != 0)
		&& ((AbProfile[ReaxCount] == 'A') || (AbProfile[ReaxCount] == 'B')))	// ���� ������������ ������������ � �� �� GUEST � �� ������������
		{
			clrbit(TIMSK, TOIE0);											// ��������� ���������� �� ������������ �� ������� 0 (����� �� ����� ������������ ��������������� ����)
			setbit(PORTC, GSMLED);											// ����� ��������� - ����� ���������

			ReaxCallNum = AbCallNum[ReaxCount];

			while (ReaxCallNum != 0)
			{
//				SetActiveUart(UART_0);												// !!!
//				printf("RCN = %d\n\r", ReaxCallNum);							// !!!
//				printf("AB = %s", AbName[ReaxCount]);							// !!!
//				SetActiveUart(UART_1);												// !!!

				switch (AbCallTime[ReaxCount])
				{
					case '0':
						ReaxCallTimeout = 30; break;								// ����� ������� �� ������� �������� � ��������
					case '1':
						ReaxCallTimeout = 60; break;
					case '2':
						ReaxCallTimeout = 120; break;
					default:
						ReaxCallTimeout = 40; break;
				}

				AbCallConfirm[ReaxCount] = 0;
				printf("ATD %s;\n\r", AbNumber[ReaxCount]);
				while(ReaxCallTimeout != 0)
				{
					printf("AT+CPAS=1\n\r");
					delay_ms(1000);
					StringBuilder();

					if (strstr(ReceiveString, "130") != NULL)					// ���� GSM-������ ������� � ����� ���������
					{
						DTMFPassPos = 0;
						DTMFPassBuf[0] = 0;
						ReaxVoiceAttempt = 0;

						while ((ReaxVoiceAttempt++ < VOICE_ATTEMPT_NUM) && (DTMFPassBuf[0] != DTMF_SHARP))	// ��������� ����� ��������� ��� � ����, ���� ������������ ������ '#'
						{
							DTMFPassPos = 0;
							DTMFPassBuf[0] = 0;

							DF_PagesToSpeaker(INTRO_START_DF, INTRO_END_DF);	// "��������, ��������� ������������. ����� �����..."
							if (Loop1Violation == 1)
								DF_PagesToSpeaker(ODIN_START_DF, ODIN_END_DF);	// "...����..."
							if (Loop2Violation == 1)
								DF_PagesToSpeaker(DVA_START_DF, DVA_END_DF);		// "...���..."
							if (Loop3Violation == 1)
								DF_PagesToSpeaker(TRI_START_DF, TRI_END_DF);		// "...���..."
							printf("AT+CPAS=1\n\r");
							delay_ms(500);
							StringBuilder();
							if (strstr(ReceiveString, "129") != NULL)				// ���� ������� ������� ������
								break;
						}

						AbCallConfirm[ReaxCount] = 1;
						ReaxCallNum = 0;
						break;
					}
					if (strstr(ReceiveString, "129") != NULL)					// ���� ������� ��� � �� ���� ������ �� ��������� �������� ���������
						break;
					ReaxCallTimeout--;
				}
				StringBuilder();
				printf("ATH\n\r");												// ������ ������
				delay_ms(2000);
				StringBuilder();
				if (ReaxCallNum != 0) ReaxCallNum--;
			}

			clrbit(PORTC, GSMLED);											// �����
			setbit(TIMSK, TOIE0);											// ��������� ���������� �� ������������ �� ������� 0
		}//����������� ��������, ���������� ������
	}//������ ��������� � ��������������� � ������������ ������������ ��������

	// �������� SMS-��������� ��� ��������� ��������� B, �� ������� �� ��� � �� ������ �����������
	for (ReaxCount = 0; ReaxCount < MAX_AB_NUMBER; ReaxCount++)
	{
		if ((strncmp(AbName[ReaxCount], "GUEST", sizeof("GUEST")) != 0)
		&& (strncmp(AbName[ReaxCount], "##", sizeof("##")) != 0)
		&& ((AbProfile[ReaxCount] == 'B') && (AbCallConfirm[ReaxCount] == 0)))
		{
			printf("AT+CMGS=\"%s\"\n\r", AbNumber[ReaxCount]);
			delay_ms(2000);
			printf("Message to %s: ALARM! LINE(S)%s.%c", AbName[ReaxCount], HackLoops, CtrlZ);										// !!! Message to - ������, ��� ������ ��� �������

			StringBuilder();
			ReaxTemp = 0;
			while ((strstr(ReceiveString, "OK") == NULL) && (strstr(ReceiveString, "ERROR") == NULL) && (ReaxTemp < 10))	// ���� �� ������ "OK", "ERROR" ��� �� ������� ������� � 10 ������.
			{
				ReaxTemp++;
				StringBuilder();
//				SetActiveUart(UART_0);												// !!!
//				printf("Wait...\n\r");												// !!!
//				SetActiveUart(UART_1);												// !!!
				delay_ms(1000);
			}//�������� ������ �� GR-47
		}//����������� ��������, ���������� SMS-���������
	}//�������� SMS "�������������" ��������� ���������
}//GuardReaxion

/*char* ToUpperString(char* UpString)					// ����������� ������ � ������� �������
{
	unsigned char UpStrCount = 0;

	for (; UpStrCount <= strlen(UpString); UpStrCount++)
		UpString[UpStrCount] = toupper(UpString[UpStrCount]);

	return UpString;
}//ToUpperString*/

void SendMessageToExtUser (char* MessageStr)	// ������� ��������� �������������, � ������� ������� �������������� ������. ���� �������������� ������ �� ������� �� � ������ ��������, �������� ������ �� �����. fw111
{
	unsigned char ExtUserCount = 0;
	unsigned char ExtUserTemp = 0;

	SetActiveUart(UART_0);												// !!!
	printf("Message =%s\n\r", MessageStr);							// !!!
	SetActiveUart(UART_1);												// !!!

	clrbit(TIMSK, TOIE0);												// ��������� ���������� �� ������������ �� ������� 0

	for (ExtUserCount = 0; ExtUserCount < MAX_AB_NUMBER; ExtUserCount++)
	{
//		SetActiveUart(UART_0);												// !!!
//		printf("SEND MESSAGE AbName = %s\n\r", AbName[ExtUserCount]);						// !!!
//		SetActiveUart(UART_1);												// !!!

		if ((strncmp(AbName[ExtUserCount], "GUEST", sizeof("GUEST")) != 0)
		&& (strncmp(AbName[ExtUserCount], "##", sizeof("##")) != 0)
		&& (AbExtService[ExtUserCount] == 1))	// ���� ������������ ������������ � �� �� GUEST � ����� ����� �� �������������� ������
		{
			printf("AT+CMGS=\"%s\"\n\r", AbNumber[ExtUserCount]);
			delay_ms(2000);
			printf("Message to %s:%s.%c", AbName[ExtUserCount], MessageStr, CtrlZ);										// !!! Message to - ������, ��� ������ ��� �������

			StringBuilder();
			ExtUserTemp = 0;
			while ((strstr(ReceiveString, "OK") == NULL) && (strstr(ReceiveString, "ERROR") == NULL) && (ExtUserTemp < 10))	// ���� �� ������ "OK", "ERROR" ��� �� ������� ������� � 10 ������.
			{
				ExtUserTemp++;
				StringBuilder();
				delay_ms(1000);
			}//�������� ������ �� GR-47
		}//����������� ��������, ���������� SMS-���������
	}//�������� SMS ���������

	setbit(TIMSK, TOIE0);											// ��������� ���������� �� ������������ �� ������� 0
}//SendMessageToExtUser

#endif
