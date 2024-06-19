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

 /** \file     TComDataCU.h
     \brief    CU data structure (header) CU数据结构(头)
     \todo     not all entities are documented 并不是所有的实体都有文档
 */

#ifndef __TCOMDATACU__
#define __TCOMDATACU__

#include <algorithm>
#include <vector>

 // Include files 包含文件
#include "CommonDef.h"
#include "TComMotionInfo.h"
#include "TComSlice.h"
#include "TComRdCost.h"
#include "TComPattern.h"

//! \ingroup TLibCommon
//! \{

class TComTU; // forward declaration 前置声明

static const UInt NUM_MOST_PROBABLE_MODES = 3;

// ====================================================================================================================
// Class definition 类定义
// ====================================================================================================================

/// CU data structure class CU数据结构类
class TComDataCU
{
public:

    // -------------------------------------------------------------------------------------------------------------------
    // class pointers 类的指针
    // -------------------------------------------------------------------------------------------------------------------

    TComPic* m_pcPic;                                ///< picture class pointer 图片类指针
    TComSlice* m_pcSlice;                              ///< slice header pointer 切片头指针

    // -------------------------------------------------------------------------------------------------------------------
    // CU description
    // -------------------------------------------------------------------------------------------------------------------

    UInt          m_ctuRsAddr;                            ///< CTU (also known as LCU) address in a slice (Raster-scan address, as opposed to tile-scan/encoding order). CTU(也称为LCU)切片中的地址(栅格扫描地址，而不是平铺扫描/编码顺序)。
    UInt          m_absZIdxInCtu;                         ///< absolute address in a CTU. It's Z scan order CTU的绝对地址。这是Z扫描顺序
    UInt          m_uiCUPelX;                             ///< CU position in a pixel (X) CU在像素(X)中的位置
    UInt          m_uiCUPelY;                             ///< CU position in a pixel (Y) CU在像素(Y)中的位置
    UInt          m_uiNumPartition;                       ///< total number of minimum partitions in a CU CU的最小分区数
    UChar* m_puhWidth;                             ///< array of widths 宽度数组
    UChar* m_puhHeight;                            ///< array of heights 高度数组
    UChar* m_puhDepth;                             ///< array of depths 深度数组
    Int           m_unitSize;                             ///< size of a "minimum partition" “最小分区”的大小

    // -------------------------------------------------------------------------------------------------------------------
    // CU data
    // -------------------------------------------------------------------------------------------------------------------
    
    Bool* m_skipFlag;                             ///< array of skip flags 跳过标志数组
    SChar* m_pePartSize;                           ///< array of partition sizes 分区大小数组
    SChar* m_pePredMode;                           ///< array of prediction modes 预测模式阵列
    SChar* m_crossComponentPredictionAlpha[MAX_NUM_COMPONENT]; ///< array of cross-component prediction alpha values 跨组件预测alpha值数组
    Bool* m_CUTransquantBypass;                   ///< array of cu_transquant_bypass flags cu_transquant_bypass标志数组
    SChar* m_phQP;                                 ///< array of QP values QP值数组
    UChar* m_ChromaQpAdj;                          ///< array of chroma QP adjustments (indexed). when value = 0, cu_chroma_qp_offset_flag=0; when value>0, indicates cu_chroma_qp_offset_flag=1 and cu_chroma_qp_offset_idx=value-1
    UInt          m_codedChromaQpAdj;                     ///<色度QP调整数组(索引)。当value =0时，cu_chroma_qp_offset_flag=0;当值为>0时，表示cu_chroma_qp_offset_flag=1, cu_chroma_qp_offset_idx=value-1
    UChar* m_puhTrIdx;                             ///< array of transform indices 变换下标数组
    UChar* m_puhTransformSkip[MAX_NUM_COMPONENT];  ///< array of transform skipping flags 转换跳过标志的数组
    UChar* m_puhCbf[MAX_NUM_COMPONENT];            ///< array of coded block flags (CBF) 编码块标志数组
    TComCUMvField m_acCUMvField[NUM_REF_PIC_LIST_01];    ///< array of motion vectors, and includes intra block copying vector field. 运动矢量数组，包括块内复制矢量字段。
    TCoeff* m_pcTrCoeff[MAX_NUM_COMPONENT];         ///< array of transform coefficient buffers (0->Y, 1->Cb, 2->Cr) 变换系数缓冲数组(0->Y, 1->Cb, 2->Cr)
#if ADAPTIVE_QP_SELECTION
    TCoeff* m_pcArlCoeff[MAX_NUM_COMPONENT];        ///< ARL coefficient buffer (0->Y, 1->Cb, 2->Cr) ARL系数缓冲(0->Y, 1->Cb, 2->Cr)
    Bool          m_ArlCoeffIsAliasedAllocation;          ///< ARL coefficient buffer is an alias of the global buffer and must not be free()'d ARL系数缓冲区是全局缓冲区的别名，不能为free()'d
#endif

    Pel* m_pcIPCMSample[MAX_NUM_COMPONENT];      ///< PCM sample buffer (0->Y, 1->Cb, 2->Cr)PCM样品缓冲(0->Y, 1->Cb, 2->Cr)

    // -------------------------------------------------------------------------------------------------------------------
    // neighbour access variables
    // -------------------------------------------------------------------------------------------------------------------

    TComDataCU* m_pCtuAboveLeft;                        ///< pointer of above-left CTU. 左上CTU的指针。
    TComDataCU* m_pCtuAboveRight;                       ///< pointer of above-right CTU. 右上方CTU的指针。
    TComDataCU* m_pCtuAbove;                            ///< pointer of above CTU. CTU上方的指针。
    TComDataCU* m_pCtuLeft;                             ///< pointer of left CTU CTU左指针
    TComMvField   m_cMvFieldA;                            ///< motion vector of position A 位置A的运动矢量
    TComMvField   m_cMvFieldB;                            ///< motion vector of position B 位置B的运动矢量
    TComMvField   m_cMvFieldC;                            ///< motion vector of position C 位置C的运动矢量
    TComMv        m_cMvPred;                              ///< motion vector predictor 运动矢量预测器

