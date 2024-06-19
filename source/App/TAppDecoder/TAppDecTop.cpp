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

 /** \file     TAppDecTop.cpp
     \brief    Decoder application class
 */

#include <list>
#include <vector>
#include <stdio.h>
#include <fcntl.h>
#include <assert.h>

#include "TAppDecTop.h"
#include "TLibDecoder/AnnexBread.h"
#include "TLibDecoder/NALread.h"
#if RExt__DECODER_DEBUG_BIT_STATISTICS
#include "TLibCommon/TComCodingStatistics.h"
#endif

 //! \ingroup TAppDecoder
 //! \{

 // ====================================================================================================================
 // Constructor / destructor / initialization / destroy 构造函数/析构函数/初始化/销毁
 // ====================================================================================================================

TAppDecTop::TAppDecTop()
    : m_iPOCLastDisplay(-MAX_INT)
    , m_pcSeiColourRemappingInfoPrevious(NULL)
{
}

Void TAppDecTop::create()
{
}

Void TAppDecTop::destroy()
{
    m_bitstreamFileName.clear();
    m_reconFileName.clear();
}

// ====================================================================================================================
// Public member functions 公共成员函数
// ====================================================================================================================

/**
 - create internal class 创建内部类
 - initialize internal class 初始化内部类
 - until the end of the bitstream, call decoding function in TDecTop class 直到比特流结束，在TDecTop类中调用解码函数
 - delete allocated buffers 删除已分配的缓冲区
 - destroy internal class 销毁内部类
 .
 */
