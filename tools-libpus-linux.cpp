#include "tools-libpus-linux.h"

pusError_t pus_tm_send_1_1(pusPacket_t* outTm, pusApid_t* apid, pusSequenceCount_t* seq, pusPacket_t* inTc, char* buffer, int* sockfd, int* pus_st13_transactionID)
{
	unsigned int length;

	if (pus_tm_1_1_createAcceptanceReportSuccess(outTm, *apid, *seq, inTc) != PUS_NO_ERROR)
	{
		printf("Error creating 1_1 Packet!\n");
		return PUS_ERROR_PACKET_CREATE;
	}

	if (encodePacket(outTm, buffer, &length) != PUS_NO_ERROR)
	{
		printf("Error encoding 1_1 Packet!\n");
		return PUS_ERROR_PACKET_ENCODE;
	}

	if (pus_tm_sendTCP(*sockfd, buffer, length, seq, pus_st13_transactionID, pus_getTcSource(inTc)) != PUS_NO_ERROR)
	{
		printf("Error sending 1_1 Packet!\n");
		return PUS_ERROR_PACKET_SEND;
	}

	printf("Sent 1_1 Packet!\n");

	bzero(buffer, length);
	pus_setPacketDefaults(outTm);
	pus_initTmPacket(outTm);

	return PUS_NO_ERROR;
}

pusError_t pus_tm_send_1_2(pusPacket_t* outTm, pusApid_t* apid, pusSequenceCount_t* seq, pusPacket_t* inTc, char* buffer, int* sockfd, int* pus_st13_transactionID,
	pusSt01FailureCode_t code, pusSt01FailureInfo_t* info)
{
	unsigned int length;

	if (pus_tm_1_2_createAcceptanceReportFailure(outTm, *apid, *seq, inTc, code, info) != PUS_NO_ERROR)
	{
		printf("Error creating 1_2 Packet!\n");
		return PUS_ERROR_PACKET_CREATE;
	}

	if (encodePacket(outTm, buffer, &length) != PUS_NO_ERROR)
	{
		printf("Error encoding 1_2 Packet!\n");
		return PUS_ERROR_PACKET_ENCODE;
	}

	if (pus_tm_sendTCP(*sockfd, buffer, length, seq, pus_st13_transactionID, pus_getTcSource(inTc)) != PUS_NO_ERROR)
	{
		printf("Error sending 1_2 Packet!\n");
		return PUS_ERROR_PACKET_SEND;
	}

	printf("Sent 1_2 Packet!\n");

	bzero(buffer, length);
	pus_setPacketDefaults(outTm);
	pus_initTmPacket(outTm);
}

pusError_t pus_tc_sendTCP(int socket, char* buffer, int packet_length, pusSequenceCount_t* sequenceCount, int* transactionID)
{
	//	-	-	-	Sending a TC packet over a TCP socket

	if (socket == NULL || buffer == NULL || packet_length == NULL)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (packet_length <= pus_ST13_MAX_PART_SIZE)
	{
		send(socket, buffer, packet_length, 0);
		(*sequenceCount)++;
		return PUS_NO_ERROR;
	}

	//	-	-	-	Packet is larger than max packet size -> use service 13

	unsigned int part_length = 0;
	char split_content[pus_ST13_MAX_PART_SIZE];
	char send_buffer[pus_ST13_MAX_PART_SIZE + 50];

	pus_tc_tm_13_splitContent(split_content, buffer, 0);

	pusPacket_t outTc;

	pus_initTcPacket(&outTc);
	pus_tc_13_9_createFirstUplinkPart(&outTc, 13, *sequenceCount, *transactionID, split_content);
	(*sequenceCount)++;

	if (encodePacket(&outTc, send_buffer, &part_length) != PUS_NO_ERROR)
	{
		return PUS_GET_ERROR();
	}

	send(socket, send_buffer, part_length, 0);

	bzero(send_buffer, pus_ST13_MAX_PART_SIZE + 50);
	bzero(split_content, pus_ST13_MAX_PART_SIZE);
	part_length = 0;

	int i = 1;
	for (; i < packet_length / pus_ST13_MAX_PART_SIZE; i++)
	{
		pus_initTcPacket(&outTc);

		pus_tc_tm_13_splitContent(split_content, buffer, i);
		pus_tc_13_10_createIntermediateUplinkPart(&outTc, 13, *sequenceCount, *transactionID, i + 1, split_content);
		(*sequenceCount)++;

		if (encodePacket(&outTc, send_buffer, &part_length) != PUS_NO_ERROR)
		{
			return PUS_GET_ERROR();
		}

		send(socket, send_buffer, part_length, 0);

		bzero(send_buffer, pus_ST13_MAX_PART_SIZE + 50);
		bzero(split_content, pus_ST13_MAX_PART_SIZE);
		part_length = 0;
	}

	pus_initTcPacket(&outTc);

	pus_tc_tm_13_splitContent(split_content, buffer, i);

	pus_tc_13_11_createLastUplinkPart(&outTc, 13, *sequenceCount, *transactionID, i + 1, split_content);
	(*sequenceCount)++;
	(*transactionID)++;

	if (encodePacket(&outTc, send_buffer, &part_length) != PUS_NO_ERROR)
	{
		return PUS_GET_ERROR();
	}

	send(socket, send_buffer, part_length, 0);

	printf("Sent large packet as TC!\n");

	return PUS_NO_ERROR;
}

