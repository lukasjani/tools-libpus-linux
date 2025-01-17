/*! \file pus_st11_packets.h
 *  \brief PUS service ST[11] time-based scheduling
 *
 *  This module provides the functions to generate packets of the service ST[11],
 *  as well as getter/setter and checking functions.
 *
 *  Only a subset of the standard requests and reports are implemented.
 *
 *  - TC[11,1] enable the time-based schedule execution function
 *  - TC[11,2] disable the time-based schedule execution function
 *  - TC[11,3] reset the time-based schedule
 *  - TC[11,4] insert activities into the time-based schedule
 *
 *  H2020 ESROCOS Project
 *  \author Company: GMV Aerospace & Defence S.A.U.
 *  \copyright Licence: GPLv2
 */

#ifndef PUS_ST11_PACKETS_H
#define PUS_ST11_PACKETS_H


#include "pus_types.h"
#ifdef PUS_CONFIGURE_ST11_ENABLED

#include "pus_apid.h"
#include "pus_error.h"
#include "pus_threads.h"
#include "pus_packet.h"
#include "pus_packet_reduced.h"

#ifdef  __cplusplus
extern "C" {
#endif

//! Build the common part of the ST[11] TC packets
/*! Builds a TC[11,X] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] subtype The TM service subtype to be created
 */
pusError_t pus_tc_11_X_createDefaultPacket(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSubservice_t subtype);


//! Build a TC to enable the time-based schedule function
/*! Builds a TC[11,1] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 */
pusError_t pus_tc_11_1_createEnableTimeBasedSchedule(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount);


//! Build a TC to disable the time-based schedule function
/*! Builds a TC[11,2] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 */
pusError_t pus_tc_11_2_createDisableTimeBasedSchedule(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount);


//! Build a TC to reset the time-based schedule
/*! Builds a TC[11,3] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 */
pusError_t pus_tc_11_3_createResetTimeBasedSchedule(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount);


//! Build a TC to insert activities into the time-based schedule
/*! Builds a TC[11,4] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 */
pusError_t pus_tc_11_4_createInsertActivityIntoSchedule(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount);

//! Getter for the number of activities in a TC[11,4] packet
int32_t pus_tc_11_4_getNCount(pusPacket_t* outTc);

//! Setter for the number of activities in a TC[11,4] packet
pusError_t pus_tc_11_4_setNCount(pusPacket_t* outTc, int32_t nCount);

//! Set an activity into a TC[11,4]
pusError_t pus_tc_11_4_setActivity(pusPacket_t* outTc, const pusPacket_t* tcAction, const pusTime_t* time);

//! Get the list of activities from a TC[11,4]
pusError_t pus_tc_11_4_getActivities(int32_t* nCount, pusSt11ScheduledActivity_t* activities, const pusPacket_t* outTc);

//! Check that a packet is of a PUS ST[11] kind
/*! \param[in] packet The PUS packet
 *  \param[in] expectedSubtype Check that the TM has this subtype; use pusSubtype_NONE to check for all TC types in ST[11]
 *  \param[in] function Function name to write as error information (use with the macro \ref PUS_EXPECT_ST11 to include the caller function's name)
 *  \return If valid TC, PUS_NO_ERROR; otherwise, an error code
 */
pusError_t pus_expectSt11Tc(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function);

//! Helper macro for pus_expectSt12; adds function name
#define PUS_EXPECT_ST11TC(packet, subtype) pus_expectSt11Tc((packet), (subtype), __func__)


#ifdef  __cplusplus
}
#endif

#endif


#endif