Void TAppDecTop::decode()
{
    Int                 poc;
    TComList<TComPic*>* pcListPic = NULL;

    ifstream bitstreamFile(m_bitstreamFileName.c_str(), ifstream::in | ifstream::binary);
    if (!bitstreamFile)
    {
        fprintf(stderr, "\nfailed to open bitstream file `%s' for reading\n", m_bitstreamFileName.c_str());
        exit(EXIT_FAILURE);
    }

    InputByteStream bytestream(bitstreamFile);

    if (!m_outputDecodedSEIMessagesFilename.empty() && m_outputDecodedSEIMessagesFilename != "-")
    {
        m_seiMessageFileStream.open(m_outputDecodedSEIMessagesFilename.c_str(), std::ios::out);
        if (!m_seiMessageFileStream.is_open() || !m_seiMessageFileStream.good())
        {
            fprintf(stderr, "\nUnable to open file `%s' for writing decoded SEI messages\n", m_outputDecodedSEIMessagesFilename.c_str());
            exit(EXIT_FAILURE);
        }
    }

    // create & initialize internal classes 创建和初始化内部类
    xCreateDecLib();
    xInitDecLib();
    m_iPOCLastDisplay += m_iSkipFrame;      // set the last displayed POC correctly for skip forward. 正确设置最后显示的POC以便跳转。

    // clear contents of colour-remap-information-SEI output file 清除color - map-information- sei输出文件的内容
    if (!m_colourRemapSEIFileName.empty())
    {
        std::ofstream ofile(m_colourRemapSEIFileName.c_str());
        if (!ofile.good() || !ofile.is_open())
        {
            fprintf(stderr, "\nUnable to open file '%s' for writing colour-remap-information-SEI video\n", m_colourRemapSEIFileName.c_str());
            exit(EXIT_FAILURE);
        }
    }

    // main decoder loop 主解码回路
    Bool openedReconFile = false; // reconstruction file not yet opened. (must be performed after SPS is seen) 重建文件尚未打开。(必须在看到SPS后执行
    Bool loopFiltered = false;

    while (!!bitstreamFile)
    {
        /* location serves to work around a design fault in the decoder, whereby
         * the process of reading a new slice that is the first slice of a new frame
         * requires the TDecTop::decode() method to be called again with the same
         * nal unit. */
         //location用于解决解码器中的一个设计错误，即读取新帧的第一个新片的过程需要使用相同的nal单元再次调用TDecTop::decode()方法。
#if RExt__DECODER_DEBUG_BIT_STATISTICS
        TComCodingStatistics::TComCodingStatisticsData backupStats(TComCodingStatistics::GetStatistics());
        streampos location = bitstreamFile.tellg() - streampos(bytestream.GetNumBufferedBytes());
#else
        streampos location = bitstreamFile.tellg();
#endif
        AnnexBStats stats = AnnexBStats();

        InputNALUnit nalu;
        byteStreamNALUnit(bytestream, nalu.getBitstream().getFifo(), stats);

        // call actual decoding function 调用实际解码函数
        Bool bNewPicture = false;
        if (nalu.getBitstream().getFifo().empty())
        {
            /* this can happen if the following occur:如果发生以下情况报错
             *  - empty input file 空输入文件
             *  - two back-to-back start_code_prefixes -两个连续的start_code_prefixes
             *  - start_code_prefix immediately followed by EOF  start_code_prefix后跟EOF
             */
            fprintf(stderr, "Warning: Attempt to decode an empty NAL unit\n");
        }
        else
        {
            read(nalu);
            if ((m_iMaxTemporalLayer >= 0 && nalu.m_temporalId > m_iMaxTemporalLayer) || !isNaluWithinTargetDecLayerIdSet(&nalu))
            {
                bNewPicture = false;
            }
            else
            {
                bNewPicture = m_cTDecTop.decode(nalu, m_iSkipFrame, m_iPOCLastDisplay);
                if (bNewPicture)
                {
                    bitstreamFile.clear();
                    /* location points to the current nalunit payload[1] due to the
                     * need for the annexB parser to read three extra bytes.
                     * [1] except for the first NAL unit in the file
                     *     (but bNewPicture doesn't happen then) */
                     //location指向当前nalunit有效负载[1]，这是由于附件b解析器需要读取额外的三个字节。[1]除了文件中的第一个NAL单元(但bNewPicture不会发生)
#if RExt__DECODER_DEBUG_BIT_STATISTICS
                    bitstreamFile.seekg(location);
                    bytestream.reset();
                    TComCodingStatistics::SetStatistics(backupStats);
#else
                    bitstreamFile.seekg(location - streamoff(3));
                    bytestream.reset();
#endif
                }
            }
        }

        if ((bNewPicture || !bitstreamFile || nalu.m_nalUnitType == NAL_UNIT_EOS) &&
            !m_cTDecTop.getFirstSliceInSequence())
        {
            if (!loopFiltered || bitstreamFile)
            {
                m_cTDecTop.executeLoopFilters(poc, pcListPic);
            }
            loopFiltered = (nalu.m_nalUnitType == NAL_UNIT_EOS);
            if (nalu.m_nalUnitType == NAL_UNIT_EOS)
            {
                m_cTDecTop.setFirstSliceInSequence(true);
            }
        }
        else if ((bNewPicture || !bitstreamFile || nalu.m_nalUnitType == NAL_UNIT_EOS) &&
            m_cTDecTop.getFirstSliceInSequence())
        {
            m_cTDecTop.setFirstSliceInPicture(true);
        }

        if (pcListPic)
        {
            if ((!m_reconFileName.empty()) && (!openedReconFile))
            {
                const BitDepths& bitDepths = pcListPic->front()->getPicSym()->getSPS().getBitDepths(); // use bit depths of first reconstructed picture. 使用第一个重建图像的位深。
                for (UInt channelType = 0; channelType < MAX_NUM_CHANNEL_TYPE; channelType++)
                {
                    if (m_outputBitDepth[channelType] == 0)
                    {
                        m_outputBitDepth[channelType] = bitDepths.recon[channelType];
                    }
                }

                m_cTVideoIOYuvReconFile.open(m_reconFileName, true, m_outputBitDepth, m_outputBitDepth, bitDepths.recon); // write mode 写模式
                openedReconFile = true;
            }

            if (bNewPicture)
            {
                if (m_cTDecTop.getTwoVersionsOfCurrDecPicFlag())
                {
                    // remove current picture before ILF 在ILF之前删除当前图片
                    m_cTDecTop.remCurPicBefILFFromDPBDecDPBFullnessByOne(pcListPic);
                    m_cTDecTop.updateCurrentPictureFlag(pcListPic);
                }
                else if (m_cTDecTop.isCurrPicAsRef())
                {
                    m_cTDecTop.markCurrentPictureAfterILFforShortTermRef(pcListPic);
                }
            }

            // write reconstruction to file 将重构写入文件
            if (bNewPicture)
            {
                xWriteOutput(pcListPic, nalu.m_temporalId);
            }
            if ((bNewPicture || nalu.m_nalUnitType == NAL_UNIT_CODED_SLICE_CRA) && m_cTDecTop.getNoOutputPriorPicsFlag())
            {
                m_cTDecTop.checkNoOutputPriorPics(pcListPic);
                m_cTDecTop.setNoOutputPriorPicsFlag(false);
            }
            if (bNewPicture &&
                (nalu.m_nalUnitType == NAL_UNIT_CODED_SLICE_IDR_W_RADL
                    || nalu.m_nalUnitType == NAL_UNIT_CODED_SLICE_IDR_N_LP
                    || nalu.m_nalUnitType == NAL_UNIT_CODED_SLICE_BLA_N_LP
                    || nalu.m_nalUnitType == NAL_UNIT_CODED_SLICE_BLA_W_RADL
                    || nalu.m_nalUnitType == NAL_UNIT_CODED_SLICE_BLA_W_LP))
            {
                xFlushOutput(pcListPic);
            }
            if (nalu.m_nalUnitType == NAL_UNIT_EOS)
            {
                xWriteOutput(pcListPic, nalu.m_temporalId);
                m_cTDecTop.setFirstSliceInPicture(false);
            }
            // write reconstruction to file -- for additional bumping as defined in C.5.2.3 将重构写入文件——用于C.5.2.3中定义的附加碰撞
            if (!bNewPicture && nalu.m_nalUnitType >= NAL_UNIT_CODED_SLICE_TRAIL_N && nalu.m_nalUnitType <= NAL_UNIT_RESERVED_VCL31)
            {
                xWriteOutput(pcListPic, nalu.m_temporalId);
            }
        }
    }

    xFlushOutput(pcListPic);
    // delete buffers 删除缓冲区
    m_cTDecTop.deletePicBuffer();

    // destroy internal classes 销毁内部类
    xDestroyDecLib();
}

// ====================================================================================================================
// Protected member functions 受保护成员函数
// ====================================================================================================================

Void TAppDecTop::xCreateDecLib()
{
    // create decoder class 创建解码器类
    m_cTDecTop.create();
}

Void TAppDecTop::xDestroyDecLib()
{
    if (!m_reconFileName.empty())
    {
        m_cTVideoIOYuvReconFile.close();
    }

    // destroy decoder class 销毁解码器类
    m_cTDecTop.destroy();

    if (m_pcSeiColourRemappingInfoPrevious != NULL)
    {
        delete m_pcSeiColourRemappingInfoPrevious;
        m_pcSeiColourRemappingInfoPrevious = NULL;
    }
}

