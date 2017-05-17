% Random Sample Consensus for avoiding outliner
function maxFeatureMatch = featuresMatching(featuresPos1, featuresDesc1, featuresPos2, featuresDesc2)
    % Find matched features
    matchDesc = [];
    for i = 1 : size(featuresDesc1, 1)
        min1 = [inf 0];
        min2 = [inf 0];

        for j = 1 : size(featuresDesc2, 1)
            distance = sqrt(sum((featuresDesc1(i, :) - featuresDesc2(j, :)) .^2 ));
            if(distance < min1(1))
                min2 = min1;
                min1 = [distance j];
            elseif(distance < min2(1))
                min2 = [distance j];
            end
        end
        % accept the distance ratio which less than 0.8
        if((min1(1) / min2(1)) < 0.8)
            matchDesc = [matchDesc; [i min1(2)]];
        end
    end

    p = 0.5;
    n = 3;
    P = 0.999;
    k = ceil(log(1 - P) / log (1 - P ^ n)) + 100;
    threshold = 10;
    
    N = size(matchDesc, 1);
    maxFeatureMatch = [];
    if(N <= n)
        maxFeatureMatch = matchDesc;
        return;
    end

    % Run k times
    for times = 1 : k
        % 1.Draw n samples from N randomly 
        random = randperm(N);
        sampleIdx = random(1 : n);
        otherIdx = random(n + 1 : N);
        sampleMatch = matchDesc(sampleIdx, :);
        otherMatch = matchDesc(otherIdx, :);
        samplePos1 = featuresPos1(sampleMatch(:, 1), :);
        samplePos2 = featuresPos2(sampleMatch(:, 2), :);
        otherPos1 = featuresPos1(otherMatch(:, 1), :);
        otherPos2 = featuresPos2(otherMatch(:, 2), :);

        % 2.Fit parameter theta with these n samples
        tmpMatch = [];
        posDiff = samplePos1 - samplePos2;
        theta = mean(posDiff);

        % 3. For each other N - n points,
        % calculate its distance to the fitted model,
        % count the number of inliner points, c
        for i = 1 : (N - n)
            distance = (otherPos1(i, :) - otherPos2(i, :)) - theta;
            if norm(distance) < threshold
                tmpMatch = [tmpMatch; otherMatch(i, :)];
            end
        end

        if size(tmpMatch, 1) > size(maxFeatureMatch, 1)
            maxFeatureMatch = tmpMatch;
        end
    end

    disp('descriptor size after match:');
    disp(size(maxFeatureMatch, 1));
end