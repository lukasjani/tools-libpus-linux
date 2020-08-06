/*
 * H2020 ESROCOS Project
 * Company: GMV Aerospace & Defence S.A.U.
 * Licence: GPLv2
 */


#include "pus_st05_packets.h"
#ifdef PUS_CONFIGURE_ST05_ENABLED


pusError_t pus_tm_5_X_createEventReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, const pusSt05Event_t* event, pusApid_t destination, pusSubservice_t subtype)
{

	if (NULL == outTm || NULL == event)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	/*
	else if (!pus_events_isInitialized())
	{
		return PUS_SET_ERROR(PUS_ERROR_NOT_INITIALIZED);
	}
	*/
	else
	{
		pus_clearError();
		//create package
		pus_initTmPacket(outTm);

		// Source information
		pus_setApid(outTm, apid);
		pus_setSequenceCount(outTm, sequenceCount);

		// Data length
		pus_setPacketDataLength(outTm, sizeof(pusPacketData_t));

		// Service identification
		pus_setTmService(outTm, pus_ST05_eventReporting);
		pus_setTmSubtype(outTm, subtype);

		// Timestamp
		pus_setTmPacketTimeNow(outTm);

		// Destination
		pus_setTmDestination(outTm, destination);

		//data field
		pus_setTmDataKind(outTm, pus_TM_DATA_ST_5_X);
		pus_tm_5_X_setTmEventReportData(outTm, event);


		return PUS_GET_ERROR();
	}
}

pusError_t pus_tc_5_X_createDefaultPacket(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSubservice_t subtype)
{
	if (NULL == outTc)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	else
	{
		pus_clearError();
		pus_initTcPacket(outTc);

		// Source information
		pus_setApid(outTc, apid);
		pus_setSequenceCount(outTc, sequenceCount);

		// Data length
		pus_setPacketDataLength(outTc, sizeof(pusPacketData_t));
		pus_setTcService(outTc, pus_ST05_eventReporting);
		pus_setTcSubtype(outTc, subtype);
		pus_setTcSource(outTc, apid);
		pus_setTcDataKind(outTc, pus_TC_DATA_ST_5_X);

		return PUS_GET_ERROR();
	}

	return PUS_NO_ERROR;
}

pusError_t pus_tm_5_1_createInformativeEventReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, const pusSt05Event_t* event, pusApid_t destination)
{
	if (NULL == outTm || NULL == event)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	else if (PUS_IS_ERROR())
	{
		return PUS_ERROR_BEFORE;
	}
	else
	{
		return pus_tm_5_X_createEventReport(outTm, apid, sequenceCount, event, destination, pus_TM_5_1_eventInformative);
	}
}

pusError_t pus_tm_5_2_createLowSeverityEventReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, const pusSt05Event_t* event, pusApid_t destination)
{
	if (NULL == outTm || NULL == event)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	else if (PUS_IS_ERROR())
	{
		return PUS_ERROR_BEFORE;
	}
	else
	{
		return pus_tm_5_X_createEventReport(outTm, apid, sequenceCount, event, destination, pus_TM_5_2_anomalyLowSeverity);
	}
}

pusError_t pus_tm_5_3_createMediumSeverityEventReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, const pusSt05Event_t* event, pusApid_t destination)
{
	if (NULL == outTm || NULL == event)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	else if (PUS_IS_ERROR())
	{
		return PUS_ERROR_BEFORE;
	}
	else
	{
		return pus_tm_5_X_createEventReport(outTm, apid, sequenceCount, event, destination, pus_TM_5_3_anomalyMediumSeverity);
	}
}

pusError_t pus_tm_5_4_createHighSeverityEventReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, const pusSt05Event_t* event, pusApid_t destination)
{
	if (NULL == outTm || NULL == event)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	else if (PUS_IS_ERROR())
	{
		return PUS_ERROR_BEFORE;
	}
	else
	{
		return pus_tm_5_X_createEventReport(outTm, apid, sequenceCount, event, destination, pus_TM_5_4_anomalyHighSeverity);
	}
}

pusError_t pus_tc_5_5_enableEventReport(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, const pusSt05Event_t* event, const int nCount)
{
	if (NULL == outTc)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (nCount > 20)
	{
		return PUS_SET_ERROR(PUS_ERROR_LIMIT);
	}

	pus_tc_5_X_createDefaultPacket(outTc, apid, sequenceCount, pus_TC_5_5_enableEventReport);

	pus_tc_tm_5_X_setNCount(outTc, nCount);

	for (int i = 0; i < nCount; i++)
	{
		pus_tc_tm_5_X_setEvent(outTc, &event[i], i);
	}

	return PUS_GET_ERROR();
}

