function Fmos = EABL(TQP, GQP, TE)
    % 常量定义
    a1 = 7.261e-5;
    a2 = -0.0044;
    a3 = 0.1365;
    b1 = 3.497e-5;
    b2 = 0.0310;
    b3 = 3.0719;
    p1 = -0.0010;
    p2 = 0.0563;
    p3 = 0.2081;
    c = 0.0050;
    d = -0.2713;

    % 计算 TQS
    TQS = 2^((TQP - 4) / 6);

    % 调整 TE
    TE = TE / (2560 * 1440);

    % 计算 DG
    DG = p1 * GQP^2 + p2 * GQP + p3;

    % 计算 X 和 J
    X = a1 * TQP^2 + a2 * TQP + a3;
    J = b1 * TQP^2 + b2 * TQP + b3;

    % 计算 YUVp200
    YUVp200 = X * TE + J;

    % 计算 xielv (斜率)
    xielv = c * YUVp200 + d;

    % 计算 CMOS
    CMOS = xielv * TQS + 90;

    % 计算 Fmos
    Fmos = CMOS * DG;
end
