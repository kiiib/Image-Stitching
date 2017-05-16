function[pos, desc] = descriptor(image, featureX, featureY)
    pos = [];
    desc = [];
    for m = 1 : size(featureY)
        x = featureX(m);
        y = featureY(m);
        pos = [pos; [x, y]];
        featureDescArray = zeros(49, 3);
        
        n = 1;
        for i = -3 : 3
           for j = -3 : 3
               featureDescArray(n, :) = [image(y+i, x+j, 1), image(y+i, x+j, 2), image(y+i, x+j, 3)];
               n = n + 1;
           end
        end
        featureDescArray = reshape(featureDescArray, 1, []);
        desc = [desc; featureDescArray];
    end
end