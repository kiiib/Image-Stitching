% Harris Corner Detection
function [featureX, featureY] = HarrisFeature(image, w, sigma, k, threshold)
    % Convert the image into gray image
    dim = ndims(image);
    if(dim == 3)
        % image is RGB
        I = rgb2gray(image);
    else
        % image already is gray image
        I = image;
    end
    
    [row, col] = size(I);
    I_double = double(I);
    %R = zero(row, col);
    
    % Calculate x and y derivatives of image
    IG = filter2(fspecial('gaussian', [w w], sigma), I_double);
    [Ix, Iy] = gradient(IG);
    
    % Calculate products of derivatives at every pixel
    Ix2 = Ix .^ 2;
    Iy2 = Iy .^ 2;
    Ixy = Ix .* Iy;
    
    % Calculate the sums of the products of derivatives at each pixel
    Sx2 = filter2(fspecial('gaussian', [w w], sigma), Ix2);
    Sy2 = filter2(fspecial('gaussian', [w w], sigma), Iy2);
    Sxy = filter2(fspecial('gaussian', [w w], sigma), Ixy);
    
     
    % the reponse of the detector at each pixel 
    R = (Sx2 .* Sy2 - Sxy .^ 2) - k * (Sx2 + Sy2) .^ 2;
    % threshold on value of R
    RT = (R > threshold);
    % computer nomax suppression
    mask = [1 1 1; 1 0 1; 1 1 1];
    RN = RT & (R > imdilate(R, mask));
    
    % do not want features close to the image border
    RN([1:15, end-16:end], :) = 0;
    RN(:, [1:15, end-16:end]) = 0;
    
    [featureY, featureX] = find(RN);
end