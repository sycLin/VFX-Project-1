% Z(i, j) is the value of the pixel at location i in image j
% B(j)    is the delta t for the image j
% l       is lambda
% w(z)    is the weighted funtion for pixel value

function [g, lE] = gSolve(Z,B,l,w)
    n = 256;
    A = zeros(size(Z,1)*size(Z,2)+1+n,size(Z,1)+n); % (N*P + 1 + 256, N + 256)
    b = zeros(size(A,1),1);
    
    % Ax = b
    k = 1;
    for i=1:size(Z,1)
        for j=1:size(Z, 2)
            wij = w(Z(i,j)+1);
            A(k,Z(i,j)+1) = wij;
            A(k,n+i) = -wij;
            b(k,1) = wij*B(j);
            k = k+1;
        end
    end
    
    A(k,129) = 1;
    k = k+1;
    
    for i=1:n-2
        A(k,i) = l*w(i+1);
        A(k,i+1) = -2*l*w(i+1);
        A(k,i+2) = l*w(i+1);
        k = k+1
    end
    
    x = A\b;
    g = x(1:n); % 0~256
    lE = x(n+1:size(x, 1)); % 0~256, ~ N
end
