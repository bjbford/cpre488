
bayer = imread('bird_wallpaper.bmp', 'bmp');
%disp(bayer)
%i, j, 1 == red
%i, j, 2 == green
%i, j, 3 == blue

for i = 1:1080
    for j = 1:1920
        
        %horizontal green/red row
        if(mod(i, 2) == 1)
            %green pixel
             if mod(j, 2) == 1
%                 green = bayer(i, j, 2);
%                 bayer(i, j, 1) = green;
%                 bayer(i, j, 3) = green;
                bayer(i, j, 1) = bitand(bayer(i, j, 1), 0);
                bayer(i, j, 3) = bitand(bayer(i, j, 3), 0);
            %red pixel
             else
%                 red = bayer(i, j, 1);
%                 bayer(i, j, 2) = red;
%                 bayer(i, j, 3) = red;
                bayer(i, j, 2) = bitand(bayer(i, j, 2), 0);
                bayer(i, j, 3) = bitand(bayer(i, j, 3), 0);
            end
            
        %horizontal blue/green row
        else
            %blue pixel
             if mod(j, 2) == 1
%                 blue = bayer(i, j, 3);
%                 bayer(i, j, 1) = blue;
%                 bayer(i, j, 2) = blue;
                bayer(i, j, 1) = bitand(bayer(i, j, 1), 0);
                bayer(i, j, 2) = bitand(bayer(i, j, 2), 0);
            %green pixel
             else
%                 green = bayer(i, j, 2);
%                 bayer(i, j, 1) = green;
%                 bayer(i, j, 3) = green;
                bayer(i, j, 1) = bitand(bayer(i, j, 1), 0);
                bayer(i, j, 3) = bitand(bayer(i, j, 3), 0);
            end
        end
    end
end

imwrite(bayer, 'bird_bayer.bmp', 'bmp');
        
yuv = bayer;
red = 0;
green = 0;
blue = 0;

%accounting for OOB
% for j = 1:1920
%     i = 1;
%     if j == 1
%         
%     elseif j == 1920
%         
%     else
%         if mod(j, 2) == 1
%             red = (bayer(i, j + 1, 1) + bayer(i, j - 1, 1)) / 2;
%             green = bayer(i, j, 2);
%             blue = (bayer(i + 1, j, 3) + bayer(i - 1, j, 3)) / 2;
%         %red pixel
%         else
%             red = bayer(i, j, 1);
%             green = (bayer(i, j + 1, 2) + bayer(i, j - 1, 2) + bayer(i + 1, j, 2) + bayer(i - 1, j, 2)) / 4;
%             blue = (bayer(i + 1, j + 1, 3) + bayer(i - 1, j - 1, 3) + bayer(i + 1, j - 1, 3) + bayer(i - 1, j + 1, 3)) / 4;
%         end
%     end
% end
% 
% for j = 1:1920
%     i = 1080;
%     if mod(j, 2) == 1
%         red = (bayer(i, j + 1, 1) + bayer(i, j - 1, 1)) / 2;
%         green = bayer(i, j, 2);
%         blue = (bayer(i + 1, j, 3) + bayer(i - 1, j, 3)) / 2;
%     %red pixel
%     else
%         red = bayer(i, j, 1);
%         green = (bayer(i, j + 1, 2) + bayer(i, j - 1, 2) + bayer(i + 1, j, 2) + bayer(i - 1, j, 2)) / 4;
%         blue = (bayer(i + 1, j + 1, 3) + bayer(i - 1, j - 1, 3) + bayer(i + 1, j - 1, 3) + bayer(i - 1, j + 1, 3)) / 4;
%     end
% end
% 
% for i = 2:1079
%     for k = 1:2
%         if k == 1
%             j = 1;
%         else
%             j = 1080;
%         end
%         
%     end
% end

%normal loop
for i = 2:1079
    for j = 2:1919
            
        %horizontal green/red row
        if mod(i, 2) == 1
            %green pixel
             if mod(j, 2) == 1
                 red = (bayer(i, j + 1, 1) + bayer(i, j - 1, 1)) / 2;
                 green = bayer(i, j, 2);
                 blue = (bayer(i + 1, j, 3) + bayer(i - 1, j, 3)) / 2;
            %red pixel
             else
                 red = bayer(i, j, 1);
                 green = (bayer(i, j + 1, 2) + bayer(i, j - 1, 2) + bayer(i + 1, j, 2) + bayer(i - 1, j, 2)) / 4;
                 blue = (bayer(i + 1, j + 1, 3) + bayer(i - 1, j - 1, 3) + bayer(i + 1, j - 1, 3) + bayer(i - 1, j + 1, 3)) / 4;
            end
            
        %horizontal blue/green row
        else
            %blue pixel
             if mod(j, 2) == 1
                 red = (bayer(i + 1, j + 1, 1) + bayer(i - 1, j - 1, 1) + bayer(i + 1, j - 1, 1) + bayer(i - 1, j + 1, 1)) / 4;
                 green = (bayer(i, j + 1, 2) + bayer(i, j - 1, 2) + bayer(i + 1, j, 2) + bayer(i - 1, j, 2)) / 4;
                 blue = bayer(i, j, 3);
            %green pixel
             else
                 red = (bayer(i + 1, j, 1) + bayer(i - 1, j, 1)) / 2;
                 green = bayer(i, j, 2);
                 blue = (bayer(i, j + 1, 3) + bayer(i, j - 1, 3)) / 2;     
            end
        end
        rgb = double([red; green; blue]);
        temp = [0.183 0.614 0.062; -0.101 -0.338 0.439; 0.439 -0.399 -0.04];
        yuv(i, j, :) = uint8((temp * rgb) + ([16; 128; 128]));
%         yuv(i, j, 1) = yuv_vec(1);
%         yuv(i, j, 2) = yuv_vec(2);
%         yuv(i, j, 3) = yuv_vec(3);
%         yuv(i, j, 1) = 16 + ((0.183 * red) + (128.553 * green) + (24.966 * blue));
%         yuv(i, j, 2) = 128 + ((-0.101 * red) - (74.203 * green) + (112.0 * blue));
%         yuv(i, j, 3) = 128 + ((0.439 * red) - (93.786 * green) - (18.214 * blue));
    end
end
imwrite(yuv, 'bird_yuv.jpg');
imshow(yuv);