    // -------------------------------------------------------------------------------------------------------------------
    // coding tool information 编码工具信息
    // -------------------------------------------------------------------------------------------------------------------

    Bool* m_pbMergeFlag;                          ///< array of merge flags 合并标志数组
    UChar* m_puhMergeIndex;                        ///< array of merge candidate indices 合并候选索引数组
#if AMP_MRG
    Bool          m_bIsMergeAMP;
#endif
    UChar* m_puhIntraDir[MAX_NUM_CHANNEL_TYPE];
    UChar* m_puhInterDir;                          ///< array of inter directions 方向间数组
    SChar* m_apiMVPIdx[NUM_REF_PIC_LIST_01];       ///< array of motion vector predictor candidates 运动矢量预测器候选数组
    SChar* m_apiMVPNum[NUM_REF_PIC_LIST_01];       ///< array of number of possible motion vectors predictors 可能的运动向量预测数的数组
    Bool* m_pbIPCMFlag;                           ///< array of intra_pcm flags intra_pcm标志数组
#if MCTS_ENC_CHECK
    Bool          m_tMctsMvpIsValid;
#endif

    // SCM new added variables SCM新增变量
    UChar* m_piSPoint[MAX_NUM_COMPONENT];            ///< 0: left run mode; 1: above run mode 0:左运行模式;1:上运行模式
    Pel* m_piLevel[MAX_NUM_COMPONENT];           ///< in run mode, it is the sample index in palette; in PCM mode, it is the abs sample value; 在运行模式下，它是调色板中的示例索引;PCM模式下，为abs采样值;
    Bool* m_pbPaletteModeFlag;                          ///< array of intra_pcm flags intra_pcm标志数组
    Pel* m_piPalette[MAX_NUM_COMPONENT];               ///< Palette 调色板
    Bool* m_bPrevPaletteReusedFlag[MAX_NUM_COMPONENT];  ///< Palette 调色板
    UChar* m_puhPaletteEscape[MAX_NUM_COMPONENT];
    Pel* m_piLastPaletteInLcuFinal[MAX_NUM_COMPONENT]; ///< Palette 调色板
    UChar         m_lastPaletteSizeFinal[MAX_NUM_COMPONENT];
    UChar         m_lastPaletteUsedSizeFinal[MAX_NUM_COMPONENT];
    Bool* m_pbPaletteScanRotationModeFlag;
    UChar* m_piEscapeFlag[MAX_NUM_COMPONENT];
    Bool* m_colourTransform;
    TComMv        m_lastIntraBCMv[2];    ///< last 2 Intra Block Copy Mv used. 最近2个内块拷贝Mv使用。
    UInt          m_paletteMaxSize;         ///< maximum palette size 最大调色板大小
    UInt          m_paletteMaxPredSize;     ///< maximum palette predictor size 最大调色板预测器大小
    Double        m_tmpIntraBCRDCost;
    Double        m_tmpInterRDCost;
    Bool          m_bIntraBCCSCEnabled;
    Bool          m_bInterCSCEnabled;

    // -------------------------------------------------------------------------------------------------------------------
    // misc. variables 混杂。变量
    // -------------------------------------------------------------------------------------------------------------------

    Bool          m_bDecSubCu;                            ///< indicates decoder-mode 表明decoder-mode
    Double        m_dTotalCost;                           ///< sum of partition RD costs 分区RD成本的总和
    Distortion    m_uiTotalDistortion;                    ///< sum of partition distortion 分割失真和
    UInt          m_uiTotalBits;                          ///< sum of partition bits 分区位和
    UInt          m_uiTotalBins;                          ///< sum of partition bins 分区箱的和
    SChar         m_codedQP;
    UChar* m_explicitRdpcmMode[MAX_NUM_COMPONENT]; ///< Stores the explicit RDPCM mode for all TUs belonging to this CU 存储属于该CU的所有tu的显式RDPCM模式

protected:

    /// adds a single possible motion vector predictor candidate 添加单个可能的运动向量预测器候选
    Bool          xAddMVPCandUnscaled(AMVPInfo& info, const RefPicList eRefPicList, const Int iRefIdx, const UInt uiPartUnitIdx, const MVP_DIR eDir) const;
    Bool          xAddMVPCandWithScaling(AMVPInfo& info, const RefPicList eRefPicList, const Int iRefIdx, const UInt uiPartUnitIdx, const MVP_DIR eDir) const;

    Void          deriveRightBottomIdx(UInt uiPartIdx, UInt& ruiPartIdxRB) const;
    Bool          xGetColMVP(const RefPicList eRefPicList, const Int ctuRsAddr, const Int partUnitIdx, TComMv& rcMv, const Int refIdx) const;
    Bool          xGetColMVPIBC(Int ctuRsAddr, Int partUnitIdx, TComMv& rcMv);

    /// compute scaling factor from POC difference 根据POC差计算比例因子
    static Int    xGetDistScaleFactor(Int iCurrPOC, Int iCurrRefPOC, Int iColPOC, Int iColRefPOC);

    Void          xDeriveCenterIdx(UInt uiPartIdx, UInt& ruiPartIdxCenter) const;

public:
    TComDataCU();
    virtual       ~TComDataCU();

    // -------------------------------------------------------------------------------------------------------------------
    // create / destroy / initialize / copy 创建/销毁/初始化/复制
    // -------------------------------------------------------------------------------------------------------------------

    Void          create(ChromaFormat chromaFormatIDC, UInt uiNumPartition, UInt uiWidth, UInt uiHeight, Bool bDecSubCu, Int unitSize, UInt paletteMaxSize, UInt paletteMaxPredSize
#if ADAPTIVE_QP_SELECTION
        , TCoeff* pParentARLBuffer = 0
#endif
    );
    Void          destroy();

