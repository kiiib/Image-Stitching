function imageOutput = blending(image1, image2, trans)
    [row1, col1, channel] = size(image1);
	[row2, col2, channel] = size(image2);
	imageOutput = zeros(row1 + abs(trans(2)), col1 + abs(trans(1)), channel, 'uint8');
    
    blendWidth = col2 - trans(1);
    
    alphaLayer1 = ones(row1, col1); 
    alphaLayer2 = ones(row2, col2);
    
    for i = 1 : row2
        alphaLayer2(i, (end - blendWidth):end) = 1:(-1 / blendWidth):0;
        for j = 1 : col2
            imageOutput(i, j, :) = image2(i, j, :) * alphaLayer2(i, j);
        end
    end
    
    for i = 1 : row1
        alphaLayer1(i, 1:blendWidth + 1) = 0:(1 / blendWidth):1;
        for j = 1 : col1
            tmpImage = imageOutput(i + abs(trans(2)), j + abs(trans(1)), :);
            imageOutput(i + abs(trans(2)), j + abs(trans(1)), :) = tmpImage + image1(i, j, :) * alphaLayer1(i, j);
        end
    end
    imshow(imageOutput);

end
