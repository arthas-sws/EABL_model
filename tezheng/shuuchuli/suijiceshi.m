clc;clear all;

%%随机试验
%第一步读取数据
%第二部分出来测试集和验证集
%第三步利用测试集建立模型 用验证集来检验PLCC
%第四步
%%
%第一步读取数据
data= xlsread('最终数据.xlsx');
datayuv = xlsread('DCT.xlsx','原始点云YUV数据');
TQP = data(:,1);
GQP = data(:,5);
MOS = data(:,2);
DCT = data(:,3);
TQS = [12.6992084157456,25.3984168314912,50.7968336629824,101.593667325965,203.187334651930];
MOSXL =datayuv(2:end,6);
%这里只选取了32的YUV数据
YUV = datayuv(2:end,3);
tqp = reshape(TQP,25,16);
gqp = reshape(GQP,25,16);
mos = reshape(MOS,25,16);
dct = reshape(DCT,25,16);%一列是一种点云
%%
%第二部分出来测试集和验证集
numbers = [];
shiyancishu = 10000;%输入你想进行多少次随机试验
%%
%无调整
for i = 1:shiyancishu
% 生成1到16之间的八个不重复的随机数 存储在shujujuzhen中
shujujuzhen(i,:) = randperm(16, 8);
% 生成另一个包含1到16之间的八个不同的随机数的矩阵
remaining_numbers(i,:) = setdiff(1:16, shujujuzhen(i,:));
yanzhengjuzhen(i,:) = setdiff(1:16, shujujuzhen(i,:));;
end
% A = yanzhengjuzhen+shujujuzhen;
% row_sums = sum(A, 2);
% 1+2+3+4+5+6+7+8+9+10+11+12+13+14+15+16
%验证可以生成目标矩阵
%%
% %有调整
% shaixuan = [4 7 14 15 ];%
% suiji = [1 2 3 5 6 8 9 10 11 12 13 16];
% for i =1:shiyancishu
% % 随机挑选四个数字的索引
% indices_A(i,:) = randperm(length(suiji), 4);
% % 构建矩阵A
% A(i,:) = suiji(indices_A(i,:));
% % 剩余的数字的索引
% indices_B(i,:) = setdiff(1:length(suiji), indices_A(i,:));
% % 构建矩阵B
% B(i,:) = suiji(indices_B(i,:));
% % sum(A)+sum(B)+sum(shaixuan)
% shujujuzhen(i,:) = horzcat(suiji(indices_A(i,:)), shaixuan);
% yanzhengjuzhen = B;
% shujuhe(i) = sum(shujujuzhen(i,:))+sum(yanzhengjuzhen(i,:));
% end

%%
%第三步 一百次实验

for shiyanxunhuan = 1:shiyancishu
    TQPC = tqp(:,shujujuzhen(shiyanxunhuan,:));
    GQPC = gqp(:,shujujuzhen(shiyanxunhuan,:));
    MOSC = mos(:,shujujuzhen(shiyanxunhuan,:));
    DCTC = dct(:,shujujuzhen(shiyanxunhuan,:));
    MOSXLC = MOSXL(shujujuzhen(shiyanxunhuan,:),1)';
    YUVC = YUV(shujujuzhen(shiyanxunhuan,:),1)';
    %%
    %DCT预测YUV数据
    for tqpnihe = 1:5
    %固定gqp为26 即索引为（1:5，：）不同的tqpnihe为不同的TQP取值
    
    
    coefficients = polyfit(DCTC(tqpnihe,:), YUVC, 1);
    xielv(tqpnihe) = coefficients(1);
    jieju(tqpnihe) = coefficients(2);
    %当tqpnihe为1时得到了DCTC和YUV拟合出的TQP26情况下的斜率和截距
    %使用TQP作
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
    %现在有了二次拟合的各项数据 可以写出YUV = xielv*DCT(i)+jieju
