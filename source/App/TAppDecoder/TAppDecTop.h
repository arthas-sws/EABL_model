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

 /** \file     TAppDecTop.h
     \brief    Decoder application class (header)
 */

#ifndef __TAPPDECTOP__
#define __TAPPDECTOP__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TLibVideoIO/TVideoIOYuv.h"
#include "TLibCommon/TComList.h"
#include "TLibCommon/TComPicYuv.h"
#include "TLibDecoder/TDecTop.h"
#include "TAppDecCfg.h"

 //! \ingroup TAppDecoder
 //! \{

 // ====================================================================================================================
 // Class definition
 // ====================================================================================================================

 /// decoder application class 解码器应用类
class TAppDecTop : public TAppDecCfg
{
private:
    // class interface 类接口
    TDecTop                         m_cTDecTop;                     ///< decoder class 译码器类
    TVideoIOYuv                     m_cTVideoIOYuvReconFile;        ///< reconstruction YUV class 重建YUV级

    // for output control 用于输出控制
    Int                             m_iPOCLastDisplay;              ///< last POC in display order 最后一个POC显示顺序
    std::ofstream                   m_seiMessageFileStream;         ///< Used for outputing SEI messages. 用于输出SEI消息。

    SEIColourRemappingInfo* m_pcSeiColourRemappingInfoPrevious;

public:
    TAppDecTop();
    virtual ~TAppDecTop() {}

    Void  create(); ///< create internal members 创建内部成员
    Void  destroy(); ///< destroy internal members销毁内部成员
    Void  decode(); ///< main decoding function 主要解码功能
    UInt  getNumberOfChecksumErrorsDetected() const { return m_cTDecTop.getNumberOfChecksumErrorsDetected(); }

protected:
    Void  xCreateDecLib(); ///< create internal classes 创建内部类
    Void  xDestroyDecLib(); ///< destroy internal classes 销毁内部类
    Void  xInitDecLib(); ///< initialize decoder class 初始化解码器类

    Void  xWriteOutput(TComList<TComPic*>* pcListPic, UInt tId); ///< write YUV to file 将YUV写入文件
    Void  xFlushOutput(TComList<TComPic*>* pcListPic); ///< flush all remaining decoded pictures to file 将所有剩余的解码图片刷新到文件中
    Bool  isNaluWithinTargetDecLayerIdSet(InputNALUnit* nalu); ///< check whether given Nalu is within targetDecLayerIdSet 检查给定的Nalu是否在targetDecLayerIdSet中

private:
    Void applyColourRemapping(const TComPicYuv& pic, SEIColourRemappingInfo& pCriSEI, const TComSPS& activeSPS);
    Void xOutputColourRemapPic(TComPic* pcPic);
};

//! \}

#endif

