%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   author: reborn
%   date:   2016/11/8
%   descr:  批量将图像灰度直方图均衡化
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

close all;
clear all;
fclose('all');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
kind = 20;
dollarArray = {'5';'10';'20';'50';'100'};
USDKind = 5;
directionArray = {'ZY';'FN';'ZN';'ZY'};
directionKind = 4;
srcImagePath1 = 'D:\研究生项目\纹理识别\中钞识别\Chinese_split\Statistics\';
srcImagePath2 = 'D:\研究生项目\纹理识别\中钞识别\Chinese_split\Testcase\';
saveImagePath1 = 'D:\研究生项目\纹理识别\中钞识别\Chinese_split\grayEqualization\Statistics\';
saveImagePath2 = 'D:\研究生项目\纹理识别\中钞识别\Chinese_split\grayEqualization\Testcase\';

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
%对图片批量灰度均衡化并保存
for i=1:kind
	USD = dollarArray{fix((i-1)/directionKind)+1};
	direction = directionArray{mod(i,directionKind)+1};
	[testcaseFiles,testcasePaths] = dfsFolder([srcImagePath1 USD '\' direction '\'],'.bmp');
	for j = 1: size(testcaseFiles,2)
		%srcImage = imread(testcasePaths{j});
		eqImage = grayToHisteq1(testcasePaths{j});
		savePath = strcat(saveImagePath1,USD,'\',direction,'\',testcaseFiles(1,j));
		imwrite(eqImage,savePath{1});
	end
end

for i=1:kind
	USD = dollarArray{fix((i-1)/directionKind)+1};
	direction = directionArray{mod(i,directionKind)+1};
	[testcaseFiles,testcasePaths] = dfsFolder([srcImagePath2 USD '\' direction '\'],'.bmp');
	for j = 1: size(testcaseFiles,2)
		%srcImage = imread(testcasePaths{j});
		eqImage = grayToHisteq1(testcasePaths{j});
		savePath = strcat(saveImagePath2,USD,'\',direction,'\',testcaseFiles(1,j));
		imwrite(eqImage,savePath{1});
	end
end	


