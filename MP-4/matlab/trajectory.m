function trajectory(x,y,z,pitch,roll,yaw,scale_factor,step)
%   function traiettoria(x,y,z,pitch,roll,yaw,scale_factor,step)
%   
%
%   x,y,z               center trajectory (vector)    [m]
%   pitch,roll,yaw      euler's angles                [rad]
%   scale_factor        normalization factor
%   step                attitude sampling factor   
%
%   *******************************
%   Function Vesrion 1.0 
%   17/02/2004
%   Valerio Scordamaglia
%   *******************************
if (length(x)~=length(y))||(length(x)~=length(z))||(length(y)~=length(z))
    disp('  Error:');
    disp('      Uncorrect Dimension of the center trajectory Vectors. Please Check the size');
    return;
end
if ((length(pitch)~=length(roll))||(length(pitch)~=length(yaw))||(length(roll)~=length(yaw)))
    disp('  Error:');
    disp('      Uncorrect Dimension of the euler''s angle Vectors. Please Check the size');
    return;
end
if length(pitch)~=length(x)
    disp('  Error:');
    disp('      Size mismatch between euler''s angle vectors and center trajectory vectors');
    return
end
if step>=length(x)
    disp('  Error:');
    disp('      Attitude samplig factor out of range. Reduce step');
    return
end
if step<1
    step=1;
end
[xxx,yyy,zzz]=miss_shape(scale_factor);
ii=length(x);
resto=mod(ii,step);
for i=1:step:(ii-resto)
  
theta=pitch(i);
phi=roll(i);
psi=yaw(i);
Tbe=[cos(psi)*cos(theta) sin(psi)*cos(theta) -sin(theta);((cos(psi)*sin(theta)*sin(phi))-(sin(psi)*cos(phi))) ((sin(psi)*sin(theta)*sin(phi))+(cos(psi)*cos(phi))) cos(theta)*sin(phi);((cos(psi)*sin(theta)*cos(phi))+(sin(psi)*sin(phi))) ((sin(psi)*sin(theta)*cos(phi))-(cos(psi)*sin(phi))) cos(theta)*cos(phi)];
Tbe=Tbe';
x_hat=0.*xxx;
y_hat=0.*yyy;
z_hat=0.*zzz;
for iii=1:size(xxx,1)
    for jjj=1:size(xxx,2)
        tmp_b=[xxx(iii,jjj);yyy(iii,jjj);zzz(iii,jjj)];
        tmp_e=Tbe'*tmp_b;
        x_hat(iii,jjj)=x(i)+tmp_e(1,1);
        y_hat(iii,jjj)=y(i)+tmp_e(2,1);
        z_hat(iii,jjj)=z(i)+tmp_e(3,1);
    end
end
plot3(x_hat,y_hat,z_hat);
%hold on;
patch(x_hat,y_hat,z_hat,[1 0 0]);
end
axis equal;
%grid off
hold on;
plot3(x,y,z);
%rotate3d on
%light;
grid on;
%view(82.50,2);
xlabel('X (m)');
ylabel('Y (m)');
zlabel('Z (m)');