Void TAppDecTop::xInitDecLib()
{
    // initialize decoder class 初始化解码器类
    m_cTDecTop.init();
    m_cTDecTop.setDecodedPictureHashSEIEnabled(m_decodedPictureHashSEIEnabled);
#if MCTS_ENC_CHECK
    m_cTDecTop.setTMctsCheckEnabled(m_tmctsCheck);
#endif
#if O0043_BEST_EFFORT_DECODING
    m_cTDecTop.setForceDecodeBitDepth(m_forceDecodeBitDepth);
#endif
    if (!m_outputDecodedSEIMessagesFilename.empty())
    {
        std::ostream& os = m_seiMessageFileStream.is_open() ? m_seiMessageFileStream : std::cout;
        m_cTDecTop.setDecodedSEIMessageOutputStream(&os);
    }
    if (m_pcSeiColourRemappingInfoPrevious != NULL)
    {
        delete m_pcSeiColourRemappingInfoPrevious;
        m_pcSeiColourRemappingInfoPrevious = NULL;
    }
}

/** \param pcListPic list of pictures to be written to file param pcListPic要写入文件的图片列表
    \param tId       temporal sub-layer ID 时序子层ID
 */
Void TAppDecTop::xWriteOutput(TComList<TComPic*>* pcListPic, UInt tId)
{
    if (pcListPic->empty())
    {
        return;
    }

    TComList<TComPic*>::iterator iterPic = pcListPic->begin();
    Int numPicsNotYetDisplayed = 0;
    Int dpbFullness = 0;
    const TComSPS* activeSPS = &(pcListPic->front()->getPicSym()->getSPS());
    UInt numReorderPicsHighestTid;
    UInt maxDecPicBufferingHighestTid;
    UInt maxNrSublayers = activeSPS->getMaxTLayers();
    if (m_iMaxTemporalLayer == -1 || m_iMaxTemporalLayer >= maxNrSublayers)
    {
        numReorderPicsHighestTid = activeSPS->getNumReorderPics(maxNrSublayers - 1);
        maxDecPicBufferingHighestTid = activeSPS->getMaxDecPicBuffering(maxNrSublayers - 1);
    }
    else
    {
        numReorderPicsHighestTid = activeSPS->getNumReorderPics(m_iMaxTemporalLayer);
        maxDecPicBufferingHighestTid = activeSPS->getMaxDecPicBuffering(m_iMaxTemporalLayer);
    }

    while (iterPic != pcListPic->end())
    {
        TComPic* pcPic = *(iterPic);
        if (pcPic->getOutputMark() && pcPic->getPOC() > m_iPOCLastDisplay)
        {
            numPicsNotYetDisplayed++;
            dpbFullness++;
        }
        else if (pcPic->getSlice(0)->isReferenced())
        {
            dpbFullness++;
        }
        iterPic++;
    }

    iterPic = pcListPic->begin();

    if (numPicsNotYetDisplayed > 2)
    {
        iterPic++;
    }

    TComPic* pcPic = *(iterPic);
    if (numPicsNotYetDisplayed > 2 && pcPic->isField()) //Field Decoding 字段解码
    {
        TComList<TComPic*>::iterator endPic = pcListPic->end();
        endPic--;
        iterPic = pcListPic->begin();
        while (iterPic != endPic)
        {
            TComPic* pcPicTop = *(iterPic);
            iterPic++;
            TComPic* pcPicBottom = *(iterPic);

            if (pcPicTop->getOutputMark() && pcPicBottom->getOutputMark() &&
                (numPicsNotYetDisplayed > numReorderPicsHighestTid || dpbFullness > maxDecPicBufferingHighestTid - m_cTDecTop.getTwoVersionsOfCurrDecPicFlag()) &&
                (!(pcPicTop->getPOC() % 2) && pcPicBottom->getPOC() == pcPicTop->getPOC() + 1) &&
                (pcPicTop->getPOC() == m_iPOCLastDisplay + 1 || m_iPOCLastDisplay < 0))
            {
                // write to file 写入文件
                numPicsNotYetDisplayed = numPicsNotYetDisplayed - 2;
                if (!m_reconFileName.empty())
                {
                    const Window& conf = pcPicTop->getConformanceWindow();
                    const Window  defDisp = m_respectDefDispWindow ? pcPicTop->getDefDisplayWindow() : Window();
                    const Bool isTff = pcPicTop->isTopField();

                    Bool display = true;
                    if (m_decodedNoDisplaySEIEnabled)
                    {
                        SEIMessages noDisplay = getSeisByType(pcPic->getSEIs(), SEI::NO_DISPLAY);
                        const SEINoDisplay* nd = (noDisplay.size() > 0) ? (SEINoDisplay*)*(noDisplay.begin()) : NULL;
                        if ((nd != NULL) && nd->m_noDisplay)
                        {
                            display = false;
                        }
                    }

                    if (display)
                    {
                        m_cTVideoIOYuvReconFile.write(pcPicTop->getPicYuvRec(), pcPicBottom->getPicYuvRec(),
                            m_outputColourSpaceConvert,
                            conf.getWindowLeftOffset() + defDisp.getWindowLeftOffset(),
                            conf.getWindowRightOffset() + defDisp.getWindowRightOffset(),
                            conf.getWindowTopOffset() + defDisp.getWindowTopOffset(),
                            conf.getWindowBottomOffset() + defDisp.getWindowBottomOffset(), NUM_CHROMA_FORMAT, isTff);
                    }
                }

                // update POC of display order 更新POC显示顺序
                m_iPOCLastDisplay = pcPicBottom->getPOC();
                
                // erase non-referenced picture in the reference picture list after display 显示后，删除参考图片列表中的非参考图片
                if (!pcPicTop->getSlice(0)->isReferenced() && pcPicTop->getReconMark() == true)
                {
                    pcPicTop->setReconMark(false);

                    // mark it should be extended later 标明以后应该延长
                    pcPicTop->getPicYuvRec()->setBorderExtension(false);
                }
                if (!pcPicBottom->getSlice(0)->isReferenced() && pcPicBottom->getReconMark() == true)
                {
                    pcPicBottom->setReconMark(false);

                    // mark it should be extended later 标明以后应该延长
                    pcPicBottom->getPicYuvRec()->setBorderExtension(false);
                }
                pcPicTop->setOutputMark(false);
                pcPicBottom->setOutputMark(false);
            }
        }
    }
    else if (!pcPic->isField()) //Frame Decoding 帧解码
    {
        iterPic = pcListPic->begin();

        while (iterPic != pcListPic->end())
        {
            pcPic = *(iterPic);

            if (pcPic->getOutputMark() && pcPic->getPOC() > m_iPOCLastDisplay &&
                (numPicsNotYetDisplayed > numReorderPicsHighestTid || dpbFullness > maxDecPicBufferingHighestTid - m_cTDecTop.getTwoVersionsOfCurrDecPicFlag()))
            {
                // write to file 写入文件
                numPicsNotYetDisplayed--;
                if (pcPic->getSlice(0)->isReferenced() == false)
                {
                    dpbFullness--;
                }

                if (!m_reconFileName.empty())
                {
                    const Window& conf = pcPic->getConformanceWindow();
                    const Window  defDisp = m_respectDefDispWindow ? pcPic->getDefDisplayWindow() : Window();

                    m_cTVideoIOYuvReconFile.write(pcPic->getPicYuvRec(),
                        m_outputColourSpaceConvert,
                        conf.getWindowLeftOffset() + defDisp.getWindowLeftOffset(),
                        conf.getWindowRightOffset() + defDisp.getWindowRightOffset(),
                        conf.getWindowTopOffset() + defDisp.getWindowTopOffset(),
                        conf.getWindowBottomOffset() + defDisp.getWindowBottomOffset(),
                        NUM_CHROMA_FORMAT, m_bClipOutputVideoToRec709Range);
                }

                if (!m_colourRemapSEIFileName.empty())
                {
                    xOutputColourRemapPic(pcPic);
                }

                // update POC of display order 更新POC显示顺序
                m_iPOCLastDisplay = pcPic->getPOC();
                //仅输出了一个0
                //cout << m_iPOCLastDisplay << endl;
                // erase non-referenced picture in the reference picture list after display 显示后，删除参考图片列表中的非参考图片
                if (!pcPic->getSlice(0)->isReferenced() && pcPic->getReconMark() == true)
                {
                    pcPic->setReconMark(false);

                    // mark it should be extended later 标明以后应该延长
                    pcPic->getPicYuvRec()->setBorderExtension(false);
                }
                pcPic->setOutputMark(false);
            }

            iterPic++;
        }
    }
}

