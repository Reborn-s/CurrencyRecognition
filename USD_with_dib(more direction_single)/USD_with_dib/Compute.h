#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GrayShow.h"
//#include<vector>

#include<io.h>



class CCompute
{
public:
	CGrayShow m_grayShow; //����һ���Ҷȹ��־�����

	/*
	double	m_dCorrelation1;
	double	m_dEnergy1;
	double	m_dEntropy1;
	double	m_dInertiaQuadrature1;
	double	m_dLocalCalm1;

	double	m_dCorrelation2;
	double	m_dEnergy2;
	double	m_dEntropy2;
	double	m_dInertiaQuadrature2;
	double	m_dLocalCalm2;

	double	m_dCorrelation3;
	double	m_dEnergy3;
	double	m_dEntropy3;
	double	m_dInertiaQuadrature3;
	double	m_dLocalCalm3;

	double	m_dCorrelation4;
	double	m_dEnergy4;
	double	m_dEntropy4;
	double	m_dInertiaQuadrature4;
	double	m_dLocalCalm4;
	*/

	CCompute();
	~CCompute();

	int splitWidth;	//�ָ���

	int movement;   //ÿ���ƶ��ĵ�λ����

	//��BMPͼƬ����ȡͼƬԭʼ����
	void OpenImage(CString& FilePathName);

	//�����������������vector��ʽ����
	vector<double> ComputeFeature();

	//��ȡָ��·���µ������ļ��к��ļ���
	void getAllFiles( string path, vector<string>& files);


	//-----------------------begin yangfan 2016_9_27
	vector<BYTE**> getAllPartition();
	//-----------------------end   yangfan 2016_9_27

	//-----------------------begin yangfan 2016_9_29
	vector<double> computeDestPartFeature(vector<BYTE**> allPartition,int destFeature,int pos,int direction);
	vector<double> computeDestDirection(vector<BYTE**> destPartition,int destFeature);
	//-----------------------end   yangfan 2016_9_29
	
	//-----------------------begin yangfan 2016_10_09
	vector<double> computeResult(int destFeature,int pos,int direction);
	vector<BYTE**> getDestRect(int pos,int direction);
	//-----------------------end   yangfan 2016_10_09
};