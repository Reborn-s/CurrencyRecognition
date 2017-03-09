function featureArray = computeIntersection(featureArray,l,featureKind,factor,interval,iallData,jallData,location,feature)

idata = iallData.data(l,:);
iaver = iallData.aver(l);
ivaria = iallData.varia(l);
			
jdata = jallData.data(l,:);
javer = jallData.aver(l);
jvaria = jallData.varia(l);

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
		%在两个标准差区间内求交集面积
		if(iaver>javer)
			ihist = histc(idata,interPoint_x:factor*ivaria/interval:interPoint_x+factor*ivaria);
			jhist = histc(jdata,interPoint_x-factor*jvaria:(factor*jvaria)/interval:interPoint_x);
			
			rarea = sum(ihist)*(factor*ivaria)/interval;
			larea = sum(jhist)*(factor*jvaria)/interval;
		else
			ihist = histc(idata,interPoint_x-factor*ivaria:(factor*ivaria)/interval:interPoint_x);
			jhist = histc(jdata,interPoint_x:(factor*jvaria)/interval:interPoint_x+factor*jvaria);
			
			larea = sum(ihist)*(factor*ivaria)/interval;
			rarea = sum(jhist)*(factor*jvaria)/interval;
		end
		area = larea+rarea;	
	%如果交点不在两个函数的均值之间
	else
		t = interPoint_x-factor*ivaria;
		if(feval(@iyf,t,iaver,ivaria)>feval(@jyf,t,javer,jvaria))
			ihist = histc(idata,interPoint_x-factor*ivaria:(factor*ivaria)/interval:interPoint_x);
			jhist = histc(jdata,interPoint_x:(factor*jvaria)/interval:interPoint_x+factor*jvaria);
			
			larea = sum(ihist)*(factor*ivaria)/interval;
			rarea = sum(jhist)*(factor*jvaria)/interval;
		else
			ihist = histc(idata,interPoint_x:factor*ivaria/interval:interPoint_x+factor*ivaria);
			jhist = histc(jdata,interPoint_x-factor*jvaria:(factor*jvaria)/interval:interPoint_x);
			
			rarea = sum(ihist)*(factor*ivaria)/interval;
			larea = sum(jhist)*(factor*jvaria)/interval;
		end
		area = larea+rarea;
	end
		if(area<featureArray(feature).area)
			featureArray(feature).area = area;
			featureArray(feature).location = location;
			featureArray(feature).feature = feature;
		end
	%处理两个交点的情况
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
		larea1 = sum(ihist1)*(factor*ivaria)/interval;
		rarea1 = sum(jhist1)*(factor*jvaria)/interval;
		
		ihist2 = histc(idata,rpoint:factor*ivaria/interval:rpoint+factor*ivaria);
		jhist2 = histc(jdata,rpoint-factor*jvaria:factor*jvaria/interval:rpoint);
		rarea2 = sum(ihist2)*(factor*ivaria)/interval;
		larea2 = sum(jhist2)*(factor*jvaria)/interval;
	else
		ihist1 = histc(idata,lpoint:factor*ivaria/interval:lpoint+factor*ivaria);
		jhist1 = histc(jdata,lpoint-factor*jvaria:factor*jvaria/interval:lpoint);
		rarea1 = sum(ihist1)*(factor*ivaria)/interval;
		larea1 = sum(jhist1)*(factor*jvaria)/interval;
		
		ihist2 = histc(idata,rpoint-factor*ivaria:factor*ivaria/interval:rpoint);
		jhist2 = histc(jdata,rpoint:factor*jvaria/interval:rpoint+factor*jvaria);
		larea2 = sum(ihist2)*(factor*ivaria)/interval;
		rarea2 = sum(jhist2)*(factor*jvaria)/interval;
	end
	area = larea1+rarea1+larea2+rarea2;
	if(area<featureArray(feature).area)
		featureArray(feature).area = area;
		featureArray(feature).location = location;
		featureArray(feature).feature = feature;
	end
%如果没有交点，说明已经分得很开了，那该怎么处理呢
elseif(pointCnt == 0)
	featureArray(feature).area = 0;
	featureArray(feature).location = location;
	featureArray(feature).feature = feature;
end

end