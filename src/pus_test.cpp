#include "pus_test.h"


void print_packet(pusPacket_t* outTm)
{
	printf("\n\nPrimary Header\n");
	printf("-----------------\n");
	printf("Packet Version:\t\t%d\n", outTm->packetVersion);
	printf("APID:\t\t\t%d\n", outTm->apid);
	printf("Sequence Flags:\t\t%d\n", outTm->sequenceFlags);
	printf("Sequence Count:\t\t%d\n", outTm->sequenceCount);
	printf("Data Length:\t\t%d\n", outTm->dataLength);
	printf("Packet Type:\t\t%d\n", outTm->packetType);
	printf("-----------------\n");

	switch (outTm->packetType)
	{
	case asn1Sccpus_TC:
	{
		printf("\nTC Secondary Header\n");
		printf("-----------------\n");
		printf("PUS Version:\t\t%d\n", outTm->data.u.tcData.header.pusVersion);
		printf("Service ID:\t\t%d\n", outTm->data.u.tcData.header.serviceId);
		printf("Subtype ID:\t\t%d\n", outTm->data.u.tcData.header.subtypeId);
		printf("Source:\t\t\t%d\n", outTm->data.u.tcData.header.source);
		printf("-----------------\n");
		break;
	}

	case asn1Sccpus_TM:
	{
		printf("\nTM Secondary Header\n");
		printf("-----------------\n");
		printf("PUS Version:\t\t%d\n", outTm->data.u.tmData.header.pusVersion);
		printf("Time Ref Status:\t%d\n", outTm->data.u.tmData.header.timeRefStatus);
		printf("Service ID:\t\t%d\n", outTm->data.u.tmData.header.serviceId);
		printf("Subtype ID:\t\t%d\n", outTm->data.u.tmData.header.subtypeId);
		printf("Msg Type Counter:\t%d\n", outTm->data.u.tmData.header.msgTypeCount);
		printf("Destination:\t\t%d\n", outTm->data.u.tmData.header.destination);
		printf("TV Sec:\t\t\t%d\n", outTm->data.u.tmData.header.time.tv_sec);
		printf("TV NSec:\t\t%d\n", outTm->data.u.tmData.header.time.tv_nsec);
		printf("-----------------\n");
		break;
	}
	}

}

void assert_primary_header(pusPacket_t* outTm1, pusPacket_t* outTm2)
{
	assert(outTm1->packetVersion == outTm2->packetVersion);
	assert(outTm1->apid == outTm2->apid);
	assert(outTm1->sequenceFlags == outTm2->sequenceFlags);
	assert(outTm1->sequenceCount == outTm2->sequenceCount);
	assert(outTm1->dataLength == outTm2->dataLength);
	printf("Primary Header identical\n");
}

void assert_secondary_tm_header(pusPacket_t* outTm1, pusPacket_t* outTm2)
{
	assert((outTm1->data.kind == PusPacketData_tmData_PRESENT) || (outTm1->data.kind == PusPacketData_tmDataNoHeader_PRESENT));
	assert(outTm1->secondaryHeaderFlag);
	assert(outTm1->data.u.tmData.header.pusVersion == outTm2->data.u.tmData.header.pusVersion);
	assert(outTm1->data.u.tmData.header.timeRefStatus == outTm2->data.u.tmData.header.timeRefStatus);
	assert(outTm1->data.u.tmData.header.serviceId == outTm2->data.u.tmData.header.serviceId);
	assert(outTm1->data.u.tmData.header.subtypeId == outTm2->data.u.tmData.header.subtypeId);
	assert(outTm1->data.u.tmData.header.msgTypeCount == outTm2->data.u.tmData.header.msgTypeCount);
	assert(outTm1->data.u.tmData.header.destination == outTm2->data.u.tmData.header.destination);
	assert(outTm1->data.u.tmData.header.time.tv_sec == outTm2->data.u.tmData.header.time.tv_sec);
	assert(outTm1->data.u.tmData.header.time.tv_nsec == outTm2->data.u.tmData.header.time.tv_nsec);
	printf("Secondary TM Header identical\n");
}

void assert_secondary_tc_header(pusPacket_t* outTm1, pusPacket_t* outTm2)
{
	assert((outTm1->data.kind == PusPacketData_tcData_PRESENT) || (outTm1->data.kind == PusPacketData_tcDataNoHeader_PRESENT));
	assert(outTm1->secondaryHeaderFlag);
	assert(outTm1->data.u.tcData.header.pusVersion == outTm2->data.u.tcData.header.pusVersion);
	assert(outTm1->data.u.tcData.header.serviceId == outTm2->data.u.tcData.header.serviceId);
	assert(outTm1->data.u.tcData.header.subtypeId == outTm2->data.u.tcData.header.subtypeId);
	assert(outTm1->data.u.tcData.header.source == outTm2->data.u.tcData.header.source);
	printf("Secondary TC Header identical\n");
}

