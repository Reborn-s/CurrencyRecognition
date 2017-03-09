//#ifndef _SINGLETXT
//#define _SINGLETXT
//#endif

#define _ALLTEXT


#include"Compute.h"
#include<list>
#include<string>
#include<locale>

CString file_FN[9] = {"all_1FN.txt","all_2FN.txt","all_5-2FN.txt","all_5-3FN.txt","all_10FN.txt","all_20FN.txt","all_50FN.txt","all_100-2FN.txt","all_100-3FN.txt"};
string filepathPrefix_FN[9] = {"F:\\Project\\USD\\split_later\\1\\1FN","F:\\Project\\USD\\split_later\\2\\2FN","F:\\Project\\USD\\split_later\\5\\5-2\\5FN",
	"F:\\Project\\USD\\split_later\\5\\5-3\\5FN","F:\\Project\\USD\\split_later\\10\\10FN","F:\\Project\\USD\\split_later\\20\\20FN",
	"F:\\Project\\USD\\split_later\\50\\50FN","F:\\Project\\USD\\split_later\\100\\100-2\\100FN","F:\\Project\\USD\\split_later\\100\\100-3\\100FN"};

CString file_FY[9] = {"all_1FY.txt","all_2FY.txt","all_5-2FY.txt","all_5-3FY.txt","all_10FY.txt","all_20FY.txt","all_50FY.txt","all_100-2FY.txt","all_100-3FY.txt"};
string filepathPrefix_FY[9] = {"F:\\Project\\USD\\split_later\\1\\1FY","F:\\Project\\USD\\split_later\\2\\2FY","F:\\Project\\USD\\split_later\\5\\5-2\\5FY",
	"F:\\Project\\USD\\split_later\\5\\5-3\\5FY","F:\\Project\\USD\\split_later\\10\\10FY","F:\\Project\\USD\\split_later\\20\\20FY",
	"F:\\Project\\USD\\split_later\\50\\50FY","F:\\Project\\USD\\split_later\\100\\100-2\\100FY","F:\\Project\\USD\\split_later\\100\\100-3\\100FY"};

CString file_ZY[9] = {"all_1ZY.txt","all_2ZY.txt","all_5-2ZY.txt","all_5-3ZY.txt","all_10ZY.txt","all_20ZY.txt","all_50ZY.txt","all_100-2ZY.txt","all_100-3ZY.txt"};
string filepathPrefix_ZY[9] = {"F:\\Project\\USD\\split_later\\1\\1ZY","F:\\Project\\USD\\split_later\\2\\2ZY","F:\\Project\\USD\\split_later\\5\\5-2\\5ZY",
	"F:\\Project\\USD\\split_later\\5\\5-3\\5ZY","F:\\Project\\USD\\split_later\\10\\10ZY","F:\\Project\\USD\\split_later\\20\\20ZY",
	"F:\\Project\\USD\\split_later\\50\\50ZY","F:\\Project\\USD\\split_later\\100\\100-2\\100ZY","F:\\Project\\USD\\split_later\\100\\100-3\\100ZY"};


CString file_ZN[9] = {"all_1ZN.txt","all_2ZN.txt","all_5.2ZN.txt","all_5.3ZN.txt","all_10ZN.txt","all_20ZN.txt","all_50ZN.txt","all_100.2ZN.txt","all_100.3ZN.txt"};
string filepathPrefix_ZN[9] = {"D:\\研究生项目\\纹理识别\\code\\USD_split\\1\\1ZN","D:\\研究生项目\\纹理识别\\code\\USD_split\\2\\2ZN","D:\\研究生项目\\纹理识别\\code\\USD_split\\5\\5-2\\5ZN",
	"D:\\研究生项目\\纹理识别\\code\\USD_split\\5\\5-3\\5ZN","D:\\研究生项目\\纹理识别\\code\\USD_split\\10\\10ZN","D:\\研究生项目\\纹理识别\\code\\USD_split\\20\\20ZN",
	"D:\\研究生项目\\纹理识别\\code\\USD_split\\50\\50ZN","D:\\研究生项目\\纹理识别\\code\\USD_split\\100\\100-2\\100ZN","D:\\研究生项目\\纹理识别\\code\\USD_split\\100\\100-3\\100ZN"};