pusError_t pus_tm_sendTCP(int socket, char* buffer, int packet_length, pusSequenceCount_t* sequenceCount, int* transactionID, pusApid_t destination)
{
	//	-	-	-	Sending a TM packet over a TCP socket

	if (socket == NULL || buffer == NULL || packet_length == NULL)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (packet_length <= pus_ST13_MAX_PART_SIZE)
	{
		send(socket, buffer, packet_length, 0);
		(*sequenceCount)++;
		return PUS_NO_ERROR;
	}

	//	-	-	-	Packet is larger than max packet size -> use service 13

	unsigned int part_length = 0;
	char split_content[pus_ST13_MAX_PART_SIZE];
	char send_buffer[pus_ST13_MAX_PART_SIZE + 50];

	pus_tc_tm_13_splitContent(split_content, buffer, 0);

	pusPacket_t outTm;

	pus_initTmPacket(&outTm);
	pus_tm_13_1_createFirstDownlinkPartReport(&outTm, 13, *sequenceCount, destination, *transactionID, split_content);
	(*sequenceCount)++;

	if (encodePacket(&outTm, send_buffer, &part_length) != PUS_NO_ERROR)
	{
		return PUS_GET_ERROR();
	}

	send(socket, send_buffer, part_length, 0);

	bzero(send_buffer, pus_ST13_MAX_PART_SIZE + 50);
	bzero(split_content, pus_ST13_MAX_PART_SIZE);
	part_length = 0;

	int i = 1;
	for (; i < packet_length / pus_ST13_MAX_PART_SIZE; i++)
	{
		pus_initTmPacket(&outTm);

		pus_tc_tm_13_splitContent(split_content, buffer, i);
		pus_tm_13_2_createIntermediateDownlinkPartReport(&outTm, 13, *sequenceCount, destination, *transactionID, i + 1, split_content);
		(*sequenceCount)++;

		if (encodePacket(&outTm, send_buffer, &part_length) != PUS_NO_ERROR)
		{
			return PUS_GET_ERROR();
		}

		send(socket, send_buffer, part_length, 0);

		bzero(send_buffer, pus_ST13_MAX_PART_SIZE + 50);
		bzero(split_content, pus_ST13_MAX_PART_SIZE);
		part_length = 0;
	}

	pus_initTmPacket(&outTm);

	pus_tc_tm_13_splitContent(split_content, buffer, i);

	pus_tm_13_3_createLastDownlinkPartReport(&outTm, 13, *sequenceCount, destination, *transactionID, i + 1, split_content);
	(*sequenceCount)++;
	(*transactionID)++;

	if (encodePacket(&outTm, send_buffer, &part_length) != PUS_NO_ERROR)
	{
		return PUS_GET_ERROR();
	}

	send(socket, send_buffer, part_length, 0);

	printf("Sent large packet as TM!\n");

	return PUS_NO_ERROR;
}

pusError_t pus_tc_recvTCP(int socket, char* buffer)
{
	//	-	-	-	Receiving a TC packet over a TCP socket

	if (socket == NULL || buffer == NULL)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	int mergedLength = 0;
	unsigned int singlePacketLength = 0;
	pusPacket_t inTc;
	pus_initTcPacket(&inTc);

	recv(socket, buffer, 10, MSG_PEEK);
	singlePacketLength = getDataLength(buffer);
	bzero(buffer, 10);

	recv(socket, buffer, singlePacketLength, 0);

	decodePacket(buffer, &inTc);

	//	-	-	-	Check for service 13 (large packet)

	if (pus_getTcService(&inTc) != pus_ST13_largePacketTransfer)
	{
		return PUS_NO_ERROR;
	}

	if (pus_getTcSubtype(&inTc) != pus_TC_13_9_createFirstUplinkPartReport)
	{
		return PUS_ERROR_TC_SUBTYPE;
	}

	pus_tc_tm_13_mergeContent(buffer, &inTc, &mergedLength);

	pus_initTcPacket(&inTc);

	char recv_buffer[pus_ST13_MAX_PART_SIZE + 50];

	recv(socket, recv_buffer, 10, MSG_PEEK);
	singlePacketLength = getDataLength(recv_buffer);
	bzero(recv_buffer, 10);

	recv(socket, recv_buffer, singlePacketLength, 0);

	decodePacket(recv_buffer, &inTc);

	int i = 2;
	int partSeqNumber = 0;

	while (pus_getTcSubtype(&inTc) != pus_TC_13_11_createLastUplinkPartReport)
	{
		if (pus_getTcSubtype(&inTc) != pus_TC_13_10_createIntermediateUplinkPartReport)
		{
			return PUS_ERROR_TC_SUBTYPE;
		}

		pus_tc_tm_13_X_getPartSeqNumber(&partSeqNumber , &inTc);

		if (partSeqNumber != i)
		{
			return PUS_ERROR_INVALID_ID;
		}

		pus_tc_tm_13_mergeContent(buffer, &inTc, &mergedLength);

		bzero(recv_buffer, pus_ST13_MAX_PART_SIZE + 50);

		pus_initTcPacket(&inTc);

		usleep(20);

		recv(socket, recv_buffer, 10, MSG_PEEK);
		singlePacketLength = getDataLength(recv_buffer);
		bzero(recv_buffer, 10);

		recv(socket, recv_buffer, singlePacketLength, 0);

		decodePacket(recv_buffer, &inTc);

		i++;
	}

	pus_tc_tm_13_X_getPartSeqNumber(&partSeqNumber, &inTc);

	if (partSeqNumber != i)
	{
		return PUS_ERROR_INVALID_ID;
	}

	pus_tc_tm_13_mergeContent(buffer, &inTc, &mergedLength);

	printf("Received large packet as TC!\n");

	return PUS_NO_ERROR;
}

pusError_t pus_tm_recvTCP(int socket, char* buffer) 
{
	//	-	-	-	Receiving a TC packet over a TCP socket

	if (socket == NULL || buffer == NULL)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	int mergedLength = 0;
	unsigned int singlePacketLength = 0;
	pusPacket_t inTm;
	pus_initTmPacket(&inTm);

	recv(socket, buffer, 10, MSG_PEEK);
	singlePacketLength = getDataLength(buffer);
	bzero(buffer, 10);

	recv(socket, buffer, singlePacketLength, 0);

	decodePacket(buffer, &inTm);

	//	-	-	-	Check for service 13 (large packet)

	if (pus_getTmService(&inTm) != pus_ST13_largePacketTransfer)
	{
		return PUS_NO_ERROR;
	}

	if (pus_getTmSubtype(&inTm) != pus_TM_13_1_createFirstDownlinkPartReport)
	{
		return PUS_ERROR_TC_SUBTYPE;
	}

	pusPacketDataLength_t largePacketDataLength;

	pus_getPacketDataLength(&inTm);

	pus_tc_tm_13_mergeContent(buffer, &inTm, &mergedLength);

	pus_initTcPacket(&inTm);

	char recv_buffer[pus_ST13_MAX_PART_SIZE + 50];

	recv(socket, recv_buffer, 10, MSG_PEEK);
	singlePacketLength = getDataLength(recv_buffer);
	bzero(recv_buffer, 10);

	recv(socket, recv_buffer, singlePacketLength, 0);

	decodePacket(recv_buffer, &inTm);

	int i = 2;
	int partSeqNumber;

	while (pus_getTmSubtype(&inTm) != pus_TM_13_3_createLastDownlinkPartReport)
	{
		if (pus_getTmSubtype(&inTm) != pus_TM_13_2_createIntermediateDownlinkPartReport)
		{
			return PUS_ERROR_TC_SUBTYPE;
		}

		pus_tc_tm_13_X_getPartSeqNumber(&partSeqNumber, &inTm);

		if (partSeqNumber != i)
		{
			return PUS_ERROR_INVALID_ID;
		}

		pus_tc_tm_13_mergeContent(buffer, &inTm, &mergedLength);

		bzero(recv_buffer, pus_ST13_MAX_PART_SIZE + 50);

		pus_initTcPacket(&inTm);

		usleep(20);

		recv(socket, recv_buffer, 10, MSG_PEEK);
		singlePacketLength = getDataLength(recv_buffer);
		bzero(recv_buffer, 10);

		recv(socket, recv_buffer, singlePacketLength, 0);

		decodePacket(recv_buffer, &inTm);

		i++;
	}

	pus_tc_tm_13_X_getPartSeqNumber(&partSeqNumber, &inTm);

	if (partSeqNumber != i)
	{
		return PUS_ERROR_INVALID_ID;
	}

	pus_tc_tm_13_mergeContent(buffer, &inTm, &mergedLength);

	printf("Received large packet as TM!\n");

	return PUS_NO_ERROR;
}