/** \param pcListPic list of pictures to be written to file param pcListPic要写入文件的图片列表
 */
Void TAppDecTop::xFlushOutput(TComList<TComPic*>* pcListPic) // 将解码得到的图像数据从缓冲区中取出，并进行格式转换和输出操作，最终将图像显示或写入文件。
{
    if (!pcListPic || pcListPic->empty())
    {
        return;
    }
    TComList<TComPic*>::iterator iterPic = pcListPic->begin();
    iterPic = pcListPic->begin();
    TComPic* pcPic = *(iterPic);

    if (pcPic->isField()) //Field Decoding 字段解码
    {
        TComList<TComPic*>::iterator endPic = pcListPic->end();
        endPic--;
        TComPic* pcPicTop, * pcPicBottom = NULL;
        while (iterPic != endPic)
        {
            pcPicTop = *(iterPic);
            iterPic++;
            pcPicBottom = *(iterPic);

            if (pcPicTop->getOutputMark() && pcPicBottom->getOutputMark() && !(pcPicTop->getPOC() % 2) && (pcPicBottom->getPOC() == pcPicTop->getPOC() + 1))
            {
                // write to file 写入文件
                if (!m_reconFileName.empty())
                {
                    const Window& conf = pcPicTop->getConformanceWindow();
                    const Window  defDisp = m_respectDefDispWindow ? pcPicTop->getDefDisplayWindow() : Window();
                    const Bool isTff = pcPicTop->isTopField();
                    m_cTVideoIOYuvReconFile.write(pcPicTop->getPicYuvRec(), pcPicBottom->getPicYuvRec(),
                        m_outputColourSpaceConvert,
                        conf.getWindowLeftOffset() + defDisp.getWindowLeftOffset(),
                        conf.getWindowRightOffset() + defDisp.getWindowRightOffset(),
                        conf.getWindowTopOffset() + defDisp.getWindowTopOffset(),
                        conf.getWindowBottomOffset() + defDisp.getWindowBottomOffset(), NUM_CHROMA_FORMAT, isTff);
                }

                // update POC of display order 更新POC显示顺序
                m_iPOCLastDisplay = pcPicBottom->getPOC();

                // erase non-referenced picture in the reference picture list after display 显示后，删除参考图片列表中的非参考图片
                if (!pcPicTop->getSlice(0)->isReferenced() && pcPicTop->getReconMark() == true)
                {
                    pcPicTop->setReconMark(false);

                    // mark it should be extended later 标明以后应该延长
                    pcPicTop->getPicYuvRec()->setBorderExtension(false);
                }
                if (!pcPicBottom->getSlice(0)->isReferenced() && pcPicBottom->getReconMark() == true)
                {
                    pcPicBottom->setReconMark(false);

                    // mark it should be extended later 标明以后应该延长
                    pcPicBottom->getPicYuvRec()->setBorderExtension(false);
                }
                pcPicTop->setOutputMark(false);
                pcPicBottom->setOutputMark(false);

                if (pcPicTop)
                {
                    pcPicTop->destroy();
                    delete pcPicTop;
                    pcPicTop = NULL;
                }
            }
        }
        if (pcPicBottom)
        {
            pcPicBottom->destroy();
            delete pcPicBottom;
            pcPicBottom = NULL;
        }
    }
    else //Frame decoding 帧解码 此处会中断
    {
        while (iterPic != pcListPic->end()) //会执行三次这个循环

        {
            pcPic = *(iterPic);

            if (pcPic->getOutputMark())
            {
                // write to file 写入文件
                if (!m_reconFileName.empty())
                {
                    const Window& conf = pcPic->getConformanceWindow();
                    const Window  defDisp = m_respectDefDispWindow ? pcPic->getDefDisplayWindow() : Window();

                    m_cTVideoIOYuvReconFile.write(pcPic->getPicYuvRec(),
                        m_outputColourSpaceConvert,
                        conf.getWindowLeftOffset() + defDisp.getWindowLeftOffset(),
                        conf.getWindowRightOffset() + defDisp.getWindowRightOffset(),
                        conf.getWindowTopOffset() + defDisp.getWindowTopOffset(),
                        conf.getWindowBottomOffset() + defDisp.getWindowBottomOffset(),
                        NUM_CHROMA_FORMAT, m_bClipOutputVideoToRec709Range);
                }

                if (!m_colourRemapSEIFileName.empty())
                {
                    xOutputColourRemapPic(pcPic);
                }
               
                // update POC of display order 更新POC显示顺序
                m_iPOCLastDisplay = pcPic->getPOC();
               

                // erase non-referenced picture in the reference picture list after display 显示后，删除参考图片列表中的非参考图片
                if (!pcPic->getSlice(0)->isReferenced() && pcPic->getReconMark() == true)
                {
                    pcPic->setReconMark(false);

                    // mark it should be extended later 标明以后应该延长
                    pcPic->getPicYuvRec()->setBorderExtension(false);
                }
                pcPic->setOutputMark(false);
            }
            if (pcPic != NULL)
            {
                pcPic->destroy();
                delete pcPic;
                pcPic = NULL;
            }
            iterPic++;
        }
    }
    pcListPic->clear();
    m_iPOCLastDisplay = -MAX_INT;
}

