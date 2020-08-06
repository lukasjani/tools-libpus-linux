/*
 * H2020 ESROCOS Project
 * Company: GMV Aerospace & Defence S.A.U.
 * Licence: GPLv2
 */


#include "pus_st13_packets.h"
#ifdef PUS_CONFIGURE_ST13_ENABLED

/*
pusError_t pus_tm_13_sendLargeFile(pusApid_t apid, pusSequenceCount_t sequenceCount, pusApid_t destination, 
	char* content, uint64_t length, uint64_t transactionID, pusMutex_t* mutex)
{
	if (NULL == content)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pus_st13_mutex = mutex;

	if (NULL != pus_st13_mutex && !pus_mutexLockOk(pus_st13_mutex))
	{
		return PUS_ERROR_NOT_AVAILABLE;
	}

	if ((pus_st13_sentOut - pus_st13_outgoingPackets) % pus_ST13_MAX_DOWNLINK_QUEUE == 1)
	{
		return PUS_SET_ERROR(PUS_ERROR_FULL_BUFFER);
	}

	if (!pus_mutexUnlockOk(pus_st13_mutex))
	{
		return PUS_ERROR_NOT_AVAILABLE;
	}

	char split_content[pus_ST13_MAX_PART_SIZE];

	pus_tc_tm_13_splitContent(split_content, content, 0);

	pusPacket_t emptyPacket;

	pus_initTmPacket(&emptyPacket);
	pus_st13_outgoingPacketList[pus_st13_outgoingPackets][0] = emptyPacket;
	pus_tm_13_1_createFirstDownlinkPartReport(&(pus_st13_outgoingPacketList[pus_st13_outgoingPackets][0]), apid, sequenceCount, destination, transactionID, split_content);

	int i = 1;
	for (; i < length / pus_ST13_MAX_PART_SIZE; i++)
	{
		pus_st13_outgoingPacketList[pus_st13_outgoingPackets][i] = emptyPacket;

		pus_tc_tm_13_splitContent(split_content, content, i);
		pus_tm_13_2_createIntermediateDownlinkPartReport(&(pus_st13_outgoingPacketList[pus_st13_outgoingPackets][i]), apid, sequenceCount, destination, transactionID, i + 1, split_content);
	}

	pus_st13_outgoingPacketList[pus_st13_outgoingPackets][i] = emptyPacket;

	pus_tc_tm_13_splitContent(split_content, content, i);

	pus_tm_13_3_createLastDownlinkPartReport(&(pus_st13_outgoingPacketList[pus_st13_outgoingPackets][i]), apid, sequenceCount, destination, transactionID, i + 1, split_content);

	pus_st13_outgoingPackets = (pus_st13_outgoingPackets + 1) % pus_ST13_MAX_DOWNLINK_QUEUE;

	return PUS_NO_ERROR;
}

pusError_t pus_tc_13_sendLargeFile(pusApid_t apid, pusSequenceCount_t sequenceCount,
	char* content, uint64_t length, uint64_t transactionID, pusMutex_t* mutex)
{
	if (NULL == content)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pus_st13_mutex = mutex;

	if (NULL != pus_st13_mutex && !pus_mutexLockOk(pus_st13_mutex))
	{
		return PUS_ERROR_NOT_AVAILABLE;
	}

	if ((pus_st13_sentOut - pus_st13_outgoingPackets) % pus_ST13_MAX_UPLINK_QUEUE == 1)
	{
		return PUS_SET_ERROR(PUS_ERROR_FULL_BUFFER);
	}

	if (!pus_mutexUnlockOk(pus_st13_mutex))
	{
		return PUS_ERROR_NOT_AVAILABLE;
	}

	char split_content[pus_ST13_MAX_PART_SIZE];

	pus_tc_tm_13_splitContent(split_content, content, 0);

	pusPacket_t emptyPacket;

	pus_initTcPacket(&emptyPacket);
	pus_st13_outgoingPacketList[pus_st13_outgoingPackets][0] = emptyPacket;
	pus_tc_13_9_createFirstUplinkPart(&(pus_st13_outgoingPacketList[pus_st13_outgoingPackets][0]), apid, sequenceCount, transactionID, split_content);

	int i = 1;
	for (; i < length / pus_ST13_MAX_PART_SIZE; i++)
	{
		pus_st13_outgoingPacketList[pus_st13_outgoingPackets][i] = emptyPacket;

		pus_tc_tm_13_splitContent(split_content, content, i);
		pus_tc_13_10_createIntermediateUplinkPart(&(pus_st13_outgoingPacketList[pus_st13_outgoingPackets][i]), apid, sequenceCount, transactionID, i + 1, split_content);
	}

	pus_st13_outgoingPacketList[pus_st13_outgoingPackets][i] = emptyPacket;

	pus_tc_tm_13_splitContent(split_content, content, i);

	pus_tc_13_11_createLastUplinkPart(&(pus_st13_outgoingPacketList[pus_st13_outgoingPackets][i]), apid, sequenceCount, transactionID, i + 1, split_content);

	pus_st13_outgoingPackets = (pus_st13_outgoingPackets + 1) % pus_ST13_MAX_UPLINK_QUEUE;

	return PUS_NO_ERROR;
}

pusError_t pus_tm_13_receiveLargeFile(char* output, pusTime_t timeout, pusMutex_t* mutex)
{
	if (NULL == output)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (pus_getTmService(&(pus_st13_incomingPacketList[pus_st13_receivedIn][0])) != pus_ST13_largePacketTransfer)
	{
		return PUS_SET_ERROR(PUS_ERROR_TM_SERVICE);
	}

	if (pus_getTmSubtype(&(pus_st13_incomingPacketList[pus_st13_receivedIn][0])) != pus_TM_13_1_createFirstDownlinkPartReport)
	{
		return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
	}

	pus_st13_mutex = mutex;

	if (NULL != pus_st13_mutex && !pus_mutexLockOk(pus_st13_mutex))
	{
		return PUS_ERROR_NOT_AVAILABLE;
	}

	if (pus_st13_incomingPackets == pus_st13_receivedIn)
	{
		return PUS_SET_ERROR(PUS_ERROR_EMPTY_BUFFER);
	}

	if (!pus_mutexUnlockOk(pus_st13_mutex))
	{
		return PUS_ERROR_NOT_AVAILABLE;
	}

	pusApid_t apid = pus_getApid(&pus_st13_incomingPacketList[pus_st13_receivedIn][0]);
	pusSequenceCount_t sequenceCount = pus_getSequenceCount(&pus_st13_incomingPacketList[pus_st13_receivedIn][0]);

	pus_tc_tm_13_X_getContent(output, &pus_st13_incomingPacketList[pus_st13_receivedIn][0]);

	uint64_t i = 1;
	uint64_t length = pus_ST13_MAX_PART_SIZE;
	pusTime_t now, then;
	pus_now(&then);

	while (pus_getTmSubtype(&pus_st13_incomingPacketList[pus_st13_receivedIn][i]) != pus_TM_13_3_createLastDownlinkPartReport)
	{
		while (&pus_st13_incomingPacketList[pus_st13_receivedIn][i] == NULL)
		{
			pus_now(&now);
			if (((now.tv_sec - then.tv_sec) > timeout.tv_sec) && ((now.tv_nsec - then.tv_nsec) > timeout.tv_nsec))
			{
				return PUS_SET_ERROR(PUS_ERROR_TIMEOUT);
			}
			usleep(20);
		}

		if (pus_getTmSubtype(&pus_st13_incomingPacketList[pus_st13_receivedIn][i]) != pus_TM_13_2_createIntermediateDownlinkPartReport)
		{
			return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
		}

		uint64_t seqNumber;
		pus_tc_tm_13_X_getPartSeqNumber(&seqNumber, &pus_st13_incomingPacketList[pus_st13_receivedIn][i]);

		if (seqNumber != i + 1)
		{
			return PUS_SET_ERROR(PUS_ERROR_INVALID_ID);
		}

		pus_tc_tm_13_mergeContent(output, &pus_st13_incomingPacketList[pus_st13_receivedIn][i], &length);
		i++;
		pus_now(&then);
		usleep(20);
	}

	pus_tc_tm_13_mergeContent(output, &pus_st13_incomingPacketList[pus_st13_receivedIn][i], &length);

	pus_st13_receivedIn = (pus_st13_receivedIn + 1) % pus_ST13_MAX_DOWNLINK_QUEUE;

	return PUS_NO_ERROR;
}

pusError_t pus_tc_13_receiveLargeFile(char* output, pusTime_t timeout, pusMutex_t* mutex)
{
	if (NULL == output)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (pus_getTcService(&(pus_st13_incomingPacketList[pus_st13_receivedIn][0])) != pus_ST13_largePacketTransfer)
	{
		return PUS_SET_ERROR(PUS_ERROR_TC_SERVICE);
	}

	if (pus_getTcSubtype(&(pus_st13_incomingPacketList[pus_st13_receivedIn][0])) != pus_TC_13_9_createFirstUplinkPartReport)
	{
		return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
	}

	pus_st13_mutex = mutex;

	if (NULL != pus_st13_mutex && !pus_mutexLockOk(pus_st13_mutex))
	{
		return PUS_ERROR_NOT_AVAILABLE;
	}

	if (pus_st13_incomingPackets == pus_st13_receivedIn)
	{
		return PUS_SET_ERROR(PUS_ERROR_EMPTY_BUFFER);
	}

	if (!pus_mutexUnlockOk(pus_st13_mutex))
	{
		return PUS_ERROR_NOT_AVAILABLE;
	}

	pusApid_t apid = pus_getApid(&pus_st13_incomingPacketList[pus_st13_receivedIn][0]);
	pusSequenceCount_t sequenceCount = pus_getSequenceCount(&pus_st13_incomingPacketList[pus_st13_receivedIn][0]);

	pus_tc_tm_13_X_getContent(output, &pus_st13_incomingPacketList[pus_st13_receivedIn][0]);

	uint64_t i = 1;
	uint64_t length = pus_ST13_MAX_PART_SIZE;
	pusTime_t now, then;
	pus_now(&then);

	while (pus_getTcSubtype(&pus_st13_incomingPacketList[pus_st13_receivedIn][i]) != pus_TC_13_11_createLastUplinkPartReport)
	{
		while (&pus_st13_incomingPacketList[pus_st13_receivedIn][i] == NULL) 
		{
			pus_now(&now);
			if (((now.tv_sec - then.tv_sec) > timeout.tv_sec) && ((now.tv_nsec - then.tv_nsec) > timeout.tv_nsec))
			{
				return PUS_SET_ERROR(PUS_ERROR_TIMEOUT);
			}
			usleep(20);
		}

		if (pus_getTcSubtype(&pus_st13_incomingPacketList[pus_st13_receivedIn][i]) != pus_TC_13_10_createIntermediateUplinkPartReport)
		{
			return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
		}

		uint64_t seqNumber;
		pus_tc_tm_13_X_getPartSeqNumber(&seqNumber, &pus_st13_incomingPacketList[pus_st13_receivedIn][i]);

		if (seqNumber != i + 1)
		{
			return PUS_SET_ERROR(PUS_ERROR_INVALID_ID);
		}

		pus_tc_tm_13_mergeContent(output, &pus_st13_incomingPacketList[pus_st13_receivedIn][i], &length);
		i++;
		pus_now(&then);
		usleep(20);
	}

	pus_tc_tm_13_mergeContent(output, &pus_st13_incomingPacketList[pus_st13_receivedIn][i], &length);

	pus_st13_receivedIn = (pus_st13_receivedIn + 1) % pus_ST13_MAX_DOWNLINK_QUEUE;

	return PUS_NO_ERROR;
}
*/
pusError_t pus_tm_13_X_createDefaultTMPacket(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSubservice_t subtype, pusApid_t destination)
{
	if (NULL == outTm)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	else
	{
		pus_clearError();
		pus_initTmPacket(outTm);

		// Source information
		pus_setApid(outTm, apid);
		pus_setSequenceCount(outTm, sequenceCount);

		// Data length
		pus_setPacketDataLength(outTm, sizeof(pusPacketData_t));
		
		// Service identification
		pus_setTmService(outTm, pus_ST13_largePacketTransfer);
		pus_setTmSubtype(outTm, subtype);
		pus_setTmDestination(outTm, destination);

		return PUS_GET_ERROR();
	}

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tc_13_X_createDefaultTCPacket(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSubservice_t subtype)
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

		// Service identification
		pus_setTcService(outTc, pus_ST13_largePacketTransfer);
		pus_setTcSubtype(outTc, subtype);
		pus_setTcSource(outTc, apid);

		return PUS_GET_ERROR();
	}

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tm_13_1_createFirstDownlinkPartReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusApid_t destination,
	int transactionID, char* content)
{
	if (NULL == outTm || NULL == transactionID || NULL == content)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pusError_t error = pus_tm_13_X_createDefaultTMPacket(outTm, apid, sequenceCount, pus_TM_13_1_createFirstDownlinkPartReport, destination);
	if (PUS_NO_ERROR != error)
	{
		return PUS_GET_ERROR();
	}

	//Set data field
	pus_setTmDataKind(outTm, pus_TM_DATA_ST_13_X);
	pus_tc_tm_13_X_setTransactionID(outTm, transactionID);
	pus_tc_tm_13_X_setPartSeqNumber(outTm, 1);
	pus_tc_tm_13_X_setContent(outTm, content);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tm_13_2_createIntermediateDownlinkPartReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusApid_t destination,
	int transactionID, int part_seq_number, char* content)
{
	if (NULL == outTm || NULL == transactionID || NULL == part_seq_number || NULL == content)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pusError_t error = pus_tm_13_X_createDefaultTMPacket(outTm, apid, sequenceCount, pus_TM_13_2_createIntermediateDownlinkPartReport, destination);
	if (PUS_NO_ERROR != error)
	{
		return PUS_GET_ERROR();
	}

	//Set data field
	pus_setTmDataKind(outTm, pus_TM_DATA_ST_13_X);
	pus_tc_tm_13_X_setTransactionID(outTm, transactionID);
	pus_tc_tm_13_X_setPartSeqNumber(outTm, part_seq_number);
	pus_tc_tm_13_X_setContent(outTm, content);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tm_13_3_createLastDownlinkPartReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusApid_t destination,
	int transactionID, int part_seq_number, char* content)
{
	if (NULL == outTm || NULL == transactionID || NULL == part_seq_number || NULL == content)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pusError_t error = pus_tm_13_X_createDefaultTMPacket(outTm, apid, sequenceCount, pus_TM_13_3_createLastDownlinkPartReport, destination);
	if (PUS_NO_ERROR != error)
	{
		return PUS_GET_ERROR();
	}

	//Set data field
	pus_setTmDataKind(outTm, pus_TM_DATA_ST_13_X);
	pus_tc_tm_13_X_setTransactionID(outTm, transactionID);
	pus_tc_tm_13_X_setPartSeqNumber(outTm, part_seq_number);
	pus_tc_tm_13_X_setContent(outTm, content);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tc_13_9_createFirstUplinkPart(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
	int transactionID, char* content)
{
	if (NULL == outTc || NULL == transactionID || NULL == content)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pusError_t error = pus_tc_13_X_createDefaultTCPacket(outTc, apid, sequenceCount, pus_TC_13_9_createFirstUplinkPartReport);
	if (PUS_NO_ERROR != error)
	{
		return PUS_GET_ERROR();
	}

	//Set data field
	pus_setTcDataKind(outTc, pus_TC_DATA_ST_13_X);
	pus_tc_tm_13_X_setTransactionID(outTc, transactionID);
	pus_tc_tm_13_X_setPartSeqNumber(outTc, 1);
	pus_tc_tm_13_X_setContent(outTc, content);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tc_13_10_createIntermediateUplinkPart(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
	int transactionID, int part_seq_number, char* content)
{
	if (NULL == outTc || NULL == transactionID || NULL == part_seq_number || NULL == content)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pusError_t error = pus_tc_13_X_createDefaultTCPacket(outTc, apid, sequenceCount, pus_TC_13_10_createIntermediateUplinkPartReport);
	if (PUS_NO_ERROR != error)
	{
		return PUS_GET_ERROR();
	}

	//Set data field
	pus_setTcDataKind(outTc, pus_TC_DATA_ST_13_X);
	pus_tc_tm_13_X_setTransactionID(outTc, transactionID);
	pus_tc_tm_13_X_setPartSeqNumber(outTc, part_seq_number);
	pus_tc_tm_13_X_setContent(outTc, content);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tc_13_11_createLastUplinkPart(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
	int transactionID, int part_seq_number, char* content)
{
	if (NULL == outTc || NULL == transactionID || NULL == part_seq_number || NULL == content)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pusError_t error = pus_tc_13_X_createDefaultTCPacket(outTc, apid, sequenceCount, pus_TC_13_11_createLastUplinkPartReport);
	if (PUS_NO_ERROR != error)
	{
		return PUS_GET_ERROR();
	}

	//Set data field
	pus_setTcDataKind(outTc, pus_TC_DATA_ST_13_X);
	pus_tc_tm_13_X_setTransactionID(outTc, transactionID);
	pus_tc_tm_13_X_setPartSeqNumber(outTc, part_seq_number);

	pus_tc_tm_13_X_setContent(outTc, content);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tm_13_16_createLargePacketUplinkAbortionReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusApid_t destination,
	int transactionID, int failureReason)
{
	if (NULL == outTm || NULL == transactionID || NULL == failureReason)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pusError_t error = pus_tm_13_X_createDefaultTMPacket(outTm, apid, sequenceCount, pus_TM_13_16_createLargePacketUplinkAbortReport, destination);
	if (PUS_NO_ERROR != error)
	{
		return PUS_GET_ERROR();
	}

	//Set data field
	pus_setTmDataKind(outTm, pus_TM_DATA_ST_13_16);
	pus_tc_tm_13_X_setTransactionID(outTm, transactionID);
	pus_tm_13_16_setFailureReason(outTm, failureReason);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tc_tm_13_splitContent(char* split_content, char* content, int seqNum)
{
	if (NULL == split_content || NULL == content)
	{
		return PUS_ERROR_NULLPTR;
	}

	for (int i = 0; i < pus_ST13_MAX_PART_SIZE; i++)
	{
		split_content[i] = content[pus_ST13_MAX_PART_SIZE * seqNum + i];
	}

	return PUS_NO_ERROR;
}

pusError_t pus_tc_tm_13_mergeContent(char* content, pusPacket_t* packet, int* length)
{
	if (NULL == packet || NULL == content)
	{
		return PUS_ERROR_NULLPTR;
	}

	char packet_content[pus_ST13_MAX_PART_SIZE] = { 0 };
	pus_tc_tm_13_X_getContent(packet_content, packet);

	int offset = *length;

	for (int i = 0; i < pus_ST13_MAX_PART_SIZE ; i++)
	{
		content[offset + i] = packet_content[i];
	}

	*length += pus_ST13_MAX_PART_SIZE;
	return PUS_NO_ERROR;
}

//
//Getter & Setter
//


pusError_t pus_tc_tm_13_X_setTransactionID(pusPacket_t* outTc, const int transactionID)
{
	if (NULL == outTc || NULL == transactionID)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_1_createFirstDownlinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_2_createIntermediateDownlinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_3_createLastDownlinkPartReport))
	{
		outTc->data.u.tmData.data.u.st_13_X.transactionID = transactionID;
		return PUS_NO_ERROR;
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_16_createLargePacketUplinkAbortReport))
	{
		outTc->data.u.tmData.data.u.st_13_16.transactionID = transactionID;
		return PUS_NO_ERROR;
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTc, pus_TC_13_9_createFirstUplinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTc, pus_TC_13_10_createIntermediateUplinkPartReport)||
		PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTc, pus_TC_13_11_createLastUplinkPartReport))
	{
		outTc->data.u.tcData.data.u.st_13_X.transactionID = transactionID;
		return PUS_NO_ERROR;
	}
	return PUS_GET_ERROR();
}