void start_server()
{
	//	-	-	-	Initialization of parameter

	int server_fd, valread, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

	char buffer[65542];
	bzero(buffer, 65542);

	unsigned int packet_length = 0;
	int serv = 0;
	int subt = 0;
	flag gotSent = false;
	pusPacket_t outTc;
	pusPacket_t inTm;
	pusApid_t apid = 1;
	pusSequenceCount_t seq = 0;
	int pus_st13_transactionID = 1;
	pus_initTcPacket(&outTc);
	pus_initTmPacket(&inTm);

	unsigned int buffer3_X_count = 0;
	pusSt20OnBoardParamId_t buffer3_X_id[20];

	asn1SccPusSt18ObcpCode outTc18String;
	char buffer18_1_id[10];
	bzero(buffer18_1_id, 10);
	char buffer18_1_string[15000];
	bzero(buffer18_1_string, 15000);

	unsigned int buffer20_1_3_count = 0;
	pusSt20OnBoardParamId_t buffer20_1_3_id[20];
	bzero(buffer20_1_3_id, 20 * sizeof(pusSt20OnBoardParamId_t));
	pusStoredParam_t buffer20_3_value[20];
	bzero(buffer20_3_value, 20 * sizeof(pusStoredParam_t));

	//	-	-	-	Initialization of socket

	printf("Started Server!\n");

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		printf("Server: Socket failed!\n");
		return;
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		&opt, sizeof(opt)))
	{
		printf("Server: Set sockopt failed!\n");
		return;
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr*) & address,
		sizeof(address)) < 0)
	{
		printf("Server: Bind failed!\n");
		return;
	}

	if (listen(server_fd, 3) < 0)
	{
		printf("Server: Listen failed!\n");
		return;
	}

