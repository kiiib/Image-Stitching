% Cylindrical reprojection
function wrapImage = warpImage(image, focalLength)
    wrapImage = zeros(size(image), 'uint8');
    mid = size(image) / 2;
    
    for y = 1 : size(image, 1)
       for x = 1: size(image, 2)
           xd = x - mid(2); 
           yd = y - mid(1); 
           theta = atan(xd / focalLength);
           h = yd / sqrt(xd^2 + focalLength^2);
           
           xp = round(focalLength * theta) + mid(2);
           yp = round(focalLength * h) + mid(1);
           
           wrapImage(yp, xp, :) = image(y, x, :);
       end
    end
end