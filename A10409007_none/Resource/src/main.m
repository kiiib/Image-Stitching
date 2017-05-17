function main()
   
    disp('Load images');
    imageTopic = 'green';
    inputPath = ['../input_image/' imageTopic '/']; % input images series' path
    outputPath = ['../result/' imageTopic '_pano.png']; % ouput path and filename
    files = dir([inputPath, '/*.jpg']);
    imageNum = length(files);
    
    disp('Load focal length file');
    focalfile = fopen(['../input_image/' imageTopic '/pano.txt'], 'r');
    focals = fscanf(focalfile, '%f');
    fclose(focalfile);
    
    disp('Do the cylindrical projection to images');
    for i = 1 : imageNum
        %imshow([inputPath, files(i).name]);
        imageName = [inputPath, files(i).name];
        image = imread(imageName);
        %imshow(image);
        disp('finish no.');
        disp(i);
        warppedImage{i} = warpImage(image, focals(i));
        %imshow(warppedImage{i});
    end
    
    disp('Harris corner features detection and descriptor');
    for i = 1 : imageNum
        [featureX, featureY] = HarrisFeature(warppedImage{i}, 5, 1, 0.04, 3);
        disp('key point');
        disp(size(featureX));
        points{i} = [featureX, featureY];
        drawHarrisCorner(warppedImage{i}, points{i});
        [featurePos, featureDescriptor] = descriptor(warppedImage{i}, featureX, featureY);
        featuresPos{i} = featurePos;
        featuresDesc{i} = featureDescriptor;
    end
    drawHarrisCorner(warppedImage{1}, points{i});
    disp('Features matching');
    for i = 1 : imageNum - 1
        featureMatch{i} = RANSAC(featuresPos{i}, featuresDesc{i}, featuresPos{i + 1}, featuresDesc{i + 1});
    end
    
    disp('Images matching');
    for i = 1 : imageNum - 1
        trans{i} = solveTranslation(featureMatch{i}, featuresPos{i}, featuresPos{i + 1});
    end

    disp('Images blending');
    imageOut = warppedImage{1};
    for i = 2 : imageNum
        imageOut = blending(imageOut, warppedImage{i}, trans{i - 1});
    end

    disp('Saving the panorama');
    imshow(imageOut);
    imwrite(imageOut, outputPath);
end