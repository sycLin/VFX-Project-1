function writeRGBE(rgbeImg, filename)
    fp = fopen(filename,'w');
    [row, col, channel] = size(rgbeImg);
    outputData = zeros(row*col*channel,1);
    index = 1;
    for r=1:row
        for c=1:col
            for ch=1:channel
                outputData(index) = rgbeImg(r,c,ch);
                index = index + 1;
            end
        end
    end
    fwrite(fp,outputData,'uint8');
end