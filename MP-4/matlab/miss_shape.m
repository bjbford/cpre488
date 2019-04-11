%the following function is a remake of the matlab function
function [x,y,z]=miss_shape(ss)
	num=30;				% Number of z-y segments to make the circle
	count=1;			% Counter for number of individual patches
	theta=[360/2/num:360/num:(360+360/2/num)]*pi/180;
	len 			= 25.7;				% Total Length (no units)
    
	radius		= 1.5/2;				% Radius of body
 	s_fore	 	= 5;					% Start of main body (w.r.t. nose)
   	thr_len 		= 1.4;				% Length of Motor exit
 	rad_throt	= 1.3/2;				% radius of motor exit
   	l_fore=len-s_fore-thr_len;			% Length of main body
   	c_g 			= 14;				% Position of c.g. w.r.t nose
   
%
% Fore Body Shape
%
	yc_range =  radius*sin(theta);
	zc_range = -radius*cos(theta);
	for i = 1:num
	   xcraft{i}=[s_fore s_fore s_fore+l_fore s_fore+l_fore ]-c_g;
	   ycraft{i}=[yc_range(i) yc_range(i+1) yc_range(i+1) yc_range(i)];
	   zcraft{i}=[zc_range(i) zc_range(i+1) zc_range(i+1) zc_range(i)];
	end
   count=num+1;   
%
% Throttle Shape
%
	yc_range2 =  rad_throt*sin(theta);axis;
	zc_range2 = -rad_throt*cos(theta);
	for i = 1:num
	   xcraft{count}=[len-thr_len len-thr_len len len]-c_g;
	   ycraft{count}=[yc_range(i) yc_range(i+1) yc_range2(i+1) yc_range2(i)];
      zcraft{count}=[zc_range(i) zc_range(i+1) zc_range2(i+1) zc_range2(i)];
      count=count+1;
	end
%
% Nose Shape
%
	for i = 1:num
	   xcraft{count}=[s_fore s_fore 0 s_fore]-c_g;
	   ycraft{count}=[yc_range(i) yc_range(i+1) 0 yc_range(i)];
      zcraft{count}=[zc_range(i) zc_range(i+1) 0 zc_range(i)];
      count=count+1;
	end
%
% Wing shapes 
%
	xcraft{count}=[10.2 13.6 14.6 15]-c_g;
	ycraft{count}=[-zc_range(1) -zc_range(1)+3 -zc_range(1)+3 -zc_range(1)];
	zcraft{count}=[0 0 0 0 ];
	xcraft{count+1}=xcraft{count};
	ycraft{count+1}=-ycraft{count};
	zcraft{count+1}=zcraft{count};
% 	xcraft{count+2}=xcraft{count};
% 	ycraft{count+2}=zcraft{count};
% 	zcraft{count+2}=ycraft{count};
% 	xcraft{count+3}=xcraft{count};
% 	ycraft{count+3}=zcraft{count};
% 	zcraft{count+3}=-ycraft{count};
%
% Tail shapes 
%
   count=count+2;
	xcraft{count}=[21 22.9 24 24]-c_g;
	ycraft{count}=[-zc_range(1) -zc_range(1)*4 -zc_range(1)*4 -zc_range(1)];
	zcraft{count}=[0 0 0 0];
  	xcraft{count+1}=xcraft{count};
	ycraft{count+1}=-ycraft{count};
	zcraft{count+1}=zcraft{count};
	xcraft{count+2}=xcraft{count};
	ycraft{count+2}=zcraft{count};
	zcraft{count+2}=ycraft{count};
% 	xcraft{count+3}=xcraft{count};
% 	ycraft{count+3}=zcraft{count};
% 	zcraft{count+3}=-ycraft{count};
   	count=count+2;
      
%
% Combine individual objects into a single set of co-ordinates and roll through 45 degrees
%
   x=[];y=[];z=[];
   roll = [1 0 0;0 cos(0/180*pi) sin(0/180*pi);0 -sin(0/180*pi) cos(0/180*pi)];
   for i = 1:count
      x = [x xcraft{i}'];
	  y = [y ycraft{i}'];
	  z = [z zcraft{i}'];
	end
   
   for i =1:4
      dum = [x(i,:);y(i,:);z(i,:)];
      dum = roll*dum;
      x(i,:)=dum(1,:);
      y(i,:)=dum(2,:);
      z(i,:)=dum(3,:);
   end
%
% Rescale vertices
%
% x = -x/len;
% 	y = y/len;
%    	z = z/len;
   
x = -x/ss;
y = y/ss;
    	z = z/ss;
   
% End miss_shape
