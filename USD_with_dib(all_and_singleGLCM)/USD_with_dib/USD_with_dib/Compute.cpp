//#ifndef _ALLDIRECTION
//#define _ALLDIRECTION
//#endif


#include"Compute.h"

CCompute::CCompute()
{
	splitWidth = 32;
}

CCompute::~CCompute()
{
	//delete m_grayShow;
}


void CCompute::OpenImage( CString& FilePathName )
{
	//m_grayShow=new CGrayShow();
	m_grayShow.loadImage(FilePathName);
}

void CCompute::getAllFiles( string path, vector<string>& files)
{
	//文件句柄  
    long   hFile   =   0; 
	//文件信息  
    struct _finddata_t fileinfo;
	string p;  
    if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
	{
		do
		{
			if(fileinfo.attrib &  _A_SUBDIR)	//判断是否是文件夹
			{
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0) 
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
					getAllFiles( p.assign(path).append("\\").append(fileinfo.name), files ); 
				}
			}
			else
				files.push_back(p.assign(path).append("\\").append(fileinfo.name) ); 

		}while(_findnext(hFile, &fileinfo)  == 0);
	}
}



vector<double> CCompute::ComputeFeature()
{
	double dEnergy1			   = 0.0;
	double dEntropy1		   = 0.0;
	double dInertiaQuadrature1 = 0.0;
	double dLocalCalm1		   = 0.0;
	double dCorrelation1	   = 0.0;

	double dEnergy2			   = 0.0;
	double dEntropy2		   = 0.0;
	double dInertiaQuadrature2 = 0.0;
	double dLocalCalm2		   = 0.0;
	double dCorrelation2	   = 0.0;

	double dEnergy3			   = 0.0;
	double dEntropy3		   = 0.0;
	double dInertiaQuadrature3 = 0.0;
	double dLocalCalm3		   = 0.0;
	double dCorrelation3	   = 0.0;

	double dEnergy4			   = 0.0;
	double dEntropy4		   = 0.0;
	double dInertiaQuadrature4 = 0.0;
	double dLocalCalm4		   = 0.0;
	double dCorrelation4	   = 0.0;

	/*
	double dEnergyH			   = 0.0;
	double dEntropyH		   = 0.0;
	double dInertiaQuadratureH = 0.0;
	double dLocalCalmH		   = 0.0;
	double dCorrelationH	   = 0.0;

	//其余3个方向的纹理参数，原始程序只计算了一个方向的
	double dEnergyLD		   = 0.0;
	double dEntropyLD		   = 0.0;
	double dInertiaQuadratureLD= 0.0;
	double dLocalCalmLD		   = 0.0;
	double dCorrelationLD	   = 0.0;

	double dEnergyRD		   = 0.0;
	double dEntropyRD		   = 0.0;
	double dInertiaQuadratureRD= 0.0;
	double dLocalCalmRD		   = 0.0;
	double dCorrelationRD	   = 0.0;

	double dEnergyV			   = 0.0;
	double dEntropyV		   = 0.0;
	double dInertiaQuadratureV = 0.0;
	double dLocalCalmV		   = 0.0;
	double dCorrelationV	   = 0.0;
	*/
	vector<double> Feature;		//将提取的数据以vector形式返回

	//0°--> PMatrixH
	//分割图片
	BYTE** srcImgAry = m_grayShow.getImageArray();
	int    srcWth    = m_grayShow.getImageWidth();
	int    srcHgt    = m_grayShow.getImageHeight();
	vector<BYTE**> splitResult = m_grayShow.split(srcImgAry, srcWth, srcHgt, splitWidth);
	vector<BYTE**>::iterator it = splitResult.begin();
	//对于每一个分割块计算纹理
	int** srcPMatrixH = NULL;
	for(;it!=splitResult.end();++it)
	{
		srcPMatrixH = m_grayShow.PMatrixH;
		m_grayShow.ComputeMatrix(*it,splitWidth,splitWidth);
		//0°-->PMatrixH
		m_grayShow.ComputeFeature(dEnergy1, dEntropy1, dInertiaQuadrature1, dCorrelation1, dLocalCalm1, srcPMatrixH, 8);
		//m_grayShow.GrayLayerNum);
		/*
		dEnergyH              += dEnergy1;
		dEntropyH             += dEntropy1;
		dInertiaQuadratureH   += dInertiaQuadrature1;
		dCorrelationH         += dCorrelation1;
		dLocalCalmH           += dLocalCalm1;
		//赋值给数据成员
		m_dEnergy1			  =  dEnergyH;
		m_dEntropy1			  =  dEntropyH;
		m_dInertiaQuadrature1 =  dInertiaQuadratureH;
		m_dCorrelation1		  =  dCorrelationH;
		m_dLocalCalm1		  =  dLocalCalmH;
		*/
		//将提取的数据以vector形式返回
		Feature.push_back(dEnergy1);
		Feature.push_back(dEntropy1);
		Feature.push_back(dInertiaQuadrature1);
		Feature.push_back(dLocalCalm1);
		Feature.push_back(dCorrelation1);

		//清零
		dEnergy1				= 0.0;
		dEntropy1				= 0.0;
		dInertiaQuadrature1		= 0.0;
		dLocalCalm1				= 0.0;
		dCorrelation1			= 0.0;

//#ifdef _ALLDIRECTION
		//45°-->PMatrixRD
		m_grayShow.ComputeFeature(dEnergy2, dEntropy2, dInertiaQuadrature2, dCorrelation2, dLocalCalm2, m_grayShow.PMatrixRD,8);// m_grayShow.GrayLayerNum);
		Feature.push_back(dEnergy2);
		Feature.push_back(dEntropy2);
		Feature.push_back(dInertiaQuadrature2);
		Feature.push_back(dLocalCalm2);
		Feature.push_back(dCorrelation2);

		//90°-->PMatrixV
		m_grayShow.ComputeFeature(dEnergy3, dEntropy3, dInertiaQuadrature3, dCorrelation3, dLocalCalm3, m_grayShow.PMatrixV,8);
		Feature.push_back(dEnergy3);
		Feature.push_back(dEntropy3);
		Feature.push_back(dInertiaQuadrature3);
		Feature.push_back(dLocalCalm3);
		Feature.push_back(dCorrelation3);

		//135°-->PMatrixLD
		m_grayShow.ComputeFeature(dEnergy4, dEntropy4, dInertiaQuadrature4, dCorrelation4, dLocalCalm4, m_grayShow.PMatrixLD,8);
		Feature.push_back(dEnergy4);
		Feature.push_back(dEntropy4);
		Feature.push_back(dInertiaQuadrature4);
		Feature.push_back(dLocalCalm4);
		Feature.push_back(dCorrelation4);

		//清零
		//dEnergy1					= 0.0;
		//dEntropy1					= 0.0;
		//dInertiaQuadrature1		= 0.0;
		//dLocalCalm1				= 0.0;
		//dCorrelation1				= 0.0;

		dEnergy2				= 0.0;
		dEntropy2				= 0.0;
		dInertiaQuadrature2		= 0.0;
		dLocalCalm2				= 0.0;
		dCorrelation2			= 0.0;

		dEnergy3				= 0.0;
		dEntropy3				= 0.0;
		dInertiaQuadrature3		= 0.0;
		dLocalCalm3				= 0.0;
		dCorrelation3			= 0.0;

		dEnergy4				= 0.0;
		dEntropy4				= 0.0;
		dInertiaQuadrature4		= 0.0;
		dLocalCalm4				= 0.0;
		dCorrelation4			= 0.0;
//#endif
	}
	//delete m_pgrayShow;

	/*
	//0°-->PMatrixH
	m_grayShow.ComputeMatrix(m_grayShow.ImageArray, m_grayShow.ImageWidth, m_grayShow.ImageHeight);
	m_grayShow.ComputeFeature(dEnergy1, dEntropy1, dInertiaQuadrature1, dCorrelation1, dLocalCalm1, m_grayShow.PMatrixH, m_grayShow.GrayLayerNum);
	dEnergyH              += dEnergy1;
	dEntropyH             += dEntropy1;
	dInertiaQuadratureH   += dInertiaQuadrature1;
	dCorrelationH         += dCorrelation1;
	dLocalCalmH           += dLocalCalm1;
	//赋值给数据成员
	m_dEnergy1			  =  dEnergyH;
	m_dEntropy1			  =  dEntropyH;
	m_dInertiaQuadrature1 =  dInertiaQuadratureH;
	m_dCorrelation1		  =  dCorrelationH;
	m_dLocalCalm1		  =  dLocalCalmH;
	//将提取的数据以vector形式返回
	vector<double> Feature;
	Feature.push_back(m_dEnergy1);
	Feature.push_back(m_dEntropy1);
	Feature.push_back(m_dInertiaQuadrature1);
	Feature.push_back(m_dLocalCalm1);
	Feature.push_back(m_dCorrelation1);

	
	//45°-->PMatrixRD
	m_grayShow.ComputeFeature(dEnergy2, dEntropy2, dInertiaQuadrature2, dCorrelation2, dLocalCalm2, m_grayShow.PMatrixRD, m_grayShow.GrayLayerNum);
	dEnergyRD              += dEnergy2;
	dEntropyRD             += dEntropy2;
	dInertiaQuadratureRD   += dInertiaQuadrature2;
	dCorrelationRD         += dCorrelation2;
	dLocalCalmRD           += dLocalCalm2;

	m_dEnergy2			  =  dEnergyRD;
	m_dEntropy2			  =  dEntropyRD;
	m_dInertiaQuadrature2 =  dInertiaQuadratureRD;
	m_dCorrelation2		  =  dCorrelationRD;
	m_dLocalCalm2		  =  dLocalCalmRD;

	Feature.push_back(m_dEnergy2);
	Feature.push_back(m_dEntropy2);
	Feature.push_back(m_dInertiaQuadrature2);
	Feature.push_back(m_dLocalCalm2);
	Feature.push_back(m_dCorrelation2);



	//90°-->PMatrixV
	m_grayShow.ComputeFeature(dEnergy3, dEntropy3, dInertiaQuadrature3, dCorrelation3, dLocalCalm3, m_grayShow.PMatrixV, m_grayShow.GrayLayerNum);
	dEnergyV              += dEnergy3;
	dEntropyV             += dEntropy3;
	dInertiaQuadratureV   += dInertiaQuadrature3;
	dCorrelationV         += dCorrelation3;
	dLocalCalmV           += dLocalCalm3;

	m_dEnergy3			  =  dEnergyV;
	m_dEntropy3			  =  dEntropyV;
	m_dInertiaQuadrature3  =  dInertiaQuadratureV;
	m_dCorrelation3		  =  dCorrelationV;
	m_dLocalCalm3		  =  dLocalCalmV;

	Feature.push_back(m_dEnergy3);
	Feature.push_back(m_dEntropy3);
	Feature.push_back(m_dInertiaQuadrature3);
	Feature.push_back(m_dLocalCalm3);
	Feature.push_back(m_dCorrelation3);	



	//135°-->PMatrixLD
	m_grayShow.ComputeFeature(dEnergy4, dEntropy4, dInertiaQuadrature4, dCorrelation4, dLocalCalm4, m_grayShow.PMatrixLD, m_grayShow.GrayLayerNum);
	dEnergyLD              += dEnergy4;
	dEntropyLD             += dEntropy4;
	dInertiaQuadratureLD   += dInertiaQuadrature4;
	dCorrelationLD         += dCorrelation4;
	dLocalCalmLD           += dLocalCalm4;

	m_dEnergy4			  =  dEnergyLD;
	m_dEntropy4			  =  dEntropyLD;
	m_dInertiaQuadrature4  =  dInertiaQuadratureLD;
	m_dCorrelation4		  =  dCorrelationLD;
	m_dLocalCalm4		  =  dLocalCalmLD;

	Feature.push_back(m_dEnergy4);
	Feature.push_back(m_dEntropy4);
	Feature.push_back(m_dInertiaQuadrature4);
	Feature.push_back(m_dLocalCalm4);
	Feature.push_back(m_dCorrelation4);	
	*/

			
	return Feature;
}