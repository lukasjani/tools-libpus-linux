/*! \file pus_st13_packets.h
 *  \brief PUS service ST[13] Large File Transfer.
 *
 *  This module contains the functions to manage the transfer of large files
 *
 *  All of the standard requests and reports are implemented.
 *
 *  H2020 ESROCOS Project
 *  \author Company: GMV Aerospace & Defence S.A.U.
 *  \copyright Licence: GPLv2
 */

#ifndef PUS_ST13_PACKETS_H
#define PUS_ST13_PACKETS_H

#include "pus_types.h"
#ifdef PUS_CONFIGURE_ST13_ENABLED

#include "pus_apid.h"
#include "pus_error.h"
#include "pus_threads.h"
#include "pus_packet.h"
#include "pus_time.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef  __cplusplus
extern "C" {
#endif

/*! Splits a char array in equal parts and stores these in TM packets in the outgoing array
*  \param[in] apid APID of the process sending the report
*  \param[in] sequenceCountcount TC sequence counter for the source APID
*  \param[in] subtype Subtype of the service
*  \param[in] destination APID of the destination process
*  \param[in] content Char array ("Large File"), which gets splitted
*  \param[in] length Length of the char array
*  \param[in] maxPartSize Maximum size of the parts
*  \param[in] transactionID Identifier for the large file transfer
*  \param[in] mutex Mutex for array counter access
*  \return Error code (PUS_NO_ERROR if success)
*/
//pusError_t pus_tm_13_sendLargeFile(pusApid_t apid, pusSequenceCount_t sequenceCount, pusApid_t destination,
//    char* content, uint64_t length, uint64_t transactionID, pusMutex_t* mutex);

/*! Splits a char array in equal parts and stores these in TC packets in the outgoing array
*  \param[in] apid APID of the process sending the report
*  \param[in] sequenceCountcount TC sequence counter for the source APID
*  \param[in] subtype Subtype of the service
*  \param[in] destination APID of the destination process
*  \param[in] content Char array ("Large File"), which gets splitted
*  \param[in] length Length of the char array
*  \param[in] maxPartSize Maximum size of the parts
*  \param[in] transactionID Identifier for the large file transfer
*  \param[in] mutex Mutex for array counter access
*  \return Error code (PUS_NO_ERROR if success)
*/
//pusError_t pus_tc_13_sendLargeFile(pusApid_t apid, pusSequenceCount_t sequenceCount,
//    char* content, uint64_t length, uint64_t transactionID, pusMutex_t* mutex);

/*! Builds a default TM[13,X] packet in the packet passed as parameter.
*  \param[out] outTm Packet variable to build the TM
*  \param[in] timeout Maximum time between two packet parts
*  \param[in] mutex Mutex for arry counter access
*/
//pusError_t pus_tm_13_receiveLargeFile(char* output, pusTime_t timeout, pusMutex_t* mutex);

/*! Builds a default TM[13,X] packet in the packet passed as parameter.
*  \param[out] outTm Packet variable to build the TM
*  \param[in] timeout Maximum time between two packet parts
*  \param[in] mutex Mutex for arry counter access
*/
//pusError_t pus_tc_13_receiveLargeFile(char* output, pusTime_t timeout, pusMutex_t* mutex);

/*! Builds a default TM[13,X] packet in the packet passed as parameter.
*  \param[out] outTm Packet variable to build the TM
*  \param[in] apid APID of the process sending the report
*  \param[in] sequenceCountcount TC sequence counter for the source APID
*  \param[in] subtype Subtype of the service
*  \param[in] destination APID of the destination process
*  \return Error code (PUS_NO_ERROR if success)
*/
pusError_t pus_tm_13_X_createDefaultTMPacket(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSubservice_t subtype, pusApid_t destination);

/*! Builds a default TC[13,X] packet in the packet passed as parameter.
*  \param[out] outTc Packet variable to build the TC
*  \param[in] apid APID of the process sending the report
*  \param[in] sequenceCountcount TC sequence counter for the source APID
*  \param[in] subtype Subtype of the service
*  \return Error code (PUS_NO_ERROR if success)
*/
pusError_t pus_tc_13_X_createDefaultTCPacket(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSubservice_t subtype);


/*! Builds a TM[13,1] packet in the packet passed as parameter.
*  \param[out] outTm Packet variable to build the TM
*  \param[in] apid APID of the process sending the reports
*  \param[in] sequenceCountcount TC sequence counter for the source APID
*  \param[in] transactionID Identifier of the large file
*  \param[in] part_seq_number Identifier of the part of the large file
*  \param[in] content Part of the large file
*  \return Error code (PUS_NO_ERROR if success)
*/
pusError_t pus_tm_13_1_createFirstDownlinkPartReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusApid_t destination,
    int transactionID, char* content);

