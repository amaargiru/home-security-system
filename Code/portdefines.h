// Порт A
#define POUT2		0	// Ключ 2
#define JIBP		1	// Джампер 4 (iButton) "верх"
#define JIBM		2	// Джампер 4 (iButton) "низ"
#define IBTN		3	// iButton
#define VDET		4	// Наличие внешнего напряжения

// Порт B
#define SCK			1	//	Тактовая для обмена с AT45DB041B
#define SI			2	// Выход информации на AT45DB041B
#define SO			3	// Вход информации от AT45DB041B
#define PWDN		4	// Отключение DTMF-декодера MT8870DS
#define CHARG		5	// Включение зарядки аккумулятора (иначе работает "капельный" режим)
#define SPKOUT		6	//	Выход ШИМ сигнала на микрофонный вход GR-47
#define POUT1		7	//	Ключ 1

// Порт C
#define SIMSW		0	//	Переключение SIM карт
#define EXTKEY		1	//	Внешний тумблер постановки на охрану
#define EGRDLED	2	//	Внешний светодиод "Режим охраны"
#define EGSMLED	3	//	Внешний светодиод "Режим GSM"
#define EPOWLED	4	//	Внешний светодиод "Режим питания"
#define GRDLED		5	// Светодиод "Режим охраны"
#define GSMLED		6	//	Светодиод "Режим GSM"
#define POWLED		7	// Светодиод "Режим питания"

// Порт D
#define PON			0	// Включение GR-47
#define PDTR		1	// DTR
#define PRD			2	//	RXD1
#define PTD			3	// TXD1
#define PRTS		4	// RTS
#define PCTS		5	// CTS
#define PDCD		6	// DCD
#define PRI			7	// RI

//	Порт E
#define RXD0		0	// Приемник UART0 / Последовательная загрузка по SPI
#define TXD0		1	// Передатчик UART0 / Послкдовательный выход по SPI
#define DTQ1		2	// Информационный бит 1 от DTMF-декодера MT8870DS
#define DTQ2		3	// Информационный бит 2 от DTMF-декодера MT8870DS
#define STD			4	// "Готовность информации" от DTMF-декодера MT8870DS
#define DTQ3		5	// Информационный бит 3 от DTMF-декодера MT8870DS
#define DTQ4		6	// Информационный бит 4 от DTMF-декодера MT8870DS
#define CON			7	// Включение токовых шлейфов

// Порт F
#define AFMS		0	// Аналоговый вход от GR-47 (для возможности организации программного DTMF-декодирования)
#define LIN1		1	// Сигнал от 1-й токовой петли
#define LIN2		2	// Сигнал от 2-й токовой петли
#define LIN3		3	// Сигнал от 3-й токовой петли
#define VBAT		4	//	Напряжение на аккумуляторе
#define J1			5	//	Аналоговый сигнал от 1-го джампера
#define J2			6	//	Аналоговый сигнал от 2-го джампера
#define J3			7	//	Аналоговый сигнал от 3-го джампера

// Порт G
#define nFCS		0	// Chip Select AT45DB041B ("активный ноль")
#define nFRES		1	// Reset AT45DB041B ("активный ноль")
#define nGSMPOW	2	// Включение GR-47 ("активный ноль")
#define K2			3	// Реле 2
#define K1			4	// Реле 1
