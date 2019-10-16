/**
* \file GPP_WritePacket.h
* \brief the file manages the functions used to create the packet to output
* \author Muoio Salvatore
*/

#ifndef __GPP_WRITE_PACKET_H__
#define __GPP_WRITE_PACKET_H__

/**
* \enum  e_WritePacket
* \brief ennumeration of which packet is sent out
*/
typedef enum
{
  EVC_Packet_to_Write = 1, /*!< EVC packet to write*/
  STM_Packet_to_Write = 2, /*!< STM packet to write*/
}e_WritePacket;

/**
* \fn void GPP_CreatePacket2Write(UINT8 aNidStm)
* \brief The function reads from the shared memory the data to write and call
*        the engine for add the signal in the output queue.
* \param aNidStm NidStim
*/
void GPP_CreatePacket2Write(UINT8 aNidStm);
/**
* \fn void GPP_BuildPacket2Queue(UINT8 aNidStm)
* \brief The function adds in the output queue the packet to write
* \param aNidStm NidStim
*/
void GPP_BuildPacket2Queue(UINT8 aNidStm);








#endif