    Void          initCtu(TComPic* pcPic, UInt ctuRsAddr);
    Void          initEstData(const UInt uiDepth, const Int qp, const Bool bTransquantBypass);
    Void          initSubCU(TComDataCU* pcCU, UInt uiPartUnitIdx, UInt uiDepth, Int qp);
    Void          setOutsideCUPart(UInt uiAbsPartIdx, UInt uiDepth);

    Void          copySubCU(TComDataCU* pcCU, UInt uiPartUnitIdx);
    Void          copyInterPredInfoFrom(TComDataCU* pcCU, UInt uiAbsPartIdx, RefPicList eRefPicList);
    Void          copyPartFrom(TComDataCU* pcCU, UInt uiPartUnitIdx, UInt uiDepth);

    Void          copyToPic(UChar uiDepth);

    // -------------------------------------------------------------------------------------------------------------------
    // member functions for CU description CU描述的成员函数
    // -------------------------------------------------------------------------------------------------------------------

    TComPic* getPic() { return m_pcPic; }
    const TComPic* getPic() const { return m_pcPic; }
    TComSlice* getSlice() { return m_pcSlice; }
    const TComSlice* getSlice() const { return m_pcSlice; }
    UInt& getCtuRsAddr() { return m_ctuRsAddr; }
    UInt          getCtuRsAddr() const { return m_ctuRsAddr; }
    UInt          getZorderIdxInCtu() const { return m_absZIdxInCtu; }
    UInt          getCUPelX() const { return m_uiCUPelX; }
    UInt          getCUPelY() const { return m_uiCUPelY; }

    UChar* getDepth() { return m_puhDepth; }
    UChar         getDepth(UInt uiIdx) const { return m_puhDepth[uiIdx]; }
    Void          setDepth(UInt uiIdx, UChar uh) { m_puhDepth[uiIdx] = uh; }

    Void          setDepthSubParts(UInt uiDepth, UInt uiAbsPartIdx);

#if MCTS_ENC_CHECK
    Void          setTMctsMvpIsValid(Bool b) { m_tMctsMvpIsValid = b; }
    Bool          getTMctsMvpIsValid() { return m_tMctsMvpIsValid; }
    Bool          isLastColumnCTUInTile() const;
#endif

    // -------------------------------------------------------------------------------------------------------------------
    // member functions for CU data CU数据的成员函数
    // -------------------------------------------------------------------------------------------------------------------

    SChar* getPartitionSize() { return m_pePartSize; }
    PartSize      getPartitionSize(UInt uiIdx) const { return static_cast<PartSize>(m_pePartSize[uiIdx]); }
    Void          setPartitionSize(UInt uiIdx, PartSize uh) { m_pePartSize[uiIdx] = uh; }
    Void          setPartSizeSubParts(PartSize eMode, UInt uiAbsPartIdx, UInt uiDepth);
    Void          setCUTransquantBypassSubParts(Bool flag, UInt uiAbsPartIdx, UInt uiDepth);

    Bool* getSkipFlag() { return m_skipFlag; }
    Bool          getSkipFlag(UInt idx) const { return m_skipFlag[idx]; }
    Void          setSkipFlag(UInt idx, Bool skip) { m_skipFlag[idx] = skip; }
    Void          setSkipFlagSubParts(Bool skip, UInt absPartIdx, UInt depth);

    SChar* getPredictionMode() { return m_pePredMode; }
    PredMode      getPredictionMode(UInt uiIdx) const { return static_cast<PredMode>(m_pePredMode[uiIdx]); }
    Void          setPredictionMode(UInt uiIdx, PredMode uh) { m_pePredMode[uiIdx] = uh; }
    Void          setPredModeSubParts(PredMode eMode, UInt uiAbsPartIdx, UInt uiDepth);

    SChar* getCrossComponentPredictionAlpha(ComponentID compID) { return m_crossComponentPredictionAlpha[compID]; }
    SChar         getCrossComponentPredictionAlpha(UInt uiIdx, ComponentID compID) { return m_crossComponentPredictionAlpha[compID][uiIdx]; }

    Bool* getCUTransquantBypass() { return m_CUTransquantBypass; }
    Bool          getCUTransquantBypass(UInt uiIdx) const { return m_CUTransquantBypass[uiIdx]; }

    UChar* getWidth() { return m_puhWidth; }
    UChar         getWidth(UInt uiIdx) const { return m_puhWidth[uiIdx]; }
    Void          setWidth(UInt uiIdx, UChar  uh) { m_puhWidth[uiIdx] = uh; }

    UChar* getHeight() { return m_puhHeight; }
    UChar         getHeight(UInt uiIdx) const { return m_puhHeight[uiIdx]; }
    Void          setHeight(UInt uiIdx, UChar  uh) { m_puhHeight[uiIdx] = uh; }

    Void          setSizeSubParts(UInt uiWidth, UInt uiHeight, UInt uiAbsPartIdx, UInt uiDepth);

    SChar* getQP() { return m_phQP; }
    SChar         getQP(UInt uiIdx) const { return m_phQP[uiIdx]; }
    Void          setQP(UInt uiIdx, SChar value) { m_phQP[uiIdx] = value; }
    Void          setQPSubParts(Int qp, UInt uiAbsPartIdx, UInt uiDepth);
    Int           getLastValidPartIdx(Int iAbsPartIdx) const;
    SChar         getLastCodedQP(UInt uiAbsPartIdx) const;
    Void          setQPSubCUs(Int qp, UInt absPartIdx, UInt depth, Bool& foundNonZeroCbf);
    Void          setCodedQP(SChar qp) { m_codedQP = qp; }
    SChar         getCodedQP() const { return m_codedQP; }

