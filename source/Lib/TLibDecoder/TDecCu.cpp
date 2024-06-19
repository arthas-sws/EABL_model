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

/** \file     TDecCu.cpp
    \brief    CU decoder class
*/

#include "TDecCu.h"
#include "TLibCommon/TComTU.h"
#include "TLibCommon/TComPrediction.h"

//! \ingroup TLibDecoder
//! \{

// ====================================================================================================================
// Constructor / destructor / create / destroy
// ====================================================================================================================

TDecCu::TDecCu()
{
  m_ppcYuvResi = NULL;
  m_ppcYuvReco = NULL;
  m_ppcCU      = NULL;
}

TDecCu::~TDecCu()
{
}

#if MCTS_ENC_CHECK
Void TDecCu::init( TDecEntropy* pcEntropyDecoder, TComTrQuant* pcTrQuant, TComPrediction* pcPrediction, TDecConformanceCheck* pConformanceCheck)
#else
Void TDecCu::init(TDecEntropy* pcEntropyDecoder, TComTrQuant* pcTrQuant, TComPrediction* pcPrediction)
#endif
{
  m_pcEntropyDecoder  = pcEntropyDecoder;
  m_pcTrQuant         = pcTrQuant;
  m_pcPrediction      = pcPrediction;
#if MCTS_ENC_CHECK
  m_pConformanceCheck = pConformanceCheck;
#endif
}

/**
 \param    uiMaxDepth      total number of allowable depth 允许深度总数
 \param    uiMaxWidth      largest CU width 最大CU宽度
 \param    uiMaxHeight     largest CU height 最大CU高度
 \param    chromaFormatIDC chroma format 色度格式
 */
Void TDecCu::create( UInt uiMaxDepth, UInt uiMaxWidth, UInt uiMaxHeight, ChromaFormat chromaFormatIDC, UInt paletteMaxSize, UInt paletteMaxPredSize )
{
  m_uiMaxDepth = uiMaxDepth+1;

  m_ppcYuvResi = new TComYuv*[m_uiMaxDepth-1];
  m_ppcYuvReco = new TComYuv*[m_uiMaxDepth-1];
  m_ppcCU      = new TComDataCU*[m_uiMaxDepth-1];

  for ( UInt ui = 0; ui < m_uiMaxDepth-1; ui++ )
  {
    UInt uiNumPartitions = 1<<( ( m_uiMaxDepth - ui - 1 )<<1 );
    UInt uiWidth  = uiMaxWidth  >> ui;
    UInt uiHeight = uiMaxHeight >> ui;

    // The following arrays (m_ppcYuvResi, m_ppcYuvReco and m_ppcCU) are only required for CU depths
    // although data is allocated for all possible depths of the CU/TU tree except the last.以下数组(m_ppcYuvResi, m_ppcYuvReco和m_ppcCU)只需要用于CU深度，
    // Since the TU tree will always include at least one additional depth greater than the CU tree,尽管数据被分配给CU/TU树的所有可能深度，除了最后一个。
    // there will be enough entries for these arrays. 由于TU树总是包含至少一个比CU树更大的深度，因此这些数组将有足够的条目。
    // (Section 7.4.3.2: "The CVS shall not contain data that result in (Log2MinTrafoSize) MinTbLog2SizeY
    //                    greater than or equal to MinCbLog2SizeY")
    // TODO: tidy the array allocation given the above comment.

    m_ppcYuvResi[ui] = new TComYuv;    m_ppcYuvResi[ui]->create( uiWidth, uiHeight, chromaFormatIDC );
    m_ppcYuvReco[ui] = new TComYuv;    m_ppcYuvReco[ui]->create( uiWidth, uiHeight, chromaFormatIDC );
    m_ppcCU     [ui] = new TComDataCU; m_ppcCU     [ui]->create( chromaFormatIDC, uiNumPartitions, uiWidth, uiHeight, true, uiMaxWidth >> (m_uiMaxDepth - 1)
                                                               , paletteMaxSize, paletteMaxPredSize );
  }

  m_bDecodeDQP = false;
  m_IsChromaQpAdjCoded = false;
  // initialize partition order.
  UInt* piTmp = &g_auiZscanToRaster[0];
  initZscanToRaster(m_uiMaxDepth, 1, 0, piTmp);
  initRasterToZscan( uiMaxWidth, uiMaxHeight, m_uiMaxDepth );

  // initialize conversion matrix from partition index to pel
  initRasterToPelXY( uiMaxWidth, uiMaxHeight, m_uiMaxDepth );
}

Void TDecCu::destroy()
{
  for ( UInt ui = 0; ui < m_uiMaxDepth-1; ui++ )
  {
    m_ppcYuvResi[ui]->destroy(); delete m_ppcYuvResi[ui]; m_ppcYuvResi[ui] = NULL;
    m_ppcYuvReco[ui]->destroy(); delete m_ppcYuvReco[ui]; m_ppcYuvReco[ui] = NULL;
    m_ppcCU     [ui]->destroy(); delete m_ppcCU     [ui]; m_ppcCU     [ui] = NULL;
  }

  delete [] m_ppcYuvResi; m_ppcYuvResi = NULL;
  delete [] m_ppcYuvReco; m_ppcYuvReco = NULL;
  delete [] m_ppcCU     ; m_ppcCU      = NULL;
}

// ====================================================================================================================
// Public member functions
// ====================================================================================================================

/** 
 Parse a CTU. 解析一个CTU。
 \param    pCtu                      [in/out] pointer to CTU data structure 指向CTU数据结构的指针
 \param    isLastCtuOfSliceSegment   [out]    true, if last CTU of the slice segment true，如果切片段的最后一个CTU
 */
Void TDecCu::decodeCtu( TComDataCU* pCtu, Bool& isLastCtuOfSliceSegment )
{
  if ( pCtu->getSlice()->getPPS()->getUseDQP() )
  {
    setdQPFlag(true);
  }

  if ( pCtu->getSlice()->getUseChromaQpAdj() )
  {
    setIsChromaQpAdjCoded(true);
  }

  // start from the top level CU 从顶层CU开始
  xDecodeCU( pCtu, 0, 0, isLastCtuOfSliceSegment);
}

/** 
 Decoding process for a CTU. CTU的解码过程。
 \param    pCtu                      [in/out] pointer to CTU data structure 指向CTU数据结构的指针
 */
Void TDecCu::decompressCtu( TComDataCU* pCtu )
{
  xDecompressCU( pCtu, 0,  0 );
}

// ====================================================================================================================
// Protected member functions 受保护成员函数
// ====================================================================================================================

