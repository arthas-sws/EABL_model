clc;clear all;
data= xlsread('94���.xlsx','Sheet3');
DCT = data(1:5,:);
YUV =data(6,:);
mosxielv =data(7,:);
TQP = [26 32 38 44 50];
% ����ͼ��
 for i =1:5
figure(i);
scatter(DCT(i,:), YUV, 'filled'); % ʹ��scatter����ɢ��ͼ
xlabel('TE', 'FontSize', 20); % ���x���ǩ
ylabel('CD', 'FontSize', 20); % ���y���ǩ
set(gca, 'FontSize', 20);
% title('TQP = 'TQP{I}''); % ���ͼ�����
 print(['TQP' num2str(TQP(i))], '-dpng');
PLCC = corrcoef(DCT(i,:), YUV);
plcc(i)=PLCC(1,2);
 end
% ������ݵ��ǩ
% for i = 1:numel(DCT)
%     text(DCT(1,:)(i), YUV(i), num2str(i), 'HorizontalAlignment', 'left', 'VerticalAlignment', 'bottom');
% end
PLCC2 = corrcoef(YUV,mosxielv);