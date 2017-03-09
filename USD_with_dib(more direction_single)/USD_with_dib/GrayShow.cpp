//////////////////////////////////////////////////////////////////////
// GrayShow.cpp: implementation of the CGrayShow class.
// �Ҷȹ��־����㷨ʵ��
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "Texture.h"
#include "GrayShow.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrayShow::CGrayShow()
{
	PMatrixRD = NULL;	//45�ȷ����ϵĻҶȹ��־���
	PMatrixLD = NULL;	//135�ȷ����ϵĻҶȹ��־���
	PMatrixV = NULL;	//90�ȷ����ϵĻҶȹ��־���
	PMatrixH = NULL;	//0�ȷ����ϵĻҶȹ��־���
	ImageArray = NULL;	//ԭʼ��ͼ����������

	m_tOriPixelArray = NULL;//ԭʼλͼ����
	m_tResPixelArray = NULL;//�ο�λͼ����

	m_dib = new CDib();

	distance = 3;
	//FilterWindowWidth = 16;
	GrayLayerNum = 8; //��ʼ����Ϊ8���ҶȲ㣬�����޸�
	//GrayLayerNum = 16;
	//�������־����ΪGrayLayerNum��GrayLayerNum
	int i;
	PMatrixH = new int*[GrayLayerNum];
	PMatrixLD= new int*[GrayLayerNum];
	PMatrixRD= new int*[GrayLayerNum];
	PMatrixV = new int*[GrayLayerNum];
	for(i=0; i<GrayLayerNum; i++)
	{
		PMatrixH[i] = new int[GrayLayerNum];
		PMatrixLD[i]= new int[GrayLayerNum];
		PMatrixRD[i]= new int[GrayLayerNum];
		PMatrixV[i] = new int[GrayLayerNum];
	}

}

//���ι��캯��
/*
CGrayShow::CGrayShow(int _distance)
{
	PMatrixRD = NULL;	//45�ȷ����ϵĻҶȹ��־���
	PMatrixLD = NULL;	//135�ȷ����ϵĻҶȹ��־���
	PMatrixV = NULL;	//90�ȷ����ϵĻҶȹ��־���
	PMatrixH = NULL;	//0�ȷ����ϵĻҶȹ��־���
	ImageArray = NULL;	//ԭʼ��ͼ����������

	m_tOriPixelArray = NULL;//ԭʼλͼ����
	m_tResPixelArray = NULL;//�ο�λͼ����
	m_dib = new CDib();

	distance = _distance;
	//FilterWindowWidth = 16;
	GrayLayerNum = 8; //��ʼ����Ϊ8���ҶȲ㣬�����޸�
	//GrayLayerNum = 16;
	//�������־����ΪGrayLayerNum��GrayLayerNum
	int i;
	PMatrixH = new int*[GrayLayerNum];
	PMatrixLD= new int*[GrayLayerNum];
	PMatrixRD= new int*[GrayLayerNum];
	PMatrixV = new int*[GrayLayerNum];
	for(i=0; i<GrayLayerNum; i++)
	{
		PMatrixH[i] = new int[GrayLayerNum];
		PMatrixLD[i]= new int[GrayLayerNum];
		PMatrixRD[i]= new int[GrayLayerNum];
		PMatrixV[i] = new int[GrayLayerNum];
	}
}
*/

CGrayShow::~CGrayShow()
{
	
	for(int i=0; i<GrayLayerNum; i++)
	{
		delete []PMatrixH[i];
		delete []PMatrixLD[i];
		delete []PMatrixRD[i];
		delete []PMatrixV[i];
		
	}
	delete []PMatrixH;
	delete []PMatrixLD;
	delete []PMatrixRD;
	delete []PMatrixV;
	//printf("\nִ����CGrayShow����������\n");
	delete m_dib;

	if(ImageArray != NULL)
	{
	for(int i=0; i<ImageHeight; i++)
		delete []ImageArray[i];
	delete []ImageArray;
	ImageArray = NULL;
	}
}

BYTE** CGrayShow::getImageArray()
{
	return this->ImageArray;
}

int CGrayShow::getImageWidth()
{
	return this->ImageWidth;
}

int CGrayShow::getImageHeight()
{
	return this->ImageHeight;
}

