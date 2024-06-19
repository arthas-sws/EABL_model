clear all; clc; close all;

load('score.mat');
numSeq = 20;
pDataAll = [];
MOSALL = [];
STDALL = [];
MOS_vector = zeros(1, 760);
pearson_corr = zeros(size(score, 1)-1, 1);
spearman_corr = zeros(size(score, 1)-1, 1);
std_pearson_corr = zeros(size(score, 1)-1, 1);
std_spearman_corr = zeros(size(score, 1)-1, 1);

for iii = 1:numSeq
    dataRaw = cell2mat(score(2:end, (iii-1)*38+1:iii*38));
    [pData, P, Q, pind] = outlier_detection(dataRaw);
    [sub_srcc, sub_plcc, srcc_ci, plcc_ci, MOS_subVideo] = subject_correlation(pData, 1:38);
    MOSALL = [MOSALL, MOS_subVideo];
    STDALL = [STDALL, nanstd(pData)];
    MOS_vector((iii-1)*38+1:iii*38) = MOS_subVideo;
end

for iii = 2:size(score, 1)
    watched_video_idx = ~cellfun(@isempty,score(iii, :));
    participant_score = cell2mat(score(iii, watched_video_idx));
    mos_score = MOS_vector(watched_video_idx);
    
    plcc_content = zeros(10, 1);
    srcc_content = zeros(10, 1);
    for jjj = 1:10
        
    plcc_content(jjj) = corr(participant_score((jjj-1)*38+1:jjj*38)', mos_score((jjj-1)*38+1:jjj*38)', 'type', 'Pearson');
    srcc_content(jjj) = corr(participant_score((jjj-1)*38+1:jjj*38)', mos_score((jjj-1)*38+1:jjj*38)', 'type', 'Spearman');

    end
    pearson_corr(iii-1) = mean(plcc_content);
    spearman_corr(iii-1) = mean(srcc_content);
    std_pearson_corr(iii-1) = std(plcc_content);
    std_spearman_corr(iii-1) = std(srcc_content);
end

mean_STDALL = mean(STDALL);
plot_subCC([spearman_corr; mean(spearman_corr)],[pearson_corr; mean(pearson_corr)],[std_spearman_corr; std(spearman_corr)],[std_pearson_corr; std(pearson_corr)]);

figure,
histogram(MOSALL, 20);
xlabel('MOS')
ylabel('Number of Point Clouds')
xlim([0, 100])
set(gca,'FontSize',18,'FontName','times new roman');
figure,
histogram(STDALL, 20);
xlabel('Standard Deviation')
ylabel('Number of Point Clouds')
xlim([0, 30])
set(gca,'FontSize',18,'FontName','times new roman');
