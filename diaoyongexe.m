
% 1. 定义exe程序的路径
exePath = 'TAppDecoder.exe';

% 2. 定义传递给exe程序的参数
param1 = '-b bag_geo_26_col_32_GOF0_attribute.bin';  % 输入你的参数
param2 = '>out.txt';  % 输出的含有TE的txt文件
%param3 = '-o E:/MPEG-PCC-TMC2/external/HM-16.20+SCM-8.8/test/bag_geo_26_col_26_GOF0_attribute.yuv' ; %yuv输出 可以注释掉


% 3. 构造命令字符串，包含.exe路径和参数
commandStr = sprintf('"%s" %s %s', exePath, param1, param2);

% 4. 使用 system 函数执行命令
[status, cmdout] = system(commandStr);

% 5. 输出结果
if status == 0
    disp('程序执行成功');
    disp(cmdout);  % 输出exe程序的返回结果
else
    disp('程序执行失败');
    disp(cmdout);  % 输出错误信息
end