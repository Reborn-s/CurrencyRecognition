// Dib.h: interface for the CDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIB_H_INCLUDED_)
#define AFX_DIB_H_INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define _AFXDLL

/////////////////////////////////////////////////
#include<iostream>
#include <afx.h> 
using namespace std;
////////////////////////////////////////////////

class CDib  
{
public:
	CDib();
	virtual ~CDib();

//Attributes
public:
	BYTE    *GetBits();
	LONG	 GetWidth();
	LONG	 GetHeight();
	int      GetBiBitCount();
	

//operations
public:
	BOOL Open(CString& pzFileName);
	void Close();

//public parameters
	BYTE		*m_pDibBits;
//implementation
public:
	BYTE		*m_pDib;
	

	BITMAPFILEHEADER bmpFileHeader;
};
#endif 
