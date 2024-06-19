clc;clear all;

%%�������
%��һ����ȡ����
%�ڶ����ֳ������Լ�����֤��
%���������ò��Լ�����ģ�� ����֤��������PLCC
%���Ĳ�
%%
%��һ����ȡ����
data= xlsread('��������.xlsx');
datayuv = xlsread('DCT.xlsx','ԭʼ����YUV����');
TQP = data(:,1);
GQP = data(:,5);
MOS = data(:,2);
DCT = data(:,3);
TQS = [12.6992084157456,25.3984168314912,50.7968336629824,101.593667325965,203.187334651930];
MOSXL =datayuv(2:end,6);
%����ֻѡȡ��32��YUV����
YUV = datayuv(2:end,3);
tqp = reshape(TQP,25,16);
gqp = reshape(GQP,25,16);
mos = reshape(MOS,25,16);
dct = reshape(DCT,25,16);%һ����һ�ֵ���
%%
%�ڶ����ֳ������Լ�����֤��
numbers = [];
shiyancishu = 10000;%����������ж��ٴ��������
%%
%�޵���
for i = 1:shiyancishu
% ����1��16֮��İ˸����ظ�������� �洢��shujujuzhen��
shujujuzhen(i,:) = randperm(16, 8);
% ������һ������1��16֮��İ˸���ͬ��������ľ���
remaining_numbers(i,:) = setdiff(1:16, shujujuzhen(i,:));
yanzhengjuzhen(i,:) = setdiff(1:16, shujujuzhen(i,:));;
end
% A = yanzhengjuzhen+shujujuzhen;
% row_sums = sum(A, 2);
% 1+2+3+4+5+6+7+8+9+10+11+12+13+14+15+16
%��֤��������Ŀ�����
%%
% %�е���
% shaixuan = [4 7 14 15 ];%
% suiji = [1 2 3 5 6 8 9 10 11 12 13 16];
% for i =1:shiyancishu
% % �����ѡ�ĸ����ֵ�����
% indices_A(i,:) = randperm(length(suiji), 4);
% % ��������A
% A(i,:) = suiji(indices_A(i,:));
% % ʣ������ֵ�����
% indices_B(i,:) = setdiff(1:length(suiji), indices_A(i,:));
% % ��������B
% B(i,:) = suiji(indices_B(i,:));
% % sum(A)+sum(B)+sum(shaixuan)
% shujujuzhen(i,:) = horzcat(suiji(indices_A(i,:)), shaixuan);
% yanzhengjuzhen = B;
% shujuhe(i) = sum(shujujuzhen(i,:))+sum(yanzhengjuzhen(i,:));
% end

%%
%������ һ�ٴ�ʵ��

for shiyanxunhuan = 1:shiyancishu
    TQPC = tqp(:,shujujuzhen(shiyanxunhuan,:));
    GQPC = gqp(:,shujujuzhen(shiyanxunhuan,:));
    MOSC = mos(:,shujujuzhen(shiyanxunhuan,:));
    DCTC = dct(:,shujujuzhen(shiyanxunhuan,:));
    MOSXLC = MOSXL(shujujuzhen(shiyanxunhuan,:),1)';
    YUVC = YUV(shujujuzhen(shiyanxunhuan,:),1)';
    %%
    %DCTԤ��YUV����
    for tqpnihe = 1:5
    %�̶�gqpΪ26 ������Ϊ��1:5��������ͬ��tqpniheΪ��ͬ��TQPȡֵ
    
    
    coefficients = polyfit(DCTC(tqpnihe,:), YUVC, 1);
    xielv(tqpnihe) = coefficients(1);
    jieju(tqpnihe) = coefficients(2);
    %��tqpniheΪ1ʱ�õ���DCTC��YUV��ϳ���TQP26����µ�б�ʺͽؾ�
    %ʹ��TQP��
    end
    
    TQPNH = [26 32 38 44 50];
    coeffxielv = polyfit(TQPNH, xielv, 2);
    xielva = coeffxielv(1);
    xielvb = coeffxielv(2);
    xielvc = coeffxielv(3);

    coeffjieju = polyfit(TQPNH, jieju, 2);
    jiejua = coeffjieju(1);
    jiejub = coeffjieju(2);
    jiejuc = coeffjieju(3);
    %�������˶�����ϵĸ������� ����д��YUV = xielv*DCT(i)+jieju