    UChar* getChromaQpAdj() { return m_ChromaQpAdj; } ///< array of chroma QP adjustments (indexed). when value = 0, cu_chroma_qp_offset_flag=0; when value>0, indicates cu_chroma_qp_offset_flag=1 and cu_chroma_qp_offset_idx=value-1
    UChar         getChromaQpAdj(Int idx) const { return m_ChromaQpAdj[idx]; } ///< When value = 0, cu_chroma_qp_offset_flag=0; when value>0, indicates cu_chroma_qp_offset_flag=1 and cu_chroma_qp_offset_idx=value-1
    Void          setChromaQpAdj(Int idx, UChar val) { m_ChromaQpAdj[idx] = val; } ///< When val = 0,   cu_chroma_qp_offset_flag=0; when val>0,   indicates cu_chroma_qp_offset_flag=1 and cu_chroma_qp_offset_idx=val-1
    Void          setChromaQpAdjSubParts(UChar val, Int absPartIdx, Int depth);
    Void          setCodedChromaQpAdj(SChar qp) { m_codedChromaQpAdj = qp; }
    SChar         getCodedChromaQpAdj() const { return m_codedChromaQpAdj; }

    Bool          isLosslessCoded(UInt absPartIdx) const;

    UChar* getTransformIdx() { return m_puhTrIdx; }
    UChar         getTransformIdx(UInt uiIdx) const { return m_puhTrIdx[uiIdx]; }
    Void          setTrIdxSubParts(UInt uiTrIdx, UInt uiAbsPartIdx, UInt uiDepth);

    UChar* getTransformSkip(ComponentID compID) { return m_puhTransformSkip[compID]; }
    UChar         getTransformSkip(UInt uiIdx, ComponentID compID) const { return m_puhTransformSkip[compID][uiIdx]; }
    Void          setTransformSkipSubParts(UInt useTransformSkip, ComponentID compID, UInt uiAbsPartIdx, UInt uiDepth);
    Void          setTransformSkipSubParts(const UInt useTransformSkip[MAX_NUM_COMPONENT], UInt uiAbsPartIdx, UInt uiDepth);

    UChar* getExplicitRdpcmMode(ComponentID component) { return m_explicitRdpcmMode[component]; }
    UChar         getExplicitRdpcmMode(ComponentID component, UInt partIdx) const { return m_explicitRdpcmMode[component][partIdx]; }
    Void          setExplicitRdpcmModePartRange(UInt rdpcmMode, ComponentID compID, UInt uiAbsPartIdx, UInt uiCoveredPartIdxes);

    Bool          isRDPCMEnabled(UInt uiAbsPartIdx) const { return getSlice()->getSPS()->getSpsRangeExtension().getRdpcmEnabledFlag(isIntra(uiAbsPartIdx) ? RDPCM_SIGNAL_IMPLICIT : RDPCM_SIGNAL_EXPLICIT); }

    Void          setCrossComponentPredictionAlphaPartRange(SChar alphaValue, ComponentID compID, UInt uiAbsPartIdx, UInt uiCoveredPartIdxes);
    Void          setTransformSkipPartRange(UInt useTransformSkip, ComponentID compID, UInt uiAbsPartIdx, UInt uiCoveredPartIdxes);

    UInt          getQuadtreeTULog2MinSizeInCU(UInt uiIdx) const;

    TComCUMvField* getCUMvField(RefPicList e) { return &m_acCUMvField[e]; }
    const TComCUMvField* getCUMvField(RefPicList e) const { return &m_acCUMvField[e]; }

    TCoeff* getCoeff(ComponentID component) { return m_pcTrCoeff[component]; }

#if ADAPTIVE_QP_SELECTION
    TCoeff* getArlCoeff(ComponentID component) { return m_pcArlCoeff[component]; }
#endif
    Pel* getPCMSample(ComponentID component) { return m_pcIPCMSample[component]; }

    UChar         getCbf(UInt uiIdx, ComponentID eType) const { return m_puhCbf[eType][uiIdx]; }
    UChar* getCbf(ComponentID eType) { return m_puhCbf[eType]; }
    UChar         getCbf(UInt uiIdx, ComponentID eType, UInt uiTrDepth) const { return ((getCbf(uiIdx, eType) >> uiTrDepth) & 0x1); }
    Void          setCbf(UInt uiIdx, ComponentID eType, UChar uh) { m_puhCbf[eType][uiIdx] = uh; }
    Void          clearCbf(UInt uiIdx, ComponentID eType, UInt uiNumParts);
    UChar         getQtRootCbf(UInt uiIdx) const;

    Void          setCbfSubParts(const UInt uiCbf[MAX_NUM_COMPONENT], UInt uiAbsPartIdx, UInt uiDepth);
    Void          setCbfSubParts(UInt uiCbf, ComponentID compID, UInt uiAbsPartIdx, UInt uiDepth);
    Void          setCbfSubParts(UInt uiCbf, ComponentID compID, UInt uiAbsPartIdx, UInt uiPartIdx, UInt uiDepth);

    Void          setCbfPartRange(UInt uiCbf, ComponentID compID, UInt uiAbsPartIdx, UInt uiCoveredPartIdxes);
    Void          bitwiseOrCbfPartRange(UInt uiCbf, ComponentID compID, UInt uiAbsPartIdx, UInt uiCoveredPartIdxes);

    // -------------------------------------------------------------------------------------------------------------------
    // member functions for coding tool information /成员函数用于编码工具信息
    // -------------------------------------------------------------------------------------------------------------------

    Bool* getMergeFlag() { return m_pbMergeFlag; }
    Bool          getMergeFlag(UInt uiIdx) const { return m_pbMergeFlag[uiIdx]; }
    Void          setMergeFlag(UInt uiIdx, Bool b) { m_pbMergeFlag[uiIdx] = b; }
    Void          setMergeFlagSubParts(Bool bMergeFlag, UInt uiAbsPartIdx, UInt uiPartIdx, UInt uiDepth);

    UChar* getMergeIndex() { return m_puhMergeIndex; }
    UChar         getMergeIndex(UInt uiIdx) const { return m_puhMergeIndex[uiIdx]; }
    Void          setMergeIndex(UInt uiIdx, UInt uiMergeIndex) { m_puhMergeIndex[uiIdx] = uiMergeIndex; }
    Void          setMergeIndexSubParts(UInt uiMergeIndex, UInt uiAbsPartIdx, UInt uiPartIdx, UInt uiDepth);
    template <typename T>
    Void          setSubPart(T bParameter, T* pbBaseCtu, UInt uiCUAddr, UInt uiCUDepth, UInt uiPUIdx);

#if AMP_MRG
    Void          setMergeAMP(Bool b) { m_bIsMergeAMP = b; }
    Bool          getMergeAMP() const { return m_bIsMergeAMP; }
#endif

