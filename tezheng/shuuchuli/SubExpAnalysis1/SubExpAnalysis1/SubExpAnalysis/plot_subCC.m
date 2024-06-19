function [] = plot_subCC(sub_srcc, sub_plcc, srcc_ci, plcc_ci)
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
%   1. sub_srcc: an (I+1)x1 vector of srcc between each inidividual to MOS%
%           I: number of subjects                                         %
%   2. sub_plcc: an (I+1)x1 vector of plcc between each inidividual to MOS%
%           I: number of subjects                                         %
%   3. srcc_ci: an (I+1)x1 vector of std of srcc                          %
%   4. plcc_ci: an (I+1)x1 vector of std of plcc                          %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    I = length(sub_srcc);
    %% SRCC figure %%
    figure;
    axis([0 I 0 1]);
    bar(1:(I-1),sub_srcc(1:end-1),'w', 'LineWidth', 0.8);%default 1.2
    hold on;
    bar(I,sub_srcc(end),'g', 'LineWidth', 0.8);%default 1.2
    errorbar(1:(I-1), sub_srcc(1:end-1),srcc_ci(1:end-1)/2,'b*', 'LineWidth', 0.8);%default 1.2
    errorbar(I, sub_srcc(end),srcc_ci(end)/2,'r*', 'LineWidth', 0.8);%default 1.2
    xlabel('Subjects','FontSize',18,'FontName','times new roman');
    ylabel('SRCC','FontSize',18,'FontName','times new roman');
    set(gca,'FontSize',18,'FontName','times new roman');
    set(gca,'XTick',0:5:I);
    set(gca,'YTick',0:0.2:1);
    ylim([0 1]);
    xlim([0 I+1]);
    
    %% PLCC figure %%
    figure;
    axis([0 I 0 1]);
    bar(1:(I-1),sub_plcc(1:end-1),'w', 'LineWidth', 0.8);%default 1.2
    hold on;
    bar(I,sub_plcc(end),'g', 'LineWidth', 0.8);%default 1.2
    errorbar(1:(I-1), sub_plcc(1:end-1),plcc_ci(1:end-1)/2,'b*', 'LineWidth', 0.8);%default 1.2
    errorbar(I, sub_plcc(end),plcc_ci(end)/2,'r*', 'LineWidth', 0.8);%default 1.2
    xlabel('Subjects','FontSize',18,'FontName','times new roman');
    ylabel('PLCC','FontSize',18,'FontName','times new roman');
    set(gca,'FontSize',18,'FontName','times new roman');
    set(gca,'XTick',0:5:I);
    set(gca,'YTick',0:0.2:1);
    ylim([0 1]);
    xlim([0 I+1]);
end