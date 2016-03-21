function HDRImg = getHDR(images, g, lt, w)
    row = size(images{1}(:,:,1), 1); 
    col = size(images{1}(:,:,1), 2);
    imageNum = size(images, 2);
    HDRlnE = zeros(row, col, 3);
    
    for channel=1:3
        for r=1:row
            for c=1:col
                sumWeightedlE = 0;
                sumWeight = 0;
                for j=1:imageNum
                    tmpZ = images{j}(r,c,channel) + 1;
                    tmpg = g(tmpZ,channel);
                    tmplt = lt(j);
                    sumWeightedlE = sumWeightedlE + w(tmpZ)*(tmpg - tmplt);
                    sumWeight = sumWeight + w(tmpZ);
                end
                HDRlnE(r, c, channel) = exp(sumWeightedlE/sumWeight);
            end
        end
    end
    HDRImg = HDRlnE;
end