/** \param nalu Input nalu to check whether its LayerId is within targetDecLayerIdSet param nalu输入nalu检查其LayerId是否在targetDecLayerIdSet中
 */
Bool TAppDecTop::isNaluWithinTargetDecLayerIdSet(InputNALUnit* nalu)
{
    if (m_targetDecLayerIdSet.size() == 0) // By default, the set is empty, meaning all LayerIds are allowed 默认情况下，该集合为空，这意味着允许所有layerid
    {
        return true;
    }
    for (std::vector<Int>::iterator it = m_targetDecLayerIdSet.begin(); it != m_targetDecLayerIdSet.end(); it++)
    {
        if (nalu->m_nuhLayerId == (*it))
        {
            return true;
        }
    }
    return false;
}

Void TAppDecTop::xOutputColourRemapPic(TComPic* pcPic)
{
    const TComSPS& sps = pcPic->getPicSym()->getSPS();
    SEIMessages colourRemappingInfo = getSeisByType(pcPic->getSEIs(), SEI::COLOUR_REMAPPING_INFO);
    SEIColourRemappingInfo* seiColourRemappingInfo = (colourRemappingInfo.size() > 0) ? (SEIColourRemappingInfo*)*(colourRemappingInfo.begin()) : NULL;

    if (colourRemappingInfo.size() > 1)
    {
        printf("Warning: Got multiple Colour Remapping Information SEI messages. Using first.");
    }
    if (seiColourRemappingInfo)
    {
        applyColourRemapping(*pcPic->getPicYuvRec(), *seiColourRemappingInfo, sps);

        // save the last CRI SEI received 保存SEI收到的最后一个CRI
        if (m_pcSeiColourRemappingInfoPrevious == NULL)
        {
            m_pcSeiColourRemappingInfoPrevious = new SEIColourRemappingInfo();
        }
        m_pcSeiColourRemappingInfoPrevious->copyFrom(*seiColourRemappingInfo);
    }
    else  // using the last CRI SEI received 使用SEI收到的最后一个CRI
    {
        // TODO: prevent persistence of CRI SEI across C(L)VS. 待办事项:防止跨C(L)VS. CRI SEI的持久性。
        if (m_pcSeiColourRemappingInfoPrevious != NULL)
        {
            if (m_pcSeiColourRemappingInfoPrevious->m_colourRemapPersistenceFlag == false)
            {
                printf("Warning No SEI-CRI message is present for the current picture, persistence of the CRI is not managed\n");
            }
            applyColourRemapping(*pcPic->getPicYuvRec(), *m_pcSeiColourRemappingInfoPrevious, sps);
        }
    }
}