/*! Builds a TM[13,2] packet in the packet passed as parameter.
*  \param[out] outTm Packet variable to build the TM
*  \param[in] apid APID of the process sending the report
*  \param[in] sequenceCountcount TC sequence counter for the source APID
*  \param[in] transactionID Identifier of the large file
*  \param[in] part_seq_number Identifier of the part of the large file
*  \param[in] content Part of the large file
*  \return Error code (PUS_NO_ERROR if success)
*/
pusError_t pus_tm_13_2_createIntermediateDownlinkPartReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusApid_t destination,
    int transactionID, int part_seq_number, char* content);

/*! Builds a TM[13,3] packet in the packet passed as parameter.
*  \param[out] outTm Packet variable to build the TM
*  \param[in] apid APID of the process sending the report
*  \param[in] sequenceCountcount TC sequence counter for the source APID
*  \param[in] transactionID Identifier of the large file
*  \param[in] part_seq_number Identifier of the part of the large file
*  \param[in] content Part of the large file
*  \return Error code (PUS_NO_ERROR if success)
*/
pusError_t pus_tm_13_3_createLastDownlinkPartReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusApid_t destination,
    int transactionID, int part_seq_number, char* content);

/*! Builds a TC[13,9] packet in the packet passed as parameter.
*  \param[out] outTc Packet variable to build the TC
*  \param[in] apid APID of the process sending the report
*  \param[in] sequenceCountcount TC sequence counter for the source APID
*  \param[in] transactionID Identifier of the large file
*  \param[in] part_seq_number Identifier of the part of the large file
*  \param[in] content Part of the large file
*  \return Error code (PUS_NO_ERROR if success)
*/
pusError_t pus_tc_13_9_createFirstUplinkPart(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
    int transactionID, char* content);

/*! Builds a TC[13,10] packet in the packet passed as parameter.
*  \param[out] outTc Packet variable to build the TC
*  \param[in] apid APID of the process sending the report
*  \param[in] sequenceCountcount TC sequence counter for the source APID
*  \param[in] transactionID Identifier of the large file
*  \param[in] part_seq_number Identifier of the part of the large file
*  \param[in] content Part of the large file
*  \return Error code (PUS_NO_ERROR if success)
*/
pusError_t pus_tc_13_10_createIntermediateUplinkPart(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
    int transactionID, int part_seq_number, char* content);

/*! Builds a TC[13,11] packet in the packet passed as parameter.
*  \param[out] outTc Packet variable to build the TC
*  \param[in] apid APID of the process sending the report
*  \param[in] sequenceCountcount TC sequence counter for the source APID
*  \param[in] transactionID Identifier of the large file
*  \param[in] part_seq_number Identifier of the part of the large file
*  \param[in] content Part of the large file
*  \return Error code (PUS_NO_ERROR if success)
*/
pusError_t pus_tc_13_11_createLastUplinkPart(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
    int transactionID, int part_seq_number, char* content);

