#ifndef _ds_1990a_
#define _ds_1990a_

#define DS_PORT PORTA 
#define DS_DIR DDRA
#define DS_PIN PINA
unsigned char DS_bit  = 3;

#define timeA 6
#define timeB 64
#define timeC 60
#define timeD 10
#define timeE 9
#define timeF 55
#define timeG 0
#define timeH 480
#define timeI 70
#define timeJ 410

#define DS1990A_ID	0x01  // READ ROM command code
#define W1_ROM_READ	0x33  // READ ROM command code

unsigned char uidDS[8];  // The 64 bit identifier

unsigned char w1_read(void);
void w1_write(unsigned char data);
unsigned char w1_init(void);
unsigned char w1_readBit(void);
void w1_writeBit(unsigned char bit);
unsigned char ds1990a_init(void);
unsigned char testDQ(void);
unsigned char CRC8(unsigned char *ptr, unsigned char count);

unsigned char testDQ(void)
{
	if (tstbit(DS_PIN,DS_bit)) return 1;
		else return 0;
}

void setDQ()
{
	clrbit(DS_DIR,DS_bit);  __no_operation();
}

void clrDQ()
{
	setbit(DS_DIR,DS_bit);
	clrbit(DS_PORT,DS_bit);
}

unsigned char w1_read(void)
{
	int loop, result=0;

	__disable_interrupt();
	for (loop = 0; loop < 8; loop++)
	{
		result >>= 1;
		if (w1_readBit())
			result |= 0x80;
	}
	__enable_interrupt();

	return result;
}

void w1_write(unsigned char data)
{
	int loop;

	__disable_interrupt();
	for (loop = 0; loop < 8; loop++)
	{
		w1_writeBit(data & 0x01);
		data >>= 1;
	}
	__enable_interrupt();

}

unsigned char w1_init(void)
{
	int result;

	__disable_interrupt();
	delay_us(timeG);
	clrDQ();
	delay_us(timeH);
	setDQ();
	delay_us(timeI);
	result = testDQ();
	delay_us(timeJ);
	__enable_interrupt();
	return result;
}

unsigned char w1_readBit(void)
{
	int result;
	clrDQ();
	delay_us(timeA);
	setDQ();
	delay_us(timeE);
	result = testDQ();
	delay_us(timeF);
	return result;
}

void w1_writeBit(unsigned char bit)
{
	if (bit)
	{
		clrDQ();
		delay_us(timeA);
		setDQ();
		delay_us(timeB);
	}
	else
	{
		clrDQ();
		delay_us(timeC);
		setDQ();
		delay_us(timeD);
	}
}

unsigned char ds1990a_init(void)
{
	unsigned char i;
	if (w1_init())
	{
		for (i=0; i<8; i++) 
			uidDS[i] = 0;
		return 0;
	}
	else
	{
		w1_write(W1_ROM_READ);
		for (i=0; i<8; i++) 
		uidDS[i] = w1_read();
		return 1;
	}		
}

unsigned char CRC8(unsigned char *ptr, unsigned char count)
{
	unsigned char crc=0;
	unsigned char i, c, tmp;

	while (count-- != 0)
	{
		c = *ptr++;
		i = 8;
		do
		{
			tmp=c;
			tmp ^= crc;
			crc >>= 1;
			c >>= 1;
			if(tmp & 1)
			{
				crc ^= 0x8C;
			}
		}
		while(--i);
	}
	return crc;
} 

#endif