//! decode end-of-slice flag 解码片末标志 确保当前slice的解码结果得到正确的重构并能够为下一帧的解码提供正确的参考图像
Bool TDecCu::xDecodeSliceEnd( TComDataCU* pcCU, UInt uiAbsPartIdx )
{
  UInt uiIsLastCtuOfSliceSegment;

  if (pcCU->isLastSubCUOfCtu(uiAbsPartIdx))
  {
    m_pcEntropyDecoder->decodeTerminatingBit( uiIsLastCtuOfSliceSegment );
  }
  else
  {
    uiIsLastCtuOfSliceSegment=0;
  }

  return uiIsLastCtuOfSliceSegment>0;
}
//int AAAAA[7] = { 0 };
extern int AAAAA[7] = { 0 };
//! decode CU block recursively 递归解码CU块
Void TDecCu::xDecodeCU( TComDataCU*const pcCU, const UInt uiAbsPartIdx, const UInt uiDepth, Bool &isLastCtuOfSliceSegment)
{
  TComPic* pcPic        = pcCU->getPic();
  const TComSPS &sps    = pcPic->getPicSym()->getSPS();
  const TComPPS &pps    = pcPic->getPicSym()->getPPS();
  const UInt maxCuWidth = sps.getMaxCUWidth();
  const UInt maxCuHeight= sps.getMaxCUHeight();
  UInt uiCurNumParts    = pcPic->getNumPartitionsInCtu() >> (uiDepth<<1);
  UInt uiQNumParts      = uiCurNumParts>>2;


  Bool bBoundary = false;
  UInt uiLPelX   = pcCU->getCUPelX() + g_auiRasterToPelX[ g_auiZscanToRaster[uiAbsPartIdx] ];
  UInt uiRPelX   = uiLPelX + (maxCuWidth>>uiDepth)  - 1;
  UInt uiTPelY   = pcCU->getCUPelY() + g_auiRasterToPelY[ g_auiZscanToRaster[uiAbsPartIdx] ];
  UInt uiBPelY   = uiTPelY + (maxCuHeight>>uiDepth) - 1;
  
  
 // cout << uiDepth << endl;
      if (uiDepth >= 0 && uiDepth <= 6) AAAAA[uiDepth]++;
  //cout << "0出现了 " << AAAAA[0] << " 次" << endl;
  //cout << "1出现了 " << AAAAA[1] << " 次" << endl;
  //cout << "2出现了 " << AAAAA[2] << " 次" << endl;
  //cout << "3出现了 " << AAAAA[3] << " 次" << endl;
  //cout << "4出现了 " << AAAAA[4] << " 次" << endl;
  //cout << "5出现了 " << AAAAA[5] << " 次" << endl;
  //cout << "6出现了 " << AAAAA[6] << " 次" << endl;

  if( ( uiRPelX < sps.getPicWidthInLumaSamples() ) && ( uiBPelY < sps.getPicHeightInLumaSamples() ) )//判断是否超出范围
  {
    m_pcEntropyDecoder->decodeSplitFlag( pcCU, uiAbsPartIdx, uiDepth );
  }
  else
  {
    bBoundary = true;
  }
  if( ( ( uiDepth < pcCU->getDepth( uiAbsPartIdx ) ) && ( uiDepth < sps.getLog2DiffMaxMinCodingBlockSize() ) ) || bBoundary )
  {
    UInt uiIdx = uiAbsPartIdx;
    if( uiDepth == pps.getMaxCuDQPDepth() && pps.getUseDQP())
    {
      setdQPFlag(true);
      pcCU->setQPSubParts( pcCU->getRefQP(uiAbsPartIdx), uiAbsPartIdx, uiDepth ); // set QP to default QP 设置QP为默认QP
    }

    if( uiDepth == pps.getPpsRangeExtension().getDiffCuChromaQpOffsetDepth() && pcCU->getSlice()->getUseChromaQpAdj() )
    {
      setIsChromaQpAdjCoded(true);
    }

    for ( UInt uiPartUnitIdx = 0; uiPartUnitIdx < 4; uiPartUnitIdx++ )
    {
      uiLPelX   = pcCU->getCUPelX() + g_auiRasterToPelX[ g_auiZscanToRaster[uiIdx] ];
      uiTPelY   = pcCU->getCUPelY() + g_auiRasterToPelY[ g_auiZscanToRaster[uiIdx] ];

      if ( !isLastCtuOfSliceSegment && ( uiLPelX < sps.getPicWidthInLumaSamples() ) && ( uiTPelY < sps.getPicHeightInLumaSamples() ) )
      {
        xDecodeCU( pcCU, uiIdx, uiDepth+1, isLastCtuOfSliceSegment );
      }
      else
      {
        pcCU->setOutsideCUPart( uiIdx, uiDepth+1 );
      }

      uiIdx += uiQNumParts;
    }
    if( uiDepth == pps.getMaxCuDQPDepth() && pps.getUseDQP())
    {
      if ( getdQPFlag() )
      {
        UInt uiQPSrcPartIdx = uiAbsPartIdx;
        pcCU->setQPSubParts( pcCU->getRefQP( uiQPSrcPartIdx ), uiAbsPartIdx, uiDepth ); // set QP to default QP 设置QP为默认QP
      }
    }
    return;
  }

  if( uiDepth <= pps.getMaxCuDQPDepth() && pps.getUseDQP())
  {
    setdQPFlag(true);
    pcCU->setQPSubParts( pcCU->getRefQP(uiAbsPartIdx), uiAbsPartIdx, uiDepth ); // set QP to default QP 设置QP为默认QP
  }

  if( uiDepth <= pps.getPpsRangeExtension().getDiffCuChromaQpOffsetDepth() && pcCU->getSlice()->getUseChromaQpAdj() )
  {
    setIsChromaQpAdjCoded(true);
  }

  if (pps.getTransquantBypassEnabledFlag())
  {
    m_pcEntropyDecoder->decodeCUTransquantBypassFlag( pcCU, uiAbsPartIdx, uiDepth );
  }

  // decode CU mode and the partition size 解码CU模式和分区大小
  if( !pcCU->getSlice()->isIntra())
  {
    m_pcEntropyDecoder->decodeSkipFlag( pcCU, uiAbsPartIdx, uiDepth );
  }


  if( pcCU->isSkipped(uiAbsPartIdx) )
  {
    m_ppcCU[uiDepth]->copyInterPredInfoFrom( pcCU, uiAbsPartIdx, REF_PIC_LIST_0 );
    m_ppcCU[uiDepth]->copyInterPredInfoFrom( pcCU, uiAbsPartIdx, REF_PIC_LIST_1 );
    TComMvField cMvFieldNeighbours[MRG_MAX_NUM_CANDS << 1]; // double length for mv of both lists 两个链表中mv的长度加倍
    UChar uhInterDirNeighbours[MRG_MAX_NUM_CANDS];
    Int numValidMergeCand = 0;
    for( UInt ui = 0; ui < m_ppcCU[uiDepth]->getSlice()->getMaxNumMergeCand(); ++ui )
    {
      uhInterDirNeighbours[ui] = 0;
    }
    m_pcEntropyDecoder->decodeMergeIndex( pcCU, 0, uiAbsPartIdx, uiDepth );
    UInt uiMergeIndex = pcCU->getMergeIndex(uiAbsPartIdx);
#if MCTS_ENC_CHECK
    UInt numSpatialMergeCandidates = 0;
    m_ppcCU[uiDepth]->getInterMergeCandidates( 0, 0, cMvFieldNeighbours, uhInterDirNeighbours, numValidMergeCand, numSpatialMergeCandidates, uiMergeIndex );
    if ( m_pConformanceCheck->getTMctsCheck() && m_ppcCU[uiDepth]->isLastColumnCTUInTile() && (uiMergeIndex >= numSpatialMergeCandidates) )
    {
      m_pConformanceCheck->flagTMctsError("Merge Index using non-spatial merge candidate (Skip)");
    }
#else
    m_ppcCU[uiDepth]->getInterMergeCandidates( 0, 0, cMvFieldNeighbours, uhInterDirNeighbours, numValidMergeCand, uiMergeIndex );
#endif
    m_ppcCU[uiDepth]->roundMergeCandidates(cMvFieldNeighbours, numValidMergeCand);
    m_ppcCU[uiDepth]->xRestrictBipredMergeCand(0,cMvFieldNeighbours, uhInterDirNeighbours, numValidMergeCand);
    pcCU->setInterDirSubParts( uhInterDirNeighbours[uiMergeIndex], uiAbsPartIdx, 0, uiDepth );

    TComMv cTmpMv( 0, 0 );
    for ( UInt uiRefListIdx = 0; uiRefListIdx < 2; uiRefListIdx++ )
    {
      if ( pcCU->getSlice()->getNumRefIdx( RefPicList( uiRefListIdx ) ) > 0 )
      {
        pcCU->setMVPIdxSubParts( 0, RefPicList( uiRefListIdx ), uiAbsPartIdx, 0, uiDepth);
        pcCU->setMVPNumSubParts( 0, RefPicList( uiRefListIdx ), uiAbsPartIdx, 0, uiDepth);
        pcCU->getCUMvField( RefPicList( uiRefListIdx ) )->setAllMvd( cTmpMv, SIZE_2Nx2N, uiAbsPartIdx, uiDepth );
        pcCU->getCUMvField( RefPicList( uiRefListIdx ) )->setAllMvField( cMvFieldNeighbours[ 2*uiMergeIndex + uiRefListIdx ], SIZE_2Nx2N, uiAbsPartIdx, uiDepth );
      }
    }
    xFinishDecodeCU( pcCU, uiAbsPartIdx, uiDepth, isLastCtuOfSliceSegment );
    return;
  }

  m_pcEntropyDecoder->decodePredMode( pcCU, uiAbsPartIdx, uiDepth );

  Bool bCodeDQP = getdQPFlag();
  Bool isChromaQpAdjCoded = getIsChromaQpAdjCoded();

  m_pcEntropyDecoder->decodePaletteModeInfo( pcCU, uiAbsPartIdx, uiDepth, bCodeDQP, isChromaQpAdjCoded );

  if (pcCU->getPaletteModeFlag(uiAbsPartIdx) )
  {
    setIsChromaQpAdjCoded( isChromaQpAdjCoded );
    setdQPFlag(bCodeDQP);
    xFinishDecodeCU( pcCU, uiAbsPartIdx, uiDepth, isLastCtuOfSliceSegment );
    return;
  }

  m_pcEntropyDecoder->decodePartSize( pcCU, uiAbsPartIdx, uiDepth );

  if (pcCU->isIntra( uiAbsPartIdx ) && pcCU->getPartitionSize( uiAbsPartIdx ) == SIZE_2Nx2N )
  {
    m_pcEntropyDecoder->decodeIPCMInfo( pcCU, uiAbsPartIdx, uiDepth );

    if(pcCU->getIPCMFlag(uiAbsPartIdx))
    {
      xFinishDecodeCU( pcCU, uiAbsPartIdx, uiDepth, isLastCtuOfSliceSegment );
      return;
    }
  }

  // prediction mode ( Intra : direction mode, Inter : Mv, reference idx ) 预测模式(Intra:方向模式，Inter: Mv，参考idx)
  m_pcEntropyDecoder->decodePredInfo( pcCU, uiAbsPartIdx, uiDepth, m_ppcCU[uiDepth]);

  // Coefficient decoding 系数解码
  m_pcEntropyDecoder->decodeCoeff( pcCU, uiAbsPartIdx, uiDepth, bCodeDQP, isChromaQpAdjCoded );
  setIsChromaQpAdjCoded( isChromaQpAdjCoded );
  setdQPFlag( bCodeDQP );
  xFinishDecodeCU( pcCU, uiAbsPartIdx, uiDepth, isLastCtuOfSliceSegment );
}
//执行了一系列操作，包括填充残差数据、更新参考图像、进行深度优先遍历等，以便 CU 可以被正确地重构和预测
Void TDecCu::xFinishDecodeCU( TComDataCU* pcCU, UInt uiAbsPartIdx, UInt uiDepth, Bool &isLastCtuOfSliceSegment)
{
  if(  pcCU->getSlice()->getPPS()->getUseDQP())
  {
    pcCU->setQPSubParts( getdQPFlag()?pcCU->getRefQP(uiAbsPartIdx):pcCU->getCodedQP(), uiAbsPartIdx, uiDepth ); // set QP 设置QP
  }

  if (pcCU->getSlice()->getUseChromaQpAdj() && !getIsChromaQpAdjCoded())
  {
    pcCU->setChromaQpAdjSubParts( pcCU->getCodedChromaQpAdj(), uiAbsPartIdx, uiDepth ); 
  }
  isLastCtuOfSliceSegment = xDecodeSliceEnd( pcCU, uiAbsPartIdx );
}
//从比特流中读取编码信息并恢复出CU内部的像素值
Void TDecCu::xDecompressCU( TComDataCU* pCtu, UInt uiAbsPartIdx,  UInt uiDepth )
{
  TComPic* pcPic = pCtu->getPic();
  TComSlice * pcSlice = pCtu->getSlice();
  const TComSPS &sps=*(pcSlice->getSPS());
  Bool bBoundary = false;
  UInt uiLPelX   = pCtu->getCUPelX() + g_auiRasterToPelX[ g_auiZscanToRaster[uiAbsPartIdx] ];
  UInt uiRPelX   = uiLPelX + (sps.getMaxCUWidth()>>uiDepth)  - 1;
  UInt uiTPelY   = pCtu->getCUPelY() + g_auiRasterToPelY[ g_auiZscanToRaster[uiAbsPartIdx] ];
  UInt uiBPelY   = uiTPelY + (sps.getMaxCUHeight()>>uiDepth) - 1;

  if( ( uiRPelX >= sps.getPicWidthInLumaSamples() ) || ( uiBPelY >= sps.getPicHeightInLumaSamples() ) )
  {
    bBoundary = true;
  }

  if( ( ( uiDepth < pCtu->getDepth( uiAbsPartIdx ) ) && ( uiDepth < sps.getLog2DiffMaxMinCodingBlockSize() ) ) || bBoundary )
  {
    UInt uiNextDepth = uiDepth + 1;
    UInt uiQNumParts = pCtu->getTotalNumPart() >> (uiNextDepth<<1);
    UInt uiIdx = uiAbsPartIdx;
    for ( UInt uiPartIdx = 0; uiPartIdx < 4; uiPartIdx++ )
    {
      uiLPelX = pCtu->getCUPelX() + g_auiRasterToPelX[ g_auiZscanToRaster[uiIdx] ];
      uiTPelY = pCtu->getCUPelY() + g_auiRasterToPelY[ g_auiZscanToRaster[uiIdx] ];

      if( ( uiLPelX < sps.getPicWidthInLumaSamples() ) && ( uiTPelY < sps.getPicHeightInLumaSamples() ) )
      {
        xDecompressCU(pCtu, uiIdx, uiNextDepth );
      }

      uiIdx += uiQNumParts;
    }
    return;
  }

  // Residual reconstruction 剩余的重建
  m_ppcYuvResi[uiDepth]->clear();

  m_ppcCU[uiDepth]->copySubCU( pCtu, uiAbsPartIdx );

  switch( m_ppcCU[uiDepth]->getPredictionMode(0) )
  {
    case MODE_INTER:
      xReconInter( m_ppcCU[uiDepth], uiDepth );
      break;
    case MODE_INTRA:
      xReconIntraQT( m_ppcCU[uiDepth], uiDepth );
      break;
    default:
      assert(0);
      break;
  }

#if DEBUG_STRING
  const PredMode predMode=m_ppcCU[uiDepth]->getPredictionMode(0);
  if (DebugOptionList::DebugString_Structure.getInt()&DebugStringGetPredModeMask(predMode))
  {
    PartSize eSize=m_ppcCU[uiDepth]->getPartitionSize(0);
    std::ostream &ss(std::cout);

    ss <<"###: " << (predMode==MODE_INTRA?"Intra   ":"Inter   ") << partSizeToString[eSize] << " CU at " << m_ppcCU[uiDepth]->getCUPelX() << ", " << m_ppcCU[uiDepth]->getCUPelY() << " width=" << UInt(m_ppcCU[uiDepth]->getWidth(0)) << std::endl;
  }
#endif

  if ( m_ppcCU[uiDepth]->isLosslessCoded(0) && (m_ppcCU[uiDepth]->getIPCMFlag(0) == false))
  {
    xFillPCMBuffer(m_ppcCU[uiDepth], uiDepth);
  }

  xCopyToPic( m_ppcCU[uiDepth], pcPic, uiAbsPartIdx, uiDepth );
}

