/*
 * H2020 ESROCOS Project
 * Company: GMV Aerospace & Defence S.A.U.
 * Licence: GPLv2
 */

#include "pus_st11_packets.h"

#ifdef PUS_CONFIGURE_ST11_ENABLED
#include <stdio.h>

pusError_t pus_tc_11_X_createDefaultPacket(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSubservice_t subtype)
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
		pus_setTcService(outTc, pus_ST11_timeBasedScheduling);
		pus_setTcSubtype(outTc, subtype);
		pus_setTcSource(outTc, apid);

		return PUS_GET_ERROR();
	}

	return PUS_NO_ERROR;
}

pusError_t pus_tc_11_1_createEnableTimeBasedSchedule(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount)
{
	if( NULL == outTc)
	{
		return PUS_ERROR_NULLPTR;
	}

	if( PUS_NO_ERROR != pus_tc_11_X_createDefaultPacket(outTc, apid, sequenceCount, pus_TC_11_1_enableTimeBasedScheduling) )
	{
		return PUS_GET_ERROR();
	}
	pus_setTcDataKind(outTc, pus_TC_DATA_NONE);
	outTc->data.u.tcData.data.u.st_0_0 = 0;

	return PUS_NO_ERROR;
}

pusError_t pus_tc_11_2_createDisableTimeBasedSchedule(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount)
{
	if( NULL == outTc)
	{
		return PUS_ERROR_NULLPTR;
	}
	if( PUS_NO_ERROR != pus_tc_11_X_createDefaultPacket(outTc, apid, sequenceCount, pus_TC_11_2_disableTimeBasedScheduling) )
	{
		return PUS_GET_ERROR();
	}
	pus_setTcDataKind(outTc, pus_TC_DATA_NONE);
	outTc->data.u.tcData.data.u.st_0_0 = 0;

	return PUS_NO_ERROR;
}


pusError_t pus_tc_11_3_createResetTimeBasedSchedule(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount)
{
	if( NULL == outTc)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if( PUS_NO_ERROR != pus_tc_11_X_createDefaultPacket(outTc, apid, sequenceCount, pus_TC_11_3_resetTimeBasedSchedule) )
	{
		return PUS_GET_ERROR();
	}
	pus_setTcDataKind(outTc, pus_TC_DATA_NONE);
	outTc->data.u.tcData.data.u.st_0_0 = 0;

	return PUS_NO_ERROR;
}

pusError_t pus_tc_11_4_createInsertActivityIntoSchedule(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount)
{
	if( NULL == outTc )
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if( PUS_NO_ERROR != pus_tc_11_X_createDefaultPacket(outTc, apid, sequenceCount, pus_TC_11_4_insertActivity) )
	{
		return PUS_GET_ERROR();
	}

	pus_setTcDataKind(outTc, pus_TC_DATA_ST_11_4);

	pus_tc_11_4_setNCount(outTc, 0);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tc_11_4_setNCount(pusPacket_t* outTc, int32_t nCount)
{
	if( NULL == outTc)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if( PUS_NO_ERROR != PUS_EXPECT_ST11TC(outTc, pus_TC_11_4_insertActivity))
	{
		return PUS_GET_ERROR();
	}
	if(nCount < 0)
	{
		PUS_SET_ERROR(PUS_ERROR_OUT_OF_RANGE);
	}

	outTc->data.u.tcData.data.u.st_11_4.nCount = nCount;
	return PUS_SET_ERROR(PUS_NO_ERROR);
}

int32_t pus_tc_11_4_getNCount(pusPacket_t* outTc)
{
	if( NULL == outTc)
	{
		PUS_SET_ERROR(PUS_ERROR_NULLPTR);
		return 0;
	}
	if( PUS_NO_ERROR != PUS_EXPECT_ST11TC(outTc, pus_TC_11_4_insertActivity))
	{
		return 0;
	}

	PUS_SET_ERROR(PUS_NO_ERROR);
	return outTc->data.u.tcData.data.u.st_11_4.nCount;
}

pusError_t pus_tc_11_4_setActivity(pusPacket_t* outTc, const pusPacket_t* tcAction, const pusTime_t* time)
{
	if( NULL == outTc )
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if( PUS_NO_ERROR != PUS_EXPECT_ST11TC(outTc, pus_TC_11_4_insertActivity))
	{
		return PUS_GET_ERROR();
	}

	int nCount = outTc->data.u.tcData.data.u.st_11_4.nCount;

	pusPacketReduced_t tcR;
	if(PUS_NO_ERROR != pus_packetReduced_createPacketReducedFromPacket(&tcR, tcAction))
	{
		return PUS_GET_ERROR();
	}

	outTc->data.u.tcData.data.u.st_11_4.arr[nCount].packetReduced = tcR;
	outTc->data.u.tcData.data.u.st_11_4.arr[nCount].time = *time;

	outTc->data.u.tcData.data.u.st_11_4.nCount = nCount + 1;

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tc_11_4_getActivities(int32_t* nCount, pusSt11ScheduledActivity_t* activities, const pusPacket_t* inTc)
{
	if( NULL == inTc || NULL == activities || NULL == inTc)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if( PUS_NO_ERROR != PUS_EXPECT_ST11TC(inTc, pus_TC_11_4_insertActivity))
	{
		return PUS_GET_ERROR();
	}

	*nCount = inTc->data.u.tcData.data.u.st_11_4.nCount;

	for(int32_t i = 0; i < *nCount; i++)
	{
		activities[i].packetReduced = inTc->data.u.tcData.data.u.st_11_4.arr[i].packetReduced ;
		activities[i].time = inTc->data.u.tcData.data.u.st_11_4.arr[i].time;
	}
	return PUS_SET_ERROR(PUS_NO_ERROR);
}


//
// Parameter checking
//
pusError_t pus_expectSt11Tc(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function)
{
	pusError_t expectResult = pus_expectTcHeader(packet, function, (pusErrorData_t){.integer=0});
	if (PUS_NO_ERROR == expectResult)
	{
		pusService_t service = pus_getTcService(packet);
		pusSubservice_t subtype = pus_getTcSubtype(packet);

		if (service != pus_ST11_timeBasedScheduling)
		{
			//pus_setError(PUS_ERROR_TC_SERVICE, function, (pusErrorData_t){.integer=service});
			return PUS_ERROR_TC_SERVICE;
		}

		if (expectedSubtype == pusSubtype_NONE)
		{
			// Check that subtype corresponds to ST[11]
			if ((subtype != pus_TC_11_1_enableTimeBasedScheduling) &&
				(subtype != pus_TC_11_2_disableTimeBasedScheduling) &&
				(subtype != pus_TC_11_3_resetTimeBasedSchedule) &&
				(subtype != pus_TC_11_4_insertActivity))
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
		if (kind != pus_TC_DATA_ST_11_4 && kind != pus_TC_DATA_NONE)
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

