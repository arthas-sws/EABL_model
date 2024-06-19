function [srcc, plcc, srcc_ci, plcc_ci, MOS] = subject_correlation( data, seqIndices )
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% WIQA data analysis toolbox                                              %
% <ORGANIZATION> = University of Waterloo                                 %
% <AUTHOR> = Zhengfang Duanmu                                             %
% <VERSION> = 1.0                                                         %
%=========================================================================%
% Redistribution and use in source and binary forms, with or without      %
% modification are permitted. The authors make no representations about   %
% the suitability of this software for any purpose. It is provided "as    %
% is" without express or implied warranty.                                %
%=========================================================================%
% The function examines the correlation of each subject to a mean subject.%
%=========================================================================%
% Inputs:                                                                 %
%   1. data: a IxJ matrix, where each entry represents the subjective     %
%           optinion of the subject on an image.                          %
%           I: number of subjects                                         %
%           J: number of samples                                          %
%   2. seqIndices: indices of sample coming from the same source clip     %
% Outputs:                                                                %
%   1. srcc: an 1xI+1 vector storing the Spearman's rank correlation      %
%           coefficient between each subject and MOS.                     %
%           The last entry is the performance of mean subject.            %
%   2. plcc: an 1xI+1 vector storing the Pearson Correlation Coefficient  %
%           between each subject and MOS.                                 %
%           The last entry is the performance of mean subject.            %
%   3. srcc_ci: 1xI+1 srcc confidence interval                            %
%   4. plcc_ci: 1xI+1 plcc confidence interval                            %
%   5. MOS: mean opinion score                                            %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    [I,J] = size(data);
    K = size(seqIndices, 1);
    srcc = zeros(I+1,1);
    plcc = zeros(I+1,1);
    MOS = nanmean(data,1);
    valid_entry = ~isnan(data);
    
    z1 = zeros(1, K);
    z2 = zeros(1, K);
    plcc_ci = zeros(I+1, 1);
    srcc_ci = zeros(I+1, 1);
    for a = 1:I
        for b = 1:K
            idx = valid_entry(a, :) & seqIndices(b, :);
            z1(b) = corr(data(a, idx)', MOS(idx)', 'type', 'Spearman');
            z2(b) = corr(data(a, idx)', MOS(idx)', 'type', 'Pearson');
        end
        srcc(a) = nanmean(z1);
        srcc_ci(a) = nanstd(z1) * 1.96 / sqrt(J/K);
        plcc(a) = nanmean(z2);
        plcc_ci(a) = nanstd(z2) * 1.96 / sqrt(J/K);
    end
    
    plcc(I+1) = mean(plcc(1:I));
    srcc(I+1) = mean(srcc(1:I));
    plcc_ci(I+1) = mean(plcc_ci(1:I));
    srcc_ci(I+1) = mean(srcc_ci(1:I));
end