Void TDecCu::xReconInter( TComDataCU* pcCU, UInt uiDepth )
{
  Int numParts = pcCU->getNumPartitions();
  for (Int iPartIdx = 0; iPartIdx < numParts; iPartIdx++)
  {
    UInt partAddr;
    Int  iWidth, iHeight;
    pcCU->getPartIndexAndSize( iPartIdx, partAddr, iWidth, iHeight );
    for (Int refList = 0; refList < 2; refList++)
    {
      RefPicList eRefPicList = RefPicList(refList);
      Int        iRefIdx     = pcCU->getCUMvField( eRefPicList )->getRefIdx( partAddr );
      if (iRefIdx != -1 && pcCU->getSlice()->getRefPic( eRefPicList, iRefIdx )->getPOC() == pcCU->getSlice()->getPOC() )
      {
        TComMv cMv    = pcCU->getCUMvField( eRefPicList )->getMv( partAddr );
        Int    mvx    = cMv.getHor()>>2, mvy = cMv.getVer()>>2; // Get integer position 获取整数位置
        UInt   raster = g_auiZscanToRaster[partAddr];
        TComPicSym *pcSym = pcCU->getPic()->getPicSym();
        Int ctuX = (pcCU->getCUPelX() + g_auiRasterToPelX[raster] + mvx) / pcCU->getSlice()->getSPS()->getMaxCUWidth();
        Int ctuY = (pcCU->getCUPelY() + g_auiRasterToPelY[raster] + mvy) / pcCU->getSlice()->getSPS()->getMaxCUHeight();
        UInt ctuAddr = ctuX + ctuY*pcSym->getFrameWidthInCtus();
        if (pcSym->getCtu(ctuAddr)->getSlice()->getSliceIdx() != pcCU->getSlice()->getSliceIdx())
        {
          printf("BV (%i,%i) for PU%i(%u)/%i in CU %u,%u, slice %u points to CTU %u, slice %u\n",
                 mvx, mvy, iPartIdx, partAddr, numParts, pcCU->getCtuRsAddr(), pcCU->getZorderIdxInCtu(),
                 pcCU->getSlice()->getSliceIdx(), ctuAddr, pcSym->getCtu(ctuAddr)->getSlice()->getSliceIdx());
        }
      }
    }
  }

  // inter prediction 帧间预测
#if MCTS_ENC_CHECK
  if (m_pConformanceCheck->getTMctsCheck()  && !m_pcPrediction->checkTMctsMvp(pcCU))
  {
    m_pConformanceCheck->flagTMctsError("motion vector across tile boundaries");
  }
#endif
  m_pcPrediction->motionCompensation( pcCU, m_ppcYuvReco[uiDepth] );

#if DEBUG_STRING
  const Int debugPredModeMask=DebugStringGetPredModeMask(MODE_INTER);
  if (DebugOptionList::DebugString_Pred.getInt()&debugPredModeMask)
  {
    printBlockToStream(std::cout, "###inter-pred: ", *(m_ppcYuvReco[uiDepth]));
  }
#endif

  // inter recon 内部侦察
  xDecodeInterTexture( pcCU, uiDepth );

#if DEBUG_STRING
  if (DebugOptionList::DebugString_Resi.getInt()&debugPredModeMask)
  {
    printBlockToStream(std::cout, "###inter-resi: ", *(m_ppcYuvResi[uiDepth]));
  }
#endif

  // clip for only non-zero cbp case
  if  ( pcCU->getQtRootCbf( 0) )
  {
    m_ppcYuvReco[uiDepth]->addClip( m_ppcYuvReco[uiDepth], m_ppcYuvResi[uiDepth], 0, pcCU->getWidth( 0 ), pcCU->getSlice()->getSPS()->getBitDepths() );
  }
  else
  {
    m_ppcYuvReco[uiDepth]->copyPartToPartYuv( m_ppcYuvReco[uiDepth],0, pcCU->getWidth( 0 ),pcCU->getHeight( 0 ));
  }
#if DEBUG_STRING
  if (DebugOptionList::DebugString_Reco.getInt()&debugPredModeMask)
  {
    printBlockToStream(std::cout, "###inter-reco: ", *(m_ppcYuvReco[uiDepth]));
  }
#endif

}