    UChar* getIntraDir(const ChannelType channelType)                   const { return m_puhIntraDir[channelType]; }
    UChar         getIntraDir(const ChannelType channelType, const UInt uiIdx) const { return m_puhIntraDir[channelType][uiIdx]; }

    Void          setIntraDirSubParts(const ChannelType channelType,
        const UInt uiDir,
        const UInt uiAbsPartIdx,
        const UInt uiDepth);

    UChar* getInterDir() { return m_puhInterDir; }
    UChar         getInterDir(UInt uiIdx) const { return m_puhInterDir[uiIdx]; }
    Void          setInterDir(UInt uiIdx, UChar  uh) { m_puhInterDir[uiIdx] = uh; }
    Void          setInterDirSubParts(UInt uiDir, UInt uiAbsPartIdx, UInt uiPartIdx, UInt uiDepth);
    Bool* getIPCMFlag() { return m_pbIPCMFlag; }
    Bool          getIPCMFlag(UInt uiIdx) const { return m_pbIPCMFlag[uiIdx]; }
    Void          setIPCMFlag(UInt uiIdx, Bool b) { m_pbIPCMFlag[uiIdx] = b; }
    Void          setIPCMFlagSubParts(Bool bIpcmFlag, UInt uiAbsPartIdx, UInt uiDepth);

    // -------------------------------------------------------------------------------------------------------------------
    // member functions for accessing partition information 访问分区信息的成员函数
    // -------------------------------------------------------------------------------------------------------------------

    Void          getPartIndexAndSize(UInt uiPartIdx, UInt& ruiPartAddr, Int& riWidth, Int& riHeight) const; // This is for use by a leaf/sub CU object only, with no additional AbsPartIdx 这只供叶子/子CU对象使用，没有额外的AbsPartIdx
    UChar         getNumPartitions(const UInt uiAbsPartIdx = 0) const;
    Bool          isFirstAbsZorderIdxInDepth(UInt uiAbsPartIdx, UInt uiDepth) const;

    // -------------------------------------------------------------------------------------------------------------------
    // member functions for motion vector 运动向量的成员函数
    // -------------------------------------------------------------------------------------------------------------------

    static Void   getMvField(const TComDataCU* pcCU, UInt uiAbsPartIdx, RefPicList eRefPicList, TComMvField& rcMvField);

    Void          fillMvpCand(const UInt uiPartIdx, const UInt uiPartAddr, const RefPicList eRefPicList, const Int iRefIdx, AMVPInfo* pInfo) const;
    Bool          isDiffMER(Int xN, Int yN, Int xP, Int yP) const;
    Void          getPartPosition(UInt partIdx, Int& xP, Int& yP, Int& nPSW, Int& nPSH) const;

    Void          setMVPIdx(RefPicList eRefPicList, UInt uiIdx, Int iMVPIdx) { m_apiMVPIdx[eRefPicList][uiIdx] = iMVPIdx; }
    Int           getMVPIdx(RefPicList eRefPicList, UInt uiIdx) const { return m_apiMVPIdx[eRefPicList][uiIdx]; }
    SChar* getMVPIdx(RefPicList eRefPicList) { return m_apiMVPIdx[eRefPicList]; }

    Void          setMVPNum(RefPicList eRefPicList, UInt uiIdx, Int iMVPNum) { m_apiMVPNum[eRefPicList][uiIdx] = iMVPNum; }
    Int           getMVPNum(RefPicList eRefPicList, UInt uiIdx) const { return m_apiMVPNum[eRefPicList][uiIdx]; }
    SChar* getMVPNum(RefPicList eRefPicList) { return m_apiMVPNum[eRefPicList]; }

    Void          setMVPIdxSubParts(Int iMVPIdx, RefPicList eRefPicList, UInt uiAbsPartIdx, UInt uiPartIdx, UInt uiDepth);
    Void          setMVPNumSubParts(Int iMVPNum, RefPicList eRefPicList, UInt uiAbsPartIdx, UInt uiPartIdx, UInt uiDepth);

    Void          clipMv(TComMv& rcMv) const;
    Void          getMvPredLeft(TComMv& rcMvPred) const { rcMvPred = m_cMvFieldA.getMv(); }
    Void          getMvPredAbove(TComMv& rcMvPred) const { rcMvPred = m_cMvFieldB.getMv(); }
    Void          getMvPredAboveRight(TComMv& rcMvPred) const { rcMvPred = m_cMvFieldC.getMv(); }

    Void          compressMV();

    // -------------------------------------------------------------------------------------------------------------------
    // utility functions for neighbouring information 用于邻近信息的实用函数
    // -------------------------------------------------------------------------------------------------------------------

    TComDataCU* getCtuLeft() { return m_pCtuLeft; }
    TComDataCU* getCtuAbove() { return m_pCtuAbove; }
    TComDataCU* getCtuAboveLeft() { return m_pCtuAboveLeft; }
    TComDataCU* getCtuAboveRight() { return m_pCtuAboveRight; }
    Bool          CUIsFromSameSlice(const TComDataCU* pCU /* Can be NULL */) const { return (pCU != NULL && pCU->getSlice()->getSliceCurStartCtuTsAddr() == getSlice()->getSliceCurStartCtuTsAddr()); }
    Bool          CUIsFromSameTile(const TComDataCU* pCU /* Can be NULL */) const;
    Bool          CUIsFromSameSliceAndTile(const TComDataCU* pCU /* Can be NULL */) const;
    Bool          CUIsFromSameSliceTileAndWavefrontRow(const TComDataCU* pCU /* Can be NULL */) const;
    Bool          isLastSubCUOfCtu(const UInt absPartIdx) const;


