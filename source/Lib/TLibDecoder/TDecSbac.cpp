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

/** \file     TDecSbac.cpp
    \brief    Context-adaptive entropy decoder class ����������Ӧ�ؽ�������
*/

#include "TDecSbac.h"
#include "TLibCommon/TComTU.h"
#include "TLibCommon/TComTrQuant.h"

#if RExt__DECODER_DEBUG_BIT_STATISTICS
#include "TLibCommon/TComCodingStatistics.h"
//
#define RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(a) , a
#else
#define RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(a)
#endif

//! \ingroup TLibDecoder
//! \{

#if ENVIRONMENT_VARIABLE_DEBUG_AND_TEST
#include "../TLibCommon/Debug.h"
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TDecSbac::TDecSbac()
// new structure here �������µĽṹ
: m_pcBitstream                              ( 0 )
, m_pcTDecBinIf                              ( NULL )
, m_numContextModels                         ( 0 )
, m_cCUSplitFlagSCModel                      ( 1,             1,                      NUM_SPLIT_FLAG_CTX                   , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUSkipFlagSCModel                       ( 1,             1,                      NUM_SKIP_FLAG_CTX                    , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUMergeFlagExtSCModel                   ( 1,             1,                      NUM_MERGE_FLAG_EXT_CTX               , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUMergeIdxExtSCModel                    ( 1,             1,                      NUM_MERGE_IDX_EXT_CTX                , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUPartSizeSCModel                       ( 1,             1,                      NUM_PART_SIZE_CTX                    , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUPredModeSCModel                       ( 1,             1,                      NUM_PRED_MODE_CTX                    , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUIntraPredSCModel                      ( 1,             1,                      NUM_INTRA_PREDICT_CTX                , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUChromaPredSCModel                     ( 1,             1,                      NUM_CHROMA_PRED_CTX                  , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUDeltaQpSCModel                        ( 1,             1,                      NUM_DELTA_QP_CTX                     , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUInterDirSCModel                       ( 1,             1,                      NUM_INTER_DIR_CTX                    , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCURefPicSCModel                         ( 1,             1,                      NUM_REF_NO_CTX                       , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUMvdSCModel                            ( 1,             1,                      NUM_MV_RES_CTX                       , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUQtCbfSCModel                          ( 1,             NUM_QT_CBF_CTX_SETS,    NUM_QT_CBF_CTX_PER_SET               , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUTransSubdivFlagSCModel                ( 1,             1,                      NUM_TRANS_SUBDIV_FLAG_CTX            , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUQtRootCbfSCModel                      ( 1,             1,                      NUM_QT_ROOT_CBF_CTX                  , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUSigCoeffGroupSCModel                  ( 1,             2,                      NUM_SIG_CG_FLAG_CTX                  , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUSigSCModel                            ( 1,             1,                      NUM_SIG_FLAG_CTX                     , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCuCtxLastX                              ( 1,             NUM_CTX_LAST_FLAG_SETS, NUM_CTX_LAST_FLAG_XY                 , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCuCtxLastY                              ( 1,             NUM_CTX_LAST_FLAG_SETS, NUM_CTX_LAST_FLAG_XY                 , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUOneSCModel                            ( 1,             1,                      NUM_ONE_FLAG_CTX                     , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUAbsSCModel                            ( 1,             1,                      NUM_ABS_FLAG_CTX                     , m_contextModels + m_numContextModels, m_numContextModels)
, m_cMVPIdxSCModel                           ( 1,             1,                      NUM_MVP_IDX_CTX                      , m_contextModels + m_numContextModels, m_numContextModels)
, m_cSaoMergeSCModel                         ( 1,             1,                      NUM_SAO_MERGE_FLAG_CTX               , m_contextModels + m_numContextModels, m_numContextModels)
, m_cSaoTypeIdxSCModel                       ( 1,             1,                      NUM_SAO_TYPE_IDX_CTX                 , m_contextModels + m_numContextModels, m_numContextModels)
, m_cTransformSkipSCModel                    ( 1,             MAX_NUM_CHANNEL_TYPE,   NUM_TRANSFORMSKIP_FLAG_CTX           , m_contextModels + m_numContextModels, m_numContextModels)
, m_CUTransquantBypassFlagSCModel            ( 1,             1,                      NUM_CU_TRANSQUANT_BYPASS_FLAG_CTX    , m_contextModels + m_numContextModels, m_numContextModels)
, m_explicitRdpcmFlagSCModel                 ( 1,             MAX_NUM_CHANNEL_TYPE,   NUM_EXPLICIT_RDPCM_FLAG_CTX          , m_contextModels + m_numContextModels, m_numContextModels)
, m_explicitRdpcmDirSCModel                  ( 1,             MAX_NUM_CHANNEL_TYPE,   NUM_EXPLICIT_RDPCM_DIR_CTX           , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCrossComponentPredictionSCModel         ( 1,             1,                      NUM_CROSS_COMPONENT_PREDICTION_CTX   , m_contextModels + m_numContextModels, m_numContextModels)
, m_ChromaQpAdjFlagSCModel                   ( 1,             1,                      NUM_CHROMA_QP_ADJ_FLAG_CTX           , m_contextModels + m_numContextModels, m_numContextModels)
, m_ChromaQpAdjIdcSCModel                    ( 1,             1,                      NUM_CHROMA_QP_ADJ_IDC_CTX            , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUColourTransformFlagSCModel            ( 1,             1,                      NUM_COLOUR_TRANS_CTX                 , m_contextModels + m_numContextModels, m_numContextModels)
, m_paletteModeFlagSCModel                   ( 1,             1,                      NUM_PALETTE_MODE_FLAG_CTX            , m_contextModels + m_numContextModels, m_numContextModels)
, m_SPointSCModel                            ( 1,             1,                      NUM_SPOINT_CTX                       , m_contextModels + m_numContextModels, m_numContextModels)
, m_cCopyTopRunSCModel                       ( 1,             1,                      NUM_TOP_RUN_CTX                      , m_contextModels + m_numContextModels, m_numContextModels)
, m_cRunSCModel                              ( 1,             1,                      NUM_LEFT_RUN_CTX                     , m_contextModels + m_numContextModels, m_numContextModels)
, m_paletteScanRotationModeFlagSCModel       ( 1,             1,                      NUM_SCAN_ROTATION_FLAG_CTX           , m_contextModels + m_numContextModels, m_numContextModels)
{
  assert( m_numContextModels <= MAX_NUM_CTX_MOD );
  m_runTopLut[0] = 0;
  m_runTopLut[1] = 1;
  m_runTopLut[2] = 1;
  m_runTopLut[3] = 2;
  m_runTopLut[4] = 2;

  m_runLeftLut[0] = 0;
  m_runLeftLut[1] = 3;
  m_runLeftLut[2] = 3;
  m_runLeftLut[3] = 4;
  m_runLeftLut[4] = 4;
}

TDecSbac::~TDecSbac()
{
}

// ====================================================================================================================
// Public member functions /������Ա����
// ====================================================================================================================

Void TDecSbac::resetEntropy(TComSlice* pSlice)
{
  SliceType sliceType  = pSlice->getSliceType();
  Int       qp         = pSlice->getSliceQp();

  if (pSlice->getPPS()->getCabacInitPresentFlag() && pSlice->getCabacInitFlag())
  {
    switch (sliceType)
    {
    case P_SLICE:           // change initialization table to B_SLICE initialization ����ʼ��������ΪB_SLICE initialization
      sliceType = B_SLICE;
      break;
    case B_SLICE:           // change initialization table to P_SLICE initialization ����ʼ��������ΪP_SLICE��ʼ��
      sliceType = P_SLICE;
      break;
    default     :           // should not occur ��Ӧ�÷���
      assert(0);
      break;
    }
  }

  m_cCUSplitFlagSCModel.initBuffer                ( sliceType, qp, (UChar*)INIT_SPLIT_FLAG );
  m_cCUSkipFlagSCModel.initBuffer                 ( sliceType, qp, (UChar*)INIT_SKIP_FLAG );
  m_cCUMergeFlagExtSCModel.initBuffer             ( sliceType, qp, (UChar*)INIT_MERGE_FLAG_EXT );
  m_cCUMergeIdxExtSCModel.initBuffer              ( sliceType, qp, (UChar*)INIT_MERGE_IDX_EXT );
  m_cCUPartSizeSCModel.initBuffer                 ( sliceType, qp, (UChar*)INIT_PART_SIZE );
  m_cCUPredModeSCModel.initBuffer                 ( sliceType, qp, (UChar*)INIT_PRED_MODE );
  m_cCUIntraPredSCModel.initBuffer                ( sliceType, qp, (UChar*)INIT_INTRA_PRED_MODE );
  m_cCUChromaPredSCModel.initBuffer               ( sliceType, qp, (UChar*)INIT_CHROMA_PRED_MODE );
  m_cCUInterDirSCModel.initBuffer                 ( sliceType, qp, (UChar*)INIT_INTER_DIR );
  m_cCUMvdSCModel.initBuffer                      ( sliceType, qp, (UChar*)INIT_MVD );
  m_cCURefPicSCModel.initBuffer                   ( sliceType, qp, (UChar*)INIT_REF_PIC );
  m_cCUDeltaQpSCModel.initBuffer                  ( sliceType, qp, (UChar*)INIT_DQP );
  m_cCUQtCbfSCModel.initBuffer                    ( sliceType, qp, (UChar*)INIT_QT_CBF );
  m_cCUQtRootCbfSCModel.initBuffer                ( sliceType, qp, (UChar*)INIT_QT_ROOT_CBF );
  m_cCUSigCoeffGroupSCModel.initBuffer            ( sliceType, qp, (UChar*)INIT_SIG_CG_FLAG );
  m_cCUSigSCModel.initBuffer                      ( sliceType, qp, (UChar*)INIT_SIG_FLAG );
  m_cCuCtxLastX.initBuffer                        ( sliceType, qp, (UChar*)INIT_LAST );
  m_cCuCtxLastY.initBuffer                        ( sliceType, qp, (UChar*)INIT_LAST );
  m_cCUOneSCModel.initBuffer                      ( sliceType, qp, (UChar*)INIT_ONE_FLAG );
  m_cCUAbsSCModel.initBuffer                      ( sliceType, qp, (UChar*)INIT_ABS_FLAG );
  m_cMVPIdxSCModel.initBuffer                     ( sliceType, qp, (UChar*)INIT_MVP_IDX );
  m_cSaoMergeSCModel.initBuffer                   ( sliceType, qp, (UChar*)INIT_SAO_MERGE_FLAG );
  m_cSaoTypeIdxSCModel.initBuffer                 ( sliceType, qp, (UChar*)INIT_SAO_TYPE_IDX );
  m_cCUTransSubdivFlagSCModel.initBuffer          ( sliceType, qp, (UChar*)INIT_TRANS_SUBDIV_FLAG );
  m_cTransformSkipSCModel.initBuffer              ( sliceType, qp, (UChar*)INIT_TRANSFORMSKIP_FLAG );
  m_CUTransquantBypassFlagSCModel.initBuffer      ( sliceType, qp, (UChar*)INIT_CU_TRANSQUANT_BYPASS_FLAG );
  m_explicitRdpcmFlagSCModel.initBuffer           ( sliceType, qp, (UChar*)INIT_EXPLICIT_RDPCM_FLAG);
  m_explicitRdpcmDirSCModel.initBuffer            ( sliceType, qp, (UChar*)INIT_EXPLICIT_RDPCM_DIR);
  m_cCrossComponentPredictionSCModel.initBuffer   ( sliceType, qp, (UChar*)INIT_CROSS_COMPONENT_PREDICTION );
  m_ChromaQpAdjFlagSCModel.initBuffer             ( sliceType, qp, (UChar*)INIT_CHROMA_QP_ADJ_FLAG );
  m_ChromaQpAdjIdcSCModel.initBuffer              ( sliceType, qp, (UChar*)INIT_CHROMA_QP_ADJ_IDC );
  m_cCUColourTransformFlagSCModel.initBuffer      ( sliceType, qp, (UChar*)INIT_COLOUR_TRANS);
  m_paletteModeFlagSCModel.initBuffer             ( sliceType, qp, (UChar*)INIT_PALETTE_MODE_FLAG );
  m_SPointSCModel.initBuffer                      ( sliceType, qp, (UChar*)INIT_SPOINT );
  m_cCopyTopRunSCModel.initBuffer                 ( sliceType, qp, (UChar*)INIT_TOP_RUN);
  m_cRunSCModel.initBuffer                        ( sliceType, qp, (UChar*)INIT_RUN);
  m_paletteScanRotationModeFlagSCModel.initBuffer ( sliceType, qp, (UChar*)INIT_SCAN_ROTATION_FLAG );
  for (UInt statisticIndex = 0; statisticIndex < RExt__GOLOMB_RICE_ADAPTATION_STATISTICS_SETS ; statisticIndex++)
  {
    m_golombRiceAdaptationStatistics[statisticIndex] = 0;
  }

  m_pcTDecBinIf->start();
}

Void TDecSbac::parseTerminatingBit( UInt& ruiBit )
{
  m_pcTDecBinIf->decodeBinTrm( ruiBit );
  if ( ruiBit == 1 )
  {
    m_pcTDecBinIf->finish();

#if RExt__DECODER_DEBUG_BIT_STATISTICS
    TComCodingStatistics::IncrementStatisticEP(STATS__TRAILING_BITS, m_pcBitstream->readOutTrailingBits(),0);
#else
    m_pcBitstream->readOutTrailingBits();
#endif
  }
}

Void TDecSbac::parseRemainingBytes( Bool noTrailingBytesExpected )
{
  if (noTrailingBytesExpected)
  {
    const UInt numberOfRemainingSubstreamBytes=m_pcBitstream->getNumBitsLeft();
    assert (numberOfRemainingSubstreamBytes == 0);
  }
  else
  {
    while (m_pcBitstream->getNumBitsLeft())
    {
      UInt trailingNullByte=m_pcBitstream->readByte();
      if (trailingNullByte!=0)
      {
        printf("Trailing byte should be 0, but has value %02x\n", trailingNullByte);
        assert(trailingNullByte==0);
      }
    }
  }
}

#if RExt__DECODER_DEBUG_BIT_STATISTICS
Void TDecSbac::xReadUnaryMaxSymbol( UInt& ruiSymbol, ContextModel* pcSCModel, Int iOffset, UInt uiMaxSymbol, const class TComCodingStatisticsClassType &whichStat )
#else
Void TDecSbac::xReadUnaryMaxSymbol( UInt& ruiSymbol, ContextModel* pcSCModel, Int iOffset, UInt uiMaxSymbol )
#endif
{
  if (uiMaxSymbol == 0)
  {
    ruiSymbol = 0;
    return;
  }

  m_pcTDecBinIf->decodeBin( ruiSymbol, pcSCModel[0] RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(whichStat) );

  if( ruiSymbol == 0 || uiMaxSymbol == 1 )
  {
    return;
  }

  UInt uiSymbol = 0;
  UInt uiCont;

  do
  {
    m_pcTDecBinIf->decodeBin( uiCont, pcSCModel[ iOffset ] RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(whichStat) );
    uiSymbol++;
  } while( uiCont && ( uiSymbol < uiMaxSymbol - 1 ) );

  if( uiCont && ( uiSymbol == uiMaxSymbol - 1 ) )
  {
    uiSymbol++;
  }

  ruiSymbol = uiSymbol;
}

#if RExt__DECODER_DEBUG_BIT_STATISTICS
Void TDecSbac::xReadEpExGolomb( UInt& ruiSymbol, UInt uiCount, const class TComCodingStatisticsClassType &whichStat )
#else
Void TDecSbac::xReadEpExGolomb( UInt& ruiSymbol, UInt uiCount )
#endif
{
  UInt uiSymbol = 0;
  UInt uiBit = 1;

  while( uiBit )
  {
    m_pcTDecBinIf->decodeBinEP( uiBit RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(whichStat) );
    uiSymbol += uiBit << uiCount++;
  }

  if ( --uiCount )
  {
    UInt bins;
    m_pcTDecBinIf->decodeBinsEP( bins, uiCount RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(whichStat) );
    uiSymbol += bins;
  }

  ruiSymbol = uiSymbol;
}

#if RExt__DECODER_DEBUG_BIT_STATISTICS
Void TDecSbac::xReadUnarySymbol( UInt& ruiSymbol, ContextModel* pcSCModel, Int iOffset, const class TComCodingStatisticsClassType &whichStat )
#else
Void TDecSbac::xReadUnarySymbol( UInt& ruiSymbol, ContextModel* pcSCModel, Int iOffset )
#endif
{
  m_pcTDecBinIf->decodeBin( ruiSymbol, pcSCModel[0] RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(whichStat));

  if( !ruiSymbol )
  {
    return;
  }

  UInt uiSymbol = 0;
  UInt uiCont;

  do
  {
    m_pcTDecBinIf->decodeBin( uiCont, pcSCModel[ iOffset ] RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(whichStat));
    uiSymbol++;
  } while( uiCont );

  ruiSymbol = uiSymbol;
}


/** Parsing of coeff_abs_level_remaing ** coeff_abs_level_remain����
 * \param rSymbol                 reference to coeff_abs_level_remaing ����coeff_abs_level_remain
 * \param rParam                  reference to parameter ��������
 * \param useLimitedPrefixLength
 * \param maxLog2TrDynamicRange
 */
#if RExt__DECODER_DEBUG_BIT_STATISTICS
Void TDecSbac::xReadCoefRemainExGolomb ( UInt &rSymbol, UInt &rParam, const Bool useLimitedPrefixLength, const Int maxLog2TrDynamicRange, const class TComCodingStatisticsClassType &whichStat )
#else
Void TDecSbac::xReadCoefRemainExGolomb ( UInt &rSymbol, UInt &rParam, const Bool useLimitedPrefixLength, const Int maxLog2TrDynamicRange )
#endif
{
  UInt prefix   = 0;
  UInt codeWord = 0;

  if (useLimitedPrefixLength)
  {
    const UInt longestPossiblePrefix = (32 - (COEF_REMAIN_BIN_REDUCTION + maxLog2TrDynamicRange)) + COEF_REMAIN_BIN_REDUCTION;

    do
    {
      prefix++;
      m_pcTDecBinIf->decodeBinEP( codeWord RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(whichStat) );
    } while((codeWord != 0) && (prefix < longestPossiblePrefix));
  }
  else
  {
    do
    {
      prefix++;
      m_pcTDecBinIf->decodeBinEP( codeWord RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(whichStat) );
    } while( codeWord);
  }

  codeWord  = 1 - codeWord;
  prefix -= codeWord;
  codeWord=0;

  if (prefix < COEF_REMAIN_BIN_REDUCTION )
  {
    m_pcTDecBinIf->decodeBinsEP(codeWord,rParam RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(whichStat));
    rSymbol = (prefix<<rParam) + codeWord;
  }
  else if (useLimitedPrefixLength)
  {
    const UInt maximumPrefixLength = (32 - (COEF_REMAIN_BIN_REDUCTION + maxLog2TrDynamicRange));

    const UInt prefixLength = prefix - COEF_REMAIN_BIN_REDUCTION;
    const UInt suffixLength = (prefixLength == maximumPrefixLength) ? (maxLog2TrDynamicRange - rParam) : prefixLength;

    m_pcTDecBinIf->decodeBinsEP(codeWord, (suffixLength + rParam) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(whichStat));

    rSymbol = codeWord + ((((1 << prefixLength) - 1) + COEF_REMAIN_BIN_REDUCTION) << rParam);
  }
  else
  {
    m_pcTDecBinIf->decodeBinsEP(codeWord,prefix-COEF_REMAIN_BIN_REDUCTION+rParam RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(whichStat));
    rSymbol = (((1<<(prefix-COEF_REMAIN_BIN_REDUCTION))+COEF_REMAIN_BIN_REDUCTION-1)<<rParam)+codeWord;
  }
}


/** Parse I_PCM information. ����I_PCM��Ϣ��
 * \param pcCU
 * \param uiAbsPartIdx
 * \param uiDepth
 * \returns Void
 *
 * If I_PCM flag indicates that the CU is I_PCM, parse its PCM alignment bits and codes. ���I_PCM��־��ʾCU��I_PCM�����������PCM����λ�ʹ��롣
 */
Void TDecSbac::parseIPCMInfo ( TComDataCU* pcCU, UInt uiAbsPartIdx, UInt uiDepth )
{
  UInt uiSymbol;

  m_pcTDecBinIf->decodeBinTrm(uiSymbol);

  if (uiSymbol == 1)
  {
    Bool bIpcmFlag = true;
    const TComSPS &sps=*(pcCU->getSlice()->getSPS());

    pcCU->setPartSizeSubParts  ( SIZE_2Nx2N, uiAbsPartIdx, uiDepth );
    pcCU->setSizeSubParts      ( sps.getMaxCUWidth()>>uiDepth, sps.getMaxCUHeight()>>uiDepth, uiAbsPartIdx, uiDepth );
    pcCU->setTrIdxSubParts     ( 0, uiAbsPartIdx, uiDepth );
    pcCU->setIPCMFlagSubParts  ( bIpcmFlag, uiAbsPartIdx, uiDepth );

    const UInt minCoeffSizeY = pcCU->getPic()->getMinCUWidth() * pcCU->getPic()->getMinCUHeight();
    const UInt offsetY       = minCoeffSizeY * uiAbsPartIdx;
    for (UInt ch=0; ch < pcCU->getPic()->getNumberValidComponents(); ch++)
    {
      const ComponentID compID = ComponentID(ch);
      const UInt offset = offsetY >> (pcCU->getPic()->getComponentScaleX(compID) + pcCU->getPic()->getComponentScaleY(compID));
      Pel * pPCMSample  = pcCU->getPCMSample(compID) + offset;
      const UInt width  = pcCU->getWidth (uiAbsPartIdx) >> pcCU->getPic()->getComponentScaleX(compID);
      const UInt height = pcCU->getHeight(uiAbsPartIdx) >> pcCU->getPic()->getComponentScaleY(compID);
      const UInt sampleBits = pcCU->getSlice()->getSPS()->getPCMBitDepth(toChannelType(compID));
      for (UInt y=0; y<height; y++)
      {
        for (UInt x=0; x<width; x++)
        {
          UInt sample;
          m_pcTDecBinIf->xReadPCMCode(sampleBits, sample);
          pPCMSample[x] = sample;
        }
        pPCMSample += width;
      }
    }

    m_pcTDecBinIf->start();
  }
}

Void TDecSbac::parseCUTransquantBypassFlag( TComDataCU* pcCU, UInt uiAbsPartIdx, UInt uiDepth )
{
  UInt uiSymbol;
  m_pcTDecBinIf->decodeBin( uiSymbol, m_CUTransquantBypassFlagSCModel.get( 0, 0, 0 ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__TQ_BYPASS_FLAG) );
  pcCU->setCUTransquantBypassSubParts(uiSymbol ? true : false, uiAbsPartIdx, uiDepth);
}

/** parse skip flag ����������־
 * \param pcCU
 * \param uiAbsPartIdx
 * \param uiDepth
 * \returns Void
 */
Void TDecSbac::parseSkipFlag( TComDataCU* pcCU, UInt uiAbsPartIdx, UInt uiDepth )
{
  if( pcCU->getSlice()->isIntra() )
  {
    return;
  }

  UInt uiSymbol = 0;
  UInt uiCtxSkip = pcCU->getCtxSkipFlag( uiAbsPartIdx );
  m_pcTDecBinIf->decodeBin( uiSymbol, m_cCUSkipFlagSCModel.get( 0, 0, uiCtxSkip ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__SKIP_FLAG) );
  DTRACE_CABAC_VL( g_nSymbolCounter++ );
  DTRACE_CABAC_T( "\tSkipFlag" );
  DTRACE_CABAC_T( "\tuiCtxSkip: ");
  DTRACE_CABAC_V( uiCtxSkip );
  DTRACE_CABAC_T( "\tuiSymbol: ");
  DTRACE_CABAC_V( uiSymbol );
  DTRACE_CABAC_T( "\n");

  if( uiSymbol )
  {
    pcCU->setSkipFlagSubParts( true,        uiAbsPartIdx, uiDepth );
    pcCU->setPredModeSubParts( MODE_INTER,  uiAbsPartIdx, uiDepth );
    pcCU->setPartSizeSubParts( SIZE_2Nx2N, uiAbsPartIdx, uiDepth );
    pcCU->setSizeSubParts( pcCU->getSlice()->getSPS()->getMaxCUWidth()>>uiDepth, pcCU->getSlice()->getSPS()->getMaxCUHeight()>>uiDepth, uiAbsPartIdx, uiDepth );
    pcCU->setMergeFlagSubParts( true , uiAbsPartIdx, 0, uiDepth );
  }
}


/** parse merge flag �����ϲ���־
 * \param pcCU
 * \param uiAbsPartIdx
 * \param uiDepth
 * \param uiPUIdx
 * \returns Void
 */
Void TDecSbac::parseMergeFlag ( TComDataCU* pcCU, UInt uiAbsPartIdx, UInt uiDepth, UInt uiPUIdx )
{
  UInt uiSymbol;
  m_pcTDecBinIf->decodeBin( uiSymbol, *m_cCUMergeFlagExtSCModel.get( 0 ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__MERGE_FLAG) );
  pcCU->setMergeFlagSubParts( uiSymbol ? true : false, uiAbsPartIdx, uiPUIdx, uiDepth );

  DTRACE_CABAC_VL( g_nSymbolCounter++ );
  DTRACE_CABAC_T( "\tMergeFlag: " );
  DTRACE_CABAC_V( uiSymbol );
  DTRACE_CABAC_T( "\tAddress: " );
  DTRACE_CABAC_V( pcCU->getCtuRsAddr() );
  DTRACE_CABAC_T( "\tuiAbsPartIdx: " );
  DTRACE_CABAC_V( uiAbsPartIdx );
  DTRACE_CABAC_T( "\n" );
}

Void TDecSbac::parseMergeIndex ( TComDataCU* pcCU, UInt& ruiMergeIndex )
{
  UInt uiUnaryIdx = 0;
  UInt uiNumCand = pcCU->getSlice()->getMaxNumMergeCand();
  if ( uiNumCand > 1 )
  {
    for( ; uiUnaryIdx < uiNumCand - 1; ++uiUnaryIdx )
    {
      UInt uiSymbol = 0;
      if ( uiUnaryIdx==0 )
      {
        m_pcTDecBinIf->decodeBin( uiSymbol, m_cCUMergeIdxExtSCModel.get( 0, 0, 0 ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__MERGE_INDEX) );
      }
      else
      {
        m_pcTDecBinIf->decodeBinEP( uiSymbol RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__MERGE_INDEX) );
      }
      if( uiSymbol == 0 )
      {
        break;
      }
    }
  }
  ruiMergeIndex = uiUnaryIdx;

  DTRACE_CABAC_VL( g_nSymbolCounter++ )
  DTRACE_CABAC_T( "\tparseMergeIndex()" )
  DTRACE_CABAC_T( "\tuiMRGIdx= " )
  DTRACE_CABAC_V( ruiMergeIndex )
  DTRACE_CABAC_T( "\n" )
}

Void TDecSbac::parseMVPIdx      ( Int& riMVPIdx )
{
  UInt uiSymbol;
  xReadUnaryMaxSymbol(uiSymbol, m_cMVPIdxSCModel.get(0), 1, AMVP_MAX_NUM_CANDS-1 RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__MVP_IDX) );
  riMVPIdx = uiSymbol;
}

Void TDecSbac::parseSplitFlag     ( TComDataCU* pcCU, UInt uiAbsPartIdx, UInt uiDepth )
{
  if( uiDepth == pcCU->getSlice()->getSPS()->getLog2DiffMaxMinCodingBlockSize() )
  {
    pcCU->setDepthSubParts( uiDepth, uiAbsPartIdx );
    return;
  }
#if RExt__DECODER_DEBUG_BIT_STATISTICS
  const TComCodingStatisticsClassType ctype(STATS__CABAC_BITS__SPLIT_FLAG, g_aucConvertToBit[pcCU->getSlice()->getSPS()->getMaxCUWidth()>>uiDepth]+2);
#endif

  UInt uiSymbol;
  m_pcTDecBinIf->decodeBin( uiSymbol, m_cCUSplitFlagSCModel.get( 0, 0, pcCU->getCtxSplitFlag( uiAbsPartIdx, uiDepth ) ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );
  DTRACE_CABAC_VL( g_nSymbolCounter++ )
  DTRACE_CABAC_T( "\tSplitFlag\n" )
  pcCU->setDepthSubParts( uiDepth + uiSymbol, uiAbsPartIdx );

  return;
}

/** parse partition size ����������С
 * \param pcCU
 * \param uiAbsPartIdx
 * \param uiDepth
 * \returns Void
 */
Void TDecSbac::parsePartSize( TComDataCU* pcCU, UInt uiAbsPartIdx, UInt uiDepth )
{
  UInt uiSymbol, uiMode = 0;
  PartSize eMode;
  const UChar cuWidth =UChar(pcCU->getSlice()->getSPS()->getMaxCUWidth()>>uiDepth);
  const UChar cuHeight=UChar(pcCU->getSlice()->getSPS()->getMaxCUHeight()>>uiDepth);
  const Int log2DiffMaxMinCodingBlockSize = pcCU->getSlice()->getSPS()->getLog2DiffMaxMinCodingBlockSize();

#if RExt__DECODER_DEBUG_BIT_STATISTICS
  const TComCodingStatisticsClassType ctype(STATS__CABAC_BITS__PART_SIZE, g_aucConvertToBit[cuWidth]+2);
#endif

  assert ( pcCU->getSlice()->getSPS()->getLog2DiffMaxMinCodingBlockSize() == log2DiffMaxMinCodingBlockSize);
  if ( pcCU->isIntra( uiAbsPartIdx ) )
  {
    uiSymbol = 1;
    if( uiDepth == log2DiffMaxMinCodingBlockSize )
    {
      m_pcTDecBinIf->decodeBin( uiSymbol, m_cCUPartSizeSCModel.get( 0, 0, 0) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );
    }
    eMode = uiSymbol ? SIZE_2Nx2N : SIZE_NxN;
    UInt uiTrLevel = 0;
    UInt uiWidthInBit  = g_aucConvertToBit[pcCU->getWidth(uiAbsPartIdx)]+2;
    UInt uiTrSizeInBit = g_aucConvertToBit[pcCU->getSlice()->getSPS()->getMaxTrSize()]+2;
    uiTrLevel          = uiWidthInBit >= uiTrSizeInBit ? uiWidthInBit - uiTrSizeInBit : 0;
    if( eMode == SIZE_NxN )
    {
      pcCU->setTrIdxSubParts( 1+uiTrLevel, uiAbsPartIdx, uiDepth );
    }
    else
    {
      pcCU->setTrIdxSubParts( uiTrLevel, uiAbsPartIdx, uiDepth );
    }
  }
  else
  {
    UInt uiMaxNumBits = 2;

    if( uiDepth == log2DiffMaxMinCodingBlockSize && !( cuWidth == 8 && cuHeight == 8 ) )
    {
      uiMaxNumBits ++;
    }

    for ( UInt ui = 0; ui < uiMaxNumBits; ui++ )
    {
      m_pcTDecBinIf->decodeBin( uiSymbol, m_cCUPartSizeSCModel.get( 0, 0, ui) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );
      if ( uiSymbol )
      {
        break;
      }
      uiMode++;
    }
    eMode = (PartSize) uiMode;
    if ( pcCU->getSlice()->getSPS()->getUseAMP() && uiDepth < log2DiffMaxMinCodingBlockSize )
    {
      if (eMode == SIZE_2NxN)
      {
        m_pcTDecBinIf->decodeBin(uiSymbol, m_cCUPartSizeSCModel.get( 0, 0, 3 ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype));
        if (uiSymbol == 0)
        {
          m_pcTDecBinIf->decodeBinEP(uiSymbol RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );
          eMode = (uiSymbol == 0? SIZE_2NxnU : SIZE_2NxnD);
        }
      }
      else if (eMode == SIZE_Nx2N)
      {
        m_pcTDecBinIf->decodeBin(uiSymbol, m_cCUPartSizeSCModel.get( 0, 0, 3 ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );
        if (uiSymbol == 0)
        {
          m_pcTDecBinIf->decodeBinEP(uiSymbol RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );
          eMode = (uiSymbol == 0? SIZE_nLx2N : SIZE_nRx2N);
        }
      }
    }
  }
  pcCU->setPartSizeSubParts( eMode, uiAbsPartIdx, uiDepth );
  pcCU->setSizeSubParts( cuWidth, cuHeight, uiAbsPartIdx, uiDepth );
}


/** parse prediction mode ����Ԥ��ģʽ
 * \param pcCU
 * \param uiAbsPartIdx
 * \param uiDepth
 * \returns Void
 */
Void TDecSbac::parsePredMode( TComDataCU* pcCU, UInt uiAbsPartIdx, UInt uiDepth )
{
  if( pcCU->getSlice()->isIntra() )
  {
    pcCU->setPredModeSubParts( MODE_INTRA, uiAbsPartIdx, uiDepth );
    return;
  }

  UInt uiSymbol;
  Int  iPredMode = MODE_INTER;
  m_pcTDecBinIf->decodeBin( uiSymbol, m_cCUPredModeSCModel.get( 0, 0, 0 ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__PRED_MODE) );
  iPredMode += uiSymbol;
  pcCU->setPredModeSubParts( (PredMode)iPredMode, uiAbsPartIdx, uiDepth );
}


Void TDecSbac::parseIntraDirLumaAng  ( TComDataCU* pcCU, UInt absPartIdx, UInt depth )
{
  PartSize mode = pcCU->getPartitionSize( absPartIdx );
  UInt partNum = mode==SIZE_NxN?4:1;
  UInt partOffset = ( pcCU->getPic()->getNumPartitionsInCtu() >> ( pcCU->getDepth(absPartIdx) << 1 ) ) >> 2;
  UInt mpmPred[4],symbol;
  Int j,intraPredMode;
  if (mode==SIZE_NxN)
  {
    depth++;
  }
#if RExt__DECODER_DEBUG_BIT_STATISTICS
  const TComCodingStatisticsClassType ctype(STATS__CABAC_BITS__INTRA_DIR_ANG, g_aucConvertToBit[pcCU->getSlice()->getSPS()->getMaxCUWidth()>>depth]+2, CHANNEL_TYPE_LUMA);
#endif
  for (j=0;j<partNum;j++)
  {
    m_pcTDecBinIf->decodeBin( symbol, m_cCUIntraPredSCModel.get( 0, 0, 0) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );
    mpmPred[j] = symbol;
  }
  for (j=0;j<partNum;j++)
  {
    Int preds[NUM_MOST_PROBABLE_MODES] = {-1, -1, -1};
    pcCU->getIntraDirPredictor(absPartIdx+partOffset*j, preds, COMPONENT_Y);
    if (mpmPred[j])
    {
      m_pcTDecBinIf->decodeBinEP( symbol RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );
      if (symbol)
      {
        m_pcTDecBinIf->decodeBinEP( symbol RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );
        symbol++;
      }
      intraPredMode = preds[symbol];
    }
    else
    {
      m_pcTDecBinIf->decodeBinsEP( symbol, 5 RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );
      intraPredMode = symbol;

      //postponed sorting of MPMs (only in remaining branch) mpm���ӳ�����(����ʣ���֧��)
      if (preds[0] > preds[1])
      {
        std::swap(preds[0], preds[1]);
      }
      if (preds[0] > preds[2])
      {
        std::swap(preds[0], preds[2]);
      }
      if (preds[1] > preds[2])
      {
        std::swap(preds[1], preds[2]);
      }
      for ( UInt i = 0; i < NUM_MOST_PROBABLE_MODES; i++ )
      {
        intraPredMode += ( intraPredMode >= preds[i] );
      }
    }
    pcCU->setIntraDirSubParts(CHANNEL_TYPE_LUMA, (UChar)intraPredMode, absPartIdx+partOffset*j, depth );
  }
}


Void TDecSbac::parseIntraDirChroma( TComDataCU* pcCU, UInt uiAbsPartIdx, UInt uiDepth )
{
  UInt uiSymbol;
#if RExt__DECODER_DEBUG_BIT_STATISTICS
  const TComCodingStatisticsClassType ctype(STATS__CABAC_BITS__INTRA_DIR_ANG, g_aucConvertToBit[pcCU->getSlice()->getSPS()->getMaxCUWidth()>>uiDepth]+2, CHANNEL_TYPE_CHROMA);
#endif

  m_pcTDecBinIf->decodeBin( uiSymbol, m_cCUChromaPredSCModel.get( 0, 0, 0 ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );
  if( uiSymbol == 0 )
  {
    uiSymbol = DM_CHROMA_IDX;
  }
  else
  {
    UInt uiIPredMode;
    m_pcTDecBinIf->decodeBinsEP( uiIPredMode, 2 RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );
    UInt uiAllowedChromaDir[ NUM_CHROMA_MODE ];
    pcCU->getAllowedChromaDir( uiAbsPartIdx, uiAllowedChromaDir );
    uiSymbol = uiAllowedChromaDir[ uiIPredMode ];
  }

  pcCU->setIntraDirSubParts( CHANNEL_TYPE_CHROMA, uiSymbol, uiAbsPartIdx, uiDepth );
}


Void TDecSbac::parseInterDir( TComDataCU* pcCU, UInt& ruiInterDir, UInt uiAbsPartIdx )
{
  UInt uiSymbol;
  const UInt uiCtx = pcCU->getCtxInterDir( uiAbsPartIdx );
  ContextModel *pCtx = m_cCUInterDirSCModel.get( 0 );

  uiSymbol = 0;
  if (pcCU->getPartitionSize(uiAbsPartIdx) == SIZE_2Nx2N || pcCU->getHeight(uiAbsPartIdx) != 8 )
  {
    m_pcTDecBinIf->decodeBin( uiSymbol, *( pCtx + uiCtx ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__INTER_DIR) );
  }

  if( uiSymbol )
  {
    uiSymbol = 2;
  }
  else
  {
    m_pcTDecBinIf->decodeBin( uiSymbol, *( pCtx + 4 ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__INTER_DIR) );
    assert(uiSymbol == 0 || uiSymbol == 1);
  }

  uiSymbol++;
  ruiInterDir = uiSymbol;
  return;
}

Void TDecSbac::parseRefFrmIdx( TComDataCU* pcCU, Int& riRefFrmIdx, RefPicList eRefList )
{
  UInt uiSymbol;

  ContextModel *pCtx = m_cCURefPicSCModel.get( 0 );
  m_pcTDecBinIf->decodeBin( uiSymbol, *pCtx RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__REF_FRM_IDX) );

  if( uiSymbol )
  {
    UInt uiRefNum = pcCU->getSlice()->getNumRefIdx( eRefList ) - 2;
    pCtx++;
    UInt ui;
    for( ui = 0; ui < uiRefNum; ++ui )
    {
      if( ui == 0 )
      {
        m_pcTDecBinIf->decodeBin( uiSymbol, *pCtx RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__REF_FRM_IDX) );
      }
      else
      {
        m_pcTDecBinIf->decodeBinEP( uiSymbol RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__REF_FRM_IDX) );
      }
      if( uiSymbol == 0 )
      {
        break;
      }
    }
    uiSymbol = ui + 1;
  }
  riRefFrmIdx = uiSymbol;

  return;
}

Void TDecSbac::parseMvd( TComDataCU* pcCU, UInt uiAbsPartIdx, UInt uiPartIdx, UInt uiDepth, RefPicList eRefList )
{
  UInt uiSymbol;
  UInt uiHorAbs;
  UInt uiVerAbs;
  UInt uiHorSign = 0;
  UInt uiVerSign = 0;
  ContextModel *pCtx = m_cCUMvdSCModel.get( 0 );

  if(pcCU->getSlice()->getMvdL1ZeroFlag() && eRefList == REF_PIC_LIST_1 && pcCU->getInterDir(uiAbsPartIdx)==3)
  {
    uiHorAbs=0;
    uiVerAbs=0;
  }
  else
  {
    m_pcTDecBinIf->decodeBin( uiHorAbs, *pCtx RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__MVD) );
    m_pcTDecBinIf->decodeBin( uiVerAbs, *pCtx RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__MVD) );

    const Bool bHorAbsGr0 = uiHorAbs != 0;
    const Bool bVerAbsGr0 = uiVerAbs != 0;
    pCtx++;

    if( bHorAbsGr0 )
    {
      m_pcTDecBinIf->decodeBin( uiSymbol, *pCtx RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__MVD) );
      uiHorAbs += uiSymbol;
    }

    if( bVerAbsGr0 )
    {
      m_pcTDecBinIf->decodeBin( uiSymbol, *pCtx RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__MVD) );
      uiVerAbs += uiSymbol;
    }

    if( bHorAbsGr0 )
    {
      if( 2 == uiHorAbs )
      {
        xReadEpExGolomb( uiSymbol, 1 RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__MVD_EP) );
        uiHorAbs += uiSymbol;
      }

      m_pcTDecBinIf->decodeBinEP( uiHorSign RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__MVD_EP) );
    }

    if( bVerAbsGr0 )
    {
      if( 2 == uiVerAbs )
      {
        xReadEpExGolomb( uiSymbol, 1 RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__MVD_EP) );
        uiVerAbs += uiSymbol;
      }

      m_pcTDecBinIf->decodeBinEP( uiVerSign RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__MVD_EP) );
    }

  }

  const TComMv cMv( uiHorSign ? -Int( uiHorAbs ): uiHorAbs, uiVerSign ? -Int( uiVerAbs ) : uiVerAbs );
  pcCU->getCUMvField( eRefList )->setAllMvd( cMv, pcCU->getPartitionSize( uiAbsPartIdx ), uiAbsPartIdx, uiDepth, uiPartIdx );
  return;
}

Void TDecSbac::parseCrossComponentPrediction( TComTU &rTu, ComponentID compID )
{
  TComDataCU *pcCU = rTu.getCU();

  if( isLuma(compID) || !pcCU->getSlice()->getPPS()->getPpsRangeExtension().getCrossComponentPredictionEnabledFlag() )
  {
    return;
  }

  const UInt uiAbsPartIdx = rTu.GetAbsPartIdxTU();

  if (!pcCU->isIntra(uiAbsPartIdx) || (pcCU->getIntraDir( CHANNEL_TYPE_CHROMA, uiAbsPartIdx ) == DM_CHROMA_IDX))
  {
    SChar alpha = 0;
    UInt symbol = 0;

    DTRACE_CABAC_VL( g_nSymbolCounter++ )
    DTRACE_CABAC_T("\tparseCrossComponentPrediction()")
    DTRACE_CABAC_T( "\tAddr=" )
    DTRACE_CABAC_V( compID )
    DTRACE_CABAC_T( "\tuiAbsPartIdx=" )
    DTRACE_CABAC_V( uiAbsPartIdx )
#if RExt__DECODER_DEBUG_BIT_STATISTICS
    TComCodingStatisticsClassType ctype(STATS__CABAC_BITS__CROSS_COMPONENT_PREDICTION, (g_aucConvertToBit[rTu.getRect(compID).width] + 2), compID);
#endif
    ContextModel *pCtx = m_cCrossComponentPredictionSCModel.get(0, 0) + ((compID == COMPONENT_Cr) ? (NUM_CROSS_COMPONENT_PREDICTION_CTX >> 1) : 0);
    m_pcTDecBinIf->decodeBin( symbol, pCtx[0] RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );

    if(symbol != 0)
    {
      // Cross-component prediction alpha is non-zero. �������Ԥ��alpha���㡣
      UInt sign = 0;
      m_pcTDecBinIf->decodeBin( symbol, pCtx[1] RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );

      if (symbol != 0)
      {
        // alpha is 2 (symbol=1), 4(symbol=2) or 8(symbol=3).
        // Read up to two more bits �ٶ���������
        xReadUnaryMaxSymbol( symbol, (pCtx + 2), 1, 2 RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );
        symbol += 1;
      }
      m_pcTDecBinIf->decodeBin( sign, pCtx[4] RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );

      alpha = (sign != 0) ? -(1 << symbol) : (1 << symbol);
    }
    DTRACE_CABAC_T( "\tAlpha=" )
    DTRACE_CABAC_V( alpha )
    DTRACE_CABAC_T( "\n" )

    pcCU->setCrossComponentPredictionAlphaPartRange( alpha, compID, uiAbsPartIdx, rTu.GetAbsPartIdxNumParts( compID ) );
  }
}

Void TDecSbac::parseTransformSubdivFlag( UInt& ruiSubdivFlag, UInt uiLog2TransformBlockSize )
{
  m_pcTDecBinIf->decodeBin( ruiSubdivFlag, m_cCUTransSubdivFlagSCModel.get( 0, 0, uiLog2TransformBlockSize )
      RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(TComCodingStatisticsClassType(STATS__CABAC_BITS__TRANSFORM_SUBDIV_FLAG, 5-uiLog2TransformBlockSize))
                          );
  DTRACE_CABAC_VL( g_nSymbolCounter++ )
  DTRACE_CABAC_T( "\tparseTransformSubdivFlag()" )
  DTRACE_CABAC_T( "\tsymbol=" )
  DTRACE_CABAC_V( ruiSubdivFlag )
  DTRACE_CABAC_T( "\tctx=" )
  DTRACE_CABAC_V( uiLog2TransformBlockSize )
  DTRACE_CABAC_T( "\n" )
}

Void TDecSbac::parseQtRootCbf( UInt uiAbsPartIdx, UInt& uiQtRootCbf )
{
  UInt uiSymbol;
  const UInt uiCtx = 0;
  m_pcTDecBinIf->decodeBin( uiSymbol , m_cCUQtRootCbfSCModel.get( 0, 0, uiCtx ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__QT_ROOT_CBF) );
  DTRACE_CABAC_VL( g_nSymbolCounter++ )
  DTRACE_CABAC_T( "\tparseQtRootCbf()" )
  DTRACE_CABAC_T( "\tsymbol=" )
  DTRACE_CABAC_V( uiSymbol )
  DTRACE_CABAC_T( "\tctx=" )
  DTRACE_CABAC_V( uiCtx )
  DTRACE_CABAC_T( "\tuiAbsPartIdx=" )
  DTRACE_CABAC_V( uiAbsPartIdx )
  DTRACE_CABAC_T( "\n" )

  uiQtRootCbf = uiSymbol;
}

Void TDecSbac::parseDeltaQP( TComDataCU* pcCU, UInt uiAbsPartIdx, UInt uiDepth )
{
  Int qp;
  UInt uiDQp;
  Int  iDQp;

  UInt uiSymbol;

  xReadUnaryMaxSymbol (uiDQp,  &m_cCUDeltaQpSCModel.get( 0, 0, 0 ), 1, CU_DQP_TU_CMAX RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__DELTA_QP_EP) );

  if( uiDQp >= CU_DQP_TU_CMAX)
  {
    xReadEpExGolomb( uiSymbol, CU_DQP_EG_k RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__DELTA_QP_EP));
    uiDQp+=uiSymbol;
  }

  if ( uiDQp > 0 )
  {
    UInt uiSign;
    Int qpBdOffsetY = pcCU->getSlice()->getSPS()->getQpBDOffset(CHANNEL_TYPE_LUMA);
    m_pcTDecBinIf->decodeBinEP(uiSign RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__DELTA_QP_EP));
    iDQp = uiDQp;
    if(uiSign)
    {
      iDQp = -iDQp;
    }
    qp = (((Int) pcCU->getRefQP( uiAbsPartIdx ) + iDQp + 52 + 2*qpBdOffsetY )%(52+qpBdOffsetY)) - qpBdOffsetY;
  }
  else
  {
    qp = pcCU->getRefQP(uiAbsPartIdx);
  }

  pcCU->setQPSubParts(qp, uiAbsPartIdx, uiDepth);
  pcCU->setCodedQP(qp);
}

/** parse chroma qp adjustment, converting to the internal table representation. ����ɫ��qp������ת��Ϊ�ڲ�����ʾ��
 * \returns Void
 */
Void TDecSbac::parseChromaQpAdjustment( TComDataCU* cu, UInt absPartIdx, UInt depth )
{
  UInt symbol;
#if RExt__DECODER_DEBUG_BIT_STATISTICS
  const TComCodingStatisticsClassType ctype(STATS__CABAC_BITS__CHROMA_QP_ADJUSTMENT, g_aucConvertToBit[cu->getSlice()->getSPS()->getMaxCUWidth()>>depth]+2, CHANNEL_TYPE_CHROMA);
#endif

  Int chromaQpOffsetListLen = cu->getSlice()->getPPS()->getPpsRangeExtension().getChromaQpOffsetListLen();

  // cu_chroma_qp_offset_flag
  m_pcTDecBinIf->decodeBin( symbol, m_ChromaQpAdjFlagSCModel.get( 0, 0, 0 ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );

  if (symbol && chromaQpOffsetListLen > 1)
  {
    // cu_chroma_qp_offset_idx
    xReadUnaryMaxSymbol( symbol,  &m_ChromaQpAdjIdcSCModel.get( 0, 0, 0 ), 0, chromaQpOffsetListLen - 1 RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );
    symbol++;
  }
  /* NB, symbol = 0 if outer flag is not set,
   *              1 if outer flag is set and there is no inner flag ����������ⲿ��־����û���ڲ���־
   *              1+ otherwise */
  cu->setChromaQpAdjSubParts( symbol, absPartIdx, depth );
  cu->setCodedChromaQpAdj(symbol);
}

Void TDecSbac::parseQtCbf( TComTU &rTu, const ComponentID compID, const Bool lowestLevel )
{
  TComDataCU* pcCU = rTu.getCU();

  const UInt absPartIdx       = rTu.GetAbsPartIdxTU(compID);
  const UInt TUDepth          = rTu.GetTransformDepthRel();
  const UInt uiCtx            = pcCU->getCtxQtCbf( rTu, toChannelType(compID) );
  const UInt contextSet       = toChannelType(compID);

  const UInt width            = rTu.getRect(compID).width;
  const UInt height           = rTu.getRect(compID).height;
  const Bool canQuadSplit     = (width >= (MIN_TU_SIZE * 2)) && (height >= (MIN_TU_SIZE * 2));
  const UInt coveredPartIdxes = rTu.GetAbsPartIdxNumParts(compID);

  //             Since the CBF for chroma is coded at the highest level possible, if sub-TUs are     ����ɫ�ȵ�CBF���ڿ��ܵ���߼����ϱ���ģ�
  //             to be coded for a 4x8 chroma TU, their CBFs must be coded at the highest 4x8 level ���ҪΪ4x8ɫ��TU������TU������CBF���������4x8�������(������TUΪ8x8������4x4)
  //             (i.e. where luma TUs are 8x8 rather than 4x4)
  //    ___ ___
  //   |   |   | <- 4 x (8x8 luma + 4x8 4:2:2 chroma)
  //   |___|___|    each quadrant has its own chroma CBF ÿ�����޶����Լ���ɫ��CBF
  //   |   |   | _ _ _ _
  //   |___|___|        |
  //   <--16--->        V
  //                   _ _
  //                  |_|_| <- 4 x 4x4 luma + 1 x 4x8 4:2:2 chroma
  //                  |_|_|    no chroma CBF is coded - instead the parent CBF is inherited û��ɫ��CBF����-���Ǽ̳и�CBF
  //                  <-8->    if sub-TUs are present, their CBFs had to be coded at the parent level ����ӵ�Ԫ���ڣ�������ڸ�������cbf���б���
   
  const UInt lowestTUDepth = TUDepth + ((!lowestLevel && !canQuadSplit) ? 1 : 0); //unsplittable TUs inherit their parent's CBF ���ɷָ��tu�̳��丸��CBF
        UInt lowestTUCBF   = 0;

  if ((width != height) && (lowestLevel || !canQuadSplit)) //if sub-TUs are present /������ӵ�λ
  {
    const UInt subTUDepth        = lowestTUDepth + 1;
    const UInt partIdxesPerSubTU = rTu.GetAbsPartIdxNumParts(compID) >> 1;

    UInt combinedSubTUCBF = 0;

    for (UInt subTU = 0; subTU < 2; subTU++)
    {
      UInt uiCbf = MAX_UINT;
      m_pcTDecBinIf->decodeBin(uiCbf, m_cCUQtCbfSCModel.get(0, contextSet, uiCtx) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(TComCodingStatisticsClassType(STATS__CABAC_BITS__QT_CBF, g_aucConvertToBit[rTu.getRect(compID).width]+2, compID)));

      const UInt subTUAbsPartIdx = absPartIdx + (subTU * partIdxesPerSubTU);
      pcCU->setCbfPartRange((uiCbf << subTUDepth), compID, subTUAbsPartIdx, partIdxesPerSubTU);
      combinedSubTUCBF |= uiCbf;

      DTRACE_CABAC_VL( g_nSymbolCounter++ )
      DTRACE_CABAC_T( "\tparseQtCbf()" )
      DTRACE_CABAC_T( "\tsub-TU=" )
      DTRACE_CABAC_V( subTU )
      DTRACE_CABAC_T( "\tsymbol=" )
      DTRACE_CABAC_V( uiCbf )
      DTRACE_CABAC_T( "\tctx=" )
      DTRACE_CABAC_V( uiCtx )
      DTRACE_CABAC_T( "\tetype=" )
      DTRACE_CABAC_V( compID )
      DTRACE_CABAC_T( "\tuiAbsPartIdx=" )
      DTRACE_CABAC_V( subTUAbsPartIdx )
      DTRACE_CABAC_T( "\n" )
    }

    //propagate the sub-TU CBF up to the lowest TU level //��subtu CBF���������TU����
    if (combinedSubTUCBF != 0)
    {
      pcCU->bitwiseOrCbfPartRange((combinedSubTUCBF << lowestTUDepth), compID, absPartIdx, coveredPartIdxes);
      lowestTUCBF = combinedSubTUCBF;
    }
  }
  else
  {
    UInt uiCbf = MAX_UINT;
    m_pcTDecBinIf->decodeBin(uiCbf, m_cCUQtCbfSCModel.get(0, contextSet, uiCtx) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(TComCodingStatisticsClassType(STATS__CABAC_BITS__QT_CBF, g_aucConvertToBit[rTu.getRect(compID).width]+2, compID)));

    pcCU->setCbfSubParts((uiCbf << lowestTUDepth), compID, absPartIdx, rTu.GetTransformDepthTotalAdj(compID));

    DTRACE_CABAC_VL( g_nSymbolCounter++ )
    DTRACE_CABAC_T( "\tparseQtCbf()" )
    DTRACE_CABAC_T( "\tsymbol=" )
    DTRACE_CABAC_V( uiCbf )
    DTRACE_CABAC_T( "\tctx=" )
    DTRACE_CABAC_V( uiCtx )
    DTRACE_CABAC_T( "\tetype=" )
    DTRACE_CABAC_V( compID )
    DTRACE_CABAC_T( "\tuiAbsPartIdx=" )
    DTRACE_CABAC_V( rTu.GetAbsPartIdxTU(compID) )
    DTRACE_CABAC_T( "\n" )

    lowestTUCBF = uiCbf;
  }

  //propagate the lowest level CBF up to the current level /����ͼ����CBF��������ǰ����
  if (lowestTUCBF != 0)
  {
    for (UInt depth = TUDepth; depth < lowestTUDepth; depth++)
    {
      pcCU->bitwiseOrCbfPartRange((lowestTUCBF << depth), compID, absPartIdx, coveredPartIdxes);
    }
  }
}


Void TDecSbac::parseTransformSkipFlags (TComTU &rTu, ComponentID component)
{
  TComDataCU* pcCU=rTu.getCU();
  UInt uiAbsPartIdx=rTu.GetAbsPartIdxTU(component);

  if (pcCU->getCUTransquantBypass(uiAbsPartIdx))
  {
    return;
  }

  if (!TUCompRectHasAssociatedTransformSkipFlag(rTu.getRect(component), pcCU->getSlice()->getPPS()->getPpsRangeExtension().getLog2MaxTransformSkipBlockSize()))
  {
    return;
  }

  UInt useTransformSkip;

  m_pcTDecBinIf->decodeBin( useTransformSkip , m_cTransformSkipSCModel.get( 0, toChannelType(component), 0 )
      RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(TComCodingStatisticsClassType(STATS__CABAC_BITS__TRANSFORM_SKIP_FLAGS, component))
                          );

  DTRACE_CABAC_VL( g_nSymbolCounter++ )
  DTRACE_CABAC_T("\tparseTransformSkip()");
  DTRACE_CABAC_T( "\tsymbol=" )
  DTRACE_CABAC_V( useTransformSkip )
  DTRACE_CABAC_T( "\tAddr=" )
  DTRACE_CABAC_V( pcCU->getCtuRsAddr() )
  DTRACE_CABAC_T( "\tetype=" )
  DTRACE_CABAC_V( component )
  DTRACE_CABAC_T( "\tuiAbsPartIdx=" )
  DTRACE_CABAC_V( rTu.GetAbsPartIdxTU() )
  DTRACE_CABAC_T( "\n" )

  pcCU->setTransformSkipPartRange( useTransformSkip, component, uiAbsPartIdx, rTu.GetAbsPartIdxNumParts(component));
}


/** Parse (X,Y) position of the last significant coefficient �������һ����Чϵ����(X,Y)λ��
 * \param uiPosLastX reference to X component of last coefficient �������һ��ϵ����X����
 * \param uiPosLastY reference to Y component of last coefficient �������һ��ϵ����Y����
 * \param width  Block width�����
 * \param height Block height ��߶�
 * \param component chroma compinent ID���ɫ�����ID
 * \param uiScanIdx scan type (zig-zag, hor, ver) uiScanIdxɨ������(z -zag, or, ver)
 *
 * This method decodes the X and Y component within a block of the last significant coefficient. �÷��������һ����Чϵ���Ŀ��ڽ���X��Y������
 */
Void TDecSbac::parseLastSignificantXY( UInt& uiPosLastX, UInt& uiPosLastY, Int width, Int height, ComponentID component, UInt uiScanIdx )
{
  UInt uiLast;

  ContextModel *pCtxX = m_cCuCtxLastX.get( 0, toChannelType(component) );
  ContextModel *pCtxY = m_cCuCtxLastY.get( 0, toChannelType(component) );

#if RExt__DECODER_DEBUG_BIT_STATISTICS
  TComCodingStatisticsClassType ctype(STATS__CABAC_BITS__LAST_SIG_X_Y, g_aucConvertToBit[width]+2, component);
#endif


  if ( uiScanIdx == SCAN_VER )
  {
    swap( width, height );
  }

  Int blkSizeOffsetX, blkSizeOffsetY, shiftX, shiftY;
  getLastSignificantContextParameters(component, width, height, blkSizeOffsetX, blkSizeOffsetY, shiftX, shiftY);

  //------------------

  // posX

  for( uiPosLastX = 0; uiPosLastX < g_uiGroupIdx[ width - 1 ]; uiPosLastX++ )
  {
    m_pcTDecBinIf->decodeBin( uiLast, *( pCtxX + blkSizeOffsetX + (uiPosLastX >>shiftX) ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );

    if( !uiLast )
    {
      break;
    }
  }

  // posY

  for( uiPosLastY = 0; uiPosLastY < g_uiGroupIdx[ height - 1 ]; uiPosLastY++ )
  {
    m_pcTDecBinIf->decodeBin( uiLast, *( pCtxY + blkSizeOffsetY + (uiPosLastY >>shiftY)) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );

    if( !uiLast )
    {
      break;
    }
  }

  // EP-coded part

  if ( uiPosLastX > 3 )
  {
    UInt uiTemp  = 0;
    UInt uiCount = ( uiPosLastX - 2 ) >> 1;
    for ( Int i = uiCount - 1; i >= 0; i-- )
    {
      m_pcTDecBinIf->decodeBinEP( uiLast RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );
      uiTemp += uiLast << i;
    }
    uiPosLastX = g_uiMinInGroup[ uiPosLastX ] + uiTemp;
  }
  if ( uiPosLastY > 3 )
  {
    UInt uiTemp  = 0;
    UInt uiCount = ( uiPosLastY - 2 ) >> 1;
    for ( Int i = uiCount - 1; i >= 0; i-- )
    {
      m_pcTDecBinIf->decodeBinEP( uiLast RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype) );
      uiTemp += uiLast << i;
    }
    uiPosLastY = g_uiMinInGroup[ uiPosLastY ] + uiTemp;
  }

  if( uiScanIdx == SCAN_VER )
  {
    swap( uiPosLastX, uiPosLastY );
  }
}

Void TDecSbac::parseCoeffNxN(  TComTU &rTu, ComponentID compID )
{
  TComDataCU* pcCU=rTu.getCU();
  const UInt uiAbsPartIdx=rTu.GetAbsPartIdxTU(compID);
  const TComRectangle &rRect=rTu.getRect(compID);
  const UInt uiWidth=rRect.width;
  const UInt uiHeight=rRect.height;
  TCoeff* pcCoef=(pcCU->getCoeff(compID)+rTu.getCoefficientOffset(compID));
  const TComSPS &sps=*(pcCU->getSlice()->getSPS());

  DTRACE_CABAC_VL( g_nSymbolCounter++ )
  DTRACE_CABAC_T( "\tparseCoeffNxN()\teType=" )
  DTRACE_CABAC_V( compID )
  DTRACE_CABAC_T( "\twidth=" )
  DTRACE_CABAC_V( uiWidth )
  DTRACE_CABAC_T( "\theight=" )
  DTRACE_CABAC_V( uiHeight )
  DTRACE_CABAC_T( "\tdepth=" )
//  DTRACE_CABAC_V( rTu.GetTransformDepthTotalAdj(compID) )
  DTRACE_CABAC_V( rTu.GetTransformDepthTotal() )
  DTRACE_CABAC_T( "\tabspartidx=" )
//  DTRACE_CABAC_V( uiAbsPartIdx )
  DTRACE_CABAC_V( rTu.GetAbsPartIdxTU(compID) )
  DTRACE_CABAC_T( "\ttoCU-X=" )
  DTRACE_CABAC_V( pcCU->getCUPelX() )
  DTRACE_CABAC_T( "\ttoCU-Y=" )
  DTRACE_CABAC_V( pcCU->getCUPelY() )
  DTRACE_CABAC_T( "\tCU-addr=" )
  DTRACE_CABAC_V(  pcCU->getCtuRsAddr() )
  DTRACE_CABAC_T( "\tinCU-X=" )
//  DTRACE_CABAC_V( g_auiRasterToPelX[ g_auiZscanToRaster[uiAbsPartIdx] ] )
  DTRACE_CABAC_V( g_auiRasterToPelX[ g_auiZscanToRaster[rTu.GetAbsPartIdxTU(compID)] ] )
  DTRACE_CABAC_T( "\tinCU-Y=" )
// DTRACE_CABAC_V( g_auiRasterToPelY[ g_auiZscanToRaster[uiAbsPartIdx] ] )
  DTRACE_CABAC_V( g_auiRasterToPelY[ g_auiZscanToRaster[rTu.GetAbsPartIdxTU(compID)] ] )
  DTRACE_CABAC_T( "\tpredmode=" )
  DTRACE_CABAC_V(  pcCU->getPredictionMode( uiAbsPartIdx ) )
  DTRACE_CABAC_T( "\n" )

  //--------------------------------------------------------------------------------------------------

  if( uiWidth > sps.getMaxTrSize() )
  {
    std::cerr << "ERROR: parseCoeffNxN was passed a TU with dimensions larger than the maximum allowed size" << std::endl;
    assert(false);
    exit(1);
  }

  //--------------------------------------------------------------------------------------------------

  //set parameters

  const ChannelType  chType            = toChannelType(compID);
  const UInt         uiLog2BlockWidth  = g_aucConvertToBit[ uiWidth  ] + 2;
  const UInt         uiLog2BlockHeight = g_aucConvertToBit[ uiHeight ] + 2;
  const UInt         uiMaxNumCoeff     = uiWidth * uiHeight;
  const UInt         uiMaxNumCoeffM1   = uiMaxNumCoeff - 1;

  const ChannelType  channelType       = toChannelType(compID);
  const Bool         extendedPrecision = sps.getSpsRangeExtension().getExtendedPrecisionProcessingFlag();

  const Bool         alignCABACBeforeBypass = sps.getSpsRangeExtension().getCabacBypassAlignmentEnabledFlag();
  const Int          maxLog2TrDynamicRange  = sps.getMaxLog2TrDynamicRange(channelType);

#if RExt__DECODER_DEBUG_BIT_STATISTICS
  TComCodingStatisticsClassType ctype_group(STATS__CABAC_BITS__SIG_COEFF_GROUP_FLAG, uiLog2BlockWidth, compID);
  TComCodingStatisticsClassType ctype_map(STATS__CABAC_BITS__SIG_COEFF_MAP_FLAG, uiLog2BlockWidth, compID);
  TComCodingStatisticsClassType ctype_gt1(STATS__CABAC_BITS__GT1_FLAG, uiLog2BlockWidth, compID);
  TComCodingStatisticsClassType ctype_gt2(STATS__CABAC_BITS__GT2_FLAG, uiLog2BlockWidth, compID);
#endif

  Bool beValid;
  if (pcCU->getCUTransquantBypass(uiAbsPartIdx))
  {
    beValid = false;
    if((!pcCU->isIntra(uiAbsPartIdx)) && pcCU->isRDPCMEnabled(uiAbsPartIdx))
    {
      parseExplicitRdpcmMode(rTu, compID);
    }
  }
  else
  {
    beValid = pcCU->getSlice()->getPPS()->getSignDataHidingEnabledFlag();
  }

  UInt absSum = 0;

  //--------------------------------------------------------------------------------------------------

  if(pcCU->getSlice()->getPPS()->getUseTransformSkip())
  {
    parseTransformSkipFlags(rTu, compID);
    //  This TU has coefficients and is transform skipped. Check whether is inter coded and if yes decode the explicit RDPCM mode ���TU��ϵ�����任������������Ƿ񻥱��룬����ǣ�������ʽRDPCMģʽ
    if(pcCU->getTransformSkip(uiAbsPartIdx, compID) && (!pcCU->isIntra(uiAbsPartIdx)) && pcCU->isRDPCMEnabled(uiAbsPartIdx) )
    {
      parseExplicitRdpcmMode(rTu, compID);
      if(pcCU->getExplicitRdpcmMode(compID, uiAbsPartIdx) != RDPCM_OFF)
      {
        //  Sign data hiding is avoided for horizontal and vertical RDPCM modes ˮƽ�ʹ�ֱRDPCMģʽ�����˷�����������
        beValid = false;
      }
    }
  }

  Int uiIntraMode = -1;
  const Bool       bIsLuma = isLuma(compID);
  Int isIntra = pcCU->isIntra(uiAbsPartIdx) ? 1 : 0;
  if ( isIntra && pcCU->isRDPCMEnabled(uiAbsPartIdx) )
  {
    const UInt partsPerMinCU = 1<<(2*(sps.getMaxTotalCUDepth() - sps.getLog2DiffMaxMinCodingBlockSize()));
    uiIntraMode = pcCU->getIntraDir( toChannelType(compID), uiAbsPartIdx );
    uiIntraMode = (uiIntraMode==DM_CHROMA_IDX && !bIsLuma) ? pcCU->getIntraDir(CHANNEL_TYPE_LUMA, getChromasCorrespondingPULumaIdx(uiAbsPartIdx, rTu.GetChromaFormat(), partsPerMinCU)) : uiIntraMode;
    uiIntraMode = ((rTu.GetChromaFormat() == CHROMA_422) && !bIsLuma) ? g_chroma422IntraAngleMappingTable[uiIntraMode] : uiIntraMode;

    Bool transformSkip = pcCU->getTransformSkip( uiAbsPartIdx,compID);
    Bool rdpcm_lossy = ( transformSkip /*&& isIntra*/ && ( (uiIntraMode == HOR_IDX) || (uiIntraMode == VER_IDX) ) );
    if ( rdpcm_lossy )
    {
      beValid = false;
    }
  }

  //--------------------------------------------------------------------------------------------------

  const Bool  bUseGolombRiceParameterAdaptation = sps.getSpsRangeExtension().getPersistentRiceAdaptationEnabledFlag();
        UInt &currentGolombRiceStatistic        = m_golombRiceAdaptationStatistics[rTu.getGolombRiceStatisticsIndex(compID)];

  //select scans ѡ��ɨ��
  TUEntropyCodingParameters codingParameters;
  getTUEntropyCodingParameters(codingParameters, rTu, compID);

  //===== decode last significant  ���������Ч=====
  UInt uiPosLastX, uiPosLastY;
  parseLastSignificantXY( uiPosLastX, uiPosLastY, uiWidth, uiHeight, compID, codingParameters.scanType );
  UInt uiBlkPosLast      = uiPosLastX + (uiPosLastY<<uiLog2BlockWidth);
  pcCoef[ uiBlkPosLast ] = 1;

  //===== decode significance flags ���������־ =====
  UInt uiScanPosLast;
  for( uiScanPosLast = 0; uiScanPosLast < uiMaxNumCoeffM1; uiScanPosLast++ )
  {
    UInt uiBlkPos = codingParameters.scan[ uiScanPosLast ];
    if( uiBlkPosLast == uiBlkPos )
    {
      break;
    }
  }

  ContextModel * const baseCoeffGroupCtx = m_cCUSigCoeffGroupSCModel.get( 0, isChroma(chType) );
  ContextModel * const baseCtx = m_cCUSigSCModel.get( 0, 0 ) + getSignificanceMapContextOffset(compID);

  const Int  iLastScanSet  = uiScanPosLast >> MLS_CG_SIZE;
  UInt c1                  = 1;
  UInt uiGoRiceParam       = 0;


  UInt uiSigCoeffGroupFlag[ MLS_GRP_NUM ];
  memset( uiSigCoeffGroupFlag, 0, sizeof(UInt) * MLS_GRP_NUM );

  Int  iScanPosSig             = (Int) uiScanPosLast;
  for( Int iSubSet = iLastScanSet; iSubSet >= 0; iSubSet-- )
  {
    Int  iSubPos   = iSubSet << MLS_CG_SIZE;
    uiGoRiceParam  = currentGolombRiceStatistic / RExt__GOLOMB_RICE_INCREMENT_DIVISOR;
    Bool updateGolombRiceStatistics = bUseGolombRiceParameterAdaptation; //leave the statistics at 0 when not using the adaptation system ��ʹ������ϵͳʱ����ͳ����Ϣ����Ϊ0
    Int numNonZero = 0;

    Int lastNZPosInCG  = -1;
    Int firstNZPosInCG = 1 << MLS_CG_SIZE;

    Bool escapeDataPresentInGroup = false;

    Int pos[1 << MLS_CG_SIZE];

    if( iScanPosSig == (Int) uiScanPosLast )
    {
      lastNZPosInCG  = iScanPosSig;
      firstNZPosInCG = iScanPosSig;
      iScanPosSig--;
      pos[ numNonZero ] = uiBlkPosLast;
      numNonZero = 1;
    }

    // decode significant_coeffgroup_flag ����significant_coeffgroup_flag
    Int iCGBlkPos = codingParameters.scanCG[ iSubSet ];
    Int iCGPosY   = iCGBlkPos / codingParameters.widthInGroups;
    Int iCGPosX   = iCGBlkPos - (iCGPosY * codingParameters.widthInGroups);

    if( iSubSet == iLastScanSet || iSubSet == 0)
    {
      uiSigCoeffGroupFlag[ iCGBlkPos ] = 1;
    }
    else
    {
      UInt uiSigCoeffGroup;
      UInt uiCtxSig  = TComTrQuant::getSigCoeffGroupCtxInc( uiSigCoeffGroupFlag, iCGPosX, iCGPosY, codingParameters.widthInGroups, codingParameters.heightInGroups );
      m_pcTDecBinIf->decodeBin( uiSigCoeffGroup, baseCoeffGroupCtx[ uiCtxSig ] RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype_group) );
      uiSigCoeffGroupFlag[ iCGBlkPos ] = uiSigCoeffGroup;
    }

    // decode significant_coeff_flag ����significant_coeff_flag
    const Int patternSigCtx = TComTrQuant::calcPatternSigCtx(uiSigCoeffGroupFlag, iCGPosX, iCGPosY, codingParameters.widthInGroups, codingParameters.heightInGroups);

    UInt uiBlkPos, uiSig, uiCtxSig;
    for( ; iScanPosSig >= iSubPos; iScanPosSig-- )
    {
      uiBlkPos  = codingParameters.scan[ iScanPosSig ];
      uiSig     = 0;

      if( uiSigCoeffGroupFlag[ iCGBlkPos ] )
      {
        if( iScanPosSig > iSubPos || iSubSet == 0  || numNonZero )
        {
          uiCtxSig  = TComTrQuant::getSigCtxInc( patternSigCtx, codingParameters, iScanPosSig, uiLog2BlockWidth, uiLog2BlockHeight, chType );
          m_pcTDecBinIf->decodeBin( uiSig, baseCtx[ uiCtxSig ] RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype_map) );
        }
        else
        {
          uiSig = 1;
        }
      }
      pcCoef[ uiBlkPos ] = uiSig;
      if( uiSig )
      {
        pos[ numNonZero ] = uiBlkPos;
        numNonZero ++;
        if( lastNZPosInCG == -1 )
        {
          lastNZPosInCG = iScanPosSig;
        }
        firstNZPosInCG = iScanPosSig;
      }
    }

    if( numNonZero > 0 )
    {
      Bool signHidden = ( lastNZPosInCG - firstNZPosInCG >= SBH_THRESHOLD );

      absSum = 0;

      const UInt uiCtxSet = getContextSetIndex(compID, iSubSet, (c1 == 0));
      c1 = 1;
      UInt uiBin;

      ContextModel *baseCtxMod = m_cCUOneSCModel.get( 0, 0 ) + (NUM_ONE_FLAG_CTX_PER_SET * uiCtxSet);

      Int absCoeff[1 << MLS_CG_SIZE];

      for ( Int i = 0; i < numNonZero; i++)
      {
        absCoeff[i] = 1;
      }
      Int numC1Flag = min(numNonZero, C1FLAG_NUMBER);
      Int firstC2FlagIdx = -1;

      for( Int idx = 0; idx < numC1Flag; idx++ )
      {
        m_pcTDecBinIf->decodeBin( uiBin, baseCtxMod[c1] RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype_gt1) );
        if( uiBin == 1 )
        {
          c1 = 0;
          if (firstC2FlagIdx == -1)
          {
            firstC2FlagIdx = idx;
          }
          else //if a greater-than-one has been encountered already this group ���һ������һ���Ѿ������������
          {
            escapeDataPresentInGroup = true;
          }
        }
        else if( (c1 < 3) && (c1 > 0) )
        {
          c1++;
        }
        absCoeff[ idx ] = uiBin + 1;
      }

      if (c1 == 0)
      {
        baseCtxMod = m_cCUAbsSCModel.get( 0, 0 ) + (NUM_ABS_FLAG_CTX_PER_SET * uiCtxSet);
        if ( firstC2FlagIdx != -1)
        {
          m_pcTDecBinIf->decodeBin( uiBin, baseCtxMod[0] RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype_gt2) );
          absCoeff[ firstC2FlagIdx ] = uiBin + 2;
          if (uiBin != 0)
          {
            escapeDataPresentInGroup = true;
          }
        }
      }

      escapeDataPresentInGroup = escapeDataPresentInGroup || (numNonZero > C1FLAG_NUMBER);

      const Bool alignGroup = escapeDataPresentInGroup && alignCABACBeforeBypass;

#if RExt__DECODER_DEBUG_BIT_STATISTICS
      TComCodingStatisticsClassType ctype_signs((alignGroup ? STATS__CABAC_BITS__ALIGNED_SIGN_BIT    : STATS__CABAC_BITS__SIGN_BIT   ), uiLog2BlockWidth, compID);
      TComCodingStatisticsClassType ctype_escs ((alignGroup ? STATS__CABAC_BITS__ALIGNED_ESCAPE_BITS : STATS__CABAC_BITS__ESCAPE_BITS), uiLog2BlockWidth, compID);
#endif

      if (alignGroup)
      {
        m_pcTDecBinIf->align();
      }

      UInt coeffSigns;
      if ( signHidden && beValid )
      {
        m_pcTDecBinIf->decodeBinsEP( coeffSigns, numNonZero-1 RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype_signs) );
        coeffSigns <<= 32 - (numNonZero-1);
      }
      else
      {
        m_pcTDecBinIf->decodeBinsEP( coeffSigns, numNonZero RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype_signs) );
        coeffSigns <<= 32 - numNonZero;
      }

      Int iFirstCoeff2 = 1;
      if (escapeDataPresentInGroup)
      {
        for( Int idx = 0; idx < numNonZero; idx++ )
        {
          UInt baseLevel  = (idx < C1FLAG_NUMBER)? (2 + iFirstCoeff2) : 1;

          if( absCoeff[ idx ] == baseLevel)
          {
            UInt uiLevel;
            xReadCoefRemainExGolomb( uiLevel, uiGoRiceParam, extendedPrecision, maxLog2TrDynamicRange RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype_escs) );

            absCoeff[ idx ] = uiLevel + baseLevel;

            if (absCoeff[idx] > (3 << uiGoRiceParam))
            {
              uiGoRiceParam = bUseGolombRiceParameterAdaptation ? (uiGoRiceParam + 1) : (std::min<UInt>((uiGoRiceParam + 1), 4));
            }

            if (updateGolombRiceStatistics)
            {
              const UInt initialGolombRiceParameter = currentGolombRiceStatistic / RExt__GOLOMB_RICE_INCREMENT_DIVISOR;

              if (uiLevel >= (3 << initialGolombRiceParameter))
              {
                currentGolombRiceStatistic++;
              }
              else if (((uiLevel * 2) < (1 << initialGolombRiceParameter)) && (currentGolombRiceStatistic > 0))
              {
                currentGolombRiceStatistic--;
              }

              updateGolombRiceStatistics = false;
            }
          }

          if(absCoeff[ idx ] >= 2)
          {
            iFirstCoeff2 = 0;
          }
        }
      }

      for( Int idx = 0; idx < numNonZero; idx++ )
      {
        Int blkPos = pos[ idx ];
        // Signs applied later. ����Ӧ�����Ժ�
        pcCoef[ blkPos ] = absCoeff[ idx ];
        absSum += absCoeff[ idx ];

        if ( idx == numNonZero-1 && signHidden && beValid )
        {
          // Infer sign of 1st element. �ƶϵ�һ��Ԫ�صķ��š�
          if (absSum&0x1)
          {
            pcCoef[ blkPos ] = -pcCoef[ blkPos ];
          }
        }
        else
        {
          Int sign = static_cast<Int>( coeffSigns ) >> 31;
          pcCoef[ blkPos ] = ( pcCoef[ blkPos ] ^ sign ) - sign;
          coeffSigns <<= 1;
        }
      }
    }
  }

#if ENVIRONMENT_VARIABLE_DEBUG_AND_TEST
  printSBACCoeffData(uiPosLastX, uiPosLastY, uiWidth, uiHeight, compID, uiAbsPartIdx, codingParameters.scanType, pcCoef);
#endif

  return;
}

Void TDecSbac::parseSaoMaxUvlc ( UInt& val, UInt maxSymbol )
{
  if (maxSymbol == 0)
  {
    val = 0;
    return;
  }

  UInt code;
  Int  i;
  m_pcTDecBinIf->decodeBinEP( code RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__SAO) );
  if ( code == 0 )
  {
    val = 0;
    return;
  }

  i=1;
  while (1)
  {
    m_pcTDecBinIf->decodeBinEP( code RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__SAO) );
    if ( code == 0 )
    {
      break;
    }
    i++;
    if (i == maxSymbol)
    {
      break;
    }
  }

  val = i;
}

Void TDecSbac::parseSaoUflc (UInt uiLength, UInt&  riVal)
{
  m_pcTDecBinIf->decodeBinsEP ( riVal, uiLength RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__SAO) );
}