//对给定的CU内的一个Intra预测块进行重构
Void
TDecCu::xIntraRecBlk(       TComYuv*    pcRecoYuv,
                            TComYuv*    pcPredYuv,
                            TComYuv*    pcResiYuv,
                      const ComponentID compID,
                            TComTU     &rTu)
{
  if (!rTu.ProcessComponentSection(compID))
  {
    return;
  }
  const Bool       bIsLuma = isLuma(compID);


  TComDataCU *pcCU = rTu.getCU();
  const TComSPS &sps=*(pcCU->getSlice()->getSPS());
  const UInt uiAbsPartIdx=rTu.GetAbsPartIdxTU();

  const TComRectangle &tuRect  =rTu.getRect(compID);
  const UInt uiWidth           = tuRect.width;
  const UInt uiHeight          = tuRect.height;
  const UInt uiStride          = pcRecoYuv->getStride (compID);
        Pel* piPred            = pcPredYuv->getAddr( compID, uiAbsPartIdx );
  const ChromaFormat chFmt     = rTu.GetChromaFormat();

  if (uiWidth != uiHeight)
  {
    //------------------------------------------------

    //split at current level if dividing into square sub-TUs 如果划分为正方形子单位，则按当前水平分割

    TComTURecurse subTURecurse(rTu, false, TComTU::VERTICAL_SPLIT, true, compID);

    //recurse further 进一步递归
    do
    {
      xIntraRecBlk(pcRecoYuv, pcPredYuv, pcResiYuv, compID, subTURecurse);
    } while (subTURecurse.nextSection(rTu));

    //------------------------------------------------

    return;
  }

  const UInt uiChPredMode  = pcCU->getIntraDir( toChannelType(compID), uiAbsPartIdx );
  const UInt partsPerMinCU = 1<<(2*(sps.getMaxTotalCUDepth() - sps.getLog2DiffMaxMinCodingBlockSize()));
  const UInt uiChCodedMode = (uiChPredMode==DM_CHROMA_IDX && !bIsLuma) ? pcCU->getIntraDir(CHANNEL_TYPE_LUMA, getChromasCorrespondingPULumaIdx(uiAbsPartIdx, chFmt, partsPerMinCU)) : uiChPredMode;
  const UInt uiChFinalMode = ((chFmt == CHROMA_422)       && !bIsLuma) ? g_chroma422IntraAngleMappingTable[uiChCodedMode] : uiChCodedMode;

  //===== init availability pattern  初始化可用性模式=====
  const Bool bUseFilteredPredictions=TComPrediction::filteringIntraReferenceSamples(compID, uiChFinalMode, uiWidth, uiHeight, chFmt, pcCU->getSlice()->getSPS()->getSpsRangeExtension().getIntraSmoothingDisabledFlag());

#if DEBUG_STRING
  std::ostream &ss(std::cout);
#endif

  assert( !pcCU->getColourTransform( 0 ));

  DEBUG_STRING_NEW(sTemp)
  m_pcPrediction->initIntraPatternChType( rTu, compID, bUseFilteredPredictions  DEBUG_STRING_PASS_INTO(sTemp) );


  //===== get prediction signal  获得预测信号=====

  m_pcPrediction->predIntraAng( compID,   uiChFinalMode, 0 /* Decoder does not have an original image 解码器没有原始图像 */, 0, piPred, uiStride, rTu, bUseFilteredPredictions );

#if DEBUG_STRING
  ss << sTemp;
#endif

  //===== inverse transform  逆变换=====
  Pel*      piResi            = pcResiYuv->getAddr( compID, uiAbsPartIdx );
  TCoeff*   pcCoeff           = pcCU->getCoeff(compID) + rTu.getCoefficientOffset(compID);//( uiNumCoeffInc * uiAbsPartIdx );

  assert(!pcCU->getColourTransform(uiAbsPartIdx));
  const QpParam cQP(*pcCU, compID, uiAbsPartIdx);

  DEBUG_STRING_NEW(sDebug);
#if DEBUG_STRING
  const Int debugPredModeMask=DebugStringGetPredModeMask(MODE_INTRA);
  std::string *psDebug=(DebugOptionList::DebugString_InvTran.getInt()&debugPredModeMask) ? &sDebug : 0;
#endif

  if (pcCU->getCbf(uiAbsPartIdx, compID, rTu.GetTransformDepthRel()) != 0)
  {
    m_pcTrQuant->invTransformNxN( rTu, compID, piResi, uiStride, pcCoeff, cQP DEBUG_STRING_PASS_INTO(psDebug) );
  }
  else
  {
    for (UInt y = 0; y < uiHeight; y++)
    {
      for (UInt x = 0; x < uiWidth; x++)
      {
        piResi[(y * uiStride) + x] = 0;
      }
    }
  }

#if DEBUG_STRING
  if (psDebug)
  {
    ss << (*psDebug);
  }
#endif

  //===== reconstruction 重建 =====
  const UInt uiRecIPredStride  = pcCU->getPic()->getPicYuvRec()->getStride(compID);

  const Bool useCrossComponentPrediction = isChroma(compID) && (pcCU->getCrossComponentPredictionAlpha(uiAbsPartIdx, compID) != 0);
  const Pel* pResiLuma  = pcResiYuv->getAddr( COMPONENT_Y, uiAbsPartIdx );
  const Int  strideLuma = pcResiYuv->getStride( COMPONENT_Y );

        Pel* pPred      = piPred;
        Pel* pResi      = piResi;
        Pel* pReco      = pcRecoYuv->getAddr( compID, uiAbsPartIdx );
        Pel* pRecIPred  = pcCU->getPic()->getPicYuvRec()->getAddr( compID, pcCU->getCtuRsAddr(), pcCU->getZorderIdxInCtu() + uiAbsPartIdx );


#if DEBUG_STRING
  const Bool bDebugPred=((DebugOptionList::DebugString_Pred.getInt()&debugPredModeMask) && DEBUG_STRING_CHANNEL_CONDITION(compID));
  const Bool bDebugResi=((DebugOptionList::DebugString_Resi.getInt()&debugPredModeMask) && DEBUG_STRING_CHANNEL_CONDITION(compID));
  const Bool bDebugReco=((DebugOptionList::DebugString_Reco.getInt()&debugPredModeMask) && DEBUG_STRING_CHANNEL_CONDITION(compID));
  if (bDebugPred || bDebugResi || bDebugReco)
  {
    ss << "###: " << "CompID: " << compID << " pred mode (ch/fin): " << uiChPredMode << "/" << uiChFinalMode << " absPartIdx: " << rTu.GetAbsPartIdxTU() << std::endl;
  }
#endif

  const Int clipbd = sps.getBitDepth(toChannelType(compID));
#if O0043_BEST_EFFORT_DECODING
  const Int bitDepthDelta = sps.getStreamBitDepth(toChannelType(compID)) - clipbd;
#endif

  if( useCrossComponentPrediction )
  {
    TComTrQuant::crossComponentPrediction( rTu, compID, pResiLuma, piResi, piResi, uiWidth, uiHeight, strideLuma, uiStride, uiStride, true );
  }

  for( UInt uiY = 0; uiY < uiHeight; uiY++ )
  {
#if DEBUG_STRING
    if (bDebugPred || bDebugResi || bDebugReco)
    {
      ss << "###: ";
    }

    if (bDebugPred)
    {
      ss << " - pred: ";
      for( UInt uiX = 0; uiX < uiWidth; uiX++ )
      {
        ss << pPred[ uiX ] << ", ";
      }
    }
    if (bDebugResi)
    {
      ss << " - resi: ";
    }
#endif

    for( UInt uiX = 0; uiX < uiWidth; uiX++ )
    {
#if DEBUG_STRING
      if (bDebugResi)
      {
        ss << pResi[ uiX ] << ", ";
      }
#endif
#if O0043_BEST_EFFORT_DECODING
      pReco    [ uiX ] = ClipBD( rightShiftEvenRounding<Pel>(pPred[ uiX ] + pResi[ uiX ], bitDepthDelta), clipbd );
#else
      pReco    [ uiX ] = ClipBD( pPred[ uiX ] + pResi[ uiX ], clipbd );
#endif
      pRecIPred[ uiX ] = pReco[ uiX ];
    }
#if DEBUG_STRING
    if (bDebugReco)
    {
      ss << " - reco: ";
      for( UInt uiX = 0; uiX < uiWidth; uiX++ )
      {
        ss << pReco[ uiX ] << ", ";
      }
    }

    if (bDebugPred || bDebugResi || bDebugReco)
    {
      ss << "\n";
    }
#endif
    pPred     += uiStride;
    pResi     += uiStride;
    pReco     += uiStride;
    pRecIPred += uiRecIPredStride;
  }
}

