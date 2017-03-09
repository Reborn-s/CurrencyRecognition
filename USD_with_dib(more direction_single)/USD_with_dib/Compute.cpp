#include"Compute.h"

CCompute::CCompute()
{
	splitWidth = 32;
	movement = 2;
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

	vector<BYTE**> splitResult = m_grayShow.split(srcImgAry, srcWth, srcHgt, splitWidth);//每次移动splitWidth个单位

	//--------------------------begin yangfan 2016_9_27
	//vector<BYTE**> splitResult = m_grayShow.splitAndMove(srcImgAry,srcWth,srcHgt,splitWidth,movement);//每次移动movement单位
	//--------------------------end   yangfan 2016_9_27

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

#ifdef _ALLDIRECTION
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
#endif
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

vector<BYTE**> CCompute::getAllPartition()
{
	vector<BYTE**> allPartition;
	BYTE** srcImgAry = m_grayShow.getImageArray();
	int    srcWth    = m_grayShow.getImageWidth();
	int    srcHgt    = m_grayShow.getImageHeight();
	//每次移动movement单位
	allPartition = m_grayShow.splitAndMove(srcImgAry,srcWth,srcHgt,splitWidth,movement);

	return allPartition;
}

vector<double> CCompute::computeDestPartFeature(vector<BYTE**> allPartition,int destFeature,int pos,int direction)
{
	//取(direction-45°,direction+45°]左开右闭区间
	vector<BYTE**> destPart;
	vector<double> result;
	int m = pos/23;//按原始的划分一行有22个分割块
	int n = pos%23;
	
	int splitCount = splitWidth/movement;//原来一个边长为splitWidth的方框现在分割了32/2=16块
	int lineCount = (22-1)*(splitCount)+1;//现在一行有 (22-1)*16+1 = 337 个分割块

	//找到pos对应allPartition中的编号lable
	//int lable = m*20*421+20*(n-1);
	int lable = m*splitCount*lineCount + (n-1)*splitCount;

	//第三个参数为0,表明要获取(-45°,45°]方向内的2*splitCount个方块
	if(direction == 0)
	{
		//如果pos在第一行（不包含最后一个），则只有(-45°,0°]范围有方块
		if(m==0 && pos!=22)
		{
			for(int i=splitCount-1;i>=0;--i)
				destPart.push_back(allPartition[lable+lineCount*i+splitCount]);//将(-45°,0°]的分割块存入vector
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(NULL);//(0°,45°]没有方块，用NULL表示
			result = computeDestDirection(destPart,destFeature);
		}
		//如果pos在最后一行（不包含最后一个），则只有(0°,45°]范围有方块
		else if(m==5 && pos!=132)
		{
			for(int i=splitCount-1;i>=0;--i)
				destPart.push_back(NULL);//(-45°,0°]没有方块，用NULL表示
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[lable-lineCount*i+splitCount]);//将(0°,45°]的分割块存入vector
			result = computeDestDirection(destPart,destFeature);
		}
		//如果pos在最后一列，则没有可计算的方块
		else if(n==22)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//pos位于中间，(-45°,45°]都有可计算的方块
		else//(m!=0 && m!=5 && n!=22)
		{
			for(int i = splitCount-1;i>=-splitCount;--i)
				destPart.push_back(allPartition[lable+lineCount*i+splitCount]);
			result = computeDestDirection(destPart,destFeature);
		}
	}

	//第三个参数为45,表明要获取(0°,90°]方向内的2*splitCount个方块
	else if(direction == 45)
	{
		//如果pos在第一行（不包含最后一个），则没有可计算的方块
		if(m==0 && pos!=22)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//如果pos在最后一列，则没有可取的值(为简便起见，不计算90°方向的唯一一个方块)
		else if(n==22)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//pos位于中间，则(0°,90°]都有方块
		else
		{
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[lable-lineCount*i+splitCount]);
			int newlable = lable - lineCount*splitCount + splitCount;//45°方向的方块
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[newlable-i]);
			result = computeDestDirection(destPart,destFeature);
		}
	}

	//第三个参数为90,表明要获取(45°,135°]方向内的2*splitCount个方块
	else if(direction == 90)
	{
		//如果pos在第一行，则没有可计算的方块
		if(m==0)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//如果pos在第一列(不包括第一行)，则只有(45°,90°]有方块
		else if(m!=0 && n==1)
		{
			int newlable = lable - lineCount*splitCount + splitCount;//45°方向的方块
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[newlable-i]);
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//如果pos在最后一列(不包括第一行)，则只有(90°,135°]有方块
		else if(m!=0 && n==22)
		{
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(NULL);
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[lable-lineCount*splitCount-i]);
			result = computeDestDirection(destPart,destFeature);
		}
		//pos位于中间，则(45°,135°]都有方块
		else
		{
			for(int i=1;i<=(splitCount+splitCount);++i)
				destPart.push_back(allPartition[lable-lineCount*splitCount+splitCount-i]);
			result = computeDestDirection(destPart,destFeature);
		}
	}

	//第三个参数为135,表明要获取(90°,180°]方向内的2*splitCount个方块
	else if(direction == 135)
	{
		//如果pos在第一行或第一列，则没有方框可以统计
		if(m==0 || n==1)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//pos在中间，(90°，180°]都有方块
		else
		{
			int newlable = lable-lineCount*splitCount-splitCount;
			for(int i=splitCount-1;i>=0;--i)
				destPart.push_back(allPartition[newlable+i]);
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[newlable+i*lineCount]);
			result = computeDestDirection(destPart,destFeature);
		}
	}

	//第三个参数为180,表明要获取(135°,-135°]方向内的2*splitCount个方块
	else if(direction == 180)
	{
		//如果pos在第一列,则没有方框
		if(n==1)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//pos在第一行（不包含第一个），则只有(180°,-135°]有方框
		else if(n!=1 && m==0)
		{
			for(int i=0;i<splitCount;++i)
				destPart.push_back(NULL);
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[lable-splitCount+i*lineCount]);
			result = computeDestDirection(destPart,destFeature);
		}
		//pos在最后一行(不包括第一个),则只有(135°,180°]有方框
		else if(n!=1 && m==5)
		{
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[lable-lineCount*splitCount-splitCount+i*lineCount]);
			for(int i=0;i<splitCount;++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//掉了else


	}

	//第三个参数为-135,表明要获取(180°,-90°]方向内的2*splitCount个方块
	else if(direction == -135)
	{
		//如果pos在第一列或最后一行,则没有方框
		if(n==1 || m==5)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//如果pos在中间，则(180°,-90°]方向都有值
		else
		{
			int newlable = lable+lineCount*splitCount-splitCount;
			for(int i=splitCount-1;i>=0;--i)
				destPart.push_back(allPartition[newlable-i*lineCount]);
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[newlable+i]);
			result = computeDestDirection(destPart,destFeature);
		}
	}

	//第三个参数为-90,表明要获取(-135°,-45°]方向内的2*splitCount个方块
	else if(direction == -90)
	{
		//如果pos在最后一行，则没有值
		if(m==5)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//如果pos在第一列（不包含最后一个），则只有(-90°,-45°]有方框
		else if(m!=5 && n==1)
		{
			for(int i=0;i<splitCount;++i)
				destPart.push_back(NULL);
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[lable+lineCount*splitCount+i]);
			result = computeDestDirection(destPart,destFeature);
		}
		//如果pos在最后一列（不包含最后一个），则只有(-135°,-90°]有方框
		else if(m!=5 && n==22)
		{
			for(int i=splitCount-1;i>=0;--i)
				destPart.push_back(allPartition[lable+lineCount*splitCount-i]);
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//如果pos在中间，则有2*splitCount个方框
		else
		{
			for(int i=splitCount-1;i>=0;--i)
				destPart.push_back(allPartition[lable+lineCount*splitCount-i]);
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[lable+lineCount*splitCount+i]);
			result = computeDestDirection(destPart,destFeature);
		}
	}

	//第三个参数为-45,表明要获取(-90°,0°]方向内的2*splitCount个方块
	else if(direction == -45)
	{
		//如果pos在最后一列或最后一行，则没有方框
		if(m==5 || n==22)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		else
		{
			int newlable = lable+lineCount*splitCount+splitCount;
			for(int i=splitCount-1;i>=0;--i)
				destPart.push_back(allPartition[newlable-i]);
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[newlable-i*lineCount]);
			result = computeDestDirection(destPart,destFeature);
		}
	}

	return result;
}

