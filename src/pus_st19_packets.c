/*
 * H2020 ESROCOS Project
 * Company: GMV Aerospace & Defence S.A.U.
 * Licence: GPLv2
 */

#include "pus_st19_packets.h"
#ifdef PUS_CONFIGURE_ST19_ENABLED



pusError_t pus_tc_19_X_createDefaultEventActionRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount)
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

		pus_setTcService(outTc, pus_ST19_eventAction);
		pus_setTcSource(outTc, apid);

		return PUS_GET_ERROR();
	}

	return PUS_NO_ERROR;
}



pusError_t pus_tc_19_1_createAddEventActionDefinitionsRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt05EventId_t* eventId, pusPacket_t* tcAction, int count)
{
	if (NULL == outTc)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (count > 20) 
	{
		return PUS_SET_ERROR(PUS_ERROR_FULL_BUFFER);
	}

	if( PUS_NO_ERROR !=  pus_tc_19_X_createDefaultEventActionRequest(outTc, apid, sequenceCount) )
	{
		return PUS_GET_ERROR();
	}

	pus_setTcSubtype(outTc, pus_TC_19_1_addEventActionDefinitions);
	pus_setTcDataKind(outTc, pus_TC_DATA_ST_19_1);

	pusPacketReduced_t tcActionR[20];
	for (int i = 0; i < count; i++)
	{
		pus_packetReduced_createPacketReducedFromPacket(&tcActionR[i], &tcAction[i]);
	}

	pus_tc_19_1_setAction(outTc, &tcActionR, count);
	pus_tc_19_X_setEventId(outTc, eventId, count);
	return PUS_NO_ERROR;
}


pusError_t pus_tc_19_2_createDeleteEventActionDefinitionsRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt05EventId_t* eventId, int count)
{
	if (NULL == outTc)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if( PUS_NO_ERROR !=  pus_tc_19_X_createDefaultEventActionRequest(outTc, apid, sequenceCount) )
	{
		return PUS_GET_ERROR();
	}

	pus_setTcSubtype(outTc, pus_TC_19_2_deleteEventActionDefinitions);
	pus_setTcDataKind(outTc, pus_TC_DATA_ST_19_X);

	pus_tc_19_X_setEventId(outTc, eventId, count);

	return PUS_NO_ERROR;
}

pusError_t pus_tc_19_4_createEnableEventActionDefinitions(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt05EventId_t* eventId, int count)
{
	if (NULL == outTc)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if( PUS_NO_ERROR !=  pus_tc_19_X_createDefaultEventActionRequest(outTc, apid, sequenceCount) )
	{
		return PUS_GET_ERROR();
	}

	pus_setTcSubtype(outTc, pus_TC_19_4_enableEventActionDefinitions);
	pus_setTcDataKind(outTc, pus_TC_DATA_ST_19_X);

	pus_tc_19_X_setEventId(outTc, eventId, count);

	return PUS_NO_ERROR;
}

pusError_t pus_tc_19_5_createDisableEventActionDefinitions(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSt05EventId_t* eventId, int count)
{
	if (NULL == outTc)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if( PUS_NO_ERROR !=  pus_tc_19_X_createDefaultEventActionRequest(outTc, apid, sequenceCount) )
	{
		return PUS_GET_ERROR();
	}

	pus_setTcSubtype(outTc, pus_TC_19_5_disableEventActionDefinitions);
	pus_setTcDataKind(outTc, pus_TC_DATA_ST_19_X);

	pus_tc_19_X_setEventId(outTc, eventId, count);

	return PUS_NO_ERROR;
}


pusError_t pus_tc_19_1_setAction(pusPacket_t* outTc, pusPacketReduced_t* actionR, int count)
{
	if (NULL == outTc || NULL == actionR)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	else if (count > 20)
	{
		return PUS_SET_ERROR(PUS_ERROR_FULL_BUFFER);
	}

	outTc->data.u.tcData.data.u.st_19_1.packetReduced[count] = *actionR;

	outTc->data.u.tcData.data.u.st_19_1.nCount++;

	return PUS_NO_ERROR;
}

pusError_t pus_tc_19_1_getAction(pusPacketReduced_t* actionR, const pusPacket_t* tc, int count)
{
    if ( NULL == tc || NULL == actionR)
    {
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
    }
    else
    {
    	*actionR = tc->data.u.tcData.data.u.st_19_1.packetReduced[count];
		return PUS_NO_ERROR;
    }
}