// compute lut from SEI 从SEI中计算lut
// use at lutPoints points aligned on a power of 2 value 使用在lutPoints点对齐为2的幂值
// SEI Lut must be in ascending values of coded Values SEI Lut必须在编码值的升序值中
static std::vector<Int>
initColourRemappingInfoLut(const Int                                          bitDepth_in,     // bit-depth of the input values of the LUT LUT输入值的位深
    const Int                                          nbDecimalValues, // Position of the fixed point 固定点的位置
    const std::vector<SEIColourRemappingInfo::CRIlut>& lut,
    const Int                                          maxValue, // maximum output value 最大输出值
    const Int                                          lutOffset)
{
    const Int lutPoints = (1 << bitDepth_in) + 1;
    std::vector<Int> retLut(lutPoints);

    // missing values: need to define default values before first definition (check codedValue[0] == 0) 缺失值:需要在第一次定义之前定义默认值(检查codedValue[0] == 0)
    Int iTargetPrev = (lut.size() && lut[0].codedValue == 0) ? lut[0].targetValue : 0;
    Int startPivot = (lut.size()) ? ((lut[0].codedValue == 0) ? 1 : 0) : 1;
    Int iCodedPrev = 0;
    // set max value with the coded bit-depth 用编码的位深度设置最大值
    // + ((1 << nbDecimalValues) - 1) is for the added bits + ((1 << nbDecimalValues) - 1)为添加的位
    const Int maxValueFixedPoint = (maxValue << nbDecimalValues) + ((1 << nbDecimalValues) - 1);

    Int iValue = 0;

    for (Int iPivot = startPivot; iPivot < (Int)lut.size(); iPivot++)
    {
        Int iCodedNext = lut[iPivot].codedValue;
        Int iTargetNext = lut[iPivot].targetValue;

        // ensure correct bit depth and avoid overflow in lut address 确保正确的位深，避免lut地址溢出
        Int iCodedNext_bitDepth = std::min(iCodedNext, (1 << bitDepth_in));

        const Int divValue = (iCodedNext - iCodedPrev > 0) ? (iCodedNext - iCodedPrev) : 1;
        const Int lutValInit = (lutOffset + iTargetPrev) << nbDecimalValues;
        const Int roundValue = divValue / 2;
        for (; iValue < iCodedNext_bitDepth; iValue++)
        {
            Int value = iValue;
            Int interpol = ((((value - iCodedPrev) * (iTargetNext - iTargetPrev)) << nbDecimalValues) + roundValue) / divValue;
            retLut[iValue] = std::min(lutValInit + interpol, maxValueFixedPoint);
        }
        iCodedPrev = iCodedNext;
        iTargetPrev = iTargetNext;
    }
    // fill missing values if necessary 如有必要，填补缺失的值
    if (iCodedPrev < (1 << bitDepth_in) + 1)
    {
        Int iCodedNext = (1 << bitDepth_in);
        Int iTargetNext = (1 << bitDepth_in) - 1;

        const Int divValue = (iCodedNext - iCodedPrev > 0) ? (iCodedNext - iCodedPrev) : 1;
        const Int lutValInit = (lutOffset + iTargetPrev) << nbDecimalValues;
        const Int roundValue = divValue / 2;

        for (; iValue <= iCodedNext; iValue++)
        {
            Int value = iValue;
            Int interpol = ((((value - iCodedPrev) * (iTargetNext - iTargetPrev)) << nbDecimalValues) + roundValue) / divValue;
            retLut[iValue] = std::min(lutValInit + interpol, maxValueFixedPoint);
        }
    }
    return retLut;
}

static Void
initColourRemappingInfoLuts(std::vector<Int>(&preLut)[3],
    std::vector<Int>(&postLut)[3],
    SEIColourRemappingInfo& pCriSEI,
    const Int               maxBitDepth)
{
    Int internalBitDepth = pCriSEI.m_colourRemapBitDepth;
    for (Int c = 0; c < 3; c++)
    {
        std::sort(pCriSEI.m_preLut[c].begin(), pCriSEI.m_preLut[c].end()); // ensure preLut is ordered in ascending values of codedValues    确保preLut在codedValues的升序值中有序
        preLut[c] = initColourRemappingInfoLut(pCriSEI.m_colourRemapInputBitDepth, maxBitDepth - pCriSEI.m_colourRemapInputBitDepth, pCriSEI.m_preLut[c], ((1 << internalBitDepth) - 1), 0); //Fill preLut 填充preLut

        std::sort(pCriSEI.m_postLut[c].begin(), pCriSEI.m_postLut[c].end()); // ensure postLut is ordered in ascending values of codedValues     确保postLut按照codedValues的升序排列   
        postLut[c] = initColourRemappingInfoLut(pCriSEI.m_colourRemapBitDepth, maxBitDepth - pCriSEI.m_colourRemapBitDepth, pCriSEI.m_postLut[c], (1 << internalBitDepth) - 1, 0); //Fill postLut 填补postLut
    }
}

// apply lut.
// Input lut values are aligned on power of 2 boundaries 输入lut值按2的幂边界对齐
static Int
applyColourRemappingInfoLut1D(Int inVal, const std::vector<Int>& lut, const Int inValPrecisionBits)
{
    const Int roundValue = (inValPrecisionBits) ? 1 << (inValPrecisionBits - 1) : 0;
    inVal = std::min(std::max(0, inVal), (Int)(((lut.size() - 1) << inValPrecisionBits)));
    Int index = (Int)std::min((inVal >> inValPrecisionBits), (Int)(lut.size() - 2));
    Int outVal = ((inVal - (index << inValPrecisionBits)) * (lut[index + 1] - lut[index]) + roundValue) >> inValPrecisionBits;
    outVal += lut[index];

    return outVal;
}

static Int
applyColourRemappingInfoMatrix(const Int(&colourRemapCoeffs)[3], const Int postOffsetShift, const Int p0, const Int p1, const Int p2, const Int offset)
{
    Int YUVMat = (colourRemapCoeffs[0] * p0 + colourRemapCoeffs[1] * p1 + colourRemapCoeffs[2] * p2 + offset) >> postOffsetShift;
    return YUVMat;
}

static Void
setColourRemappingInfoMatrixOffset(Int(&matrixOffset)[3], Int offset0, Int offset1, Int offset2)
{
    matrixOffset[0] = offset0;
    matrixOffset[1] = offset1;
    matrixOffset[2] = offset2;
}

