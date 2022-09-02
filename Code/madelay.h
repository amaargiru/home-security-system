#ifndef _ma_delay_
#define _ma_delay_

#define delay_us(c) __delay_cycles(MasterClock/1000000*c)	// Микросекундная задержка
#define delay_ms(c) __delay_cycles(MasterClock/1000*c)		// Миллисекундная задержка

#endif
