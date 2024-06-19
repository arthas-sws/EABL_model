clear; clc; close all;

bitratePath = 'D:\Work\subjectiveExp\dataAnalysis\bitrate\';
qualityPath = 'D:\Work\subjectiveExp\dataAnalysis\forComcast\MOS';
qualityPath_Ryan = 'D:\Work\subjectiveExp\dataAnalysis\forComcast\RYAN';

v = {'1_561', '2_372', '3_412', '4_573', '5_1689', ...
     '6_315', '7_1320', '8_30', '9_1389', '10_1828', ...
     '11_484', '12_467', '13_438', '14_268', '15_1592', ...
     '16_161', '17_276', '18_1149', '19_498', '20_128'};
 
% pltIdx = [7, 8, 10, 13];
pltIdx = [1, 3, 9, 10, 11];

% vFinished = 14;
% nRes = 3;

for iii = 1:length(pltIdx)
    %% load data %%
    % AV1
    filename = sprintf('%s%s%s', '.\bitrate\AV1\', v{pltIdx(iii)}, '.mat')
    load(filename);
    av1.r = log10(x(:, [1, 3, 4]));
    filename = sprintf('%s%s%s%s', qualityPath, '\AV1\', v{pltIdx(iii)}, '.mat');
    load(filename);
    av1.q = q(:, [1, 3, 4]);

    filename = sprintf('%s%s%s', '.\bitrate\AV1\', v{pltIdx(iii)}, '.mat')
    load(filename);
    av1_mos.r = log10(x(:, [1, 3, 4]));
    filename = sprintf('%s%s%s%s', qualityPath_Ryan, '\AV1\', v{pltIdx(iii)}, '.mat');
    load(filename);
    av1_mos.q = q(:, [1, 3, 4]);
  
    filename = sprintf('%s%s%s%s', '.\VQAScore\SSIMplus\UP3216Q', '\AV1\', v{pltIdx(iii)}, '.mat');
    load(filename);
    av1_ssim.q = q(:, [1, 3, 4]);
    filename = sprintf('%s%s%s%s', '.\VQAScore\VMAF', '\AV1\', v{pltIdx(iii)}, '.mat');
    load(filename);
    av1_vmaf.q = q(:, [1, 3, 4]);
    filename = sprintf('%s%s%s%s', '.\VQAScore\PSNR', '\AV1\', v{pltIdx(iii)}, '.mat');
    load(filename);
    av1_psnr.q = q(:, [1, 3, 4]);
%     % AVS2
%     filename = sprintf('%s%s%s', '.\bitrate\AVS2\', v{pltIdx(iii)}, '.mat');
%     load(filename);
%     avs2.r = log10(x);
%     filename = sprintf('%s%s%s%s', qualityPath, '\AVS2\', v{pltIdx(iii)}, '.mat');
%     load(filename);
%     avs2.q = q;
    
    % H264
    filename = sprintf('%s%s%s', '.\bitrate\H264\', v{pltIdx(iii)}, '.mat');
    load(filename);
    h264.r = log10(x(:, [1, 3, 4]));
    filename = sprintf('%s%s%s%s', qualityPath, '\H264\', v{pltIdx(iii)}, '.mat');
    load(filename);
    h264.q = q(:, [1, 3, 4]);
    
    filename = sprintf('%s%s%s', '.\bitrate\H264\', v{pltIdx(iii)}, '.mat');
    load(filename);
    h264_mos.r = log10(x(:, [1, 3, 4]));
    filename = sprintf('%s%s%s%s', qualityPath_Ryan, '\H264\', v{pltIdx(iii)}, '.mat');
    load(filename);
    h264_mos.q = q(:, [1, 3, 4]);

    filename = sprintf('%s%s%s%s', '.\VQAScore\SSIMplus\UP3216Q', '\AV1\', v{pltIdx(iii)}, '.mat');
    load(filename);
    av1_ssim.q = q(:, [1, 3, 4]);
    filename = sprintf('%s%s%s%s', '.\VQAScore\VMAF', '\AV1\', v{pltIdx(iii)}, '.mat');
    load(filename);
    av1_vmaf.q = q(:, [1, 3, 4]);
    filename = sprintf('%s%s%s%s', '.\VQAScore\PSNR', '\AV1\', v{pltIdx(iii)}, '.mat');
    load(filename);
    av1_psnr.q = q(:, [1, 3, 4]);
%     % VP9
%     filename = sprintf('%s%s%s', '.\bitrate\VP9\', v{pltIdx(iii)}, '.mat');
%     load(filename);
%     vp9.r = log10(x);
%     filename = sprintf('%s%s%s%s', qualityPath, '\VP9\', v{pltIdx(iii)}, '.mat');
%     load(filename);
%     vp9.q = q;
    
    % HEVC
    filename = sprintf('%s%s%s', '.\bitrate\HEVC\', v{pltIdx(iii)}, '.mat');
    load(filename);
    hevc.r = log10(x(:, [1, 3, 4]));
    filename = sprintf('%s%s%s%s', qualityPath, '\HEVC\', v{pltIdx(iii)}, '.mat');
    load(filename);
    hevc.q = q(:, [1, 3, 4]);

    filename = sprintf('%s%s%s', '.\bitrate\HEVC\', v{pltIdx(iii)}, '.mat');
    load(filename);
    hevc_mos.r = log10(x(:, [1, 3, 4]));
    filename = sprintf('%s%s%s%s', qualityPath_Ryan, '\HEVC\', v{pltIdx(iii)}, '.mat');
    load(filename);
    hevc_mos.q = q(:, [1, 3, 4]);
    
    %% plot %%
    figure
%     figure('units','normalized','pos',[0 0 0.5 0.4]);
    subplot(2, 3, 2)
    im = imread(sprintf('../screen_shots/%d.jpg', pltIdx(iii)));
    imshow(im);
    
%     rl = min([h264.r(1, :), vp9.r(1, :), avs2.r(1, :), hevc.r(1, :), av1.r(1, :), h264.r(2, :), vp9.r(2, :), avs2.r(2, :), hevc.r(2, :), av1.r(2, :), h264.r(3, :), vp9.r(3, :), avs2.r(3, :), hevc.r(3, :), av1.r(3, :)]);
%     rh = max([h264.r(1, :), vp9.r(1, :), avs2.r(1, :), hevc.r(1, :), av1.r(1, :), h264.r(2, :), vp9.r(2, :), avs2.r(2, :), hevc.r(2, :), av1.r(2, :), h264.r(3, :), vp9.r(3, :), avs2.r(3, :), hevc.r(3, :), av1.r(3, :)]);
    rl = min([h264.r(3, :), hevc.r(3, :), av1.r(3, :)]);
    rh = max([h264.r(3, :), hevc.r(3, :), av1.r(3, :)]);
    rl = round((rl-0.1)*10)/10;
    rh = round((rh+0.1)*10)/10;
%     % 540p
%     
%     subplot(1, 3, 1);
%     plot(h264.r(1, :), h264.q(1, :), 'Color', [0, 0.4470, 0.7410], ...
%         'Marker', 'x', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
%     
%     plot(vp9.r(1, :), vp9.q(1, :), 'Color', [0.8500, 0.3250, 0.0980], ...
%         'Marker', '+', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
%     
%     plot(avs2.r(1, :), avs2.q(1, :), 'Color', [0.9290, 0.6940, 0.1250], ...
%         'Marker', 'o', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
%     
%     plot(hevc.r(1, :), hevc.q(1, :), 'Color', [0.4940, 0.1840, 0.5560], ...
%         'Marker', 'd', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
%     
%     plot(av1.r(1, :), av1.q(1, :), 'Color', [0.4660, 0.6740, 0.1880], ...
%         'Marker', 's', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
%     
%     r = linspace(h264.r(1, 1), h264.r(1, end), 500);
%     q = pchip(h264.r(1, :), h264.q(1, :), r);
%     plot(r, q, 'Color', [0, 0.4470, 0.7410], 'LineWidth', 2); hold on;
%     
%     r = linspace(vp9.r(1, 1), vp9.r(1, end), 500);
%     q = pchip(vp9.r(1, :), vp9.q(1, :), r);
%     plot(r, q, 'Color', [0.8500, 0.3250, 0.0980], 'LineWidth', 2); hold on;
%     
%     r = linspace(avs2.r(1, 1), avs2.r(1, end), 500);
%     q = pchip(avs2.r(1, :), avs2.q(1, :), r);
%     plot(r, q, 'Color', [0.9290, 0.6940, 0.1250], 'LineWidth', 2); hold on;
%     
%     r = linspace(hevc.r(1, 1), hevc.r(1, end), 500);
%     q = pchip(hevc.r(1, :), hevc.q(1, :), r);
%     plot(r, q, 'Color', [0.4940, 0.1840, 0.5560], 'LineWidth', 2); hold on;
%     
%     r = linspace(av1.r(1, 1), av1.r(1, end), 500);
%     q = pchip(av1.r(1, :), av1.q(1, :), r);
%     plot(r, q, 'Color', [0.4660, 0.6740, 0.1880], 'LineWidth', 2); hold on;
%     
%     ylabel('MOS', 'FontSize', 20, 'FontName', 'times new roman');
%     xlabel('log(bitrate[kbps])','FontSize', 20, 'FontName', 'times new roman');
%     legend('H.264', 'VP9', 'AVS2', 'HEVC', 'AV1', 'Location', 'southeast');
%     set(gca, 'FontSize', 20, 'FontName', 'times new roman');
% %     rl = min([h264.r(1, :), vp9.r(1, :), avs2.r(1, :), hevc.r(1, :), av1.r(1, :)]);
% %     rh = max([h264.r(1, :), vp9.r(1, :), avs2.r(1, :), hevc.r(1, :), av1.r(1, :)]);
% %     rl = round((rl-0.1)*10)/10;
% %     rh = round((rh+0.1)*10)/10;
%     xticks(rl:0.2:rh);
%     xlim([rl, rh]);
%     ylim([0, 100]);
%     title('540p');
% %     saveas(gcf, sprintf('../plot/%d_540p.eps', pltIdx(iii)), 'epsc');
% %     saveas(gcf, sprintf('../plot/%d_540p.png', pltIdx(iii)));
% %     close;
%     
%     % 1080p
% %     figure;
%     subplot(1, 3, 2);
%     plot(h264.r(2, :), h264.q(2, :), 'color', [0, 0.4470, 0.741], ...
%         'Marker', 'x', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
%     
%     plot(vp9.r(2, :), vp9.q(2, :), 'color', [0.8500, 0.3250, 0.0980], ...
%         'Marker', '+', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
%     
%     plot(avs2.r(2, :), avs2.q(2, :), 'color', [0.9290, 0.6940, 0.1250], ...
%         'Marker', 'o', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
%     
%     plot(hevc.r(2, :), hevc.q(2, :), 'color', [0.4940, 0.1840, 0.5560], ...
%         'Marker', 'd', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
%     
%     plot(av1.r(2, :), av1.q(2, :), 'color', [0.4660, 0.6740, 0.1880], ...
%         'Marker', 's', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
%     
%     r = linspace(h264.r(2, 1), h264.r(2, end), 500);
%     q = pchip(h264.r(2, :), h264.q(2, :), r);
%     plot(r, q, 'color', [0, 0.4470, 0.741], 'LineWidth', 2);
%     
%     r = linspace(vp9.r(2, 1), vp9.r(2, end), 500);
%     q = pchip(vp9.r(2, :), vp9.q(2, :), r);
%     plot(r, q, 'color', [0.8500, 0.3250, 0.0980], 'LineWidth', 2);
%     
%     r = linspace(avs2.r(2, 1), avs2.r(2, end), 500);
%     q = pchip(avs2.r(2, :), avs2.q(2, :), r);
%     plot(r, q, 'color', [0.9290, 0.6940, 0.1250], 'LineWidth', 2);
%     
%     r = linspace(hevc.r(2, 1), hevc.r(2, end), 500);
%     q = pchip(hevc.r(2, :), hevc.q(2, :), r);
%     plot(r, q, 'color', [0.4940, 0.1840, 0.5560], 'LineWidth', 2);
%     
%     r = linspace(av1.r(2, 1), av1.r(2, end), 500);
%     q = pchip(av1.r(2, :), av1.q(2, :), r);
%     plot(r, q, 'color', [0.4660, 0.6740, 0.1880], 'LineWidth', 2);
%     
% %     ylabel('MOS', 'FontSize', 20, 'FontName', 'times new roman');
%     xlabel('log(bitrate[kbps])','FontSize', 20, 'FontName', 'times new roman');
% %     legend('H.264', 'VP9', 'AVS2', 'HEVC', 'AV1', 'Location', 'northwest');
%     set(gca, 'FontSize', 20, 'FontName', 'times new roman');
% %     rl = min([h264.r(2, :), vp9.r(2, :), avs2.r(2, :), hevc.r(2, :), av1.r(2, :)]);
% %     rh = max([h264.r(2, :), vp9.r(2, :), avs2.r(2, :), hevc.r(2, :), av1.r(2, :)]);
% %     rl = round((rl-0.1)*10)/10;
% %     rh = round((rh+0.1)*10)/10;
%     xticks(rl:0.2:rh);
%     xlim([rl, rh]);
%     ylim([0, 100]);
%     title('1080p');
% %     saveas(gcf, sprintf('../plot/%d_1080p.eps', pltIdx(iii)), 'epsc');
% %     saveas(gcf, sprintf('../plot/%d_1080p.jpeg', pltIdx(iii)));
% %     close;
    
    % 4K
    subplot(2, 2, 3);
    plot(h264.r(3, :), h264.q(3, :), 'color', [0, 0.4470, 0.741], ...
        'Marker', 'x', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
%     plot(vp9.r(3, :), vp9.q(3, :), 'color', [0.8500, 0.3250, 0.0980], ...
%         'Marker', '+', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
%     
%     plot(avs2.r(3, :), avs2.q(3, :), 'color', [0.9290, 0.6940, 0.1250], ...
%         'Marker', 'o', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    plot(hevc.r(3, :), hevc.q(3, :), 'color', [0.4940, 0.1840, 0.5560], ...
        'Marker', 'd', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    plot(av1.r(3, :), av1.q(3, :), 'color', [0.4660, 0.6740, 0.1880], ...
        'Marker', 's', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    r = linspace(h264.r(3, 1), h264.r(3, end), 500);
    q = pchip(h264.r(3, :), h264.q(3, :), r);
    plot(r, q, 'color', [0, 0.4470, 0.741], 'LineWidth', 2);
   
%     r = linspace(vp9.r(3, 1), vp9.r(3, end), 500);
%     q = pchip(vp9.r(3, :), vp9.q(3, :), r);
%     plot(r, q, 'color', [0.8500, 0.3250, 0.0980], 'LineWidth', 2);
%     
%     r = linspace(avs2.r(3, 1), avs2.r(3, end), 100);
%     q = pchip(avs2.r(3, :), avs2.q(3, :), r);
%     plot(r, q, 'color', [0.9290, 0.6940, 0.1250], 'LineWidth', 2);
    
    r = linspace(hevc.r(3, 1), hevc.r(3, end), 100);
    q = pchip(hevc.r(3, :), hevc.q(3, :), r);
    plot(r, q, 'color', [0.4940, 0.1840, 0.5560], 'LineWidth', 2);
    
    r = linspace(av1.r(3, 1), av1.r(3, end), 100);
    q = pchip(av1.r(3, :), av1.q(3, :), r);
    plot(r, q, 'color', [0.4660, 0.6740, 0.1880], 'LineWidth', 2);
    
%     ylabel('MOS', 'FontSize', 20, 'FontName', 'times new roman');
    xlabel('log(bitrate[kbps])','FontSize', 20, 'FontName', 'times new roman');
    legend('H.264', 'HEVC', 'AV1', 'Location', 'northwest');
    set(gca, 'FontSize', 20, 'FontName', 'times new roman');
%     rl = min([h264.r(3, :), vp9.r(3, :), avs2.r(3, :), hevc.r(3, :), av1.r(3, :)]);
%     rh = max([h264.r(3, :), vp9.r(3, :), avs2.r(3, :), hevc.r(3, :), av1.r(3, :)]);
%     rl = round((rl-0.1)*10)/10;
%     rh = round((rh+0.1)*10)/10;
    xticks(rl:0.2:rh);
    xlim([rl, rh]);
    ylim([0, 100]);
    title('MOS@2160p');
%     saveas(gcf, sprintf('../plot/%d_4K.eps', pltIdx(iii)), 'epsc');
%     saveas(gcf, sprintf('../plot/%d_4K.png', pltIdx(iii)));
%     close;

 % 4K
    subplot(2, 2, 4);
    plot(h264_mos.r(3, :), h264_mos.q(3, :), 'color', [0, 0.4470, 0.741], ...
        'Marker', 'x', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
%     plot(vp9.r(3, :), vp9.q(3, :), 'color', [0.8500, 0.3250, 0.0980], ...
%         'Marker', '+', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
%     
%     plot(avs2.r(3, :), avs2.q(3, :), 'color', [0.9290, 0.6940, 0.1250], ...
%         'Marker', 'o', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    plot(hevc_mos.r(3, :), hevc_mos.q(3, :), 'color', [0.4940, 0.1840, 0.5560], ...
        'Marker', 'd', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    plot(av1_mos.r(3, :), av1_mos.q(3, :), 'color', [0.4660, 0.6740, 0.1880], ...
        'Marker', 's', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    r = linspace(h264_mos.r(3, 1), h264_mos.r(3, end), 500);
    q = pchip(h264_mos.r(3, :), h264_mos.q(3, :), r);
    plot(r, q, 'color', [0, 0.4470, 0.741], 'LineWidth', 2);
   
%     r = linspace(vp9.r(3, 1), vp9.r(3, end), 500);
%     q = pchip(vp9.r(3, :), vp9.q(3, :), r);
%     plot(r, q, 'color', [0.8500, 0.3250, 0.0980], 'LineWidth', 2);
%     
%     r = linspace(avs2.r(3, 1), avs2.r(3, end), 100);
%     q = pchip(avs2.r(3, :), avs2.q(3, :), r);
%     plot(r, q, 'color', [0.9290, 0.6940, 0.1250], 'LineWidth', 2);
    
    r = linspace(hevc_mos.r(3, 1), hevc_mos.r(3, end), 100);
    q = pchip(hevc_mos.r(3, :), hevc_mos.q(3, :), r);
    plot(r, q, 'color', [0.4940, 0.1840, 0.5560], 'LineWidth', 2);
    
    r = linspace(av1_mos.r(3, 1), av1_mos.r(3, end), 100);
    q = pchip(av1_mos.r(3, :), av1_mos.q(3, :), r);
    plot(r, q, 'color', [0.4660, 0.6740, 0.1880], 'LineWidth', 2);
    
%     ylabel('MOS', 'FontSize', 20, 'FontName', 'times new roman');
    xlabel('log(bitrate[kbps])','FontSize', 20, 'FontName', 'times new roman');
    legend('H.264', 'HEVC', 'AV1', 'Location', 'northwest');
    set(gca, 'FontSize', 20, 'FontName', 'times new roman');
%     rl = min([h264.r(3, :), vp9.r(3, :), avs2.r(3, :), hevc.r(3, :), av1.r(3, :)]);
%     rh = max([h264.r(3, :), vp9.r(3, :), avs2.r(3, :), hevc.r(3, :), av1.r(3, :)]);
%     rl = round((rl-0.1)*10)/10;
%     rh = round((rh+0.1)*10)/10;
    xticks(rl:0.2:rh);
    xlim([rl, rh]);
    ylim([0, 100]);
    title('ChuckRyan@2160p');
end