SERVER_CONNECT: 
	if ((new_socket = accept(server_fd, (struct sockaddr*) & address,
		(socklen_t*)&addrlen)) < 0)
	{
		printf("Server: Accept failed!\n");
		return;
	}

	int fd_stdin;
	fd_stdin = fileno(stdin);
	int recVal;
	struct pollfd pfds[2];
	pfds[0].fd = fd_stdin;
	pfds[0].events = POLLIN;
	pfds[1].fd = new_socket;
	pfds[1].events = POLLIN;

	printf("\nServer: Socket connected!\n");

	//	-	-	-	Start of service routine

	printf("\nEnter Service and Subtype: \n");

	while (1)
	{
		//	-	-	-	Blocking until one of the file descriptors is ready

		recVal = poll(pfds, 2, -1);

		//	-	-	-	Error handling

		if (recVal == -1)
		{
			printf("Server: Routine Error!\n");
			usleep(50000);
			continue;
		}

		//	-	-	-	TC descriptor activated 

		if (pfds[0].revents & POLLIN)
		{
			scanf("%d %d", &serv, &subt);
			getchar();
			printf("Got Service %d and Subtype %d\n", serv, subt);
		}

		//	-	-	-	TM descriptor activated 

		else if (pfds[1].revents & POLLIN)
		{
			if (pus_tm_recvTCP(new_socket, buffer) != PUS_NO_ERROR)
			{
				printf("Receive Error!");
			}

			if (PUS_GET_ERROR() == PUS_ERROR_NOT_AVAILABLE)
			{
				printf("Connection lost!\n");
				printf("Reconnecting!\n");
				goto SERVER_CONNECT;
				break;
			}
			else if (PUS_GET_ERROR() != PUS_NO_ERROR)
			{
				printf("Encountered unexpected error!\n");
				printf("Reconnecting!\n");
				goto SERVER_CONNECT;
				break;
			}

			if (decodePacket(buffer, &inTm) != PUS_NO_ERROR)
			{
				printf("Failed to decode Packet from Client!\n");
				printf("Reconnecting!\n");
				goto SERVER_CONNECT;
				break;
			}

			//	-	-	-	Check all possible packet types 

			else if (inTm.data.u.tmData.data.kind != st_1_X_PRESENT && 
					inTm.data.u.tmData.data.kind != st_3_X_PRESENT &&
					inTm.data.u.tmData.data.kind != st_5_X_PRESENT &&
					inTm.data.u.tmData.data.kind != st_20_2_PRESENT)
			{
				if (inTm.data.u.tmData.header.serviceId == 17 && inTm.data.u.tmData.header.subtypeId == 2)
				{
					printf("\t'Yes, I am alive!'\n");
				}
				else
				{
					printf("Received unexpected Packet from Client!\n");
					printf("Service: %d\n", inTm.data.u.tmData.header.serviceId);
					printf("Subtype: %d\n", inTm.data.u.tmData.header.subtypeId);
				}
			}

			else if (inTm.data.u.tmData.header.serviceId == 1)
			{
				if (inTm.data.u.tmData.header.subtypeId == 1)
				{
					printf("Packet was succesfully received by Client!!!\n");
				}
				else if (inTm.data.u.tmData.header.subtypeId == 2)
				{
					printf("Client failed to accept Packet!!!\n");
				}
			}

			else if (inTm.data.u.tmData.header.serviceId == 3)
			{
				if (inTm.data.u.tmData.header.subtypeId == 25) 
				{
					printf("Received Housekeeping Data Report No. %llu!\n", inTm.data.u.tmData.data.u.st_3_X.reportId);
				}
				else if (inTm.data.u.tmData.header.subtypeId == 26) 
				{
					printf("Received Diagnostic Data Report No. %llu!\n", inTm.data.u.tmData.data.u.st_3_X.reportId);
				}

				for (int i = 0; i < inTm.data.u.tmData.data.u.st_3_X.parameters.nCount; i++)
				{
					printf("Value:\t%f\n", inTm.data.u.tmData.data.u.st_3_X.parameters.arr[i]);
				}
			}

			else if (inTm.data.u.tmData.header.serviceId == 5)
			{
				switch (inTm.data.u.tmData.header.subtypeId)
				{
				case 1:
				{
					printf("Received Informative Event Report ID: %d\n", inTm.data.u.tmData.data.u.st_5_X.eventId);
					printf("Content:\t%d\t%d\n", inTm.data.u.tmData.data.u.st_5_X.data.data1, inTm.data.u.tmData.data.u.st_5_X.data.data2);
					break;
				}
				case 2:
				{
					printf("Received Low Severity Event Report ID: %d\n", inTm.data.u.tmData.data.u.st_5_X.eventId);
					printf("Content:\t%d\t%d\n", inTm.data.u.tmData.data.u.st_5_X.data.data1, inTm.data.u.tmData.data.u.st_5_X.data.data2);
					break;
				}
				case 3:
				{
					printf("Received Medium Severity Event Report ID: %d\n", inTm.data.u.tmData.data.u.st_5_X.eventId);
					printf("Content:\t%d\t%d\n", inTm.data.u.tmData.data.u.st_5_X.data.data1, inTm.data.u.tmData.data.u.st_5_X.data.data2);
					break;
				}
				case 4:
				{
					printf("Received High Severity Event Report ID: %d\n", inTm.data.u.tmData.data.u.st_5_X.eventId);
					printf("Content:\t%d\t%d\n", inTm.data.u.tmData.data.u.st_5_X.data.data1, inTm.data.u.tmData.data.u.st_5_X.data.data2);
					break;
				}
				}
			}
			
			else if (inTm.data.u.tmData.header.serviceId == 20 && inTm.data.u.tmData.header.subtypeId == 2)
			{
				printf("Received parameter value report!\n");
				printf("Content: \n");
				for (int i = 0; i < inTm.data.u.tmData.data.u.st_20_2_3.nCount; i++)
				{
					printf("ID:\t%d\tValue:\t%lf\n", inTm.data.u.tmData.data.u.st_20_2_3.arr[i], inTm.data.u.tmData.data.u.st_20_2_3.value[i]);

				}
			}

			else
			{
				printf("Received %d_%d Packet, which cannot be handled!\n", inTm.data.u.tmData.header.serviceId, inTm.data.u.tmData.header.subtypeId);
			}

			usleep(5000);
			continue;
		}

		//	-	-	-	Handle TC

		switch (serv)
		{

		case 3:
		{
			if(subt == 5 || subt == 6 || subt == 7 || subt == 8)
			{
				printf("Enter number of parameters: ");
				scanf("%d", &buffer3_X_count);

				if (buffer3_X_count > 20)
				{
					printf("Invalid number! (Max %d", 20);
					break;
				}

				for (int i = 0; i < buffer3_X_count; i++)
				{
					printf("Enter ID of parameter %d: ", i + 1);
					scanf("%d", &buffer3_X_id[i]);

					if (buffer3_X_id[i] >= pus_ST20_PARAM_LIMIT)
					{
						printf("Invalid number! (Max %d", pus_ST20_PARAM_LIMIT);
						break;
					}
				}

				switch (subt)
				{
				case 5:
				{
					if (pus_tc_3_5_createEnableHousekeepingRequest(&outTc, apid, seq, buffer3_X_id, buffer3_X_count) != PUS_NO_ERROR)
					{
						printf("Error creating 3_5 Packet!\n");
						break;
					}
					break;
				}
				case 6:
				{
					if (pus_tc_3_6_createDisableHousekeepingRequest(&outTc, apid, seq, buffer3_X_id, buffer3_X_count) != PUS_NO_ERROR)
					{
						printf("Error creating 3_6 Packet!\n");
						break;
					}
					break;
				}
				case 7:
				{
					if (pus_tc_3_7_createEnableDiagnosticRequest(&outTc, apid, seq, buffer3_X_id, buffer3_X_count) != PUS_NO_ERROR)
					{
						printf("Error creating 3_7 Packet!\n");
						break;
					}
					break;
				}
				case 8:
				{
					if (pus_tc_3_8_createDisableDiagnosticRequest(&outTc, apid, seq, buffer3_X_id, buffer3_X_count) != PUS_NO_ERROR)
					{
						printf("Error creating 3_8 Packet!\n");
						break;
					}
					break;
				}
				}

				if (encodePacket(&outTc, buffer, &packet_length) != PUS_NO_ERROR)
				{
					printf("Error encoding 3_%d Packet!\n", subt);
					break;
				}

				if (pus_tc_sendTCP(new_socket, buffer, packet_length, &seq, &pus_st13_transactionID) != PUS_NO_ERROR)
				{
					printf("Error sending 3_%d Packet!\n", subt);
					break;
				}

				printf("Succesfully sent 3_%d Packet!\n", subt);

				pus_setPacketDefaults(&outTc);
				pus_initTcPacket(&outTc);
				bzero(buffer, packet_length + 1);
				packet_length = 0;
				bzero(buffer3_X_id, (buffer3_X_count + 1) * sizeof(pusSt20OnBoardParamId_t));
				buffer3_X_count = 0;
				gotSent = true;

				break;
			}
			else
			{
				printf("Invalid Subtype! (Try '5', '6', '7' or '8')\n");
				break;
			}
		}
		case 17:
		{
			if (subt != 1)
			{
				printf("Invalid Subtype! (Try '1')\n");
				break;
			}

			if (pus_tc_17_1_createConnectionTestRequest(&outTc, apid, seq) != PUS_NO_ERROR)
			{
				printf("Error creating 17_1 Packet!\n");
				break;
			}

			if (encodePacket(&outTc, buffer, &packet_length) != PUS_NO_ERROR)
			{
				printf("Error encoding 17_1 Packet!\n");
				break;
			}

			if (pus_tc_sendTCP(new_socket, buffer, packet_length, &seq, &pus_st13_transactionID) != PUS_NO_ERROR)
			{
				printf("Error sending 17_1 Packet!\n");
				break;
			}

			printf("Succesfully sent 17_1 Packet!\n");

			pus_setPacketDefaults(&outTc);
			pus_initTcPacket(&outTc);
			bzero(buffer, packet_length + 1);
			packet_length = 0;
			gotSent = true;

			break;
		}
		case 18:
		{
			if (subt != 1)
			{
				printf("Invalid Subtype! (Try '1')\n");
				break;
			}

			printf("Enter String ID: ");
			fgets(buffer18_1_id, 10, stdin);

			pusSt18ObcpId_t outTc18Id;
			for (int i = 0; i < 10; i++)
			{
				outTc18Id.arr[i] = (byte) buffer18_1_id[i];
			}

			usleep(5);

			printf("Enter String to be sent: ");
			fgets(buffer18_1_string, 15000, stdin);

			for (int i = 0; i < 15000; i++)
			{
				outTc18String.arr[i] = buffer18_1_string[i];
				if (buffer18_1_string[i] == 0x00)
				{
					outTc18String.nCount = i;
					i = 15000;
				}
			}

			if (pus_tc_18_1_createLoadObcpDirectRequest(&outTc, apid, seq, &outTc18Id, &outTc18String) != PUS_NO_ERROR)
			{
				printf("Error creating 18_1 Packet!\n");
				break;
			}

			if (encodePacket(&outTc, buffer, &packet_length) != PUS_NO_ERROR)
			{
				printf("Error encoding 18_1 Packet!\n");
				break;
			}

			if (pus_tc_sendTCP(new_socket, buffer, packet_length, &seq, &pus_st13_transactionID) != PUS_NO_ERROR)
			{
				printf("Error sending 18_1 Packet!\n");
				break;
			}

			printf("Succesfully sent 18_1 Packet!\n");

			pus_setPacketDefaults(&outTc);
			pus_initTcPacket(&outTc);
			bzero(buffer, packet_length + 1);
			packet_length = 0;
			bzero(buffer18_1_id, 10);
			bzero(buffer18_1_string, outTc18String.nCount + 1);
			gotSent = true;

			break;
		}
		case 20:
		{
			switch (subt)
			{
			case 1:
			{
				printf("Enter number of parameters: ");
				scanf("%d", &buffer20_1_3_count);

				if (buffer20_1_3_count > 20)
				{
					printf("Invalid number! (Max %d", 20);
					break;
				}

				for (int i = 0; i < buffer20_1_3_count; i++)
				{
					printf("Enter ID of parameter %d: ", i + 1);
					scanf("%d", &buffer20_1_3_id[i]);

					if (buffer20_1_3_id[i] >= pus_ST20_PARAM_LIMIT)
					{
						printf("Invalid number! (Max %d", pus_ST20_PARAM_LIMIT);
						break;
					}
				}

				if (pus_tc_20_1_createParameterValueRequest(&outTc, apid, seq, buffer20_1_3_id, buffer20_1_3_count) != PUS_NO_ERROR)
				{
					printf("Error creating 20_1 Packet!\n");
					break;
				}

				if (encodePacket(&outTc, buffer, &packet_length) != PUS_NO_ERROR)
				{
					printf("Error encoding 20_1 Packet!\n");
					break;
				}

				if (pus_tc_sendTCP(new_socket, buffer, packet_length, &seq, &pus_st13_transactionID) != PUS_NO_ERROR)
				{
					printf("Error sending 20_1 Packet!\n");
					break;
				}

				printf("Succesfully sent 20_1 Packet!\n");

				pus_setPacketDefaults(&outTc);
				pus_initTcPacket(&outTc);
				bzero(buffer, packet_length + 1);
				packet_length = 0;
				bzero(buffer20_1_3_id, (buffer20_1_3_count + 1) * sizeof(pusSt20OnBoardParamId_t));
				buffer20_1_3_count = 0;
				gotSent = true;

				break;
			}
			case 3:
			{
				printf("Enter number of parameters: ");
				scanf("%d", &buffer20_1_3_count);

				if (buffer20_1_3_count > 20)
				{
					printf("Invalid number! (Max %d", 20);
					break;
				}

				for (int i = 0; i < buffer20_1_3_count; i++)
				{
					printf("Enter ID of parameter %d: ", i + 1);
					scanf("%d", &buffer20_1_3_id[i]);

					if (buffer20_1_3_id[i] >= pus_ST20_PARAM_LIMIT)
					{
						printf("Invalid number! (Max %d", pus_ST20_PARAM_LIMIT);
						break;
					}

					printf("Enter value of parameter %d: ", i + 1);
					scanf("%lf", &buffer20_3_value[i]);
				}

				if (pus_tc_20_3_createSetParameterValueRequest(&outTc, apid, seq, buffer20_1_3_id, buffer20_3_value, buffer20_1_3_count) != PUS_NO_ERROR)
				{
					printf("Error creating 20_3 Packet!\n");
					break;
				}

				if (encodePacket(&outTc, buffer, &packet_length) != PUS_NO_ERROR)
				{
					printf("Error encoding 20_3 Packet!\n");
					break;
				}

				if (pus_tc_sendTCP(new_socket, buffer, packet_length, &seq, &pus_st13_transactionID) != PUS_NO_ERROR)
				{
					printf("Error sending 20_3 Packet!\n");
					break;
				}

				printf("Succesfully sent 20_3 Packet!\n");

				pus_setPacketDefaults(&outTc);
				pus_initTcPacket(&outTc);
				bzero(buffer, packet_length + 1);
				packet_length = 0;
				bzero(buffer20_1_3_id, (buffer20_1_3_count + 1) * sizeof(pusSt20OnBoardParamId_t));
				bzero(buffer20_3_value, (buffer20_1_3_count + 1) * sizeof(pusStoredParam_t));
				buffer20_1_3_count = 0;
				gotSent = true;

				break;
			}
			default:
			{
				printf("Invalid Subtype! (Try '1' or '3')\n");
			}
			}

			break;
		}
		default:
		{
			printf("Invalid Service! \nTry:\n\n");
			printf("3\tHousekeeping\n");
			printf("\t\t5\tEnable parameter for housekeeping report\n");
			printf("\t\t6\tDisable parameter for housekeeping report\n");
			printf("\t\t7\tEnable parameter for diagnostic report\n");
			printf("\t\t8\tDisable parameter for diagnostic report\n");
			printf("17\tTest\n");
			printf("\t\t1\tAre you alive? - Ping\n");
			printf("18\tOn-board operations procedure\n");
			printf("\t\t1\tSend a string command\n");
			printf("20\tOn-board parameter management\n");
			printf("\t\t1\tReport parameter values\n");
			printf("\t\t3\tSet parameter values\n");
			printf("\n");
		}
		}

		//	-	-	-	Non-blocking check, if another file descriptor is ready

		recVal = poll(pfds, 2, 0);

		if (pfds[0].revents & POLLIN || pfds[1].revents & POLLIN)
		{
			usleep(500);
			continue;
		}

		usleep(5000);
	}

	printf("\nServer Routine Stopped!\n");
}

