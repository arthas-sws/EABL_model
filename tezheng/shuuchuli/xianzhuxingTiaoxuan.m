clc; clear all;

% data = xlsread('四个库总数据.xlsx','vsense挑选');
% data = xlsread('四个库总数据.xlsx','vsense2挑选');
% data = xlsread('四个库总数据.xlsx','MPCCD挑选');
 data = xlsread('四个库总数据.xlsx','WPC2挑选');
MOS = data(:,1);
DCT = data(:,2);
for i = 1:size(data,2)-1
temp = feval('regress_hamid', data(:,i+1), MOS);
X(:,i) = temp - MOS;
end

for i = 1 : size(X,2)
    for j = 1 : size(X,2)
        x = X(:,i);
        y = X(:,j);
        if(vartest2(x,y) == 0)
            H(i,j) = '-';
        else
            if(vartest2(x,y, 0.05, 'right') == 0)
                H(i,j) = '1';
            else
                H(i,j) = '0';
            end
        end
    end
end
    H
%  xlswrite('显著性实验挑选.xlsx',H,'WPC2挑选');