#ifndef EEUtil_H
#define EEUtil_H

#include <Arduino.h>
#include <inttypes.h>
#include <EEPROM.h>
#include <NetwBase.h>

//#define offsetof(TYPE, MEMBER) __builtin_offsetof (TYPE, MEMBER)
// #elif defined ARDUINO_ARCH_RP2040


template <class T> int EEUtil_writeAny(int ee, const T& value)
{
	//Serial.print("EEWriteAny:");Serial.print(value);Serial.print(", offset:");Serial.println(ee);
	const byte* p = (const byte*)(const void*)&value;
	unsigned int i;
	for (i = 0; i < sizeof(value); i++)
			EEPROM.write(ee++, *p++);

//		#ifdef ESP8266
//			EEPROM.commit();
//		#endif

	return (i);
}

template <class T> int EEUtil_readAny(int ee, T& value)
{
	byte* p = (byte*)(void*)&value;
	unsigned int i;
	for (i = 0; i < sizeof(value); i++)
			*p++ = EEPROM.read(ee++);
	return (i);
}

class EEUtil
{
public:
	EEUtil()
	{
		// bootMessages = 0;
//		#ifdef ESP8266
//		#endif
	}
	// x(class Netw &netww){ this->parent = &parentNode; } NetwBase        *parent;
	//EEProm( class NetwBase &parentNode ){ this->parent = &parentNode; *hangPtr = &hang; }

    // ATmega328 has 1024 byte space for variables


	int (*uploadFunc) (int id, long val, unsigned long timeStamp) = 0;
    void onUpload( int (*function)(int id, long val, unsigned long timeStamp) )
    {
    	uploadFunc = function;
    }
    int upload(int id);

	volatile long 	samplePeriode_sec  = 51L;
	volatile long 	bootCount          = 0;

	volatile bool 	changed	= true;
	bool 			hang = false;
	uint8_t  		bootMessages = 0;
	unsigned long 	bootTimer = 100;
	bool 		 	bootTimerOverflow = false;

	virtual int setVal( int id, long value );
//	virtual int getVal( int id, long * value );
	virtual int handleRequest( RxItem *rxItem);

	void  resetBootCount(void);
	// {
	// 	bootCount=0;
	// 	changed = true;
	// 	//write(offsetof(EEProm, bootCount), bootCount);
	// }

    bool (*user_onReBoot)(void);
    void onReBoot( bool (*function)(void)){user_onReBoot = function;}

    long  readLong(int offSet);
    int   readInt(int offSet);
    bool  readBool(int offSet);
    float readFloat(int offSet);
    byte  readByte(int offSet);
    void write(int offSet, unsigned long newVal);
    void write(int offSet, long newVal);
    void write(int offSet, int newVal);
    void write(int offSet, bool newVal);
    void write(int offSet, float newVal);
    void write(int offset, const char* source, int len );
    void readString(char* dest, int offSet, int len  );
    void readAll();
    void writeAll();
	void loop();
};

#endif
