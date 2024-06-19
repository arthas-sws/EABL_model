clc,clear

% score = struct;
rows_index = 2;
for iii = 1:20
    subscoreFile = sprintf('subjectiveDataBackUp%d.mat', iii);
    load(['./score/' subscoreFile]);
    [rows, cols] = size(exp1);
    if iii > 11
%         score(1, (iii-12)*60+1:(iii-12)*60+cols) = exp1(1, :);
        score(rows_index:rows_index+rows-2, (iii-1)*38+1:760) = exp1(2:end, 1:(21-iii)*38);
        score(rows_index:rows_index+rows-2, 1:(iii-11)*38) = exp1(2:end, (21-iii)*38+1:380);
        rows_index = rows_index + rows-1;
    else
        score(1, (iii-1)*38+1:(iii-1)*38+cols) = exp1(1, :); 
        score(rows_index:rows_index+rows-2, (iii-1)*38+1:(iii-1)*38+cols) = exp1(2:end, :);
        rows_index = rows_index + rows-1;
    end
end

save('score.mat', 'score');