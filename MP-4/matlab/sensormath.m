try
    s = serial('COM7');
    set(s,'BaudRate',115200);
    fopen(s);
catch ME
    fclose(s)
 
end
%fprintf(s,'*IDN?')
count = 0;
roll_gyr =0;
pitch_gyr=0;
yaw_gyr = 0;
roll_k = 0;
pitch_k  =0;
yaw_k = 0;
a = .35;
while(count < 100000)
    raw = fgetl(s);
    %raw = '10,20,10,.1,.2,0.1,\n';
    if (strlength(raw) > 6)
        %fprintf('\n%d:%s',count,raw);
        values = strsplit(raw,',');
        values = str2double(values)
        if (length(values) > 5)
            x = values(1)/(2^12);
            y = values(2)/(2^12);
            z = -values(3)/(2^12);
            %fprintf('\nx: %d\ty: %d\tz: %d',x,y,z);
            roll_acc = atan(x/(y^2+z^2));
            roll_gyr = roll_gyr - values(5) *.00061035;%*.020 * (2000 / 2^16);
            pitch_acc = atan(y/(x^2+z^2));
            pitch_gyr = pitch_gyr + values(4) *.00061035;
            %fprintf('\nroll: %d\tpitch: %d',roll_acc,pitch_acc);
            yaw_gyr = yaw_gyr - values(6)  *.00061035;
            count=count+1;
%             for  i = 1:3
%                 if(values(i) > 3000)
%                     values(i) = values(i) - 64000;
%                 end
%                 values(i) = values(i) * 8/(2^16);
%             end
%             for  i = 4:6
%                 if(values(i) > 3000)
%                     values(i) = values(i) - 64000;
%                 end
%                 values(i) = values(i) * 2000/(2^16);
%             end
            roll_k = a*roll_k+(1-a)*roll_acc-a*values(5)*.00061035;
            pitch_k = a*pitch_k+(1-a)*pitch_acc+a*values(4)*.00061035;
            yaw_k = yaw_k-a*values(6)*.00061035;
            hold off
            %quiver3(0,0,0,roll_acc,pitch_acc,yaw_gyr,'color','red');
            if(count == 1)
                f1 = figure('Name', 'Kalman Output');
                f2 = figure('Name', 'Accelerometer (x,y), Gyro (z)');
                f3 = figure('Name', 'Gyroscope');
            end
            figure(f1)
            trajectory(0,0,2,pitch_k,roll_k,yaw_k,10,.5)
            hold off
            figure(f2)
            trajectory(0,0,0,pitch_acc,roll_acc,yaw_gyr,10,.5)
            hold off
            figure(f3)
            trajectory(0,0,1,pitch_gyr,roll_gyr,yaw_gyr,10,.5)
            hold off
        end
    end
    pause(.001);
end 


fclose(s)
