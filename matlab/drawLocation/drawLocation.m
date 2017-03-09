clear all;

featureKind = 10;
length = 32;
lineNumber = 20;
lineSize = 2;
wSize = [32,32];
color = [255,255,0];%黄色
flag = 2;

paramPath = 'minLocation_f10d5_1factor.txt';

allStatistics = importdata(paramPath);
allCouple(1).couple = {allStatistics(1,1),allStatistics(1,2)};
allCouple(1).feature(1) = allStatistics(1,3);
allCouple(1).location(1) = allStatistics(1,4);
featureCnt = 1;
for l = 2: size(allStatistics,1)
	featureCnt = featureCnt+1;
	allCouple(1).feature(featureCnt) = allStatistics(l,3);
	allCouple(1).location(featureCnt) = allStatistics(l,4);
end

f5 = '5ZN.bmp';
f100 = '100ZN.bmp';
p5 = imread(f5);
I5 = imresize(p5,[256,640]);
p100 = imread(f100);
I100 = imresize(p100,[256,640]);
for f = 1: featureKind
	[m,n,z] = size(I5);
	location = allStatistics(f,4);
	pt = [(mod(location,lineNumber)-1)*length,fix(location/lineNumber)*length];
	dest = drawRect(I5,pt,wSize,lineSize,color,flag);
	figure(3);
	imshow(dest);
		
	[m,n,z] = size(I100);
	location = allStatistics(f,4);
	pt = [(mod(location,lineNumber)-1)*length,fix(location/lineNumber)*length];
	dest = drawRect(I100,pt,wSize,lineSize,color,flag);
	figure(4);
	imshow(dest);

	pause();
end