    const TComDataCU* getPULeft(UInt& uiLPartUnitIdx,
        UInt  uiCurrPartUnitIdx,
        Bool  bEnforceSliceRestriction = true,
        Bool  bEnforceTileRestriction = true) const;

    const TComDataCU* getPUAbove(UInt& uiAPartUnitIdx,
        UInt  uiCurrPartUnitIdx,
        Bool  bEnforceSliceRestriction = true,
        Bool  planarAtCTUBoundary = false,
        Bool  bEnforceTileRestriction = true) const;

    const TComDataCU* getPUAboveLeft(UInt& uiALPartUnitIdx, UInt uiCurrPartUnitIdx, Bool bEnforceSliceRestriction = true) const;

    const TComDataCU* getQpMinCuLeft(UInt& uiLPartUnitIdx, UInt uiCurrAbsIdxInCtu) const;
    const TComDataCU* getQpMinCuAbove(UInt& uiAPartUnitIdx, UInt uiCurrAbsIdxInCtu) const;

    /// returns CU and part index of the PU above the top row of the current uiCurrPartUnitIdx of the CU, at a horizontal offset (to the right) of uiPartUnitOffset (in parts)
    //返回CU和CU的当前uiCurrPartUnitIdx的顶部行的PU的部分索引，在uiPartUnitOffset的水平偏移(在部分中)
    const TComDataCU* getPUAboveRight(UInt& uiARPartUnitIdx, UInt uiCurrPartUnitIdx, UInt uiPartUnitOffset = 1, Bool bEnforceSliceRestriction = true) const;
    /// returns CU and part index of the PU left of the lefthand column of the current uiCurrPartUnitIdx of the CU, at a vertical offset (below) of uiPartUnitOffset (in parts)
    //返回CU和CU的当前uiCurrPartUnitIdx的左列左侧的PU的部分索引，在uiPartUnitOffset的垂直偏移(下面)(部分)
    const TComDataCU* getPUBelowLeft(UInt& uiBLPartUnitIdx, UInt uiCurrPartUnitIdx, UInt uiPartUnitOffset = 1, Bool bEnforceSliceRestriction = true) const;

    SChar         getRefQP(UInt uiCurrAbsIdxInCtu) const;

    Void          deriveLeftRightTopIdx(UInt uiPartIdx, UInt& ruiPartIdxLT, UInt& ruiPartIdxRT) const;
    Void          deriveLeftBottomIdx(UInt uiPartIdx, UInt& ruiPartIdxLB) const;

    Bool          hasEqualMotion(UInt uiAbsPartIdx, const TComDataCU* pcCandCU, UInt uiCandAbsPartIdx) const;
#if MCTS_ENC_CHECK
    Void          getInterMergeCandidates(UInt uiAbsPartIdx, UInt uiPUIdx, TComMvField* pcMFieldNeighbours, UChar* puhInterDirNeighbours, Int& numValidMergeCand, UInt& numSpatialMergeCandidates, Int mrgCandIdx = -1) const;
#else
    Void          getInterMergeCandidates(UInt uiAbsPartIdx, UInt uiPUIdx, TComMvField* pcMFieldNeighbours, UChar* puhInterDirNeighbours, Int& numValidMergeCand, Int mrgCandIdx = -1) const;
#endif

    Void          deriveLeftRightTopIdxGeneral(UInt uiAbsPartIdx, UInt uiPartIdx, UInt& ruiPartIdxLT, UInt& ruiPartIdxRT) const;
    Void          deriveLeftBottomIdxGeneral(UInt uiAbsPartIdx, UInt uiPartIdx, UInt& ruiPartIdxLB) const;

    // -------------------------------------------------------------------------------------------------------------------
    // member functions for modes 模态的成员函数
    // -------------------------------------------------------------------------------------------------------------------

    Bool          isIntra(UInt uiPartIdx) const { return m_pePredMode[uiPartIdx] == MODE_INTRA; }
    Bool          isInter(UInt uiPartIdx) const { return m_pePredMode[uiPartIdx] == MODE_INTER; }
    Bool          isSkipped(UInt uiPartIdx) const; ///< returns true, if the partiton is skipped 如果跳过分区，则返回true
    Bool          isBipredRestriction(UInt puIdx, Bool bCheckIBCRestricion = false) const;

    // -------------------------------------------------------------------------------------------------------------------
    // member functions for symbol prediction (most probable / mode conversion) 符号预测的成员函数(最可能/模式转换)
    // -------------------------------------------------------------------------------------------------------------------

    UInt          getIntraSizeIdx(UInt uiAbsPartIdx) const;

    Void          getAllowedChromaDir(UInt uiAbsPartIdx, UInt* uiModeList) const;
    Void          getIntraDirPredictor(UInt uiAbsPartIdx, Int uiIntraDirPred[NUM_MOST_PROBABLE_MODES], const ComponentID compID, Int* piMode = NULL) const;

    // -------------------------------------------------------------------------------------------------------------------
    // member functions for SBAC context SBAC上下文的成员函数
    // -------------------------------------------------------------------------------------------------------------------

    UInt          getCtxSplitFlag(UInt   uiAbsPartIdx, UInt uiDepth) const;
    UInt          getCtxQtCbf(TComTU& rTu, const ChannelType chType) const;

    UInt          getCtxSkipFlag(UInt   uiAbsPartIdx) const;
    UInt          getCtxInterDir(UInt   uiAbsPartIdx) const;

    UInt& getTotalBins() { return m_uiTotalBins; }
    // -------------------------------------------------------------------------------------------------------------------
    // member functions for RD cost storage 用于RD成本存储的成员函数
    // -------------------------------------------------------------------------------------------------------------------

    Double& getTotalCost() { return m_dTotalCost; }
    Distortion& getTotalDistortion() { return m_uiTotalDistortion; }
    UInt& getTotalBits() { return m_uiTotalBits; }
    UInt& getTotalNumPart() { return m_uiNumPartition; }

    UInt          getCoefScanIdx(const UInt uiAbsPartIdx, const UInt uiWidth, const UInt uiHeight, const ComponentID compID) const;

