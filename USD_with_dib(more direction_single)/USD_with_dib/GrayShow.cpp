//////////////////////////////////////////////////////////////////////
// GrayShow.cpp: implementation of the CGrayShow class.
// 灰度共现矩阵算法实现
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
	PMatrixRD = NULL;	//45度方向上的灰度共现矩阵
	PMatrixLD = NULL;	//135度方向上的灰度共现矩阵
	PMatrixV = NULL;	//90度方向上的灰度共现矩阵
	PMatrixH = NULL;	//0度方向上的灰度共现矩阵
	ImageArray = NULL;	//原始的图像数据数组

	m_tOriPixelArray = NULL;//原始位图数据
	m_tResPixelArray = NULL;//参考位图数据

	m_dib = new CDib();

	distance = 3;
	//FilterWindowWidth = 16;
	GrayLayerNum = 8; //初始化设为8个灰度层，可以修改
	//GrayLayerNum = 16;
	//这样共现矩阵均为GrayLayerNum×GrayLayerNum
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

//带参构造函数
/*
CGrayShow::CGrayShow(int _distance)
{
	PMatrixRD = NULL;	//45度方向上的灰度共现矩阵
	PMatrixLD = NULL;	//135度方向上的灰度共现矩阵
	PMatrixV = NULL;	//90度方向上的灰度共现矩阵
	PMatrixH = NULL;	//0度方向上的灰度共现矩阵
	ImageArray = NULL;	//原始的图像数据数组

	m_tOriPixelArray = NULL;//原始位图数据
	m_tResPixelArray = NULL;//参考位图数据
	m_dib = new CDib();

	distance = _distance;
	//FilterWindowWidth = 16;
	GrayLayerNum = 8; //初始化设为8个灰度层，可以修改
	//GrayLayerNum = 16;
	//这样共现矩阵均为GrayLayerNum×GrayLayerNum
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
	//printf("\n执行了CGrayShow的析构函数\n");
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

//图像分割函数,暂时将分割后最后一列不足splitWidth宽的抛弃掉
vector<BYTE**>  CGrayShow::split(BYTE** ImageArray,int ImageWidth,int ImageHeight,int splitWidth)
{
	vector<BYTE**> splitResult;		//返回值
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
	//分割后最后一列宽度不足splitWidth
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

//每次横向移动Move个单位
vector<BYTE**>  CGrayShow::splitAndMove(BYTE** ImageArray,int ImageWidth,int ImageHeight,int splitWidth,int Move)
{
	vector<BYTE**> splitResult;		//返回值
	//计算每行的分割块数
	int widthCount = ImageWidth-splitWidth;
	widthCount = widthCount/Move;
	widthCount += 1;
	//计算每列的分割块数
	int heightCount = ImageHeight-splitWidth;
	heightCount = heightCount/Move;
	heightCount += 1;
	//总分割块数
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
//功能：计算共现矩阵
//参数：LocalImage－用来计算的局部纹理区域图像
//      LocalImageWidth－局部纹理区域宽度
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
			//分成GrayLayerNum个灰度级
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

	//计算0度的灰度共现阵
	for(i=0; i<LocalImageHeight; i++)
	{
		for(j=0; j<LocalImageWidth-distance; j++)
		{
			//由NewImage[i][j]来转换得到
			PMatrixH[(unsigned int)NewImage[i][j]][(unsigned int)NewImage[i][j+distance]] += 1;
			PMatrixH[(unsigned int)NewImage[i][j+distance]][(unsigned int)NewImage[i][j]] += 1;
		}
	}

	//=======================================================输出测试
	//cout<<"=================================0°输出测试================================="<<endl;
	//cout<<"归一化前的0°灰度共现阵: "<<endl;
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
	//cout<<"=================================0°输出测试================================="<<endl;
	//cout<<endl<<endl;
	//=======================================================输出测试


	//计算45度的灰度共现阵
	for(i=distance; i<LocalImageHeight; i++)
	{
		for(j=0; j<LocalImageWidth-distance; j++)
		{
			//由NewImage[i][j]来转换得到
			int newi, newj;
			newi = i-distance;
			newj = j+distance;
			PMatrixRD[(unsigned int)NewImage[i][j]][(unsigned int)NewImage[newi][newj]] += 1;
			PMatrixRD[(unsigned int)NewImage[newi][newj]][(unsigned int)NewImage[i][j]] += 1;
		}
	}
	//=======================================================输出测试
	//cout<<"=================================45°输出测试================================="<<endl;
	//cout<<"归一化前的45°灰度共现阵: "<<endl;
	//for(int m=0; m<GrayLayerNum; m++)
	//{
		//for(int n=0; n<GrayLayerNum; n++)
		//{
			//cout<<PMatrixRD[m][n]<<" ";
		//}
		//cout<<endl;
	//}
	//cout<<"=================================45°输出测试================================="<<endl;
	//cout<<endl<<endl;
	//=======================================================输出测试



	//计算90度的灰度共现阵
	for(j=0; j<LocalImageWidth; j++)
	{
		for(i=0; i<LocalImageHeight-distance; i++)
		{
			//由NewImage[i][j]来转换得到
			PMatrixV[(unsigned int)NewImage[i][j]][(unsigned int)NewImage[i+distance][j]] += 1;
			PMatrixV[(unsigned int)NewImage[i+distance][j]][(unsigned int)NewImage[i][j]] += 1;
		}
	}
	//=======================================================输出测试
	//cout<<"=================================90°输出测试================================="<<endl;
	//cout<<"归一化前的90°灰度共现阵: "<<endl;
	//for(int m=0; m<GrayLayerNum; m++)
	//{
		//for(int n=0; n<GrayLayerNum; n++)
		//{
			//cout<<PMatrixV[m][n]<<" ";
		//}
		//cout<<endl;
	//}
	//cout<<"=================================90°输出测试================================="<<endl;
	//cout<<endl<<endl;
	//=======================================================输出测试




	//计算135度的灰度共现阵
	for(i=0; i<LocalImageHeight-distance; i++)
	{
		for(j=0; j<LocalImageWidth-distance; j++)
		{
			//由NewImage[i][j]来转换得到
			int newi, newj;
			newi = i+distance;
			newj = j+distance;
			PMatrixLD[(unsigned int)NewImage[i][j]][(unsigned int)NewImage[newi][newj]] += 1;
			PMatrixLD[(unsigned int)NewImage[newi][newj]][(unsigned int)NewImage[i][j]] += 1;
		}
	}
	//=======================================================输出测试
	//cout<<"=================================135°输出测试================================="<<endl;
	//cout<<"归一化前的135°灰度共现阵: "<<endl;
	//for(int m=0; m<GrayLayerNum; m++)
	//{
		//for(int n=0; n<GrayLayerNum; n++)
		//{
			//cout<<PMatrixLD[m][n]<<" ";
		//}
		//cout<<endl;
	//}
	//cout<<"=================================135°输出测试================================="<<endl;
	//cout<<endl<<endl;
	//=======================================================输出测试
	
	for(i=0; i<LocalImageHeight; i++)
	{
		delete []NewImage[i];
	}
	delete []NewImage;
	NewImage = NULL;
}






////////////////////////////////////////////////////////////////////
//功能：装载图像
//参数：FilePathName－图像文件路径名
void CGrayShow::loadImage(CString &FilePathName)
{
	int i,j;
	//检测是否为空，不为空则要释放内存
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
		printf("路径不存在，打开图片出错！");
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
	//从Dib.cpp中可以看出，colorTable指向实际的图像数据
	//BMP:BITMAPFILEHEADER(文件头) + BITMAPINFOHEADER(信息头) + color table(24位真彩图没有此项) + bits data
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

			//ImageArray[i][j]为原图像像素的RGB值求和取平均，此值为灰度共生矩阵的灰度值来源
			ImageArray[i][j] = (BYTE)(((unsigned int)m_tOriPixelArray[i][j].rgbBlue
								+(unsigned int)m_tOriPixelArray[i][j].rgbGreen
								+(unsigned int)m_tOriPixelArray[i][j].rgbRed)/3);
			count += byteBitCount-3;
		}
		//每扫描完一行要填充，必须是4的倍数个字节
		count += (4-(ImageWidth*byteBitCount)%4)%4;
	}

	//printf("调用了CGrayShow::loadImage()\n");

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
	//输出TestCase_BMP文件夹下的Test1.bmp的ImageArray数据
	//cout<<"调用loadImage()函数后得到的ImageArray数组数据："<<endl;
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
//函数功能：计算纹理特征
//参数：FeatureEnergy－能量
//      FeatureEntropy－熵
//      FeatureInertiaQuadrature－惯性矩
//      FeatureCorrelation－相关
//      FeatureLocalCalm－局部平稳
//      pMatrix－共现矩阵
//      dim－共现矩阵的维数
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
			//当全部是0时,进行修正，给(0,0)外其余的位置赋个很小的值，避免相关性计算除0的情况产生
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


	//计算能量、熵、惯性矩、局部平稳
	for(i=0; i<dim; i++)
	{
		for(j=0; j<dim; j++)
		{
			//能量
			FeatureEnergy += pdMatrix[i][j]*pdMatrix[i][j];

			//熵
			if(pdMatrix[i][j]>1e-12)
			{
				FeatureEntropy -= pdMatrix[i][j]*log(pdMatrix[i][j]);
			}

			//惯性矩(对比度)
			FeatureInertiaQuadrature += (double)(i-j)*(double)(i-j)*pdMatrix[i][j];

			//局部平稳(逆差矩)
			FeatureLocalCalm += pdMatrix[i][j]/(1+(double)(i-j)*(double)(i-j));
		}
	}

	//计算ux
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

	//计算uy
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


	//计算sigmax
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


	//计算sigmay
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



	//计算相关
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
	//标志除零数据
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


