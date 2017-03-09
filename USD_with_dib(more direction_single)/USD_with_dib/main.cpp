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

CString singleFileName_FN[9] = {"1FN_","2FN_","5-2FN_","5-3FN_","10FN_","20FN_","50FN_","100-2FN_","100-3FN_"};
CString singleFileName_FY[9] = {"1FY_","2FY_","5-2FY_","5-3FY_","10FY_","20FY_","50FY_","100-2FY_","100-3FY_"};
CString singleFileName_ZN[9] = {"1ZN_","2ZN_","5-2ZN_","5-3ZN_","10ZN_","20ZN_","50ZN_","100-2ZN_","100-3ZN_"};
CString singleFileName_ZY[9] = {"1ZY_","2ZY_","5-2ZY_","5-3ZY_","10ZY_","20ZY_","50ZY_","100-2ZY_","100-3ZY_"};


CString file_ZN[9] = {"all_1ZN.txt","all_2ZN.txt","all_5-2ZN.txt","all_5-3ZN.txt","all_10ZN.txt","all_20ZN.txt","all_50ZN.txt","all_100-2ZN.txt","all_100-3ZN.txt"};




string filepathPrefix_ZN[9] = {"D:\\研究生项目\\纹理识别\\code\\USD_split\\1\\1ZN","D:\\研究生项目\\纹理识别\\code\\USD_split\\2\\2ZN","D:\\研究生项目\\纹理识别\\code\\USD_split\\5\\5-2\\5ZN",
	"D:\\研究生项目\\纹理识别\\code\\USD_split\\5\\5-3\\5ZN","D:\\研究生项目\\纹理识别\\code\\USD_split\\10\\10ZN","D:\\研究生项目\\纹理识别\\code\\USD_split\\20\\20ZN",
	"D:\\研究生项目\\纹理识别\\code\\USD_split\\50\\50ZN","D:\\研究生项目\\纹理识别\\code\\USD_split\\100\\100-2\\100ZN","D:\\研究生项目\\纹理识别\\code\\USD_split\\100\\100-3\\100ZN"};





//最后一个参数返回的是子字符串的数量
CString * SplitString(CString str, char split, int& iSubStrs)
{
	int iPos = 0; //分割符位置
    int iNums = 0; //分割符的总数
    CString strTemp = str;
    CString strRight;
    //先计算子字符串的数量
    while (iPos != -1)
    {
        iPos = strTemp.Find(split);
        if (iPos == -1)
        {
            break;
        }
        strRight = strTemp.Mid(iPos + 1, str.GetLength());
        strTemp = strRight;
        iNums++;
    }
    if (iNums == 0) //没有找到分割符
    {
        //子字符串数就是字符串本身
        iSubStrs = 1; 
        return NULL;
    }
    //子字符串数组
    iSubStrs = iNums + 1; //子串的数量 = 分割符数量 + 1
    CString* pStrSplit;
    pStrSplit = new CString[iSubStrs];
    strTemp = str;
    CString strLeft;
    for (int i = 0; i < iNums; i++)
    {
        iPos = strTemp.Find(split);
        //左子串
        strLeft = strTemp.Left(iPos);
        //右子串
        strRight = strTemp.Mid(iPos + 1, strTemp.GetLength());
        strTemp = strRight;
        pStrSplit[i] = strLeft;
    }
    pStrSplit[iNums] = strTemp;
    return pStrSplit;
}

void getAllFiles( string path, vector<string>& files)
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