static Void
setColourRemappingInfoMatrixOffsets(Int(&matrixInputOffset)[3],
    Int(&matrixOutputOffset)[3],
    const Int  bitDepth,
    const Bool crInputFullRangeFlag,
    const Int  crInputMatrixCoefficients,
    const Bool crFullRangeFlag,
    const Int  crMatrixCoefficients)
{
    // set static matrix offsets 设置静态矩阵偏移量
    Int crInputOffsetLuma = (crInputFullRangeFlag) ? 0 : -(16 << (bitDepth - 8));
    Int crOffsetLuma = (crFullRangeFlag) ? 0 : (16 << (bitDepth - 8));
    Int crInputOffsetChroma = 0;
    Int crOffsetChroma = 0;

    switch (crInputMatrixCoefficients)
    {
    case MATRIX_COEFFICIENTS_RGB:
        crInputOffsetChroma = 0;
        if (!crInputFullRangeFlag)
        {
            fprintf(stderr, "WARNING: crInputMatrixCoefficients set to MATRIX_COEFFICIENTS_RGB and crInputFullRangeFlag not set\n");
            crInputOffsetLuma = 0;
        }
        break;
    case MATRIX_COEFFICIENTS_UNSPECIFIED:
    case MATRIX_COEFFICIENTS_BT709:
    case MATRIX_COEFFICIENTS_BT2020_NON_CONSTANT_LUMINANCE:
        crInputOffsetChroma = -(1 << (bitDepth - 1));
        break;
    default:
        fprintf(stderr, "WARNING: crInputMatrixCoefficients set to undefined value: %d\n", crInputMatrixCoefficients);
    }

    switch (crMatrixCoefficients)
    {
    case MATRIX_COEFFICIENTS_RGB:
        crOffsetChroma = 0;
        if (!crFullRangeFlag)
        {
            fprintf(stderr, "WARNING: crMatrixCoefficients set to MATRIX_COEFFICIENTS_RGB and crInputFullRangeFlag not set\n");
            crOffsetLuma = 0;
        }
        break;
    case MATRIX_COEFFICIENTS_UNSPECIFIED:
    case MATRIX_COEFFICIENTS_BT709:
    case MATRIX_COEFFICIENTS_BT2020_NON_CONSTANT_LUMINANCE:
        crOffsetChroma = (1 << (bitDepth - 1));
        break;
    default:
        fprintf(stderr, "WARNING: crMatrixCoefficients set to undefined value: %d\n", crMatrixCoefficients);
    }

    setColourRemappingInfoMatrixOffset(matrixInputOffset, crInputOffsetLuma, crInputOffsetChroma, crInputOffsetChroma);
    setColourRemappingInfoMatrixOffset(matrixOutputOffset, crOffsetLuma, crOffsetChroma, crOffsetChroma);
}

