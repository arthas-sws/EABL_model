clc;clear all;
%�������� ����׼��
data2 = xlsread('geo out.xlsx');
data= xlsread('geo out.xlsx','Sheet3');
MOS= xlsread('WPC2.0_MOS.xlsx');
MOS = MOS(:,3);
yuanshifenbianlv = data(:,1);%ԭʼ�ֱ���
ldfbl4 = data(:,17);%4���ȷֱ���
ldfbl8 = data(:,21);
ldfbl16 = data(:,25);
ldfbl32 = data(:,29);
chongjian = ldfbl4+ldfbl8+ldfbl16+ldfbl32;
guiyichongjian = chongjian./ yuanshifenbianlv;
%���ֵķֱ�����Ϊ���� �ֱܷ���Ϊ��ĸ
guiyi4 = ldfbl4./yuanshifenbianlv;
guiyi8 = ldfbl8./yuanshifenbianlv;
guiyi16 = ldfbl16./yuanshifenbianlv;
guiyi32 = ldfbl32./yuanshifenbianlv;
GQP = [26 32 38 44 50];
% ��չ��80x1�ľ��� B
GQP80 = repmat(GQP, 16, 1)';
GQP80 = reshape(GQP80,80,1);
Dguiyi4 = 1./guiyi4;
Dguiyi8 = 1./guiyi8;
Dguiyi16 = 1./guiyi16;
Dguiyi32 = 1./guiyi32;
Dguiyizong = 1./guiyichongjian;
%%
%��������
%%
TEST =reshape( guiyichongjian,5,16);
for i = 1:5
ans(i) = mean(TEST(i,:));
end
A = corrcoef(GQP,ans);
%%
%����ͼ��
for i= 1:49
    A = corrcoef(MOS,data2(:,i));
    plcc(i) = A(1,2);
end
max(abs(plcc))