vector<double> CCompute::computeDestDirection(vector<BYTE**> destPartition,int destFeature)
{
	vector<double> feature;
	double dEnergy1			   = 0.0;
	double dEntropy1		   = 0.0;
	double dInertiaQuadrature1 = 0.0;
	double dLocalCalm1		   = 0.0;
	double dCorrelation1	   = 0.0;

	//double dEnergy2			   = 0.0;
	//double dEntropy2		   = 0.0;
	//double dInertiaQuadrature2 = 0.0;
	//double dLocalCalm2		   = 0.0;
	//double dCorrelation2	   = 0.0;

	//double dEnergy3			   = 0.0;
	//double dEntropy3		   = 0.0;
	//double dInertiaQuadrature3 = 0.0;
	//double dLocalCalm3		   = 0.0;
	//double dCorrelation3	   = 0.0;

	//double dEnergy4			   = 0.0;
	//double dEntropy4		   = 0.0;
	//double dInertiaQuadrature4 = 0.0;
	//double dLocalCalm4		   = 0.0;
	//double dCorrelation4	   = 0.0;

	vector<BYTE**>::iterator it;
	double a[5]={0,0,0,0,0};
	for(it=destPartition.begin();it!=destPartition.end();++it)
	{
		int t = (destFeature-1)/5;
		if(t==0)//前5个数代表要计算0°方向的参数
		{
			if(*it==NULL)//方块不存在，返回-1
				feature.push_back(-1);
			else
			{
				m_grayShow.ComputeMatrix(*it,splitWidth,splitWidth);
				m_grayShow.ComputeFeature(dEnergy1, dEntropy1, dInertiaQuadrature1, dCorrelation1, dLocalCalm1, m_grayShow.PMatrixH, 8);
				//a[0] = dEnergy1;a[1] = dEntropy1;a[2] = dInertiaQuadrature1;a[3] = dCorrelation1;a[4] = dLocalCalm1;
				a[0] = dEnergy1;a[1] = dEntropy1;a[2] = dInertiaQuadrature1;a[3] = dLocalCalm1;a[4] = dCorrelation1;//之前得到的最后两个参数顺序反了
				feature.push_back(a[destFeature-1]);
			}
		}
		else if(t==1)//6`10行表示要计算45°方向的参数
		{
			if(*it==NULL)//方块不存在，返回-1
				feature.push_back(-1);
			else
			{
				m_grayShow.ComputeMatrix(*it,splitWidth,splitWidth);
				m_grayShow.ComputeFeature(dEnergy1, dEntropy1, dInertiaQuadrature1, dCorrelation1, dLocalCalm1, m_grayShow.PMatrixRD, 8);
				//a[0] = dEnergy1;a[1] = dEntropy1;a[2] = dInertiaQuadrature1;a[3] = dCorrelation1;a[4] = dLocalCalm1;
				a[0] = dEnergy1;a[1] = dEntropy1;a[2] = dInertiaQuadrature1;a[3] = dLocalCalm1;a[4] = dCorrelation1;
				feature.push_back(a[destFeature-6]);
			}
		}
		else if(t==2)//11`15行表示要计算90°方向的参数
		{
			if(*it==NULL)//方块不存在，返回-1
				feature.push_back(-1);
			else
			{
				m_grayShow.ComputeMatrix(*it,splitWidth,splitWidth);
				m_grayShow.ComputeFeature(dEnergy1, dEntropy1, dInertiaQuadrature1, dCorrelation1, dLocalCalm1, m_grayShow.PMatrixV, 8);
				//a[0] = dEnergy1;a[1] = dEntropy1;a[2] = dInertiaQuadrature1;a[3] = dCorrelation1;a[4] = dLocalCalm1;
				a[0] = dEnergy1;a[1] = dEntropy1;a[2] = dInertiaQuadrature1;a[3] = dLocalCalm1;a[4] = dCorrelation1;
				feature.push_back(a[destFeature-11]);
			}
		}
		else//16`20行表示要计算135°方向的参数
		{
			if(*it==NULL)//方块不存在，返回-1
				feature.push_back(-1);
			else
			{
				m_grayShow.ComputeMatrix(*it,splitWidth,splitWidth);
				m_grayShow.ComputeFeature(dEnergy1, dEntropy1, dInertiaQuadrature1, dCorrelation1, dLocalCalm1, m_grayShow.PMatrixLD, 8);
				//a[0] = dEnergy1;a[1] = dEntropy1;a[2] = dInertiaQuadrature1;a[3] = dCorrelation1;a[4] = dLocalCalm1;
				a[0] = dEnergy1;a[1] = dEntropy1;a[2] = dInertiaQuadrature1;a[3] = dLocalCalm1;a[4] = dCorrelation1;
				feature.push_back(a[destFeature-16]);
			}
		}
	}
	return feature;
}

