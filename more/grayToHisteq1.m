%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   author: reborn
%   date:   2016/11/10
%   descr:  ����ͼ��ĻҶ�ֱ��ͼ������ҶȾ��⻯
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function equalImage = grayToHisteq1(srcImage);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
%ʹ���Ա���㷨
% ����ͼƬ�ҶȾ��⻯

	I = imread(srcImage);
	[height,width] = size(I);  
	% figure  
	% subplot(221)  
	% imshow(I)%��ʾԭʼͼ��
	% title('ԭͼ');	
	% subplot(222)  
	% imhist(I)%��ʾԭʼͼ��ֱ��ͼ  
	% title('ԭͼ�Ҷ�ͼ');
  
	%�������ػҶ�ͳ��;  
	NumPixel = zeros(1,256);%ͳ�Ƹ��Ҷ���Ŀ����256���Ҷȼ�  
	for i = 1:height  
		for j = 1: width  
			NumPixel(I(i,j) + 1) = NumPixel(I(i,j) + 1) + 1;%��Ӧ�Ҷ�ֵ���ص���������һ  
		end  
	end  
	%����Ҷȷֲ��ܶ�  
	ProbPixel = zeros(1,256);  
	for i = 1:256  
		ProbPixel(i) = NumPixel(i) / (height * width * 1.0);  
	end  
	%�����ۼ�ֱ��ͼ�ֲ�  
	CumuPixel = zeros(1,256);  
	for i = 1:256  
		if i == 1  
			CumuPixel(i) = ProbPixel(i);  
		else  
			CumuPixel(i) = CumuPixel(i - 1) + ProbPixel(i);  
		end  
	end  
	%�ۼƷֲ�ȡ��  
	CumuPixel = uint8(255 .* CumuPixel + 0.5);  
	%�ԻҶ�ֵ����ӳ�䣨���⻯�� 
	equalImage = I;
	for i = 1:height  
		for j = 1: width  
			equalImage(i,j) = CumuPixel(I(i,j)+1);  
		end  
	end  
  
	% subplot(223)  
	% imshow(I)%��ʾԭʼͼ��  
	% title('�ҶȾ��⻯��ͼ');
	% subplot(224)  
	% imhist(I)%��ʾԭʼͼ��ֱ��ͼ  
	% title('���ͼ�ĻҶ�ͼ');
	
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
%ʹ���Դ�����
% [testcaseFiles,testcasePaths] = dfsFolder('D:/�о�����Ŀ/����ʶ��/�г�ʶ��/matlab/exceptionTestcase/temp2/','.bmp');
% ����ͼƬ�ҶȾ��⻯
% for i =1: size(testcaseFiles,2)
% srcImage = imread(testcasePaths{i});
% figure(i);
% subplot(2,3,1);
% imshow(srcImage);
% title(testcaseFiles(1,i));

% subplot(2,3,[2 3]);
% imhist(srcImage);
% title('ԭͼ�Ҷ�ͼ');

% subplot(2,3,4);
% eqImage = histeq(srcImage);
% imshow(eqImage);
% title('�ҶȾ��⻯��ͼ');

% subplot(2,3,[5 6]);
% imhist(eqImage);
% title('���ͼ�ĻҶ�ͼ');

% pause();
% end




