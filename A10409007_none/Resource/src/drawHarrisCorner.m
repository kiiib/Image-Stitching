function drawHarrisCorner(image, points)
    X = points(:, 2);
    Y = points(:, 1);
    imshow(image);
    hold on
    plot(X, Y, 'b*');
    set(gcf, 'Color', 'w');
end