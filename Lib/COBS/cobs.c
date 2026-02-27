/**
  * @file cobs.c
  * @brief Consistent Overhead Byte Stuffing (COBS) encoding routines
  *
  * @author
  *   Riccardo Antonello (riccardo.antonello@unipd.it)
  *   Dept. of Information Engineering, University of Padova
  *
  * @date 15 December 2017
  */

#include "simstruc.h"


/*	COBS encoding routine  */
#define FinishBlock(X) (*code_ptr = (X), code_ptr = dst++, code = 0x01)

void cobsEncode(const uint8_T *ptr, int_T length, uint8_T *dst)
{
    const uint8_T *end = ptr + length;
    uint8_T *code_ptr = dst++;
    uint8_T code = 0x01;
    
    while (ptr < end)
    {
        if (*ptr == 0)
            FinishBlock(code);
        else
        {
            *dst++ = *ptr;
            if (++code == 0xFF)
                FinishBlock(code);
        }
        ptr++;
    }
    
    FinishBlock(code);
}


/*	COBS decoding routine */
void cobsDecode(const uint8_T *ptr, int length, uint8_T *dst)
{
  const uint8_T *end = ptr + length;
  int i, code;

  while (ptr < end)
  {
    code = *ptr++;

    for (i = 1; i < code; i++)
      *dst++ = *ptr++;

    if (code < 0xFF)
      *dst++ = 0;
  }
}