pusError_t pus_tc_19_X_setNCount(pusPacket_t* outTc, int count)
{
	if (NULL == outTc)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (pus_getTcDataKind(outTc) == pus_TC_DATA_ST_19_1)
	{
		outTc->data.u.tcData.data.u.st_19_1.nCount = count;
		
		return PUS_NO_ERROR;
	}
	else if (pus_getTcDataKind(outTc) == pus_TC_DATA_ST_19_X)
	{
		outTc->data.u.tcData.data.u.st_19_X.nCount = count;

		return PUS_NO_ERROR;
	}

	return PUS_ERROR_TC_KIND;
}

int pus_tc_19_X_getNCount(const pusPacket_t* outTc)
{
	if (NULL == outTc)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (pus_getTcDataKind(outTc) == pus_TC_DATA_ST_19_1)
	{
		PUS_SET_ERROR(PUS_NO_ERROR);

		return outTc->data.u.tcData.data.u.st_19_1.nCount;
	}
	else if (pus_getTcDataKind(outTc) == pus_TC_DATA_ST_19_X)
	{
		PUS_SET_ERROR(PUS_NO_ERROR);

		return outTc->data.u.tcData.data.u.st_19_X.nCount;
	}

	PUS_SET_ERROR(PUS_ERROR_TC_KIND);

	return 0;
}

pusError_t pus_tc_19_X_setEventId(pusPacket_t* packet, pusSt05EventId_t* eventId, int count)
{
	if ( NULL == packet || NULL == eventId)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (count > 20)
	{
		return PUS_SET_ERROR(PUS_ERROR_FULL_BUFFER);
	}

	if( pus_getTcDataKind(packet) == pus_TC_DATA_ST_19_1)
	{
		packet->data.u.tcData.data.u.st_19_1.eventId[count] = *eventId;

		return PUS_NO_ERROR;
	}
	else if( pus_getTcDataKind(packet) == pus_TC_DATA_ST_19_X)
	{
		packet->data.u.tcData.data.u.st_19_X.eventId[count] = *eventId;

		return PUS_NO_ERROR;
	}

	return PUS_ERROR_TC_KIND;
}

pusError_t pus_tc_19_X_getEventId(pusSt05EventId_t* eventId, const pusPacket_t* packet, int count)
{
	if ( NULL == packet || NULL == eventId)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if( pus_getTcDataKind(packet) == pus_TC_DATA_ST_19_1)
	{
		*eventId = packet->data.u.tcData.data.u.st_19_1.eventId[count];

		return PUS_NO_ERROR;
	}
	else if(pus_getTcDataKind(packet) == pus_TC_DATA_ST_19_X)
	{
		*eventId = packet->data.u.tcData.data.u.st_19_X.eventId[count];

		return PUS_NO_ERROR;
	}

	return PUS_ERROR_TC_KIND;
}

//
// Parameter checking
//
pusError_t pus_expectSt19Tc(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function)
{
	pusError_t expectResult = pus_expectTcHeader(packet, function, (pusErrorData_t){.integer=0});
	if (PUS_NO_ERROR == expectResult)
	{
		pusService_t service = pus_getTcService(packet);
		pusSubservice_t subtype = pus_getTcSubtype(packet);

		if (service != pus_ST19_eventAction)
		{
			//pus_setError(PUS_ERROR_TC_SERVICE, function, (pusErrorData_t){.integer=service});
			return PUS_ERROR_TC_SERVICE;
		}

		if (expectedSubtype == pusSubtype_NONE)
		{
			// Check that subtype corresponds to ST[01]
			if ((subtype != pus_TC_19_1_addEventActionDefinitions) &&
				(subtype != pus_TC_19_2_deleteEventActionDefinitions) &&
				(subtype != pus_TC_19_4_enableEventActionDefinitions) &&
				(subtype != pus_TC_19_5_disableEventActionDefinitions))
			{
				//pus_setError(PUS_ERROR_TC_SUBTYPE, function, (pusErrorData_t){.integer=subtype});
				return PUS_ERROR_TC_SUBTYPE;
			}
		}
		else
		{
			// Check that subtype is as expected
			if (subtype != expectedSubtype)
			{
				//pus_setError(PUS_ERROR_TC_SUBTYPE, function, (pusErrorData_t){.integer=subtype});
				return PUS_ERROR_TC_SUBTYPE;

			}
		}

		pusPacketDataKind_t kind = pus_getTcDataKind(packet);
		if (kind != pus_TC_DATA_ST_19_1 && kind != pus_TC_DATA_ST_19_X)
		{
			//pus_setError(PUS_ERROR_TC_KIND, function, (pusErrorData_t){.integer=kind});
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
