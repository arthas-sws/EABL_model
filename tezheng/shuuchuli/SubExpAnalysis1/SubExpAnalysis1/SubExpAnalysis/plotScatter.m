clear; clc; close all;

dataPath = '.\VQAScore';
load('.\MOS\MOS_row.mat');
load(sprintf('%s\\psnr.mat', dataPath));
load(sprintf('%s\\ssimplus_macbook.mat', dataPath));
load(sprintf('%s\\vmaf.mat', dataPath));
load(sprintf('%s\\vqm.mat', dataPath))
load(sprintf('%s\\msssim.mat', dataPath))
q_ssimplus = q_ssimplus_macbook;
q_msssim = 100*q_msssim;
q_vqm = 100*q_vqm;

% AV1, AVS2, H264, VP9, HEVC
% av1Idx = [(0*60+1):(0*60+4), (1*60+1):(1*60+4), (2*60+1):(2*60+4), (3*60+1):(3*60+4), (4*60+1):(4*60+4), (5*60+1):(5*60+4), ...
%           (6*60+1):(6*60+4), (7*60+1):(7*60+4), (8*60+1):(8*60+4), (9*60+1):(9*60+4), (10*60+1):(10*60+4), ...
%           (11*60+1):(11*60+4), (4*60+1):(4*60+4), (13*60+1):(13*60+4), (14*60+1):(14*60+4), (15*60+1):(15*60+4), ...
%           (16*60+1):(16*60+4), (17*60+1):(17*60+4), (18*60+1):(18*60+4), (19*60+1):(19*60+4)] + 8;
% 
% avs2Idx = [(0*60+13):(0*60+16), (1*60+13):(1*60+16), (2*60+13):(2*60+16), (3*60+13):(3*60+16), (4*60+13):(4*60+16), (5*60+13):(5*60+16), ...
%           (6*60+13):(6*60+16), (7*60+13):(7*60+16), (8*60+13):(8*60+16), (9*60+13):(9*60+16), (10*60+13):(10*60+16), ...
%           (11*60+13):(11*60+16), (12*60+13):(12*60+16), (13*60+13):(13*60+16), (14*60+13):(14*60+16), (15*60+13):(15*60+16), ...
%           (16*60+13):(16*60+16), (17*60+13):(17*60+16), (18*60+13):(18*60+16), (19*60+13):(19*60+16)] + 8;
% 
% h264Idx = [(0*60+25):(0*60+28), (1*60+25):(1*60+28), (2*60+25):(2*60+28), (3*60+25):(3*60+28), (4*60+25):(4*60+28), (5*60+25):(5*60+28), ...
%           (6*60+25):(6*60+28), (7*60+25):(7*60+28), (8*60+25):(8*60+28), (9*60+25):(9*60+28), (10*60+25):(10*60+28), ...
%           (11*60+25):(11*60+28), (12*60+25):(12*60+28), (13*60+25):(13*60+28), (14*60+25):(14*60+28), (15*60+25):(15*60+28), ...
%           (16*60+25):(16*60+28), (17*60+25):(17*60+28), (18*60+25):(18*60+28), (19*60+25):(19*60+28)] + 8;
% 
% vp9Idx = [(0*60+37):(0*60+40), (1*60+37):(1*60+40), (2*60+37):(2*60+40), (3*60+37):(3*60+40), (4*60+37):(4*60+40), (5*60+37):(5*60+40), ...
%           (6*60+37):(6*60+40), (7*60+37):(7*60+40), (8*60+37):(8*60+40), (9*60+37):(9*60+40), (10*60+37):(10*60+40), ...
%           (11*60+37):(11*60+40), (12*60+37):(12*60+40), (13*60+37):(13*60+40), (14*60+37):(14*60+40), (15*60+37):(15*60+40), ...
%           (16*60+37):(16*60+40), (17*60+37):(17*60+40), (18*60+37):(18*60+40), (19*60+37):(19*60+40)] + 8;
% 
% hevcIdx = [(0*60+49):(0*60+52), (1*60+49):(1*60+52), (2*60+49):(2*60+52), (3*60+49):(3*60+52), (4*60+49):(4*60+52), (5*60+49):(5*60+52), ...
%           (6*60+49):(6*60+52), (7*60+49):(7*60+52), (8*60+49):(8*60+52), (9*60+49):(9*60+52), (10*60+49):(10*60+52), ...
%           (11*60+49):(11*60+52), (12*60+49):(12*60+52), (13*60+49):(13*60+52), (14*60+49):(14*60+52), (15*60+49):(15*60+52), ...
%           (16*60+49):(16*60+52), (17*60+49):(17*60+52), (18*60+49):(18*60+52), (19*60+49):(19*60+52)] + 8;

