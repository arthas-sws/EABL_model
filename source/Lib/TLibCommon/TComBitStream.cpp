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

/** \file     TComBitStream.cpp
    \brief    class for handling bitstream 类用于处理位流
*/

#include <stdint.h>
#include <vector>
#include "TComBitStream.h"
#include <string.h>
#include <memory.h>

using namespace std;

//! \ingroup TLibCommon
//! \{

// ====================================================================================================================
// Constructor / destructor / create / destroy 构造函数/析构函数/创建/销毁
// ====================================================================================================================

TComOutputBitstream::TComOutputBitstream()
{
  clear();
}

TComOutputBitstream::~TComOutputBitstream()
{
}


TComInputBitstream::TComInputBitstream()
: m_fifo()
, m_emulationPreventionByteLocation()
, m_fifo_idx(0)
, m_num_held_bits(0)
, m_held_bits(0)
, m_numBitsRead(0)
{ }

TComInputBitstream::TComInputBitstream(const TComInputBitstream &src)
: m_fifo(src.m_fifo)
, m_emulationPreventionByteLocation(src.m_emulationPreventionByteLocation)
, m_fifo_idx(src.m_fifo_idx)
, m_num_held_bits(src.m_num_held_bits)
, m_held_bits(src.m_held_bits)
, m_numBitsRead(src.m_numBitsRead)
{ }

// ====================================================================================================================
// Public member functions 公共成员功能
// ====================================================================================================================

Void TComInputBitstream::resetToStart()
{
  m_fifo_idx=0;
  m_num_held_bits=0;
  m_held_bits=0;
  m_numBitsRead=0;
}

UChar* TComOutputBitstream::getByteStream() const
{
  return (UChar*) &m_fifo.front();
}

UInt TComOutputBitstream::getByteStreamLength()
{
  return UInt(m_fifo.size());
}

Void TComOutputBitstream::clear()
{
  m_fifo.clear();
  m_held_bits = 0;
  m_num_held_bits = 0;
}

Void TComOutputBitstream::write   ( UInt uiBits, UInt uiNumberOfBits )
{
  assert( uiNumberOfBits <= 32 );
  assert( uiNumberOfBits == 32 || (uiBits & (~0 << uiNumberOfBits)) == 0 );

  /* any modulo 8 remainder of num_total_bits cannot be written this time, 任意num_total_bits的模8余数这次不能写入，将保留到下次
   * and will be held until next time. */
  UInt num_total_bits = uiNumberOfBits + m_num_held_bits;
  UInt next_num_held_bits = num_total_bits % 8;

  /* form a byte aligned word (write_bits), by concatenating any held bits 形成一个字节对齐的字(write_bits)，通过连接任何保留的位与新位，丢弃将形成next_held_bits的位。
   * with the new bits, discarding the bits that will form the next_held_bits.
   * eg: H = held bits, V = n new bits        /---- next_held_bits
   * len(H)=7, len(V)=1: ... ---- HHHH HHHV . 0000 0000, next_num_held_bits=0
   * len(H)=7, len(V)=2: ... ---- HHHH HHHV . V000 0000, next_num_held_bits=1
   * if total_bits < 8, the value of v_ is not used */
  UChar next_held_bits = uiBits << (8 - next_num_held_bits);

  if (!(num_total_bits >> 3))
  {
    /* insufficient bits accumulated to write out, append new_held_bits to 写入的字节数不够，将new_held_bits追加到当前held_bits 
     * current held_bits */
    /* NB, this requires that v only contains 0 in bit positions {31..n} */
    m_held_bits |= next_held_bits;
    m_num_held_bits = next_num_held_bits;
    return;
  }

  /* topword serves to justify held_bits to align with the msb of uiBits */
  UInt topword = (uiNumberOfBits - next_num_held_bits) & ~((1 << 3) -1);
  UInt write_bits = (m_held_bits << topword) | (uiBits >> next_num_held_bits);

  switch (num_total_bits >> 3)
  {
  case 4: m_fifo.push_back(write_bits >> 24);
  case 3: m_fifo.push_back(write_bits >> 16);
  case 2: m_fifo.push_back(write_bits >> 8);
  case 1: m_fifo.push_back(write_bits);
  }

  m_held_bits = next_held_bits;
  m_num_held_bits = next_num_held_bits;
}

Void TComOutputBitstream::writeAlignOne()
{
  UInt num_bits = getNumBitsUntilByteAligned();
  write((1 << num_bits) - 1, num_bits);
  return;
}

Void TComOutputBitstream::writeAlignZero()
{
  if (0 == m_num_held_bits)
  {
    return;
  }
  m_fifo.push_back(m_held_bits);
  m_held_bits = 0;
  m_num_held_bits = 0;
}

