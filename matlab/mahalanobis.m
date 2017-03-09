%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   author: reborn
%   date:   2016/10/17
%   descr:  calculate the mahalanobis distance between two matrix 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function d = mahalanobis(x,y)

for i = 1:size(x,1)
	for j = 1:size(y,1)
		a = x(i,:);
		b = y(j,:);
		c = cov(x);
		d(i,j) = sqrt((a-b)*pinv(c)*(a-b)');
	end
end