av1Idx = [(0*60+1):(0*60+4), (1*60+1):(1*60+4), (2*60+1):(2*60+4), (3*60+1):(3*60+4), (4*60+1):(4*60+4), (5*60+1):(5*60+4), ...
          (6*60+1):(6*60+4), (7*60+1):(7*60+4), (8*60+1):(8*60+4), (9*60+1):(9*60+4), (10*60+1):(10*60+4), ...
          (11*60+1):(11*60+4), (4*60+1):(4*60+4), (13*60+1):(13*60+4), (14*60+1):(14*60+4), (15*60+1):(15*60+4), ...
          (16*60+1):(16*60+4), (17*60+1):(17*60+4), (18*60+1):(18*60+4), (19*60+1):(19*60+4)];

avs2Idx = [(0*60+13):(0*60+16), (1*60+13):(1*60+16), (2*60+13):(2*60+16), (3*60+13):(3*60+16), (4*60+13):(4*60+16), (5*60+13):(5*60+16), ...
          (6*60+13):(6*60+16), (7*60+13):(7*60+16), (8*60+13):(8*60+16), (9*60+13):(9*60+16), (10*60+13):(10*60+16), ...
          (11*60+13):(11*60+16), (12*60+13):(12*60+16), (13*60+13):(13*60+16), (14*60+13):(14*60+16), (15*60+13):(15*60+16), ...
          (16*60+13):(16*60+16), (17*60+13):(17*60+16), (18*60+13):(18*60+16), (19*60+13):(19*60+16)];

h264Idx = [(0*60+25):(0*60+28), (1*60+25):(1*60+28), (2*60+25):(2*60+28), (3*60+25):(3*60+28), (4*60+25):(4*60+28), (5*60+25):(5*60+28), ...
          (6*60+25):(6*60+28), (7*60+25):(7*60+28), (8*60+25):(8*60+28), (9*60+25):(9*60+28), (10*60+25):(10*60+28), ...
          (11*60+25):(11*60+28), (12*60+25):(12*60+28), (13*60+25):(13*60+28), (14*60+25):(14*60+28), (15*60+25):(15*60+28), ...
          (16*60+25):(16*60+28), (17*60+25):(17*60+28), (18*60+25):(18*60+28), (19*60+25):(19*60+28)];

vp9Idx = [(0*60+37):(0*60+40), (1*60+37):(1*60+40), (2*60+37):(2*60+40), (3*60+37):(3*60+40), (4*60+37):(4*60+40), (5*60+37):(5*60+40), ...
          (6*60+37):(6*60+40), (7*60+37):(7*60+40), (8*60+37):(8*60+40), (9*60+37):(9*60+40), (10*60+37):(10*60+40), ...
          (11*60+37):(11*60+40), (12*60+37):(12*60+40), (13*60+37):(13*60+40), (14*60+37):(14*60+40), (15*60+37):(15*60+40), ...
          (16*60+37):(16*60+40), (17*60+37):(17*60+40), (18*60+37):(18*60+40), (19*60+37):(19*60+40)];