Void TAppDecTop::applyColourRemapping(const TComPicYuv& pic, SEIColourRemappingInfo& criSEI, const TComSPS& activeSPS)
{
    const Int maxBitDepth = 16;

    // create colour remapped picture 创建彩色重映射的图片
    if (!criSEI.m_colourRemapCancelFlag && pic.getChromaFormat() != CHROMA_400) // 4:0:0 not supported. 不支持4:0:0。
    {
        const Int          iHeight = pic.getHeight(COMPONENT_Y);
        const Int          iWidth = pic.getWidth(COMPONENT_Y);
        const ChromaFormat chromaFormatIDC = pic.getChromaFormat();

        TComPicYuv picYuvColourRemapped;
        picYuvColourRemapped.createWithoutCUInfo(iWidth, iHeight, chromaFormatIDC);

        const Int  iStrideIn = pic.getStride(COMPONENT_Y);
        const Int  iCStrideIn = pic.getStride(COMPONENT_Cb);
        const Int  iStrideOut = picYuvColourRemapped.getStride(COMPONENT_Y);
        const Int  iCStrideOut = picYuvColourRemapped.getStride(COMPONENT_Cb);
        const Bool b444 = (pic.getChromaFormat() == CHROMA_444);
        const Bool b422 = (pic.getChromaFormat() == CHROMA_422);
        const Bool b420 = (pic.getChromaFormat() == CHROMA_420);

        std::vector<Int> preLut[3];
        std::vector<Int> postLut[3];
        Int matrixInputOffset[3];
        Int matrixOutputOffset[3];
        const Pel* YUVIn[MAX_NUM_COMPONENT];
        Pel* YUVOut[MAX_NUM_COMPONENT];
        YUVIn[COMPONENT_Y] = pic.getAddr(COMPONENT_Y);
        YUVIn[COMPONENT_Cb] = pic.getAddr(COMPONENT_Cb);
        YUVIn[COMPONENT_Cr] = pic.getAddr(COMPONENT_Cr);
        YUVOut[COMPONENT_Y] = picYuvColourRemapped.getAddr(COMPONENT_Y);
        YUVOut[COMPONENT_Cb] = picYuvColourRemapped.getAddr(COMPONENT_Cb);
        YUVOut[COMPONENT_Cr] = picYuvColourRemapped.getAddr(COMPONENT_Cr);

        const Int bitDepth = criSEI.m_colourRemapBitDepth;
        BitDepths        bitDepthsCriFile;
        bitDepthsCriFile.recon[CHANNEL_TYPE_LUMA] = bitDepth;
        bitDepthsCriFile.recon[CHANNEL_TYPE_CHROMA] = bitDepth; // Different bitdepth is not implemented 没有实现不同的位深

        const Int postOffsetShift = criSEI.m_log2MatrixDenom;
        const Int matrixRound = 1 << (postOffsetShift - 1);
        const Int postLutInputPrecision = (maxBitDepth - criSEI.m_colourRemapBitDepth);

        if (!criSEI.m_colourRemapVideoSignalInfoPresentFlag) // setting default 默认设置
        {
            setColourRemappingInfoMatrixOffsets(matrixInputOffset, matrixOutputOffset, maxBitDepth,
                activeSPS.getVuiParameters()->getVideoFullRangeFlag(), activeSPS.getVuiParameters()->getMatrixCoefficients(),
                activeSPS.getVuiParameters()->getVideoFullRangeFlag(), activeSPS.getVuiParameters()->getMatrixCoefficients());
        }
        else
        {
            setColourRemappingInfoMatrixOffsets(matrixInputOffset, matrixOutputOffset, maxBitDepth,
                activeSPS.getVuiParameters()->getVideoFullRangeFlag(), activeSPS.getVuiParameters()->getMatrixCoefficients(),
                criSEI.m_colourRemapFullRangeFlag, criSEI.m_colourRemapMatrixCoefficients);
        }

        // add matrix rounding to output matrix offsets 将矩阵舍入添加到输出矩阵偏移量
        matrixOutputOffset[0] = (matrixOutputOffset[0] << postOffsetShift) + matrixRound;
        matrixOutputOffset[1] = (matrixOutputOffset[1] << postOffsetShift) + matrixRound;
        matrixOutputOffset[2] = (matrixOutputOffset[2] << postOffsetShift) + matrixRound;

        // Merge   matrixInputOffset and matrixOutputOffset to matrixOutputOffset 合并matrixInputOffset和matrixOutputOffset到matrixOutputOffset
        matrixOutputOffset[0] += applyColourRemappingInfoMatrix(criSEI.m_colourRemapCoeffs[0], 0, matrixInputOffset[0], matrixInputOffset[1], matrixInputOffset[2], 0);
        matrixOutputOffset[1] += applyColourRemappingInfoMatrix(criSEI.m_colourRemapCoeffs[1], 0, matrixInputOffset[0], matrixInputOffset[1], matrixInputOffset[2], 0);
        matrixOutputOffset[2] += applyColourRemappingInfoMatrix(criSEI.m_colourRemapCoeffs[2], 0, matrixInputOffset[0], matrixInputOffset[1], matrixInputOffset[2], 0);

        // rescaling output: include CRI/output frame difference 缩放输出:包括CRI/输出帧差
        const Int scaleShiftOut_neg = abs(bitDepth - maxBitDepth);
        const Int scaleOut_round = 1 << (scaleShiftOut_neg - 1);
        //没反应
        //cout << scaleShiftOut_neg << endl;
        initColourRemappingInfoLuts(preLut, postLut, criSEI, maxBitDepth);

        assert(pic.getChromaFormat() != CHROMA_400);
        const Int hs = pic.getComponentScaleX(ComponentID(COMPONENT_Cb));
        const Int maxOutputValue = (1 << bitDepth) - 1;

        for (Int y = 0; y < iHeight; y++)
        {
            for (Int x = 0; x < iWidth; x++)
            {
                const Int xc = (x >> hs);
                Bool computeChroma = b444 || ((b422 || !(y & 1)) && !(x & 1));

                Int YUVPre_0 = applyColourRemappingInfoLut1D(YUVIn[COMPONENT_Y][x], preLut[0], 0);
                Int YUVPre_1 = applyColourRemappingInfoLut1D(YUVIn[COMPONENT_Cb][xc], preLut[1], 0);
                Int YUVPre_2 = applyColourRemappingInfoLut1D(YUVIn[COMPONENT_Cr][xc], preLut[2], 0);

                Int YUVMat_0 = applyColourRemappingInfoMatrix(criSEI.m_colourRemapCoeffs[0], postOffsetShift, YUVPre_0, YUVPre_1, YUVPre_2, matrixOutputOffset[0]);
                Int YUVLutB_0 = applyColourRemappingInfoLut1D(YUVMat_0, postLut[0], postLutInputPrecision);
                YUVOut[COMPONENT_Y][x] = std::min(maxOutputValue, (YUVLutB_0 + scaleOut_round) >> scaleShiftOut_neg);

                if (computeChroma)
                {
                    Int YUVMat_1 = applyColourRemappingInfoMatrix(criSEI.m_colourRemapCoeffs[1], postOffsetShift, YUVPre_0, YUVPre_1, YUVPre_2, matrixOutputOffset[1]);
                    Int YUVLutB_1 = applyColourRemappingInfoLut1D(YUVMat_1, postLut[1], postLutInputPrecision);
                    YUVOut[COMPONENT_Cb][xc] = std::min(maxOutputValue, (YUVLutB_1 + scaleOut_round) >> scaleShiftOut_neg);

                    Int YUVMat_2 = applyColourRemappingInfoMatrix(criSEI.m_colourRemapCoeffs[2], postOffsetShift, YUVPre_0, YUVPre_1, YUVPre_2, matrixOutputOffset[2]);
                    Int YUVLutB_2 = applyColourRemappingInfoLut1D(YUVMat_2, postLut[2], postLutInputPrecision);
                    YUVOut[COMPONENT_Cr][xc] = std::min(maxOutputValue, (YUVLutB_2 + scaleOut_round) >> scaleShiftOut_neg);
                }
            }

            YUVIn[COMPONENT_Y] += iStrideIn;
            YUVOut[COMPONENT_Y] += iStrideOut;
            if (!(b420 && !(y & 1)))
            {
                YUVIn[COMPONENT_Cb] += iCStrideIn;
                YUVIn[COMPONENT_Cr] += iCStrideIn;
                YUVOut[COMPONENT_Cb] += iCStrideOut;
                YUVOut[COMPONENT_Cr] += iCStrideOut;
            }
        }
        //Write remapped picture in display order 按显示顺序写重新映射的图片
        picYuvColourRemapped.dump(m_colourRemapSEIFileName, bitDepthsCriFile, true);
        picYuvColourRemapped.destroy();
    }
}

//! \}
