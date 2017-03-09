%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   author: reborn
%   date:   2016/10/22
%   descr:  recognize the given USD belonging to which kind with the comparing results.
%			�Ż��㷨��ʹ��ֱ�ӽ����ݶ����ڴ棬���ö��ļ���ȥ���ڱߴ�����롣
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

tic;
close all;                        %�ر����д���
clear all;						  %��չ������������������
fclose('all');					  %�ر������ļ�
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
testcaseCnt = 7;
USDKind = 20;
featureKind = 10;
interval = 100;
USDcouple = USDKind*(USDKind-1)/2;
titleArray = {'����';'��ֵ';'�Աȶ�';'����';'�����'};
dollarArray = {'5FN';'5FY';'5ZN';'5ZY';'10FN';'10FY';'10ZN';'10ZY';...
			   '20FN';'20FY';'20ZN';'20ZY';'50FN';'50FY';'50ZN';'50ZY';...
			   '100FN';'100FY';'100ZN';'100ZY'};
dataPath = '..\newall\f10d3\ALL';
testcasePath = '../matlabTestcase/f10d3/ALL/';	%���޸�
%testcasePath = './miniRecoTestcase/'
paramPath = './recognitionResult/f10d3/ALL/minLocation_f10d3_1factor.txt';
exceptionValue = 100.0;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%��ȡ����%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fparam = fopen(paramPath,'rt');
firstLine = fgetl(fparam);
splitLine = regexp(firstLine,'\s+','split');
allCouple(1).couple = {char(splitLine(1)),char(splitLine(2))};
tempCouple = {char(splitLine(1)),char(splitLine(2))};
allCouple(1).feature(1) = str2num(char(splitLine(3)));
allCouple(1).location(1) = str2num(char(splitLine(4)));
coupleCnt = 1;
featureCnt = 1;
while(~feof(fparam))
	thisLine = fgetl(fparam);
	allStatistics = regexp(thisLine,'\s+','split');
	if(size(tempCouple{1},2)==size(char(allStatistics(1)),2)&&size(tempCouple{2},2)==size(char(allStatistics(2)),2))
		if(min(tempCouple{1}==char(allStatistics(1)))==1&&min(tempCouple{2}==char(allStatistics(2)))==1)
			featureCnt = featureCnt+1;
			allCouple(coupleCnt).feature(featureCnt) = str2num(char(allStatistics(3)));
			allCouple(coupleCnt).location(featureCnt) = str2num(char(allStatistics(4)));
		else
			coupleCnt = coupleCnt+1;
			featureCnt = 1;
			allCouple(coupleCnt).couple = {char(allStatistics(1)),char(allStatistics(2))};
			allCouple(coupleCnt).feature(featureCnt) = str2num(char(allStatistics(3)));
			allCouple(coupleCnt).location(featureCnt) = str2num(char(allStatistics(4)));
			tempCouple = {char(allStatistics(1)),char(allStatistics(2))};
		end
	else
		coupleCnt = coupleCnt+1;
		featureCnt = 1;
		allCouple(coupleCnt).couple = {char(allStatistics(1)),char(allStatistics(2))};
		allCouple(coupleCnt).feature(featureCnt) = str2num(char(allStatistics(3)));
		allCouple(coupleCnt).location(featureCnt) = str2num(char(allStatistics(4)));
		tempCouple = {char(allStatistics(1)),char(allStatistics(2))};
	end
end

[testcaseFiles,testcasePaths] = dfsFolder(testcasePath,'.txt');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%��ʼʶ��%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fo = fopen('./recognitionResult/f10d3/ALL/recognizeResult_1factor.txt','wt');
fprintf(fo,'%s\t%s\n','ʶ������','ʵ������');
rightCnt = 0;