pusError_t pus_tc_5_6_disableEventReport(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, const pusSt05Event_t* event, const int nCount)
{
	if (NULL == outTc)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (nCount > 20)
	{
		return PUS_SET_ERROR(PUS_ERROR_LIMIT);
	}

	pus_tc_5_X_createDefaultPacket(outTc, apid, sequenceCount, pus_TC_5_6_disableEventReport);

	pus_tc_tm_5_X_setNCount(outTc, nCount);

	for (int i = 0; i < nCount; i++)
	{
		pus_tc_tm_5_X_setEvent(outTc, &event[i], i);
	}

	return PUS_GET_ERROR();
}

pusError_t pus_tc_5_7_requestDisabledEvents(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount)
{
	if (NULL == outTc)
	{
		return PUS_ERROR_NULLPTR;
	}

	if (PUS_NO_ERROR != pus_tc_5_X_createDefaultPacket(outTc, apid, sequenceCount, pus_TC_5_7_reportDisabledEvents))
	{
		return PUS_GET_ERROR();
	}

	pus_setTcDataKind(outTc, pus_TC_DATA_NONE);
	outTc->data.u.tcData.data.u.st_0_0 = 0;

	return PUS_NO_ERROR;
}

pusError_t pus_tm_5_8_reportDisabledEvents(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusApid_t destination, const pusSt05Event_t* event, const int nCount)
{
	if (NULL == outTm)
	{
		return PUS_ERROR_NULLPTR;
	}
	if (nCount > 20)
	{
		return PUS_SET_ERROR(PUS_ERROR_LIMIT);
	}

	//create package
	pus_initTmPacket(outTm);

	// Source information
	pus_setApid(outTm, apid);
	pus_setSequenceCount(outTm, sequenceCount);

	// Data length
	pus_setPacketDataLength(outTm, sizeof(pusPacketData_t));

	// Service identification
	pus_setTmService(outTm, pus_ST05_eventReporting);
	pus_setTmSubtype(outTm, pus_TM_5_8_disabledEventsReport);

	// Timestamp
	pus_setTmPacketTimeNow(outTm);

	// Destination
	pus_setTmDestination(outTm, destination);

	//data field
	pus_setTmDataKind(outTm, pus_TM_DATA_ST_5_8);


	pus_tc_tm_5_X_setNCount(outTm, nCount);

	for (int i = 0; i < nCount; i++)
	{
		pus_tc_tm_5_X_setEvent(outTm, &event[i], i);
	}

	return PUS_GET_ERROR();
}


//
//Getter & Setter
//

pusError_t pus_tm_5_X_setTmEventReportData(pusPacket_t* outTm, const pusSt05Event_t* event)
{
	if (NULL == outTm|| NULL == event)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	else if( PUS_NO_ERROR != PUS_EXPECT_ST05TM(outTm, pusSubtype_NONE))
	{
		return PUS_GET_ERROR();
	}
	{
		outTm->data.u.tmData.data.u.st_5_X.eventId = event->eventId;
		outTm->data.u.tmData.data.u.st_5_X.data = event->data;
		return PUS_NO_ERROR;
	}
}

pusError_t pus_tm_5_X_getTmEventReportData(const pusPacket_t* outTm, pusSt05Event_t* event)
{

	if (NULL == outTm|| NULL == event)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	else if( PUS_NO_ERROR != PUS_EXPECT_ST05TM(outTm, pusSubtype_NONE))
	{
		return PUS_GET_ERROR();
	}
	else
	{
		event->eventId = outTm->data.u.tmData.data.u.st_5_X.eventId;
		event->data = outTm->data.u.tmData.data.u.st_5_X.data;
		return PUS_NO_ERROR;
	}
}

pusError_t pus_tc_tm_5_X_setNCount(pusPacket_t* outTc, const int nCount)
{

	if (NULL == outTc || NULL == nCount)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	else if (PUS_NO_ERROR == PUS_EXPECT_ST05TC(outTc, pusSubtype_NONE))
	{
		outTc->data.u.tcData.data.u.st_5_X.nCount = nCount;
		return PUS_NO_ERROR;
	}
	else if (PUS_NO_ERROR == PUS_EXPECT_ST05TM(outTc, pus_TM_5_8_disabledEventsReport))
	{
		outTc->data.u.tmData.data.u.st_5_8.nCount = nCount;
		return PUS_NO_ERROR;
	}
	else return PUS_GET_ERROR();
}