string getDestBmpFilePath(string filePrefix)
{
	string destBmpFilePath;
	vector<string> destBmpFoldFiles;
	int _lable = filePrefix.find("_");
	//获取bmp文件编号
	string destNum = filePrefix.substr(_lable+1,filePrefix.length());
	int num;
	num = atoi(destNum.c_str());
	//根据文件名是否包含.来分为两大类
	int dlable = filePrefix.find(".");
	if(dlable != string::npos)//文件名包含.，只能是5元或100元
	{
		string prechar = filePrefix.substr(dlable-1,1);
		if(prechar == "5")//是5元的
		{
			string nxtchar = filePrefix.substr(dlable+1,1);
			if(nxtchar == "2")//5.2开头的
			{
				getAllFiles(filepathPrefix_ZN[2],destBmpFoldFiles);
				destBmpFilePath = destBmpFoldFiles[num-1];
			}
			else//5.3开头的
			{
				getAllFiles(filepathPrefix_ZN[3],destBmpFoldFiles);
				destBmpFilePath = destBmpFoldFiles[num-1];
			}
		}
		else//是100元的
		{
			string nxtchar = filePrefix.substr(dlable+1,1);
			if(nxtchar == "2")//100.2开头的
			{
				getAllFiles(filepathPrefix_ZN[7],destBmpFoldFiles);
				destBmpFilePath = destBmpFoldFiles[num-1];
			}
			else//100.3开头的
			{
				getAllFiles(filepathPrefix_ZN[8],destBmpFoldFiles);
				destBmpFilePath = destBmpFoldFiles[num-1];
			}
		}
	}
	else//文件名不包含.，只能是1,2,10,20,50的
	{
		//根据文件名中Z前面的位置来分为3大类(1,2,0)
		int zlable = filePrefix.find("Z");
		string prechar = filePrefix.substr(zlable-1,1);//Z前面的一个字符
		if(prechar == "1")//1元
		{
			getAllFiles(filepathPrefix_ZN[0],destBmpFoldFiles);
			destBmpFilePath = destBmpFoldFiles[num-1];
		}
		else if(prechar == "2")//2元
		{
			getAllFiles(filepathPrefix_ZN[1],destBmpFoldFiles);
			destBmpFilePath = destBmpFoldFiles[num-1];
		}
		else//10,20,50元之一
		{
			string preprechar = filePrefix.substr(zlable-2,1);//Z前面的第二个字符
			if(preprechar == "1")//10元
			{
				getAllFiles(filepathPrefix_ZN[4],destBmpFoldFiles);
				destBmpFilePath = destBmpFoldFiles[num-1];
			}
			else if(preprechar == "2")//20元
			{
				getAllFiles(filepathPrefix_ZN[5],destBmpFoldFiles);
				destBmpFilePath = destBmpFoldFiles[num-1];
			}
			else//50元
			{
				getAllFiles(filepathPrefix_ZN[6],destBmpFoldFiles);
				destBmpFilePath = destBmpFoldFiles[num-1];
			}
		}
	}

	return destBmpFilePath;
}


int main()
{
	vector<double> feature_result;
	CFileException mException;
	//保证能写入中文
	setlocale(LC_CTYPE,"chs") ;

	CCompute* main_compute = new CCompute();
	
	list<string> filepathList;
	vector<string>files;
	//vector<string>destBmpFilePath;
	string TestCaseTxtPath("D:\\研究生项目\\纹理识别\\code\\USD_with_dib\\matlab\\MostDirectionForTestcases");
	//获取目录下的所有文件夹和文件名称，存入list中
	int len = TestCaseTxtPath.length();

	main_compute->getAllFiles(TestCaseTxtPath, files);
	vector<string>::iterator ite_str;
	for(ite_str=files.begin();ite_str!=files.end();ite_str++)
	{
		string substring = (*ite_str).substr((*ite_str).length()-3,(*ite_str).length());
		//是txt文件则存入list中
		if(substring == "txt")//||substring == "jpg")
			filepathList.push_back(*ite_str);
	}
	

	//对于list中的每一个txt文件的操作
	list<string>::iterator it_str = filepathList.begin();
	int count = 0;			//文件计数
	CString readLine;
	for( ;it_str!= filepathList.end(); ++it_str )
	{
		count++;

		string test = (*it_str).substr(len+1,(*it_str).length());
		int le = test.find_last_of('.');
		string des = test.substr(0,le);
		//获取目标bmp文件路径
		string destBmpFilePath = getDestBmpFilePath(des);

		CString imageFilePath(destBmpFilePath.c_str());
		main_compute->OpenImage(imageFilePath);
		//打开TestCase文件夹下的txt
		CStdioFile* p_readf = new CStdioFile();
		CString _file = (*it_str).c_str();
		p_readf->Open(_file,CFile::modeRead);
		//创建要生成的txt
		CStdioFile* p_createf = new CStdioFile();
		CString _tmpCreFile1("compareDirection_");//txt前缀
		CString _tmpCreFile2 = test.c_str();
		CString _fullFile = "D:\\研究生项目\\纹理识别\\code\\USD_with_dib\\matlab\\CompareDirectionForTestcases\\"+_tmpCreFile1 + _tmpCreFile2;
		p_createf->Open(_fullFile,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite,&mException);

		while(p_readf->ReadString(readLine))//没有到文件尾
		{
			readLine.Trim();
			//if(readLine != "")
			//{
				int i = 0;
				CString* p_split = SplitString(readLine,'\t',i);
				int destFeature = _ttoi(p_split[0]);//目标纹理参数
				int pos = _ttoi(p_split[1]);//起始位置
				int direction = _ttoi(p_split[2]);//方向
				//feature_result = main_compute->computeDestPartFeature(allPartition,destFeature,pos,direction);
				feature_result = main_compute->computeResult(destFeature,pos,direction);

				vector<double>::iterator it;
				for(it=feature_result.begin();it!=feature_result.end();++it)
				{
					CString p ;
					p.Format(_T("%f"),*it);
					p_createf->WriteString( p );
					if(it!=feature_result.end()-1)
						p_createf->WriteString(_T("\t"));
					else
						p_createf->WriteString(_T("\n"));
					
				}
			//}
			//else
			//{
				//p_createf->WriteString(_T("\n"));
				//break;
			//}
		}
		p_createf->Close();
	}
	
	delete main_compute;
	main_compute = NULL;
	cout<<count<<" 计算完毕！"<<endl;
	return 0;
}