int test_tm_service_one()
{
	printf("\nStarted testing TM Service 1\n\n");

	pusPacket_t outTm1;
	pusPacket_t outTm2;
	pusPacket_t dummy;
	pusApid_t apid = 0x88;
	pusSequenceCount_t seq = 0x88;
	pusSt01FailureCode_t fac = 0x88;
	pusStepId_t step = 0x88;
	dummy.apid = 0x88;
	dummy.dataLength = 0x88;
	dummy.sequenceCount = 0x88;
	dummy.sequenceFlags = asn1Sccpus_STANDALONE_PACKET;
	asn1SccPusSt01FailureInfo fai = { 0x88, 0x88, 0x88 };

	//pus_tm_1_1_createAcceptanceReportSuccess(&outTm1, apid, seq, &dummy);
	//pus_tm_1_2_createAcceptanceReportFailure(&outTm1, apid, seq, &dummy, fac, &fai);
	//pus_tm_1_5_createProgressReportSuccess(&outTm1, apid, seq, &dummy, step);
	pus_tm_1_6_createProgressReportFailure(&outTm1, apid, seq, &dummy, step, fac, &fai);

	printf("Sending 1_2 Packet\n");

	char buffer[40000];
	unsigned int length = 0;

	if (encodePacket(&outTm1, buffer, &length) != PUS_NO_ERROR)
	{
		printf("Error sending 1_2 Packet!\n");
		return 1;
	}

	//send(new_socket, buffer, length, 0);


	//if (receivePacket(recv_buffer, &outTm2) != PUS_NO_ERROR)
	//{
	//	printf("Error receiving 1_2 Packet!\n");
	//	return 1;
	//}

	//printf("\nReceived 1_2 Packet\n");
	//assert_primary_header(&outTm1, &outTm2);
	//assert_secondary_tm_header(&outTm1, &outTm2);
	////assert(outTm1.data.u.tmData.data.u.st_1_X == outTm2.data.u.tmData.data.u.st_1_X);
	///*printf("Data Kind: %d   ->   %d\n", outTm1.data.u.tmData.data.kind, outTm2.data.u.tmData.data.kind);
	//printf("\n");
	//printf("Req Packet Version: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_1_X.request.packetVersion, outTm2.data.u.tmData.data.u.st_1_X.request.packetVersion);
	//printf("Req Packet Type: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_1_X.request.packetType, outTm2.data.u.tmData.data.u.st_1_X.request.packetType);
	//printf("Req Sec Header Flag: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_1_X.request.secondaryHeaderFlag, outTm2.data.u.tmData.data.u.st_1_X.request.secondaryHeaderFlag);
	//printf("Req APID: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_1_X.request.apid, outTm2.data.u.tmData.data.u.st_1_X.request.apid);
	//printf("Req Sequence Flags: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_1_X.request.sequenceFlags, outTm2.data.u.tmData.data.u.st_1_X.request.sequenceFlags);
	//printf("Req Sequence Count: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_1_X.request.sequenceCount, outTm2.data.u.tmData.data.u.st_1_X.request.sequenceCount);
	//printf("\n");
	//printf("Step Id: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_1_X.step, outTm2.data.u.tmData.data.u.st_1_X.step);
	//printf("Failure Code: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_1_X.failure.code, outTm2.data.u.tmData.data.u.st_1_X.failure.code);
	//printf("Subcode: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_1_X.failure.info.subcode, outTm2.data.u.tmData.data.u.st_1_X.failure.info.subcode);
	//printf("Data: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_1_X.failure.info.data, outTm2.data.u.tmData.data.u.st_1_X.failure.info.data);
	//printf("Address: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_1_X.failure.info.address, outTm2.data.u.tmData.data.u.st_1_X.failure.info.address);
	//printf("\n");*/

	//int comp = memcmp(&outTm1, &outTm2, 200);
	//char temp[1000];
	//memcpy(temp, &outTm1, 1000);

	//if (comp != 0)
	//{
	//	printf("Memory Compare didn't work!\n");
	//}
	//else
	//{
	//	printf("Memory Compare worked!!!aelfvznawleföaosfh Notice ME!!!!!!!\n");
	//}

	return 0;
}

int test_tc_service_one()
{
	//no tc
	return 0;
}

int test_tm_service_two()
{
	//not implemented
	return 0;
}

int test_tc_service_two()
{
	//not implemented
	return 0;
}

int test_tm_service_three()
{
	printf("\nStarted testing TM Service 3\n\n");


	pusPacket_t outTm1;
	pusPacket_t outTm2;
	pusApid_t apid = 0x88;
	pusSequenceCount_t seq = 0x88;
	pusSt03HousekeepingReportId_t rep = 0x89;
	pusApid_t dst = 0x88;
	pus_st20_paramInfo[1].st03HousekeepingReportEnabled = true;
	pus_st20_paramInfo[2].st03HousekeepingReportEnabled = true;
	pus_st20_params[1] = 3.1415;
	pus_st20_params[2] = -4.2;

	pus_tm_3_25_createHousekeepingReport(&outTm1, apid, seq, rep, dst);

	printf("Sending 3_25 Packet\n");

	char buffer[3000000];
	unsigned int length = 0;

	if (encodePacket(&outTm1, buffer, &length) != PUS_NO_ERROR)
	{
		printf("Error sending 3_25 Packet!\n");
		return 1;
	}

	printf("Length: %d\n", length);
	for (int i = 0; i < length; i++)
	{
		printf("%hhx ", (unsigned char)buffer[i]);
	}
	printf("\n!\n");

	if (decodePacket(buffer, &outTm2) != PUS_NO_ERROR)
	{
		printf("Error receiving 3_25 Packet!\n");
		return 1;
	}

	printf("\nReceived 3_25 Packet\n");
	//assert_primary_header(&outTm1, &outTm2);
	assert_secondary_tm_header(&outTm1, &outTm2);
	printf("\n");
	printf("%d\t%d\n", outTm1.data.u.tmData.data.u.st_3_X.reportId, outTm2.data.u.tmData.data.u.st_3_X.reportId);
	printf("%d\t%d\n", outTm1.data.u.tmData.data.u.st_3_X.parameters.nCount, outTm2.data.u.tmData.data.u.st_3_X.parameters.nCount);
	printf("%f\t%f\n", outTm1.data.u.tmData.data.u.st_3_X.parameters.arr[0], outTm2.data.u.tmData.data.u.st_3_X.parameters.arr[0]);
	printf("%f\t%f\n", outTm1.data.u.tmData.data.u.st_3_X.parameters.arr[1], outTm2.data.u.tmData.data.u.st_3_X.parameters.arr[1]);

	return 0;
}

int test_tc_service_three()
{

	return 0;
}

int test_tm_service_four()
{
	//not implemented
	return 0;
}

int test_tc_service_four()
{
	//not implemented
	return 0;
}