pusError_t pus_tc_tm_13_X_getTransactionID(int* transactionID, const pusPacket_t* outTc)
{
	if (NULL == outTc || NULL == transactionID)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_1_createFirstDownlinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_2_createIntermediateDownlinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_3_createLastDownlinkPartReport))
	{
		*transactionID = outTc->data.u.tmData.data.u.st_13_X.transactionID;
		return PUS_NO_ERROR;
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_16_createLargePacketUplinkAbortReport))
	{
		*transactionID = outTc->data.u.tmData.data.u.st_13_16.transactionID;
		return PUS_NO_ERROR;
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTc, pus_TC_13_9_createFirstUplinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTc, pus_TC_13_10_createIntermediateUplinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTc, pus_TC_13_11_createLastUplinkPartReport))
	{
		*transactionID = outTc->data.u.tcData.data.u.st_13_X.transactionID;
		return PUS_NO_ERROR;
	}
	return PUS_GET_ERROR();
}

pusError_t pus_tc_tm_13_X_setPartSeqNumber(pusPacket_t* outTc, const int part_seq_number)
{
	if (NULL == outTc || NULL == part_seq_number)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_1_createFirstDownlinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_2_createIntermediateDownlinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_3_createLastDownlinkPartReport))
	{
		outTc->data.u.tmData.data.u.st_13_X.part_seq_number = part_seq_number;
		return PUS_NO_ERROR;
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTc, pus_TC_13_9_createFirstUplinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTc, pus_TC_13_10_createIntermediateUplinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTc, pus_TC_13_11_createLastUplinkPartReport))
	{
		outTc->data.u.tcData.data.u.st_13_X.part_seq_number = part_seq_number;
		return PUS_NO_ERROR;
	}
	return PUS_GET_ERROR();
}