/*! Builds a TM[13,16] packet in the packet passed as parameter.
*  \param[out] outTm Packet variable to build the TM
*  \param[in] apid APID of the process sending the report
*  \param[in] sequenceCountcount TC sequence counter for the source APID
*  \param[in] transactionID Identifier of the large file
*  \param[in] failureReason Reason for the abortion
*  \return Error code (PUS_NO_ERROR if success)
*/
pusError_t pus_tm_13_16_createLargePacketUplinkAbortionReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusApid_t destination,
    int transactionID, int failureReason);

/*! Splits a char array in equal parts and saves them
*  \param[out] split_content Two dimensional char array, where the parts are saved
*  \param[in] content Char array to be split
*  \param[in] seqNum Offset in the array
*  \return Error code (PUS_NO_ERROR if success)
*/
pusError_t pus_tc_tm_13_splitContent(char* split_content, char* content, int seqNum);

/*! Merges the data of a packet with a char array
*  \param[out] content Char array, on which the data gets appended
*  \param[in] packet Packet, whiches data gets appended
*  \param[in] length Current length of the destination array
*  \return Error code (PUS_NO_ERROR if success)
*/
pusError_t pus_tc_tm_13_mergeContent(char* content, pusPacket_t* packet, int* length);


//
//Getter & Setter
//

//! Setter for the transactionID of a TC[13,X] packet
pusError_t pus_tc_tm_13_X_setTransactionID(pusPacket_t* outTc, const int transactionID);

//! Getter for the transactionID of a TC[13,X] packet
pusError_t pus_tc_tm_13_X_getTransactionID(int* transactionID, const pusPacket_t* outTc);

//! Setter for the sequence number of a TC[13,X] packet
pusError_t pus_tc_tm_13_X_setPartSeqNumber(pusPacket_t* outTc, const int part_seq_number);

//! Getter for the sequence number of a TC[13,X] packet
pusError_t pus_tc_tm_13_X_getPartSeqNumber(int* part_seq_number, const pusPacket_t* outTc);

//! Setter for the content of a TC[13,X] packet
pusError_t pus_tc_tm_13_X_setContent(pusPacket_t* outTc, const char* content);

//! Getter for the content of a TC[13,X] packet
pusError_t pus_tc_tm_13_X_getContent(char* content, const pusPacket_t* outTc);

//! Setter for the failure reason of a TM[13,16] packet
pusError_t pus_tm_13_16_setFailureReason(pusPacket_t* outTm, const int failureReason);

//! Getter for the failure reason of a TM[13,16] packet
pusError_t pus_tm_13_16_getFailureReason(int* failureReason, const pusPacket_t* outTm);

//
// Parameter checking
//

//! Check that a packet is of a PUS ST[13] kind
/*! \param[in] packet The PUS packet
 *  \param[in] expectedSubtype Check that the TC has this subtype; use pusSubtype_NONE to check for all TC types in ST[23]
 *  \param[in] function Function name to write as error information (use with the macro \ref PUS_EXPECT_ST23 to include the caller function's name)
 *  \return If valid TC, PUS_NO_ERROR; otherwise, an error code
 */
pusError_t pus_expectSt13Tc(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function);

//! Check that a packet is of a PUS ST[13] kind
/*! \param[in] packet The PUS packet
 *  \param[in] expectedSubtype Check that the TM has this subtype; use pusSubtype_NONE to check for all TM types in ST[23]
 *  \param[in] function Function name to write as error information (use with the macro \ref PUS_EXPECT_ST23 to include the caller function's name)
 *  \return If valid TM, PUS_NO_ERROR; otherwise, an error code
 */
pusError_t pus_expectSt13Tm(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function);

//! Helper macro for pus_expectSt20Tc; adds function name
#define PUS_EXPECT_ST13TC(packet, subtype) pus_expectSt13Tc((packet), (subtype), __func__)

//! Helper macro for pus_expectSt20Tm; adds function name
#define PUS_EXPECT_ST13TM(packet, subtype) pus_expectSt13Tm((packet), (subtype), __func__)


#ifdef  __cplusplus
}
#endif

#endif

#endif // PUS_ST13_PACKETS_H
