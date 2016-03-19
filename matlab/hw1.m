

function hw1()
    [~, speed, ~, ~, ~] = textread('memorial.hdr_image_list.txt','%s %f %d %d %d');
    for i=1:16
        Img{i} = imread(['memorial00', num2str(i+60), '.png']);
    end
    
    for g=1:3
        G{g} = [];
        for j=1:16
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
    lt = B();
    g = zeros(256, 3);
    lE = zeros(50, 3);
    
    for i=1:3
        [g(:,i), lE(:,i)] = gSolve(Z{i}, lt, 10, w);
    end
%{     
    y = linspace(1,256,256);
    x = g(y,1);
    figure
    plot(x, y)
     
    y = linspace(1,256,256);
    x = g(y,2);
    figure
    plot(x, y)
     
    y = linspace(1,256,256);
    x = g(y,3);
    figure
    plot(x, y)
%}     
    img = getHDR(Img, g, lt, w);

end

function weight = weighted()
        weight = [1:256];
	    weight = min(weight, 256-weight);
end

function lt = B()
    [~, speed, ~, ~, ~] = textread('memorial.hdr_image_list.txt','%s %f %d %d %d');
    lt = [];
    for i=1:size(speed,1)
        lt = cat(2, lt, log(1/speed(i)));
    end
end