//用于在帧内预测模式下对一个CU进行递归分割和重构的函数
Void
TDecCu::xReconIntraQT( TComDataCU* pcCU, UInt uiDepth )
{
  if (pcCU->getIPCMFlag(0)) //当前 CU 是否采用了独立像素编码模式（IPCM）的标志位
  {
    xReconPCM( pcCU, uiDepth ); //调用xReconPCM函数对该块进行重构
    return;
  }
  if (pcCU->getPaletteModeFlag(0)) //该标志位为真，则表示当前CU采用调色板模式进行编码和解码
  {
    xReconPaletteMode( pcCU, uiDepth );
    return;
  }

  if( !pcCU->getSlice()->getPPS()->getPpsScreenExtension().getUseColourTrans () )//用于检查是否启用了色彩转换的条件语句
  {
    const UInt numChType = pcCU->getPic()->getChromaFormat()!=CHROMA_400 ? 2 : 1;
    for (UInt chType=CHANNEL_TYPE_LUMA; chType<numChType; chType++)
    {
      const ChannelType chanType=ChannelType(chType);
      const Bool NxNPUHas4Parts = ::isChroma(chanType) ? enable4ChromaPUsInIntraNxNCU(pcCU->getPic()->getChromaFormat()) : true;
      const UInt uiInitTrDepth = ( pcCU->getPartitionSize(0) != SIZE_2Nx2N && NxNPUHas4Parts ? 1 : 0 );

      TComTURecurse tuRecurseCU(pcCU, 0);
      TComTURecurse tuRecurseWithPU(tuRecurseCU, false, (uiInitTrDepth==0)?TComTU::DONT_SPLIT : TComTU::QUAD_SPLIT);

      do
      {
        xIntraRecQT( m_ppcYuvReco[uiDepth], m_ppcYuvReco[uiDepth], m_ppcYuvResi[uiDepth], chanType, tuRecurseWithPU );
      } while (tuRecurseWithPU.nextSection(tuRecurseCU));
    }
  }
  else
  {
    assert( pcCU->getPic()->getChromaFormat() == CHROMA_444 );
    const UInt uiInitTrDepth = ( pcCU->getPartitionSize(0) != SIZE_2Nx2N? 1 : 0 );

    TComTURecurse tuRecurseCU(pcCU, 0);
    TComTURecurse tuRecurseWithPU(tuRecurseCU, false, (uiInitTrDepth==0)?TComTU::DONT_SPLIT : TComTU::QUAD_SPLIT);

    do
    {
      xIntraRecQT( m_ppcYuvReco[uiDepth], m_ppcYuvReco[uiDepth], m_ppcYuvResi[uiDepth], tuRecurseWithPU );
    } while (tuRecurseWithPU.nextSection(tuRecurseCU));
  }
}



