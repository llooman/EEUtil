#include "EEUtil.h"


void EEUtil::readAll()
{
	bootCount  = readLong(offsetof(EEUtil, bootCount))+1;
	write(offsetof(EEUtil, bootCount), bootCount);
	#ifdef ESP8266
		EEPROM.commit();
	#endif
}

void  EEUtil::resetBootCount()
{
	bootCount=0;
	// changed = true;
	writeAll( );
	//write(offsetof(EEProm, bootCount), bootCount);
}



void EEUtil::writeAll( )
{

	write(offsetof(EEUtil, bootCount), bootCount);
	#ifdef ESP8266
		EEPROM.commit();
	#endif
}


int EEUtil::handleRequest(RxItem *rxItem)  // cmd, to, parm1, parm2  // TODO
{
	switch (rxItem->data.msg.cmd)
	{
	case 'N':
		bootMessages=0;
		break;

	case 'b':
		resetBootCount();		 
		break;

 	default: return -1; break;
	}
	return 0;
}

void EEUtil::loop()
{
 
}

int EEUtil::setVal( int id, long value ) //bool sending, int id,
{
	switch(id)
	{		
	case 0:
	case 1:
		bootMessages=0;
		break;

	case 3: resetBootCount();						break;
	case 50: digitalWrite(LED_BUILTIN, (value>0));	break;
 	default: return -1; break;
	}
	return 0;
}

int EEUtil::upload(int id)
{
	if(uploadFunc==0 ) return 0;

	int ret=0;

	switch( id )
	{
	case 1:ret = uploadFunc(id, millis(), 0 ); 					break;
	case 3:ret = uploadFunc(id, bootCount, 0 ); 				break;
	case 5:ret = uploadFunc(id, 0, 0 );							break;    
	case 50:ret = uploadFunc(id, digitalRead(LED_BUILTIN), 0);	break;
	default: return 0;											break;
	}
	return ret;
}
 
long EEUtil::readLong(int offSet)
{
	long tempLong;
	EEUtil_readAny(offSet, tempLong);
	return tempLong;
}
byte EEUtil::readByte(int offSet)
{
	byte _byte;
	EEUtil_readAny(offSet, _byte);
	return _byte;
}
int EEUtil::readInt(int offSet)
{
	int tempInt;
	EEUtil_readAny(offSet, tempInt);
//	Serial.print("EEReadInt:"); Serial.print(tempInt);;Serial.print(", offset:");Serial.println(offSet);
	return tempInt;
}
bool EEUtil::readBool(int offSet  )
{
	bool tempBool;
	EEUtil_readAny(offSet, tempBool);
	return tempBool;
}
float EEUtil::readFloat(int offSet  )
{
	float tempFloat;
	EEUtil_readAny(offSet, tempFloat);
	return tempFloat;
}
/*void myEEwrite(int address, byte val)
{
  if (EEPRO.read(address) == val) return;  // if the value already exist, no need to write.
  EEUtil.write(address.value);
}*/
void EEUtil::write(int offSet, long newVal )
{
	long tempLong;
	EEUtil_readAny(offSet, tempLong);
	if( newVal != tempLong )EEUtil_writeAny(offSet, newVal);
}
void EEUtil::write(int offSet, unsigned long newVal )
{
	unsigned long tempLong;
	EEUtil_readAny(offSet, tempLong);
	if( newVal != tempLong )EEUtil_writeAny(offSet, newVal);
}
void EEUtil::write(int offSet, int newVal )
{
	int tempInt;
	EEUtil_readAny(offSet, tempInt);
	if( newVal != tempInt )
	{
//		Serial.print("EEWriteInt:");Serial.print(newVal);Serial.print(", offset:");Serial.println(offSet);
		EEUtil_writeAny(offSet, newVal);
	}
}
void EEUtil::write(int offSet, bool newVal )
{
	bool tempBool;
	EEUtil_readAny(offSet, tempBool);
	if( newVal != tempBool )EEUtil_writeAny(offSet, newVal);
}
void EEUtil::write(int offSet, float newVal )
{
	float tempFloat;
	EEUtil_readAny(offSet, tempFloat);
	if( newVal != tempFloat ) 
		EEUtil_writeAny(offSet, newVal);
}

// Strings
void EEUtil::write(int offSet, const char* source, int len )
{
	char letter;

	for (int i = 0; i < len; i++)
	{
		EEUtil_readAny(offSet+i, letter);
		if( *source != letter )
		{
			EEPROM.write(offSet++, *source++);
		}
	}

	#ifdef ESP8266
		EEPROM.commit();
	#endif
}
void EEUtil::readString(char* dest, int offSet, int len  )
{
	unsigned int i;

	for (i = 0; i<len; i++)
	{
		*dest++ = EEPROM.read(offSet++);
	}

	dest--;
	*dest=0x00;  	// make sure 0x00 is at the end!!
}