unsigned char**  CGrayShow::cmatrix(long nrl,long nrh,long ncl,long nch)
{
	int i;
	unsigned char **m;

	m=(unsigned char **) calloc((unsigned) (nrh-nrl/*+1*/),sizeof(unsigned char*));
	if (!m) printf("allocation failure 1 in dmatrix()");
	m -= nrl;

	for(i=nrl;i<=nrh;i++) {
		m[i]=(unsigned char *) calloc((unsigned) (nch-ncl/*+1*/),sizeof(unsigned char));
		if (!m[i]) printf("allocation failure 2 in dmatrix()");
		m[i] -= ncl;
	}
	return m;
}

//ͼ��ָ��,��ʱ���ָ�����һ�в���splitWidth���������
vector<BYTE**>  CGrayShow::split(BYTE** ImageArray,int ImageWidth,int ImageHeight,int splitWidth)
{
	vector<BYTE**> splitResult;		//����ֵ
	int widthCount  = ImageWidth/splitWidth;
	int heightCount = ImageHeight/splitWidth;
	int roll = widthCount * heightCount;

	for(int i=0;i<roll;++i)
	{
		BYTE** splitImage = new BYTE*[splitWidth];
		for(int k=0;k<splitWidth;k++)
		{
			splitImage[k] = new BYTE[splitWidth];
		}
		//int a = splitWidth*sizeof(BYTE*);
		//memset(splitImage,0,splitWidth*sizeof(BYTE*));
		for(int p=0;p<splitWidth;p++)
		{
			for(int q=0;q<splitWidth;q++)
			{
				splitImage[p][q] = ImageArray[p+(i/widthCount)*splitWidth][q+(i%widthCount)*splitWidth];
			}
		}
		splitResult.push_back(splitImage);
	}
	return splitResult;


	/*
	int roll = ImageWidth/splitWidth;
	vector<BYTE**> splitResult;
	
	//BYTE** splitImage;
	//splitImage = cmatrix(0, ImageHeight, 0, splitWidth);

	for(int i=0;i<roll;++i)
	{
		//-----------------------begin yangfan 2016_7_31
		BYTE** splitImage;
		splitImage = cmatrix(0, ImageHeight, 0, splitWidth);
		//-----------------------end   yangfan 2016_7_31

		for(int p=0;p<ImageHeight;p++)
		{
			for(int q=0;q<splitWidth;q++)
			{
				splitImage[p][q] = ImageArray[p][q+i*splitWidth];
			}
		}
		splitResult.push_back(splitImage);
	}
	//�ָ�����һ�п�Ȳ���splitWidth
	if(roll*splitWidth<ImageWidth)
	{
		BYTE** LastSplitImage;
		LastSplitImage = cmatrix(0,ImageHeight, 0, ImageWidth-roll*splitWidth);
		for(int p=0;p<ImageHeight;p++)
		{
			for(int q=roll*splitWidth;q<ImageWidth;++q)
			{
				//--------------------------------------begin yangfan 2016_7_4
				LastSplitImage[p][q-roll*splitWidth] = ImageArray[p][q];
				//--------------------------------------end   yangfan 2016_7_4
			}
		}
		splitResult.push_back(LastSplitImage);
	}
	return splitResult;
	*/
}

//ÿ�κ����ƶ�Move����λ
vector<BYTE**>  CGrayShow::splitAndMove(BYTE** ImageArray,int ImageWidth,int ImageHeight,int splitWidth,int Move)
{
	vector<BYTE**> splitResult;		//����ֵ
	//����ÿ�еķָ����
	int widthCount = ImageWidth-splitWidth;
	widthCount = widthCount/Move;
	widthCount += 1;
	//����ÿ�еķָ����
	int heightCount = ImageHeight-splitWidth;
	heightCount = heightCount/Move;
	heightCount += 1;
	//�ָܷ����
	int roll = widthCount * heightCount;
	for(int i=0;i<roll;++i)
	{
		BYTE** splitImage = new BYTE*[splitWidth];
		for(int k=0;k<splitWidth;k++)
		{
			splitImage[k] = new BYTE[splitWidth];
		}
		for(int p=0;p<splitWidth;p++)
		{
			for(int q=0;q<splitWidth;q++)
			{
				splitImage[p][q] = ImageArray[p+(i/widthCount)*Move][q+(i%widthCount)*Move];
			}
		}
		splitResult.push_back(splitImage);
	}

	return splitResult;
}