Void TDecSbac::parseSaoMerge (UInt&  ruiVal)
{
  UInt uiCode;
  m_pcTDecBinIf->decodeBin( uiCode, m_cSaoMergeSCModel.get( 0, 0, 0 ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__SAO) );
  ruiVal = (Int)uiCode;
}

Void TDecSbac::parseSaoTypeIdx (UInt&  ruiVal)
{
  UInt uiCode;
  m_pcTDecBinIf->decodeBin( uiCode, m_cSaoTypeIdxSCModel.get( 0, 0, 0 ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__SAO) );
  if (uiCode == 0)
  {
    ruiVal = 0;
  }
  else
  {
    m_pcTDecBinIf->decodeBinEP( uiCode RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__SAO) );
    if (uiCode == 0)
    {
      ruiVal = 1;
    }
    else
    {
      ruiVal = 2;
    }
  }
}

Void TDecSbac::parseSaoSign(UInt& val)
{
  m_pcTDecBinIf->decodeBinEP ( val RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__SAO) );
}

Void TDecSbac::parseSAOBlkParam (SAOBlkParam& saoBlkParam
                                , Bool* sliceEnabled
                                , Bool leftMergeAvail
                                , Bool aboveMergeAvail
                                , const BitDepths &bitDepths
                                )
{
  UInt uiSymbol;

  Bool isLeftMerge = false;
  Bool isAboveMerge= false;

  if(leftMergeAvail)
  {
    parseSaoMerge(uiSymbol); //sao_merge_left_flag
    isLeftMerge = (uiSymbol?true:false);
  }

  if( aboveMergeAvail && !isLeftMerge)
  {
    parseSaoMerge(uiSymbol); //sao_merge_up_flag
    isAboveMerge = (uiSymbol?true:false);
  }

  if(isLeftMerge || isAboveMerge) //merge mode
  {
    for (UInt componentIndex = 0; componentIndex < MAX_NUM_COMPONENT; componentIndex++)
    {
      saoBlkParam[componentIndex].modeIdc = (sliceEnabled[componentIndex]) ? SAO_MODE_MERGE : SAO_MODE_OFF;
      saoBlkParam[componentIndex].typeIdc = (isLeftMerge)?SAO_MERGE_LEFT:SAO_MERGE_ABOVE;
    }
  }
  else //new or off mode
  {
    for(Int compId=COMPONENT_Y; compId < MAX_NUM_COMPONENT; compId++)
    {
      const ComponentID compIdx=ComponentID(compId);
      const ComponentID firstCompOfChType = getFirstComponentOfChannel(toChannelType(compIdx));
      SAOOffset& ctbParam = saoBlkParam[compIdx];
#if O0043_BEST_EFFORT_DECODING
      const Int bitDepthOrig = bitDepths.stream[toChannelType(compIdx)];
      const Int forceBitDepthAdjust = bitDepthOrig - bitDepths.recon[toChannelType(compIdx)];
#else
      const Int bitDepthOrig = bitDepths.recon[toChannelType(compIdx)];
#endif
      const Int maxOffsetQVal=TComSampleAdaptiveOffset::getMaxOffsetQVal(bitDepthOrig);
      if(!sliceEnabled[compIdx])
      {
        //off
        ctbParam.modeIdc = SAO_MODE_OFF;
        continue;
      }

      //type
      if(compIdx == firstCompOfChType)
      {
        parseSaoTypeIdx(uiSymbol); //sao_type_idx_luma or sao_type_idx_chroma

        assert(uiSymbol ==0 || uiSymbol ==1 || uiSymbol ==2);

        if(uiSymbol ==0) //OFF
        {
          ctbParam.modeIdc = SAO_MODE_OFF;
        }
        else if(uiSymbol == 1) //BO
        {
          ctbParam.modeIdc = SAO_MODE_NEW;
          ctbParam.typeIdc = SAO_TYPE_START_BO;
        }
        else //2, EO
        {
          ctbParam.modeIdc = SAO_MODE_NEW;
          ctbParam.typeIdc = SAO_TYPE_START_EO;
        }

      }
      else //Cr, follow Cb SAO type
      {
        ctbParam.modeIdc = saoBlkParam[COMPONENT_Cb].modeIdc;
        ctbParam.typeIdc = saoBlkParam[COMPONENT_Cb].typeIdc;
      }

      if(ctbParam.modeIdc == SAO_MODE_NEW)
      {
        Int offset[4];
        for(Int i=0; i< 4; i++)
        {
          parseSaoMaxUvlc(uiSymbol, maxOffsetQVal ); //sao_offset_abs
          offset[i] = (Int)uiSymbol;
        }

        if(ctbParam.typeIdc == SAO_TYPE_START_BO)
        {
          for(Int i=0; i< 4; i++)
          {
            if(offset[i] != 0)
            {
              parseSaoSign(uiSymbol); //sao_offset_sign
              if(uiSymbol)
              {
#if O0043_BEST_EFFORT_DECODING
                offset[i] >>= forceBitDepthAdjust;
#endif
                offset[i] = -offset[i];
              }
            }
          }
          parseSaoUflc(NUM_SAO_BO_CLASSES_LOG2, uiSymbol ); //sao_band_position
          ctbParam.typeAuxInfo = uiSymbol;

          for(Int i=0; i<4; i++)
          {
            ctbParam.offset[(ctbParam.typeAuxInfo+i)%MAX_NUM_SAO_CLASSES] = offset[i];
          }

        }
        else //EO
        {
          ctbParam.typeAuxInfo = 0;

          if(firstCompOfChType == compIdx)
          {
            parseSaoUflc(NUM_SAO_EO_TYPES_LOG2, uiSymbol ); //sao_eo_class_luma or sao_eo_class_chroma
            ctbParam.typeIdc += uiSymbol;
          }
          else
          {
            ctbParam.typeIdc = saoBlkParam[firstCompOfChType].typeIdc;
          }
          ctbParam.offset[SAO_CLASS_EO_FULL_VALLEY] = offset[0];
          ctbParam.offset[SAO_CLASS_EO_HALF_VALLEY] = offset[1];
          ctbParam.offset[SAO_CLASS_EO_PLAIN      ] = 0;
          ctbParam.offset[SAO_CLASS_EO_HALF_PEAK  ] = -offset[2];
          ctbParam.offset[SAO_CLASS_EO_FULL_PEAK  ] = -offset[3];
        }
      }
    }
  }
}