CString singleFileName_FN[9] = {"1FN_","2FN_","5.2FN_","5.3FN_","10FN_","20FN_","50FN_","100.2FN_","100.3FN_"};
CString singleFileName_FY[9] = {"1FY_","2FY_","5.2FY_","5.3FY_","10FY_","20FY_","50FY_","100.2FY_","100.3FY_"};
CString singleFileName_ZN[9] = {"1ZN_","2ZN_","5.2ZN_","5.3ZN_","10ZN_","20ZN_","50ZN_","100.2ZN_","100.3ZN_"};
CString singleFileName_ZY[9] = {"1ZY_","2ZY_","5.2ZY_","5.3ZY_","10ZY_","20ZY_","50ZY_","100.2ZY_","100.3ZY_"};


DWORD WINAPI ComputeFY(LPVOID lpParamater)
{
	vector<double> feature_result;
	CFileException mException;

	for(int i=0;i<9;++i)
	{
	CStdioFile* mFile = new CStdioFile();
	mFile->Open(file_ZN[i],CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite,&mException);
	list<string> filepathList;
	vector<string>files;
	CCompute* main_compute = new CCompute();
	main_compute->getAllFiles(filepathPrefix_FY[i], files);
	vector<string>::iterator ite_str;
	for(ite_str=files.begin();ite_str!=files.end();ite_str++)
	{
		string substring = (*ite_str).substr((*ite_str).length()-3,(*ite_str).length());
		//是bmp文件则存入list中
		if(substring == "bmp")//||substring == "jpg")
			filepathList.push_back(*ite_str);
	}
	//对于list中的每一个bmp文件的操作
	list<string>::iterator it_str = filepathList.begin();
	int count = 0;			//文件计数
	for( ;it_str!= filepathList.end(); ++it_str )
	{
		count++;
		CString imageFilePath((*it_str).c_str());
		main_compute->OpenImage(imageFilePath);
		feature_result = main_compute->ComputeFeature();
		int i=0;	//用于输出每20个纹理参数后分行
		for(vector<double>::iterator it=feature_result.begin();it!=feature_result.end();++it)
		{
			CString p ;
			p.Format(_T("%f"),*it);
			mFile->WriteString( p );
			mFile->WriteString(_T("\t"));

			++i;
			if(i%5==0)
				mFile->WriteString(_T("\n"));
		}
		mFile->WriteString(_T("\n"));
		}
	mFile->Close();
	delete mFile;
	delete main_compute;
	}
	return 0;
}