/**
 - add substream to the end of the current bitstream 在当前位流的末尾添加子流
 .
 \param  pcSubstream  substream to be added
 */
Void   TComOutputBitstream::addSubstream( TComOutputBitstream* pcSubstream )
{
  UInt uiNumBits = pcSubstream->getNumberOfWrittenBits();

  const vector<uint8_t>& rbsp = pcSubstream->getFIFO();
  for (vector<uint8_t>::const_iterator it = rbsp.begin(); it != rbsp.end();)
  {
    write(*it++, 8);
  }
  if (uiNumBits&0x7)
  {
    write(pcSubstream->getHeldBits()>>(8-(uiNumBits&0x7)), uiNumBits&0x7);
  }
}

Void TComOutputBitstream::writeByteAlignment()
{
  write( 1, 1);
  writeAlignZero();
}

Int TComOutputBitstream::countStartCodeEmulations()
{
  UInt cnt = 0;
  vector<uint8_t>& rbsp   = getFIFO();
  for (vector<uint8_t>::iterator it = rbsp.begin(); it != rbsp.end();)
  {
    vector<uint8_t>::iterator found = it;
    do
    {
      // find the next emulated 00 00 {00,01,02,03} 找到下一个模拟对象
      // NB, end()-1, prevents finding a trailing two byte sequence 注意，end()-1防止查找尾随的两个字节序列
      found = search_n(found, rbsp.end()-1, 2, 0);
      found++;
      // if not found, found == end, otherwise found = second zero byte 如果没有找到，found == end，否则found =第二个零字节
      if (found == rbsp.end())
      {
        break;
      }
      if (*(++found) <= 3)
      {
        break;
      }
    } while (true);
    it = found;
    if (found != rbsp.end())
    {
      cnt++;
    }
  }
  return cnt;
}

/**
 * read uiNumberOfBits from bitstream without updating the bitstream
 * state, storing the result in ruiBits. 从bitstream读取uiNumberOfBits而不更新bitstream状态，将结果存储在ruiBits中。
 *
 * If reading uiNumberOfBits would overrun the bitstream buffer,
 * the bitstream is effectively padded with sufficient zero-bits to
 * avoid the overrun. 如果读取uiNumberOfBits会溢出位流缓冲区，位流会有效地填充足够的零位以避免溢出
 */
Void TComInputBitstream::pseudoRead ( UInt uiNumberOfBits, UInt& ruiBits )
{
  UInt saved_num_held_bits = m_num_held_bits;
  UChar saved_held_bits = m_held_bits;
  UInt saved_fifo_idx = m_fifo_idx;

  UInt num_bits_to_read = min(uiNumberOfBits, getNumBitsLeft());
  read(num_bits_to_read, ruiBits);
  ruiBits <<= (uiNumberOfBits - num_bits_to_read);

  m_fifo_idx = saved_fifo_idx;
  m_held_bits = saved_held_bits;
  m_num_held_bits = saved_num_held_bits;
}


Void TComInputBitstream::read (UInt uiNumberOfBits, UInt& ruiBits)
{
  assert( uiNumberOfBits <= 32 );

  m_numBitsRead += uiNumberOfBits;

  /* NB, bits are extracted from the MSB of each byte.  NB，从每个字节的MSB中提取位。*/
  UInt retval = 0;
  if (uiNumberOfBits <= m_num_held_bits)
  {
    /* n=1, len(H)=7:   -VHH HHHH, shift_down=6, mask=0xfe
     * n=3, len(H)=7:   -VVV HHHH, shift_down=4, mask=0xf8
     */
    retval = m_held_bits >> (m_num_held_bits - uiNumberOfBits);
    retval &= ~(0xff << uiNumberOfBits);
    m_num_held_bits -= uiNumberOfBits;
    ruiBits = retval;
    return;
  }

  /* all num_held_bits will go into retval 所有num_held_bits将进入retval
   *   => need to mask leftover bits from previous extractions
   *   => align retval with top of extracted word  需要掩盖从以前提取的剩余位对齐retval与提取字的顶部*/ 
  /* n=5, len(H)=3: ---- -VVV, mask=0x07, shift_up=5-3=2,
   * n=9, len(H)=3: ---- -VVV, mask=0x07, shift_up=9-3=6 */
  uiNumberOfBits -= m_num_held_bits;
  retval = m_held_bits & ~(0xff << m_num_held_bits);
  retval <<= uiNumberOfBits;

  /* number of whole bytes that need to be loaded to form retval 需要加载以形成检索的整个字节数 */ 
  /* n=32, len(H)=0, load 4bytes, shift_down=0
   * n=32, len(H)=1, load 4bytes, shift_down=1
   * n=31, len(H)=1, load 4bytes, shift_down=1+1
   * n=8,  len(H)=0, load 1byte,  shift_down=0
   * n=8,  len(H)=3, load 1byte,  shift_down=3
   * n=5,  len(H)=1, load 1byte,  shift_down=1+3
   */
  UInt aligned_word = 0;
  UInt num_bytes_to_load = (uiNumberOfBits - 1) >> 3;
  assert(m_fifo_idx + num_bytes_to_load < m_fifo.size());

  switch (num_bytes_to_load)
  {
  case 3: aligned_word  = m_fifo[m_fifo_idx++] << 24;
  case 2: aligned_word |= m_fifo[m_fifo_idx++] << 16;
  case 1: aligned_word |= m_fifo[m_fifo_idx++] <<  8;
  case 0: aligned_word |= m_fifo[m_fifo_idx++];
  }

  /* resolve remainder bits  解析剩余位*/
  UInt next_num_held_bits = (32 - uiNumberOfBits) % 8;

  /* copy required part of aligned_word into retval  将aligned_word所需的部分复制到retval中*/
  retval |= aligned_word >> next_num_held_bits;

  /* store held bits  存储位*/
  m_num_held_bits = next_num_held_bits;
  m_held_bits = aligned_word;

  ruiBits = retval;
}