void start_client()
{
	//	-	-	-	Initialization of parameters

	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent* server;
	portno = PORT;

	char buffer[65542];
	bzero(buffer, 65542);

	flag gotRecv = false;
	pusPacket_t outTm;
	pusPacket_t inTc;
	pusApid_t apid = 1;
	pusSequenceCount_t seq = 0;
	int pus_st13_transactionID = 1;
	pus_initTmPacket(&outTm);
	pus_initTcPacket(&inTc);
	asn1SccPusSt01FailureInfo fai;

	asn1SccPusSt05EventAuxData buffer5_1_data, buffer5_2_data, buffer5_3_data, buffer5_4_data;
	asn1SccPusSt05EventId buffer5_1_id, buffer5_2_id, buffer5_3_id, buffer5_4_id;
	asn1SccPusSt05Event buffer5_X_event;
	byte st05Flags = 0;

	asn1SccPusSt18ObcpCode inTc18String;
	char buffer18_1_id[10];
	bzero(buffer18_1_id, 10);
	char buffer18_1_string[15000];
	bzero(buffer18_1_string, 15000);

	pusSt20OnBoardParamId_t buffer20_2_id[20];
	bzero(buffer20_2_id, 20 * sizeof(pusSt20OnBoardParamId_t));
	pusStoredParam_t buffer20_2_value[20];
	bzero(buffer20_2_value, 20 * sizeof(pusStoredParam_t));

	unsigned int hRepId = 0;
	unsigned int dRepId = 0;
	pusTime_t housekeepingInterval = { 27, 0 };
	pusTime_t diagnosticInterval = { 10, 0 };
	pusTime_t now, thenH, thenD;
	pus_now(&thenH);
	pus_now(&thenD);

	unsigned int length = 0;
	unsigned int socketData = 0;

	//	-	-	-	Initialization of socket

	printf("Started Client!\n");

CLIENT_CONNECT: 
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("Client: Socket failed!\n");
		return;
	}

	server = gethostbyname(SERVER_IP);

	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char*)server->h_addr,
		(char*)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);

	while (connect(sockfd, (const sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		usleep(500);
	}

	printf("\nClient: Socket connected!\n");

	pusMutex_t mutex;
	pus_parameters_initialize(&mutex);

	//	-	-	-	Start of service routine

	while (1)
	{
		pus_now(&now);

		//	-	-	-	Check if diagnostic interval has past

		if (((now.tv_sec - thenD.tv_sec) >= diagnosticInterval.tv_sec) && 
			((now.tv_nsec - thenD.tv_nsec) >= diagnosticInterval.tv_nsec))
		{
			if (pus_tm_3_26_createDiagnosticReport(&outTm, apid, seq, dRepId, 1) != PUS_NO_ERROR)
			{
				printf("Error creating 3_26 Packet!\n");
				pus_now(&thenD);
				continue;
			}

			if (encodePacket(&outTm, buffer, &length) != PUS_NO_ERROR)
			{
				printf("Error encoding 3_26 Packet!\n");
				pus_now(&thenD);
				continue;
			}

			if (pus_tm_sendTCP(sockfd, buffer, length, &seq, &pus_st13_transactionID, 1) != PUS_NO_ERROR)
			{
				printf("Error sending 3_26 Packet!\n");
				pus_now(&thenD);
				continue;
			}

			printf("Sent Diagnostic Data Report No. %d!\n", dRepId);
			dRepId++;

			pus_now(&thenD);
			bzero(buffer, length);
			length = 0;
			pus_setPacketDefaults(&outTm);
			pus_initTmPacket(&outTm);
		}

		//	-	-	-	Check if housekeeping interval has past

		if(((now.tv_sec - thenH.tv_sec) >= housekeepingInterval.tv_sec) && 
		   ((now.tv_nsec - thenH.tv_nsec) >= housekeepingInterval.tv_nsec))
		{
			if (pus_tm_3_25_createHousekeepingReport(&outTm, apid, seq, hRepId, 1) != PUS_NO_ERROR)
			{
				printf("Error creating 3_25 Packet!\n");
				pus_now(&thenD);
				continue;
			}

			if (encodePacket(&outTm, buffer, &length) != PUS_NO_ERROR)
			{
				printf("Error sending 3_25 Packet!\n");
				pus_now(&thenD);
				continue;
			}

			if (pus_tm_sendTCP(sockfd, buffer, length, &seq, &pus_st13_transactionID, 1) != PUS_NO_ERROR)
			{
				printf("Error sending 3_25 Packet!\n");
				pus_now(&thenD);
				continue;
			}

			printf("Sent Housekeeping Data Report No. %d!\n", hRepId);
			hRepId++;

			pus_now(&thenH);
			bzero(buffer, length);
			length = 0;
			pus_setPacketDefaults(&outTm);
			pus_initTmPacket(&outTm);
		}

		//	-	-	-	Check if an event flag has been set

		if (st05Flags)
		{
			if (st05Flags & 0x01)
			{
				buffer5_X_event = { buffer5_1_id, buffer5_1_data};
				if (pus_tm_5_1_createInformativeEventReport(&outTm, apid, seq, &buffer5_X_event, 1) != PUS_NO_ERROR)
				{
					printf("Error creating 5_1 Packet!\n");
					continue;
				}

				if (encodePacket(&outTm, buffer, &length) != PUS_NO_ERROR)
				{
					printf("Error encoding 5_1 Packet!\n");
					continue;
				}

				if (pus_tm_sendTCP(sockfd, buffer, length, &seq, &pus_st13_transactionID, 1) != PUS_NO_ERROR)
				{
					printf("Error sending 5_1 Packet!\n");
					continue;
				}

				printf("Sent Informative Event Report!\n");

				bzero(buffer, length);
				length = 0;
				buffer5_1_data.data1 = 0;
				buffer5_1_data.data2 = 0;
				buffer5_X_event.data.data1 = 0;
				buffer5_X_event.data.data2 = 0;
				buffer5_X_event.eventId = 0;
				buffer5_1_id = 0;
				pus_setPacketDefaults(&outTm);
				pus_initTmPacket(&outTm);

				st05Flags = st05Flags & 0xfe;
			}
			if (st05Flags & 0x02)
			{
				buffer5_X_event = { buffer5_2_id, buffer5_2_data };
				if (pus_tm_5_2_createLowSeverityEventReport(&outTm, apid, seq, &buffer5_X_event, 1) != PUS_NO_ERROR)
				{
					printf("Error creating 5_2 Packet!\n");
					continue;
				}

				if (encodePacket(&outTm, buffer, &length) != PUS_NO_ERROR)
				{
					printf("Error encoding 5_2 Packet!\n");
					continue;
				}

				if (pus_tm_sendTCP(sockfd, buffer, length, &seq, &pus_st13_transactionID, 1) != PUS_NO_ERROR)
				{
					printf("Error sending 5_2 Packet!\n");
					continue;
				}

				printf("Sent Low Severity Event Report!\n");

				bzero(buffer, length);
				length = 0;
				buffer5_2_data.data1 = 0;
				buffer5_2_data.data2 = 0;
				buffer5_X_event.data.data1 = 0;
				buffer5_X_event.data.data2 = 0;
				buffer5_X_event.eventId = 0;
				buffer5_2_id = 0;
				pus_setPacketDefaults(&outTm);
				pus_initTmPacket(&outTm);

				st05Flags = st05Flags & 0xfd;
			}
			if (st05Flags & 0x04)
			{
				buffer5_X_event = { buffer5_3_id, buffer5_3_data };
				if (pus_tm_5_3_createMediumSeverityEventReport(&outTm, apid, seq, &buffer5_X_event, 1) != PUS_NO_ERROR)
				{
					printf("Error creating 5_3 Packet!\n");
					continue;
				}

				if (encodePacket(&outTm, buffer, &length) != PUS_NO_ERROR)
				{
					printf("Error encoding 5_3 Packet!\n");
					continue;
				}

				if (pus_tm_sendTCP(sockfd, buffer, length, &seq, &pus_st13_transactionID, 1) != PUS_NO_ERROR)
				{
					printf("Error sending 5_3 Packet!\n");
					continue;
				}

				printf("Sent Medium Severity Event Report!\n");

				bzero(buffer, length);
				length = 0;
				buffer5_3_data.data1 = 0;
				buffer5_3_data.data2 = 0;
				buffer5_X_event.data.data1 = 0;
				buffer5_X_event.data.data2 = 0;
				buffer5_X_event.eventId = 0;
				buffer5_3_id = 0;
				pus_setPacketDefaults(&outTm);
				pus_initTmPacket(&outTm);

				st05Flags = st05Flags & 0xfb;
			}
			if (st05Flags & 0x08)
			{
				buffer5_X_event = { buffer5_4_id, buffer5_4_data };
				if (pus_tm_5_4_createHighSeverityEventReport(&outTm, apid, seq, &buffer5_X_event, 1) != PUS_NO_ERROR)
				{
					printf("Error creating 5_4 Packet!\n");
					continue;
				}

				if (encodePacket(&outTm, buffer, &length) != PUS_NO_ERROR)
				{
					printf("Error encoding 5_4 Packet!\n");
					continue;
				}

				if (pus_tm_sendTCP(sockfd, buffer, length, &seq, &pus_st13_transactionID, 1) != PUS_NO_ERROR)
				{
					printf("Error sending 5_4 Packet!\n");
					continue;
				}

				printf("Sent High Severity Event Report!\n");

				bzero(buffer, length);
				length = 0;
				buffer5_4_data.data1 = 0;
				buffer5_4_data.data2 = 0;
				buffer5_X_event.data.data1 = 0;
				buffer5_X_event.data.data2 = 0;
				buffer5_X_event.eventId = 0;
				buffer5_4_id = 0;
				pus_setPacketDefaults(&outTm);
				pus_initTmPacket(&outTm);

				st05Flags = st05Flags & 0xf7;
			}
		}

		//	-	-	-	Check if a TC packet is on the socket

		socketData = recv(sockfd, buffer, 1500, MSG_PEEK | MSG_DONTWAIT);

		if (socketData == -1)
		{
			usleep(5000);
			continue;
		}

		//	-	-	-	Receive the TC packet

		pus_tc_recvTCP(sockfd, buffer);

		if (decodePacket(buffer, &inTc) != PUS_NO_ERROR)
		{
			printf("Error receiving packet!\n");
			printf("Reconnecting!\n");
			goto CLIENT_CONNECT;
			return;
		}

		switch (inTc.data.u.tcData.header.serviceId)
		{
		case 3:
		{
			switch (inTc.data.u.tcData.header.subtypeId)
			{
			case 5:
			{
				printf("Received 3_5 Packet!\n");

				if (pus_tm_send_1_1(&outTm, &apid, &seq, &inTc, buffer, &sockfd, &pus_st13_transactionID) != PUS_NO_ERROR) continue;

				for (int i = 0; i < inTc.data.u.tcData.data.u.st_3_X.nCount; i++)
				{
					pus_st20_paramInfo[inTc.data.u.tcData.data.u.st_3_X.arr[i]].st03HousekeepingReportEnabled = true;
					printf("Client: Enabled House %d\n", inTc.data.u.tcData.data.u.st_3_X.arr[i]);
				}

				continue;
			}
			case 6:
			{
				printf("Received 3_6 Packet!\n");

				if (pus_tm_send_1_1(&outTm, &apid, &seq, &inTc, buffer, &sockfd, &pus_st13_transactionID) != PUS_NO_ERROR) continue;

				for (int i = 0; i < inTc.data.u.tcData.data.u.st_3_X.nCount; i++)
				{
					pus_st20_paramInfo[inTc.data.u.tcData.data.u.st_3_X.arr[i]].st03HousekeepingReportEnabled = false;
					printf("Client: Disabled House %d\n", inTc.data.u.tcData.data.u.st_3_X.arr[i]);
				}

				continue;
			}
			case 7:
			{
				printf("Received 3_7 Packet!\n");

				if (pus_tm_send_1_1(&outTm, &apid, &seq, &inTc, buffer, &sockfd, &pus_st13_transactionID) != PUS_NO_ERROR) continue;

				printf("NCount: %d\n", inTc.data.u.tcData.data.u.st_3_X.nCount);

				for (int i = 0; i < inTc.data.u.tcData.data.u.st_3_X.nCount; i++)
				{
					pus_st20_paramInfo[inTc.data.u.tcData.data.u.st_3_X.arr[i]].st03DiagnosticReportEnabled = true;
					printf("Enabled Diagn %d\n", inTc.data.u.tcData.data.u.st_3_X.arr[i]);
				}

				continue;
			}
			case 8:
			{
				printf("Received 3_8 Packet!\n");

				if (pus_tm_send_1_1(&outTm, &apid, &seq, &inTc, buffer, &sockfd, &pus_st13_transactionID) != PUS_NO_ERROR) continue;

				for (int i = 0; i < inTc.data.u.tcData.data.u.st_3_X.nCount; i++)
				{
					pus_st20_paramInfo[inTc.data.u.tcData.data.u.st_3_X.arr[i]].st03DiagnosticReportEnabled = false;
					printf("Disabled Diagn %d\n", inTc.data.u.tcData.data.u.st_3_X.arr[i]);
				}

				continue;
			}
			default:
			{
				printf("Subtype Error Service 3\n");

				fai = { inTc.data.u.tcData.header.subtypeId, 0, 0 };

				pus_tm_send_1_2(&outTm, &apid, &seq, &inTc, buffer, &sockfd, &pus_st13_transactionID, 18, &fai);
				continue;
			}
			}
		}
		case 17:
		{
			if (inTc.data.u.tcData.header.subtypeId != 1)
			{
				printf("Subtype Error Service 17\n");

				fai = { inTc.data.u.tcData.header.subtypeId, 0, 0 };

				pus_tm_send_1_2(&outTm, &apid, &seq, &inTc, buffer, &sockfd, &pus_st13_transactionID, 17, &fai);
				continue;
			}

			printf("Received 17_1 Packet!\n");

			if (pus_tm_send_1_1(&outTm, &apid, &seq, &inTc, buffer, &sockfd, &pus_st13_transactionID) != PUS_NO_ERROR) continue;

			if (pus_tm_17_2_createConnectionTestReport(&outTm, apid, seq, pus_getTcSource(&inTc)) != PUS_NO_ERROR)
			{
				printf("Error creating 17_2 Packet!\n");
				continue;
			}

			if (encodePacket(&outTm, buffer, &length) != PUS_NO_ERROR)
			{
				printf("Error encoding 17_2 Packet!\n");
				continue;
			}

			if (pus_tm_sendTCP(sockfd, buffer, length, &seq, &pus_st13_transactionID, pus_getTcSource(&inTc)) != PUS_NO_ERROR)
			{
				printf("Error sending 17_2 Packet!\n");
				continue;
			}

			printf("Sent 17_2 Packet!\n");

			bzero(buffer, length);
			pus_setPacketDefaults(&outTm);
			pus_initTmPacket(&outTm);

			continue;
		}
		case 18:
		{
			if (inTc.data.u.tcData.header.subtypeId != 1)
			{
				printf("Subtype Error Service 18\n");

				fai = { inTc.data.u.tcData.header.subtypeId, 0, 0 };

				pus_tm_send_1_2(&outTm, &apid, &seq, &inTc, buffer, &sockfd, &pus_st13_transactionID, 18, &fai);
				continue;
			}

			printf("Received 18_1 Packet!\n");

			printf("String ID: ");
			for (int i = 0; i < 10; i++)
			{
				buffer18_1_id[i] = inTc.data.u.tcData.data.u.st_18_1.obcpId.arr[i];
				printf("%c", (char)buffer18_1_id[i]);
			}
			printf("\n");

			printf("String: ");
			for (int i = 0; i < inTc.data.u.tcData.data.u.st_18_1.obcpCode.nCount; i++)
			{
				buffer18_1_string[i] = inTc.data.u.tcData.data.u.st_18_1.obcpCode.arr[i];
				printf("%c", (char)buffer18_1_string[i]);
			}
			printf("\n");

			if (pus_tm_send_1_1(&outTm, &apid, &seq, &inTc, buffer, &sockfd, &pus_st13_transactionID) != PUS_NO_ERROR) continue;

			continue;
		}
		case 20:
		{
			switch (inTc.data.u.tcData.header.subtypeId)
			{
			case 1:
			{
				printf("Received 20_1 Packet!\n");

				if (pus_tm_send_1_1(&outTm, &apid, &seq, &inTc, buffer, &sockfd, &pus_st13_transactionID) != PUS_NO_ERROR) continue;

				for (int i = 0; i < inTc.data.u.tcData.data.u.st_20_1.nCount; i++)
				{
					buffer20_2_id[i] = inTc.data.u.tcData.data.u.st_20_1.arr[i];
					buffer20_2_value[i] = pus_st20_params[buffer20_2_id[i]];
					printf("Param List: ID %d\t V%f\n", buffer20_2_id[i], buffer20_2_value[i]);
				}

				if (pus_tm_20_2_createParameterValueReport(&outTm, apid, seq, inTc.data.u.tcData.header.source, buffer20_2_id, buffer20_2_value, inTc.data.u.tcData.data.u.st_20_1.nCount) != PUS_NO_ERROR)
				{
					printf("Error creating 20_2 Packet!\n");
					continue;
				}

				if (encodePacket(&outTm, buffer, &length) != PUS_NO_ERROR)
				{
					printf("Error encoding 20_2 Packet!\n");
					continue;
				}

				if (pus_tm_sendTCP(sockfd, buffer, length, &seq, &pus_st13_transactionID, pus_getTcSource(&inTc)) != PUS_NO_ERROR)
				{
					printf("Error sending 20_2 Packet!\n");
					continue;
				}

				printf("Sent 20_2 Packet!\n");

				bzero(buffer, length);
				bzero(buffer20_2_id, inTc.data.u.tcData.data.u.st_20_1.nCount * sizeof(pusSt20OnBoardParamId_t));
				bzero(buffer20_2_value, inTc.data.u.tcData.data.u.st_20_1.nCount * sizeof(pusStoredParam_t));
				pus_setPacketDefaults(&outTm);
				pus_initTmPacket(&outTm);
				pus_setPacketDefaults(&inTc);
				pus_initTcPacket(&inTc);

				continue;
			}
			case 3:
			{
				printf("Received 20_3 Packet!\n");

				if (pus_tm_send_1_1(&outTm, &apid, &seq, &inTc, buffer, &sockfd, &pus_st13_transactionID) != PUS_NO_ERROR) continue;

				for (int i = 0; i < inTc.data.u.tcData.data.u.st_20_2_3.nCount; i++)
				{
					pus_st20_params[inTc.data.u.tcData.data.u.st_20_2_3.arr[i]] = inTc.data.u.tcData.data.u.st_20_2_3.value[i];
				}
				
				continue;
			}
			default:
			{
				printf("Subtype Error Service 20\n");

				fai = { inTc.data.u.tcData.header.subtypeId, 0, 0 };

				pus_tm_send_1_2(&outTm, &apid, &seq, &inTc, buffer, &sockfd, &pus_st13_transactionID, 20, &fai);
				continue;
			}
			}
			continue;
		}
		default:
		{
			printf("Service Error\n");

			fai = { inTc.data.u.tcData.header.subtypeId, 0, 0 };

			pus_tm_send_1_2(&outTm, &apid, &seq, &inTc, buffer, &sockfd, &pus_st13_transactionID, 0, &fai);
			continue;
		}
		}

		usleep(5000);
	}

	printf("\nClient Routine Stopped!\n");
}

int main(int argc, char const* argv[])
{
	if (IS_SERVER)
	{
		start_server();
	}
	else
	{
		start_client();
	}

	return 0;
}