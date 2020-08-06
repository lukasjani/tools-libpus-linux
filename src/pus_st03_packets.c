/*
 * H2020 ESROCOS Project
 * Company: GMV Aerospace & Defence S.A.U.
 * Licence: GPLv2
 */


#include "pus_st03_packets.h"

#ifdef PUS_CONFIGURE_ST03_ENABLED

// Mutex defined in pus_housekeeping.c
extern pusMutex_t* pus_hk_mutex;

// Array for parameters values (all stored in 64 bits)
extern pusStoredParam_t pus_st03_params[];


//
// Packet creation
//
pusError_t pus_tc_3_X_createDefaultTCPacket(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt20OnBoardParamId_t* param, int count, pusSubservice_t subtype)
{
	if (NULL == outTc || param == NULL || count == 0)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (count > 20)
	{
		return PUS_SET_ERROR(PUS_ERROR_LIMIT);
	}

	pus_clearError();
	pus_initTcPacket(outTc);
	pus_setTcDataKind(outTc, PusTcApplicationData_st_3_X_PRESENT);

	// Source information
	pus_setApid(outTc, apid);
	pus_setSequenceCount(outTc, sequenceCount);

	// Data length
	pus_setPacketDataLength(outTc, sizeof(pusPacketData_t));

	// Service identification
	pus_setTcService(outTc, pus_ST03_housekeeping);
	pus_setTcSubtype(outTc, subtype);
	pus_setTcSource(outTc, apid);

	pus_tc_3_X_setNumParameters(outTc, count);
	for (int i = 0; i < count; i++)
	{
		pus_tc_3_X_setParamId(outTc, param[i], i);
	}

	return PUS_GET_ERROR();
}

pusError_t pus_tc_3_5_createEnableHousekeepingRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt20OnBoardParamId_t* param, int count)
{
	return pus_tc_3_X_createDefaultTCPacket(outTc, apid, sequenceCount, param, count, pus_TC_3_5_enableHousekeeping);
}

pusError_t pus_tc_3_6_createDisableHousekeepingRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt20OnBoardParamId_t* param, int count)
{
	return pus_tc_3_X_createDefaultTCPacket(outTc, apid, sequenceCount, param, count, pus_TC_3_6_disableHousekeeping);
}

pusError_t pus_tc_3_7_createEnableDiagnosticRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt20OnBoardParamId_t* param, int count)
{
	return pus_tc_3_X_createDefaultTCPacket(outTc, apid, sequenceCount, param, count, pus_TC_3_7_enableDiagnostic);
}

pusError_t pus_tc_3_8_createDisableDiagnosticRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt20OnBoardParamId_t* param, int count)
{
	return pus_tc_3_X_createDefaultTCPacket(outTc, apid, sequenceCount, param, count, pus_TC_3_8_disableDiagnostic);
}

pusError_t pus_tm_3_25_createHousekeepingReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt03HousekeepingReportId_t reportId, pusApid_t destination)
{
	if (NULL == outTm)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	// Build the TM packet
	pus_clearError();
	pus_initTmPacket(outTm);
	pus_setTmDataKind(outTm, pus_TM_DATA_ST_3_X);

	// Source information
	pus_setApid(outTm, apid);
	pus_setSequenceCount(outTm, sequenceCount);

	// Data length
	pus_setPacketDataLength(outTm, sizeof(pusPacketData_t));

	// Service identification
	pus_setTmService(outTm, pus_ST03_housekeeping);
	pus_setTmSubtype(outTm, pus_TM_3_25_housekeepingReport);

	// Destination
	pus_setTmDestination(outTm, destination);

	// Timestamp
	pus_setTmPacketTimeNow(outTm);

	// Report contents
	pus_tm_3_X_setReportId(outTm, reportId);

	int parameterCount = 0;

	for (int i = 0; i < pus_ST20_PARAM_LIMIT; i++)
	{
		if (pus_st20_paramInfo[i].st03HousekeepingReportEnabled)
		{
			pus_tm_3_X_setParameterValue(outTm, parameterCount, pus_st20_params[i]);
			parameterCount++;
			//printf("Housekeeping - ID: %d, Value: %f\n", i, pus_st20_params[i]);
			//printf("Housekepingset ID: %d, Value: %f\n", i, outTm->data.u.tmData.data.u.st_3_X.parameters.arr[parameterCount-1]);
		}
	}

	pus_tm_3_X_setNumParameters(outTm, parameterCount);

	return PUS_GET_ERROR();
}

