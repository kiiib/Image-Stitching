function trans = solveTranslation(featuresMatch,featuresPos1, featuresPos2)
	n = size(featuresMatch, 1);
	A = zeros(n * 2 + 1, 3);
	b = zeros(n * 2 + 1, 1);

	for i = 1 : n
		% put x and x' into matrices
		A(i, 1) = 1;
		A(i, 3) = featuresPos1(featuresMatch(i, 1), 1);
		b(i, 1) = featuresPos2(featuresMatch(i, 2), 1);
    end
    for i = 1 : n
		% put y and y' into matrices
		A(i + n, 2) = 1;
		A(i + n, 3) = featuresPos1(featuresMatch(i, 1), 2);
		b(i + n, 1) = featuresPos2(featuresMatch(i, 2), 2);
    end
    
	A(n * 2 + 1, 3) = 1;
	b(n * 2 + 1, 1) = 1;
    

	% Solve transaltion
	r = round(A \ b);
	trans = r(1 : 2);
    %disp('trans are');
    disp(trans);
end