/**
 - Initialize our contexts from the nominated source. ��ָ����Դ��ʼ�������ġ�
 .
 \param pSrc Contexts to be copied. Ҫ���Ƶ������ġ�
 */
Void TDecSbac::xCopyContextsFrom( const TDecSbac* pSrc )
{
  memcpy(m_contextModels, pSrc->m_contextModels, m_numContextModels*sizeof(m_contextModels[0]));
  memcpy(m_golombRiceAdaptationStatistics, pSrc->m_golombRiceAdaptationStatistics, (sizeof(UInt) * RExt__GOLOMB_RICE_ADAPTATION_STATISTICS_SETS));
}

Void TDecSbac::xCopyFrom( const TDecSbac* pSrc )
{
  m_pcTDecBinIf->copyState( pSrc->m_pcTDecBinIf );
  xCopyContextsFrom( pSrc );
}

Void TDecSbac::load ( const TDecSbac* pSrc )
{
  xCopyFrom(pSrc);
}

Void TDecSbac::loadContexts ( const TDecSbac* pSrc )
{
  xCopyContextsFrom(pSrc);
}

/** Performs CABAC decoding of the explicit RDPCM mode
 * \param rTu current TU data structure
 * \param compID component identifier
 */
Void TDecSbac::parseExplicitRdpcmMode( TComTU &rTu, ComponentID compID )
{
  TComDataCU* cu = rTu.getCU();
  const UInt absPartIdx=rTu.GetAbsPartIdxTU(compID);
  const TComRectangle &rect = rTu.getRect(compID);
  const UInt tuHeight = g_aucConvertToBit[rect.height];
  const UInt tuWidth  = g_aucConvertToBit[rect.width];
  UInt code = 0;

  assert(tuHeight == tuWidth);

#if RExt__DECODER_DEBUG_BIT_STATISTICS
  const TComCodingStatisticsClassType ctype(STATS__EXPLICIT_RDPCM_BITS, g_aucConvertToBit[cu->getSlice()->getSPS()->getMaxCUWidth()>>rTu.GetTransformDepthTotal()]+2);
#endif

  m_pcTDecBinIf->decodeBin(code, m_explicitRdpcmFlagSCModel.get (0, toChannelType(compID), 0) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype));

  if(code == 0)
  {
    cu->setExplicitRdpcmModePartRange( RDPCM_OFF, compID, absPartIdx, rTu.GetAbsPartIdxNumParts(compID));
  }
  else
  {
    m_pcTDecBinIf->decodeBin(code, m_explicitRdpcmDirSCModel.get (0, toChannelType(compID), 0) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(ctype));
    if(code == 0)
    {
      cu->setExplicitRdpcmModePartRange( RDPCM_HOR, compID, absPartIdx, rTu.GetAbsPartIdxNumParts(compID));
    }
    else
    {
      cu->setExplicitRdpcmModePartRange( RDPCM_VER, compID, absPartIdx, rTu.GetAbsPartIdxNumParts(compID));
    }
  }
}

