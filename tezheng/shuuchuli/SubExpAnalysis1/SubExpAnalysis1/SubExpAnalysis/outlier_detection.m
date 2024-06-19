function [new_data, P, Q, pind] = outlier_detection(data, varargin)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Outlier removal for subjective image/video quality assessment           %
% <ORGANIZATION> = Hong Kong Polytechnic University and                   %
%                  University of Waterloo                                 %
% <AUTHOR> = Hongwei Yong, Kede Ma and Zhengfang Duanmu                   %
% <VERSION> = 1.0                                                         %
%=========================================================================%
% Redistribution and use in source and binary forms, with or without      %
% modification are permitted. The authors make no representations about   %
% the suitability of this software for any purpose. It is provided "as    %
% is" without express or implied warranty.                                %
%                                                                         %
% Kindly report any suggestions or corrections to zduanmu@uwaterloo.ca    %
%=========================================================================%
% The function is an implementation of the outlier removal strategy of    %
% subjective quality assessment in ITU REC. BT500-13. Currently, the      %
% function is only applicable to the results of single stimuli(SS),       %
% double stimulus impairment scale(DSIS), double scale continuous quality %
% scale(DSCQS), paired comparison(PC) and multi-stimulus ranking(MSR)     %
% experiment methods.                                                     %
%                                                                         %
% Reference: ITU-R BT.500-13, "Recommendation: Methodology for the        %
% subjective assessment of the quality of television pictures," Jan. 2012.%
%=========================================================================%
% Inputs:                                                                 %
%   1. data: an IxJ matrix, where each entry represents the subjective    %
%           optinion of the subject on an image.                          %
% Optional Inputs:                                                        %
%   1. range: a vector [m,n] storing the range of subjective score        %
%           m: lower bound; n: upper bound                                %
%           [0,100] by default                                            %
%           eg. [0,10]                                                    %
% Outputs:                                                                %
%   1. new_data: an I'xJ matrix, where the outlier entries are labeled    %
%           as NaN and non-outlier entries remain as the original data.   %
%           I': number of subject after outlier detection                 %
%   2. P: an Ix1 vector. Each entry i represents the number of scores     %
%           that are too high comparing to the mean opinion of the i-th   %
%           subject                                                       %
%   3. Q: an Ix1 vector. Each entry i represents the number of scores     %
%           that are too low comparing to the mean opinion of the i-th    %
%           subject                                                       %
%   4. pind: an Ix1 vector. Each entry i is an indictor of whether        %
%           subject i is unreliable, and has been removed                 %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %% parse input %%
    p = inputParser;
    dafault_range = [0,100];
    addRequired(p,'data');
    addParameter(p, 'range', dafault_range, @isnumeric);
    parse(p, data, varargin{:});
    range = p.Results.range;
    
    %% compensation %%
    z_scores = (data - repmat( nanmean(data, 2), [1, size(data,2)] )) ./ repmat(nanstd(data,[],2), [1, size(data,2)]);
    z_min = min(z_scores,[],2);
    z_max = max(z_scores,[],2);
    new_data = z_scores.*repmat((range(2) - range(1)) ./ (z_max - z_min), [1, size(data,2)]) + ...
        repmat((z_max - range(2)*z_min)./(z_max-z_min), [1, size(data,2)]);
    
    %% outlier detection %%
    % determine if the distribution of each image is gaussian-like
    % T1: gaussian-like image/video indices
    [prst_mean, prst_var, prst_kur] = presentation_statistcs(data);
    T1 = repmat( ( prst_kur >= 2 ) .* ( prst_kur <= 4 ), [size(data,1), 1]);
    ind1 = find( T1 == 1 );
    dT11 = ( data - repmat( prst_mean, [size(data,1), 1] ) ) >= 2 * repmat(prst_var.^0.5, [size(data,1), 1]);
    dT12 = ( data - repmat( prst_mean, [size(data,1), 1] ) ) <= -2 * repmat(prst_var.^0.5, [size(data,1), 1]);
    ind11 = find(dT11 == 1);
    ind12 = find(dT12 == 1);

    % T2: non-gaussian-like image/video indices
    T2 = 1 - T1;
    ind2 = find( T2 == 1);
    dT21 = ( data - repmat( prst_mean, [size(data,1), 1] ) ) >= 20^0.5 * repmat(prst_var.^0.5, [size(data,1), 1]);
    dT22 = ( data - repmat( prst_mean, [size(data,1), 1] ) ) <=-20^0.5 * repmat(prst_var.^0.5, [size(data,1), 1]);
    ind21 = find(dT21 == 1);
    ind22 = find(dT22 == 1);

    % outlier scores
    indP = union( intersect( ind1, ind11 ), intersect( ind2, ind21 ) );
    indQ = union( intersect( ind1, ind12 ), intersect( ind2, ind22 ) );
    X = zeros( size(data) );
    X(indP) = 1;
    P = sum( X, 2 );
    X = zeros( size( data ) );
    X( indQ ) = 1;
    Q = sum( X, 2 );
    
    % outlier subject indices
    pind = ( P + Q > 0.05 * size(data,2) ) & ( abs( (P - Q) ./ (P + Q) ) < 0.3 );

    new_data( indP ) = NaN;
    new_data( indQ ) = NaN;
    new_data = new_data(~pind, :);
end