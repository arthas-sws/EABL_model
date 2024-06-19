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

/** \file     TAppDecCfg.h
    \brief    Decoder configuration class (header)
*/

#ifndef __TAPPDECCFG__
#define __TAPPDECCFG__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TLibCommon/CommonDef.h"
#include <vector>

//! \ingroup TAppDecoder
//! \{

// ====================================================================================================================
// Class definition
// ====================================================================================================================

/// Decoder configuration class
class TAppDecCfg
{
protected:
  std::string   m_bitstreamFileName;                    ///< input bitstream file name 输入位流文件名
  std::string   m_reconFileName;                        ///< output reconstruction file name 输出重构文件名
  Int           m_iSkipFrame;                           ///< counter for frames prior to the random access point to skip  在要跳过的随机访问点之前的帧的计数器
  Int           m_outputBitDepth[MAX_NUM_CHANNEL_TYPE]; ///< bit depth used for writing output 写入输出时使用的位深度
  InputColourSpaceConversion m_outputColourSpaceConvert;

  Int           m_iMaxTemporalLayer;                  ///< maximum temporal layer to be decoded 要解码的最大时间层
  Int           m_decodedPictureHashSEIEnabled;       ///< Checksum(3)/CRC(2)/MD5(1)/disable(0) acting on decoded picture hash SEI message 校验和(3)/CRC(2)/MD5(1)/disable(0)作用于解码后的图片散列SEI消息
  Bool          m_decodedNoDisplaySEIEnabled;         ///< Enable(true)/disable(false) writing only pictures that get displayed based on the no display SEI message 启用(true)/禁用(false)只写入基于no display SEI消息显示的图片
  std::string   m_colourRemapSEIFileName;             ///< output Colour Remapping file name output颜色重映射文件名
  std::vector<Int> m_targetDecLayerIdSet;             ///< set of LayerIds to be included in the sub-bitstream extraction process. 子比特流提取过程中包含的LayerIds集。
  Int           m_respectDefDispWindow;               ///< Only output content inside the default display window 只输出默认显示窗口内的内容
#if O0043_BEST_EFFORT_DECODING
  UInt          m_forceDecodeBitDepth;                ///< if non-zero, force the bit depth at the decoder (best effort decoding)
#endif
  std::string   m_outputDecodedSEIMessagesFilename;   ///< filename to output decoded SEI messages to. If '-', then use stdout. If empty, do not output details. 输出已解码的SEI消息的文件名。如果'-'，则使用stdout。如果为空，不输出详细信息。
  Bool          m_bClipOutputVideoToRec709Range;      ///< If true, clip the output video to the Rec 709 range on saving. 如果为真，在保存时将输出视频剪辑到Rec 709范围。
#if MCTS_ENC_CHECK
  Bool          m_tmctsCheck;
#endif

public:
  TAppDecCfg()
  : m_bitstreamFileName()
  , m_reconFileName()
  , m_iSkipFrame(0)
  // m_outputBitDepth array initialised below
  , m_outputColourSpaceConvert(IPCOLOURSPACE_UNCHANGED)
  , m_iMaxTemporalLayer(-1)
  , m_decodedPictureHashSEIEnabled(0)
  , m_decodedNoDisplaySEIEnabled(false)
  , m_colourRemapSEIFileName()
  , m_targetDecLayerIdSet()
  , m_respectDefDispWindow(0)
#if O0043_BEST_EFFORT_DECODING
  , m_forceDecodeBitDepth(0)
#endif
  , m_outputDecodedSEIMessagesFilename()
  , m_bClipOutputVideoToRec709Range(false)
#if MCTS_ENC_CHECK
  , m_tmctsCheck(false)
#endif
  {
    for (UInt channelTypeIndex = 0; channelTypeIndex < MAX_NUM_CHANNEL_TYPE; channelTypeIndex++)
    {
      m_outputBitDepth[channelTypeIndex] = 0;
    }
  }

  virtual ~TAppDecCfg() {}

  Bool  parseCfg        ( Int argc, TChar* argv[] );   ///< initialize option class from configuration
};

//! \}

#endif