Void TDecSbac::parseColourTransformFlag( Bool & flag )
{
  UInt symbol;
  const UInt ctx = 0;
  m_pcTDecBinIf->decodeBin( symbol , m_cCUColourTransformFlagSCModel.get( 0, 0, ctx ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__QT_ROOT_CBF) );
  flag = (symbol > 0 ? true: false);
}

#if RExt__DECODER_DEBUG_BIT_STATISTICS
Void TDecSbac::xReadTruncBinCode(UInt& symbol, UInt maxSymbol, const class TComCodingStatisticsClassType &whichStat)
#else
Void TDecSbac::xReadTruncBinCode(UInt& symbol, UInt maxSymbol)
#endif
{
  UInt thresh;
  if (maxSymbol > 256)
  {
    UInt threshVal = 1 << 8;
    thresh = 8;
    while (threshVal <= maxSymbol)
    {
      thresh++;
      threshVal <<= 1;
    }
    thresh--;
  }
  else
  {
    thresh = g_uhPaletteTBC[maxSymbol];
  }

  UInt val = 1 << thresh;
  UInt b = maxSymbol - val;
  m_pcTDecBinIf->decodeBinsEP(symbol, thresh RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(whichStat));
  if (symbol >= val - b)
  {
    UInt s;
    m_pcTDecBinIf->decodeBinEP(s RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(whichStat));
    symbol <<= 1;
    symbol += s;
    symbol -= (val - b);
  }
}