pusError_t pus_tm_3_26_createDiagnosticReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt03HousekeepingReportId_t reportId, pusApid_t destination)
{
	if (NULL == outTm)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	// Build the TM packet
	pus_clearError();
	pus_initTmPacket(outTm);
	pus_setTmDataKind(outTm, pus_TM_DATA_ST_3_X);

	// Source information
	pus_setApid(outTm, apid);
	pus_setSequenceCount(outTm, sequenceCount);

	// Data length
	pus_setPacketDataLength(outTm, sizeof(pusPacketData_t));

	// Service identification
	pus_setTmService(outTm, pus_ST03_housekeeping);
	pus_setTmSubtype(outTm, pus_TM_3_26_diagnosticReport);

	// Destination
	pus_setTmDestination(outTm, destination);

	// Timestamp
	pus_setTmPacketTimeNow(outTm);

	// Report contents
	pus_tm_3_X_setReportId(outTm, reportId);

	int parameterCount = 0;

	for (int i = 0; i < pus_ST20_PARAM_LIMIT; i++)
	{
		if (pus_st20_paramInfo[i].st03DiagnosticReportEnabled)
		{
			pus_tm_3_X_setParameterValue(outTm, parameterCount, pus_st20_params[i]);
			parameterCount++;
			//printf("Diagnostic - ID: %d, Value: %f\n", i, pus_st20_params[i]);
			//printf("Diagnsticset ID: %d, Value: %f\n", i, outTm->data.u.tmData.data.u.st_3_X.parameters.arr[parameterCount - 1]);
		}
	}

	pus_tm_3_X_setNumParameters(outTm, parameterCount);

	return PUS_GET_ERROR();
}

//
// Packet field getters and setters
//


pusError_t pus_tc_3_X_setParamId(pusPacket_t* outTc, pusSt03HousekeepingReportId_t reportId, int index)
{
	if (NULL == outTc)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (PUS_NO_ERROR == PUS_EXPECT_ST03TC(outTc, pusSubtype_NONE))
	{
		outTc->data.u.tcData.data.u.st_3_X.arr[index] = reportId;

		return PUS_NO_ERROR;
	}

	return PUS_GET_ERROR();
}

pusError_t pus_tc_3_X_getParamId(pusPacket_t* outTc, int index, pusSt03HousekeepingReportId_t* reportId)
{
	if (NULL == outTc || NULL == reportId)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (PUS_NO_ERROR == PUS_EXPECT_ST03TC(outTc, pusSubtype_NONE))
	{
		*reportId = outTc->data.u.tcData.data.u.st_3_X.arr[index];

		return PUS_NO_ERROR;
	}

	return PUS_GET_ERROR();
}

pusError_t pus_tc_3_X_getNumParameters(const pusPacket_t* outTc, int* outNumParams)
{
	if (NULL == outTc || NULL == outNumParams)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (PUS_NO_ERROR == PUS_EXPECT_ST03TC(outTc, pusSubtype_NONE))
	{
		*outNumParams = outTc->data.u.tcData.data.u.st_3_X.nCount;

		return PUS_NO_ERROR;
	}

	return PUS_GET_ERROR();
}

pusError_t pus_tc_3_X_setNumParameters(pusPacket_t* outTc, int inNumParams)
{
	if (NULL == outTc)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (PUS_NO_ERROR == PUS_EXPECT_ST03TC(outTc, pusSubtype_NONE))
	{
		outTc->data.u.tcData.data.u.st_3_X.nCount = inNumParams;

		return PUS_SET_ERROR(PUS_NO_ERROR);
	}

	return PUS_GET_ERROR();
}

pusSt03HousekeepingReportId_t pus_tm_3_X_getReportId(const pusPacket_t* outTm)
{
	if (NULL == outTm)
	{
		PUS_SET_ERROR(PUS_ERROR_NULLPTR);
		return 0;
	}
	else if (PUS_NO_ERROR == PUS_EXPECT_ST03TM(outTm, pusSubtype_NONE))
	{
		PUS_SET_ERROR(PUS_NO_ERROR);

		return outTm->data.u.tmData.data.u.st_3_X.reportId;
	}
	else
	{
		return 0;
	}
}

pusError_t pus_tm_3_X_setReportId(pusPacket_t* outTm, pusSt03HousekeepingReportId_t reportId)
{
	if (NULL == outTm)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	else if (PUS_NO_ERROR == PUS_EXPECT_ST03TM(outTm, pusSubtype_NONE))
	{
		PUS_SET_ERROR(PUS_NO_ERROR);

		outTm->data.u.tmData.data.u.st_3_X.reportId = reportId;
	}
	else
	{
		return PUS_GET_ERROR();
	}
}

pusError_t pus_tm_3_X_setParameterValue(pusPacket_t* outTm, int index, pusStoredParam_t inValue)
{
	if ( NULL == outTm)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (PUS_NO_ERROR == PUS_EXPECT_ST03TM(outTm, pusSubtype_NONE))
	{
		outTm->data.u.tmData.data.u.st_3_X.parameters.arr[index] = inValue;

		return PUS_SET_ERROR(PUS_NO_ERROR);
	}

	return PUS_GET_ERROR();
}