/////////////////////////////////////////////////////////////////////////////////////
//���ܣ����㹲�־���
//������LocalImage����������ľֲ���������ͼ��
//      LocalImageWidth���ֲ�����������
////////////////////////////////////////////////////////////////////////////////////
void CGrayShow::ComputeMatrix(BYTE **LocalImage, int LocalImageWidth, int LocalImageHeight)
{
	BYTE **NewImage;
	NewImage = new BYTE*[LocalImageHeight];

	int i,j;
	for(i=0; i<LocalImageHeight; i++)
		NewImage[i] = new BYTE[LocalImageWidth];

	for(i=0; i<LocalImageHeight; i++)
	{
		for(j=0; j<LocalImageWidth; j++)
		{
			//�ֳ�GrayLayerNum���Ҷȼ�
			NewImage[i][j] = LocalImage[i][j] / (256/GrayLayerNum); 


		}
	}

	for(i=0; i<GrayLayerNum; i++)
	{
		//
		for(j=0; j<GrayLayerNum; j++)
		{
			PMatrixH[i][j]  = 0;
			PMatrixLD[i][j] = 0;
			PMatrixRD[i][j] = 0;
			PMatrixV[i][j]  = 0;
		}
	}

	//����0�ȵĻҶȹ�����
	for(i=0; i<LocalImageHeight; i++)
	{
		for(j=0; j<LocalImageWidth-distance; j++)
		{
			//��NewImage[i][j]��ת���õ�
			PMatrixH[(unsigned int)NewImage[i][j]][(unsigned int)NewImage[i][j+distance]] += 1;
			PMatrixH[(unsigned int)NewImage[i][j+distance]][(unsigned int)NewImage[i][j]] += 1;
		}
	}

	//=======================================================�������
	//cout<<"=================================0���������================================="<<endl;
	//cout<<"��һ��ǰ��0��Ҷȹ�����: "<<endl;
	//int k=0;
	//for(int m=0; m<GrayLayerNum; m++)
	//{
		//for(int n=0; n<GrayLayerNum; n++)
		//{
			//cout<<PMatrixH[m][n]<<" ";
			//k+=PMatrixH[m][n];
		//}
		//cout<<endl;
	//}
	//cout<<k<<endl;
	//cout<<"=================================0���������================================="<<endl;
	//cout<<endl<<endl;
	//=======================================================�������


	//����45�ȵĻҶȹ�����
	for(i=distance; i<LocalImageHeight; i++)
	{
		for(j=0; j<LocalImageWidth-distance; j++)
		{
			//��NewImage[i][j]��ת���õ�
			int newi, newj;
			newi = i-distance;
			newj = j+distance;
			PMatrixRD[(unsigned int)NewImage[i][j]][(unsigned int)NewImage[newi][newj]] += 1;
			PMatrixRD[(unsigned int)NewImage[newi][newj]][(unsigned int)NewImage[i][j]] += 1;
		}
	}
	//=======================================================�������
	//cout<<"=================================45���������================================="<<endl;
	//cout<<"��һ��ǰ��45��Ҷȹ�����: "<<endl;
	//for(int m=0; m<GrayLayerNum; m++)
	//{
		//for(int n=0; n<GrayLayerNum; n++)
		//{
			//cout<<PMatrixRD[m][n]<<" ";
		//}
		//cout<<endl;
	//}
	//cout<<"=================================45���������================================="<<endl;
	//cout<<endl<<endl;
	//=======================================================�������



	//����90�ȵĻҶȹ�����
	for(j=0; j<LocalImageWidth; j++)
	{
		for(i=0; i<LocalImageHeight-distance; i++)
		{
			//��NewImage[i][j]��ת���õ�
			PMatrixV[(unsigned int)NewImage[i][j]][(unsigned int)NewImage[i+distance][j]] += 1;
			PMatrixV[(unsigned int)NewImage[i+distance][j]][(unsigned int)NewImage[i][j]] += 1;
		}
	}
	//=======================================================�������
	//cout<<"=================================90���������================================="<<endl;
	//cout<<"��һ��ǰ��90��Ҷȹ�����: "<<endl;
	//for(int m=0; m<GrayLayerNum; m++)
	//{
		//for(int n=0; n<GrayLayerNum; n++)
		//{
			//cout<<PMatrixV[m][n]<<" ";
		//}
		//cout<<endl;
	//}
	//cout<<"=================================90���������================================="<<endl;
	//cout<<endl<<endl;
	//=======================================================�������




	//����135�ȵĻҶȹ�����
	for(i=0; i<LocalImageHeight-distance; i++)
	{
		for(j=0; j<LocalImageWidth-distance; j++)
		{
			//��NewImage[i][j]��ת���õ�
			int newi, newj;
			newi = i+distance;
			newj = j+distance;
			PMatrixLD[(unsigned int)NewImage[i][j]][(unsigned int)NewImage[newi][newj]] += 1;
			PMatrixLD[(unsigned int)NewImage[newi][newj]][(unsigned int)NewImage[i][j]] += 1;
		}
	}
	//=======================================================�������
	//cout<<"=================================135���������================================="<<endl;
	//cout<<"��һ��ǰ��135��Ҷȹ�����: "<<endl;
	//for(int m=0; m<GrayLayerNum; m++)
	//{
		//for(int n=0; n<GrayLayerNum; n++)
		//{
			//cout<<PMatrixLD[m][n]<<" ";
		//}
		//cout<<endl;
	//}
	//cout<<"=================================135���������================================="<<endl;
	//cout<<endl<<endl;
	//=======================================================�������
	
	for(i=0; i<LocalImageHeight; i++)
	{
		delete []NewImage[i];
	}
	delete []NewImage;
	NewImage = NULL;
}