    // SCM new added functions SCM新增功能
    Void          initRQTData(const UInt depth, TComDataCU* pSrcCU, Bool bCopySrc, Bool bResetIntraMode, Bool bResetTUSplit);
    Void          setSlice(TComSlice* pcSlice) { m_pcSlice = pcSlice; }

    Void          setTmpIntraBCRDCost(Double d) { m_tmpIntraBCRDCost = d; }
    Double        getTmpIntraBCRDCost() { return m_tmpIntraBCRDCost; }
    Void          setTmpInterRDCost(Double d) { m_tmpInterRDCost = d; }
    Double        getTmpInterRDCost() { return m_tmpInterRDCost; }
    Void          setIntraBCCSCEnabled(Bool b) { m_bIntraBCCSCEnabled = b; }
    Bool          getIntraBCCSCEnabled() { return m_bIntraBCCSCEnabled; }
    Void          setInterCSCEnabled(Bool b) { m_bInterCSCEnabled = b; }
    Bool          getInterCSCEnabled() { return m_bInterCSCEnabled; }

    Void          getStartPosition(UInt partIdx, Int& xStartInCU, Int& yStartInCU);
    Bool          isIntraBC(UInt absPartIdx) const;
    TComMv        getLastIntraBCMv(Int idx = 0) { return m_lastIntraBCMv[idx]; }
    Void          setLastIntraBCMv(TComMv mv, Int idx = 0) { m_lastIntraBCMv[idx] = mv; }
    UInt          getIntraBCSearchAreaWidth(UInt uiMaxSearchWidthToLeftInCTUs);
    Bool          is8x8BipredRestriction(TComMv mvL0, TComMv mvL1, Int iRefIdxL0, Int iRefIdxL1) const;
    Void          xRestrictBipredMergeCand(UInt puIdx, TComMvField* mvFieldNeighbours, UChar* interDirNeighbours, Int numValidMergeCand);
    Void          roundMergeCandidates(TComMvField* pcMvFieldNeighbours, Int iCount) const;
    Bool          hasAssociatedACTFlag(UInt absPartIdx);
    UInt          getCtxEscapeFlag(UInt   absPartIdx, UInt idx, Pel* pEscapeFlag);

    Void          getIntraBCMVPsEncOnly(UInt absPartIdx, TComMv* MvPredEnc, Int& nbPred, Int iPartIdx);
    Bool          getDerivedBV(UInt absPartIdx, const TComMv& currentMv, TComMv& derivedMv, UInt width, UInt height);
    Bool* getColourTransform() { return m_colourTransform; }
    Bool          getColourTransform(UInt idx) { return m_colourTransform[idx]; }
    Bool          getColourTransform(UInt idx)              const { return m_colourTransform[idx]; }
    Void          setColourTransformSubParts(Bool colourTransform, UInt absPartIdx, UInt depth);
    UChar* getSPoint(ComponentID component) { return m_piSPoint[component]; }
    Pel* getLevel(ComponentID component) { return m_piLevel[component]; }
    TCoeff* getRun(ComponentID component) { return m_pcTrCoeff[component]; }
    Bool* getPaletteModeFlag() { return m_pbPaletteModeFlag; }
    Bool          getPaletteModeFlag(UInt idx) const { return m_pbPaletteModeFlag[idx]; }
    Void          setRLModeFlag(UInt idx, Bool b) { m_pbPaletteModeFlag[idx] = b; }
    Void          setPaletteModeFlagSubParts(Bool bRLModeFlag, UInt absPartIdx, UInt uiDepth);
    Pel* getPalette(UChar ucCh) { return m_piPalette[ucCh]; }
    Pel* getPalette(UChar ucCh, UInt idx) { return m_piPalette[ucCh] + (idx >> 2) * m_paletteMaxSize; }
    Pel           getPalette(UChar ucCh, UInt idx, UInt paletteIdx) { return m_piPalette[ucCh][(idx >> 2) * m_paletteMaxSize + paletteIdx]; }
    Void          setPalette(UChar ucCh, UInt idx, Pel value, UInt paletteIdx) { m_piPalette[ucCh][(idx >> 2) * m_paletteMaxSize + paletteIdx] = value; }
    Void          setPaletteSubParts(UChar ucCh, Pel value, UInt paletteIdx, UInt absPartIdx, UInt depth);

    UChar* getPaletteSize(UChar ucCh) { return m_puhTransformSkip[ucCh]; }
    UChar         getPaletteSize(UChar ucCh, UInt idx) { return m_puhTransformSkip[ucCh][idx]; }
    Void          setPaletteSize(UChar ucCh, UInt idx, Bool b) { m_puhTransformSkip[ucCh][idx] = b; }
    Void          setPaletteSizeSubParts(UChar ucCh, UChar ucRLDictSize, UInt absPartIdx, UInt depth);

    UChar* getPaletteEscape(UChar ucCh) { return m_puhPaletteEscape[ucCh]; }
    UChar         getPaletteEscape(UChar ucCh, UInt idx) { return m_puhPaletteEscape[ucCh][idx]; }
    Void          setPaletteEscape(UChar ucCh, UInt idx, Bool b) { m_puhPaletteEscape[ucCh][idx] = b; }
    Void          setPaletteEscapeSubParts(UChar ucCh, UChar ucUseEscape, UInt absPartIdx, UInt depth);

    Bool* getPrevPaletteReusedFlag(UChar ucCh) { return m_bPrevPaletteReusedFlag[ucCh]; }
    Bool* getPrevPaletteReusedFlag(UChar ucCh, UInt idx) { return m_bPrevPaletteReusedFlag[ucCh] + (idx >> 2) * m_paletteMaxPredSize; }
    Bool          getPrevPaletteReusedFlag(UChar ucCh, UInt idx, UInt paletteIdx) { return m_bPrevPaletteReusedFlag[ucCh][(idx >> 2) * m_paletteMaxPredSize + paletteIdx]; }
    Void          setPrevPaletteReusedFlag(UChar ucCh, UInt idx, Bool b, UInt paletteIdx) { m_bPrevPaletteReusedFlag[ucCh][(idx >> 2) * m_paletteMaxPredSize + paletteIdx] = b; }
    Void          setPrevPaletteReusedFlagSubParts(UChar ucCh, Bool b, UInt paletteIdx, UInt absPartIdx, UInt depth);

