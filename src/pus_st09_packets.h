/*! \file pus_st09_packets.h
 *  \brief PUS service ST[09] Time management
 *
 *  This module provides the functions to generate the TC and TM of the service ST[09],
 *  as well as getter/setter and checking functions.
 *
 *	Implemented subservices:
 *  - TC[9,1] Set Time Report Rate.
 *	- TC[9,2] CUC Time Report.
 *
 *  H2020 ESROCOS Project
 *  \author Company: GMV Aerospace & Defence S.A.U.
 *  \copyright Licence: GPLv2
 */


#ifndef PUS_ST09_PACKETS_H
#define PUS_ST09_PACKETS_H

#include "pus_types.h"
#ifdef PUS_CONFIGURE_ST09_ENABLED

#include "pus_time.h"
#include "pus_error.h"
#include "pus_packet.h"
#include "pus_apid.h"

#ifdef  __cplusplus
extern "C" {
#endif


extern pusSt09ExponentialRate_t pus_time_reportGenerationExponentialRate;


//! Build a Set time report rate
/*! Builds a TC[9,1] packet in the packet passed as parameter.
 *  \param[out] outTm Packet variable to build the TC
 *  \param[in] apid APID of the process that send the packet
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_9_1_createSetTimeReportRate(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt09ExponentialRate_t expRate);

//! Build a CUC time report
/*! Builds a TM[9,2] packet in the packet passed as parameter.
 *  \param[out] outTm Packet variable to build the TM
 *  \param[in] apid APID of the process sending the report
 *  \param[in] destination Destination of the packet
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tm_9_2_createCucTimeReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusTime_t* time, pusSt09ExponentialRate_t* expRate);


//! Set the exponential rate to a TC[9,1] packet
pusError_t pus_tc_9_1_setExponentialRate(pusPacket_t* tc, pusSt09ExponentialRate_t expRate);

//! Get the exponential rate from a TC[9,1] packet
pusError_t pus_tc_9_1_getExponentialRate(pusSt09ExponentialRate_t* expRate, const pusPacket_t* tc);

//! Set the data field of a TM[9,2] and TM[9,3] packet
pusError_t pus_tm_9_2_3_setDataField(pusPacket_t* tm, pusTime_t* time, pusSt09ExponentialRate_t* expRate);

//! Get the data field of a TM[9,2] and TM[9,3] packet
pusError_t pus_tm_9_2_3_getDataField(const pusPacket_t* tm, pusTime_t* time, pusSt09ExponentialRate_t* expRate);


//! Getter for the time report generation rate of the service
pusError_t pus_time_getReportGenerationExponentialRate(pusSt09ExponentialRate_t* expRate);

//! Setter for the time report generation rate of the service
pusError_t pus_time_setReportGenerationExponentialRate(pusSt09ExponentialRate_t expRate);

pusError_t pus_time_getReportGenerationRate(uint64_t* expRate);

//! Check that a packet is of a PUS ST[09] TC kind
/*! \param[in] packet The PUS packet
 *  \param[in] expectedSubtype Check that the TC has this subtype; use pusSubtype_NONE to check for all TC types in ST[09]
 *  \param[in] function Function name to write as error information (use with the macro \ref PUS_EXPECT_ST09TC to include the caller function's name)
 *  \return If valid TC, PUS_NO_ERROR; otherwise, an error code
 */
pusError_t pus_expectSt09Tc(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function);


//! Check that a packet is of a PUS ST[09] TM kind
/*! \param[in] packet The PUS packet
 *  \param[in] expectedSubtype Check that the TM has this subtype; use pusSubtype_NONE to check for all TM types in ST[09]
 *  \param[in] function Function name to write as error information (use with the macro \ref PUS_EXPECT_ST09TM to include the caller function's name)
 *  \return If valid TM, PUS_NO_ERROR; otherwise, an error code
 */
pusError_t pus_expectSt09Tm(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function);


//! Helper macro for pus_expectSt09Tc; adds function name
#define PUS_EXPECT_ST09TC(packet, subtype) pus_expectSt09Tc((packet), (subtype), __func__)

//! Helper macro for pus_expectSt09Tm; adds function name
#define PUS_EXPECT_ST09TM(packet, subtype) pus_expectSt09Tm((packet), (subtype), __func__)


#ifdef  __cplusplus
}
#endif

#endif

#endif
