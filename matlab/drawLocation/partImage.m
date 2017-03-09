close all;
fn = '100ZN.bmp';
p = imread(fn);
I = imresize(p,[256,640]);
rs = size(I, 1); cs = size(I, 2);%取图像的行列数
sz = 32;%设置块的大小
ch = sz; cw = sz;
numr = rs/sz;
numc = cs/sz;
t1 = (0:numr-1)*ch+1 ;
t3 = (0:numc-1)*cw +1;
figure(1);
for i = 1 : numr
    for j = 1 : numc
     I(t1(i),:,:)=255;
     I(:,t3(j),:)=255;
    end
end
imshow(I);

fn = '5ZN.bmp';
p = imread(fn);
I = imresize(p,[256,640]);
rs = size(I, 1); cs = size(I, 2);%取图像的行列数
sz = 32;%设置块的大小
ch = sz; cw = sz;
numr = rs/sz;
numc = cs/sz;
t1 = (0:numr-1)*ch+1 ;
t3 = (0:numc-1)*cw +1;
figure(2);
for i = 1 : numr
    for j = 1 : numc
     I(t1(i),:,:)=255;
     I(:,t3(j),:)=255;
    end
end
imshow(I);