#if RExt__DECODER_DEBUG_BIT_STATISTICS
Void TDecSbac::xAdjustPaletteIndex(UInt siCurLevel, UInt idx, Pel *pLevel, Int iMaxSymbol,
  const class TComCodingStatisticsClassType &whichStat, UChar *pSPoint, Int iWidth,
  UChar *pEscapeFlag)
#else
Void TDecSbac::xAdjustPaletteIndex(UInt siCurLevel, UInt idx, Pel *pLevel, Int iMaxSymbol, UChar *pSPoint, Int iWidth,
  UChar *pEscapeFlag)
#endif
{
  UInt symbol;
  Int iRefLevel = MAX_INT;
  UInt traIdx = m_pScanOrder[idx];

  if (idx)
  {
    UInt traIdxLeft = m_pScanOrder[idx - 1];
    if (pSPoint[traIdxLeft] == PALETTE_RUN_LEFT)
    {
      iRefLevel = pLevel[traIdxLeft];
      if (pEscapeFlag[traIdxLeft])
      {
        iRefLevel = iMaxSymbol - 1;
      }
    }
    else
    {
      assert(traIdxLeft >= iWidth);
      iRefLevel = pLevel[traIdx - iWidth];
      if (pEscapeFlag[traIdx - iWidth])
      {
        iRefLevel = iMaxSymbol - 1;
      }
    }
    iMaxSymbol--;
  }

  symbol = siCurLevel;
  if (siCurLevel >= iRefLevel)
  {
    symbol++;
  }
  pLevel[traIdx] = symbol;
}