pusError_t pus_tc_tm_13_X_getPartSeqNumber(int* part_seq_number, const pusPacket_t* outTc)
{
	if (NULL == outTc || NULL == part_seq_number)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_1_createFirstDownlinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_2_createIntermediateDownlinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_3_createLastDownlinkPartReport))
	{
		*part_seq_number = outTc->data.u.tmData.data.u.st_13_X.part_seq_number;
		return PUS_NO_ERROR;
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTc, pus_TC_13_9_createFirstUplinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTc, pus_TC_13_10_createIntermediateUplinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTc, pus_TC_13_11_createLastUplinkPartReport))
	{
		*part_seq_number = outTc->data.u.tcData.data.u.st_13_X.part_seq_number;
		return PUS_NO_ERROR;
	}
	return PUS_GET_ERROR();
}

pusError_t pus_tc_tm_13_X_setContent(pusPacket_t* outTc, const char* content)
{
	if (NULL == outTc || NULL == content)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_1_createFirstDownlinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_2_createIntermediateDownlinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_3_createLastDownlinkPartReport))
	{
		outTc->data.u.tmData.data.u.st_13_X.content = (char*) calloc(pus_ST13_MAX_PART_SIZE, sizeof(char));

		for (int i = 0; i < pus_ST13_MAX_PART_SIZE; i++) 
		{
			outTc->data.u.tmData.data.u.st_13_X.content[i] = content[i];
		}
		return PUS_NO_ERROR;
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTc, pus_TC_13_9_createFirstUplinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTc, pus_TC_13_10_createIntermediateUplinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTc, pus_TC_13_11_createLastUplinkPartReport))
	{
		outTc->data.u.tcData.data.u.st_13_X.content = (char*) calloc(pus_ST13_MAX_PART_SIZE, sizeof(char));

		for (int i = 0; i < pus_ST13_MAX_PART_SIZE; i++)
		{
			outTc->data.u.tcData.data.u.st_13_X.content[i] = content[i];
		}
		return PUS_NO_ERROR;
	}
	return PUS_GET_ERROR();
}