/** Function for deriving reconstructed PU/CU chroma samples with QTree structure 用QTree结构推导重构PU/CU色度样品的函数
 * \param pcRecoYuv pointer to reconstructed sample arrays  指针重建样本数组
 * \param pcPredYuv pointer to prediction sample arrays 预测样本数组指针
 * \param pcResiYuv pointer to residue sample arrays 指向剩余样本数组的指针
 * \param chType    texture channel type (luma/chroma) 纹理通道类型(亮度/色度)
 * \param rTu       reference to transform data 引用转换数据
 *
 \ This function derives reconstructed PU/CU chroma samples with QTree recursive structure 该函数采用QTree递归结构导出重构的PU/CU色度样本
 */
//在帧内预测模式下对一个CU进行递归分割和重构
Void
TDecCu::xIntraRecQT(TComYuv*    pcRecoYuv,
                    TComYuv*    pcPredYuv,
                    TComYuv*    pcResiYuv,
                    const ChannelType chType,
                    TComTU     &rTu)
{
  UInt uiTrDepth    = rTu.GetTransformDepthRel();
  TComDataCU *pcCU  = rTu.getCU();
  UInt uiAbsPartIdx = rTu.GetAbsPartIdxTU();
  UInt uiTrMode     = pcCU->getTransformIdx( uiAbsPartIdx );
  if( uiTrMode == uiTrDepth )
  {
    if (isLuma(chType))
    {
      xIntraRecBlk( pcRecoYuv, pcPredYuv, pcResiYuv, COMPONENT_Y,  rTu );
    }
    else
    {
      const UInt numValidComp=getNumberValidComponents(rTu.GetChromaFormat());
      for(UInt compID=COMPONENT_Cb; compID<numValidComp; compID++)
      {
        xIntraRecBlk( pcRecoYuv, pcPredYuv, pcResiYuv, ComponentID(compID), rTu );
      }
    }
  }
  else
  {
    TComTURecurse tuRecurseChild(rTu, false);
    do
    {
      xIntraRecQT( pcRecoYuv, pcPredYuv, pcResiYuv, chType, tuRecurseChild );
    } while (tuRecurseChild.nextSection(rTu));
  }
}
//将解码后的图像块拷贝到图像缓存中的函数
Void TDecCu::xCopyToPic( TComDataCU* pcCU, TComPic* pcPic, UInt uiZorderIdx, UInt uiDepth )
{
  UInt uiCtuRsAddr = pcCU->getCtuRsAddr();

  m_ppcYuvReco[uiDepth]->copyToPicYuv  ( pcPic->getPicYuvRec (), uiCtuRsAddr, uiZorderIdx );

  return;
}
//解码帧间预测残差，并将其添加到参考图像上以产生重构图像
Void TDecCu::xDecodeInterTexture ( TComDataCU* pcCU, UInt uiDepth )
{

  TComTURecurse tuRecur(pcCU, 0, uiDepth);
  if ( pcCU->getSlice()->getPPS()->getPpsScreenExtension().getUseColourTrans() )
  {
    m_pcTrQuant->invRecurTransformACTNxN( m_ppcYuvResi[uiDepth], tuRecur );
  }
  else
  {
    for(UInt ch=0; ch<pcCU->getPic()->getNumberValidComponents(); ch++)
    {
      const ComponentID compID=ComponentID(ch);
      DEBUG_STRING_OUTPUT(std::cout, debug_reorder_data_token[pcCU->isIntraBC( 0 ) ? 1 : 0][compID])

      m_pcTrQuant->invRecurTransformNxN( compID, m_ppcYuvResi[uiDepth], tuRecur );
    }
  }

  DEBUG_STRING_OUTPUT(std::cout, debug_reorder_data_token[pcCU->isIntraBC(0)?1:0][MAX_NUM_COMPONENT])
}

/** Function for deriving reconstructed luma/chroma samples of a PCM mode CU. 用于推导PCM模式CU的重建亮度/色度样本的函数。
 * \param pcCU pointer to current CU pcCU当前CU指针
 * \param uiPartIdx part index uiPartIdx部分索引
 * \param piPCM pointer to PCM code arrays piPCM指针指向PCM代码数组
 * \param piReco pointer to reconstructed sample arrays  piReco指针重建样本数组
 * \param uiStride stride of reconstructed sample arrays uiStride重建样本数组的步幅
 * \param uiWidth CU width  CU的宽度
 * \param uiHeight CU height CU的高度
 * \param compID colour component ID 颜色组件ID
 * \returns Void
 */
Void TDecCu::xDecodePCMTexture( TComDataCU* pcCU, const UInt uiPartIdx, const Pel *piPCM, Pel* piReco, const UInt uiStride, const UInt uiWidth, const UInt uiHeight, const ComponentID compID)
{
        Pel* piPicReco         = pcCU->getPic()->getPicYuvRec()->getAddr(compID, pcCU->getCtuRsAddr(), pcCU->getZorderIdxInCtu()+uiPartIdx);
  const UInt uiPicStride       = pcCU->getPic()->getPicYuvRec()->getStride(compID);
  const TComSPS &sps           = *(pcCU->getSlice()->getSPS());
  const UInt uiPcmLeftShiftBit = sps.getBitDepth(toChannelType(compID)) - sps.getPCMBitDepth(toChannelType(compID));

  for(UInt uiY = 0; uiY < uiHeight; uiY++ )
  {
    for(UInt uiX = 0; uiX < uiWidth; uiX++ )
    {
      piReco[uiX] = (piPCM[uiX] << uiPcmLeftShiftBit);
      piPicReco[uiX] = piReco[uiX];
    }
    piPCM += uiWidth;
    piReco += uiStride;
    piPicReco += uiPicStride;
  }
}

/** Function for reconstructing a PCM mode CU. 重建PCM模式CU的函数。
 * \param pcCU pointer to current CU 当前CU指针
 * \param uiDepth CU Depth  CU深度
 * \returns Void
 */
