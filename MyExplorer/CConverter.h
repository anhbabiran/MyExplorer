#ifndef CCONVERTER_H
#define CCONVERTER_H
#include <windows.h>
#include "tchar.h"

class CConverter
{
public:
	static LPWSTR Convert(__int64 nSize);
};

#endif