#if RExt__DECODER_DEBUG_BIT_STATISTICS
Pel TDecSbac::xReadPaletteIndex(UInt idx, Pel *pLevel, Int iMaxSymbol, const class TComCodingStatisticsClassType &whichStat, UChar *pSPoint, Int iWidth, UChar *pEscapeFlag)
#else
Pel TDecSbac::xReadPaletteIndex(UInt idx, Pel *pLevel, Int iMaxSymbol, UChar *pSPoint, Int iWidth, UChar *pEscapeFlag)
#endif
{
  UInt symbol;
  Int iRefLevel = MAX_INT;
  UInt traIdx = m_pScanOrder[idx];

  if (idx)
  {
    UInt traIdxLeft = m_pScanOrder[idx - 1];
    if (pSPoint[traIdxLeft] == PALETTE_RUN_LEFT)
    {
      iRefLevel = pLevel[traIdxLeft];
      if(pEscapeFlag[traIdxLeft])
      {
        iRefLevel = iMaxSymbol - 1;
      }
    }
    else
    {
      assert(traIdxLeft >= iWidth);
      iRefLevel = pLevel[traIdx - iWidth];
      if(pEscapeFlag[traIdx - iWidth])
      {
        iRefLevel = iMaxSymbol - 1;
      }
    }
    iMaxSymbol--;
  }
  if (iMaxSymbol > 1)
  {
    xReadTruncBinCode(symbol, iMaxSymbol RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__PALETTE_MODE));
  }
  else
  {
    symbol = 0;
  }
  Pel siCurLevel = symbol;
  if (symbol >= iRefLevel)
  {
    symbol++;
  }
  pLevel[traIdx] = symbol;
  return siCurLevel;
}

