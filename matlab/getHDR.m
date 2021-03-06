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
                
                exclude = zeros(1,imageNum);
                tmpSum = 0;
                for j=1:imageNum
                    tmpE = (g(images{j}(r,c,channel) + 1) - lt(j))*w(images{j}(r,c,channel) + 1);
                    tmpSum = tmpSum + tmpE;
                end
               
                avg = tmpSum/imageNum;
               
                for j=1:imageNum
                    tmpE = (g(images{j}(r,c,channel) + 1) - lt(j))*w(images{j}(r,c,channel) + 1);
                    removeOneAvg = (tmpSum - tmpE)/(imageNum - 1);
                    if abs(removeOneAvg - avg)/avg > 0.2
                        exclude(j) = 1;
                    end
                end
                
                for j=1:imageNum
                    if exclude(j) == 0
                        tmpZ = images{j}(r,c,channel) + 1;
                        tmpg = g(tmpZ,channel);
                        tmplt = lt(j);
                        sumWeightedlE = sumWeightedlE + w(tmpZ)*(tmpg - tmplt);
                        sumWeight = sumWeight + w(tmpZ);
                    end
                end
                if sumWeight == 0 && sumWeightedlE == 0
                    for j=1:imageNum
                        tmpZ = images{j}(r,c,channel) + 1;
                        tmpg = g(tmpZ,channel);
                        tmplt = lt(j);
                        sumWeightedlE = sumWeightedlE + w(tmpZ)*(tmpg - tmplt);
                        sumWeight = sumWeight + w(tmpZ);
                    end
                end
                HDRlnE(r, c, channel) = exp(sumWeightedlE/sumWeight);
            end
        end
    end
    HDRImg = HDRlnE;
end