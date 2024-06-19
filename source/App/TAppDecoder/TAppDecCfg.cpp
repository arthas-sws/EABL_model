/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2017, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

 /** \file     TAppDecCfg.cpp
     \brief    Decoder configuration class
 */

#include <cstdio>
#include <cstring>
#include <string>
#include "TAppDecCfg.h"
#include "TAppCommon/program_options_lite.h"
#include "TLibCommon/TComChromaFormat.h"
#ifdef WIN32
#define strdup _strdup
#endif

using namespace std;
namespace po = df::program_options_lite;

//! \ingroup TAppDecoder
//! \{

// ====================================================================================================================
// Public member functions 公共成员函数
// ====================================================================================================================

/** \param argc number of arguments param argc 参数个数
    \param argv array of arguments param argv参数数组
 */
Bool TAppDecCfg::parseCfg(Int argc, TChar* argv[])
{
    Bool do_help = false;
    string cfg_TargetDecLayerIdSetFile;
    string outputColourSpaceConvert;
    Int warnUnknowParameter = 0;

    po::Options opts;
    opts.addOptions()


        ("help", do_help, false, "this help text")
        ("BitstreamFile,b", m_bitstreamFileName, string(""), "bitstream input file name")//位流输入文件名
        ("ReconFile,o", m_reconFileName, string(""), "reconstructed YUV output file name\n"//重建的YUV输出文件名
            "YUV writing is skipped if omitted")//如果省略YUV写作，则跳过
        ("WarnUnknowParameter,w", warnUnknowParameter, 0, "warn for unknown configuration parameters instead of failing")//警告未知的配置参数，而不是失败
        ("SkipFrames,s", m_iSkipFrame, 0, "number of frames to skip before random access")//随机访问前要跳过的帧数
        ("OutputBitDepth,d", m_outputBitDepth[CHANNEL_TYPE_LUMA], 0, "bit depth of YUV output luma component (default: use 0 for native depth)")//YUV输出亮度组件的位深度(默认值:本机深度为0)
        ("OutputBitDepthC,d", m_outputBitDepth[CHANNEL_TYPE_CHROMA], 0, "bit depth of YUV output chroma component (default: use 0 for native depth)")//YUV输出色度分量的位深度(默认:本机深度使用0)”)
        ("OutputColourSpaceConvert", outputColourSpaceConvert, string(""), "Colour space conversion to apply to input 444 video. Permitted values are (empty string=UNCHANGED) " + getListOfColourSpaceConverts(false))//色彩空间转换适用于输入444视频。允许的值为(空字符串=未更改)
        ("MaxTemporalLayer,t", m_iMaxTemporalLayer, -1, "Maximum Temporal Layer to be decoded. -1 to decode all layers")//要解码的最大时间层。-1解码所有层
        ("SEIDecodedPictureHash", m_decodedPictureHashSEIEnabled, 1, "Control handling of decoded picture hash SEI messages\n"//控制处理解码的图片散列SEI消息\n
            "\t1: check hash in SEI messages if available in the bitstream\n"//检查SEI消息中的哈希是否在位流中可用
            "\t0: ignore SEI message")//忽略SEI消息
        ("SEINoDisplay", m_decodedNoDisplaySEIEnabled, true, "Control handling of decoded no display SEI messages")//控件处理解码后的不显示SEI消息
        ("TarDecLayerIdSetFile,l", cfg_TargetDecLayerIdSetFile, string(""), "targetDecLayerIdSet file name. The file should include white space separated LayerId values to be decoded. Omitting the option or a value of -1 in the file decodes all layers.")//targetDecLayerIdSet文件名。该文件应该包含要解码的空白分隔的LayerId值。在文件中省略选项或-1值将解码所有层。
        ("RespectDefDispWindow,w", m_respectDefDispWindow, 0, "Only output content inside the default display window\n")//只输出默认显示窗口内的内容
        ("SEIColourRemappingInfoFilename", m_colourRemapSEIFileName, string(""), "Colour Remapping YUV output file name. If empty, no remapping is applied (ignore SEI message)\n")//颜色重映射YUV输出文件名。如果为空，则不应用重映射(忽略SEI消息)
#if O0043_BEST_EFFORT_DECODING
        ("ForceDecodeBitDepth", m_forceDecodeBitDepth, 0U, "Force the decoder to operate at a particular bit-depth (best effort decoding)")//强制解码器在特定的位深度上操作(尽力解码)
#endif
        ("OutputDecodedSEIMessagesFilename", m_outputDecodedSEIMessagesFilename, string(""), "When non empty, output decoded SEI messages to the indicated file. If file is '-', then output to stdout\n")//当非空时，将解码后的SEI消息输出到指定的文件。如果文件为'-'，则输出到标准输出
        ("ClipOutputVideoToRec709Range", m_bClipOutputVideoToRec709Range, false, "If true then clip output video to the Rec. 709 Range on saving")//如果为true，则在保存时将输出视频剪辑到Rec. 709范围
#if MCTS_ENC_CHECK
        ("TMCTSCheck", m_tmctsCheck, false, "If enabled, the decoder checks for violations of mc_exact_sample_value_match_flag in Temporal MCTS ")//如果启用，解码器将检查Temporal MCTS中是否违反mc_exact_sample_value_match_flag
#endif
        ;

    po::setDefaults(opts);
    po::ErrorReporter err;
    const list<const TChar*>& argv_unhandled = po::scanArgv(opts, argc, (const TChar**)argv, err);

    for (list<const TChar*>::const_iterator it = argv_unhandled.begin(); it != argv_unhandled.end(); it++)
    {
        fprintf(stderr, "Unhandled argument ignored: `%s'\n", *it);
    }

    if (argc == 1 || do_help)
    {
        po::doHelp(cout, opts);
        return false;
    }

    if (err.is_errored)
    {
        if (!warnUnknowParameter)
        {
            /* errors have already been reported to stderr */ //错误已经报告到stderr
            return false;
        }
    }

    m_outputColourSpaceConvert = stringToInputColourSpaceConvert(outputColourSpaceConvert, false);
    if (m_outputColourSpaceConvert >= NUMBER_INPUT_COLOUR_SPACE_CONVERSIONS)
    {
        fprintf(stderr, "Bad output colour space conversion string\n");//不好的输出颜色空间转换字符串
        return false;
    }

    if (m_bitstreamFileName.empty())
    {
        fprintf(stderr, "No input file specified, aborting\n");//没有指定输入文件，中止
        return false;
    }

    if (!cfg_TargetDecLayerIdSetFile.empty())
    {
        FILE* targetDecLayerIdSetFile = fopen(cfg_TargetDecLayerIdSetFile.c_str(), "r");
        if (targetDecLayerIdSetFile)
        {
            Bool isLayerIdZeroIncluded = false;
            while (!feof(targetDecLayerIdSetFile))
            {
                Int layerIdParsed = 0;
                if (fscanf(targetDecLayerIdSetFile, "%d ", &layerIdParsed) != 1)
                {
                    if (m_targetDecLayerIdSet.size() == 0)
                    {
                        fprintf(stderr, "No LayerId could be parsed in file %s. Decoding all LayerIds as default.\n", cfg_TargetDecLayerIdSetFile.c_str());//在文件%s中无法解析LayerId。解码所有layerid为默认值。
                    }
                    break;
                }
                if (layerIdParsed == -1) // The file includes a -1, which means all LayerIds are to be decoded. 该文件包含一个-1，这意味着所有的layerid都将被解码。
                {
                    m_targetDecLayerIdSet.clear(); // Empty set means decoding all layers. 空集表示解码所有层。
                    break;
                }
                if (layerIdParsed < 0 || layerIdParsed >= MAX_NUM_LAYER_IDS)
                {
                    fprintf(stderr, "Warning! Parsed LayerId %d is not within allowed range [0,%d]. Ignoring this value.\n", layerIdParsed, MAX_NUM_LAYER_IDS - 1);//警告!解析LayerId %d不在允许范围内[0，%d]。忽略这个值。
                }
                else
                {
                    isLayerIdZeroIncluded = layerIdParsed == 0 ? true : isLayerIdZeroIncluded;
                    m_targetDecLayerIdSet.push_back(layerIdParsed);
                }
            }
            fclose(targetDecLayerIdSetFile);
            if (m_targetDecLayerIdSet.size() > 0 && !isLayerIdZeroIncluded)
            {
                fprintf(stderr, "TargetDecLayerIdSet must contain LayerId=0, aborting");//TargetDecLayerIdSet必须包含LayerId=0，中止
                return false;
            }
        }
        else
        {
            fprintf(stderr, "File %s could not be opened. Using all LayerIds as default.\n", cfg_TargetDecLayerIdSetFile.c_str());//无法打开文件%s。使用所有layerid作为默认值。
        }
    }

    return true;
}

//! \}
