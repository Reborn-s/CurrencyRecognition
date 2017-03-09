%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   author: reborn
%   date:   2016/10/22
%   descr:  compare two funtions of the same feature for two kinds of USD
%			�ཻ������Ϊ[����-factor*sigma1,����+factor*sigma2]
%			�Ż��㷨��ʹ��ֱ�ӽ����ݶ����ڴ棬���ö��ļ���ȥ���ڱߴ�����롣
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% function ans = Compare2USD()
% ans = allMinLocation;

tic;
close all;                        %��չ�����������������򣬹ر����д���
clear all;
fclose('all');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 

USDKind = 5;

featureStart = 6;
featureEnd = 10;
featureKind = featureEnd-featureStart+1;
totalFeatureKind = 10;

titleArray = {'����';'��ֵ';'�Աȶ�';'����';'�����'};
%titleArray = {'����0';'��ֵ0';'�Աȶ�0';'����0';'�����0';'����90';'��ֵ90';'�Աȶ�90';'����90';'�����90'};
dollarArray = {'5';'10';'20';'50';'100'};
dataPath = '..\newall\f10d1\ZN\';
outputPath = './recognitionResult/f10d1_90/ZN/'
% dataPath = '..\newall\f5d1\grayEqualization\ZN\';
% outputPath = './recognitionResult/f5d1_ori/grayEqualization/ZN/'
coupleCnt = 1;
interval = 100;
%factor = 1;
factor = 1;
exceptionValue = 100.0;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%��ʼ����%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fo = fopen([outputPath 'ÿ����������ֵ������λ��f10d1_1factor.txt'],'wt');
fo1 = fopen([outputPath 'minLocation_f10d1_1factor.txt'],'wt');
fprintf(fo,'%s\t%s\t%s\t%s\t%s\n','����1','����2','��������','��Сλ��','�ཻ���');

for i = 1: USDKind-1
	for j = i+1: USDKind
		disp(dollarArray{i});
		disp(dollarArray{j});

		separatedLocation.location = -1;
		
		for f = 1:featureKind
			featureArray(f).area = 100;
			featureArray(f).location = -1;
			featureArray(f).feature = -1;
			featureArray(f).couple = {dollarArray{i};dollarArray{j}};
		end
		
		fi  = importdata([dataPath 'newall_' dollarArray{i} 'ZN.txt']);
		fj = importdata([dataPath 'newall_' dollarArray{j} 'ZN.txt']);
		
		line = featureStart;
		feature = 1;
		location = 1;
		while(line<=size(fi,1)&&line<=size(fj,1))
			if(line~=1&&mod(line,totalFeatureKind)==1)
				location = location+1
				feature = 1;
				line = line+featureStart-1;
			end
			
			idata = fi(line,:);
			jdata = fj(line,:);
			iaver = mean(idata);
			javer = mean(jdata);
			ivaria = std(idata);
			jvaria = std(jdata);
			
			% x = -1:0.000001:2;
			% y1=normpdf(x,iaver,ivaria);
			% y2=normpdf(x,javer,jvaria);
			% plot(x,y1,x,y2);
			
			syms x1;
			iy = normpdf(x1,iaver,ivaria);
			jy = normpdf(x1,javer,jvaria);
				
			interPoint_xarray = double(solve(iy-jy));
			pointCnt = 0;
			for p = 1:size(interPoint_xarray)
				if(interPoint_xarray(p)<max(max(idata),max(jdata))&&interPoint_xarray(p)>min(min(idata),min(jdata)))
					interPoint_x = interPoint_xarray(p);
					pointCnt = pointCnt+1;
				end
			end
				
			area = 0.0;
			if(pointCnt==1)
				larea = 0.0;
				rarea = 0.0;
				if((interPoint_x>=factor*iaver&&interPoint_x<=factor*javer)||(interPoint_x>=factor*javer&&interPoint_x<=factor*iaver))
					%��������׼���������󽻼����
					if(iaver>javer)
						ihist = histc(idata,interPoint_x:factor*ivaria/interval:interPoint_x+factor*ivaria);
						jhist = histc(jdata,interPoint_x-factor*jvaria:(factor*jvaria)/interval:interPoint_x);
						for a = 1:size(ihist,2)
							rarea = rarea+ ihist(a)*(factor*ivaria)/interval;
						end
						for b = 1:size(jhist,2)
							larea = larea+ jhist(b)*(factor*jvaria)/interval;
						end
					else
						ihist = histc(idata,interPoint_x-factor*ivaria:(factor*ivaria)/interval:interPoint_x);
						jhist = histc(jdata,interPoint_x:(factor*jvaria)/interval:interPoint_x+factor*jvaria);
						for a = 1:size(ihist,2)
							larea = larea+ ihist(a)*(factor*ivaria)/interval;
						end
						for b = 1:size(jhist,2)
							rarea = rarea+ jhist(b)*(factor*jvaria)/interval;
						end
					end
					area = larea+rarea;	
				%������㲻�����������ľ�ֵ֮��
				else
					t = interPoint_x-factor*ivaria;
					if(feval(@iyf,t,iaver,ivaria)>feval(@jyf,t,javer,jvaria))
						ihist = histc(idata,interPoint_x-factor*ivaria:(factor*ivaria)/interval:interPoint_x);
						jhist = histc(jdata,interPoint_x:(factor*jvaria)/interval:interPoint_x+factor*jvaria);
						for a = 1:size(ihist,2)
							larea = larea+ ihist(a)*(factor*ivaria)/interval;
						end
						for b = 1:size(jhist,2)
							rarea = rarea+ jhist(b)*(factor*jvaria)/interval;
						end
					else
						ihist = histc(idata,interPoint_x:factor*ivaria/interval:interPoint_x+factor*ivaria);
						jhist = histc(jdata,interPoint_x-factor*jvaria:(factor*jvaria)/interval:interPoint_x);
						for a = 1:size(ihist,2)
							rarea = rarea+ ihist(a)*(factor*ivaria)/interval;
						end
						for b = 1:size(jhist,2)
							larea = larea+ jhist(b)*(factor*jvaria)/interval;
						end
					end
					area = larea+rarea;
				end
				if(area<featureArray(feature).area)
					featureArray(feature).area = area;
					featureArray(feature).location = location;
					featureArray(feature).feature = feature;
				end
			%����������������
			elseif(pointCnt == 2)
				larea1 = 0.0;
				rarea1 = 0.0;
				larea2 = 0.0;
				rarea2 = 0.0;
				lpoint = min(interPoint_xarray);
				rpoint = max(interPoint_xarray);
				t1 = lpoint-factor*ivaria;
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
			%���û�н��㣬˵���Ѿ��ֵúܿ��ˣ��Ǹ���ô������
			elseif(pointCnt == 0)
				featureArray(feature).area = 0;
				featureArray(feature).location = location;
				featureArray(feature).feature = feature;
					
				figure(j);
				max(idata)
				max(jdata)
				min(idata)
				min(jdata)
					
				disp(iaver);
				disp(javer);
				disp(pointCnt);
				interPoint_xarray
				x = -5:0.000001:5;
				y1=normpdf(x,iaver,ivaria);
				y2=normpdf(x,javer,jvaria);
				plot(x,y1,x,y2);
				pause();
			end
			line = line+1;
			feature = feature+1;
		end
		
		fprintf(fo,'%s\t%s\t',char(featureArray(f).couple{1}),char(featureArray(f).couple{2}));
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
	%break;
end

fclose(fo);
fclose(fo1);
fclose('all');

toc;
					

