/**
  * @file cobs.h
  * @brief Header file for cobs.c
  *
  * @author
  *   Riccardo Antonello (riccardo.antonello@unipd.it)
  *   Dept. of Information Engineering, University of Padova
  *
  * @date 15 December 2017
  */

/*  Function prototypes  */
void cobsEncode(const uint8_T *ptr, int_T length, uint8_T *dst);
void cobsDecode(const uint8_T *ptr, int length, uint8_T *dst);