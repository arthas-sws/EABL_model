% ����ʾ�����ݣ�1�������ڣ�0�������ڣ�-1�����޷��Ƚ�
results = [1 0 -1; 1 1 0; -1 0 1];

% ��ͼ�δ�С����Ϊ�����ݾ����С��ͬ
figure();
 imagesc(results); % ʹ�� imagesc ���� imshow

colormap([0.5 0.5 0.5; 1 1 1; 0 0 0]); % ���ú졢�̡���������ɫ
% colormap([0.5 0.5 0.5; 1 1 1; 0 0 0; 0.5 0.5 0.5]); % ����������ɫ��ƥ������ֵ

% colorbar; % ��ʾ��ɫ��

% ���ӱ���ͱ�ǩ
title('������ʵ����ͼ');
xlabel('�㷨���');
ylabel('ʵ����');

% ����ͼ��
% legend({'0: ����', '1: ����', '-1: �޷��Ƚ�'}, 'Location', 'Best');