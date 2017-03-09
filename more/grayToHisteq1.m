%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   author: reborn
%   date:   2016/11/10
%   descr:  画出图像的灰度直方图并将其灰度均衡化
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function equalImage = grayToHisteq1(srcImage);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
%使用自编的算法
% 单张图片灰度均衡化

	I = imread(srcImage);
	[height,width] = size(I);  
	% figure  
	% subplot(221)  
	% imshow(I)%显示原始图像
	% title('原图');	
	% subplot(222)  
	% imhist(I)%显示原始图像直方图  
	% title('原图灰度图');
  
	%进行像素灰度统计;  
	NumPixel = zeros(1,256);%统计各灰度数目，共256个灰度级  
	for i = 1:height  
		for j = 1: width  
			NumPixel(I(i,j) + 1) = NumPixel(I(i,j) + 1) + 1;%对应灰度值像素点数量增加一  
		end  
	end  
	%计算灰度分布密度  
	ProbPixel = zeros(1,256);  
	for i = 1:256  
		ProbPixel(i) = NumPixel(i) / (height * width * 1.0);  
	end  
	%计算累计直方图分布  
	CumuPixel = zeros(1,256);  
	for i = 1:256  
		if i == 1  
			CumuPixel(i) = ProbPixel(i);  
		else  
			CumuPixel(i) = CumuPixel(i - 1) + ProbPixel(i);  
		end  
	end  
	%累计分布取整  
	CumuPixel = uint8(255 .* CumuPixel + 0.5);  
	%对灰度值进行映射（均衡化） 
	equalImage = I;
	for i = 1:height  
		for j = 1: width  
			equalImage(i,j) = CumuPixel(I(i,j)+1);  
		end  
	end  
  
	% subplot(223)  
	% imshow(I)%显示原始图像  
	% title('灰度均衡化的图');
	% subplot(224)  
	% imhist(I)%显示原始图像直方图  
	% title('结果图的灰度图');
	
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
%使用自带函数
% [testcaseFiles,testcasePaths] = dfsFolder('D:/研究生项目/纹理识别/中钞识别/matlab/exceptionTestcase/temp2/','.bmp');
% 单张图片灰度均衡化
% for i =1: size(testcaseFiles,2)
% srcImage = imread(testcasePaths{i});
% figure(i);
% subplot(2,3,1);
% imshow(srcImage);
% title(testcaseFiles(1,i));

% subplot(2,3,[2 3]);
% imhist(srcImage);
% title('原图灰度图');

% subplot(2,3,4);
% eqImage = histeq(srcImage);
% imshow(eqImage);
% title('灰度均衡化的图');

% subplot(2,3,[5 6]);
% imhist(eqImage);
% title('结果图的灰度图');

% pause();
% end




