four0s = [0;0;0;0];
a = [1;2;3;4];
b = [2;-3;4;5];
c = [-3;3;3;3];
hold off
quiver3(0,0,0,1,2,4,'color','red');
hold on;
quiver3(0,0,0,1,2,3,'color','blue');
%quiver3(four0s,four0s,four0s,a,b,c,['color','red';'color','red';'color','blue';'color','red']);
view(-35,45)
rotate3d('on')

trajectory(0,0,0,0,0,0,10,.1)
trajectory(0,0,1,0,0,0,10,.1)




