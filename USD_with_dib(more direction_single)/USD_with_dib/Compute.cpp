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
	//�ļ����  
    long   hFile   =   0; 
	//�ļ���Ϣ  
    struct _finddata_t fileinfo;
	string p;  
    if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
	{
		do
		{
			if(fileinfo.attrib &  _A_SUBDIR)	//�ж��Ƿ����ļ���
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

	//����3����������������ԭʼ����ֻ������һ�������
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
	vector<double> Feature;		//����ȡ��������vector��ʽ����

	//0��--> PMatrixH
	//�ָ�ͼƬ
	BYTE** srcImgAry = m_grayShow.getImageArray();
	int    srcWth    = m_grayShow.getImageWidth();
	int    srcHgt    = m_grayShow.getImageHeight();

	vector<BYTE**> splitResult = m_grayShow.split(srcImgAry, srcWth, srcHgt, splitWidth);//ÿ���ƶ�splitWidth����λ

	//--------------------------begin yangfan 2016_9_27
	//vector<BYTE**> splitResult = m_grayShow.splitAndMove(srcImgAry,srcWth,srcHgt,splitWidth,movement);//ÿ���ƶ�movement��λ
	//--------------------------end   yangfan 2016_9_27

	vector<BYTE**>::iterator it = splitResult.begin();
	//����ÿһ���ָ���������
	int** srcPMatrixH = NULL;
	for(;it!=splitResult.end();++it)
	{
		srcPMatrixH = m_grayShow.PMatrixH;
		m_grayShow.ComputeMatrix(*it,splitWidth,splitWidth);
		//0��-->PMatrixH
		m_grayShow.ComputeFeature(dEnergy1, dEntropy1, dInertiaQuadrature1, dCorrelation1, dLocalCalm1, srcPMatrixH, 8);
		//m_grayShow.GrayLayerNum);
		/*
		dEnergyH              += dEnergy1;
		dEntropyH             += dEntropy1;
		dInertiaQuadratureH   += dInertiaQuadrature1;
		dCorrelationH         += dCorrelation1;
		dLocalCalmH           += dLocalCalm1;
		//��ֵ�����ݳ�Ա
		m_dEnergy1			  =  dEnergyH;
		m_dEntropy1			  =  dEntropyH;
		m_dInertiaQuadrature1 =  dInertiaQuadratureH;
		m_dCorrelation1		  =  dCorrelationH;
		m_dLocalCalm1		  =  dLocalCalmH;
		*/
		//����ȡ��������vector��ʽ����
		Feature.push_back(dEnergy1);
		Feature.push_back(dEntropy1);
		Feature.push_back(dInertiaQuadrature1);
		Feature.push_back(dLocalCalm1);
		Feature.push_back(dCorrelation1);

		//����
		dEnergy1				= 0.0;
		dEntropy1				= 0.0;
		dInertiaQuadrature1		= 0.0;
		dLocalCalm1				= 0.0;
		dCorrelation1			= 0.0;

#ifdef _ALLDIRECTION
		//45��-->PMatrixRD
		m_grayShow.ComputeFeature(dEnergy2, dEntropy2, dInertiaQuadrature2, dCorrelation2, dLocalCalm2, m_grayShow.PMatrixRD,8);// m_grayShow.GrayLayerNum);
		Feature.push_back(dEnergy2);
		Feature.push_back(dEntropy2);
		Feature.push_back(dInertiaQuadrature2);
		Feature.push_back(dLocalCalm2);
		Feature.push_back(dCorrelation2);

		//90��-->PMatrixV
		m_grayShow.ComputeFeature(dEnergy3, dEntropy3, dInertiaQuadrature3, dCorrelation3, dLocalCalm3, m_grayShow.PMatrixV,8);
		Feature.push_back(dEnergy3);
		Feature.push_back(dEntropy3);
		Feature.push_back(dInertiaQuadrature3);
		Feature.push_back(dLocalCalm3);
		Feature.push_back(dCorrelation3);

		//135��-->PMatrixLD
		m_grayShow.ComputeFeature(dEnergy4, dEntropy4, dInertiaQuadrature4, dCorrelation4, dLocalCalm4, m_grayShow.PMatrixLD,8);
		Feature.push_back(dEnergy4);
		Feature.push_back(dEntropy4);
		Feature.push_back(dInertiaQuadrature4);
		Feature.push_back(dLocalCalm4);
		Feature.push_back(dCorrelation4);

		//����
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
	//0��-->PMatrixH
	m_grayShow.ComputeMatrix(m_grayShow.ImageArray, m_grayShow.ImageWidth, m_grayShow.ImageHeight);
	m_grayShow.ComputeFeature(dEnergy1, dEntropy1, dInertiaQuadrature1, dCorrelation1, dLocalCalm1, m_grayShow.PMatrixH, m_grayShow.GrayLayerNum);
	dEnergyH              += dEnergy1;
	dEntropyH             += dEntropy1;
	dInertiaQuadratureH   += dInertiaQuadrature1;
	dCorrelationH         += dCorrelation1;
	dLocalCalmH           += dLocalCalm1;
	//��ֵ�����ݳ�Ա
	m_dEnergy1			  =  dEnergyH;
	m_dEntropy1			  =  dEntropyH;
	m_dInertiaQuadrature1 =  dInertiaQuadratureH;
	m_dCorrelation1		  =  dCorrelationH;
	m_dLocalCalm1		  =  dLocalCalmH;
	//����ȡ��������vector��ʽ����
	vector<double> Feature;
	Feature.push_back(m_dEnergy1);
	Feature.push_back(m_dEntropy1);
	Feature.push_back(m_dInertiaQuadrature1);
	Feature.push_back(m_dLocalCalm1);
	Feature.push_back(m_dCorrelation1);

	
	//45��-->PMatrixRD
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



	//90��-->PMatrixV
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



	//135��-->PMatrixLD
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
	//ÿ���ƶ�movement��λ
	allPartition = m_grayShow.splitAndMove(srcImgAry,srcWth,srcHgt,splitWidth,movement);

	return allPartition;
}

vector<double> CCompute::computeDestPartFeature(vector<BYTE**> allPartition,int destFeature,int pos,int direction)
{
	//ȡ(direction-45��,direction+45��]���ұ�����
	vector<BYTE**> destPart;
	vector<double> result;
	int m = pos/23;//��ԭʼ�Ļ���һ����22���ָ��
	int n = pos%23;
	
	int splitCount = splitWidth/movement;//ԭ��һ���߳�ΪsplitWidth�ķ������ڷָ���32/2=16��
	int lineCount = (22-1)*(splitCount)+1;//����һ���� (22-1)*16+1 = 337 ���ָ��

	//�ҵ�pos��ӦallPartition�еı��lable
	//int lable = m*20*421+20*(n-1);
	int lable = m*splitCount*lineCount + (n-1)*splitCount;

	//����������Ϊ0,����Ҫ��ȡ(-45��,45��]�����ڵ�2*splitCount������
	if(direction == 0)
	{
		//���pos�ڵ�һ�У����������һ��������ֻ��(-45��,0��]��Χ�з���
		if(m==0 && pos!=22)
		{
			for(int i=splitCount-1;i>=0;--i)
				destPart.push_back(allPartition[lable+lineCount*i+splitCount]);//��(-45��,0��]�ķָ�����vector
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(NULL);//(0��,45��]û�з��飬��NULL��ʾ
			result = computeDestDirection(destPart,destFeature);
		}
		//���pos�����һ�У����������һ��������ֻ��(0��,45��]��Χ�з���
		else if(m==5 && pos!=132)
		{
			for(int i=splitCount-1;i>=0;--i)
				destPart.push_back(NULL);//(-45��,0��]û�з��飬��NULL��ʾ
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[lable-lineCount*i+splitCount]);//��(0��,45��]�ķָ�����vector
			result = computeDestDirection(destPart,destFeature);
		}
		//���pos�����һ�У���û�пɼ���ķ���
		else if(n==22)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//posλ���м䣬(-45��,45��]���пɼ���ķ���
		else//(m!=0 && m!=5 && n!=22)
		{
			for(int i = splitCount-1;i>=-splitCount;--i)
				destPart.push_back(allPartition[lable+lineCount*i+splitCount]);
			result = computeDestDirection(destPart,destFeature);
		}
	}

	//����������Ϊ45,����Ҫ��ȡ(0��,90��]�����ڵ�2*splitCount������
	else if(direction == 45)
	{
		//���pos�ڵ�һ�У����������һ��������û�пɼ���ķ���
		if(m==0 && pos!=22)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//���pos�����һ�У���û�п�ȡ��ֵ(Ϊ��������������90�㷽���Ψһһ������)
		else if(n==22)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//posλ���м䣬��(0��,90��]���з���
		else
		{
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[lable-lineCount*i+splitCount]);
			int newlable = lable - lineCount*splitCount + splitCount;//45�㷽��ķ���
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[newlable-i]);
			result = computeDestDirection(destPart,destFeature);
		}
	}

	//����������Ϊ90,����Ҫ��ȡ(45��,135��]�����ڵ�2*splitCount������
	else if(direction == 90)
	{
		//���pos�ڵ�һ�У���û�пɼ���ķ���
		if(m==0)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//���pos�ڵ�һ��(��������һ��)����ֻ��(45��,90��]�з���
		else if(m!=0 && n==1)
		{
			int newlable = lable - lineCount*splitCount + splitCount;//45�㷽��ķ���
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[newlable-i]);
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//���pos�����һ��(��������һ��)����ֻ��(90��,135��]�з���
		else if(m!=0 && n==22)
		{
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(NULL);
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[lable-lineCount*splitCount-i]);
			result = computeDestDirection(destPart,destFeature);
		}
		//posλ���м䣬��(45��,135��]���з���
		else
		{
			for(int i=1;i<=(splitCount+splitCount);++i)
				destPart.push_back(allPartition[lable-lineCount*splitCount+splitCount-i]);
			result = computeDestDirection(destPart,destFeature);
		}
	}

	//����������Ϊ135,����Ҫ��ȡ(90��,180��]�����ڵ�2*splitCount������
	else if(direction == 135)
	{
		//���pos�ڵ�һ�л��һ�У���û�з������ͳ��
		if(m==0 || n==1)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//pos���м䣬(90�㣬180��]���з���
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

	//����������Ϊ180,����Ҫ��ȡ(135��,-135��]�����ڵ�2*splitCount������
	else if(direction == 180)
	{
		//���pos�ڵ�һ��,��û�з���
		if(n==1)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//pos�ڵ�һ�У���������һ��������ֻ��(180��,-135��]�з���
		else if(n!=1 && m==0)
		{
			for(int i=0;i<splitCount;++i)
				destPart.push_back(NULL);
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[lable-splitCount+i*lineCount]);
			result = computeDestDirection(destPart,destFeature);
		}
		//pos�����һ��(��������һ��),��ֻ��(135��,180��]�з���
		else if(n!=1 && m==5)
		{
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[lable-lineCount*splitCount-splitCount+i*lineCount]);
			for(int i=0;i<splitCount;++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//����else


	}

	//����������Ϊ-135,����Ҫ��ȡ(180��,-90��]�����ڵ�2*splitCount������
	else if(direction == -135)
	{
		//���pos�ڵ�һ�л����һ��,��û�з���
		if(n==1 || m==5)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//���pos���м䣬��(180��,-90��]������ֵ
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

	//����������Ϊ-90,����Ҫ��ȡ(-135��,-45��]�����ڵ�2*splitCount������
	else if(direction == -90)
	{
		//���pos�����һ�У���û��ֵ
		if(m==5)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//���pos�ڵ�һ�У����������һ��������ֻ��(-90��,-45��]�з���
		else if(m!=5 && n==1)
		{
			for(int i=0;i<splitCount;++i)
				destPart.push_back(NULL);
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[lable+lineCount*splitCount+i]);
			result = computeDestDirection(destPart,destFeature);
		}
		//���pos�����һ�У����������һ��������ֻ��(-135��,-90��]�з���
		else if(m!=5 && n==22)
		{
			for(int i=splitCount-1;i>=0;--i)
				destPart.push_back(allPartition[lable+lineCount*splitCount-i]);
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(NULL);
			result = computeDestDirection(destPart,destFeature);
		}
		//���pos���м䣬����2*splitCount������
		else
		{
			for(int i=splitCount-1;i>=0;--i)
				destPart.push_back(allPartition[lable+lineCount*splitCount-i]);
			for(int i=1;i<=splitCount;++i)
				destPart.push_back(allPartition[lable+lineCount*splitCount+i]);
			result = computeDestDirection(destPart,destFeature);
		}
	}

	//����������Ϊ-45,����Ҫ��ȡ(-90��,0��]�����ڵ�2*splitCount������
	else if(direction == -45)
	{
		//���pos�����һ�л����һ�У���û�з���
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
		if(t==0)//ǰ5��������Ҫ����0�㷽��Ĳ���
		{
			if(*it==NULL)//���鲻���ڣ�����-1
				feature.push_back(-1);
			else
			{
				m_grayShow.ComputeMatrix(*it,splitWidth,splitWidth);
				m_grayShow.ComputeFeature(dEnergy1, dEntropy1, dInertiaQuadrature1, dCorrelation1, dLocalCalm1, m_grayShow.PMatrixH, 8);
				//a[0] = dEnergy1;a[1] = dEntropy1;a[2] = dInertiaQuadrature1;a[3] = dCorrelation1;a[4] = dLocalCalm1;
				a[0] = dEnergy1;a[1] = dEntropy1;a[2] = dInertiaQuadrature1;a[3] = dLocalCalm1;a[4] = dCorrelation1;//֮ǰ�õ��������������˳����
				feature.push_back(a[destFeature-1]);
			}
		}
		else if(t==1)//6`10�б�ʾҪ����45�㷽��Ĳ���
		{
			if(*it==NULL)//���鲻���ڣ�����-1
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
		else if(t==2)//11`15�б�ʾҪ����90�㷽��Ĳ���
		{
			if(*it==NULL)//���鲻���ڣ�����-1
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
		else//16`20�б�ʾҪ����135�㷽��Ĳ���
		{
			if(*it==NULL)//���鲻���ڣ�����-1
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
	//���ݵ�2�͵�3�������õ�Ҫ����ľ��η��򣬴����Ӧ��vector
	destRect = getDestRect(pos,direction);
	//���ݵ�1������������������Ŀ�귽��õ�ÿ��������Ҫ
	result = computeDestDirection(destRect,destFeature);

	return result;
}

vector<BYTE**> CCompute::getDestRect(int pos,int direction)
{
	vector<BYTE**> destRect;
	//int m = pos/23;//��ԭʼ�Ļ���һ����22���ָ��
	//int n = pos%23;
	int m = (pos-1)/22;
	if(m<0)
	{
		printf("m:txt�еڶ�����������\n");
		return destRect;
	}
	int n = pos%22;
	if(n<0||n>=22)
	{
		printf("n:txt�еڶ�����������\n");
		return destRect;
	}
	int splitCount = splitWidth/movement;//ԭ��һ���߳�ΪsplitWidth�ķ������ڷָ���32/2=16��
	BYTE** pImg = m_grayShow.ImageArray;

	//����������Ϊ0,����Ҫ��ȡ(-45��,45��]�����ڵ�2*splitCount������
	if(direction == 0)
	{
		//���pos�ڵ�һ�У����������һ��������ֻ��(-45��,0��]��Χ�з���
		if(m==0 && pos!=22)
		{
			for(int i=splitCount-1;i>=-splitCount;--i)
			{
				if(i<0)//(0��,45��]û��ֵ
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
		//���pos�����һ�У����������һ��������ֻ��(0��,45��]��Χ�з���
		else if(m==5 && pos!=132)
		{
			for(int i=splitCount-1;i>=-splitCount;--i)
			{
				if(i>=0)//(-45��,0��]û��ֵ
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
		//���pos�����һ�У���û�пɼ���ķ���
		else if(n==0)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destRect.push_back(NULL);
		}
		//posλ���м䣬(-45��,45��]���пɼ���ķ���
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

	//����������Ϊ45,����Ҫ��ȡ(0��,90��]�����ڵ�2*splitCount������
	else if(direction == 45)
	{
		//���pos�ڵ�һ�У����������һ��������û�пɼ���ķ���
		if(m==0 && pos!=22)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destRect.push_back(NULL);
		}
		//���pos�����һ�У���û�п�ȡ��ֵ(Ϊ��������������90�㷽���Ψһһ������)
		else if(n==0)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destRect.push_back(NULL);
		}
		//posλ���м䣬��(0��,90��]���з���
		else
		{
			//(0��,45��]��Χ�ķ���
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
			//(45��,90��]��Χ�ķ���
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

	//����������Ϊ90,����Ҫ��ȡ(45��,135��]�����ڵ�2*splitCount������
	else if(direction == 90)
	{
		//���pos�ڵ�һ�У���û�пɼ���ķ���
		if(m==0)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destRect.push_back(NULL);
		}
		//���pos�ڵ�һ��(��������һ��)����ֻ��(45��,90��]�з���
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
		//���pos�����һ��(��������һ��)����ֻ��(90��,135��]�з���
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
		//posλ���м䣬��(45��,135��]���з���
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

	//����������Ϊ135,����Ҫ��ȡ(90��,180��]�����ڵ�2*splitCount������
	else if(direction == 135)
	{
		//���pos�ڵ�һ�л��һ�У���û�з������ͳ��
		if(m==0 || n==1)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destRect.push_back(NULL);
		}
		//pos���м䣬(90�㣬180��]���з���
		else
		{
			//(90��,135��]��Χ�ķ���
			if(n==0)//��������һ�У�Ϊ����Ӧ����ĸ�ֵ��ʽ����n��ֵ22
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
			//(135��,180��]��Χ�ķ���
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

	//����������Ϊ180,����Ҫ��ȡ(135��,-135��]�����ڵ�2*splitCount������
	else if(direction == 180)
	{
		//���pos�ڵ�һ��,��û�з���
		if(n==1)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destRect.push_back(NULL);
			
		}
		//pos�ڵ�һ�У���������һ��������ֻ��(180��,-135��]�з���
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
		//pos�����һ��(��������һ��),��ֻ��(135��,180��]�з���
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
		//pos���м�,(135��,-135��]����ȡֵ
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

	//����������Ϊ-135,����Ҫ��ȡ(180��,-90��]�����ڵ�2*splitCount������
	else if(direction == -135)
	{
		//���pos�ڵ�һ�л����һ��,��û�з���
		if(n==1 || m==5)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destRect.push_back(NULL);
		}
		//���pos���м䣬��(180��,-90��]������ֵ
		else
		{
			if(n==0)
				n =22;
			//(180��,-135��]
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
			//(-135��,-90��]
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

	//����������Ϊ-90,����Ҫ��ȡ(-135��,-45��]�����ڵ�2*splitCount������
	else if(direction == -90)
	{
		//���pos�����һ�У���û��ֵ
		if(m==5)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destRect.push_back(NULL);
		}
		//���pos�ڵ�һ�У����������һ��������ֻ��(-90��,-45��]�з���
		else if(m!=5 && n==1)
		{
			//(-135��,-90��]
			for(int i=0;i<splitCount;++i)
				destRect.push_back(NULL);
			//(-90��,-45��]
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
		//���pos�����һ�У����������һ��������ֻ��(-135��,-90��]�з���
		else if(m!=5 && n==22)
		{
			//(-135��,-90��]
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
			//(-90��,-45��]
			for(int i=0;i<splitCount;++i)
				destRect.push_back(NULL);
		}
		//���pos���м䣬����2*splitCount������
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

	//����������Ϊ-45,����Ҫ��ȡ(-90��,0��]�����ڵ�2*splitCount������
	else if(direction == -45)
	{
		//���pos�����һ�л����һ�У���û�з���
		if(m==5 || n==22)
		{
			for(int i=0;i<(splitCount+splitCount);++i)
				destRect.push_back(NULL);
		}
		else
		{
			//(-90��,-45��]
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
			//(-45��,0��]
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
		printf("txt�е�������������\n");

	return destRect;
}