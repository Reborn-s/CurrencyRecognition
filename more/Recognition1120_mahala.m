%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   author: reborn
%   date:   2016/11/20
%   descr:  recognize the given USD belonging to which kind with the comparing results.
%			优化算法，使其直接将数据读入内存，不用读文件，去除黑边处理代码。
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

tic;
close all;                        %关闭所有窗口
clear all;						  %清空工作区，清空命令区域
fclose('all');					  %关闭所有文件
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
USDKind = 5;
featureKind = 10;
interval = 100;
USDcouple = USDKind*(USDKind-1)/2;
factor = 1;
%titleArray = {'能量';'熵值';'对比度';'逆差矩';'相关性'};
titleArray = {'能量0';'熵值0';'对比度0';'逆差矩0';'相关性0';'能量90';'熵值90';'对比度90';'逆差矩90';'相关性90'};
dollarArray = {'5';'10';'20';'50';'100'};
dataPath = '..\newall\f10d3\ALL';
testcasePath = '../matlabTestcase/f10d3/ALL/';	%待修改
paramPath = './recognitionResult/f10d3/ALL/minLocation_f10d3_1factor.txt';
% dataPath = '..\newall\f5d1\grayEqualization\ZN\';
% testcasePath = '../matlabTestcase/f5d1/grayEqualization/ZN/';	%待修改                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
% paramPath = './recognitionResult/f5d1_ori/grayEqualization/ZN/minLocation_f5d1_0.99factor.txt';
exceptionValue = 100.0;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%读取数据%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
allStatistics = importdata(paramPath);
allCouple(1).couple = {allStatistics(1,1),allStatistics(1,2)};
tempCouple = {allStatistics(1,1),allStatistics(1,2)};
allCouple(1).feature(1) = allStatistics(1,3);
allCouple(1).location(1) = allStatistics(1,4);
coupleCnt = 1;
featureCnt = 1;
for l = 2: size(allStatistics,1)
	if(tempCouple{1}==allStatistics(l,1)&&tempCouple{2}==allStatistics(l,2))
		featureCnt = featureCnt+1;
		allCouple(coupleCnt).feature(featureCnt) = allStatistics(l,3);
		allCouple(coupleCnt).location(featureCnt) = allStatistics(l,4);
	else
		coupleCnt = coupleCnt+1;
		featureCnt = 1;
		allCouple(coupleCnt).couple = {allStatistics(l,1),allStatistics(l,2)};
		allCouple(coupleCnt).feature(featureCnt) = allStatistics(l,3);
		allCouple(coupleCnt).location(featureCnt) = allStatistics(l,4);
		tempCouple = {allStatistics(l,1),allStatistics(l,2)};
	end
end

[testcaseFiles,testcasePaths] = dfsFolder(testcasePath,'.txt');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%开始识别%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%fo = fopen('./recognitionResult/f5d1_ori/grayEqualization/ZN/recognizeResult_1.15factor.txt','wt');
fo = fopen('./recognitionResult/f10d5/ZN/recognizeResult_1factorMahala.txt','wt');
fprintf(fo,'%s\t%s\n','识别种类','实际种类');
rightCnt = 0;

for i = 1: size(testcaseFiles,2)
	firstCouple = allCouple(1);
	ignoreCnt = 1;
	ignoreUSD = zeros(ignoreCnt);
	fileName = char(testcaseFiles(1,i));
	trueKind = '';
	for s = 1:length(fileName)
		if fileName(s) ~='Z'
			trueKind = strcat(trueKind,fileName(s));
		else
			break;
		end
	end
	
	distance1 = 0;
	distance2 = 0;
	
	ft = importdata(testcasePaths{i});
	f1 = importdata([dataPath '\newall_' num2str(firstCouple.couple{1}) 'ZN' '.txt']);
	f2 = importdata([dataPath '\newall_' num2str(firstCouple.couple{2}) 'ZN' '.txt']);
	
	array1 = zeros(1,featureKind);
	array2 = zeros(1,featureKind);
	tarray = zeros(1,featureKind);
	
	for f = 1: featureKind
		line = f+(firstCouple.location(f)-1)*featureKind;
		tdata = ft(line,:);
		data1 = f1(line,:);
		data2 = f2(line,:);
		
		if(tdata==exceptionValue||ismember(exceptionValue,data1)||ismember(exceptionValue,data2))
			continue;
		end
		array1(1,f) = mean(data1);
		array2(1,f) = mean(data2);
		tarray(1,f) = tdata;
	end
	array12 = [array1;array2];
	tarray = tarray;
	distanceArray = mahalanobis(array12,tarray);
	distance1 = distanceArray(1);
	distance2 = distanceArray(2);
	
	if(distance1<=distance2)
		ignoreUSD(ignoreCnt) = firstCouple.couple{2}
	else
		ignoreUSD(ignoreCnt) = firstCouple.couple{1}
	end
	
	%pause();
	%break;
	
	for c = 2:coupleCnt
		flag = 0;
		for g = 1:size(ignoreUSD,2)
			if ((allCouple(c).couple{1}==ignoreUSD(g))||(allCouple(c).couple{2}==ignoreUSD(g)))
				flag = 1;
				break;
			end
		end
		if(flag==1)
			continue;
		end
		
		ignoreCnt = ignoreCnt+1;
		firstCouple = allCouple(c);
		distance1 = 0;
		distance2 = 0;
		
		f1 = importdata([dataPath '\newall_' num2str(firstCouple.couple{1}) 'ZN' '.txt']);
		f2 = importdata([dataPath '\newall_' num2str(firstCouple.couple{2}) 'ZN' '.txt']);
		for f = 1: featureKind
			line = f+(firstCouple.location(f)-1)*featureKind;
			tdata = ft(line,:);
			data1 = f1(line,:);
			data2 = f2(line,:);
		
			if(tdata==exceptionValue||ismember(exceptionValue,data1)||ismember(exceptionValue,data2))
				continue;
			end
			array1(1,f) = mean(data1);
			array2(1,f) = mean(data2);
			tarray(1,f) = tdata;
		end
		array12 = [array1;array2];
		tarray = tarray;
		distanceArray = mahalanobis(array12,tarray);
		distance1 = distanceArray(1);
		distance2 = distanceArray(2);
		if(distance1<distance2)
			ignoreUSD(ignoreCnt) = firstCouple.couple{2}
			minUSD = firstCouple.couple{1}
		else
			ignoreUSD(ignoreCnt) = firstCouple.couple{1}
			minUSD = firstCouple.couple{2}
		end
		%pause();
	end
	disp('识别种类为：');
	disp(minUSD);
	disp('实际种类为：');
	disp(trueKind);
	
	if(minUSD==str2num(trueKind))
		fprintf(fo,'%3.0f\t\t%s\n',minUSD,trueKind);
		rightCnt = rightCnt+1;
	else	
		fprintf(fo,'%3.0f\t\t%s\t%s\n',minUSD,trueKind,fileName);
	end
	
end

fprintf(fo,'\n','');
fprintf(fo,'%s\t%3.0f\n','正确识别数：',rightCnt);
fprintf(fo,'%s\t%3.0f\n','总数为：',size(testcaseFiles,2));
accuracyRate = rightCnt/size(testcaseFiles,2);
disp('准确率为：');
disp(accuracyRate);
fprintf(fo,'%s\t','准确率为：');
fprintf(fo,'%3.6f\n',accuracyRate);

fclose(fo);
fclose('all');

toc;
	
		
		
		
			
		
		
	
	
