function rgbeImg = getRGBE(img)
    [row, col , channel] = size(img);
    rgbeImg = zeros(row, col, 4);
    
    maxValue = max(img,[], 3);
    
    [F,E] = log2(maxValue); % maxValue = F * 2^E

    F = F*256./maxValue;
    
    for i=1:channel
        rgbeImg(:,:,i) = round(img(:,:,i).*F);
    end
    
    rgbeImg(:,:,4) = E+128;
    
end