int main()
{
	//开启另外一个线程
	//HANDLE h1 = CreateThread(NULL,0,ComputeFY,NULL,0,NULL);



	vector<double> feature_result;
	//CCompute main_compute;

//只需要一个文件

#ifdef _ALLTEXT
	//CStdioFile mFile;
	CFileException mException;
	//保证能写入中文
	setlocale(LC_CTYPE,"chs") ;



	for(int i=0;i<9;++i)
	{
	CStdioFile* mFile = new CStdioFile();
	mFile->Open(/*_T("all_5-2ZN.txt")*/"D:\\研究生项目\\纹理识别\\code\\GLCMdata\\f20d1\\all\\ZN\\"+file_ZN[i],CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite,&mException);
#endif

#ifdef _SINGLETXT
	for(int i=0;i<9;++i)
	{
#endif

	list<string> filepathList;
	vector<string>files;
	//string filepathPrefix("F:\\Project\\USD\\split_later\\5\\5-2\\5ZN");

	//获取目录下的所有文件夹和文件名称，存入list中
	CCompute* main_compute = new CCompute();
	main_compute->getAllFiles(filepathPrefix_ZN[i], files);
	vector<string>::iterator ite_str;
	for(ite_str=files.begin();ite_str!=files.end();ite_str++)
	{
		string substring = (*ite_str).substr((*ite_str).length()-3,(*ite_str).length());
		//是bmp文件则存入list中
		if(substring == "bmp")//||substring == "jpg")
			filepathList.push_back(*ite_str);
	}

	//对于list中的每一个bmp文件的操作
	list<string>::iterator it_str = filepathList.begin();
	int count = 0;			//文件计数
	for( ;it_str!= filepathList.end(); ++it_str )
	{
		count++;
		CString imageFilePath((*it_str).c_str());
		main_compute->OpenImage(imageFilePath);
		feature_result = main_compute->ComputeFeature();

		//***************************************************//
#ifdef _SINGLETXT
		CStdioFile* mEachFile=new CStdioFile();
		CFileException mException;
		//保证能写入中文
		setlocale(LC_CTYPE,"chs") ;

		//CString fname1 ="1FN_";
		CString fname1 = singleFileName_ZN[i];
		CString fname2;
		fname2.Format(_T("%d"),count);
		CString fname3 =".txt";
		CString fname = "D:\\研究生项目\\纹理识别\\code\\GLCMdata\\f20d3\\single\\ZN\\"+fname1 + fname2 + fname3;
		mEachFile->Open(fname,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite,&mException);

		int j=0;	//用于输出每5个纹理参数后分行
		for(vector<double>::iterator it=feature_result.begin();it!=feature_result.end();++it)
		{
			CString p ;
			p.Format(_T("%f"),*it);
			mEachFile->WriteString( p );
			mEachFile->WriteString(_T("\t"));

			++j;
			if(it!=(feature_result.end()-1))
			{
				if(j%20==0)
					mEachFile->WriteString(_T("\n"));
			}
		}
		
		//mEachFile.WriteString(_T("\n"));
		mEachFile->Close();
		delete mEachFile;
	}

	}
#endif
		//***************************************************//
#ifndef _SINGLETXT
		int i=0;	//用于输出每20个纹理参数后分行
		for(vector<double>::iterator it=feature_result.begin();it!=feature_result.end();++it)
		{
			CString p ;
			p.Format(_T("%f"),*it);
			mFile->WriteString( p );
			mFile->WriteString(_T("\t"));

			++i;
			//if(it!=(feature_result.end()-1))
			//{
			if(it_str==--filepathList.end()&&it==(feature_result.end()-1))
			{
				int j = i;
				break;
			}
			else if(i%20==0)
				mFile->WriteString(_T("\n"));
			//}
		}
		if(it_str!=--filepathList.end())
			mFile->WriteString(_T("\n"));
		
	}
	mFile->Close();
	delete mFile;
	delete main_compute;
	}