Void TDecCu::xReconPCM( TComDataCU* pcCU, UInt uiDepth )
{
  const UInt maxCuWidth     = pcCU->getSlice()->getSPS()->getMaxCUWidth();
  const UInt maxCuHeight    = pcCU->getSlice()->getSPS()->getMaxCUHeight();
  for (UInt ch=0; ch < pcCU->getPic()->getNumberValidComponents(); ch++)
  {
    const ComponentID compID = ComponentID(ch);
    const UInt width  = (maxCuWidth >>(uiDepth+m_ppcYuvResi[uiDepth]->getComponentScaleX(compID)));
    const UInt height = (maxCuHeight>>(uiDepth+m_ppcYuvResi[uiDepth]->getComponentScaleY(compID)));
    const UInt stride = m_ppcYuvResi[uiDepth]->getStride(compID);
    Pel * pPCMChannel = pcCU->getPCMSample(compID);
    Pel * pRecChannel = m_ppcYuvReco[uiDepth]->getAddr(compID);
    xDecodePCMTexture( pcCU, 0, pPCMChannel, pRecChannel, stride, width, height, compID );
  }
}

/** Function for filling the PCM buffer of a CU using its reconstructed sample array 用重构样本数组填充CU的PCM缓冲区
 * \param pCU   pointer to current CU 当前CU指针
 * \param depth CU Depth cu深度
 */
