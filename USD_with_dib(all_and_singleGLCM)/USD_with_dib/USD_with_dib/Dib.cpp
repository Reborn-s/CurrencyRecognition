#include "Dib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDib::CDib()
{
	m_pDib=NULL;
}

CDib::~CDib()
{
	Close();
}

LONG CDib::GetWidth()
{
	return ((BITMAPINFOHEADER *)m_pDib)->biWidth;
}

LONG CDib::GetHeight()
{
	return	((BITMAPINFOHEADER *)m_pDib)->biHeight;
}

void CDib::Close()
{
	if(m_pDib!=NULL)
	{
		delete m_pDib;
		m_pDib=NULL;
	}
}

BOOL CDib::Open(CString& pzFileName)
{
	CFile file;
	int nBmpFileHeaderSize;
	Close();

	nBmpFileHeaderSize=sizeof(BITMAPFILEHEADER);

	if(!file.Open(pzFileName,CFile::modeRead | CFile::typeBinary))
		goto exit;  

	if(file.Read((void *)&bmpFileHeader,nBmpFileHeaderSize)!=(UINT)nBmpFileHeaderSize)
		goto failure;

	if(bmpFileHeader.bfType!=*((WORD *)"BM"))
		goto failure;

	m_pDib=(BYTE *)new char [bmpFileHeader.bfSize-nBmpFileHeaderSize];

	if(!m_pDib)
		goto failure;

	file.Read(m_pDib,bmpFileHeader.bfSize-nBmpFileHeaderSize);

	if(((BITMAPINFOHEADER *)m_pDib)->biSizeImage==0)
	{
		BITMAPINFOHEADER *pDib=(BITMAPINFOHEADER *)m_pDib;
		pDib->biSizeImage=((((pDib->biWidth*pDib->biBitCount)+31)&~31)>>3)*pDib->biHeight;
	}

	m_pDibBits=GetBits();

	file.Close();
	return TRUE;

failure:
	file.Close();
	cout<<"请选择BMP格式的图片！"<<endl;
exit:
	Close();
	cout<<"打开文件失败，请输入正确的BMP图片存储位置！"<<endl;
	return FALSE;
}


BYTE * CDib::GetBits()
{
	DWORD dwNumColors,dwColorTableSize;
	BITMAPINFOHEADER *lpDib=(BITMAPINFOHEADER *)m_pDib;

	WORD wBitCount=lpDib->biBitCount;

	if(lpDib->biSize>=36)
		dwNumColors=lpDib->biClrUsed;
	else
		dwNumColors=0;

	if(dwNumColors==0)
	{
		if(wBitCount!=24)
			dwNumColors=1L<<wBitCount;
		else 
			dwNumColors=0;
	}

	dwColorTableSize=dwNumColors*sizeof(RGBQUAD);

	return m_pDib+lpDib->biSize+dwColorTableSize;
}

int CDib::GetBiBitCount()
{
	if(m_pDib!=NULL)
		return ((BITMAPINFOHEADER *)m_pDib)->biBitCount; 
	return 0;
}
