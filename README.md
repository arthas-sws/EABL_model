# EABL Model

This repository provides the Energy-Adaptive Bitstream-Layer (EABL) model developed using WPC2.0.

## Extract TE with the Windows decoder

From the folder containing `TAppDecoder.exe` and the sample attribute bitstream, run this command in Windows Command Prompt.

```bat
TAppDecoder.exe -b "bag_geo_26_col_32_GOF0_attribute.bin"
```

The decoder prints the following result for the provided sample.

```text
TE=1424068304
```

To save the result, append `> out.txt` to the command. The decoder does not need a reconstructed YUV output file to extract TE.

## Calculate the EABL score in MATLAB

Set MATLAB's current folder to [`matlab`](matlab) to use the editable [`EABL.m`](matlab/EABL.m). The repository root also contains `EABL.p` for existing workflows. Pass the raw `TE` value printed by the decoder. The function applies its own normalization.

```matlab
Fmos_value = EABL(TQP, GQP, TE);
```

You can also run [`diaoyongexe.m`](diaoyongexe.m) from the repository root to extract TE into `out.txt`.

## Evaluation metrics

With MATLAB's current folder set to `matlab`, pass paired vectors of subjective MOS and predicted scores to [`eabl_quality_metrics.m`](matlab/eabl_quality_metrics.m). It requires Statistics and Machine Learning Toolbox.

```matlab
[plcc, srcc, rmse] = eabl_quality_metrics(mos, predictedScores);
```

SRCC uses the raw predictions. PLCC and RMSE use predictions mapped to the MOS scale with a four-parameter logistic fit.

## Build the decoder from source

Start with HM 16.20 + SCM 8.8 and copy this repository's `source/` tree over the corresponding HM source tree. This includes the decoder application and library as well as `source/Lib/TLibCommon/TComTrQuant.cpp`, which accumulates the transform energy. On Windows, build the `TAppDecoder` target in x64 Release using the HM Visual Studio solution. The build was tested with Visual Studio 2022 after retargeting the HM projects to the installed toolset and Windows SDK.

For other V-PCC streams, first extract the HEVC attribute sub-bitstream with TMC2. Pass that sub-bitstream to `TAppDecoder.exe`.

The HM-derived source and decoder binary are accompanied by the [HM copyright and BSD license notice](LICENSE-HM.txt).
