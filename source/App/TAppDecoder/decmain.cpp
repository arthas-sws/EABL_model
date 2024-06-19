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
 // Main function ��Ҫ����
 // ====================================================================================================================

int main(int argc, char* argv[])
{
    Int returnCode = EXIT_SUCCESS;
    TAppDecTop  cTAppDecTop;

    // print information ��ӡ��Ϣ
   // fprintf(stdout, "\n");
   // fprintf(stdout, "HM software: Decoder Version [%s] (including RExt)", NV_VERSION);
   // fprintf(stdout, NVM_ONOS);
   // fprintf(stdout, NVM_COMPILEDBY);
   // fprintf(stdout, NVM_BITS);
   // fprintf(stdout, "\n");

    // create application decoder class ����Ӧ�ó����������
    cTAppDecTop.create();

    // parse configuration ��������
    if (!cTAppDecTop.parseCfg(argc, argv))
    {
        cTAppDecTop.destroy();
        returnCode = EXIT_FAILURE;
        return returnCode;
    }






    // starting time ��ʼʱ��
    Double dResult;
    clock_t lBefore = clock();

    // call decoding function ���ý��빦��
    cTAppDecTop.decode();
    if (cTAppDecTop.getNumberOfChecksumErrorsDetected() != 0)
    {
        printf("\n\n***ERROR*** A decoding mismatch occured: signalled md5sum does not match\n");
        returnCode = EXIT_FAILURE;
    }
    // ending time ����ʱ��
    // cout << TComDataCU::m_phQP << endl;
    dResult = (Double)(clock() - lBefore) / CLOCKS_PER_SEC;
    printf("\n Total Time: %12.3f sec.\n", dResult);


    // destroy application decoder class ����Ӧ�ó����������
    cTAppDecTop.destroy();
    /*
  //  printf("0chuxian  %d \n", AAAAA[0]);
    //LorS ˳��Ϊ4*4 L 4*4 S 8*8L 8*8S....
    //cout << "4DCTϵ��ƽ����Ϊ"<< DCT4HE<< " " << endl;
    //cout << "81DCTϵ��ƽ����Ϊ" << DCT8HE1 << " " << endl;
    cout << "80DCTϵ��ƽ����Ϊ" << DCT8HE0 << " " << endl;
    cout << "161DCTϵ��ƽ����Ϊ" << DCT16HE1 << " " << endl;
    cout << "321DCTϵ��ƽ����Ϊ" << DCT32HE1 << " " << endl;
    cout << "160DCTϵ��ƽ����Ϊ" << DCT16HE0 << " " << endl;
    cout << "320DCTϵ��ƽ����Ϊ" << DCT32HE0 << " " << endl;
    cout << "8DCTϵ��ƽ����Ϊ" << DCT8HE << " " << endl;
    cout << "16DCTϵ��ƽ����Ϊ" << DCT16HE << " " << endl;
    cout << "32DCTϵ��ƽ����Ϊ" << DCT32HE << " " << endl;
    //cout << "0λ�����  " << AAAAA[0] << " " << endl;
    //cout << "1λ�����  " << AAAAA[1] << " " << endl;
    //cout << "2λ�����  " << AAAAA[2] << " " << endl;
    //cout << "3λ�����  " << AAAAA[3] << " " << endl;
    //cout << "canchahe" << AAAA << endl;
    //cout << "canchajueduizhi" << BBBB << endl;
    cout << "4DSTϵ��ƽ����Ϊ"<< DST4HE<< " " << endl;
    cout << "4���ȳ��ִ���" << LorS[0] << endl;
    cout << "4ɫ�ȳ��ִ���" << LorS[1] << endl;
    cout << "8���ȳ��ִ���" << LorS[2] << endl;
    cout << "8ɫ�ȳ��ִ���" << LorS[3] << endl;
    cout << "16���ȳ��ִ���" << LorS[4] << endl;
    cout << "16ɫ�ȳ��ִ���" << LorS[5] << endl;
    cout << "32���ȳ��ִ���" << LorS[6] << endl;
    cout << "32ɫ�ȳ��ִ���" << LorS[7] << endl;
    */
    cout << "ʱ��" << DECODER_time << endl;
    //cout << "DST������  " << CCCC0 << endl;
    //cout <<"4*4������  "  <<CCCC4 << endl;
    //cout << "8*8������  " << CCCC8 << endl;
    //cout << "16*16������  " << CCCC16 << endl;
    //cout << "32*32������  " << CCCC32 << endl;
    //cout << "M*N������  " << CCCCMN << endl;
    return returnCode;
}




//! \}