    Pel* getPalettePred(TComDataCU* pcCU, UInt ch, UInt& paletteSizePrev);
    UChar         getLastPaletteInLcuSizeFinal(UChar ucCh) { return m_lastPaletteSizeFinal[ucCh]; }
    Void          setLastPaletteInLcuSizeFinal(UChar ucCh, UChar uh) { m_lastPaletteSizeFinal[ucCh] = uh; }

    Pel* getLastPaletteInLcuFinal(UChar ucCh) { return m_piLastPaletteInLcuFinal[ucCh]; }
    Pel           getLastPaletteInLcuFinal(UChar ucCh, UInt paletteIdx) { return m_piLastPaletteInLcuFinal[ucCh][paletteIdx]; }
    Void          setLastPaletteInLcuFinal(UChar ucCh, Pel value, UInt paletteIdx) { m_piLastPaletteInLcuFinal[ucCh][paletteIdx] = value; }
    Void          saveLastPaletteInLcuFinal(TComDataCU* pcSrc, UInt absPartIdx, UInt numValidComp);
    Int           xCalcMaxVals(TComDataCU* pcCU, ComponentID compID);
    Bool          getPaletteScanRotationModeFlag(UInt idx) { return m_pbPaletteScanRotationModeFlag[idx]; }
    Bool* getPaletteScanRotationModeFlag() { return m_pbPaletteScanRotationModeFlag; }
    Void          setPaletteScanRotationModeFlagSubParts(Bool bPaletteScanRotationModeFlag, UInt absPartIdx, UInt depth);
    UChar* getEscapeFlag(ComponentID component) { return m_piEscapeFlag[component]; }
#if REDUCED_ENCODER_MEMORY
    Void          storeMVForIBCEnc();
#endif
};

namespace RasterAddress
{
    /** Check whether 2 addresses point to the same column 检查2个地址是否指向同一列
     * \param addrA          First address in raster scan order 按光栅扫描顺序显示的第一个地址
     * \param addrB          Second address in raters scan order Second address in raters scan order
     * \param numUnitsPerRow Number of units in a row 一行中的单元数
     * \return Result of test
     */
    static inline Bool isEqualCol(Int addrA, Int addrB, Int numUnitsPerRow)
    {
        // addrA % numUnitsPerRow == addrB % numUnitsPerRow
        return ((addrA ^ addrB) & (numUnitsPerRow - 1)) == 0;
    }

    /** Check whether 2 addresses point to the same row 检查2个地址是否指向同一行
     * \param addrA          First address in raster scan order 按光栅扫描顺序显示的第一个地址
     * \param addrB          Second address in raters scan order 评分者扫描顺序中的第二个地址
     * \param numUnitsPerRow Number of units in a row 一行中的单元数
     * \return Result of test
     */
    static inline Bool isEqualRow(Int addrA, Int addrB, Int numUnitsPerRow)
    {
        // addrA / numUnitsPerRow == addrB / numUnitsPerRow
        return ((addrA ^ addrB) & ~(numUnitsPerRow - 1)) == 0;
    }

    /** Check whether 2 addresses point to the same row or column 检查2个地址是否指向同一行或同一列
     * \param addrA          First address in raster scan order 按光栅扫描顺序显示的第一个地址
     * \param addrB          Second address in raters scan order 评分者扫描顺序中的第二个地址
     * \param numUnitsPerRow Number of units in a row 一行中的单元数
     * \return Result of test
     */
    static inline Bool isEqualRowOrCol(Int addrA, Int addrB, Int numUnitsPerRow)
    {
        return isEqualCol(addrA, addrB, numUnitsPerRow) | isEqualRow(addrA, addrB, numUnitsPerRow);
    }

    /** Check whether one address points to the first column 检查是否有一个地址指向第一列
     * \param addr           Address in raster scan order 地址在光栅扫描的顺序
     * \param numUnitsPerRow Number of units in a row 一行中的单位数目
     * \return Result of test 测试结果
     */
    static inline Bool isZeroCol(Int addr, Int numUnitsPerRow)
    {
        // addr % numUnitsPerRow == 0
        return (addr & (numUnitsPerRow - 1)) == 0;
    }

    /** Check whether one address points to the first row 检查是否有一个地址指向第一行
     * \param addr           Address in raster scan order
     * \param numUnitsPerRow Number of units in a row
     * \return Result of test
     */
    static inline Bool isZeroRow(Int addr, Int numUnitsPerRow)
    {
        // addr / numUnitsPerRow == 0
        return (addr & ~(numUnitsPerRow - 1)) == 0;
    }

    /** Check whether one address points to a column whose index is smaller than a given value 检查是否有一个地址指向索引小于给定值的列
     * \param addr           Address in raster scan order 地址在光栅扫描的顺序
     * \param val            Given column index value 给定列索引值
     * \param numUnitsPerRow Number of units in a row
     * \return Result of test
     */
    static inline Bool lessThanCol(Int addr, Int val, Int numUnitsPerRow)
    {
        // addr % numUnitsPerRow < val
        return (addr & (numUnitsPerRow - 1)) < val;
    }

    /** Check whether one address points to a row whose index is smaller than a given value 检查是否有一个地址指向索引小于给定值的行
     * \param addr           Address in raster scan order 地址在光栅扫描的顺序
     * \param val            Given row index value
     * \param numUnitsPerRow Number of units in a row
     * \return Result of test
     */
    static inline Bool lessThanRow(Int addr, Int val, Int numUnitsPerRow)
    {
        // addr / numUnitsPerRow < val
        return addr < val * numUnitsPerRow;
    }
}

//! \}

#endif