#if RExt__DECODER_DEBUG_BIT_STATISTICS
Void  TDecSbac::xDecodeRun(UInt & symbol, Bool bCopyTopMode, const UInt paletteIdx, const UInt maxRun, const class TComCodingStatisticsClassType &whichStat)
#else
Void  TDecSbac::xDecodeRun(UInt & symbol, Bool bCopyTopMode, const UInt paletteIdx, const UInt maxRun)
#endif
{
  ContextModel *pcModel;
  UChar *ucCtxLut;
  if ( bCopyTopMode )
  {
    pcModel = m_cCopyTopRunSCModel.get(0);
    ucCtxLut = m_runTopLut;
  }
  else
  {
    pcModel = m_cRunSCModel.get(0);
    ucCtxLut = m_runLeftLut;
    m_runLeftLut[0] = (paletteIdx < SCM__S0269_PALETTE_RUN_MSB_IDX_CTX_T1 ? 0: (paletteIdx < SCM__S0269_PALETTE_RUN_MSB_IDX_CTX_T2 ? 1 : 2));
  }
  symbol = xReadTruncMsbP1RefinementBits( pcModel, maxRun, SCM__S0269_PALETTE_RUN_MSB_IDX_CABAC_BYPASS_THRE, ucCtxLut RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(whichStat) );
}

#if RExt__DECODER_DEBUG_BIT_STATISTICS
Void TDecSbac::xDecodePalettePredIndicator(Bool *bReusedPrev, UInt paletteSizePrev, UInt maxPaletteSize, const class TComCodingStatisticsClassType &whichStat)
#else
Void TDecSbac::xDecodePalettePredIndicator(Bool *bReusedPrev, UInt paletteSizePrev, UInt maxPaletteSize)
#endif
{
  UInt symbol, numPalettePredicted = 0, idx = 0;

  xReadEpExGolomb( symbol, 0 RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__PALETTE_MODE) );

  if( symbol != 1 )
  {
    while (idx < paletteSizePrev && numPalettePredicted < maxPaletteSize)
    {
      if( idx > 0 )
      {
        xReadEpExGolomb( symbol, 0 RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__PALETTE_MODE) );
      }
      if( symbol == 1 )
      {
        break;
      }

      if( symbol )
      {
        idx += symbol - 1;
      }

      bReusedPrev[idx] = true;

      numPalettePredicted++;
      idx++;
    }
  }
}

Void TDecSbac::parsePaletteModeFlag( TComDataCU* pcCU, UInt absPartIdx, UInt depth )
{
  UInt symbol;
  m_pcTDecBinIf->decodeBin( symbol, m_paletteModeFlagSCModel.get( 0, 0, 0 ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__PALETTE_MODE) );
  pcCU->setPaletteModeFlagSubParts(symbol ? true : false, absPartIdx, depth);
  if (pcCU->getPaletteModeFlag(absPartIdx) )
  {
    const TComSPS &sps=*(pcCU->getSlice()->getSPS());
    pcCU->setSizeSubParts( sps.getMaxCUWidth()>>depth, sps.getMaxCUHeight()>>depth, absPartIdx, depth );
    pcCU->setPartSizeSubParts(SIZE_2Nx2N, absPartIdx, depth );
    pcCU->setPredModeSubParts(MODE_INTRA, absPartIdx, depth );
  }
}