pusError_t pus_tc_tm_13_X_getContent(char* content, const pusPacket_t* outTc)
{
	if (NULL == outTc || NULL == content)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_1_createFirstDownlinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_2_createIntermediateDownlinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTc, pus_TM_13_3_createLastDownlinkPartReport))
	{
		for (int i = 0; i < pus_ST13_MAX_PART_SIZE; i++) {
			content[i] = outTc->data.u.tmData.data.u.st_13_X.content[i];
		}
		return PUS_NO_ERROR;
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTc, pus_TC_13_9_createFirstUplinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTc, pus_TC_13_10_createIntermediateUplinkPartReport) ||
		PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTc, pus_TC_13_11_createLastUplinkPartReport))
	{
		for (int i = 0; i < pus_ST13_MAX_PART_SIZE; i++) {
			content[i] = outTc->data.u.tcData.data.u.st_13_X.content[i];
		}
		return PUS_NO_ERROR;
	}
	return PUS_GET_ERROR();
}

pusError_t pus_tm_13_16_setFailureReason(pusPacket_t* outTm, const int failureReason)
{
	if (NULL == outTm)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST13TC(outTm, pus_TM_13_16_createLargePacketUplinkAbortReport))
	{
		outTm->data.u.tmData.data.u.st_13_16.failure_reason = failureReason;
		return PUS_NO_ERROR;
	}
	return PUS_GET_ERROR();
}

