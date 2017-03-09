function [dest] = drawRect(src,pt,wSize,lineSize,color,flag)

if nargin < 5
    color = [255 255 0];
end

if nargin < 4
    lineSize = 1;
end

if nargin < 3
    disp('输入参数不够 !!!');
    return;
end

%判断框的边界问题
[yA, xA, z] = size(src);
x1 = pt(1)
y1 = pt(2)
wx = wSize(1);
wy = wSize(2);
if  x1>xA || ...
        y1>yA||...
        (x1+wx)>xA||...
        (y1+wy)>yA

    disp('画的框将超过图像 !!!');
    return;
end

%如果是单通道的灰度图，转成3通道的图像
if 1==z
    dest(:, : ,1) = src;
    dest(:, : ,2) = src;
    dest(:, : ,3) = src;
else
    dest = src;
end

%开始画框图
for c = 1 : 3                 %3个通道，r，g，b分别画
    for dl = 1 : lineSize   %线的宽度，线条是向外面扩展的
        d = dl - 1;
        if  1==flag %有缺口的框
            dest(  y1-d ,            x1:(x1+wx) ,  c  ) =  color(c); %上方线条
            dest(  y1+wy+d ,     x1:(x1+wx) , c  ) =  color(c); %下方线条
            dest(  y1:(y1+wy) ,   x1-d ,           c  ) =  color(c); %左方线条
            dest(  y1:(y1+wy) ,   x1+wx+d ,    c  ) =  color(c); %左方线条
        elseif 2==flag %无缺口的框
            dest(  y1-d ,            (x1-d):(x1+wx+d) ,  c  ) =  color(c); %上方线条
            dest(  y1+wy+d ,    (x1-d):(x1+wx+d) ,  c  ) =  color(c); %下方线条
            dest(  (y1-d):(y1+wy+d) ,   x1-d ,           c  ) =  color(c); %左方线条
            dest(  (y1-d):(y1+wy+d) ,   x1+wx+d ,    c  ) =  color(c); %左方线条
        end
    end    
end %主循环尾


end %函数尾
