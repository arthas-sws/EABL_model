clear all; clc; close all;

load('score.mat');
numSeq = 20;
MOS_vector = zeros(1, 760);
subjectStartIndex = zeros(size(score, 1)-1, 1);
tempIndex = 2;
subjectScoreMatrix = zeros(0, 0);
seqIndices = zeros(20, 760);
for iii = 1:20
    seqIndices(iii, (iii-1)*38+1:iii*38) = 1;
end

pDataAll = [];
maxNumRows = 30;
for iii = 1:numSeq
    Opinion = cell2mat(score(2:end, (iii-1)*38+1:iii*38));
%     data_idx = ~cellfun(@isempty,score(2:end, (iii-1)*60+1:iii*60));

    [pData, P, Q, pind] = outlier_detection(Opinion); % Redo!!
    [sub_srcc, sub_plcc, srcc_ci, plcc_ci, MOS_subVideo] = subject_correlation(pData, 1:38);
    MOS_vector((iii-1)*38+1:iii*38) = MOS_subVideo;
end

pearson_corr = zeros(size(score, 1)-1, 1);
spearman_corr = zeros(size(score, 1)-1, 1);

for iii = 2:size(score, 1)
    watched_video_idx = ~cellfun(@isempty,score(iii, :));
    participant_score = cell2mat(score(iii, watched_video_idx));
    mos_score = MOS_vector(watched_video_idx);
%     resolution_index = reshape([9:12:length(mos_score); ...
%         10:12:length(mos_score); 11:12:length(mos_score); ...
%         12:12:length(mos_score)], [], 1); % Change this to get resolution specific correlations
%     pearson_corr(iii-1) = corr(participant_score(resolution_index)', mos_score(resolution_index)', 'type', 'Pearson');
%     spearman_corr(iii-1) = corr(participant_score(resolution_index)', mos_score(resolution_index)', 'type', 'Spearman');
    pearson_corr(iii-1) = corr(participant_score', mos_score', 'type', 'Pearson');
    spearman_corr(iii-1) = corr(participant_score', mos_score', 'type', 'Spearman');
end

plot_subCC([spearman_corr; mean(spearman_corr)],[pearson_corr; mean(pearson_corr)],zeros(61, 1),zeros(61, 1));

mean(spearman_corr)
mean(pearson_corr)