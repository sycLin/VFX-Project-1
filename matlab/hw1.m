

function hw1(ImgDir, imagePrefix, startingNumber, totalNumber, extension, exposurePath)
    for i=1:str2num(totalNumber)
        Img{i} = imread([ImgDir, imagePrefix, num2str(i+str2num(startingNumber)-1), extension]); %'./aligned/SAM_00'
    end
    
    for g=1:3
        G{g} = [];
        for j=1:size(Img,2)
            tmp = [];
            for r=1:size(Img{j}(:,:,g), 1)
                for c=1:size(Img{j}(:,:,g), 2)
                    tmp = cat(1, tmp, Img{j}(r,c,g)); % [] r*c length vertical array for image j
                end
            end
            G{g} = cat(2, G{g}, tmp);
        end
    end
     
    for g=1:3
        Z{g} = [];
        for i=1:50
            r = round(rand(1)*(size(G{g},1)-1)) + 1;
            Z{g} = cat(1, Z{g}, G{g}(r,:));
        end
    end
    
    w = weighted();
    w = w/max(w);
    lt = B(exposurePath);
    g = zeros(256, 3);
    lE = zeros(50, 3);
    
    for i=1:3
        [g(:,i), lE(:,i)] = gSolve(Z{i}, lt, 100, w);
    end
     
    y = linspace(1,256,256);
    x1 = g(y,1);
    x2 = g(y,2);
    x3 = g(y,3);
    figure;
    plot(x1, y, 'r', x2, y, 'g', x3, y, 'b');
    savefig('mapping.fig');
    
    
    img = getHDR(Img, g, lt, w);
%     hdrwrite(img,'test.hdr')
    rgbeImg = getRGBE(img);
    writeRGBE(rgbeImg, 'result.rgbe');
    writeHDR(rgbeImg, ['result','.hdr']);
    hdr = hdrread('result.hdr');
    rgb = tonemap(hdr,'AdjustLightness',[0.1 1],'AdjustSaturation',2);
    imwrite(rgb,'result.jpg');
end

function weight = weighted()
        weight = [1:256];
	    weight = min(weight, 256-weight);
end

function lt = B(exposurePath)
    [~, speed] = textread(exposurePath,'%s %f');
    lt = [];
    for i=1:size(speed,1)
        lt = cat(2, lt, log(1/speed(i)));
    end
end