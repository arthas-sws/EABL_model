clear; clc; close all;

bitratePath = 'D:\Work\subjectiveExp\dataAnalysis\bitrate\';
qualityPath = 'D:\Work\subjectiveExp\dataAnalysis\forComcast\MOS';
qualityPath_Ryan = 'D:\Work\subjectiveExp\dataAnalysis\forComcast\RYAN';

v = {'1_561', '2_372', '3_412', '4_573', '5_1689', ...
     '6_315', '7_1320', '8_30', '9_1389', '10_1828', ...
     '11_484', '12_467', '13_438', '14_268', '15_1592', ...
     '16_161', '17_276', '18_1149', '19_498', '20_128'};
 
pltIdx = [1, 3, 9, 10, 11];


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
  
    filename = sprintf('%s%s%s%s', '.\VQAScore\SSIMplus', '\AV1\UP3216Q\', v{pltIdx(iii)}, '.mat');
    load(filename);
    av1_ssim.q = q(:, [1, 3, 4]);
    filename = sprintf('%s%s%s%s', '.\VQAScore\VMAF', '\AV1\', v{pltIdx(iii)}, '.mat');
    load(filename);
    av1_vmaf.q = q(:, [1, 3, 4]);
    filename = sprintf('%s%s%s%s', '.\VQAScore\PSNR', '\AV1\', v{pltIdx(iii)}, '.mat');
    load(filename);
    av1_psnr.q = q(:, [1, 3, 4]);
    
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

    filename = sprintf('%s%s%s%s', '.\VQAScore\SSIMplus', '\H264\UP3216Q\', v{pltIdx(iii)}, '.mat');
    load(filename);
    h264_ssim.q = q(:, [1, 3, 4]);
    filename = sprintf('%s%s%s%s', '.\VQAScore\VMAF', '\H264\', v{pltIdx(iii)}, '.mat');
    load(filename);
    h264_vmaf.q = q(:, [1, 3, 4]);
    filename = sprintf('%s%s%s%s', '.\VQAScore\PSNR', '\H264\', v{pltIdx(iii)}, '.mat');
    load(filename);
    h264_psnr.q = q(:, [1, 3, 4]);
    
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
    
    filename = sprintf('%s%s%s%s', '.\VQAScore\SSIMplus', '\HEVC\UP3216Q\', v{pltIdx(iii)}, '.mat');
    load(filename);
    hevc_ssim.q = q(:, [1, 3, 4]);
    filename = sprintf('%s%s%s%s', '.\VQAScore\VMAF', '\HEVC\', v{pltIdx(iii)}, '.mat');
    load(filename);
    hevc_vmaf.q = q(:, [1, 3, 4]);
    filename = sprintf('%s%s%s%s', '.\VQAScore\PSNR', '\HEVC\', v{pltIdx(iii)}, '.mat');
    load(filename);
    hevc_psnr.q = q(:, [1, 3, 4]);
    %% plot %%
    figure
    subplot(2, 3, 2)
    im = imread(sprintf('../screen_shots/%d.jpg', pltIdx(iii)));
    imshow(im);
    
    rl = min([h264.r(3, :), hevc.r(3, :), av1.r(3, :)]);
    rh = max([h264.r(3, :), hevc.r(3, :), av1.r(3, :)]);
    rl = round((rl-0.1)*10)/10;
    rh = round((rh+0.1)*10)/10;


    % 4K
    subplot(2, 2, 3);
    plot(h264.r(3, :), h264.q(3, :), 'color', [0, 0.4470, 0.741], ...
        'Marker', 'x', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    plot(hevc.r(3, :), hevc.q(3, :), 'color', [0.4940, 0.1840, 0.5560], ...
        'Marker', 'd', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    plot(av1.r(3, :), av1.q(3, :), 'color', [0.4660, 0.6740, 0.1880], ...
        'Marker', 's', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    r = linspace(h264.r(3, 1), h264.r(3, end), 500);
    q = pchip(h264.r(3, :), h264.q(3, :), r);
    plot(r, q, 'color', [0, 0.4470, 0.741], 'LineWidth', 2);
    
    r = linspace(hevc.r(3, 1), hevc.r(3, end), 100);
    q = pchip(hevc.r(3, :), hevc.q(3, :), r);
    plot(r, q, 'color', [0.4940, 0.1840, 0.5560], 'LineWidth', 2);
    
    r = linspace(av1.r(3, 1), av1.r(3, end), 100);
    q = pchip(av1.r(3, :), av1.q(3, :), r);
    plot(r, q, 'color', [0.4660, 0.6740, 0.1880], 'LineWidth', 2);
    
    xlabel('log(bitrate[kbps])','FontSize', 20, 'FontName', 'times new roman');
    legend('H.264', 'HEVC', 'AV1', 'Location', 'northwest');
    set(gca, 'FontSize', 20, 'FontName', 'times new roman');
    xticks(rl:0.2:rh);
    xlim([rl, rh]);
    ylim([0, 100]);
    title('MOS@2160p');

 % 4K
    subplot(2, 2, 4);
    plot(h264_mos.r(3, :), h264_mos.q(3, :), 'color', [0, 0.4470, 0.741], ...
        'Marker', 'x', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    plot(hevc_mos.r(3, :), hevc_mos.q(3, :), 'color', [0.4940, 0.1840, 0.5560], ...
        'Marker', 'd', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    plot(av1_mos.r(3, :), av1_mos.q(3, :), 'color', [0.4660, 0.6740, 0.1880], ...
        'Marker', 's', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    r = linspace(h264_mos.r(3, 1), h264_mos.r(3, end), 500);
    q = pchip(h264_mos.r(3, :), h264_mos.q(3, :), r);
    plot(r, q, 'color', [0, 0.4470, 0.741], 'LineWidth', 2);
    
    r = linspace(hevc_mos.r(3, 1), hevc_mos.r(3, end), 100);
    q = pchip(hevc_mos.r(3, :), hevc_mos.q(3, :), r);
    plot(r, q, 'color', [0.4940, 0.1840, 0.5560], 'LineWidth', 2);
    
    r = linspace(av1_mos.r(3, 1), av1_mos.r(3, end), 100);
    q = pchip(av1_mos.r(3, :), av1_mos.q(3, :), r);
    plot(r, q, 'color', [0.4660, 0.6740, 0.1880], 'LineWidth', 2);
    
    xlabel('log(bitrate[kbps])','FontSize', 20, 'FontName', 'times new roman');
    legend('H.264', 'HEVC', 'AV1', 'Location', 'northwest');
    set(gca, 'FontSize', 20, 'FontName', 'times new roman');
    xticks(rl:0.2:rh);
    xlim([rl, rh]);
    ylim([0, 100]);
    title('ChuckRyan@2160p');
 
    figure('units','normalized','pos',[0 0 1 0.4]);
 % 4K_SSIM
    subplot(1, 3, 1);
    plot(h264_mos.r(3, :), h264_ssim.q(3, :), 'color', [0, 0.4470, 0.741], ...
        'Marker', 'x', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    plot(hevc_mos.r(3, :), hevc_ssim.q(3, :), 'color', [0.4940, 0.1840, 0.5560], ...
        'Marker', 'd', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    plot(av1_mos.r(3, :), av1_ssim.q(3, :), 'color', [0.4660, 0.6740, 0.1880], ...
        'Marker', 's', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    r = linspace(h264_mos.r(3, 1), h264_mos.r(3, end), 500);
    q = pchip(h264_mos.r(3, :), h264_ssim.q(3, :), r);
    plot(r, q, 'color', [0, 0.4470, 0.741], 'LineWidth', 2);
    
    r = linspace(hevc_mos.r(3, 1), hevc_mos.r(3, end), 100);
    q = pchip(hevc_mos.r(3, :), hevc_ssim.q(3, :), r);
    plot(r, q, 'color', [0.4940, 0.1840, 0.5560], 'LineWidth', 2);
    
    r = linspace(av1_mos.r(3, 1), av1_mos.r(3, end), 100);
    q = pchip(av1_mos.r(3, :), av1_ssim.q(3, :), r);
    plot(r, q, 'color', [0.4660, 0.6740, 0.1880], 'LineWidth', 2);
    
    xlabel('log(bitrate[kbps])','FontSize', 20, 'FontName', 'times new roman');
    legend('H.264', 'HEVC', 'AV1', 'Location', 'northwest');
    set(gca, 'FontSize', 20, 'FontName', 'times new roman');
    xticks(rl:0.2:rh);
    xlim([rl, rh]);
    ylim([0, 100]);
    title('SSIMplus@2160p');

 % 4K_VMAF
    subplot(1, 3, 2);
    plot(h264_mos.r(3, :), h264_vmaf.q(3, :), 'color', [0, 0.4470, 0.741], ...
        'Marker', 'x', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    plot(hevc_mos.r(3, :), hevc_vmaf.q(3, :), 'color', [0.4940, 0.1840, 0.5560], ...
        'Marker', 'd', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    plot(av1_mos.r(3, :), av1_vmaf.q(3, :), 'color', [0.4660, 0.6740, 0.1880], ...
        'Marker', 's', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    r = linspace(h264_mos.r(3, 1), h264_mos.r(3, end), 500);
    q = pchip(h264_mos.r(3, :), h264_vmaf.q(3, :), r);
    plot(r, q, 'color', [0, 0.4470, 0.741], 'LineWidth', 2);
    
    r = linspace(hevc_mos.r(3, 1), hevc_mos.r(3, end), 100);
    q = pchip(hevc_mos.r(3, :), hevc_vmaf.q(3, :), r);
    plot(r, q, 'color', [0.4940, 0.1840, 0.5560], 'LineWidth', 2);
    
    r = linspace(av1_mos.r(3, 1), av1_mos.r(3, end), 100);
    q = pchip(av1_mos.r(3, :), av1_vmaf.q(3, :), r);
    plot(r, q, 'color', [0.4660, 0.6740, 0.1880], 'LineWidth', 2);
    
    xlabel('log(bitrate[kbps])','FontSize', 20, 'FontName', 'times new roman');
    legend('H.264', 'HEVC', 'AV1', 'Location', 'northwest');
    set(gca, 'FontSize', 20, 'FontName', 'times new roman');
    xticks(rl:0.2:rh);
    xlim([rl, rh]);
    ylim([0, 100]);
    title('VMAF@2160p');
    
 % 4K_PSNR
    subplot(1, 3, 3);
    plot(h264_mos.r(3, :), h264_psnr.q(3, :), 'color', [0, 0.4470, 0.741], ...
        'Marker', 'x', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    plot(hevc_mos.r(3, :), hevc_psnr.q(3, :), 'color', [0.4940, 0.1840, 0.5560], ...
        'Marker', 'd', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    plot(av1_mos.r(3, :), av1_psnr.q(3, :), 'color', [0.4660, 0.6740, 0.1880], ...
        'Marker', 's', 'LineStyle', 'none', 'LineWidth', 2, 'MarkerSize', 10); hold on;
    
    r = linspace(h264_mos.r(3, 1), h264_mos.r(3, end), 500);
    q = pchip(h264_mos.r(3, :), h264_psnr.q(3, :), r);
    plot(r, q, 'color', [0, 0.4470, 0.741], 'LineWidth', 2);
    
    r = linspace(hevc_mos.r(3, 1), hevc_mos.r(3, end), 100);
    q = pchip(hevc_mos.r(3, :), hevc_psnr.q(3, :), r);
    plot(r, q, 'color', [0.4940, 0.1840, 0.5560], 'LineWidth', 2);
    
    r = linspace(av1_mos.r(3, 1), av1_mos.r(3, end), 100);
    q = pchip(av1_mos.r(3, :), av1_psnr.q(3, :), r);
    plot(r, q, 'color', [0.4660, 0.6740, 0.1880], 'LineWidth', 2);
    
    xlabel('log(bitrate[kbps])','FontSize', 20, 'FontName', 'times new roman');
    legend('H.264', 'HEVC', 'AV1', 'Location', 'northwest');
    set(gca, 'FontSize', 20, 'FontName', 'times new roman');
    xticks(rl:0.2:rh);
    xlim([rl, rh]);
    ylim([0, 100]);
    title('PSNR@2160p');
end