for i = 1: size(testcaseFiles,2)
	for g = 1:USDKind-1
		ignoreUSD(g).USD = '';
	end
	firstCouple = allCouple(1);
	ignoreCnt = 1;
	%ignoreUSD = zeros(ignoreCnt);
	fileName = char(testcaseFiles(1,i));
	trueKind = '';
	for s = 1:length(fileName)
		if fileName(s) ~='_'
			trueKind = strcat(trueKind,fileName(s));
		else
			break;
		end
	end
	
	distance1 = 0;
	distance2 = 0;
	
	ft = importdata(testcasePaths{i});
	f1 = importdata([dataPath '\newall_' firstCouple.couple{1} '.txt']);
	f2 = importdata([dataPath '\newall_' firstCouple.couple{2} '.txt']);
	for f = 1: featureKind
		line = f+(firstCouple.location(f)-1)*featureKind;
		tdata = ft(line,:);
		data1 = f1(line,:);
		data2 = f2(line,:);
		
		if(tdata==exceptionValue||ismember(exceptionValue,data1)||ismember(exceptionValue,data2))
			continue;
		end
	
		aver1 = mean(data1);
		varia1 = std(data1);
		aver2 = mean(data2);
		varia2 = std(data2);
		
		distance1 = distance1+(abs(aver1-tdata))/varia1;
		distance2 = distance2+(abs(aver2-tdata))/varia2;
		
	end
	if(distance1<=distance2)
		ignoreUSD(ignoreCnt).USD = firstCouple.couple{2}
	else
		ignoreUSD(ignoreCnt).USD = firstCouple.couple{1}
	end
	
	%pause();
	%break;
	
	for c = 2:coupleCnt
		flag = 0;
		for g = 1:size(ignoreUSD,2)
			if((size(allCouple(c).couple{1},2)==size(ignoreUSD(g).USD,2)&&min(allCouple(c).couple{1}==ignoreUSD(g).USD)==1)||...
			(size(allCouple(c).couple{2},2)==size(ignoreUSD(g).USD,2)&&min(allCouple(c).couple{2}==ignoreUSD(g).USD)==1))
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
		
		f1 = importdata([dataPath '\newall_' firstCouple.couple{1} '.txt']);
		f2 = importdata([dataPath '\newall_' firstCouple.couple{2} '.txt']);
		for f = 1: featureKind
			line = f+(firstCouple.location(f)-1)*featureKind;
			tdata = ft(line,:);
			data1 = f1(line,:);
			data2 = f2(line,:);
			
			if(tdata==exceptionValue||ismember(exceptionValue,data1)||ismember(exceptionValue,data2))
				continue;
			end
	
			aver1 = mean(data1);
			varia1 = std(data1);
			aver2 = mean(data2);
			varia2 = std(data2);
		
			distance1 = distance1+(abs(aver1-tdata))/varia1;
			distance2 = distance2+(abs(aver2-tdata))/varia2;
			% hist1 = histc(data1,min(data1):(max(data1)-min(data1))/interval:max(data1);
			% hist2 = histc(data2,min(data2):(max(data2)-min(data2))/interval:max(data2);
			% distance1 = distance1+(abs(-tdata))/varia1;
			% distance2 = distance2+(abs(aver2-tdata))/varia2;
		end
		if(distance1<distance2)
			ignoreUSD(ignoreCnt).USD = firstCouple.couple{2}
			minUSD = firstCouple.couple{1}
		else
			ignoreUSD(ignoreCnt).USD = firstCouple.couple{1}
			minUSD = firstCouple.couple{2}
		end
		%pause();
	end
	disp('��������Ϊ��');
	disp(minUSD);
	disp('ʵ������Ϊ��');
	disp(trueKind);
	
	if((size(minUSD,2)==size(trueKind,2))&&min(minUSD==trueKind)==1)
		fprintf(fo,'%s\t\t%s\n',minUSD,trueKind);
		rightCnt = rightCnt+1;
	else	
		fprintf(fo,'%s\t\t%s\t%s\n',minUSD,trueKind,fileName);
	end
	
end

fprintf(fo,'\n','');
fprintf(fo,'%s\t%3.0f\n','��ȷʶ������',rightCnt);
fprintf(fo,'%s\t%3.0f\n','����Ϊ��',size(testcaseFiles,2));
accuracyRate = rightCnt/size(testcaseFiles,2);
disp('׼ȷ��Ϊ��');
disp(accuracyRate);
fprintf(fo,'%s\t','׼ȷ��Ϊ��');
fprintf(fo,'%3.3f\n',accuracyRate);

fclose(fo);
fclose('all');

toc;
	
		
		
		
			
		
		
	
	