Void TDecSbac::parsePaletteModeSyntax(TComDataCU *pcCU, UInt absPartIdx, UInt depth, UInt numComp, Bool& bCodeDQP, Bool& codeChromaQpAdj)
{
  UInt width, height, total;
  UInt idx, dictMaxSize, dictIdxBits;
  UInt sampleBits[3] = { 0 };
  Pel  *pLevel = 0, *pPalette[3] = { 0 };
  ComponentID compBegin = COMPONENT_Y;
  const UInt minCoeffSizeY = pcCU->getPic()->getMinCUWidth() * pcCU->getPic()->getMinCUHeight();
  const UInt offsetY = minCoeffSizeY * absPartIdx;
  const UInt offset = offsetY >> (pcCU->getPic()->getComponentScaleX(compBegin) + pcCU->getPic()->getComponentScaleY(compBegin));
  UInt scaleX = pcCU->getPic()->getComponentScaleX(COMPONENT_Cb);
  UInt scaleY = pcCU->getPic()->getComponentScaleY(COMPONENT_Cb);
  const UInt offsetC = offsetY >> (scaleX + scaleY);

  UInt symbol = 0;
  Pel *pPixelValue[3] = { 0 };

  for (Int comp = compBegin; comp < compBegin + numComp; comp++)
  {
    sampleBits[comp] = pcCU->getSlice()->getSPS()->getBitDepth(toChannelType(ComponentID(comp)));
  }

  width = pcCU->getWidth(absPartIdx) >> (pcCU->getPic()->getComponentScaleX(compBegin));
  height = pcCU->getHeight(absPartIdx) >> (pcCU->getPic()->getComponentScaleY(compBegin));
  dictMaxSize = 0;
  total = width * height;
  pLevel = pcCU->getLevel(compBegin) + offset;
  UChar *pSPoint = pcCU->getSPoint(compBegin) + offset;
  UChar *pEscapeFlag = pcCU->getEscapeFlag(compBegin) + offset;
  UInt run = 0;
  UInt stride = width;

  UInt paletteSizePrev;
  Pel *pPalettePrev[3];
  Bool isLossless = pcCU->getCUTransquantBypass( absPartIdx );

  for (UInt comp = compBegin; comp < compBegin + numComp; comp++)
  {
    ComponentID compID = (ComponentID)comp;
    pPalettePrev[comp] = pcCU->getPalettePred(pcCU, comp, paletteSizePrev);

    if ( comp == compBegin )
    {
      pPixelValue[comp] = pcCU->getLevel( compID ) + offset;
    }
    else
    {
      pPixelValue[comp] = pcCU->getLevel(compID) + offsetC;
    }
    pPalette[comp] = pcCU->getPalette(comp, absPartIdx);
  }

  Bool isScanTraverseMode = true;

  {
    Bool *bReusedPrev = pcCU->getPrevPaletteReusedFlag( compBegin, absPartIdx );
    UInt numPaletteRceived = 0, numPalettePredicted = 0;
    memset( bReusedPrev, 0, sizeof( Bool ) * paletteSizePrev );
    if ( paletteSizePrev )
    {
      xDecodePalettePredIndicator(bReusedPrev, paletteSizePrev, pcCU->getSlice()->getSPS()->getSpsScreenExtension().getPaletteMaxSize() RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__PALETTE_MODE));

      for ( UInt comp = 0; comp < MAX_NUM_COMPONENT; comp++ )
      {
        for ( UInt idxPrev = 0; idxPrev < pcCU->getSlice()->getSPS()->getSpsScreenExtension().getPaletteMaxPredSize(); idxPrev++ )
        {
          pcCU->setPrevPaletteReusedFlagSubParts( comp, bReusedPrev[idxPrev], idxPrev, absPartIdx, depth );
        }
      }
      for ( Int idxPrev = 0; idxPrev < paletteSizePrev; idxPrev++ )
      {
        if ( bReusedPrev[idxPrev] )
        {
          for ( UInt comp = compBegin; comp < compBegin + numComp; comp++ )
          {
            pPalette[comp][numPalettePredicted] = pPalettePrev[comp][idxPrev];
            pcCU->setPaletteSubParts( comp, pPalette[comp][numPalettePredicted], numPalettePredicted, absPartIdx, depth );
          }
          numPalettePredicted++;
        }
      }
    }

    if ( numPalettePredicted < pcCU->getSlice()->getSPS()->getSpsScreenExtension().getPaletteMaxSize())
    {
      xReadEpExGolomb(numPaletteRceived, 0 RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__PALETTE_MODE));
    }

    dictMaxSize = numPaletteRceived + numPalettePredicted;
    for ( UInt comp = compBegin; comp < compBegin + numComp; comp++ )
    {
      for ( UInt paletteIdx = numPalettePredicted; paletteIdx < dictMaxSize; paletteIdx++ )
      {
        m_pcTDecBinIf->decodeBinsEP( symbol, sampleBits[comp] RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG( STATS__CABAC_BITS__PALETTE_MODE ) );
        pPalette[comp][paletteIdx] = symbol;
        pcCU->setPaletteSubParts( comp, pPalette[comp][paletteIdx], paletteIdx, absPartIdx, depth );
      }
    }
  }

  for ( UInt comp = compBegin; comp < compBegin + numComp; comp++ )
  {
    pcCU->setPaletteSizeSubParts(comp, dictMaxSize, absPartIdx, depth);
  }
  dictIdxBits = 0;
  while ((1 << dictIdxBits) < dictMaxSize)
  {
    dictIdxBits++;
  }
  UInt indexMaxSize = dictMaxSize;
  Bool paletteEscapeValPresentFlag = false;
  UInt signalEscape = 1;
  if (dictMaxSize > 0)
  {
    UInt uiCode = 0;
    m_pcTDecBinIf->decodeBinEP(uiCode RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__PALETTE_MODE));
    paletteEscapeValPresentFlag = (uiCode!=0);
    if (paletteEscapeValPresentFlag)
    {
      signalEscape = 1;
    }
    else
    {
      signalEscape = 0;
    }
  }

  UInt uiDictIdxBitsExteneded = dictIdxBits;
  if ( signalEscape )
  {
    while ( (1 << uiDictIdxBitsExteneded) <= dictMaxSize )
    {
      uiDictIdxBitsExteneded++;
    }
    indexMaxSize++;
  }
  assert(dictMaxSize <= pcCU->getSlice()->getSPS()->getSpsScreenExtension().getPaletteMaxSize());

  m_pScanOrder = g_scanOrder[SCAN_UNGROUPED][(isScanTraverseMode)?SCAN_TRAV:SCAN_HOR][g_aucConvertToBit[width]+2][g_aucConvertToBit[height]+2];
  Int iNumCopyIndexRuns = -1;
  UInt lastRunType = 0;
  UInt numIndices = 0;
  UInt adjust = 0;
  std::list<Int> lParsedIdxList;
  if (indexMaxSize > 1)
  {
    UInt currParam = 3 + ((indexMaxSize) >> 3);
    xReadCoefRemainExGolomb(numIndices, currParam, false, MAX_NUM_CHANNEL_TYPE RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__PALETTE_MODE));
    numIndices++;
    iNumCopyIndexRuns = numIndices;
    while (numIndices--)
    {
      xReadTruncBinCode(symbol, indexMaxSize - adjust RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__PALETTE_MODE));
      adjust = 1;
      lParsedIdxList.push_back(symbol);
    }
    m_pcTDecBinIf->decodeBin(lastRunType, m_SPointSCModel.get(0, 0, 0) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__PALETTE_MODE));
    parseScanRotationModeFlag(pcCU, absPartIdx, depth);
    adjust = 0;
  }
  else
  {
    pcCU->setPaletteScanRotationModeFlagSubParts(false, absPartIdx, depth);
  }

  if ( signalEscape )
  {
    if( pcCU->getSlice()->getPPS()->getUseDQP() && bCodeDQP )
    {
      parseDeltaQP( pcCU, absPartIdx, pcCU->getDepth( absPartIdx ) );
      bCodeDQP = false;
    }

    if( pcCU->getSlice()->getUseChromaQpAdj() && !pcCU->getCUTransquantBypass( absPartIdx ) && codeChromaQpAdj )
    {
      parseChromaQpAdjustment( pcCU, absPartIdx, pcCU->getDepth( absPartIdx ) );
      codeChromaQpAdj = false;
    }
  }

  idx = 0;
  while (idx < total)
  {
    UInt traIdx = m_pScanOrder[idx];
    if (indexMaxSize > 1)
    {
      if (traIdx >= width && pSPoint[m_pScanOrder[idx - 1]] != PALETTE_RUN_ABOVE)
      {
        if (iNumCopyIndexRuns && idx < total - 1)
        {
          m_pcTDecBinIf->decodeBin(symbol, m_SPointSCModel.get(0, 0, 0)
            RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__PALETTE_MODE));
        }
        else
        {
          if (idx == total - 1 && iNumCopyIndexRuns)
          {
            symbol = 0;
          }
          else
          {
            symbol = 1;
          }
        }
      }
      else
      {
        symbol = 0;
      }
    }
    else
    {
      symbol = 0;
    }
    pSPoint[traIdx] = symbol;
    Pel siCurLevel = 0;
    if (!symbol)
    {
      if (!lParsedIdxList.empty())
      {
        siCurLevel = lParsedIdxList.front();
        lParsedIdxList.pop_front();
      }
      else
      {
        siCurLevel = 0;
      }
      xAdjustPaletteIndex(siCurLevel, idx, pLevel, indexMaxSize
        RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__PALETTE_MODE), pSPoint, width,
        pEscapeFlag);
    }
    UInt preDecodeLevel = pLevel[traIdx];
    Bool isEscapePixel = (!symbol && (preDecodeLevel == dictMaxSize)) ? true : false;
    pEscapeFlag[traIdx] = (isEscapePixel)? 1: 0;
    {
      UInt pos = 0;
      if (indexMaxSize > 1)
      {
        iNumCopyIndexRuns -= (pSPoint[traIdx] == PALETTE_RUN_LEFT);
        Bool bLastRun = iNumCopyIndexRuns == 0 && pSPoint[traIdx] == lastRunType;
        if (!bLastRun)
        {
          xDecodeRun(run, pSPoint[traIdx], siCurLevel, (total - iNumCopyIndexRuns - idx - 1 - lastRunType) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__PALETTE_MODE));
        }
        else
        {
          run = total - idx - 1;
        }
      }
      else
      {
        run = total - idx - 1;
      }

      if (pSPoint[traIdx] == PALETTE_RUN_LEFT)
      {
        symbol = preDecodeLevel;
        pLevel[traIdx] = symbol;
        UChar sEscapeColor = (pLevel[traIdx] == dictMaxSize);
        assert(pEscapeFlag[traIdx] == sEscapeColor);
        pos = 0;
        while (pos < run)
        {
          pos++;
          idx++;
          traIdx = m_pScanOrder[idx];
          pLevel [traIdx] = symbol;
          pSPoint[traIdx] = PALETTE_RUN_LEFT;
          pEscapeFlag[traIdx] = sEscapeColor;
        }
        idx++;
      }
      else  //pSPoint[traIdx] == PALETTE_RUN_ABOVE
      {
        pLevel[traIdx] = pLevel[traIdx - stride];
        pEscapeFlag[traIdx] = pEscapeFlag[traIdx - stride];
        pos = 0;
        while (pos < run)
        {
          pos++;
          idx++;
          traIdx = m_pScanOrder[idx];
          pLevel [traIdx] =  pLevel [traIdx - stride];
          pSPoint[traIdx] = PALETTE_RUN_ABOVE;
          pEscapeFlag[traIdx] = pEscapeFlag[traIdx - stride];
        }
        idx++;
      }
    }
  }
  assert (idx == total);
  for (UInt comp = compBegin; comp < compBegin + numComp; comp++)
  {
    for( idx = 0; idx < total; idx++ )
    {
      UInt traIdx = m_pScanOrder[idx];
      if( pEscapeFlag[traIdx] )
      {
        UInt y, x;
        y = traIdx/width;
        x = traIdx%width;
        UInt xC, yC, traIdxC;
        if(!pcCU->getPaletteScanRotationModeFlag(absPartIdx))
        {
          xC = (x>>scaleX);
          yC = (y>>scaleY);
          traIdxC = yC * (width>>scaleX) + xC;
        }
        else
        {
          xC = (x>>scaleY);
          yC = (y>>scaleX);
          traIdxC = yC * (height>>scaleY) + xC;
        }

        if(comp == compBegin)
        {
          if ( isLossless )
          {
            m_pcTDecBinIf->decodeBinsEP( symbol, pcCU->getSlice()->getSPS()->getBitDepth( comp > 0 ? CHANNEL_TYPE_CHROMA : CHANNEL_TYPE_LUMA ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG( STATS__CABAC_BITS__PALETTE_MODE ) );
          }
          else
          {
            xReadEpExGolomb( symbol, 3 RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG( STATS__CABAC_BITS__PALETTE_MODE ) );
            assert( symbol < ( 1 << ( pcCU->getSlice()->getSPS()->getBitDepth( comp > 0 ? CHANNEL_TYPE_CHROMA : CHANNEL_TYPE_LUMA ) + 1 ) ) );
          }
          pPixelValue[comp][traIdx] = symbol;
        }
        else
        {
          if(   pcCU->getPic()->getChromaFormat() == CHROMA_444 ||
            ( pcCU->getPic()->getChromaFormat() == CHROMA_420 && ((x&1) == 0) && ((y&1) == 0)) ||
            ( pcCU->getPic()->getChromaFormat() == CHROMA_422 && ((!pcCU->getPaletteScanRotationModeFlag(absPartIdx) && ((x&1) == 0)) || (pcCU->getPaletteScanRotationModeFlag(absPartIdx) && ((y&1) == 0))) )
            )
          {
            if ( isLossless )
            {
              m_pcTDecBinIf->decodeBinsEP( symbol, pcCU->getSlice()->getSPS()->getBitDepth( comp > 0 ? CHANNEL_TYPE_CHROMA : CHANNEL_TYPE_LUMA ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG( STATS__CABAC_BITS__PALETTE_MODE ) );
            }
            else
            {
              xReadEpExGolomb( symbol, 3 RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG( STATS__CABAC_BITS__PALETTE_MODE ) );
              assert( symbol < ( 1 << ( pcCU->getSlice()->getSPS()->getBitDepth( comp > 0 ? CHANNEL_TYPE_CHROMA : CHANNEL_TYPE_LUMA ) + 1 ) ) );
            }
            pPixelValue[comp][traIdxC] = symbol;
          }
        }
      }
    }
  }
}

Void TDecSbac::parseScanRotationModeFlag( TComDataCU* pcCU, UInt absPartIdx, UInt depth )
{
  UInt symbol;
  UInt ctx = 0;
  m_pcTDecBinIf->decodeBin( symbol, m_paletteScanRotationModeFlagSCModel.get( 0, 0, ctx ) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__PALETTE_MODE) );
  pcCU->setPaletteScanRotationModeFlagSubParts(symbol ? true : false, absPartIdx, depth);
}

#if RExt__DECODER_DEBUG_BIT_STATISTICS
UInt TDecSbac::xReadTruncUnarySymbol( ContextModel* pcSCModel, UInt uiMax, UInt uiCtxT, UChar *ucCtxLut, const class TComCodingStatisticsClassType &whichStat)
#else
UInt TDecSbac::xReadTruncUnarySymbol( ContextModel* pcSCModel, UInt uiMax, UInt uiCtxT, UChar *ucCtxLut)
#endif
{
  if (uiMax == 0)
  {
    return 0;
  }
  UInt uiBin, uiIdx = 0;
  do
  {
    if (uiIdx > uiCtxT)
    {
      m_pcTDecBinIf->decodeBinEP(uiBin RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(whichStat));
    }
    else
    {
      m_pcTDecBinIf->decodeBin(uiBin, uiIdx <= uiCtxT ? pcSCModel[ucCtxLut[uiIdx]] : pcSCModel[ucCtxLut[uiCtxT]] RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(whichStat));
    }
    uiIdx++;
  } while ( uiBin && uiIdx < uiMax );

  return ( uiBin && uiIdx == uiMax ) ? uiMax : uiIdx-1;
}

#if RExt__DECODER_DEBUG_BIT_STATISTICS
UInt TDecSbac::xReadTruncMsbP1RefinementBits( ContextModel* pcSCModel, UInt maxVal, UInt ctxT, UChar *ucCtxLut, const class TComCodingStatisticsClassType &whichStat)
#else
UInt TDecSbac::xReadTruncMsbP1RefinementBits( ContextModel* pcSCModel, UInt maxVal, UInt ctxT, UChar *ucCtxLut)
#endif
{
  if (maxVal==0)
  {
    return 0;
  }
  UInt symbol;
  UInt msbP1 = xReadTruncUnarySymbol( pcSCModel, g_getMsbP1Idx(maxVal), ctxT, ucCtxLut RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(whichStat));
  if ( msbP1 > 1)
  {
    UInt numBins = g_getMsbP1Idx(maxVal);
    if ( msbP1 < numBins)
    {
      UInt bits = msbP1-1;
      m_pcTDecBinIf->decodeBinsEP( symbol, bits RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(whichStat));
      symbol |= (1 << bits);
    }
    else
    {
      UInt curValue = 1 << (numBins-1);
      xReadTruncBinCode(symbol, (maxVal+1-curValue) RExt__DECODER_DEBUG_BIT_STATISTICS_PASS_OPT_ARG(STATS__CABAC_BITS__PALETTE_MODE));
      symbol += curValue;
    }
  }
  else
  {
    symbol = msbP1;
  }

  return symbol;
}

//! \}