#endif
	


	
	cout<<"计算完毕！"<<endl;
	/*
	vector<double> feature_result;
	CCompute main_compute;
	CStdioFile mFile;
	CFileException mException;
	//保证能写入中文
	setlocale(LC_CTYPE,"chs") ;
	mFile.Open(_T("TextureResult.txt"),CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite,&mException);
	string filepathPrefix("F:\\Project\\Extract_Texture\\6_25\\Testcase");
	list<string> filepath_testcase;
	filepath_testcase.push_back( filepathPrefix + "\\4x4_0d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\4x4_45d135d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\4x4_90d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\8x8_0d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\8x8_45d135d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\8x8_90d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\16x16_0d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\16x16_45d135d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\16x16_90d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\32x32_0d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\32x32_45d135d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\32x32_90d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\64x64_0d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\64x64_90d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\128x128_0d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\128x128_90d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\256x256_0d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\256x256_90d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\512x512_0d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\512x512_90d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\1024x1024_0d.bmp" );
	filepath_testcase.push_back( filepathPrefix + "\\1024x1024_90d.bmp" );

	list<string>::iterator it_str = filepath_testcase.begin();
	for( ;it_str!= filepath_testcase.end(); ++it_str )
	{
		CString wfilepath((*it_str).c_str());
		main_compute.OpenImage( wfilepath );
		feature_result = main_compute.ComputeFeature();
		vector<double>::iterator it_double=feature_result.begin();
		//向txt中写入提取的纹理参数
		//0度纹理特征值
		CString strEnergy0;
		strEnergy0.Format(_T("%f"),*it_double++);
		CString strEntropy0;
		strEntropy0.Format(_T("%f"),*it_double++);
		CString strInertiaQuadrature0;
		strInertiaQuadrature0.Format(_T("%f"),*it_double++);
		CString strLocalCalm0;
		strLocalCalm0.Format(_T("%f"),*it_double++);
		CString strCorrelation0;
		strCorrelation0.Format(_T("%f"),*it_double++);

		mFile.WriteString( wfilepath + "\t" );
		//mFile.WriteString( _T("-----------------------------0度纹理特征值----------------------------\n"));
		mFile.WriteString( _T("能量：")         + strEnergy0               +   _T(" "));
		mFile.WriteString( _T("熵：")           + strEntropy0              +   _T(" ") );
		mFile.WriteString( _T("惯性矩：")       + strInertiaQuadrature0    +   _T(" ") );
		mFile.WriteString( _T("局部平稳性：")   + strLocalCalm0            +   _T(" ") );
		mFile.WriteString( _T("相关性：")       + strCorrelation0          +   _T("\t") );
		//mFile.WriteString( _T("-----------------------------0度纹理特征值----------------------------\n\n\n"));
		
		
		//45度纹理特征值
		CString strEnergy1;
		strEnergy1.Format(_T("%f"),*it_double++);
		CString strEntropy1;
		strEntropy1.Format(_T("%f"),*it_double++);
		CString strInertiaQuadrature1;
		strInertiaQuadrature1.Format(_T("%f"),*it_double++);
		CString strLocalCalm1;
		strLocalCalm1.Format(_T("%f"),*it_double++);
		CString strCorrelation1;
		strCorrelation1.Format(_T("%f"),*it_double++);

		//mFile.WriteString( _T("-----------------------------45度纹理特征值---------------------------\n"));
		mFile.WriteString( _T("能量：")         + strEnergy1               +   _T(" "));
		mFile.WriteString( _T("熵：")           + strEntropy1              +   _T(" ") );
		mFile.WriteString( _T("惯性矩：")       + strInertiaQuadrature1    +   _T(" ") );
		mFile.WriteString( _T("局部平稳性：")   + strLocalCalm1            +   _T(" ") );
		mFile.WriteString( _T("相关性：")       + strCorrelation1          +   _T("\t") );
		//mFile.WriteString( _T("-----------------------------45度纹理特征值---------------------------\n\n\n"));
		
		

		//90度纹理特征值
		CString strEnergy2;
		strEnergy2.Format(_T("%f"),*it_double++);
		CString strEntropy2;
		strEntropy2.Format(_T("%f"),*it_double++);
		CString strInertiaQuadrature2;
		strInertiaQuadrature2.Format(_T("%f"),*it_double++);
		CString strLocalCalm2;
		strLocalCalm2.Format(_T("%f"),*it_double++);
		CString strCorrelation2;
		strCorrelation2.Format(_T("%f"),*it_double++);

		//mFile.WriteString( _T("-----------------------------90度纹理特征值---------------------------\n"));
		mFile.WriteString( _T("能量：")         + strEnergy2               +   _T(" "));
		mFile.WriteString( _T("熵：")           + strEntropy2              +   _T(" ") );
		mFile.WriteString( _T("惯性矩：")       + strInertiaQuadrature2    +   _T(" ") );
		mFile.WriteString( _T("局部平稳性：")   + strLocalCalm2            +   _T(" ") );
		mFile.WriteString( _T("相关性：")       + strCorrelation2          +   _T("\t") );
		//mFile.WriteString( _T("-----------------------------90度纹理特征值---------------------------\n\n\n"));



		//135度纹理特征值
		CString strEnergy3;
		strEnergy3.Format(_T("%f"),*it_double++);
		CString strEntropy3;
		strEntropy3.Format(_T("%f"),*it_double++);
		CString strInertiaQuadrature3;
		strInertiaQuadrature3.Format(_T("%f"),*it_double++);
		CString strLocalCalm3;
		strLocalCalm3.Format(_T("%f"),*it_double++);
		CString strCorrelation3;
		strCorrelation3.Format(_T("%f"),*it_double);

		//mFile.WriteString( _T("-----------------------------135度纹理特征值---------------------------\n"));
		mFile.WriteString( _T("能量：")         + strEnergy3               +   _T(" "));
		mFile.WriteString( _T("熵：")           + strEntropy3              +   _T(" ") );
		mFile.WriteString( _T("惯性矩：")       + strInertiaQuadrature3    +   _T(" ") );
		mFile.WriteString( _T("局部平稳性：")   + strLocalCalm3            +   _T(" ") );
		mFile.WriteString( _T("相关性：")       + strCorrelation3          +   _T("\n") );
		//mFile.WriteString( _T("-----------------------------135度纹理特征值---------------------------\n\n\n"));
		
		//mFile.WriteString( _T("\n\n\n\n\n\n"));
	}
	mFile.Close();
	*/

	//CloseHandle(h1);
	return 0;
}