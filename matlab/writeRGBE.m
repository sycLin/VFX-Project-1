function writeRGBE(rgbeImg, filename)
    fp = fopen(filename,'w');
    [row, col, channel] = size(rgbeImg);
    
    fprintf(fp,'#?RADIANCE\n');
    fprintf(fp,'FORMAT=32-bit_rle_rgbe\n');
    fprintf(fp,'EXPOSURE= 1.0000000000000\n\n');
    fprintf(fp,'-Y %d +X %d\n',row,col);
    
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
    fclose(fp);
end