////////////////////////////////////////////////////////////////////
//���ܣ�װ��ͼ��
//������FilePathName��ͼ���ļ�·����
void CGrayShow::loadImage(CString &FilePathName)
{
	int i,j;
	//����Ƿ�Ϊ�գ���Ϊ����Ҫ�ͷ��ڴ�
	if(m_tOriPixelArray !=NULL)
	{
		for(i=0; i<ImageHeight; i++)
		{
			delete []m_tOriPixelArray[i];
			delete []m_tResPixelArray[i];
			delete []ImageArray[i];
		}
		delete []m_tOriPixelArray;
		delete []m_tResPixelArray;
		delete []ImageArray;

		m_tOriPixelArray = NULL;
		m_tResPixelArray = NULL;
		ImageArray       = NULL;
	}

	if(!m_dib->Open(FilePathName))
	{
		printf("·�������ڣ���ͼƬ����");
		return;
	}
	ImageWidth = m_dib->GetWidth();
	ImageHeight = m_dib->GetHeight();
	//----------------------------------------begin yangfan 2016_9_5
	if(ImageWidth<0)
		ImageWidth=-ImageWidth;

	if(ImageHeight<0)
		ImageHeight=-ImageHeight;
	//----------------------------------------end   yangfan 2016_9_5

	BYTE	*colorTable;
	//��Dib.cpp�п��Կ�����colorTableָ��ʵ�ʵ�ͼ������
	//BMP:BITMAPFILEHEADER(�ļ�ͷ) + BITMAPINFOHEADER(��Ϣͷ) + color table(24λ���ͼû�д���) + bits data
	colorTable = (BYTE *)m_dib->m_pDibBits;
	int byteBitCount  = m_dib->GetBiBitCount()/8;

	m_tOriPixelArray  = new RGBQUAD*[ImageHeight];
	m_tResPixelArray  = new RGBQUAD*[ImageHeight];
	ImageArray = new BYTE*[ImageHeight];
	for(int l=0 ; l<ImageHeight; l++)
	{
		m_tOriPixelArray[l] = new RGBQUAD[ImageWidth];
		m_tResPixelArray[l] = new RGBQUAD[ImageWidth];
		ImageArray[l] = new BYTE[ImageWidth];
	}

	int count = 0;
	for(i=ImageHeight-1; i>=0; i--)
	{
		for(j=0; j<ImageWidth; j++)
		{
			m_tOriPixelArray[i][j].rgbBlue =colorTable[count++];
			m_tOriPixelArray[i][j].rgbGreen=colorTable[count++];
			m_tOriPixelArray[i][j].rgbRed  =colorTable[count++];
			m_tOriPixelArray[i][j].rgbReserved = 0;
			m_tResPixelArray[i][j]=m_tOriPixelArray[i][j];

			//ImageArray[i][j]Ϊԭͼ�����ص�RGBֵ���ȡƽ������ֵΪ�Ҷȹ�������ĻҶ�ֵ��Դ
			ImageArray[i][j] = (BYTE)(((unsigned int)m_tOriPixelArray[i][j].rgbBlue
								+(unsigned int)m_tOriPixelArray[i][j].rgbGreen
								+(unsigned int)m_tOriPixelArray[i][j].rgbRed)/3);
			count += byteBitCount-3;
		}
		//ÿɨ����һ��Ҫ��䣬������4�ı������ֽ�
		count += (4-(ImageWidth*byteBitCount)%4)%4;
	}

	//printf("������CGrayShow::loadImage()\n");

	//for(int i=0;i<ImageHeight;++i)
	//{
		//delete []m_tOriPixelArray[i];
		//delete []m_tResPixelArray[i];
	//}
	//delete []m_tOriPixelArray;
	//delete []m_tResPixelArray;

	//m_tOriPixelArray = NULL;
	//m_tResPixelArray = NULL;

	//---------------------------------------------begin yangfan 2016_6_18
	//���TestCase_BMP�ļ����µ�Test1.bmp��ImageArray����
	//cout<<"����loadImage()������õ���ImageArray�������ݣ�"<<endl;
	//for(int m=ImageHeight-1;m>=0;m--)
	//{
		//for(int n=0; n<ImageWidth; n++)
		//{
			//cout<<(int)ImageArray[m][n]<<" ";
		//}
		//cout<<endl<<endl;
	//}
	//---------------------------------------------end   yangfan 2016_6_18

}