Void TDecCu::xFillPCMBuffer(TComDataCU* pCU, UInt depth)
{
  const ChromaFormat format = pCU->getPic()->getChromaFormat();
  const UInt numValidComp   = getNumberValidComponents(format);
  const UInt maxCuWidth     = pCU->getSlice()->getSPS()->getMaxCUWidth();
  const UInt maxCuHeight    = pCU->getSlice()->getSPS()->getMaxCUHeight();

  for (UInt componentIndex = 0; componentIndex < numValidComp; componentIndex++)
  {
    const ComponentID component = ComponentID(componentIndex);

    const UInt width  = maxCuWidth  >> (depth + getComponentScaleX(component, format));
    const UInt height = maxCuHeight >> (depth + getComponentScaleY(component, format));

    Pel *source      = m_ppcYuvReco[depth]->getAddr(component, 0, width);
    Pel *destination = pCU->getPCMSample(component);

    const UInt sourceStride = m_ppcYuvReco[depth]->getStride(component);

    for (Int line = 0; line < height; line++)
    {
      for (Int column = 0; column < width; column++)
      {
        destination[column] = source[column];
      }

      source      += sourceStride;
      destination += width;
    }
  }
}
//用于存储当前处理的亮度或色度块的重建像素值
Void
TDecCu::xIntraRecBlk( TComYuv*    pcRecoYuv,
                      TComYuv*    pcPredYuv,
                      TComYuv*    pcResiYuv,
                      TComTU     &rTu)
{
  TComDataCU         *pcCU        = rTu.getCU();
  const UInt         absPartIdx   = rTu.GetAbsPartIdxTU();
  const ChromaFormat chFmt        = rTu.GetChromaFormat();
  const Bool    extendedPrecision = pcCU->getSlice()->getSPS()->getSpsRangeExtension().getExtendedPrecisionProcessingFlag();

  for(UInt ch = 0; ch < MAX_NUM_COMPONENT; ch++)
  {
    ComponentID         compID   = ComponentID(ch);
    const TComRectangle &tuRect  = rTu.getRect(compID);
    const UInt          width    = tuRect.width;
    const UInt          height   = tuRect.height;
    const Bool          bIsLuma  = isLuma(compID);
    assert( width == height );

    Pel*                piPred   = pcPredYuv->getAddr( compID, absPartIdx );
    Pel*                piResi   = pcResiYuv->getAddr( compID, absPartIdx );
    TCoeff*             pcCoeff  = pcCU->getCoeff(compID) + rTu.getCoefficientOffset(compID);
    const UInt          stride   = pcPredYuv->getStride( compID );

    //===== get prediction signal =====
    const TComSPS *sps = pcCU->getSlice()->getSPS(); 
    const UInt partsPerMinCU = 1<<(2*(sps->getMaxTotalCUDepth() - sps->getLog2DiffMaxMinCodingBlockSize()));
    const UInt chPredMode    = pcCU->getIntraDir( toChannelType(compID), absPartIdx );
    const UInt chFinalMode   = (chPredMode == DM_CHROMA_IDX && !bIsLuma) ? pcCU->getIntraDir(CHANNEL_TYPE_LUMA, getChromasCorrespondingPULumaIdx(absPartIdx, chFmt, partsPerMinCU)) : chPredMode;

    const Bool bUseFilteredPredictions = TComPrediction::filteringIntraReferenceSamples(compID, chFinalMode, width, height, chFmt, pcCU->getSlice()->getSPS()->getSpsRangeExtension().getIntraSmoothingDisabledFlag());
#if DEBUG_STRING
    std::ostream &ss(std::cout);
#endif
    DEBUG_STRING_NEW(sTemp)
      m_pcPrediction->initIntraPatternChType( rTu, compID, bUseFilteredPredictions  DEBUG_STRING_PASS_INTO(sTemp)  );
    m_pcPrediction->predIntraAng( compID,   chFinalMode, 0, 0, piPred, stride, rTu, bUseFilteredPredictions );

#if DEBUG_STRING
    ss << sTemp;
#endif

    //===== inverse transform 逆变换=====
    QpParam cQP(*pcCU, compID, absPartIdx);

    DEBUG_STRING_NEW( sDebug );
#if DEBUG_STRING
    const Int debugPredModeMask=DebugStringGetPredModeMask( MODE_INTRA );
    std::string *psDebug=(DebugOptionList::DebugString_InvTran.getInt()&debugPredModeMask) ? &sDebug : 0;
#endif

    if (pcCU->getCbf(absPartIdx, compID, rTu.GetTransformDepthRel()) != 0)
    {
      m_pcTrQuant->invTransformNxN( rTu, compID, piResi, stride, pcCoeff, cQP DEBUG_STRING_PASS_INTO(psDebug) );
    }
    else
    {
      for ( UInt y = 0; y < height; y++ )
      {
        for ( UInt x = 0; x < width; x++ )
        {
          piResi[(y * stride) + x] = 0;
        }
      }
    }

#if DEBUG_STRING
    if (psDebug)
      ss << (*psDebug);
#endif

    const Bool useCrossComponentPrediction = isChroma(compID) && (pcCU->getCrossComponentPredictionAlpha(absPartIdx, compID) != 0);
    if( useCrossComponentPrediction ) 
    {
      const Pel* pResiLuma                      = pcResiYuv->getAddr( COMPONENT_Y, absPartIdx );
      const Int  strideLuma                     = pcResiYuv->getStride( COMPONENT_Y );
      TComTrQuant::crossComponentPrediction( rTu, compID, pResiLuma, piResi, piResi, width, height, strideLuma, stride, stride, true );
    }
  }

  UInt uiTrMode = rTu.GetTransformDepthRel();
  if( pcCU->getColourTransform(absPartIdx) && (pcCU->getCbf(absPartIdx, COMPONENT_Y, uiTrMode) || pcCU->getCbf(absPartIdx, COMPONENT_Cb, uiTrMode) || pcCU->getCbf(absPartIdx, COMPONENT_Cr, uiTrMode)) )
  {
    pcResiYuv->convert(extendedPrecision, rTu.getRect(COMPONENT_Y).x0, rTu.getRect(COMPONENT_Y).y0, rTu.getRect(COMPONENT_Y).width, false, pcCU->getSlice()->getSPS()->getBitDepths(), pcCU->isLosslessCoded(absPartIdx));
  }

  //===== reconstruction 重建 =====

  for(UInt ch = 0; ch < MAX_NUM_COMPONENT; ch++)
  {
    ComponentID         compID           = ComponentID(ch);
    const TComRectangle &tuRect          = rTu.getRect(compID);
    const UInt          width            = tuRect.width;
    const UInt          height           = tuRect.height;
    Pel*                pPred            = pcPredYuv->getAddr( compID, absPartIdx );
    Pel*                pResi            = pcResiYuv->getAddr( compID, absPartIdx );
    Pel*                pReco            = pcRecoYuv->getAddr( compID, absPartIdx );
    Pel*                pRecIPred        = pcCU->getPic()->getPicYuvRec()->getAddr( compID, pcCU->getCtuRsAddr(), pcCU->getZorderIdxInCtu() + absPartIdx );
    const UInt          stride           = pcResiYuv->getStride( compID );
    const UInt          recIPredStride   = pcCU->getPic()->getPicYuvRec()->getStride(compID);
    const Int           clipbd           = pcCU->getSlice()->getSPS()->getBitDepths().recon[toChannelType(compID)];
    for( UInt y = 0; y < height; y++ )
    {
      for( UInt x = 0; x < width; x++ )
      {
        pReco    [ x ] = ClipBD( pPred[ x ] + pResi[ x ], clipbd );
        pRecIPred[ x ] = pReco[ x ];
      }

      pPred     += stride;
      pResi     += stride;
      pReco     += stride;
      pRecIPred += recIPredStride;
    }
  }
}
//对一个 Intra 预测块进行递归 Quadtree 分割
Void
TDecCu::xIntraRecQT(TComYuv*    pcRecoYuv,
                    TComYuv*    pcPredYuv,
                    TComYuv*    pcResiYuv,
                    TComTU      &rTu)
{
  UInt trDepth    = rTu.GetTransformDepthRel();
  TComDataCU *pcCU  = rTu.getCU();
  UInt absPartIdx   = rTu.GetAbsPartIdxTU();
  UInt trMode     = pcCU->getTransformIdx( absPartIdx );

  if (trMode == trDepth)
  {
    xIntraRecBlk(pcRecoYuv, pcPredYuv, pcResiYuv, rTu);
  }
  else
  {
    TComTURecurse tuRecurseChild(rTu, false);
    do
    {
      xIntraRecQT( pcRecoYuv, pcPredYuv, pcResiYuv, tuRecurseChild );
    } while (tuRecurseChild.nextSection(rTu));
  }
}
//作用是对当前CU块进行调色板纹理的解码
Void TDecCu::xDecodePaletteTexture( TComDataCU* pcCU, const UInt partIdx, Pel* pPalette,  Pel* pLevel, Pel *pPixelValue, Pel* piReco,const UInt stride, const UInt width, const UInt height, const ComponentID compID, UChar* pEscapeFlag )
{
  Bool bLossless = pcCU->getCUTransquantBypass (partIdx);
  Pel* piPicReco         = pcCU->getPic()->getPicYuvRec()->getAddr(compID, pcCU->getCtuRsAddr(), pcCU->getZorderIdxInCtu()+partIdx);
  const UInt picStride = pcCU->getPic()->getPicYuvRec()->getStride(compID);
  UInt idx = 0;
  TCoeff iValue = 0;
  Bool bRotation = pcCU->getPaletteScanRotationModeFlag(partIdx);

  if(!bRotation)
  {
    for(UInt y = 0; y < height; y++ )
    {
      for(UInt x = 0; x < width; x++ )
      {
        idx = (y<<pcCU->getPic()->getComponentScaleY(compID))*(width<<pcCU->getPic()->getComponentScaleX(compID))+(x<<pcCU->getPic()->getComponentScaleX(compID));
        UInt idxComp = y*width + x;
        if( pEscapeFlag[idx] )
        {
          if ( bLossless )
          {
            iValue = pPixelValue[idxComp];
          }
          else
          {
            assert(!pcCU->getColourTransform(partIdx));
            QpParam cQP(*pcCU, compID, partIdx);
            Int iQP = cQP.Qp;
            Int iQPrem = iQP % 6;
            Int iQPper = iQP / 6;
            Int InvquantiserRightShift = IQUANT_SHIFT;
            Int iAdd = 1 << (InvquantiserRightShift - 1);
            iValue = ((((pPixelValue[idxComp]*g_invQuantScales[iQPrem])<<iQPper) + iAdd)>>InvquantiserRightShift);
            iValue = ClipBD<Int>(iValue, pcCU->getSlice()->getSPS()->getBitDepths().recon[compID?1:0]);
          }
        }
        else
        {
          iValue = pPalette[pLevel[idx]];
        }
        piReco[y*stride+x] = iValue;
        piPicReco[y*picStride+x] = iValue;
      }
    }
  }
  else
  {
    for(UInt y = 0; y < width; y++ )
    {
      for(UInt x = 0; x < height; x++ )
      {
        idx = (y<<pcCU->getPic()->getComponentScaleX(compID))*(height<<pcCU->getPic()->getComponentScaleY(compID))+(x<<pcCU->getPic()->getComponentScaleY(compID));
        UInt idxComp = y*height + x;
        if( pEscapeFlag[idx] )
        {
          if ( bLossless )
          {
            iValue = pPixelValue[idxComp];
          }
          else
          {
            assert(!pcCU->getColourTransform(partIdx));
            QpParam cQP(*pcCU, compID, partIdx);
            Int iQP = cQP.Qp;
            Int iQPrem = iQP % 6;
            Int iQPper = iQP / 6;
            Int InvquantiserRightShift = IQUANT_SHIFT;
            Int iAdd = 1 << (InvquantiserRightShift - 1);
            iValue = ((((pPixelValue[idxComp]*g_invQuantScales[iQPrem])<<iQPper) + iAdd)>>InvquantiserRightShift);
            iValue = ClipBD<Int>(iValue, pcCU->getSlice()->getSPS()->getBitDepths().recon[compID?1:0]);
          }
        }
        else
        {
          iValue = pPalette[pLevel[idx]];
        }

        piReco[x*stride+y] = iValue;
        piPicReco[x*picStride+y] = iValue;
      }
    }
  }
}
//对当前CU块进行调色板模式下的重构
Void TDecCu::xReconPaletteMode(TComDataCU *pcCU, UInt depth)
{
  Pel *pLevel, *pPalette, *pRecChannel;
  for (UInt ch = 0; ch < pcCU->getPic()->getNumberValidComponents(); ch++)
  {
    const ComponentID compID = ComponentID(ch);
    const UInt width = (pcCU->getSlice()->getSPS()->getMaxCUWidth() >> (depth + m_ppcYuvResi[depth]->getComponentScaleX(compID)));
    const UInt height = (pcCU->getSlice()->getSPS()->getMaxCUHeight() >> (depth + m_ppcYuvResi[depth]->getComponentScaleY(compID)));
    const UInt stride = m_ppcYuvResi[depth]->getStride(compID);

    pLevel = pcCU->getLevel(COMPONENT_Y);
    pPalette = pcCU->getPalette(compID, 0);
    pRecChannel = m_ppcYuvReco[depth]->getAddr(compID);

    Pel *pPixelValue = pcCU->getLevel(compID);
    xDecodePaletteTexture(pcCU, 0, pPalette, pLevel, pPixelValue, pRecChannel, stride, width, height, compID, pcCU->getEscapeFlag(COMPONENT_Y));
  }
}

//! \}