/**
 * insert the contents of the bytealigned (and flushed) bitstream src 在字节位置pos处插入字节对齐(并刷新)位流SRC的内容
 * into this at byte position pos.
 */
Void TComOutputBitstream::insertAt(const TComOutputBitstream& src, UInt pos)
{
  UInt src_bits = src.getNumberOfWrittenBits();
  assert(0 == src_bits % 8);

  vector<uint8_t>::iterator at = m_fifo.begin() + pos;
  m_fifo.insert(at, src.m_fifo.begin(), src.m_fifo.end());
}

UInt TComInputBitstream::readOutTrailingBits ()
{
  UInt count=0;
  UInt uiBits = 0;

  while ( ( getNumBitsLeft() > 0 ) && (getNumBitsUntilByteAligned()!=0) )
  {
    count++;
    read ( 1, uiBits );
  }
  return count;
}
//
//TComOutputBitstream& TComOutputBitstream::operator= (const TComOutputBitstream& src)
//{
//  vector<uint8_t>::iterator at = m_fifo.begin();
//  m_fifo.insert(at, src.m_fifo.begin(), src.m_fifo.end());
//
//  m_num_held_bits             = src.m_num_held_bits;
//  m_held_bits                 = src.m_held_bits;
//
//  return *this;
//}

/**
 Extract substream from the current bitstream. 从当前位流中提取子流。

 \param  uiNumBits    number of bits to transfer
 */
TComInputBitstream *TComInputBitstream::extractSubstream( UInt uiNumBits )
{
  UInt uiNumBytes = uiNumBits/8;
  TComInputBitstream *pResult = new TComInputBitstream;

  std::vector<uint8_t> &buf = pResult->getFifo();
  buf.reserve((uiNumBits+7)>>3);

  if (m_num_held_bits == 0)
  {
    std::size_t currentOutputBufferSize=buf.size();
    const UInt uiNumBytesToReadFromFifo = std::min<UInt>(uiNumBytes, (UInt)m_fifo.size() - m_fifo_idx);
    buf.resize(currentOutputBufferSize+uiNumBytes);
    memcpy(&(buf[currentOutputBufferSize]), &(m_fifo[m_fifo_idx]), uiNumBytesToReadFromFifo); m_fifo_idx+=uiNumBytesToReadFromFifo;
    if (uiNumBytesToReadFromFifo != uiNumBytes)
    {
      memset(&(buf[currentOutputBufferSize+uiNumBytesToReadFromFifo]), 0, uiNumBytes - uiNumBytesToReadFromFifo);
    }
  }
  else
  {
    for (UInt ui = 0; ui < uiNumBytes; ui++)
    {
      UInt uiByte;
      read(8, uiByte);
      buf.push_back(uiByte);
    }
  }
  if (uiNumBits&0x7)
  {
    UInt uiByte = 0;
    read(uiNumBits&0x7, uiByte);
    uiByte <<= 8-(uiNumBits&0x7);
    buf.push_back(uiByte);
  }
  return pResult;
}

UInt TComInputBitstream::readByteAlignment()
{
  UInt code = 0;
  read( 1, code );
  assert(code == 1);

  UInt numBits = getNumBitsUntilByteAligned();
  if(numBits)
  {
    assert(numBits <= getNumBitsLeft());
    read( numBits, code );
    assert(code == 0);
  }
  return numBits+1;
}

//! \}