%     X(i) =  xielva*TQP(i)*TQP(i)-xielvb*TQP(i)+xielvc;
%     J(i) = jiejua*TQP(i)*TQP(i)-jiejub*TQP(i)+jiejuc;
%     YUVp(i) = X(i)*DCT(i)+J(i);
%
%%
%ʹ��ԭʼ����YUV��ԭʼMOSб�����MOSб��
    yuvmos = polyfit(YUVC,MOSXLC,1);
   %%
    %��֤��
    TQPY = tqp(:,yanzhengjuzhen(shiyanxunhuan,:));
    GQPY = gqp(:,yanzhengjuzhen(shiyanxunhuan,:));
    MOSY = mos(:,yanzhengjuzhen(shiyanxunhuan,:));
    DCTY = dct(:,yanzhengjuzhen(shiyanxunhuan,:));
    TQPT = reshape(TQPY,200,1);
    GQPY = reshape(GQPY,200,1);
    MOSY = reshape(MOSY,200,1);
    DCTY = reshape(DCTY,200,1);
   f1 =  0.6106;
    f2 = 45.1778;
    f3 = -3.9346;
    f4 =  0.3916;
    TQS = 2.^((TQPT-4)./6);
    for yanzheng = 1:200
%     k = mod(yanzheng-1, 5) + 1;
    X(yanzheng) =  xielva*TQP(yanzheng)*TQP(yanzheng)+xielvb*TQP(yanzheng)+xielvc;
    J(yanzheng) = jiejua*TQP(yanzheng)*TQP(yanzheng)+jiejub*TQP(yanzheng)+jiejuc;
    YUVp(yanzheng) = X(yanzheng)*DCTY(yanzheng)+J(yanzheng);
    mosxielv(yanzheng) = yuvmos(1)*YUVp(yanzheng)+yuvmos(2); %б����ΪY XΪԭʼYUV����
    CMOS(yanzheng) = mosxielv(yanzheng)*TQS(yanzheng)+90;
    DG(yanzheng) = f4+f1./(1+exp(1).^((-GQPY(yanzheng)+f2)./f3));
    FMOS(yanzheng) = CMOS(yanzheng)*DG(yanzheng);
    end
    PLCC = corrcoef(FMOS,MOSY);
    plcc(shiyanxunhuan) = PLCC(1,2);
    SRCC(shiyanxunhuan) = corr(FMOS',MOSY, 'Type', 'Spearman');
    
    
end

%%
%��ͼ

TIME = linspace(1,1000,1000);
% ����ɢ��ͼ
scatter(TIME,plcc,25,'filled','MarkerFaceColor', 'blue');
ylim([0,1])
meanPLCC=mean(plcc);
varianceValue=var(plcc);
text(600, 0.2, ['Mean: ' num2str(meanPLCC, '%.4f')], 'VerticalAlignment', 'bottom', 'HorizontalAlignment', 'left', 'FontSize', 15);
text(600, 0.1, ['Variance: ' num2str(varianceValue, '%.4f')], 'VerticalAlignment', 'bottom', 'HorizontalAlignment', 'left', 'FontSize', 15);
% ������ǩ�ͱ���
xlabel('Times', 'FontSize', 20);
ylabel('PLCC', 'FontSize', 20);
set(gca, 'FontSize', 16);
% print('�������PLCC2', '-dpng');

scatter(TIME,SRCC,25,'filled','MarkerFaceColor', 'green');
ylim([0,1])
meanSRCC=mean(SRCC);
varianceValue=var(SRCC);
text(600, 0.2, ['Mean: ' num2str(meanSRCC,'%.4f')], 'VerticalAlignment', 'bottom', 'HorizontalAlignment', 'left','FontSize', 15);
text(600, 0.1, ['Variance: ' num2str(varianceValue,'%.4f')], 'VerticalAlignment', 'bottom', 'HorizontalAlignment', 'left','FontSize', 15);
% ������ǩ�ͱ���
xlabel('Times', 'FontSize', 20);
ylabel('SRCC', 'FontSize', 20);
set(gca, 'FontSize', 16);
% print('�������SRCC2', '-dpng');

% ��ʾͼ��
% grid on; % ��ʾ����



% max(abs(plcc))
% max(plcc)
% mean(abs(plcc))
% count = sum(plcc(:) > 0)
% [row, col] = find(abs(plcc) > 0.8);
% haoshuju = shujujuzhen(col,:);

% %%
% % ������ľ�����Ϊmatrix�������滻�����ʵ�ʾ���
% 
% 
% % ������չ����һά����
% vector = haoshuju(:);
% 
% % ʹ��histcounts��������ÿ�����ֵ�Ƶ��
% [counts, values] = histcounts(vector,1:17);
% 
% % ʹ��sort������Ƶ�ʽ������򣬲���ȡǰ����Ƶ����ߵ�����
% [sorted_counts, sorted_indices] = sort(counts, 'descend');
% top_six_values = values(sorted_indices(1:8));
% top_six_counts = sorted_counts(1:8);
% 
% % ��ʾ���
% disp('����Ƶ����ߵ�ǰ�˸����ֺ����ǵ�Ƶ�ʣ�');
% for i = 1:8
%     fprintf('���֣�%d��Ƶ�ʣ�%d\n', top_six_values(i), top_six_counts(i));
% end

% 
% %zuidashujuji = [2,9,16,13,12,6,14,11]
% %zuidayanzhengji = [1 3 4 5 7 8 10 15]
% %%
% for shiyanxunhuan = 1:shiyancishu
%     TQPC = tqp(:,[2,9,16,13,12,6,14,11]);
%     GQPC = gqp(:,[2,9,16,13,12,6,14,11]);
%     MOSC = mos(:,[2,9,16,13,12,6,14,11]);
%     DCTC = dct(:,[2,9,16,13,12,6,14,11]);
%     MOSXLC = MOSXL([2,9,16,13,12,6,14,11],1)';
%     YUVC = YUV([2,9,16,13,12,6,14,11],1)';
%     %%
%     %DCTԤ��YUV����
%     for tqpnihe = 1:5
%     %�̶�gqpΪ26 ������Ϊ��1:5��������ͬ��tqpniheΪ��ͬ��TQPȡֵ
%     
%     
%     coefficients = polyfit(DCTC(tqpnihe,:), YUVC, 1);
%     xielv(tqpnihe) = coefficients(1);
%     jieju(tqpnihe) = coefficients(2);
%     %��tqpniheΪ1ʱ�õ���DCTC��YUV��ϳ���TQP26����µ�б�ʺͽؾ�
%     %ʹ��TQP��
%     end
%     
%     TQPNH = [26 32 38 44 50];
%     coeffxielv = polyfit(TQPNH, xielv, 2);
%     xielva = coeffxielv(1);
%     xielvb = coeffxielv(2);
%     xielvc = coeffxielv(3);
% 
%     coeffjieju = polyfit(TQPNH, jieju, 2);
%     jiejua = coeffjieju(1);
%     jiejub = coeffjieju(2);
%     jiejuc = coeffjieju(3);
%     %�������˶�����ϵĸ������� ����д��YUV = xielv*DCT(i)+jieju
% %     X(i) =  xielva*TQP(i)*TQP(i)-xielvb*TQP(i)+xielvc;
% %     J(i) = jiejua*TQP(i)*TQP(i)-jiejub*TQP(i)+jiejuc;
% %     YUVp(i) = X(i)*DCT(i)+J(i);
% %
% %%
% %ʹ��ԭʼ����YUV��ԭʼMOSб�����MOSб��
%     yuvmos = polyfit(YUVC,MOSXLC,1);
%    %%
%     %��֤��
%     TQPY = tqp(:,[1 3 4 5 7 8 10 15]);
%     GQPY = gqp(:,[1 3 4 5 7 8 10 15]);
%     MOSY = mos(:,[1 3 4 5 7 8 10 15]);
%     DCTY = dct(:,[1 3 4 5 7 8 10 15]);
%     TQPT = reshape(TQPY,200,1);
%     GQPY = reshape(GQPY,200,1);
%     MOSY = reshape(MOSY,200,1);
%     DCTY = reshape(DCTY,200,1);
%    f1 =  0.6106;
%     f2 = 45.1778;
%     f3 = -3.9346;
%     f4 =  0.3916;
%     TQS = 2.^((TQPT-4)./6);
%     for yanzheng = 1:200
% %     k = mod(yanzheng-1, 5) + 1;
%     X(yanzheng) =  xielva*TQP(yanzheng)*TQP(yanzheng)+xielvb*TQP(yanzheng)+xielvc;
%     J(yanzheng) = jiejua*TQP(yanzheng)*TQP(yanzheng)+jiejub*TQP(yanzheng)+jiejuc;
%     YUVp(yanzheng) = X(yanzheng)*DCTY(yanzheng)+J(yanzheng);
%     mosxielv(yanzheng) = yuvmos(1)*YUVp(yanzheng)+yuvmos(2); %б����ΪY XΪԭʼYUV����
%     CMOS(yanzheng) = mosxielv(yanzheng)*TQS(yanzheng)+90;
%     DG(yanzheng) = f4+f1./(1+exp(1).^((-GQPY(yanzheng)+f2)./f3));
%     FMOS(yanzheng) = CMOS(yanzheng)*DG(yanzheng);
%     end
%     PLCC = corrcoef(FMOS,MOSY);
%     plcc(shiyanxunhuan) = PLCC(1,2);
%     
% end
