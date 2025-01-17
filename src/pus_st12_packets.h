/*! \file pus_st12_packets.h
 *  \brief PUS service ST[12] On-board Monitoring
 *
 *  This module provides the functions to generate packets of the service ST[12],
 *  as well as getter/setter and checking functions.
 *
 *  Only a subset of the standard requests and reports are implemented.
 *
 *  - TC[12,1] enable Parameter Monitoring Definitions
 *  - TC[12,2] disable Parameter Monitoring Definitions
 *  - TM[12,12] check Transition Report
 *  - TC[12,15] enable Parameter Monitoring
 *  - TC[12,16] disable Parameter Monitoring
 *
 *  H2020 ESROCOS Project
 *  \author Company: GMV Aerospace & Defence S.A.U.
 *  \copyright Licence: GPLv2
 */

#ifndef PUS_ST12_PACKETS_H
#define PUS_ST12_PACKETS_H

#include "pus_types.h"
#ifdef PUS_CONFIGURE_ST12_ENABLED

#include "pus_apid.h"
#include "pus_error.h"
#include "pus_threads.h"
#include "pus_packet.h"

#ifdef  __cplusplus
extern "C" {
#endif




//! Build the common part of the TC[12] request
/*! Builds a TC[12,X] packet in the packet passed as parameter.
 *  \param[out] outTm Packet variable to build the TM
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TM sequence counter for the source APID
 *  \param[in] subtype The TM service subtype to be created
 */
pusError_t pus_tc_12_X_createDefaultPacket(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSubservice_t subtype);

/*! Builds a TC[12,1] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \param[in] pmon PMON definition identifier
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_12_1_createEnableParameterMonitoringDefinitions(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt12PmonId_t* pmon, int count);

/*! Builds a TC[12,2] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \param[in] pmon PMON definition identifier
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_12_2_createDisableParameterMonitoringDefinitions(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt12PmonId_t* pmon, int count);

//pusError_t pus_tm_12_12_createCheckTransitionReport(pusPacket_t* outTm, pusApidInfo_t* apid);

/*! Builds a TC[12,15] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_12_15_createEnableParameterMonitoring(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount);

/*! Builds a TC[12,16] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_12_16_createDisableParameterMonitoring(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount);

//! Set pmonId to the data field of TC[12,1] and TC[12,2]
pusError_t pus_tc_12_1_2_setPmonId(pusPacket_t* outTc, pusSt12PmonId_t pmon, int count);

//! Get pmonId from the data field of TC[12,1] and TC[12,2]
pusError_t pus_tc_12_1_2_getPmonId(pusSt12PmonId_t* pmon, const pusPacket_t* tcPacket);

//! Set nCount to the data field of TC[12,1] and TC[12,2]
pusError_t pus_tc_12_1_2_setNCount(pusPacket_t* outTc, const int count);

//! Get nCount from the data field of TC[12,1] and TC[12,2]
int pus_tc_12_1_2_getNCount(const pusPacket_t* tcPacket);


//! Check that a packet is of a PUS ST[12] kind
/*! \param[in] packet The PUS packet
 *  \param[in] expectedSubtype Check that the TM has this subtype; use pusSubtype_NONE to check for all TC types in ST[12]
 *  \param[in] function Function name to write as error information (use with the macro \ref PUS_EXPECT_ST12 to include the caller function's name)
 *  \return If valid TC, PUS_NO_ERROR; otherwise, an error code
 */
pusError_t pus_expectSt12Tc(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function);

//! Helper macro for pus_expectSt12; adds function name
#define PUS_EXPECT_ST12TC(packet, subtype) pus_expectSt12Tc((packet), (subtype), __func__)

#ifdef  __cplusplus
}
#endif

#endif

#endif
