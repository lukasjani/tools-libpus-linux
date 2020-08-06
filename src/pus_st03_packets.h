/*! \file pus_st03_packets.h
 *  \brief PUS service ST[03] Housekeeping.
 *
 *  This module provides the functions to generate packets of the service ST[03],
 *  as well as getter/setter and checking functions.
 *
 *  Only a subset of the standard requests and reports are implemented.
 *
 *  - Housekeeping
 *    + TM[3,25] Housekeeping parameter report
 *
 *  H2020 ESROCOS Project
 *  \author Company: GMV Aerospace & Defence S.A.U.
 *  \copyright Licence: GPLv2
 */

#ifndef PUS_ST03_PACKETS_H
#define PUS_ST03_PACKETS_H

#include "pus_types.h"
#ifdef PUS_CONFIGURE_ST03_ENABLED

#include "pus_apid.h"
#include "pus_error.h"
#include "pus_housekeeping.h"
#include "pus_parameter_management.h"
#include "pus_threads.h"
#include "pus_packet.h"
#include <string.h>



#ifdef  __cplusplus
extern "C" {
#endif

//
// Packet creation
//

//! Build the default housekeeping TC Packet
/*! Builds a TM[3_X] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCount TC sequence counter for the source APID
 *  \param[in] param Array of Parameter IDs to be added in parameter report
 *  \param[in] count Number of Parameter IDs
 *  \param[in] subtype Subtype which invoked this function
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_3_X_createDefaultTCPacket(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt20OnBoardParamId_t* param, int count, pusSubservice_t subtype);

//! Build the default housekeeping TC Packet
/*! Builds a TM[3_5] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCount TC sequence counter for the source APID
 *  \param[in] param Array of Parameter IDs to be added in parameter report
 *  \param[in] count Number of Parameter IDs
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_3_5_createEnableHousekeepingRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt20OnBoardParamId_t* param, int count);

//! Build the default housekeeping TC Packet
/*! Builds a TM[3_6] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCount TC sequence counter for the source APID
 *  \param[in] param Array of Parameter IDs to be added in parameter report
 *  \param[in] count Number of Parameter IDs
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_3_6_createDisableHousekeepingRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt20OnBoardParamId_t* param, int count);

//! Build the default housekeeping TC Packet
/*! Builds a TM[3_7] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCount TC sequence counter for the source APID
 *  \param[in] param Array of Parameter IDs to be added in parameter report
 *  \param[in] count Number of Parameter IDs
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_3_7_createEnableDiagnosticRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt20OnBoardParamId_t* param, int count);

//! Build the default housekeeping TC Packet
/*! Builds a TM[3_5] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCount TC sequence counter for the source APID
 *  \param[in] param Array of Parameter IDs to be added in parameter report
 *  \param[in] count Number of Parameter IDs
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_3_8_createDisableDiagnosticRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt20OnBoardParamId_t* param, int count);

//! Build a housekeeping parameter report
/*! Builds a TM[3,25] packet in the packet passed as parameter.
 *  \param[out] outTm Packet variable to build the TM
 *  \param[in] apid APID of the process sending the report
 *  \param[in] count TM sequence counter for the source APID
 *  \param[in] reportId The report ID
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tm_3_25_createHousekeepingReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt03HousekeepingReportId_t reportId, pusApid_t destination);

//! Build a housekeeping parameter report
/*! Builds a TM[3,25] packet in the packet passed as parameter.
 *  \param[out] outTm Packet variable to build the TM
 *  \param[in] apid APID of the process sending the report
 *  \param[in] count TM sequence counter for the source APID
 *  \param[in] reportId The report ID
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tm_3_26_createDiagnosticReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt03HousekeepingReportId_t reportId, pusApid_t destination);


//
// Getters and setters for report fields
//


//! Setter for the TC[3,X] report ID field
pusError_t pus_tc_3_X_setParamId(pusPacket_t* outTc, pusSt03HousekeepingReportId_t reportId, int index);

//! Getter for the TC[3,X] report ID field
pusError_t pus_tc_3_X_getParamId(pusPacket_t* outTc, int index, pusSt03HousekeepingReportId_t* reportId);

//! Getter for the TC[3,X] report ID field
pusError_t pus_tc_3_X_getNumParameters(const pusPacket_t* outTc, int* outNumParams);

//! Setter for the TC[3,X] report ID field
pusError_t pus_tc_3_X_setNumParameters(pusPacket_t* outTc, int inNumParams);

//! Getter for the TM[3,X] report ID field
pusSt03HousekeepingReportId_t pus_tm_3_X_getReportId(const pusPacket_t* tm);

//! Setter for the TM[3,25] report ID field
pusError_t pus_tm_3_X_setReportId(pusPacket_t* outTm, pusSt03HousekeepingReportId_t reportId);

//! Set the value of one parameter in the report (to be casted to the correct type)
/*! \param[out] tm The TM packet
 *  \param[in] index The index of the parameter to be returned; must be at most \ref pus_ST03_MAX_REPORT_LENGTH
 *  \param[in] inValue Input parameter value as \ref pusStoredParam_t, to be casted to the correct type
 */
pusError_t pus_tm_3_X_setParameterValue(pusPacket_t* tm, int index, pusStoredParam_t inValue);

//! Get the value of one parameter in the report (to be casted to the correct type)
/*! \param[in] tm The TM packet
 *  \param[in] index The index of the parameter to be returned; must be at most \ref pus_ST03_MAX_REPORT_LENGTH
 *  \param[out] outValue Outputs the parameter value as \ref pusInternalParam_t, to be casted to the correct type
 */
pusError_t pus_tm_3_X_getParameterValue(const pusPacket_t* tm, int index, pusStoredParam_t* outValue);

//! Get the number of parameters in the report
/*! \param[in] tm The TM packet
 *  \param[out] outNumParams Outputs the number of parameters
 */
pusError_t pus_tm_3_X_getNumParameters(const pusPacket_t* tm, int* outNumParams);

//! Set the number of parameters in the report
/*! \param[out] tm The TM packet
 *  \param[in] inNumParams Input for the number of parameters
 */
pusError_t pus_tm_3_X_setNumParameters(pusPacket_t* tm, int outNumParams);

//
// Parameter checking
//

//! Check that a packet is of a PUS ST[03] kind
/*! \param[in] packet The PUS packet
 *  \param[in] expectedSubtype Check that the TC has this subtype; use pusSubtype_NONE to check for all TC types in ST[03]
 *  \param[in] function Function name to write as error information (use with the macro \ref PUS_EXPECT_ST03TC to include the caller function's name)
 *  \return If valid TC, PUS_NO_ERROR; otherwise, an error code
 */
pusError_t pus_expectSt03Tc(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function);

//! Check that a packet is of a PUS ST[03] kind
/*! \param[in] packet The PUS packet
 *  \param[in] expectedSubtype Check that the TM has this subtype; use pusSubtype_NONE to check for all TM types in ST[03]
 *  \param[in] function Function name to write as error information (use with the macro \ref PUS_EXPECT_ST03TM to include the caller function's name)
 *  \return If valid TM, PUS_NO_ERROR; otherwise, an error code
 */
pusError_t pus_expectSt03Tm(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function);

//! Helper macro for pus_expectSt03Tc; adds function name
#define PUS_EXPECT_ST03TC(packet, subtype) pus_expectSt03Tc((packet), (subtype), __func__)

//! Helper macro for pus_expectSt03Tm; adds function name
#define PUS_EXPECT_ST03TM(packet, subtype) pus_expectSt03Tm((packet), (subtype), __func__)

#ifdef  __cplusplus
}
#endif

#endif //service enabled

#endif // PUS_ST03_PACKETS_H
