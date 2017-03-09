%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   author: reborn
%   date:   2016/11/8
%   descr:  ������ͼ��Ҷ�ֱ��ͼ���⻯
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
srcImagePath1 = 'D:\�о�����Ŀ\����ʶ��\�г�ʶ��\Chinese_split\Statistics\';
srcImagePath2 = 'D:\�о�����Ŀ\����ʶ��\�г�ʶ��\Chinese_split\Testcase\';
saveImagePath1 = 'D:\�о�����Ŀ\����ʶ��\�г�ʶ��\Chinese_split\grayEqualization\Statistics\';
saveImagePath2 = 'D:\�о�����Ŀ\����ʶ��\�г�ʶ��\Chinese_split\grayEqualization\Testcase\';

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
%��ͼƬ�����ҶȾ��⻯������
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