vector<double> CCompute::computeResult(int destFeature,int pos,int direction)
{
	vector<double> result;
	vector<BYTE**> destRect;
	//根据第2和第3个参数得到要计算的矩形方框，存入对应的vector
	destRect = getDestRect(pos,direction);
	//根据第1个参数和上面计算出的目标方框得到每个方框需要
	result = computeDestDirection(destRect,destFeature);

	return result;
}

vector<BYTE**> CCompute::getDestRect(int pos,int direction)
{
	vector<BYTE**> destRect;
	//int m = pos/23;//按原始的划分一行有22个分割块
	//int n = pos%23;
	int m = (pos-1)/22;
	if(m<0)
	{
		printf("m:txt中第二个参数错误\n");
		return destRect;
	}
	int n = pos%22;
	if(n<0||n>=22)
	{
		printf("n:txt中第二个参数错误\n");
		return destRect;
	}
	int splitCount = splitWidth/movement;//原来一个边长为splitWidth的方框现在分割了32/2=16块
	BYTE** pImg = m_grayShow.ImageArray;

	//第三个参数为0,表明要获取(-45°,45°]方向内的2*splitCount个方块
	if(direction == 0)
	{
		//如果pos在第一行（不包含最后一个），则只有(-45°,0°]范围有方块
		if(m==0 && pos!=22)
		{
			for(int i=splitCount-1;i>=-splitCount;--i)
			{
				if(i<0)//(0°,45°]没有值
					destRect.push_back(NULL);
				else
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
							splitImage[p][q] = pImg[p+m*splitWidth+i*movement][q+n*splitWidth];
						}
					}
					destRect.push_back(splitImage);
				}
			}
		}
		//如果pos在最后一行（不包含最后一个），则只有(0°,45°]范围有方块
		else if(m==5 && pos!=132)
		{
			for(int i=splitCount-1;i>=-splitCount;--i)
			{
				if(i>=0)//(-45°,0°]没有值
					destRect.push_back(NULL);
				else
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
							splitImage[p][q] = pImg[p+m*splitWidth+i*movement][q+n*splitWidth];
						}
					}
					destRect.push_back(splitImage);
				}
			}
		}
		//如果pos在最后一列，则没有可计算的方块
		else if(n==0)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destRect.push_back(NULL);
		}
		//pos位于中间，(-45°,45°]都有可计算的方块
		else
		{
			for(int i=splitCount-1;i>=-splitCount;--i)
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
						splitImage[p][q] = pImg[p+m*splitWidth+i*movement][q+n*splitWidth];
					}
				}
				destRect.push_back(splitImage);
			}
		}
	}

	//第三个参数为45,表明要获取(0°,90°]方向内的2*splitCount个方块
	else if(direction == 45)
	{
		//如果pos在第一行（不包含最后一个），则没有可计算的方块
		if(m==0 && pos!=22)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destRect.push_back(NULL);
		}
		//如果pos在最后一列，则没有可取的值(为简便起见，不计算90°方向的唯一一个方块)
		else if(n==0)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destRect.push_back(NULL);
		}
		//pos位于中间，则(0°,90°]都有方块
		else
		{
			//(0°,45°]范围的方框
			for(int i=splitCount-1;i>=0;--i)
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
						splitImage[p][q] = pImg[p+(m-1)*splitWidth+i*movement][q+n*splitWidth];
						//printf("%d ",(int)splitImage[p][q]);
					}
					//printf("\n");
				}
				destRect.push_back(splitImage);
			}
			//(45°,90°]范围的方块
			for(int i=1;i<=splitCount;++i)
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
						splitImage[p][q] = pImg[p+(m-1)*splitWidth][q+n*splitWidth-i*movement];
					}
				}
				destRect.push_back(splitImage);
			}
		}
	}

	//第三个参数为90,表明要获取(45°,135°]方向内的2*splitCount个方块
	else if(direction == 90)
	{
		//如果pos在第一行，则没有可计算的方块
		if(m==0)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destRect.push_back(NULL);
		}
		//如果pos在第一列(不包括第一行)，则只有(45°,90°]有方块
		else if(m!=0 && n==1)
		{
			for(int i=splitCount-1;i>=-splitCount;--i)
			{
				if(i<0)
					destRect.push_back(NULL);
				else
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
							splitImage[p][q] = pImg[p+(m-1)*splitWidth][q+(n-1)*splitWidth+i*movement];
						}
					}
					destRect.push_back(splitImage);
				}
			}
		}
		//如果pos在最后一列(不包括第一行)，则只有(90°,135°]有方块
		else if(m!=0 && n==0)
		{
			for(int i=splitCount-1;i>=-splitCount;--i)
			{
				if(i>=0)
					destRect.push_back(NULL);
				else
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
							splitImage[p][q] = pImg[p+(m-1)*splitWidth][q+(n-1)*splitWidth+i*movement];
						}
					}
					destRect.push_back(splitImage);
				}
			}
		}
		//pos位于中间，则(45°,135°]都有方块
		else
		{
			for(int i=splitCount-1;i>=-splitCount;--i)
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
						splitImage[p][q] = pImg[p+(m-1)*splitWidth][q+(n-1)*splitWidth+i*movement];
					}
				}
				destRect.push_back(splitImage);
			}
		}
	}

	//第三个参数为135,表明要获取(90°,180°]方向内的2*splitCount个方块
	else if(direction == 135)
	{
		//如果pos在第一行或第一列，则没有方框可以统计
		if(m==0 || n==1)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destRect.push_back(NULL);
		}
		//pos在中间，(90°，180°]都有方块
		else
		{
			//(90°,135°]范围的方框
			if(n==0)//如果在最后一列，为了适应下面的赋值公式，将n赋值22
				n = 22;
			for(int i=splitCount-1;i>=0;--i)
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
						splitImage[p][q] = pImg[p+(m-1)*splitWidth][q+(n-2)*splitWidth+i*movement];
					}
				}
				destRect.push_back(splitImage);
			}
			//(135°,180°]范围的方块
			for(int i=1;i<=splitCount;++i)
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
						splitImage[p][q] = pImg[p+(m-1)*splitWidth+i*movement][q+(n-2)*splitWidth];
					}
				}
				destRect.push_back(splitImage);
			}
		}
	}

	//第三个参数为180,表明要获取(135°,-135°]方向内的2*splitCount个方块
	else if(direction == 180)
	{
		//如果pos在第一列,则没有方框
		if(n==1)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destRect.push_back(NULL);
			
		}
		//pos在第一行（不包含第一个），则只有(180°,-135°]有方框
		else if(n!=1 && m==0)
		{
			if(n==0)
				n = 22;
			for(int i=-splitCount+1;i<=splitCount;++i)
			{
				if(i<=0)
					destRect.push_back(NULL);
				else
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
							splitImage[p][q] = pImg[p+m*splitWidth+i*movement][q+(n-2)*splitWidth];
						}
					}
					destRect.push_back(splitImage);
				}
			}
		}
		//pos在最后一行(不包括第一个),则只有(135°,180°]有方框
		else if(n!=1 && m==5)
		{
			if(n==0)
				n = 22;
			for(int i=-splitCount+1;i<=splitCount;++i)
			{
				if(i>0)
					destRect.push_back(NULL);
				else
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
							if(pImg)
								splitImage[p][q] = pImg[p+m*splitWidth+i*movement][q+(n-2)*splitWidth];
							else
								splitImage = NULL;
						}
					}
					destRect.push_back(splitImage);
				}
			}
		}
		//pos在中间,(135°,-135°]都有取值
		else
		{
			if(n==0)
				n = 22;
			for(int i=-splitCount+1;i<=splitCount;++i)
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
						splitImage[p][q] = pImg[p+m*splitWidth+i*movement][q+(n-2)*splitWidth];
					}
				}
				destRect.push_back(splitImage);
			}
		}	
	}

	//第三个参数为-135,表明要获取(180°,-90°]方向内的2*splitCount个方块
	else if(direction == -135)
	{
		//如果pos在第一列或最后一行,则没有方框
		if(n==1 || m==5)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destRect.push_back(NULL);
		}
		//如果pos在中间，则(180°,-90°]方向都有值
		else
		{
			if(n==0)
				n =22;
			//(180°,-135°]
			for(int i=splitCount-1;i>=0;--i)
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
						splitImage[p][q] = pImg[p+(m+1)*splitWidth-i*movement][q+(n-2)*splitWidth];
					}
				}
				destRect.push_back(splitImage);
			}
			//(-135°,-90°]
			for(int i=1;i<=splitCount;++i)
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
						splitImage[p][q] = pImg[p+(m+1)*splitWidth][q+(n-2)*splitWidth+i*movement];
					}
				}
				destRect.push_back(splitImage);
			}
		}
	}

	//第三个参数为-90,表明要获取(-135°,-45°]方向内的2*splitCount个方块
	else if(direction == -90)
	{
		//如果pos在最后一行，则没有值
		if(m==5)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destRect.push_back(NULL);
		}
		//如果pos在第一列（不包含最后一个），则只有(-90°,-45°]有方框
		else if(m!=5 && n==1)
		{
			//(-135°,-90°]
			for(int i=0;i<splitCount;++i)
				destRect.push_back(NULL);
			//(-90°,-45°]
			for(int i=1;i<=splitCount;++i)
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
						splitImage[p][q] = pImg[p+(m+1)*splitWidth][q+(n-1)*splitWidth+i*movement];
					}
				}
				destRect.push_back(splitImage);
			}
		}
		//如果pos在最后一列（不包含最后一个），则只有(-135°,-90°]有方框
		else if(m!=5 && n==22)
		{
			//(-135°,-90°]
			for(int i=splitCount-1;i<=0;--i)
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
						splitImage[p][q] = pImg[p+(m+1)*splitWidth][q+(n-1)*splitWidth-i*movement];
					}
				}
				destRect.push_back(splitImage);
			}
			//(-90°,-45°]
			for(int i=0;i<splitCount;++i)
				destRect.push_back(NULL);
		}
		//如果pos在中间，则有2*splitCount个方框
		else
		{
			for(int i=-splitCount+1;i<=splitCount;++i)
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
						splitImage[p][q] = pImg[p+(m+1)*splitWidth][q+(n-1)*splitWidth+i*movement];
					}
				}
				destRect.push_back(splitImage);
			}
		}
	}

	//第三个参数为-45,表明要获取(-90°,0°]方向内的2*splitCount个方块
	else if(direction == -45)
	{
		//如果pos在最后一列或最后一行，则没有方框
		if(m==5 || n==22)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destRect.push_back(NULL);
		}
		else
		{
			//(-90°,-45°]
			for(int i=splitCount-1;i>=0;--i)
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
						splitImage[p][q] = pImg[p+(m+1)*splitWidth][q+n*splitWidth-i*movement];
					}
				}
				destRect.push_back(splitImage);
			}
			//(-45°,0°]
			for(int i=1;i<=splitCount;++i)
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
						splitImage[p][q] = pImg[p+(m+1)*splitWidth-i*movement][q+n*splitWidth];
					}
				}
				destRect.push_back(splitImage);
			}
		}
	}

	else
		printf("txt中第三个参数错误\n");

	return destRect;
}