int pus_tc_tm_5_X_getNCount(const pusPacket_t* outTc)
{

	if (NULL == outTc)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	else if (PUS_NO_ERROR == PUS_EXPECT_ST05TC(outTc, pusSubtype_NONE))
	{
		return outTc->data.u.tcData.data.u.st_5_X.nCount;
	}
	else if(PUS_NO_ERROR == PUS_EXPECT_ST05TM(outTc, pus_TM_5_8_disabledEventsReport))
	{
		return outTc->data.u.tmData.data.u.st_5_8.nCount;
	}
	else return PUS_GET_ERROR();
}

pusError_t pus_tc_tm_5_X_setEvent(pusPacket_t* outTc, const pusSt05Event_t* event, const int nCount)
{

	if (NULL == outTc || NULL == event)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	else if (PUS_NO_ERROR == PUS_EXPECT_ST05TC(outTc, pusSubtype_NONE))
	{
		outTc->data.u.tcData.data.u.st_5_X.arr[nCount] = *event;
		return PUS_NO_ERROR;
	}
	else if(PUS_NO_ERROR == PUS_EXPECT_ST05TM(outTc, pus_TM_5_8_disabledEventsReport))
	{
		outTc->data.u.tmData.data.u.st_5_8.arr[nCount] = *event;
		return PUS_NO_ERROR;
	}
	else return PUS_GET_ERROR();
}

pusError_t pus_tc_tm_5_X_getEvent(const pusPacket_t* outTc, pusSt05Event_t* event, const int nCount)
{

	if (NULL == outTc || NULL == event)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	else if (PUS_NO_ERROR == PUS_EXPECT_ST05TC(outTc, pusSubtype_NONE))
	{
		*event = outTc->data.u.tcData.data.u.st_5_X.arr[nCount];
		return PUS_NO_ERROR;
	}
	else if (PUS_NO_ERROR == PUS_EXPECT_ST05TM(outTc, pus_TM_5_8_disabledEventsReport))
	{
		*event = outTc->data.u.tmData.data.u.st_5_8.arr[nCount];
		return PUS_NO_ERROR;
	}
	else return PUS_GET_ERROR();
}

//
// Parameter checking
//

pusError_t pus_expectSt05Tm(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function)
{
	pusError_t expectResult = pus_expectTmHeader(packet, function, (pusErrorData_t){.integer=0});
	if (PUS_NO_ERROR == expectResult)
	{
		pusService_t service = pus_getTmService(packet);
		pusSubservice_t subtype = pus_getTmSubtype(packet);

		if (service != pus_ST05_eventReporting)
		{
			//pus_setError(PUS_ERROR_TM_SERVICE, function, (pusErrorData_t){.integer=service});
			return PUS_ERROR_TM_SERVICE;
		}

		if (expectedSubtype == pusSubtype_NONE)
		{
			// Check that subtype corresponds to ST[05]
			if ((subtype != pus_TM_5_1_eventInformative) &&
				(subtype != pus_TM_5_2_anomalyLowSeverity) &&
				(subtype != pus_TM_5_3_anomalyMediumSeverity) &&
				(subtype != pus_TM_5_4_anomalyHighSeverity) &&
				(subtype != pus_TM_5_8_disabledEventsReport))
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
		if ((kind != pus_TM_DATA_ST_5_X) &&
			(kind != pus_TM_DATA_ST_5_8))
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

pusError_t pus_expectSt05Tc(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function)
{
	pusError_t expectResult = pus_expectTcHeader(packet, function, (pusErrorData_t) { .integer = 0 });
	if (PUS_NO_ERROR == expectResult)
	{
		pusService_t service = pus_getTcService(packet);
		pusSubservice_t subtype = pus_getTcSubtype(packet);

		if (service != pus_ST05_eventReporting)
		{
			//pus_setError(PUS_ERROR_TC_SERVICE, function, (pusErrorData_t) { .integer = service });
			return PUS_ERROR_TC_SERVICE;
		}

		if (expectedSubtype == pusSubtype_NONE)
		{
			// Check that subtype corresponds to ST[05]
			if ((subtype != pus_TC_5_6_disableEventReport) &&
				(subtype != pus_TC_5_5_enableEventReport) &&
				(subtype != pus_TC_5_7_reportDisabledEvents))
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
				pus_setError(PUS_ERROR_TC_SUBTYPE, function, (pusErrorData_t) { .integer = subtype });
				return PUS_ERROR_TC_SUBTYPE;
			}
		}

		pusPacketDataKind_t kind = pus_getTcDataKind(packet);
		if (kind != pus_TC_DATA_ST_5_X)
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
