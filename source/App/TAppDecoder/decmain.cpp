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

 /** \file     decmain.cpp
     \brief    Decoder application main
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "TAppDecTop.h"
#include"TLibCommon/TComDataCU.h"
#include"TLibDecoder/TDecCu.h"
#include <TLibDecoder/TDecCu.cpp>
#include <TLibCommon/TComTrQuant.cpp>

 //! \ingroup TAppDecoder
 //! \{

 // ====================================================================================================================
 // Main function 主要功能
 // ====================================================================================================================

int main(int argc, char* argv[])
{
    Int returnCode = EXIT_SUCCESS;
    TAppDecTop  cTAppDecTop;

    // print information 打印信息
   // fprintf(stdout, "\n");
   // fprintf(stdout, "HM software: Decoder Version [%s] (including RExt)", NV_VERSION);
   // fprintf(stdout, NVM_ONOS);
   // fprintf(stdout, NVM_COMPILEDBY);
   // fprintf(stdout, NVM_BITS);
   // fprintf(stdout, "\n");

    // create application decoder class 创建应用程序解码器类
    cTAppDecTop.create();

    // parse configuration 解析配置
    if (!cTAppDecTop.parseCfg(argc, argv))
    {
        cTAppDecTop.destroy();
        returnCode = EXIT_FAILURE;
        return returnCode;
    }






    // starting time 起始时间
    Double dResult;
    clock_t lBefore = clock();

    // call decoding function 引用解码功能
    cTAppDecTop.decode();
    if (cTAppDecTop.getNumberOfChecksumErrorsDetected() != 0)
    {
        printf("\n\n***ERROR*** A decoding mismatch occured: signalled md5sum does not match\n");
        returnCode = EXIT_FAILURE;
    }
    // ending time 结束时间
    // cout << TComDataCU::m_phQP << endl;
    dResult = (Double)(clock() - lBefore) / CLOCKS_PER_SEC;
    printf("\n Total Time: %12.3f sec.\n", dResult);


    // destroy application decoder class 销毁应用程序解码器类
    cTAppDecTop.destroy();
    /*
  //  printf("0chuxian  %d \n", AAAAA[0]);
    //LorS 顺序为4*4 L 4*4 S 8*8L 8*8S....
    //cout << "4DCT系数平方和为"<< DCT4HE<< " " << endl;
    //cout << "81DCT系数平方和为" << DCT8HE1 << " " << endl;
    cout << "80DCT系数平方和为" << DCT8HE0 << " " << endl;
    cout << "161DCT系数平方和为" << DCT16HE1 << " " << endl;
    cout << "321DCT系数平方和为" << DCT32HE1 << " " << endl;
    cout << "160DCT系数平方和为" << DCT16HE0 << " " << endl;
    cout << "320DCT系数平方和为" << DCT32HE0 << " " << endl;
    cout << "8DCT系数平方和为" << DCT8HE << " " << endl;
    cout << "16DCT系数平方和为" << DCT16HE << " " << endl;
    cout << "32DCT系数平方和为" << DCT32HE << " " << endl;
    //cout << "0位深出现  " << AAAAA[0] << " " << endl;
    //cout << "1位深出现  " << AAAAA[1] << " " << endl;
    //cout << "2位深出现  " << AAAAA[2] << " " << endl;
    //cout << "3位深出现  " << AAAAA[3] << " " << endl;
    //cout << "canchahe" << AAAA << endl;
    //cout << "canchajueduizhi" << BBBB << endl;
    cout << "4DST系数平方和为"<< DST4HE<< " " << endl;
    cout << "4亮度出现次数" << LorS[0] << endl;
    cout << "4色度出现次数" << LorS[1] << endl;
    cout << "8亮度出现次数" << LorS[2] << endl;
    cout << "8色度出现次数" << LorS[3] << endl;
    cout << "16亮度出现次数" << LorS[4] << endl;
    cout << "16色度出现次数" << LorS[5] << endl;
    cout << "32亮度出现次数" << LorS[6] << endl;
    cout << "32色度出现次数" << LorS[7] << endl;
    */
    cout << "时间" << DECODER_time << endl;
    //cout << "DST调用了  " << CCCC0 << endl;
    //cout <<"4*4调用了  "  <<CCCC4 << endl;
    //cout << "8*8调用了  " << CCCC8 << endl;
    //cout << "16*16调用了  " << CCCC16 << endl;
    //cout << "32*32调用了  " << CCCC32 << endl;
    //cout << "M*N调用了  " << CCCCMN << endl;
    return returnCode;
}




//! \}
