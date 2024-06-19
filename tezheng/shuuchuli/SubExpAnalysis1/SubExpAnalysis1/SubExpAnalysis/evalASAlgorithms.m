clear all; clc;

load('../sourceVideo.mat');
load('../MOS.mat');
sourceNames = sourceVideo.name;
algorithms = {'AIMD', 'BufferBased', 'Elastic', 'Festive', 'QDash', 'RateBased'};
algorithmScores = cell(6, 1);
group2 = 1:20;
% group 1 - without trim
group1 = [1, 2, 6, 12, 15];
% group 2 - with trim
group2(group1) = [];
count = 1;

for iii = 1:length(sourceNames)
    load(['../../playInfo/' sourceNames{iii} '.mat']); % playInfo
    load(['../../collapsedPlayInfo/' sourceNames{iii} '.mat']); % collapsedPlayInfo
    if sum(find(iii==group1))==0
        load(['../../selectedStreamInfo/' sourceNames{iii} 'SelectedSeq']); % SelectedSeq
        for jjj = 1:length(selectedSeq)
            seq = collapsedPlayInfo{selectedSeq(jjj), 5};
            for kkk = 1:length(seq)
                alg = playInfo(seq(kkk), 2);
                algIdx = find(ismember(algorithms, alg));
                algorithmScores{algIdx}(end+1) = MOS(count);
            end
            count = count + 1;
        end
    else
        for jjj = 1:length(collapsedPlayInfo)
            for kkk = 1:length(collapsedPlayInfo{jjj, 5})
                alg = playInfo(collapsedPlayInfo{jjj, 5}(kkk), 2);
                algIdx = find(ismember(algorithms, alg));
                algorithmScores{algIdx}(end+1) = MOS(count);
            end
            count = count + 1;
        end
    end
end

algMOS = zeros(length(algorithms), 1);
algMOSStd = zeros(length(algorithms), 1);
for iii = 1:length(algMOS)
    algMOS(iii) = mean(algorithmScores{iii});
    algMOSStd(iii) = std(algorithmScores{iii});
end

figure('Position', [10, 10, 1200, 960]);
% figure;
axis([0 6 0 1]);
bar(1:6,algMOS,'w', 'LineWidth', 2);
hold on;
errorbar(1:6,algMOS,algMOSStd/2,'b*', 'LineWidth', 2);
ylabel('MOS','FontSize',24,'FontName','times new roman');
set(gca,'FontSize',24,'FontName','times new roman');
set(gca,'XTick',1:6,'XTickLabel',{'AIMD', 'BufferBased', 'Elastic', 'Festive', 'QDash', 'RateBased'});
set(gca,'YTick',0:20:100);
ylim([0 100]);
xlim([0 6+1]);
rotateticklabel(gca,30);

for i = 1 : length(algorithmScores)
    for j = 1 : length(algorithmScores)
        x = algorithmScores{i};
        y = algorithmScores{j};
        if(ttest2(x,y) == 0)
            H(i,j) = '-';
        else
            if(ttest2(x,y, 0.05, 'right') == 0)
                H(i,j) = '0';
            else
                H(i,j) = '1';
            end
        end
    end
end


H