%     X(i) =  xielva*TQP(i)*TQP(i)-xielvb*TQP(i)+xielvc;
%     J(i) = jiejua*TQP(i)*TQP(i)-jiejub*TQP(i)+jiejuc;
%     YUVp(i) = X(i)*DCT(i)+J(i);
%
%%
%使用原始点云YUV和原始MOS斜率拟合MOS斜率
    yuvmos = polyfit(YUVC,MOSXLC,1);
   %%
    %验证集
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
    mosxielv(yanzheng) = yuvmos(1)*YUVp(yanzheng)+yuvmos(2); %斜率作为Y X为原始YUV数据
    CMOS(yanzheng) = mosxielv(yanzheng)*TQS(yanzheng)+90;
    DG(yanzheng) = f4+f1./(1+exp(1).^((-GQPY(yanzheng)+f2)./f3));
    FMOS(yanzheng) = CMOS(yanzheng)*DG(yanzheng);
    end
    PLCC = corrcoef(FMOS,MOSY);
    plcc(shiyanxunhuan) = PLCC(1,2);
    SRCC(shiyanxunhuan) = corr(FMOS',MOSY, 'Type', 'Spearman');
    
    
end

%%
%画图

TIME = linspace(1,1000,1000);
% 绘制散点图
scatter(TIME,plcc,25,'filled','MarkerFaceColor', 'blue');
ylim([0,1])
meanPLCC=mean(plcc);
varianceValue=var(plcc);
text(600, 0.2, ['Mean: ' num2str(meanPLCC, '%.4f')], 'VerticalAlignment', 'bottom', 'HorizontalAlignment', 'left', 'FontSize', 15);
text(600, 0.1, ['Variance: ' num2str(varianceValue, '%.4f')], 'VerticalAlignment', 'bottom', 'HorizontalAlignment', 'left', 'FontSize', 15);
% 添加轴标签和标题
xlabel('Times', 'FontSize', 20);
ylabel('PLCC', 'FontSize', 20);
set(gca, 'FontSize', 16);
% print('随机试验PLCC2', '-dpng');

scatter(TIME,SRCC,25,'filled','MarkerFaceColor', 'green');
ylim([0,1])
meanSRCC=mean(SRCC);
varianceValue=var(SRCC);
text(600, 0.2, ['Mean: ' num2str(meanSRCC,'%.4f')], 'VerticalAlignment', 'bottom', 'HorizontalAlignment', 'left','FontSize', 15);
text(600, 0.1, ['Variance: ' num2str(varianceValue,'%.4f')], 'VerticalAlignment', 'bottom', 'HorizontalAlignment', 'left','FontSize', 15);
% 添加轴标签和标题
xlabel('Times', 'FontSize', 20);
ylabel('SRCC', 'FontSize', 20);
set(gca, 'FontSize', 16);
% print('随机试验SRCC2', '-dpng');

% 显示图形
% grid on; % 显示网格



% max(abs(plcc))
% max(plcc)
% mean(abs(plcc))
% count = sum(plcc(:) > 0)
% [row, col] = find(abs(plcc) > 0.8);
% haoshuju = shujujuzhen(col,:);

% %%
% % 假设你的矩阵名为matrix（可以替换成你的实际矩阵）
% 
% 
% % 将矩阵展开成一维向量
% vector = haoshuju(:);
% 
% % 使用histcounts函数计算每个数字的频率
% [counts, values] = histcounts(vector,1:17);
% 
% % 使用sort函数对频率进行排序，并获取前六个频率最高的数字
% [sorted_counts, sorted_indices] = sort(counts, 'descend');
% top_six_values = values(sorted_indices(1:8));
% top_six_counts = sorted_counts(1:8);
% 
% % 显示结果
% disp('出现频率最高的前八个数字和它们的频率：');
% for i = 1:8
%     fprintf('数字：%d，频率：%d\n', top_six_values(i), top_six_counts(i));
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
%     %DCT预测YUV数据
%     for tqpnihe = 1:5
%     %固定gqp为26 即索引为（1:5，：）不同的tqpnihe为不同的TQP取值
%     
%     
%     coefficients = polyfit(DCTC(tqpnihe,:), YUVC, 1);
%     xielv(tqpnihe) = coefficients(1);
%     jieju(tqpnihe) = coefficients(2);
%     %当tqpnihe为1时得到了DCTC和YUV拟合出的TQP26情况下的斜率和截距
%     %使用TQP作
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
%     %现在有了二次拟合的各项数据 可以写出YUV = xielv*DCT(i)+jieju
% %     X(i) =  xielva*TQP(i)*TQP(i)-xielvb*TQP(i)+xielvc;
% %     J(i) = jiejua*TQP(i)*TQP(i)-jiejub*TQP(i)+jiejuc;
% %     YUVp(i) = X(i)*DCT(i)+J(i);
% %
% %%
% %使用原始点云YUV和原始MOS斜率拟合MOS斜率
%     yuvmos = polyfit(YUVC,MOSXLC,1);
%    %%
%     %验证集
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
%     mosxielv(yanzheng) = yuvmos(1)*YUVp(yanzheng)+yuvmos(2); %斜率作为Y X为原始YUV数据
%     CMOS(yanzheng) = mosxielv(yanzheng)*TQS(yanzheng)+90;
%     DG(yanzheng) = f4+f1./(1+exp(1).^((-GQPY(yanzheng)+f2)./f3));
%     FMOS(yanzheng) = CMOS(yanzheng)*DG(yanzheng);
%     end
%     PLCC = corrcoef(FMOS,MOSY);
%     plcc(shiyanxunhuan) = PLCC(1,2);
%     
% end