hevcIdx = [(0*60+49):(0*60+52), (1*60+49):(1*60+52), (2*60+49):(2*60+52), (3*60+49):(3*60+52), (4*60+49):(4*60+52), (5*60+49):(5*60+52), ...
          (6*60+49):(6*60+52), (7*60+49):(7*60+52), (8*60+49):(8*60+52), (9*60+49):(9*60+52), (10*60+49):(10*60+52), ...
          (11*60+49):(11*60+52), (12*60+49):(12*60+52), (13*60+49):(13*60+52), (14*60+49):(14*60+52), (15*60+49):(15*60+52), ...
          (16*60+49):(16*60+52), (17*60+49):(17*60+52), (18*60+49):(18*60+52), (19*60+49):(19*60+52)];

% videoindexes = [1, 3, 4, 9, 11, 12, 33, 35, 36, 37, 39, 40, 41, 43, 44];
av1Idx = [av1Idx, av1Idx + 4, av1Idx + 8];
vp9Idx = [vp9Idx, vp9Idx + 4, vp9Idx + 8];
h264Idx = [h264Idx, h264Idx + 4, h264Idx + 8];
hevcIdx = [hevcIdx, hevcIdx + 4, hevcIdx + 8];
avs2Idx = [avs2Idx, avs2Idx + 4, avs2Idx + 8];
% valid videos are 1-20 for now
validIdx = [av1Idx, avs2Idx, h264Idx, vp9Idx, hevcIdx];
% ryanAV1Idx = [1, 2, 3, 10, 11, 12, 19, 20, 21, 28, 29, 30, 37, 38, 39];
% scatter plots
% PSNR
figure; 
% subplot(1, 5, 1)
plot(q_psnr(av1Idx), mos(av1Idx), '*', ...
             q_psnr(avs2Idx), mos(avs2Idx), 'o', ...
             q_psnr(h264Idx), mos(h264Idx), 'x', ...
             q_psnr(vp9Idx), mos(vp9Idx), 'd', ...
             q_psnr(hevcIdx), mos(hevcIdx), 'gs'); hold on;
ylim([0 100]);
xlim([20 54]);
xlabel('PSNR', 'fontname', 'Helvetica', 'FontSize', 18);
ylabel('ChuckRyan', 'fontname', 'Helvetica', 'FontSize', 18);
pmos = feval('regress_hamid', q_psnr, mos);
[Obj_sort, ind] = sort(q_psnr);
MOS_sort = pmos(ind);
plot(Obj_sort, MOS_sort, 'k-', 'LineWidth', 1.2);
set(gca, 'FontSize', 18);
legend('AV1', 'H.264', 'HEVC', 'Location', 'northwest');

% SSIMplus
figure,
% subplot(1, 5, 2)
plot(q_ssimplus(av1Idx), mos(av1Idx), '*', ...
             q_ssimplus(avs2Idx), mos(avs2Idx), 'o', ...
             q_ssimplus(h264Idx), mos(h264Idx), 'x', ...
             q_ssimplus(vp9Idx), mos(vp9Idx), 'd', ...
             q_ssimplus(hevcIdx), mos(hevcIdx), 'gs'); hold on;
ylim([0 100]);
xlabel('SSIMplus', 'fontname', 'Helvetica', 'FontSize', 18);
ylabel('ChuckRyan', 'fontname', 'Helvetica', 'FontSize', 18);
pmos = feval('regress_hamid', q_ssimplus, mos);
[Obj_sort, ind] = sort(q_ssimplus);
MOS_sort = pmos(ind);
plot(Obj_sort, MOS_sort, 'k-', 'LineWidth', 1.2);
set(gca, 'FontSize', 18);
legend('AV1', 'H.264', 'HEVC', 'Location', 'northwest');

% VMAF
figure,
% subplot(1, 5, 3)
plot(q_vmaf(av1Idx), mos(av1Idx), '*', ...
             q_vmaf(avs2Idx), mos(avs2Idx), 'o', ...
             q_vmaf(h264Idx), mos(h264Idx), 'x', ...
             q_vmaf(vp9Idx), mos(vp9Idx), 'd', ...
             q_vmaf(hevcIdx), mos(hevcIdx), 'gs'); hold on;
