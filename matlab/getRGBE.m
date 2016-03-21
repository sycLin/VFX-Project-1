function rgbeImg = getRGBE(img)
    [row, col , channel] = size(img);
    rgbeImg = zeros(row, col, 4);
    
    maxValue = max(img,[], 3);
    Low = find(maxValue<1e-32);
    
    [F,E] = log2(maxValue); % maxValue = F * 2^E
    
    E = E + 128;
    F = F*256./maxValue;
    F(Low) = 0;
    E(Low) = 0;
    
    for i=1:channel
        rgbeImg(:,:,i) = round(img(:,:,i).*F);
    end
    
    rgbeImg(:,:,4) = E;
    
end