pusError_t pus_tm_13_16_getFailureReason(int* failureReason, const pusPacket_t* outTm)
{
	if (NULL == outTm)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST13TM(outTm, pus_TM_13_16_createLargePacketUplinkAbortReport))
	{
		*failureReason = outTm->data.u.tmData.data.u.st_13_16.failure_reason;
		return PUS_NO_ERROR;
	}
	return PUS_GET_ERROR();
}

//
// Parameter checking
//

pusError_t pus_expectSt13Tm(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function)
{
	pusError_t expectResult = pus_expectTmHeader(packet, function, (pusErrorData_t) { .integer = 0 });
	if (PUS_NO_ERROR == expectResult)
	{
		pusService_t service = pus_getTmService(packet);
		pusSubservice_t subtype = pus_getTmSubtype(packet);

		if (service != pus_ST13_largePacketTransfer)
		{
			//pus_setError(PUS_ERROR_TM_SERVICE, function, (pusErrorData_t) { .integer = service });
			return PUS_ERROR_TM_SERVICE;
		}

		if (expectedSubtype == pusSubtype_NONE)
		{
			// Check that subtype corresponds to ST[13]
			if ((subtype != pus_TM_13_1_createFirstDownlinkPartReport) &&
				(subtype != pus_TM_13_2_createIntermediateDownlinkPartReport) &&
				(subtype != pus_TM_13_3_createLastDownlinkPartReport) &&
				(subtype != pus_TM_13_16_createLargePacketUplinkAbortReport))
			{
				//pus_setError(PUS_ERROR_TM_SUBTYPE, function, (pusErrorData_t) { .integer = subtype });
				return PUS_ERROR_TM_SUBTYPE;
			}
		}
		else
		{
			// Check that subtype is as expected
			if (subtype != expectedSubtype)
			{
				//pus_setError(PUS_ERROR_TM_SUBTYPE, function, (pusErrorData_t) { .integer = subtype });
				return PUS_ERROR_TM_SUBTYPE;

			}
		}

		pusPacketDataKind_t kind = pus_getTmDataKind(packet);
		if ((kind != pus_TM_DATA_ST_13_X) &&
			(kind != pus_TM_DATA_ST_13_16))
		{
			//pus_setError(PUS_ERROR_TM_KIND, function, (pusErrorData_t) { .integer = kind });
			return PUS_ERROR_TM_KIND;
		}

		return PUS_NO_ERROR;
	}
	else
	{
		return expectResult;
	}
}

pusError_t pus_expectSt13Tc(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function)
{
	pusError_t expectResult = pus_expectTcHeader(packet, function, (pusErrorData_t) { .integer = 0 });
	if (PUS_NO_ERROR == expectResult)
	{
		pusService_t service = pus_getTcService(packet);
		pusSubservice_t subtype = pus_getTcSubtype(packet);

		if (service != pus_ST13_largePacketTransfer)
		{
			//pus_setError(PUS_ERROR_TC_SERVICE, function, (pusErrorData_t) { .integer = service });
			return PUS_ERROR_TC_SERVICE;
		}

		if (expectedSubtype == pusSubtype_NONE)
		{
			// Check that subtype corresponds to ST[13]
			if ((subtype != pus_TC_13_9_createFirstUplinkPartReport) &&
				(subtype != pus_TC_13_10_createIntermediateUplinkPartReport) &&
				(subtype != pus_TC_13_11_createLastUplinkPartReport))
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
		if ((kind != pus_TC_DATA_ST_13_X))
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

