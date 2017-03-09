%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   author: reborn
%   date:   2016/10/22
%   descr:  compare two funtions of the same feature for two kinds of USD
%			相交面积宽度为[交点-factor*sigma1,交点+factor*sigma2]
%			优化算法，使其直接将数据读入内存，不用读文件，去除黑边处理代码。
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% function ans = Compare2USD()
% ans = allMinLocation;

tic;
close all;                        %清空工作区，清空命令区域，关闭所有窗口
clear all;
fclose('all');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 

USDKind = 5;
featureKind = 10;
%titleArray = {'能量';'熵值';'对比度';'逆差矩';'相关性'};
titleArray = {'能量0';'熵值0';'对比度0';'逆差矩0';'相关性0';'能量90';'熵值90';'对比度90';'逆差矩90';'相关性90'};
dollarArray = {'5';'10';'20';'50';'100'};
dataPath = '..\newall\f10d1\ZN\';
outputPath = './recognitionResult/f10d1/ZN/'
% dataPath = '..\newall\f5d1\grayEqualization\ZN\';
% outputPath = './recognitionResult/f5d1_ori/grayEqualization/ZN/'
coupleCnt = 1;
interval = 100;
%factor = 1;
factor = 1;
exceptionValue = 100.0;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%开始处理%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fo = fopen([outputPath '每对美金特征值相差最大位置f10d1_1factor.txt'],'wt');
fo1 = fopen([outputPath 'minLocation_f10d1_1factor.txt'],'wt');
fprintf(fo,'%s\t%s\t%s\t%s\t%s\n','美金1','美金2','特征参数','最小位子','相交面积');

for i = 1:USDKind
	allData(i).data = importdata([dataPath 'newall_' dollarArray{i} 'ZN.txt']);
	allData(i).USD = dollarArray{i};
	
	line = 1;
	allData(i).aver = zeros(size(allData(i).data,1));
	allData(i).varia = zeros(size(allData(i).data,1));
	while(line<=size(allData(i).data,1))
		
		aver = mean(allData(i).data(line,:));
		varia = std(allData(i).data(line,:));
		
		allData(i).aver(line) = aver;
		allData(i).varia(line) = varia;
		
		line = line+1;
	end
end


for i = 1: USDKind-1
	for j = i+1: USDKind
		idollar = allData(i).USD;
		jdollar = allData(j).USD;
		disp(idollar);
		disp(jdollar);
		
		for f = 1:featureKind
			featureArray(f).area = 100;
			featureArray(f).location = -1;
			featureArray(f).feature = -1;
			featureArray(f).couple = {idollar,jdollar};
		end
		
		for l = 1:size(allData(i).data,1)
			
			location = fix(l/featureKind)+1;
			feature = mod(l,featureKind);
			if(feature==0)
				feature = 10;
			end
			
			featureArray = computeIntersection(featureArray,l,featureKind,factor,interval,allData(i),allData(j),location,feature);
		end
		
		fprintf(fo,'%s\t%s\t',idollar,jdollar);
		for f = 1:featureKind
			fprintf(fo,'%s\t%s\t','','');
			fprintf(fo,'%s\t\t',titleArray{featureArray(f).feature});
			fprintf(fo,'%d\t',featureArray(f).location);
			fprintf(fo,'%3.10f\n',featureArray(f).area);
			
			fprintf(fo1,'%s %s ',char(featureArray(f).couple{1}),char(featureArray(f).couple{2}));
			fprintf(fo1,'%d ',featureArray(f).feature);
			fprintf(fo1,'%d ',featureArray(f).location);
			fprintf(fo1,'%3.10f\n',featureArray(f).area);
		end
		coupleCnt= coupleCnt+1;
		%pause();
		%break;
	end
	clear allData(i);
	%break;
end

fclose(fo);
fclose(fo1);
fclose('all');

toc;
					