int test_tm_service_five()
{

	printf("\nStarted testing TM Service 5\n\n");

	pusPacket_t outTm1;
	pusPacket_t outTm2;
	pusApid_t apid = 0x88;
	pusApid_t dst = 0x88;
	pusSequenceCount_t seq = 0x88;
	pusSt05Event_t evt = { 0x88, {0x88, 0x88} };
	const pusSt05Event_t events[3] = { {7, {8, 9}}, {10, {11, 12}}, {1, {2, 3}} };

	pus_tm_5_8_reportDisabledEvents(&outTm1, apid, seq, dst, events, 3);

	printf("Sending 5_8 Packet\n");

	char buffer[40000];
	unsigned int length = 0;

	if (encodePacket(&outTm1, buffer, &length) != PUS_NO_ERROR)
	{
		printf("Error sending 5_8 Packet!\n");
		return 1;
	}

	if (decodePacket(buffer, &outTm2) != PUS_NO_ERROR)
	{
		printf("Error receiving 5_8 Packet!\n");
		return 1;
	}

	printf("\nReceived 5_8 Packet\n");
	assert_primary_header(&outTm1, &outTm2);
	assert_secondary_tm_header(&outTm1, &outTm2);
	printf("\n");
	printf("Data Kind: %d   ->   %d\n", outTm1.data.u.tmData.data.kind, outTm2.data.u.tmData.data.kind);
	printf("\n");
	printf("NCount: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_5_8.nCount, outTm2.data.u.tmData.data.u.st_5_8.nCount);

	for (int i = 0; i < outTm1.data.u.tmData.data.u.st_5_8.nCount; i++)
	{
		printf("Event ID: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_5_8.arr[i].eventId, outTm2.data.u.tmData.data.u.st_5_8.arr[i].eventId);
		printf("Data1: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_5_8.arr[i].data.data1, outTm2.data.u.tmData.data.u.st_5_8.arr[i].data.data1);
		printf("Data2: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_5_8.arr[i].data.data2, outTm2.data.u.tmData.data.u.st_5_8.arr[i].data.data2);
	}

	return 0;
}

int test_tc_service_five()
{

	printf("\nStarted testing TC Service 5\n\n");

	pusPacket_t outTm1;
	pusPacket_t outTm2;
	pusPacket_t dummy;
	pusApid_t apid = 0x2b;
	pusSequenceCount_t seq = 0x88;
	const pusSt05Event_t events[3] = { {7, {8, 9}}, {10, {11, 12}}, {1, {2, 3}} };


	pus_tc_5_5_enableEventReport(&outTm1, apid, seq, events, 3);

	printf("Sending 5_5 Packet\n");

	char buffer[40000];
	unsigned int length = 0;

	if (encodePacket(&outTm1, buffer, &length) != PUS_NO_ERROR)
	{
		printf("Error sending 5_5 Packet!\n");
		return 1;
	}

	print_packet(&outTm1);

	//send(new_socket, buffer, length, 0);

	/*if (receivePacket(buffer, &outTm2) != PUS_NO_ERROR)
	{
		printf("Error receiving 5_5 Packet!\n");
		return 1;
	}

	printf("\nReceived 5_6 Packet\n");
	assert_primary_header(&outTm1, &outTm2);
	assert_secondary_tc_header(&outTm1, &outTm2);
	printf("\n");
	printf("Data Kind: %d   ->   %d\n", outTm1.data.u.tmData.data.kind, outTm2.data.u.tmData.data.kind);
	printf("\n");
	printf("nCount: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_5_X.nCount, outTm2.data.u.tcData.data.u.st_5_X.nCount);
	for (int i = 0; i < outTm1.data.u.tcData.data.u.st_5_X.nCount; i++)
	{
		printf("Event ID: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_5_X.arr[i].eventId, outTm2.data.u.tcData.data.u.st_5_X.arr[i].eventId);
		printf("Data 1: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_5_X.arr[i].data.data1, outTm2.data.u.tcData.data.u.st_5_X.arr[i].data.data1);
		printf("Data 2: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_5_X.arr[i].data.data2, outTm2.data.u.tcData.data.u.st_5_X.arr[i].data.data2);
	}
	printf("\n");

	int comp = memcmp(&outTm1, &outTm2, 200);
	char temp[1000];
	memcpy(temp, &outTm1, 1000);

	if (comp != 0)
	{
		printf("Memory Compare didn't work!\n");
	}
	else
	{
		printf("Memory Compare worked!!!aelfvznawleföaosfh Notice ME!!!!!!!\n");
	}*/

	return length;
}

int test_tm_service_six()
{
	//not implemented
	return 0;
}

int test_tc_service_six()
{
	//not implemented
	return 0;
}

int test_tm_service_seven()
{
	//reserved
	return 0;
}

int test_tc_service_seven()
{
	//not implemented
	return 0;
}

int test_tm_service_eight()
{
	//no tm
	return 0;
}

int test_tc_service_eight()
{

	printf("\nStarted testing TC Service 8\n\n");

	pusPacket_t outTm1;
	pusPacket_t outTm2;
	pusApid_t apid = 0x88;
	pusSequenceCount_t seq = 0x88;
	pusSt08FunctiontId_t fid = 0x88;


	pus_tc_8_1_createPerformFuctionRequest(&outTm1, apid, seq, fid);

	printf("Sending 8_1 Packet\n");

	char buffer[40000];
	unsigned int length = 0;

	if (encodePacket(&outTm1, buffer, &length) != PUS_NO_ERROR)
	{
		printf("Error sending 8_1 Packet!\n");
		return 1;
	}

	//send(new_socket, buffer, length, 0);

	/*if (receivePacket(buffer, &outTm2) != PUS_NO_ERROR)
	{
		printf("Error receiving 8_1 Packet!\n");
		return 1;
	}

	printf("\nReceived 8_1 Packet\n");
	assert_primary_header(&outTm1, &outTm2);
	assert_secondary_tc_header(&outTm1, &outTm2);
	printf("\n");
	printf("Data Kind: %d   ->   %d\n", outTm1.data.u.tcData.data.kind, outTm2.data.u.tcData.data.kind);
	printf("\n");
	printf("Function ID: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_8_1.functionId, outTm2.data.u.tcData.data.u.st_8_1.functionId);

	int comp = memcmp(&outTm1, &outTm2, 200);
	char temp[1000];
	memcpy(temp, &outTm1, 1000);

	if (comp != 0)
	{
		printf("Memory Compare didn't work!\n");
	}
	else
	{
		printf("Memory Compare worked!!!aelfvznawleföaosfh Notice ME!!!!!!!\n");
	}*/

	return length;
}

int test_tm_service_nine()
{
	printf("\nStarted testing TM Service 9\n\n");

	pusPacket_t outTm1;
	pusPacket_t outTm2;
	pusApid_t apid = 0x88;
	pusSequenceCount_t seq = 0x88;
	pusTime_t time = { 0x88, 0x88 };
	pusSt09ExponentialRate_t exp = 0x88;

	pus_tm_9_2_createCucTimeReport(&outTm1, apid, seq, &time, &exp);

	printf("Sending 9_2 Packet\n");

	char buffer[40000];
	unsigned int length = 0;

	if (encodePacket(&outTm1, buffer, &length) != PUS_NO_ERROR)
	{
		printf("Error sending 9_2 Packet!\n");
		return 1;
	}

	if (decodePacket(buffer, &outTm2) != PUS_NO_ERROR)
	{
		printf("Error receiving 9_2 Packet!\n");
		return 1;
	}

	printf("\nReceived 9_2 Packet\n");
	assert_primary_header(&outTm1, &outTm2);
	printf("\n");
	printf("Data Kind: %d   ->   %d\n", outTm1.data.u.tmDataNoHeader.kind, outTm2.data.u.tmDataNoHeader.kind);
	printf("\n");
	printf("Exponential Rate: %d   ->   %d\n", outTm1.data.u.tmDataNoHeader.u.st_9_2_3.exponentialRate, outTm2.data.u.tmDataNoHeader.u.st_9_2_3.exponentialRate);
	printf("Time (sec): %d   ->   %d\n", outTm1.data.u.tmDataNoHeader.u.st_9_2_3.time.tv_sec, outTm2.data.u.tmDataNoHeader.u.st_9_2_3.time.tv_sec);
	printf("Time (nsec) Rate: %d   ->   %d\n", outTm1.data.u.tmDataNoHeader.u.st_9_2_3.time.tv_nsec, outTm2.data.u.tmDataNoHeader.u.st_9_2_3.time.tv_nsec);

}

int test_tc_service_nine()
{
	printf("\nStarted testing TC Service 9\n\n");

	pusPacket_t outTm1;
	pusPacket_t outTm2;
	pusApid_t apid = 0x88;
	pusSequenceCount_t seq = 0x88;
	pusSt09ExponentialRate_t exp = 0x08;


	pus_tc_9_1_createSetTimeReportRate(&outTm1, apid, seq, exp);

	printf("Sending 9_1 Packet\n");

	char buffer[40000];
	unsigned int length = 0;

	if (encodePacket(&outTm1, buffer, &length) != PUS_NO_ERROR)
	{
		printf("Error sending 9_1 Packet!\n");
		return 1;
	}

	//send(new_socket, buffer, length, 0);

	/*if (receivePacket(buffer, &outTm2) != PUS_NO_ERROR)
	{
		printf("Error receiving 9_1 Packet!\n");
		return 1;
	}

	printf("\nReceived 9_1 Packet\n");
	assert_primary_header(&outTm1, &outTm2);
	assert_secondary_tc_header(&outTm1, &outTm2);
	printf("\n");
	printf("Data Kind: %d   ->   %d\n", outTm1.data.u.tcData.data.kind, outTm2.data.u.tcData.data.kind);
	printf("\n");
	printf("Exp Rate: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_9_1.exponentialRate, outTm2.data.u.tcData.data.u.st_9_1.exponentialRate);

	int comp = memcmp(&outTm1, &outTm2, 200);
	char temp[1000];
	memcpy(temp, &outTm1, 1000);

	if (comp != 0)
	{
		printf("Memory Compare didn't work!\n");
	}
	else
	{
		printf("Memory Compare worked!!!aelfvznawleföaosfh Notice ME!!!!!!!\n");
	}*/

	return length;
}

int test_tm_service_ten()
{
	//reserved
	return 0;
}

int test_tc_service_ten()
{
	//reserved
	return 0;
}

int test_tm_service_eleven()
{
	//no tm
	return 0;
}

int test_tc_service_eleven()
{
}

int test_tm_service_twelve()
{
	//no tm
	return 0;
}

int test_tc_service_twelve()
{
	printf("\nStarted testing TC Service 12\n\n");

	pusPacket_t outTm1;
	pusPacket_t outTm2;
	pusApid_t apid = 0x88;
	pusSequenceCount_t seq = 0x88;
	pusSt09ExponentialRate_t exp = 0x08;
	pusSt12PmonId_t pmon[4] = { 4, 8, 16, 32 };


	pus_tc_12_1_createEnableParameterMonitoringDefinitions(&outTm1, apid, seq, pmon, 4);

	printf("Sending 12_1 Packet\n");

	char buffer[40000];
	unsigned int length = 0;

	if (encodePacket(&outTm1, buffer, &length) != PUS_NO_ERROR)
	{
		printf("Error sending 12_1 Packet!\n");
		return 1;
	}

	//send(new_socket, buffer, length, 0);

	/*if (receivePacket(buffer, &outTm2) != PUS_NO_ERROR)
	{
		printf("Error receiving 12_1 Packet!\n");
		return 1;
	}

	printf("\nReceived 12_1 Packet\n");
	assert_primary_header(&outTm1, &outTm2);
	assert_secondary_tc_header(&outTm1, &outTm2);
	printf("\n");
	printf("Data Kind: %d   ->   %d\n", outTm1.data.u.tcData.data.kind, outTm2.data.u.tcData.data.kind);
	printf("\n");
	printf("NCount: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_12_1_2.nCount, outTm2.data.u.tcData.data.u.st_12_1_2.nCount);
	printf("\n");
	for (int i = 0; i < outTm1.data.u.tcData.data.u.st_12_1_2.nCount; i++)
	{
		printf("pmonID %d: %d   ->   %d\n", i, outTm1.data.u.tcData.data.u.st_12_1_2.pmonId[i], outTm2.data.u.tcData.data.u.st_12_1_2.pmonId[i]);
	}


	int comp = memcmp(&outTm1, &outTm2, 200);
	char temp[1000];
	memcpy(temp, &outTm1, 1000);

	if (comp != 0)
	{
		printf("Memory Compare didn't work!\n");
	}
	else
	{
		printf("Memory Compare worked!!!aelfvznawleföaosfh Notice ME!!!!!!!\n");
	}*/

	return length;
}

int test_tm_service_thirteen()
{
	//implicit
	return 0;
}

int test_tc_service_thirteen()
{
	//implicit
	return 0;
}

int test_tm_service_fourteen()
{
	//not implemented
	return 0;
}

int test_tc_service_fourteen()
{
	//not implemented
	return 0;
}

int test_tm_service_fifteen()
{
	//not implemented
	return 0;
}

int test_tc_service_fifteen()
{
	//not implemented
	return 0;
}

int test_tm_service_sixteen()
{
	//reserved
	return 0;
}

int test_tc_service_sixteen()
{
	//reserved
	return 0;
}

int test_tm_service_seventeen()
{
	return 0;
}

int test_tc_service_seventeen()
{
	return 0;
}

int test_tm_service_eighteen()
{
	//no tm
	return 0;
}

int test_tc_service_eighteen()
{
	printf("\nStarted testing TC Service 18\n\n");

	pusPacket_t outTm1;
	pusPacket_t outTm2;
	pusApid_t apid = 0x88;
	pusSequenceCount_t seq = 0x88;
	pusSt09ExponentialRate_t exp = 0x08;
	const pusSt18ObcpId_t oId[10] = { 4, 8, 16, 32, 64, 1, 2, 3, 4, 5 };
	const pusSt18ObcpCode_t oCo[17] = { 17, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 } };
	pusSt18ObservabilityLevel_t obs = asn1Sccstep;
	pusSt18ObcpStepId_t step = 0x80;
	asn1SccPusByteArray rep = { 4, "this" };
	asn1SccPusByteArray file = { 5, "thaat" };

	//pus_tc_18_1_createLoadObcpDirectRequest(&outTm1, apid, seq, oId, oCo);
	//pus_tc_18_2_createUnloadObcpRequest(&outTm1, apid, seq, oId);
	//pus_tc_18_3_createActivateObcpRequest(&outTm1, apid, seq, oId, obs);
	//pus_tc_18_4_createStopObcpRequest(&outTm1, apid, seq, oId, step);
	pus_tc_18_13_createLoadObcpReferenceRequest(&outTm1, apid, seq, oId, &rep, &file);

	printf("Sending 18_1 Packet\n");

	char buffer[40000];
	unsigned int length = 0;

	if (encodePacket(&outTm1, buffer, &length) != PUS_NO_ERROR)
	{
		printf("Error sending 18_1 Packet!\n");
		return 1;
	}

	if (decodePacket(buffer, &outTm2) != PUS_NO_ERROR)
	{
		printf("Error receiving 18_1 Packet!\n");
		return 1;
	}

	printf("\nReceived 18_1 Packet\n");
	print_packet(&outTm1);
	print_packet(&outTm2);
	assert_primary_header(&outTm1, &outTm2);
	assert_secondary_tc_header(&outTm1, &outTm2);
	printf("\n");
	printf("Data Kind: %d   ->   %d\n", outTm1.data.u.tcData.data.kind, outTm2.data.u.tcData.data.kind);
	printf("\n");
	for (int i = 0; i < 10; i++)
	{
		printf("ObcpId %d: %d   ->   %d\n", i, outTm1.data.u.tcData.data.u.st_18_13.obcpId.arr[i], outTm2.data.u.tcData.data.u.st_18_13.obcpId.arr[i]);
	}
	printf("\n");
	/*printf("NCount: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_18_1.obcpCode.nCount, outTm2.data.u.tcData.data.u.st_18_1.obcpCode.nCount);
	printf("\n");
	for (int i = 0; i < outTm1.data.u.tcData.data.u.st_18_1.obcpCode.nCount; i++)
	{
		printf("ObcpCode %d: %d   ->   %d\n", i, outTm1.data.u.tcData.data.u.st_18_1.obcpCode.arr[i], outTm2.data.u.tcData.data.u.st_18_1.obcpCode.arr[i]);
	}*/
	
	//printf("Observability Level: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_18_3.observabilityLevel, outTm2.data.u.tcData.data.u.st_18_3.observabilityLevel);
	//printf("Step ID: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_18_4_5.stepId, outTm2.data.u.tcData.data.u.st_18_4_5.stepId);
	
	
	//assert(outTm1.data.u.tcData.data.u.st_18_13.repository.arr == outTm2.data.u.tcData.data.u.st_18_13.repository.arr);
	printf("Repository: %s   ->   %s\n", outTm1.data.u.tcData.data.u.st_18_13.repository.arr, outTm2.data.u.tcData.data.u.st_18_13.repository.arr);
	printf("Repository: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_18_13.repository.nCount, outTm2.data.u.tcData.data.u.st_18_13.repository.nCount);
	printf("File: %s   ->   %s\n", outTm1.data.u.tcData.data.u.st_18_13.fileName.arr, outTm2.data.u.tcData.data.u.st_18_13.fileName.arr);
	printf("File: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_18_13.fileName.nCount, outTm2.data.u.tcData.data.u.st_18_13.fileName.nCount);
	


	//int comp = memcmp(&outTm1, &outTm2, 200);
	//char temp[1000];
	//memcpy(temp, &outTm1, 1000);

	//if (comp != 0)
	//{
	//	printf("Memory Compare didn't work!\n");
	//}
	//else
	//{
	//	printf("Memory Compare worked!!!aelfvznawleföaosfh Notice ME!!!!!!!\n");
	//}

	return length;
}

int test_tm_service_nineteen()
{
	//no tm
	return 0;
}

int test_tc_service_nineteen()
{
	printf("\nStarted testing TC Service 19\n\n");

	pusPacket_t outTm1;
	pusPacket_t outTm2;
	pusApid_t apid = 0x88;
	pusSequenceCount_t seq = 0x88;
	pusSt09ExponentialRate_t exp = 0x08;
	pusPacket_t a, b, c;
	pusPacket_t act[3] = { a, b, c };
	pusSt05EventId_t eve[3] = { 7, 8, 9 };

	//pus_tc_19_1_createAddEventActionDefinitionsRequest(&outTm1, apid, seq, eve, act, 3);
	pus_tc_19_2_createDeleteEventActionDefinitionsRequest(&outTm1, apid, seq, eve, 3);

	printf("Sending 19_1 Packet\n");

	char buffer[40000];
	unsigned int length = 0;

	if (encodePacket(&outTm1, buffer, &length) != PUS_NO_ERROR)
	{
		printf("Error sending 19_1 Packet!\n");
		return 1;
	}

	//send(new_socket, buffer, length, 0);

	//if (receivePacket(buffer, &outTm2) != PUS_NO_ERROR)
	//{
	//	printf("Error receiving 19_1 Packet!\n");
	//	return 1;
	//}

	//printf("\nReceived 19_1 Packet\n");
	//assert_primary_header(&outTm1, &outTm2);
	//assert_secondary_tc_header(&outTm1, &outTm2);
	//printf("\n");
	//printf("Data Kind: %d   ->   %d\n", outTm1.data.u.tcData.data.kind, outTm2.data.u.tcData.data.kind);
	//printf("\n");
	///*printf("NCount: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_19_1.nCount, outTm2.data.u.tcData.data.u.st_19_1.nCount);
	//for (int i = 0; i < outTm1.data.u.tcData.data.u.st_19_1.nCount; i++)
	//{
	//	printf("ObcpId %d: %d   ->   %d\n", i, outTm1.data.u.tcData.data.u.st_18_2_6_12.obcpId.arr[i], outTm2.data.u.tcData.data.u.st_18_2_6_12.obcpId.arr[i]);
	//	printf("ObcpId %d: %d   ->   %d\n", i, outTm1.data.u.tcData.data.u.st_18_2_6_12.obcpId.arr[i], outTm2.data.u.tcData.data.u.st_18_2_6_12.obcpId.arr[i]);
	//}*/
	//printf("NCount: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_19_X.nCount, outTm2.data.u.tcData.data.u.st_19_X.nCount);
	//for (int i = 0; i < outTm1.data.u.tcData.data.u.st_19_X.nCount; i++)
	//{
	//	printf("EventID %d: %d   ->   %d\n", i, outTm1.data.u.tcData.data.u.st_19_X.eventId[i], outTm2.data.u.tcData.data.u.st_19_X.eventId[i]);
	//}

	//printf("\n");



	//int comp = memcmp(&outTm1, &outTm2, 200);
	//char temp[1000];
	//memcpy(temp, &outTm1, 1000);

	//if (comp != 0)
	//{
	//	printf("Memory Compare didn't work!\n");
	//}
	//else
	//{
	//	printf("Memory Compare worked!!!aelfvznawleföaosfh Notice ME!!!!!!!\n");
	//}

	return length;
}

int test_tm_service_twenty()
{
	printf("\nStarted testing TM Service 20\n\n");

	pusPacket_t outTm1;
	pusPacket_t outTm2;
	pusApid_t apid = 0x88;
	pusApid_t dst = 0x89;
	pusSequenceCount_t seq = 0x8a;
	pusSt20OnBoardParamId_t par[7];
	pusStoredParam_t val[7];

	for (int i = 0; i < 7; i++)
	{
		par[i] = 0x80 + i;
		val[i] = 0x70 + i;
	}

	pus_tm_20_2_createParameterValueReport(&outTm1, apid, seq, dst, par, val, 7);

	printf("Sending 20_2 Packet\n");

	char buffer[40000];
	unsigned int length = 0;


	if (encodePacket(&outTm1, buffer, &length) != PUS_NO_ERROR)
	{
		printf("Error sending 20_2 Packet!\n");
		return 1;
	}

	if (decodePacket(buffer, &outTm2) != PUS_NO_ERROR)
	{
		printf("Error receiving 20_2 Packet!\n");
		return 1;
	}

	printf("\nReceived 20_2 Packet\n");
	assert_primary_header(&outTm1, &outTm2);
	assert_secondary_tm_header(&outTm1, &outTm2);
	printf("\n");
	printf("Data Kind: %d   ->   %d\n", outTm1.data.u.tmData.data.kind, outTm2.data.u.tmData.data.kind);
	printf("\n");
	printf("NCount: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_20_2_3.nCount, outTm2.data.u.tmData.data.u.st_20_2_3.nCount);
	for (int i = 0; i < outTm1.data.u.tmData.data.u.st_20_2_3.nCount; i++)
	{
		printf("Parameter ID: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_20_2_3.arr[i], outTm2.data.u.tmData.data.u.st_20_2_3.arr[i]);
		printf("Parameter Value: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_20_2_3.value[i], outTm2.data.u.tmData.data.u.st_20_2_3.value[i]);
	}
	return 0;
}

int test_tc_service_twenty()
{
	printf("\nStarted testing TC Service 20\n\n");

	pusPacket_t outTm1;
	pusPacket_t outTm2;
	pusApid_t apid = 0x88;
	pusSequenceCount_t seq = 0x88;
	pusSt20OnBoardParamId_t pId[4] = { 7, 5, 3, 1 };
	pusStoredParam_t pVal[4] = { 0x80, 0x88, 0x90, 0xfa };

	//pus_tc_20_1_createParameterValueRequest(&outTm1, apid, seq, pId, 4);
	pus_tc_20_3_createSetParameterValueRequest(&outTm1, apid, seq, pId, pVal, 4);

	printf("Sending 20_1 Packet\n");

	char buffer[40000];
	unsigned int length = 0;

	if (encodePacket(&outTm1, buffer, &length) != PUS_NO_ERROR)
	{
		printf("Error sending 20_1 Packet!\n");
		return 1;
	}

	//send(new_socket, buffer, length, 0);

	/*if (receivePacket(buffer, &outTm2) != PUS_NO_ERROR)
	{
		printf("Error receiving 20_1 Packet!\n");
		return 1;
	}

	printf("\nReceived 20_1 Packet\n");
	assert_primary_header(&outTm1, &outTm2);
	assert_secondary_tc_header(&outTm1, &outTm2);
	printf("\n");
	printf("Data Kind: %d   ->   %d\n", outTm1.data.u.tcData.data.kind, outTm2.data.u.tcData.data.kind);
	printf("\n");
	printf("NCount: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_20_2_3.nCount, outTm2.data.u.tcData.data.u.st_20_2_3.nCount);
	for (int i = 0; i < outTm1.data.u.tcData.data.u.st_20_2_3.nCount; i++)
	{
		printf("Param ID %d: %d   ->   %d\n", i, outTm1.data.u.tcData.data.u.st_20_2_3.arr[i], outTm2.data.u.tcData.data.u.st_20_2_3.arr[i]);
		printf("Param Value %d: %d   ->   %d\n", i, outTm1.data.u.tcData.data.u.st_20_2_3.value[i], outTm2.data.u.tcData.data.u.st_20_2_3.value[i]);
	}

	printf("\n");



	int comp = memcmp(&outTm1, &outTm2, 200);
	char temp[1000];
	memcpy(temp, &outTm1, 1000);

	if (comp != 0)
	{
		printf("Memory Compare didn't work!\n");
	}
	else
	{
		printf("Memory Compare worked!!!aelfvznawleföaosfh Notice ME!!!!!!!\n");
	}*/

	return length;
}

int test_tm_service_twentyone()
{
	//not implemented
	return 0;
}

int test_tc_service_twentyone()
{
	//not implemented
	return 0;
}

int test_tm_service_twentytwo()
{
	//not implemented
	return 0;
}

int test_tc_service_twentytwo()
{
	//not implemented
	return 0;
}

int test_tm_service_twentythree()
{

	printf("\nStarted testing TM Service 23\n\n");

	pusPacket_t outTm1;
	pusPacket_t outTm2;
	pusApid_t apid = 0x88;
	pusApid_t dst = 0x89;
	pusSequenceCount_t seq = 0x8a;
	const pusSt23RepositoryPath_t rep = { 4, "this" };
	const pusSt23FileName_t fil = { 5, "thhat" };
	const asn1SccPusByteArray src = { 6, "jeness" };
	const asn1SccPusByteArray pat[8] = { {4, "asdf"}, {3, "qss"}, {2, "ws"}, {2, "es"}, {2, "rs"}, {2, "ts"}, {5, "zzzzs"}, {3, "abc"} };
	const asn1SccPusSt23ObjectType obj[8] = { st_23_13_file, st_23_13_directory, st_23_13_file, st_23_13_directory, st_23_13_file, st_23_13_directory, st_23_13_file , st_23_13_directory };

	//pus_tm_23_4_createFileAtributesReport(&outTm1, apid, seq, dst, &rep, &fil, 8);
	//pus_tm_23_8_createFoundFileReport(&outTm1, apid, seq,  dst, &rep, &src, 7, pat);
	pus_tm_23_13_createSummaryRepositoryReport(&outTm1, apid, seq, dst, &rep, 8, obj, pat);

	printf("Sending 23_13 Packet\n");

	char buffer[40000];
	unsigned int length = 0;


	if (encodePacket(&outTm1, buffer, &length) != PUS_NO_ERROR)
	{
		printf("Error sending 23_13 Packet!\n");
		return 1;
	}

	if (decodePacket(buffer, &outTm2) != PUS_NO_ERROR)
	{
		printf("Error receiving 23_13 Packet!\n");
		return 1;
	}

	printf("\nReceived 23_13 Packet\n");
	assert_primary_header(&outTm1, &outTm2);
	assert_secondary_tm_header(&outTm1, &outTm2);
	printf("\n");
	/*printf("Repository: %s   ->   %s\n", outTm1.data.u.tmData.data.u.st_23_4.repository.arr, outTm2.data.u.tmData.data.u.st_23_4.repository.arr);
	printf("R: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_23_4.repository.nCount, outTm2.data.u.tmData.data.u.st_23_4.repository.nCount);
	printf("File: %s   ->   %s\n", outTm1.data.u.tmData.data.u.st_23_4.fileName.arr, outTm2.data.u.tmData.data.u.st_23_4.fileName.arr);
	printf("F: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_23_4.fileName.nCount, outTm2.data.u.tmData.data.u.st_23_4.fileName.nCount);
	printf("Size: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_23_4.maxSize, outTm2.data.u.tmData.data.u.st_23_4.maxSize);*/
	/*printf("Repository: %s   ->   %s\n", outTm1.data.u.tmData.data.u.st_23_8.repository.arr, outTm2.data.u.tmData.data.u.st_23_8.repository.arr);
	printf("R: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_23_8.repository.nCount, outTm2.data.u.tmData.data.u.st_23_8.repository.nCount);
	printf("Search: %s   ->   %s\n", outTm1.data.u.tmData.data.u.st_23_8.searchPattern.arr, outTm2.data.u.tmData.data.u.st_23_8.searchPattern.arr);
	printf("S: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_23_8.searchPattern.nCount, outTm2.data.u.tmData.data.u.st_23_8.searchPattern.nCount);
	printf("Matches: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_23_8.matches, outTm2.data.u.tmData.data.u.st_23_8.matches);
	printf("\n");
	for (int i = 0; i < outTm1.data.u.tmData.data.u.st_23_8.matches; i++)
	{
		printf("Match %d: %s   ->   %s\n", i, outTm1.data.u.tmData.data.u.st_23_8.filePaths[i].arr, outTm2.data.u.tmData.data.u.st_23_8.filePaths[i].arr);
		printf("M %d: %d   ->   %d\n", i, outTm1.data.u.tmData.data.u.st_23_8.filePaths[i].nCount, outTm2.data.u.tmData.data.u.st_23_8.filePaths[i].nCount);
	}*/
	printf("Repository: %s   ->   %s\n", outTm1.data.u.tmData.data.u.st_23_13.repository.arr, outTm2.data.u.tmData.data.u.st_23_13.repository.arr);
	printf("R: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_23_13.repository.nCount, outTm2.data.u.tmData.data.u.st_23_13.repository.nCount);
	printf("Number: %d   ->   %d\n", outTm1.data.u.tmData.data.u.st_23_13.numberObjects, outTm2.data.u.tmData.data.u.st_23_13.numberObjects);
	for (int i = 0; i < outTm1.data.u.tmData.data.u.st_23_13.numberObjects; i++)
	{
		printf("Type %d: %d   ->   %d\n", i, outTm1.data.u.tmData.data.u.st_23_13.objectTypes[i], outTm2.data.u.tmData.data.u.st_23_13.objectTypes[i]);
		printf("Object %d: %s   ->   %s\n", i, outTm1.data.u.tmData.data.u.st_23_13.objectNames[i].arr, outTm2.data.u.tmData.data.u.st_23_13.objectNames[i].arr);
		printf("O %d: %d   ->   %d\n", i, outTm1.data.u.tmData.data.u.st_23_13.objectNames[i].nCount, outTm2.data.u.tmData.data.u.st_23_13.objectNames[i].nCount);
	}
	return 0;
}

int test_tc_service_twentythree()
{
	printf("\nStarted testing TC Service 23\n\n");

	pusPacket_t outTm1;
	pusPacket_t outTm2;
	pusApid_t apid = 0x88;
	pusSequenceCount_t seq = 0x88;
	asn1SccPusByteArray rep = { 5, "thiis" };
	asn1SccPusByteArray file = { 6, "thhhat" };
	asn1SccPusByteArray dir = { 17, "abcdefghijklmnopq" };
	asn1SccPusByteArray car = { 8, "zyxwvuts" };
	pusSt23MaximumSize_t size = 0x80;


	//pus_tc_23_1_createCreateFileRequest(&outTm1, apid, seq, &rep, &file, size, 1);
	//pus_tc_23_2_createDeleteFileRequest(&outTm1, apid, seq, &rep, &file);
	//pus_tc_23_11_createRenameDirectoryRequest(&outTm1, apid, seq, &rep, &file, &dir);
	//pus_tc_23_12_createSummaryRepositoryRequest(&outTm1, apid, seq, &rep);
	pus_tc_23_14_createCopyFileRequest(&outTm1, apid, seq, &rep, &file, &dir, &car);

	printf("Sending 23_1 Packet\n");

	char buffer[40000];
	unsigned int length = 0;

	if (encodePacket(&outTm1, buffer, &length) != PUS_NO_ERROR)
	{
		printf("Error sending 23_1 Packet!\n");
		return 1;
	}

	//send(new_socket, buffer, length, 0);

	//if (receivePacket(buffer, &outTm2) != PUS_NO_ERROR)
	//{
	//	printf("Error receiving 23_1 Packet!\n");
	//	return 1;
	//}

	//printf("\nReceived 23_1 Packet\n");

	//assert(outTm1.packetVersion == outTm2.packetVersion);
	//assert_primary_header(&outTm1, &outTm2);
	//assert_secondary_tc_header(&outTm1, &outTm2);
	//printf("\n");
	//printf("Data Kind: %d   ->   %d\n", outTm1.data.u.tcData.data.kind, outTm2.data.u.tcData.data.kind);
	//printf("\n");
	//printf("R: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_23_14.sourceRepository.nCount, outTm2.data.u.tcData.data.u.st_23_14.sourceRepository.nCount);
	//printf("Rep: %s   ->   %s\n", outTm1.data.u.tcData.data.u.st_23_14.sourceRepository.arr, outTm2.data.u.tcData.data.u.st_23_14.sourceRepository.arr);
	//printf("F: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_23_14.sourceFileName.nCount, outTm2.data.u.tcData.data.u.st_23_14.sourceFileName.nCount);
	//printf("File: %s   ->   %s\n", outTm1.data.u.tcData.data.u.st_23_14.sourceFileName.arr, outTm2.data.u.tcData.data.u.st_23_14.sourceFileName.arr);
	//printf("F: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_23_14.targetRepository.nCount, outTm2.data.u.tcData.data.u.st_23_14.targetRepository.nCount);
	//printf("File: %s   ->   %s\n", outTm1.data.u.tcData.data.u.st_23_14.targetRepository.arr, outTm2.data.u.tcData.data.u.st_23_14.targetRepository.arr);
	//printf("F: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_23_14.targetFileName.nCount, outTm2.data.u.tcData.data.u.st_23_14.targetFileName.nCount);
	//printf("File: %s   ->   %s\n", outTm1.data.u.tcData.data.u.st_23_14.targetFileName.arr, outTm2.data.u.tcData.data.u.st_23_14.targetFileName.arr);
	////printf("Size: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_23_1.maxSize, outTm2.data.u.tcData.data.u.st_23_1.maxSize);
	////printf("Locked: %d   ->   %d\n", outTm1.data.u.tcData.data.u.st_23_1.locked, outTm2.data.u.tcData.data.u.st_23_1.locked);
	//

	//printf("\n");



	//int comp = memcmp(&outTm1, &outTm2, 200);
	//char temp[1000];
	//memcpy(temp, &outTm1, 1000);

	//if (comp != 0)
	//{
	//	printf("Memory Compare didn't work!\n");
	//}
	//else
	//{
	//	printf("Memory Compare worked!!!aelfvznawleföaosfh Notice ME!!!!!!!\n");
	//}

	return length;
}

int test_server()
{
	int server_fd, valread, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char recv_buffer[1500] = { 0 };
	char send_buffer[1500] = { 0 };

	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		printf("socket failed\n");
		return 1;
	}

	// Forcefully attaching socket to the port 1025 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		&opt, sizeof(opt)))
	{
		printf("setsockopt");
		return 1;
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 1025 
	if (bind(server_fd, (struct sockaddr*) & address,
		sizeof(address)) < 0)
	{
		printf("bind failed\n");
		return 1;
	}
	if (listen(server_fd, 3) < 0)
	{
		printf("listen\n");
		return 1;
	}
	if ((new_socket = accept(server_fd, (struct sockaddr*) & address,
		(socklen_t*)&addrlen)) < 0)
	{
		printf("accept\n");
		return 1;
	}

	printf("\nSocket connected\n");

	int i = test_tc_service_five();
	printf("%d\n", i);

	pusPacket_t recv;
	pusError_t pusError;
	pus_initTmPacket(&recv);
	valread = read(new_socket, recv_buffer, 1500);

	pusError = decodePacket(recv_buffer, &recv);

	if (pusError != PUS_NO_ERROR)
	{
		printf("Failed to properly receive Packet from Client!\n");
	}
	else
	{
		printf("Received a good Packet from Client!\n");
		print_packet(&recv);
	}


	if (recv.data.u.tmData.header.serviceId == 1 && recv.data.u.tmData.header.subtypeId == 1)
	{
		printf("Packet was succesfully received by Client!!!\n");
		printf("%d\n", recv.data.u.tmData.data.u.st_1_X.request.apid);
		return 0;
	}
	else if (recv.data.u.tmData.header.serviceId == 1 && recv.data.u.tmData.header.subtypeId == 2)
	{
		printf("Client failed to accept Packet!!!\n");
		printf("%d\n", recv.data.u.tmData.data.u.st_1_X.request.apid);
		return 0;
	}
	else
	{
		printf("Something unexpected happened at the Client!!!!!");
		return 1;
	}
}

int test_client()
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent* server;

	char recv_buffer[1500];
	char send_buffer[1500];

	portno = PORT;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("ERROR opening socket\n");
	}

	server = gethostbyname(SERVER_IP);

	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char*)server->h_addr,
		(char*)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);

	if (connect(sockfd, (const sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("ERROR connecting\n");
		return 1;
	}

	bzero(recv_buffer, 1500);
	bzero(send_buffer, 1500);
	n = read(sockfd, recv_buffer, 1500);
	pusPacket_t recv, sent;
	pusError_t pusError;
	pusError = decodePacket(recv_buffer, &recv);
	unsigned int length = 0;

	printf("Received Packet from Server!\n");
	print_packet(&recv);

	for (int i = 0; i < 10; i++)
	{
		printf("%d, %x\n", i, recv_buffer[i]);
	}

	if (pusError == PUS_NO_ERROR && 
		recv.data.u.tcData.data.u.st_5_X.arr[0].eventId == 7 && 
		recv.data.u.tcData.data.u.st_5_X.arr[1].eventId == 10 &&
		recv.data.u.tcData.data.u.st_5_X.arr[2].eventId == 1)
	{
		pus_tm_1_1_createAcceptanceReportSuccess(&sent, 0x88, 0x99, &recv);
		printf("Sending 1_1 Packet!\n");
	}
	else
	{
		asn1SccPusSt01FailureInfo fai = { 0xbb, 0xcc, 0xdd };
		pus_tm_1_2_createAcceptanceReportFailure(&sent, 0x88, 0x99, &recv, 0xaa, &fai);
		printf("Sending 1_2 Packet!\n");
	}
	print_packet(&sent);

	if (encodePacket(&sent, send_buffer, &length) != PUS_NO_ERROR)
	{
		printf("Error sending response Packet!\n");
		return 0;
	}

	send(sockfd, send_buffer, length, 0);

	printf("Wrote Response on Socket!\n");

	/*while (1) {
		printf("Please enter the message: ");
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);
		printf("%s\n", buffer);
		if (strcmp(buffer, "exit") == 0) break;
		n = write(sockfd, buffer, strlen(buffer));
		if (n < 0)
			error("ERROR writing to socket");
		bzero(buffer, 256);
		n = read(sockfd, buffer, 255);
		if (n < 0)
			error("ERROR reading from socket");
		printf("%s\n", buffer);
	}*/

	usleep(500);

	//close(sockfd);
	return 0;
}