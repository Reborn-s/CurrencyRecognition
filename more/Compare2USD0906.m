%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   author: reborn
%   date:   2016/8/6
%   descr:  compare two funtions of the same feature for two kinds of USD
%			相交面积的宽度修改为[u1-sigma1,u2+sigma2]
%			在算法中添加处理黑边的代码，增强鲁棒性：遇到代表黑边的异常数据不进行处理跳过这个位置
%			将美金种类增加为九种，因为5和100分别有两种
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% function ans = Compare2USD()
% ans = allMinLocation;

tic;
close all;                        %清空工作区，清空命令区域，关闭所有窗口
clear all;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 

USDKind = 5;
featureKind = 5;
titleArray = {'能量';'熵值';'对比度';'逆差矩';'相关性'};
dollarArray = {'5';'10';'20';'50';'100'};
dataPath = '..\newall\f5d1\ZN\';
coupleCnt = 1;
interval = 100;
factor = 1;
exceptionValue = 100.0;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%开始处理%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fo = fopen('每对美金特征值相差最大的位置f5d1.txt','wt');
fo1 = fopen('minLocation_f5d1.txt','wt');
fprintf(fo,'%s\t%s\t%s\t%s\t%s\n','美金1','美金2','特征参数','最小位子','相交面积');

for i = 1: USDKind-1
	for j = i+1: USDKind
		disp(dollarArray{i});
		disp(dollarArray{j});
		
		feature = 1;
		location = 1;
		separatedLocation.location = -1;
		
		for f = 1:featureKind
			featureArray(f).area = 100;
			featureArray(f).location = -1;
			featureArray(f).feature = -1;
			featureArray(f).couple = {dollarArray{i};dollarArray{j}};
		end
		
		fi = fopen([dataPath 'newall_' dollarArray{i} 'ZN.txt'],'rt');
		fj = fopen([dataPath 'newall_' dollarArray{j} 'ZN.txt'],'rt');
		while (~feof(fi))&& (~feof(fj))
			iline = fgetl(fi);
			jline = fgetl(fj);
			if (isempty(iline)&&isempty(jline))		%空行表示新的位置
				location = location+1
				feature = 1;
			else
				iline = str2num(iline);
				idata = [];
				idata = [idata;iline];
				jline = str2num(jline);
				jdata = [];
				jdata = [jdata;jline];
				
				if(ismember(exceptionValue,idata)||ismember(exceptionValue,jdata))
					continue;
				end
				
				iaver = mean(idata);
				ivaria = std(idata);
				javer = mean(jdata);
				jvaria = std(jdata);
				
				close all;
				x = -1:0.000001:2;
				y1=normpdf(x,iaver,ivaria);
				y2=normpdf(x,javer,jvaria);
				plot(x,y1,x,y2);
				
				syms x1;
				iy = normpdf(x1,iaver,ivaria);
				jy = normpdf(x1,javer,jvaria);
				
				if(iaver>javer)
					xl = min(iaver-ivaria,javer+jvaria);
					xr = max(iaver-ivaria,javer+jvaria);
				else
					xl = min(iaver+ivaria,javer-jvaria);
					xr = max(iaver+ivaria,javer-jvaria);
				end
				interPoint_xarray = double(solve(iy-jy));
				pointCnt = 0;
				for p = 1:size(interPoint_xarray)
					if(interPoint_xarray(p)<xr&&interPoint_xarray(p)>xl)
						interPoint_x = interPoint_xarray(p);
						pointCnt = pointCnt+1;
					end
				end
				
				area = 0.0;
				if(pointCnt==1)
					flag = 1
					larea = 0.0;
					rarea = 0.0;
					if(feval(@iyf,xl,iaver,ivaria)>feval(@jyf,xl,javer,jvaria))
						ihist = histc(idata,xl:(factor*ivaria)/interval:xr);
						jhist = histc(jdata,xl:(factor*jvaria)/interval:xr);
						for a = 1:size(ihist,2)
							larea = larea+ ihist(a)*(factor*ivaria)/interval;
						end
						for b = 1:size(jhist,2)
							rarea = rarea+ jhist(b)*(factor*jvaria)/interval;
						end
					else
						ihist = histc(idata,xl:(factor*ivaria)/interval:xr);
						jhist = histc(jdata,xl:(factor*jvaria)/interval:xr);
						for a = 1:size(ihist,2)
							rarea = rarea+ ihist(a)*(factor*ivaria)/interval;
						end
						for b = 1:size(jhist,2)
							larea = larea+ jhist(b)*(factor*jvaria)/interval;
						end
					end
					area = larea+rarea;
					if(area<featureArray(feature).area)
						featureArray(feature).area = area;
						featureArray(feature).location = location;
						featureArray(feature).feature = feature;
					end
						
				%处理两个交点的情况
				elseif(pointCnt == 2)
					flag = 2
					larea1 = 0.0;
					rarea1 = 0.0;
					larea2 = 0.0;
					rarea2 = 0.0;
					lpoint = min(interPoint_xarray);
					rpoint = max(interPoint_xarray);
					t1 = lpoint-ivaria;
					if(feval(@iyf,t1,iaver,ivaria)>feval(@jyf,t1,javer,jvaria))
						ihist1 = histc(idata,lpoint-factor*ivaria:factor*ivaria/interval:lpoint);
						jhist1 = histc(jdata,lpoint:factor*jvaria/interval:lpoint+factor*jvaria);
						for a = 1:size(ihist1,2)
							larea1 = larea1+ihist1(a)*(factor*ivaria)/interval;
						end
						for b = 1:size(jhist1,2)
							rarea1 = rarea1+jhist1(b)*(factor*jvaria)/interval;
						end
						ihist2 = histc(idata,rpoint:factor*ivaria/interval:rpoint+factor*ivaria);
						jhist2 = histc(jdata,rpoint-factor*jvaria:factor*jvaria/interval:rpoint);
						for a = 1:size(ihist2,2)
							rarea2 = rarea2+ihist2(a)*(factor*ivaria)/interval;
						end
						for b = 1:size(jhist2,2)
							larea2 = larea2+jhist2(b)*(factor*jvaria)/interval;
						end
					else
						ihist1 = histc(idata,lpoint:factor*ivaria/interval:lpoint+factor*ivaria);
						jhist1 = histc(jdata,lpoint-factor*jvaria:factor*jvaria/interval:lpoint);
						for a = 1:size(ihist1,2)
							rarea1 = rarea1+ihist1(a)*(factor*ivaria)/interval;
						end
						for b = 1:size(jhist1,2)
							larea1 = larea1+jhist1(b)*(factor*jvaria)/interval;
						end
						ihist2 = histc(idata,rpoint-factor*ivaria:factor*ivaria/interval:rpoint);
						jhist2 = histc(jdata,rpoint:factor*jvaria/interval:rpoint+factor*jvaria);
						for a = 1:size(ihist2,2)
							larea2 = larea2+ihist2(a)*(factor*ivaria)/interval;
						end
						for b = 1:size(jhist2,2)
							rarea2 = rarea2+jhist2(b)*(factor*jvaria)/interval;
						end
					end
					area = larea1+rarea1+larea2+rarea2;
					if(area<featureArray(feature).area)
						featureArray(feature).area = area;
						featureArray(feature).location = location;
						featureArray(feature).feature = feature;
					end
				%如果没有交点
				elseif(pointCnt == 0)
					flag = 0
					if(feval(@iyf,xl,iaver,ivaria)>feval(@jyf,xl,javer,jvaria))
						ihist = histc(idata,xl:(factor*ivaria)/interval:xr);
						for a = 1:size(ihist,2)
							area = area+ihist(a)*(factor*ivaria)/interval;
						end
					else
						jhist = histc(jdata,xl:(factor*jvaria)/interval:xr);
						for a = 1:size(jhist,2)
							area = area+jhist(a)*(factor*jvaria)/interval;
						end
					end
					if(area<featureArray(feature).area)
						featureArray(feature).area = area;
						featureArray(feature).location = location;
						featureArray(feature).feature = feature;
					end
				end
				feature = feature+1;
			end
			area = area
			pause();
			%break;
		end	
		% allMinLocation(coupleCnt).area = minLocation.area;
		% allMinLocation(coupleCnt).location = minLocation.location;
		% allMinLocation(coupleCnt).couple = minLocation.couple;
		% allMinLocation(coupleCnt).feature = minLocation.feature;
		fprintf(fo,'%s\t\t%s\t\t',char(featureArray(f).couple{1}),char(featureArray(f).couple{2}));
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
		fclose(fi);
		fclose(fj);
		coupleCnt= coupleCnt+1;
		%pause();
		%break;
	end
	%break;
end

fclose(fo);
fclose(fo1);

toc;
					