pusError_t pus_tm_3_X_getParameterValue(const pusPacket_t* outTm, int index, pusStoredParam_t* outValue)
{
	if (NULL == outTm || NULL == outValue)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (PUS_NO_ERROR == PUS_EXPECT_ST03TM(outTm, pusSubtype_NONE))
	{
		*outValue = outTm->data.u.tmData.data.u.st_3_X.parameters.arr[index];

		return PUS_SET_ERROR(PUS_NO_ERROR);
	}

	return PUS_GET_ERROR();
}

pusError_t pus_tm_3_X_getNumParameters(const pusPacket_t* outTm, int* outNumParams)
{
	if (NULL == outTm || NULL == outNumParams)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (PUS_NO_ERROR == PUS_EXPECT_ST03TM(outTm, pusSubtype_NONE))
	{
		*outNumParams = outTm->data.u.tmData.data.u.st_3_X.parameters.nCount;

		return PUS_SET_ERROR(PUS_NO_ERROR);
	}

	return PUS_GET_ERROR();
}

pusError_t pus_tm_3_X_setNumParameters(pusPacket_t* outTm, int inNumParams)
{
	if (NULL == outTm)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (PUS_NO_ERROR == PUS_EXPECT_ST03TM(outTm, pusSubtype_NONE))
	{
		outTm->data.u.tmData.data.u.st_3_X.parameters.nCount = inNumParams;

		return PUS_SET_ERROR(PUS_NO_ERROR);
	}

	return PUS_GET_ERROR();
}

//
// Parameter checking
//

pusError_t pus_expectSt03Tm(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function)
{
	pusError_t expectResult = pus_expectTmHeader(packet, function, (pusErrorData_t){.integer=0});
	if (PUS_NO_ERROR == expectResult)
	{
		pusService_t service = pus_getTmService(packet);
		pusSubservice_t subtype = pus_getTmSubtype(packet);

		if (service != pus_ST03_housekeeping)
		{
			//pus_setError(PUS_ERROR_TM_SERVICE, function, (pusErrorData_t){.integer=service});
			return PUS_ERROR_TM_SERVICE;
		}

		if (expectedSubtype == pusSubtype_NONE)
		{
			// Check that subtype corresponds to ST[03]
			if ((subtype != pus_TM_3_25_housekeepingReport) &&
				 (subtype != pus_TM_3_26_diagnosticReport))
			{
				//pus_setError(PUS_ERROR_TM_SUBTYPE, function, (pusErrorData_t){.integer=subtype});
				return PUS_ERROR_TM_SUBTYPE;
			}
		}
		else
		{
			// Check that subtype is as expected
			if (subtype != expectedSubtype)
			{
				//pus_setError(PUS_ERROR_TM_SUBTYPE, function, (pusErrorData_t){.integer=subtype});
				return PUS_ERROR_TM_SUBTYPE;

			}
		}

		pusPacketDataKind_t kind = pus_getTmDataKind(packet);
		if (kind != pus_TM_DATA_ST_3_X)
		{
			//pus_setError(PUS_ERROR_TM_KIND, function, (pusErrorData_t){.integer=kind});
			return PUS_ERROR_TM_KIND;
		}

		return PUS_NO_ERROR;
	}
	else
	{
		return expectResult;
	}
}

pusError_t pus_expectSt03Tc(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function)
{
	pusError_t expectResult = pus_expectTcHeader(packet, function, (pusErrorData_t) { .integer = 0 });
	if (PUS_NO_ERROR == expectResult)
	{
		pusService_t service = pus_getTcService(packet);
		pusSubservice_t subtype = pus_getTcSubtype(packet);

		if (service != pus_ST03_housekeeping)
		{
			//pus_setError(PUS_ERROR_TC_SERVICE, function, (pusErrorData_t) { .integer = service });
			return PUS_ERROR_TC_SERVICE;
		}

		if (expectedSubtype == pusSubtype_NONE)
		{
			// Check that subtype corresponds to ST[03]
			if ((subtype != pus_TC_3_5_enableHousekeeping) &&
				(subtype != pus_TC_3_6_disableHousekeeping) &&
				(subtype != pus_TC_3_7_enableDiagnostic) &&
				(subtype != pus_TC_3_8_disableDiagnostic))
			{
				//pus_setError(PUS_ERROR_TC_SUBTYPE, function, (pusErrorData_t) { .integer = subtype });
				return PUS_ERROR_TC_SUBTYPE;
			}
		}
		else
		{
			// Check that subtype is as expected
			if (subtype != expectedSubtype)
			{
				//pus_setError(PUS_ERROR_TC_SUBTYPE, function, (pusErrorData_t) { .integer = subtype });
				return PUS_ERROR_TC_SUBTYPE;

			}
		}

		pusPacketDataKind_t kind = pus_getTcDataKind(packet);
		if (kind != pus_TC_DATA_ST_3_X)
		{
			//pus_setError(PUS_ERROR_TC_KIND, function, (pusErrorData_t) { .integer = kind });
			return PUS_ERROR_TC_KIND;
		}

		return PUS_NO_ERROR;
	}
	else
	{
		return expectResult;
	}
}

#endif
