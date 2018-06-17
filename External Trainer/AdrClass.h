#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <ctime>

#pragma warning(disable : 4996)

class AdrClass
{
private:
	//Definitionen der Attribute
	DWORD ClassBaseAdress; //Hauptadresse des Speichers
	DWORD* ClassOffsets; //Offset Array um Beschreibare Adresse herauszufinden
	char ClassValue[]; //eingesetzter Wert

public:
	//Definitionen der Methoden
	//Konstruktor
	AdrClass(const char* Value, DWORD BaseAdress, int OffsetNumber, DWORD* Offsets);
	//Destructor
	~AdrClass();

	//Get Methoden
	DWORD GetClassBaseAdress();
	DWORD* GetClassOffsets();
	char* GetClassValue();

};


AdrClass::AdrClass(const char* Value, DWORD BaseAdress, int OffsetNumber, DWORD* Offsets)
{
	ClassOffsets = new DWORD[OffsetNumber];
	AdrClass::ClassBaseAdress = BaseAdress;
	for (int i = 0; i < OffsetNumber; i++)
	{
		AdrClass::ClassOffsets[i] = Offsets[i];
	}

	strcpy(AdrClass::ClassValue, Value);
}



//Destructor Definition
AdrClass::~AdrClass()
{
	delete [] ClassOffsets;
}


//Get Methoden
DWORD AdrClass::GetClassBaseAdress()
{
	return AdrClass::ClassBaseAdress;
}

DWORD* AdrClass::GetClassOffsets()
{
	return AdrClass::ClassOffsets;
}

char* AdrClass::GetClassValue()
{
	return AdrClass::ClassValue;
}