ylim([0 100]);
xlabel('VMAF', 'fontname', 'Helvetica', 'FontSize', 18);
ylabel('MOS', 'fontname', 'Helvetica', 'FontSize', 18);
pmos = feval('regress_hamid', q_vmaf, mos);
[Obj_sort, ind] = sort(q_vmaf);
MOS_sort = pmos(ind);
plot(Obj_sort, MOS_sort, 'k-', 'LineWidth', 1.2);
set(gca, 'FontSize', 18);
% legend('AV1', 'AVS2', 'H.264', 'VP9', 'HEVC', 'Location', 'northwest');

% VQM
figure,
% subplot(1, 5, 4)
plot(q_vqm(av1Idx), mos(av1Idx), '*', ...
             q_vqm(avs2Idx), mos(avs2Idx), 'o', ...
             q_vqm(h264Idx), mos(h264Idx), 'x', ...
             q_vqm(vp9Idx), mos(vp9Idx), 'd', ...
             q_vqm(hevcIdx), mos(hevcIdx), 'gs'); hold on;
ylim([0 100]);
xlabel('VQM', 'fontname', 'Helvetica', 'FontSize', 18);
ylabel('MOS', 'fontname', 'Helvetica', 'FontSize', 18);
pmos = feval('regress_hamid', q_vqm, mos);
[Obj_sort, ind] = sort(q_vqm);
MOS_sort = pmos(ind);
plot(Obj_sort, MOS_sort, 'k-', 'LineWidth', 1.2);
set(gca, 'FontSize', 18);
% legend('AV1', 'AVS2', 'H.264', 'VP9', 'HEVC', 'Location', 'northwest');


% MS-SSIM
figure,
% subplot(1, 5, 5)
plot(q_msssim(av1Idx), mos(av1Idx), '*', ...
             q_msssim(avs2Idx), mos(avs2Idx), 'o', ...
             q_msssim(h264Idx), mos(h264Idx), 'x', ...
             q_msssim(vp9Idx), mos(vp9Idx), 'd', ...
             q_msssim(hevcIdx), mos(hevcIdx), 'gs'); hold on;
ylim([0 100]);
xlabel('MS-SSIM', 'fontname', 'Helvetica', 'FontSize', 18);
ylabel('MOS', 'fontname', 'Helvetica', 'FontSize', 18);
pmos = feval('regress_hamid', q_msssim, mos);
[Obj_sort, ind] = sort(q_msssim);
MOS_sort = pmos(ind);
plot(Obj_sort, MOS_sort, 'k-', 'LineWidth', 1.2);
set(gca, 'FontSize', 18);
% legend('AV1', 'AVS2', 'H.264', 'VP9', 'HEVC', 'Location', 'northwest');

validIdx = 1:1200;

plcc_psnr = corr(q_psnr(validIdx)', mos(validIdx)', 'Type', 'Pearson');
plcc_vmaf = corr(q_vmaf(validIdx)', mos(validIdx)', 'Type', 'Pearson');
plcc_ssimplus = corr(q_ssimplus(validIdx)', mos(validIdx)', 'Type', 'Pearson');
plcc_vqm = corr(q_vqm(validIdx)', mos(validIdx)', 'Type', 'Pearson');
plcc_msssim = corr(q_msssim(validIdx)', mos(validIdx)', 'Type', 'Pearson');

srcc_psnr = corr(q_psnr(validIdx)', mos(validIdx)', 'Type', 'Spearman');
srcc_vmaf = corr(q_vmaf(validIdx)', mos(validIdx)', 'Type', 'Spearman');
srcc_ssimplus = corr(q_ssimplus(validIdx)', mos(validIdx)', 'Type', 'Spearman');
srcc_vqm = corr(q_vqm(validIdx)', mos(validIdx)', 'Type', 'Spearman');
srcc_msssim = corr(q_msssim(validIdx)', mos(validIdx)', 'Type', 'Spearman');

% close all;