////////////////////////////////////////////////////////////////////////////////////
//�������ܣ�������������
//������FeatureEnergy������
//      FeatureEntropy����
//      FeatureInertiaQuadrature�����Ծ�
//      FeatureCorrelation�����
//      FeatureLocalCalm���ֲ�ƽ��
//      pMatrix�����־���
//      dim�����־����ά��
///////////////////////////////////////////////////////////////////////////////////
void CGrayShow::ComputeFeature(double &FeatureEnergy, double &FeatureEntropy, 
							   double &FeatureInertiaQuadrature, double &FeatureCorrelation, 
							   double &FeatureLocalCalm, int** pMatrix, int dim)
{
	int i,j;
	double **pdMatrix;
	pdMatrix = new double*[dim];
	for(i=0; i<dim; i++)
		pdMatrix[i] = new double[dim];

	int total = 0;
	for(i=0; i<dim; i++)
	{
		for(j=0; j<dim; j++)
		{
			total += pMatrix[i][j];
		}
	}

	for(i=0; i<dim; i++)
	{
		for(j=0; j<dim; j++)
		{
			pdMatrix[i][j] = (double)pMatrix[i][j]/(double)total;

			//-------------------------------begin yangfan 2016_9_6
			//��ȫ����0ʱ,������������(0,0)�������λ�ø�����С��ֵ����������Լ����0���������
			if(pdMatrix[i][j] <= 1e-12)
				pdMatrix[i][j] = 1e-12;
			if(pdMatrix[i][j] == 1 )
				pdMatrix[i][j] = 1-(dim*dim-1)*(1e-12);
			//-------------------------------end   yangfan 2016_9_6
		}
	}
	
	FeatureEnergy = 0.0;
	FeatureEntropy = 0.0;
	FeatureInertiaQuadrature = 0.0;
	FeatureLocalCalm = 0.0;


	//�����������ء����Ծء��ֲ�ƽ��
	for(i=0; i<dim; i++)
	{
		for(j=0; j<dim; j++)
		{
			//����
			FeatureEnergy += pdMatrix[i][j]*pdMatrix[i][j];

			//��
			if(pdMatrix[i][j]>1e-12)
			{
				FeatureEntropy -= pdMatrix[i][j]*log(pdMatrix[i][j]);
			}

			//���Ծ�(�Աȶ�)
			FeatureInertiaQuadrature += (double)(i-j)*(double)(i-j)*pdMatrix[i][j];

			//�ֲ�ƽ��(����)
			FeatureLocalCalm += pdMatrix[i][j]/(1+(double)(i-j)*(double)(i-j));
		}
	}

	//����ux
	double ux = 0.0;
	double localtotal = 0.0;
	for(i=0; i<dim; i++)
	{
		localtotal = 0.0;
		for(j=0; j<dim; j++)
		{
			localtotal += pdMatrix[i][j];
		}
		ux += (double)i * localtotal;
	}
	//-------------------------------------begin yangfan 2016_5_29
	//cout<<"begin----"<<"ux="<<ux<<"----";
	//-------------------------------------end   yangfan 2016_5_29

	//����uy
	double uy = 0.0;
	for(j=0; j<dim; j++)
	{
		localtotal = 0.0;
		for(i=0; i<dim; i++)
		{
			localtotal += pdMatrix[i][j];
		}
		uy += (double)j * localtotal;
	}
	//-------------------------------------begin yangfan 2016_5_29
	//cout<<"   ----"<<"uy="<<uy<<"----";
	//-------------------------------------end   yangfan 2016_5_29


	//����sigmax
	double sigmax = 0.0;
	for(i=0; i<dim; i++)
	{
		localtotal = 0.0;
		for(j=0; j<dim; j++)
		{
			localtotal += pdMatrix[i][j];
		}
		sigmax += (double)(i-ux) * (double)(i-ux) * localtotal;
	}
	//-------------------------------------begin yangfan 2016_5_29
	//cout<<"   ----"<<"sigmax="<<sigmax<<"----";
	//-------------------------------------end   yangfan 2016_5_29


	//����sigmay
	double sigmay = 0.0;
	for(j=0; j<dim; j++)
	{
		localtotal = 0.0;
		for(i=0; i<dim; i++)
		{
			localtotal += pdMatrix[i][j];
		}
		sigmay += (double)(j-uy) * (double)(j-uy) * localtotal;
	}
	//-------------------------------------begin yangfan 2016_5_29
	//cout<<"   ----"<<"sigmay="<<sigmay<<"----";
	//-------------------------------------end   yangfan 2016_5_29



	//�������
	FeatureCorrelation = 0.0;
	for(i=0; i<dim; i++)
	{
		for(j=0; j<dim; j++)
		{
			FeatureCorrelation += (double)(i-ux) * (double)(j-uy) * pdMatrix[i][j];
		}
	}
	//-------------------------------------begin yangfan 2016_5_29
	//cout<<"   ----"<<"FeatureCorrelation="<<FeatureCorrelation<<"----@@@@++++end.";
	//-------------------------------------end   yangfan 2016_5_29

	//-------------------------------------begin yangfan 2016_5_29
	FeatureCorrelation /= sigmax;
	//cout<<"    FeatureCorrelation /= sigmax: "<<FeatureCorrelation<<",";
	//-------------------------------------end   yangfan 2016_5_29
	
	//-------------------------------------begin yangfan 2016_5_29
	FeatureCorrelation /= sigmay;
	//cout<<"    FeatureCorrelation /= sigmay: "<<FeatureCorrelation<<".     "<<endl;
	//-------------------------------------end   yangfan 2016_5_29


	//------------------begin yangfan 2016_9_7
	//��־��������
	//int a = (int)FeatureEnergy;
	if((int)FeatureEnergy==0 && (int)FeatureEntropy==0 && (int)FeatureInertiaQuadrature==0 && (int)FeatureLocalCalm==0 && (int)FeatureCorrelation>50)
	{
		FeatureEnergy = 100.0;
		FeatureEntropy = 100.0;
		FeatureInertiaQuadrature = 100.0;
		FeatureLocalCalm = 100.0;
		FeatureCorrelation = 100.0;
	}
	//------------------end   yangfan 2016_9_7

	for(int i=0;i<dim;++i)
	{
		delete []pdMatrix[i];
	}
	delete []pdMatrix;
	pdMatrix = NULL;
}


