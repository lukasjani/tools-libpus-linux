#include "pus_encoder.h"

unsigned int getDataLength(char* buffer)
{
	//	-	-	-	Returns the data length of the packet in the buffer

	unsigned int temp = 4;
	return pop16Bit(buffer, &temp);
}

void push16Bit(short* var, char* buffer, unsigned int* cont)
{
	//	-	-	-	Writes 16 bit into the buffer at offset cont

	short val = *var;
	unsigned char temp = 0;
	for (int i = 0; i < 2; i++)
	{
		temp = (val >> (8 - (8 * i)));
		buffer[*cont] = temp;
		*cont += 1;
	}
}

unsigned short pop16Bit(char* buffer, unsigned int* cont)
{
	//	-	-	-	Reads 16 bit from the buffer at offset cont

	unsigned short val = 0;
	unsigned char temp = buffer[*cont + 1];
	val |= temp;
	temp = buffer[*cont];
	val |= ((unsigned short) temp) << 8;
	*cont += 2;
	return val;
}

void push32Bit(int* var, char* buffer, unsigned int* cont)
{
	//	-	-	-	Writes 32 bit into the buffer at offset cont

	int val = *var;
	unsigned char temp = 0;
	for (int i = 0; i < 4; i++)
	{
		temp = (val >> (24 - (8 * i)));
		buffer[*cont] = temp;
		*cont += 1;
	}
}

unsigned int pop32Bit(char* buffer, unsigned int* cont)
{
	//	-	-	-	Reads 32 bit from the buffer at offset cont

	unsigned int val = 0;
	unsigned char temp = 0;
	for (int i = 0; i < 4; i++)
	{
		temp = buffer[*cont + 3 - i];
		val |= (temp << (i * 8));
	}
	*cont += 4;
	return val;
}

void push64UBit(asn1SccPusUInt64* var, char* buffer, unsigned int* cont)
{
	//	-	-	-	Writes 64 bit of unsigned type into the buffer at offset cont

	asn1SccPusUInt64 val = *var;
	unsigned char temp = 0;
	for (int i = 0; i < 8; i++)
	{
		temp = (val >> (56 - (8 * i)));
		buffer[*cont] = temp;
		*cont += 1;
	}
}

void push64SBit(asn1SccPusInt64* var, char* buffer, unsigned int* cont)
{
	//	-	-	-	Writes 64 bit of signed type into the buffer at offset cont

	asn1SccPusInt64 val = *var;
	unsigned char temp = 0;
	for (int i = 0; i < 8; i++)
	{
		temp = (val >> (56 - (8 * i)));
		buffer[*cont] = temp;
		*cont += 1;
	}
}

void push64DBit(asn1SccPusDouble* var, char* buffer, unsigned int* cont)
{
	//	-	-	-	Writes 64 bit of double type into the buffer at offset cont

	asn1SccPusInt64 val = *reinterpret_cast<asn1SccPusInt64*> (var);
	push64SBit(&val, buffer, cont);
}

asn1SccPusUInt64 pop64UBit(char* buffer, unsigned int* cont)
{
	//	-	-	-	Reads 64 bit of unsigned type from the buffer at offset cont

	asn1SccPusUInt64 val = 0x00LL;
	asn1SccPusUInt64 shift = 0x00LL;
	unsigned char temp = 0;
	for (int i = 0; i < 8; i++)
	{
		temp = buffer[*cont + 7 - i];
		shift = 0x00LL;
		shift |= temp;
		val |= (shift << (i * 8));
	}
	*cont += 8;
	return val;
}

asn1SccPusInt64 pop64SBit(char* buffer, unsigned int* cont)
{
	//	-	-	-	Reads 64 bit of signed type from the buffer at offset cont

	asn1SccPusInt64 val = 0x00LL;
	asn1SccPusInt64 shift = 0x00LL;
	unsigned char temp = 0;
	for (int i = 0; i < 8; i++)
	{
		temp = buffer[*cont + 7 - i];
		shift = 0x00LL;
		shift |= temp;
		val |= (shift << (i * 8));
	}
	*cont += 8;
	return val;
}

asn1SccPusDouble pop64DBit(char* buffer, unsigned int* cont)
{
	//	-	-	-	Reads 64 bit of double type from the buffer at offset cont

	asn1SccPusDouble val;
	asn1SccPusInt64 var = pop64SBit(buffer, cont);
	val = *reinterpret_cast<asn1SccPusDouble*> (&var);
	return val;
}

pusError_t decodeReduced(char* buffer, asn1SccPusPacketReduced* outTm, unsigned int* cont)
{
	//	-	-	-	Decodes a reduced packet from the buffer at offset cont and writes appropriate values into outTm

	//no demonstrator packet uses reduced packets
}

pusError_t decodePacket(char* buffer, pusPacket_t* outTm)
{
	//	-	-	-	Decodes a packet from the start of the buffer and writes appropriate values into outTm

	pus_clearError();

	if (NULL == outTm || NULL == buffer)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	unsigned int cont = 0;

	if (((buffer[cont] & 0xe0) >> 5) != 0)
	{
		return PUS_SET_ERROR(PUS_ERROR_UNEXPECTED_PACKET_VERSION);
	}
	pus_setPacketVersion(outTm, asn1Sccpus_CCSDS_V1);

	if ((buffer[cont] & 0x10) >> 4)
	{
		pus_setPacketType(outTm, asn1Sccpus_TC);
		pus_initTcPacket(outTm);
	}
	else
	{
		pus_setPacketType(outTm, asn1Sccpus_TM);
		pus_initTmPacket(outTm);
	}

	pus_setSecondaryHeaderFlag(outTm, (buffer[cont] & 0x08) >> 3);

	pus_setApid(outTm, pop16Bit(buffer, &cont) & 0x07ff);

	if (((buffer[cont] & 0xc0) >> 6) != 0x03)
	{
		return PUS_SET_ERROR(PUS_ERROR_UNEXPECTED_SEQUENCE_FLAGS);
	}
	pus_setSequenceFlags(outTm, asn1Sccpus_STANDALONE_PACKET);

	pus_setSequenceCount(outTm, pop16Bit(buffer, &cont) & 0x3fff);

	pus_setPacketDataLength(outTm, pop16Bit(buffer, &cont));

	unsigned int* point = &cont;

	if (outTm->packetType == asn1Sccpus_TC)
	{
		if (outTm->secondaryHeaderFlag)
		{
			return decodeTc(buffer, outTm, cont);
		}
		else
		{
			return decodeTcNoHeader(buffer, outTm, point);
		}
	}
	else
	{
		if (outTm->secondaryHeaderFlag)
		{
			return decodeTm(buffer, outTm, cont);
		}
		else
		{
			return decodeTmNoHeader(buffer, outTm, point);
		}
	}

	return PUS_ERROR_DEFINITION_NOT_FOUND;
}

pusError_t decodeTmNoHeader(char* buffer, pusPacket_t* outTm, unsigned int* cont)
{
	//	-	-	-	Decodes TM packet user data from the buffer at offset cont and writes appropriate values into outTm

	if (NULL == outTm || NULL == buffer)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (*cont < 6)
	{
		return PUS_SET_ERROR(PUS_ERROR_LIMIT);
	}
	

	if (pus_getSecondaryHeaderFlag(outTm))
	{
		switch (pus_getTmService(outTm))
		{
		case 0: //pusService_NONE
		{
			return PUS_SET_ERROR(PUS_ERROR_TM_SERVICE);
		}
		case 1: //pus_ST01_requestVerification
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			case 1:
			case 3:
			case 7:
			{
				pus_setTmDataKind(outTm, st_1_X_PRESENT);

				if ((buffer[*cont] & 0xe0 >> 5) != 0)
				{
					return PUS_SET_ERROR(PUS_ERROR_UNEXPECTED_PACKET_VERSION);
				}
				pus_tm_1_X_setPacketVersionNumber(outTm, asn1Sccpus_CCSDS_V1);

				if ((buffer[*cont] & 0x10) >> 4)
				{
					pus_tm_1_X_setPacketType(outTm, asn1Sccpus_TC);
				}
				else
				{
					pus_tm_1_X_setPacketType(outTm, asn1Sccpus_TM);
				}

				pus_tm_1_X_setSecondaryHeaderFlag(outTm, (buffer[*cont] & 0x0a) >> 7);

				pus_tm_1_X_setApid(outTm, pop16Bit(buffer, cont) & 0x07ff);

				if (((buffer[*cont] & 0xc0) >> 6) != 0x03)
				{
					return PUS_SET_ERROR(PUS_ERROR_UNEXPECTED_SEQUENCE_FLAGS);
				}
				pus_tm_1_X_setSequenceFlags(outTm, asn1Sccpus_STANDALONE_PACKET);

				pus_tm_1_X_setSequenceCount(outTm, pop16Bit(buffer, cont) & 0x3fff);

				return PUS_GET_ERROR();
			}
			case 2:
			case 4:
			case 8:
			case 10:
			{
				pus_setTmDataKind(outTm, st_1_X_PRESENT);

				if ((buffer[*cont] & 0xe0 >> 5) != 0)
				{
					return PUS_SET_ERROR(PUS_ERROR_UNEXPECTED_PACKET_VERSION);
				}
				pus_tm_1_X_setPacketVersionNumber(outTm, asn1Sccpus_CCSDS_V1);

				if ((buffer[*cont] & 0x10) >> 4)
				{
					pus_tm_1_X_setPacketType(outTm, asn1Sccpus_TC);
				}
				else
				{
					pus_tm_1_X_setPacketType(outTm, asn1Sccpus_TM);
				}

				pus_tm_1_X_setSecondaryHeaderFlag(outTm, (buffer[*cont] & 0x0a) >> 7);

				pus_tm_1_X_setApid(outTm, pop16Bit(buffer, cont) & 0x07ff);

				if (((buffer[*cont] & 0xc0) >> 6) != 0x03)
				{
					return PUS_SET_ERROR(PUS_ERROR_UNEXPECTED_SEQUENCE_FLAGS);
				}
				pus_tm_1_X_setSequenceFlags(outTm, asn1Sccpus_STANDALONE_PACKET);

				pus_tm_1_X_setSequenceCount(outTm, pop16Bit(buffer, cont) & 0x3fff);

				pusSt01FailureCode_t packetCode = pop64UBit(buffer, cont);

				pusSt01FailureInfo_t packetInfo;

				pus_setSt01FailureInfo(&packetInfo, pop64SBit(buffer, cont), pop64SBit(buffer, cont), pop64SBit(buffer, cont));

				pus_tm_1_X_setFailureInfo(outTm, packetCode, &packetInfo);

				return PUS_GET_ERROR();
			}
			case 5:
			{
				pus_setTmDataKind(outTm, st_1_X_PRESENT);

				if ((buffer[*cont] & 0xe0 >> 5) != 0)
				{
					return PUS_SET_ERROR(PUS_ERROR_UNEXPECTED_PACKET_VERSION);
				}
				pus_tm_1_X_setPacketVersionNumber(outTm, asn1Sccpus_CCSDS_V1);

				if ((buffer[*cont] & 0x10) >> 4)
				{
					pus_tm_1_X_setPacketType(outTm, asn1Sccpus_TC);
				}
				else
				{
					pus_tm_1_X_setPacketType(outTm, asn1Sccpus_TM);
				}

				pus_tm_1_X_setPacketType(outTm, asn1Sccpus_TM);

				pus_tm_1_X_setApid(outTm, pop16Bit(buffer, cont) & 0x07ff);

				if (((buffer[*cont] & 0xc0) >> 6) != 0x03)
				{
					return PUS_SET_ERROR(PUS_ERROR_UNEXPECTED_SEQUENCE_FLAGS);
				}
				pus_tm_1_X_setSequenceFlags(outTm, asn1Sccpus_STANDALONE_PACKET);

				pus_tm_1_X_setSequenceCount(outTm, pop16Bit(buffer, cont) & 0x3fff);

				pus_tm_1_X_setStep(outTm, pop64UBit(buffer, cont));

				return PUS_GET_ERROR();
			}
			case 6:
			{
				pus_setTmDataKind(outTm, st_1_X_PRESENT);

				if ((buffer[*cont] & 0xe0 >> 5) != 0)
				{
					return PUS_SET_ERROR(PUS_ERROR_UNEXPECTED_PACKET_VERSION);
				}
				pus_tm_1_X_setPacketVersionNumber(outTm, asn1Sccpus_CCSDS_V1);

				if ((buffer[*cont] & 0x10) >> 4)
				{
					pus_tm_1_X_setPacketType(outTm, asn1Sccpus_TC);
				}
				else
				{
					pus_tm_1_X_setPacketType(outTm, asn1Sccpus_TM);
				}

				pus_tm_1_X_setApid(outTm, pop16Bit(buffer, cont) & 0x07ff);

				pus_tm_1_X_setApid(outTm, pop16Bit(buffer, cont) & 0x07ff);

				if (((buffer[*cont] & 0xc0) >> 6) != 0x03)
				{
					return PUS_SET_ERROR(PUS_ERROR_UNEXPECTED_SEQUENCE_FLAGS);
				}
				pus_tm_1_X_setSequenceFlags(outTm, asn1Sccpus_STANDALONE_PACKET);

				pus_tm_1_X_setSequenceCount(outTm, pop16Bit(buffer, cont) & 0x3fff);

				pus_tm_1_X_setStep(outTm, pop64UBit(buffer, cont));

				pusSt01FailureCode_t packetCode = pop64UBit(buffer, cont);

				pusSt01FailureInfo_t packetInfo;

				pus_setSt01FailureInfo(&packetInfo, pop64SBit(buffer, cont), pop64SBit(buffer, cont), pop64SBit(buffer, cont));

				pus_tm_1_X_setFailureInfo(outTm, packetCode, &packetInfo);

				return PUS_GET_ERROR();
			}
			default:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			}
		}
		case 2: //pus_ST02_deviceAccess
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 3: //pus_ST03_housekeeping
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			case 5:
			case 6:
			case 7:
			case 8:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			case 25:
			case 26:
			{
				pus_setTmDataKind(outTm, st_3_X_PRESENT);

				pus_tm_3_X_setReportId(outTm, pop64UBit(buffer, cont));

				int tempCount = pop32Bit(buffer, cont);

				pus_tm_3_X_setNumParameters(outTm, tempCount);

				for (int i = 0; i < tempCount; i++)
				{
					pus_tm_3_X_setParameterValue(outTm, i, pop64DBit(buffer, cont));
				}

				return PUS_GET_ERROR();
			}
			default:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			}
		}
		case 4: //pus_ST04_parameterStatisticsReporting
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 5: //pus_ST05_eventReporting
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			case 1:
			case 2:
			case 3:
			case 4:
			{
				pus_setTmDataKind(outTm, st_5_X_PRESENT);

				pusSt05Event_t packetId;

				packetId.eventId = pop64UBit(buffer, cont);
				packetId.data.data1 = pop64UBit(buffer, cont);
				packetId.data.data2 = pop64UBit(buffer, cont);

				pus_tm_5_X_setTmEventReportData(outTm, &packetId);

				return PUS_GET_ERROR();
			}
			case 5:
			case 6:
			case 7:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			case 8:
			{
				pus_setTmDataKind(outTm, st_5_8_PRESENT);

				int tempCount = pop32Bit(buffer, cont);

				pus_tc_tm_5_X_setNCount(outTm, tempCount);

				pusSt05Event_t packetEvents[20];

				for (int i = 0; i < tempCount; i++)
				{
					packetEvents[i].eventId = pop64UBit(buffer, cont);
					packetEvents[i].data.data1 = pop64UBit(buffer, cont);
					packetEvents[i].data.data2 = pop64UBit(buffer, cont);

					pus_tc_tm_5_X_setEvent(outTm, &(packetEvents[i]), i);
				}

				return PUS_GET_ERROR();
			}
			default:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			}
		}
		case 6: //pus_ST06_memoryManagement
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 7: //reserved
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 8: //pus_ST08_functionManagement
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			case 1:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			}
		}
		case 9: //pus_ST09_timeManagement
		{
			return PUS_SET_ERROR(PUS_ERROR_HEADER_MISMATCH);
		}
		case 10: //reserved
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 11: //pus_ST11_timeBasedScheduling
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			case 1:
			case 2:
			case 3:
			case 4:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			}
		}
		case 12: //pus_ST12_onBoardMonitoring
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			case 1:
			case 2:
			case 15:
			case 16:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			}
		}
		case 13: //pus_ST13_largePacketTransfer
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			case 1:
			case 2:
			case 3:
			{
				pus_setTmDataKind(outTm, st_13_X_PRESENT);

				pus_tc_tm_13_X_setTransactionID(outTm, pop32Bit(buffer, cont));
				pus_tc_tm_13_X_setPartSeqNumber(outTm, pop32Bit(buffer, cont));

				char packetContent[pus_ST13_MAX_PART_SIZE];

				for (int i = 0; i < pus_ST13_MAX_PART_SIZE; i++)
				{
					packetContent[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_tm_13_X_setContent(outTm, packetContent);

				return PUS_GET_ERROR();
			}
			case 9:
			case 10:
			case 11:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			case 16:
			{
				pus_setTmDataKind(outTm, st_13_16_PRESENT);

				pus_tc_tm_13_X_setTransactionID(outTm, pop32Bit(buffer, cont));

				pus_tm_13_16_setFailureReason(outTm, pop32Bit(buffer, cont));

				return PUS_GET_ERROR();
			}
			default:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			}
		}
		case 14: //pus_ST14_realTimeForwardingControl
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 15: //pus_ST15_onBoardStorageAndRetrieval
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 16: //reserved
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 17: //pus_ST17_test
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			case 1:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			case 2:
				return PUS_GET_ERROR();
			default:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			}
		}
		case 18: //pus_ST18_onBoardControlProcedure
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 12:
			case 13:
			case 21:
			case 22:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			}
		}
		case 19: //pus_ST19_eventAction
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			}
		}
		case 20: //pus_ST20_parameterManagement
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			case 1:
			case 3:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			case 2:
			{
				pus_setTmDataKind(outTm, st_20_2_PRESENT);

				int tempCount = pop32Bit(buffer, cont);

				pus_tc_tm_20_X_setNCount(outTm, tempCount);

				for (int i = 0; i < tempCount; i++)
				{
					pus_tc_tm_20_X_setParamId(outTm, pop64UBit(buffer, cont), i);
					pus_tc_tm_20_2_3_setParamValue(outTm, pop64DBit(buffer, cont), i);
				}

				return PUS_GET_ERROR();
			}
			default:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			}
		}
		case 21: //pus_ST21_requestSequencing
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 22: //pus_ST22_partitionBasedScheduling
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 23: //pus_ST23_fileManagement
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			case 1:
			case 2:
			case 3:
			case 5:
			case 6:
			case 7:
			case 9:
			case 10:
			case 11:
			case 12:
			case 14:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			case 4:
			{
				pus_setTmDataKind(outTm, st_23_4_PRESENT);

				int tempCount = pop32Bit(buffer, cont);

				pusSt23RepositoryPath_t packetRep;

				packetRep.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetRep.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_tm_23_X_setRepositoryPath(outTm, &packetRep);

				tempCount = pop32Bit(buffer, cont);

				pusSt23FileName_t packetFile;

				packetFile.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetFile.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_tm_23_X_setFileName(outTm, &packetFile);

				pus_tc_tm_23_1_4_setMaximumSize(outTm, pop64UBit(buffer, cont));

				if (buffer[*cont])
				{
					pus_tc_tm_23_1_4_setLock(outTm, true);
				}
				else
				{
					pus_tc_tm_23_1_4_setLock(outTm, false);
				}
				*cont += 1;

				return PUS_GET_ERROR();
			}

			case 8:
			{
				pus_setTmDataKind(outTm, st_23_8_PRESENT);

				int tempCount = pop32Bit(buffer, cont);

				pusSt23RepositoryPath_t packetRep;

				packetRep.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetRep.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_tm_23_X_setRepositoryPath(outTm, &packetRep);

				tempCount = pop32Bit(buffer, cont);

				asn1SccPusByteArray packetSearch;

				packetSearch.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetSearch.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_tm_23_7_8_setSearchPattern(outTm, &packetSearch);

				tempCount = pop32Bit(buffer, cont);

				pus_tm_23_8_setMatches(outTm, tempCount);

				pusSt23FileName_t packetFilePaths[30];

				for (int i = 0; i < tempCount; i++)
				{
					packetFilePaths[i].nCount = pop32Bit(buffer, cont);

					for (int j = 0; j < packetFilePaths[i].nCount; j++)
					{
						packetFilePaths[i].arr[j] = buffer[*cont];
						*cont += 1;
					}
				}

				pus_tm_23_8_setFilePaths(outTm, packetFilePaths);

				return PUS_GET_ERROR();
			}
			case 13:
			{
				pus_setTmDataKind(outTm, st_23_13_PRESENT);

				int tempCount = pop32Bit(buffer, cont);

				pusSt23RepositoryPath_t packetRep;

				packetRep.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetRep.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_tm_23_X_setRepositoryPath(outTm, &packetRep);

				tempCount = pop32Bit(buffer, cont);

				pus_tm_23_13_setNumberObjects(outTm, tempCount);

				asn1SccPusSt23ObjectType packetTypes[30];
				asn1SccPusByteArray packetObjects[30];

				for (int i = 0; i < tempCount; i++)
				{
					if (buffer[*cont] == 'D')
					{
						packetTypes[i] = st_23_13_directory;
					}
					else if (buffer[*cont] == 'F')
					{
						packetTypes[i] = st_23_13_file;
					}
					else
					{
						return PUS_SET_ERROR(PUS_ERROR_DEFINITION_NOT_FOUND);
					}
					*cont += 1;

					packetObjects[i].nCount = pop32Bit(buffer, cont);

					for (int j = 0; j < packetObjects[i].nCount; j++)
					{
						packetObjects[i].arr[j] = buffer[*cont];
						*cont += 1;
					}
				}

				pus_tm_23_13_setObjectTypes(outTm, packetTypes);

				pus_tm_23_13_setObjectNames(outTm, packetObjects);

				return PUS_GET_ERROR();
			}
			default:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			}
		}
		default:
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		}
	}
	else
	{
		pus_setTmDataKind(outTm, st_9_2_PRESENT);

		pusSt09ExponentialRate_t packetExp = pop64UBit(buffer, cont);

		pusTime_t packetTime = { pop64SBit(buffer, cont) , pop64SBit(buffer, cont) };

		pus_tm_9_2_3_setDataField(outTm, &packetTime, &packetExp);

		return PUS_GET_ERROR();
	}
}

pusError_t decodeTm(char* buffer, pusPacket_t* outTm, unsigned int cont)
{
	//	-	-	-	Decodes TM packet header from the buffer at offset cont and writes appropriate values into outTm

	if (NULL == outTm || NULL == buffer)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (cont != 6)
	{
		return PUS_SET_ERROR(PUS_ERROR_LIMIT);
	}

	switch (buffer[cont] >> 4)
	{
	case 0: pus_setTmPusVersion(outTm, asn1Sccpus_V0);
		break;
	case 1: pus_setTmPusVersion(outTm, asn1Sccpus_V1);
		break;
	case 2: pus_setTmPusVersion(outTm, asn1Sccpus_V2);
		break;
	default: pus_setTmPusVersion(outTm, asn1Sccpus_INVALID_VERSION);
	}

	pus_setTmTimeReferenceStatus(outTm, buffer[cont] & 0x0f);
	cont += 1;

	pus_setTmService(outTm, buffer[cont]);
	cont += 1;

	pus_setTmSubtype(outTm, buffer[cont]);
	cont += 1;

	pus_setTmMessageTypeCounter(outTm, pop16Bit(buffer, &cont));

	pus_setTmDestination(outTm, pop16Bit(buffer, &cont));

	pusTime_t packetTime = { pop64SBit(buffer, &cont) , pop64SBit(buffer, &cont) };

	pus_setTmPacketTime(outTm, &packetTime);

	return decodeTmNoHeader(buffer, outTm, &cont);
}

pusError_t decodeTcNoHeader(char* buffer, pusPacket_t* outTc, unsigned int* cont)
{
	//	-	-	-	Decodes TC packet user data from the buffer at offset cont and writes appropriate values into outTc

	if (NULL == outTc || NULL == buffer)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (*cont < 6)
	{
		return PUS_SET_ERROR(PUS_ERROR_LIMIT);
	}

	if (pus_getSecondaryHeaderFlag(outTc))
	{
		switch (pus_getTcService(outTc))
		{
		case 0: //pusService_NONE
		{
			return PUS_SET_ERROR(PUS_ERROR_TC_SERVICE);
		}
		case 1: //pus_ST01_requestVerification
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 10:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TC);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 2: //pus_ST02_deviceAccess
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 3: //pus_ST03_housekeeping
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 5:
			case 6:
			case 7:
			case 8:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_3_X_PRESENT);

				pus_tc_3_X_setNumParameters(outTc, pop32Bit(buffer, cont));

				for (int i = 0; i < outTc->data.u.tcData.data.u.st_3_X.nCount; i++)
				{
					pus_tc_3_X_setParamId(outTc, pop64UBit(buffer, cont), i);
				}

				return PUS_GET_ERROR();
			}
			case 25:
			case 26:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TC);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 4: //pus_ST04_parameterStatisticsReporting
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 5: //pus_ST05_eventReporting
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			case 2:
			case 3:
			case 4:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TC);
			case 5:
			case 6:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_5_X_PRESENT);

				int tempCount = pop32Bit(buffer, cont);

				pus_tc_tm_5_X_setNCount(outTc, tempCount);

				pusSt05Event_t packetEvents[20];

				for (int i = 0; i < tempCount; i++)
				{
					packetEvents[i].eventId = pop64UBit(buffer, cont);
					packetEvents[i].data.data1 = pop64UBit(buffer, cont);
					packetEvents[i].data.data2 = pop64UBit(buffer, cont);

					pus_tc_tm_5_X_setEvent(outTc, &(packetEvents[i]), i);
				}

				return PUS_GET_ERROR();
			}
			case 7:
				return PUS_GET_ERROR();
			case 8:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TC);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 6: //pus_ST06_memoryManagement
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 7: //reserved
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 8: //pus_ST08_functionManagement
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_8_1_PRESENT);

				pus_tc_8_1_setFunctionId(outTc, pop64UBit(buffer, cont));

				return PUS_GET_ERROR();
			}
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 9: //pus_ST09_timeManagement
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_9_1_PRESENT);

				pus_tc_9_1_setExponentialRate(outTc, pop64UBit(buffer, cont));

				return PUS_GET_ERROR();
			}
			case 2:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TC);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 10: //reserved
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 11: //pus_ST11_timeBasedScheduling
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			case 2:
			case 3:
				return PUS_GET_ERROR();
			case 4:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_11_4_PRESENT);

				int tempCount = pop32Bit(buffer, cont);

				pus_tc_11_4_setNCount(outTc, tempCount);

				pusTime_t packetTime[5];
				pusPacket_t packetPackets[5];
				asn1SccPusPacketReduced packetReduced[5];

				for (int i = 0; i < tempCount; i++)
				{
					packetTime[i].tv_sec = pop64SBit(buffer, cont);
					packetTime[i].tv_nsec = pop64SBit(buffer, cont);

					pusError_t error;
					error = decodeReduced(buffer, &(packetReduced[i]), cont);

					if (error != PUS_NO_ERROR) return error;

					pus_packetReduced_createPacketFromPacketReduced(&(packetPackets[i]), &(packetReduced[i]));

					pus_tc_11_4_setActivity(outTc, &(packetPackets[i]), &(packetTime[i]));
				}

				return PUS_GET_ERROR();
			}
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 12: //pus_ST12_onBoardMonitoring
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			case 2:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_12_1_2_PRESENT);

				int tempCount = pop32Bit(buffer, cont);

				pus_tc_12_1_2_setNCount(outTc, tempCount);

				for (int i = 0; i < tempCount; i++)
				{
					pus_tc_12_1_2_setPmonId(outTc, pop64UBit(buffer, cont), i);
				}

				return PUS_GET_ERROR();
			}
			case 15:
			case 16:
				return PUS_GET_ERROR();

			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 13: //pus_ST13_largePacketTransfer
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			case 2:
			case 3:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TC);
			case 9:
			case 10:
			case 11:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_13_X_PRESENT);

				pus_tc_tm_13_X_setTransactionID(outTc, pop32Bit(buffer, cont));
				pus_tc_tm_13_X_setPartSeqNumber(outTc, pop32Bit(buffer, cont));

				char packetContent[pus_ST13_MAX_PART_SIZE];

				for (int i = 0; i < pus_ST13_MAX_PART_SIZE; i++)
				{
					packetContent[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_tm_13_X_setContent(outTc, packetContent);

				return PUS_GET_ERROR();
			}
			case 16:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TC);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 14: //pus_ST14_realTimeForwardingControl
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 15: //pus_ST15_onBoardStorageAndRetrieval
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 16: //reserved
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 17: //pus_ST17_test
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
				return PUS_GET_ERROR();
			case 2:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TC);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 18: //pus_ST18_onBoardControlProcedure
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_18_1_PRESENT);

				pusSt18ObcpId_t packetId;

				for (int i = 0; i < 10; i++)
				{
					packetId.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_18_X_setObcpId(outTc, &packetId);

				pusSt18ObcpCode_t packetCode;

				packetCode.nCount = pop32Bit(buffer, cont);

				for (int i = 0; i < packetCode.nCount; i++)
				{
					packetCode.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_18_1_setObcpCode(outTc, &packetCode);

				pus_tc_18_1_setObcpChecksum(outTc, pop64UBit(buffer, cont));

				return PUS_GET_ERROR();
			}
			case 2:
			case 6:
			case 12:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_18_2_6_12_PRESENT);

				pusSt18ObcpId_t packetId;

				for (int i = 0; i < 10; i++)
				{
					packetId.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_18_X_setObcpId(outTc, &packetId);

				return PUS_GET_ERROR();
			}
			case 3:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_18_3_PRESENT);

				pusSt18ObcpId_t packetId;

				for (int i = 0; i < 10; i++)
				{
					packetId.arr[i] = buffer[*cont];
					*cont += 1;
				}

				int packetObsLev = pop32Bit(buffer, cont);

				switch (packetObsLev)
				{
				case 0: pus_tc_18_3_setObservabilityLevel(outTc, asn1Sccnone);
					break;
				case 1: pus_tc_18_3_setObservabilityLevel(outTc, asn1Sccprocedure);
					break;
				case 2: pus_tc_18_3_setObservabilityLevel(outTc, asn1Sccstep);
					break;
				case 3: pus_tc_18_3_setObservabilityLevel(outTc, asn1Sccdetailed);
					break;
				}

				return PUS_GET_ERROR();
			}
			case 4:
			case 5:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_18_4_5_PRESENT);

				pusSt18ObcpId_t packetId;

				for (int i = 0; i < 10; i++)
				{
					packetId.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_18_4_5_setObcpStepId(outTc, pop64UBit(buffer, cont));

				return PUS_GET_ERROR();
			}
			case 13:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_18_13_PRESENT);

				pusSt18ObcpId_t packetId;

				for (int i = 0; i < 10; i++)
				{
					packetId.arr[i] = buffer[*cont];
					*cont += 1;
				}

				int tempCount = pop32Bit(buffer, cont);

				pusSt23FileName_t packetFile;

				packetFile.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetFile.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_18_13_setFileName(outTc, &packetFile);

				tempCount = pop32Bit(buffer, cont);

				pusSt23RepositoryPath_t packetRepos;

				packetRepos.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetRepos.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_18_13_setRepositoryPath(outTc, &packetRepos);

				return PUS_GET_ERROR();
			}
			case 21:
			case 22:
				return PUS_GET_ERROR();
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 19: //pus_ST19_eventAction
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_19_1_PRESENT);

				int tempCount = pop32Bit(buffer, cont);

				pus_tc_19_X_setNCount(outTc, tempCount);

				asn1SccPusSt05EventId packetEvents[20];
				asn1SccPusPacketReduced packetReduced[20];

				for (int i = 0; i < tempCount; i++)
				{
					packetEvents[i] = pop64UBit(buffer, cont);
					pus_tc_19_X_setEventId(outTc, &(packetEvents[i]), i);

					decodeReduced(buffer, &(packetReduced[i]), cont);

					pus_tc_19_1_setAction(outTc, &(packetReduced[i]), i);
				}

				return PUS_GET_ERROR();
			}
			case 2:
			case 4:
			case 5:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_19_X_PRESENT);

				int tempCount = pop32Bit(buffer, cont);

				pus_tc_19_X_setNCount(outTc, tempCount);

				pusSt05EventId_t packetEvents[20];

				for (int i = 0; i < tempCount; i++)
				{
					packetEvents[i] = pop64UBit(buffer, cont);

					pus_tc_19_X_setEventId(outTc, &(packetEvents[i]), i);
				}

				return PUS_GET_ERROR();

			}
			case 3:
				return PUS_GET_ERROR();
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 20: //pus_ST20_parameterManagement
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_20_1_PRESENT);

				int tempCount = pop32Bit(buffer, cont);

				pus_tc_tm_20_X_setNCount(outTc, tempCount);

				for (int i = 0; i < tempCount; i++)
				{
					pus_tc_tm_20_X_setParamId(outTc, pop64UBit(buffer, cont), i);
				}

				return PUS_GET_ERROR();
			}
			case 2:
				return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
			case 3:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_20_3_PRESENT);

				int tempCount = pop32Bit(buffer, cont);

				pus_tc_tm_20_X_setNCount(outTc, tempCount);

				for (int i = 0; i < tempCount; i++)
				{
					pus_tc_tm_20_X_setParamId(outTc, pop64UBit(buffer, cont), i);
					pus_tc_tm_20_2_3_setParamValue(outTc, pop64DBit(buffer, cont), i);
				}

				return PUS_GET_ERROR();
			}
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 21: //pus_ST21_requestSequencing
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 22: //pus_ST22_partitionBasedScheduling
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 23: //pus_ST23_fileManagement
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_23_1_PRESENT);

				int tempCount = pop32Bit(buffer, cont);

				pusSt23RepositoryPath_t packetRepos;

				packetRepos.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetRepos.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_tm_23_X_setRepositoryPath(outTc, &packetRepos);

				tempCount = pop32Bit(buffer, cont);

				pusSt23FileName_t packetFile;

				packetFile.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetFile.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_tm_23_X_setFileName(outTc, &packetFile);

				pus_tc_tm_23_1_4_setMaximumSize(outTc, pop64UBit(buffer, cont));

				if (buffer[*cont])
				{
					pus_tc_tm_23_1_4_setLock(outTc, true);
				}
				else
				{
					pus_tc_tm_23_1_4_setLock(outTc, false);
				}
				*cont += 1;

				return PUS_GET_ERROR();
			}
			case 2:
			case 3:
			case 5:
			case 6:
			case 7:
			case 9:
			case 10:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_23_X_PRESENT);

				int tempCount = pop32Bit(buffer, cont);

				pusSt23RepositoryPath_t packetRepos;

				packetRepos.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetRepos.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_tm_23_X_setRepositoryPath(outTc, &packetRepos);

				tempCount = pop32Bit(buffer, cont);

				pusSt23FileName_t packetFile;

				packetFile.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetFile.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_tm_23_X_setFileName(outTc, &packetFile);

				return PUS_GET_ERROR();
			}
			case 11:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_23_11_PRESENT);

				int tempCount = pop32Bit(buffer, cont);

				pusSt23RepositoryPath_t packetRepos;

				packetRepos.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetRepos.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_tm_23_X_setRepositoryPath(outTc, &packetRepos);

				tempCount = pop32Bit(buffer, cont);

				pusSt23DirectoryName_t packetDirOld;

				packetDirOld.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetDirOld.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_23_11_setOldDirectoryName(outTc, &packetDirOld);

				tempCount = pop32Bit(buffer, cont);

				pusSt23DirectoryName_t packetDirNew;

				packetDirNew.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetDirNew.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_23_11_setNewDirectoryName(outTc, &packetDirNew);

				return PUS_GET_ERROR();
			}
			case 12:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_23_12_PRESENT);

				int tempCount = pop32Bit(buffer, cont);

				pusSt23RepositoryPath_t packetRepos;

				packetRepos.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetRepos.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_tm_23_X_setRepositoryPath(outTc, &packetRepos);

				return PUS_GET_ERROR();
			}
			case 14:
			{
				pus_setTcDataKind(outTc, PusTcApplicationData_st_23_14_PRESENT);

				int tempCount = pop32Bit(buffer, cont);

				pusSt23RepositoryPath_t packetReposSource;

				packetReposSource.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetReposSource.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_23_14_setSourceRepositoryPath(outTc, &packetReposSource);

				tempCount = pop32Bit(buffer, cont);

				pusSt23FileName_t packetFileSource;

				packetFileSource.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetFileSource.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_23_14_setSourceFileName(outTc, &packetFileSource);

				tempCount = pop32Bit(buffer, cont);

				pusSt23RepositoryPath_t packetReposTarget;

				packetReposTarget.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetReposTarget.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_23_14_setTargetRepositoryPath(outTc, &packetReposTarget);

				tempCount = pop32Bit(buffer, cont);

				pusSt23FileName_t packetFileTarget;

				packetFileTarget.nCount = tempCount;

				for (int i = 0; i < tempCount; i++)
				{
					packetFileTarget.arr[i] = buffer[*cont];
					*cont += 1;
				}

				pus_tc_23_14_setTargetFileName(outTc, &packetFileTarget);

				return PUS_GET_ERROR();
			}
			case 4:
			case 8:
			case 13:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TC);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		default:
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		}
	}
	else 
	{
		if (pus_getTcService(outTc) != pus_ST02_deviceAccess)
		{
			return PUS_SET_ERROR(PUS_ERROR_HEADER_MISMATCH);
		}

		return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
	}
}

pusError_t decodeTc(char* buffer, pusPacket_t* outTc, unsigned int cont)
{
	//	-	-	-	Decodes TC packet header from the buffer at offset cont and writes appropriate values into outTc

	if (NULL == outTc || NULL == buffer)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (cont != 6)
	{
		return PUS_SET_ERROR(PUS_ERROR_LIMIT);
	}

	switch (buffer[cont] >> 4)
	{
	case 0: pus_setTcPusVersion(outTc, asn1Sccpus_V0);
		break;
	case 1: pus_setTcPusVersion(outTc, asn1Sccpus_V1);
		break;
	case 2: pus_setTcPusVersion(outTc, asn1Sccpus_V2);
		break;
	default: pus_setTcPusVersion(outTc, asn1Sccpus_INVALID_VERSION);
	}

	pus_setTcAckFlags(outTc, buffer[cont] & 0x08, buffer[cont] & 0x04, buffer[cont] & 0x02, buffer[cont] & 0x01);
	cont += 1;

	pus_setTcService(outTc, buffer[cont]);
	cont += 1;

	pus_setTcSubtype(outTc, buffer[cont]);
	cont += 1;

	pus_setTcSource(outTc, pop16Bit(buffer, &cont));

	return decodeTcNoHeader(buffer, outTc, &cont);
}

pusError_t encodeReduced(asn1SccPusPacketReduced* outTm, char* buffer, unsigned int* cont)
{
	//	-	-	-	Encodes the reduced packet outTm into the buffer at offset cont

	//no demonstrator packet uses reduced packets
}

pusError_t encodePacket(pusPacket_t* outTm, char* buffer, unsigned int* length)
{
	//	-	-	-	Encodes a packet into the start of the buffer and writes the datalength when it finishes

	pus_clearError();

	if (NULL == outTm || NULL == buffer)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	unsigned int cont = 0;

	char temp = 0;
	temp = (pus_getPacketVersion(outTm) << 5) | (pus_getPacketType(outTm) << 4) | (pus_getSecondaryHeaderFlag(outTm) << 3) | (pus_getApid(outTm) >> 8);
	buffer[cont] = temp;
	cont += 1;
	temp = (pus_getApid(outTm) & 0xff);
	buffer[cont] = temp;
	cont += 1;
	temp = (pus_getSequenceFlags(outTm) << 6) | (pus_getSequenceCount(outTm) >> 8);
	buffer[cont] = temp;
	cont += 1;
	temp = (pus_getSequenceCount(outTm) & 0xff);
	buffer[cont] = temp;
	cont += 1;
	temp = (pus_getPacketDataLength(outTm) >> 8);
	buffer[cont] = temp;
	cont += 1;
	temp = (pus_getPacketDataLength(outTm) & 0xff);
	buffer[cont] = temp;
	cont += 1;
	*length = cont;

	switch (pus_getPacketDataKind(outTm))
	{
	case PusPacketData_NONE:
		return PUS_SET_ERROR(PUS_ERROR_PACKET_TYPE);
	case PusPacketData_tmData_PRESENT:
		encodeTm(outTm, buffer, length);
		break;
	case PusPacketData_tmDataNoHeader_PRESENT:
		encodeTmNoHeader(outTm, buffer, length);
		break;
	case PusPacketData_tcData_PRESENT:
		encodeTc(outTm, buffer, length);
		break;
	case PusPacketData_tcDataNoHeader_PRESENT:
		encodeTcNoHeader(outTm, buffer, length);
		break;
	default:
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	unsigned int realDataLength = *length;
	temp = (realDataLength >> 8);
	buffer[4] = temp;
	temp = (realDataLength & 0xff);
	buffer[5] = temp;

	pus_setPacketDataLength(outTm, realDataLength);

	return PUS_GET_ERROR();
}

pusError_t encodeTmNoHeader(pusPacket_t* outTm, char* buffer, unsigned int* cont)
{
	//	-	-	-	Encodes TM packet user data into the the buffer at offset cont

	if (NULL == outTm || NULL == buffer)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (*cont < 6)
	{
		return PUS_SET_ERROR(PUS_ERROR_LIMIT);
	}

	if (pus_getSecondaryHeaderFlag(outTm))
	{
		switch (pus_getTmService(outTm))
		{
		case 0: //pusService_NONE
		{
			return PUS_SET_ERROR(PUS_ERROR_TM_SERVICE);
		}
		case 1: //pus_ST01_requestVerification
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			case 1:
			case 3:
			case 7:
			{
				if (pus_getTmDataKind(outTm) != st_1_X_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TM_KIND);
				}

				char temp = 0;
				temp = (pus_tm_1_X_getPacketVersionNumber(outTm) << 5) | (pus_tm_1_X_getPacketType(outTm) << 4) | (pus_tm_1_X_getSecondaryHeaderFlag(outTm) << 3) | (pus_tm_1_X_getApid(outTm) >> 8);
				buffer[*cont] = temp;
				*cont += 1;
				temp = (pus_tm_1_X_getApid(outTm) & 0xff);
				buffer[*cont] = temp;
				*cont += 1;
				temp = (pus_tm_1_X_getSequenceFlags(outTm) << 6) | (pus_tm_1_X_getSequenceCount(outTm) >> 8);
				buffer[*cont] = temp;
				*cont += 1;
				temp = (pus_tm_1_X_getSequenceCount(outTm) & 0xff);
				buffer[*cont] = temp;
				*cont += 1;

				return PUS_GET_ERROR();
			}
			case 2:
			case 4:
			case 8:
			case 10:
			{
				if (pus_getTmDataKind(outTm) != st_1_X_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TM_KIND);
				}

				char temp = 0;
				temp = (pus_tm_1_X_getPacketVersionNumber(outTm) << 5) | (pus_tm_1_X_getPacketType(outTm) << 4) | (pus_tm_1_X_getSecondaryHeaderFlag(outTm) << 3) | (pus_tm_1_X_getApid(outTm) >> 8);
				buffer[*cont] = temp;
				*cont += 1;
				temp = (pus_tm_1_X_getApid(outTm) & 0xff);
				buffer[*cont] = temp;
				*cont += 1;
				temp = (pus_tm_1_X_getSequenceFlags(outTm) << 6) | (pus_tm_1_X_getSequenceCount(outTm) >> 8);
				buffer[*cont] = temp;
				*cont += 1;
				temp = (pus_tm_1_X_getSequenceCount(outTm) & 0xff);
				buffer[*cont] = temp;
				*cont += 1;

				pusSt01FailureInfo_t packetFailInfo;
				pusSt01FailureCode_t packetFailCode;
				packetFailCode = pus_tm_1_X_getFailureInfo(outTm, &packetFailInfo);

				push64UBit(&packetFailCode, buffer, cont);
				push64SBit(&(packetFailInfo.subcode), buffer, cont);
				push64SBit(&(packetFailInfo.data), buffer, cont);
				push64UBit(&(packetFailInfo.address), buffer, cont);

				return PUS_GET_ERROR();
			}
			case 5:
			{
				if (pus_getTmDataKind(outTm) != st_1_X_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TM_KIND);
				}

				char temp = 0;
				temp = (pus_tm_1_X_getPacketVersionNumber(outTm) << 5) | (pus_tm_1_X_getPacketType(outTm) << 4) | (pus_tm_1_X_getSecondaryHeaderFlag(outTm) << 3) | (pus_tm_1_X_getApid(outTm) >> 8);
				buffer[*cont] = temp;
				*cont += 1;
				temp = (pus_tm_1_X_getApid(outTm) & 0xff);
				buffer[*cont] = temp;
				*cont += 1;
				temp = (pus_tm_1_X_getSequenceFlags(outTm) << 6) | (pus_tm_1_X_getSequenceCount(outTm) >> 8);
				buffer[*cont] = temp;
				*cont += 1;
				temp = (pus_tm_1_X_getSequenceCount(outTm) & 0xff);
				buffer[*cont] = temp;
				*cont += 1;

				pusStepId_t packetStep = pus_tm_1_X_getStep(outTm);

				push64UBit(&packetStep, buffer, cont);

				return PUS_GET_ERROR();
			}
			case 6:
			{
				if (pus_getTmDataKind(outTm) != st_1_X_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TM_KIND);
				}

				char temp = 0;
				temp = (pus_tm_1_X_getPacketVersionNumber(outTm) << 5) | (pus_tm_1_X_getPacketType(outTm) << 4) | (pus_tm_1_X_getSecondaryHeaderFlag(outTm) << 3) | (pus_tm_1_X_getApid(outTm) >> 8);
				buffer[*cont] = temp;
				*cont += 1;
				temp = (pus_tm_1_X_getApid(outTm) & 0xff);
				buffer[*cont] = temp;
				*cont += 1;
				temp = (pus_tm_1_X_getSequenceFlags(outTm) << 6) | (pus_tm_1_X_getSequenceCount(outTm) >> 8);
				buffer[*cont] = temp;
				*cont += 1;
				temp = (pus_tm_1_X_getSequenceCount(outTm) & 0xff);
				buffer[*cont] = temp;
				*cont += 1;

				pusStepId_t packetStep = pus_tm_1_X_getStep(outTm);

				push64UBit(&packetStep, buffer, cont);

				pusSt01FailureInfo_t packetFailInfo;
				pusSt01FailureCode_t packetFailCode;
				packetFailCode = pus_tm_1_X_getFailureInfo(outTm, &packetFailInfo);

				push64UBit(&packetFailCode, buffer, cont);
				push64SBit(&(packetFailInfo.subcode), buffer, cont);
				push64SBit(&(packetFailInfo.data), buffer, cont);
				push64UBit(&(packetFailInfo.address), buffer, cont);

				return PUS_GET_ERROR();
			}
			default:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			}
		}
		case 2: //pus_ST02_deviceAccess
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 3: //pus_ST03_housekeeping
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			case 5:
			case 6:
			case 7:
			case 8:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			case 25:
			case 26:
			{

				if (pus_getTmDataKind(outTm) != st_3_X_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TM_KIND);
				}

				pusSt03HousekeepingReportId_t packetReportId = pus_tm_3_X_getReportId(outTm);

				push64UBit(&packetReportId, buffer, cont);

				int tempCount;

				pus_tm_3_X_getNumParameters(outTm, &tempCount);

				push32Bit(&tempCount, buffer, cont);

				pusStoredParam_t tempValue;

				for (int i = 0; i < tempCount; i++)
				{
					pus_tm_3_X_getParameterValue(outTm, i, &tempValue);

					push64DBit(&tempValue, buffer, cont);
				}

				return PUS_GET_ERROR();
			}
			default:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			}
		}
		case 4: //pus_ST04_parameterStatisticsReporting
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 5: //pus_ST05_eventReporting
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			case 1:
			case 2:
			case 3:
			case 4:
			{
				if (pus_getTmDataKind(outTm) != st_5_X_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TM_KIND);
				}

				pusSt05Event_t packetEvent;

				pus_tm_5_X_getTmEventReportData(outTm, &packetEvent);

				push64UBit(&(packetEvent.eventId), buffer, cont);
				push64UBit(&(packetEvent.data.data1), buffer, cont);
				push64UBit(&(packetEvent.data.data2), buffer, cont);

				return PUS_GET_ERROR();
			}
			case 5:
			case 6:
			case 7:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			case 8:
			{
				if (pus_getTmDataKind(outTm) != st_5_8_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TM_KIND);
				}

				int tempCount = pus_tc_tm_5_X_getNCount(outTm);

				push32Bit(&tempCount, buffer, cont);

				pusSt05Event_t packetEvents[20];

				for (int i = 0; i < tempCount; i++)
				{
					pus_tc_tm_5_X_getEvent(outTm, &(packetEvents[i]), i);

					push64UBit(&(packetEvents[i].eventId), buffer, cont);
					push64UBit(&(packetEvents[i].data.data1), buffer, cont);
					push64UBit(&(packetEvents[i].data.data2), buffer, cont);
				}

				return PUS_GET_ERROR();
			}
			default:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			}
		}
		case 6: //pus_ST06_memoryManagement
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 7: //reserved
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 8: //pus_ST08_functionManagement
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			case 1:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			}
		}
		case 9: //pus_ST09_timeManagement
		{
			return PUS_SET_ERROR(PUS_ERROR_HEADER_MISMATCH);
		}
		case 10: //reserved
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 11: //pus_ST11_timeBasedScheduling
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			case 1:
			case 2:
			case 3:
			case 4:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TM_SUBTYPE);
			}
		}
		case 12: //pus_ST12_onBoardMonitoring
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			case 2:
			case 15:
			case 16:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 13: //pus_ST13_largePacketTransfer
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			case 2:
			case 3:
			{
				if (pus_getTmDataKind(outTm) != st_13_X_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TM_KIND);
				}

				int packetId;

				pus_tc_tm_13_X_getTransactionID(&packetId, outTm);

				push32Bit(&packetId, buffer, cont);

				int packetSeq;

				pus_tc_tm_13_X_getPartSeqNumber(&packetSeq, outTm);

				push32Bit(&packetSeq, buffer, cont);

				char packetContent[pus_ST13_MAX_PART_SIZE];

				pus_tc_tm_13_X_getContent(packetContent, outTm);

				for (int i = 0; i < pus_ST13_MAX_PART_SIZE; i++)
				{
					buffer[*cont] = packetContent[i];
					*cont += 1;
				}

				return PUS_GET_ERROR();
			}
			case 9:
			case 10:
			case 11:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			case 16:
			{
				if (pus_getTmDataKind(outTm) != st_13_16_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TM_KIND);
				}

				int packetId;

				pus_tc_tm_13_X_getTransactionID(&packetId, outTm);

				push32Bit(&packetId, buffer, cont);

				int packetFail;

				pus_tm_13_16_getFailureReason(&packetFail, outTm);

				push32Bit(&packetFail, buffer, cont);

				return PUS_GET_ERROR();
			}
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 14: //pus_ST14_realTimeForwardingControl
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 15: //pus_ST15_onBoardStorageAndRetrieval
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 16: //reserved
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 17: //pus_ST17_test
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			case 2:
				return PUS_GET_ERROR();
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 18: //pus_ST18_onBoardControlProcedure
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 12:
			case 13:
			case 21:
			case 22:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 19: //pus_ST19_eventAction
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 20: //pus_ST20_parameterManagement
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			case 3:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			case 2:
			{
				if (pus_getTmDataKind(outTm) != st_20_2_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TM_KIND);
				}

				int tempCount = pus_tc_tm_20_X_getNCount(outTm);

				push32Bit(&tempCount, buffer, cont);

				pusSt20OnBoardParamId_t tempId;
				pusStoredParam_t tempValue;

				for (int i = 0; i < tempCount; i++)
				{
					tempId = pus_tc_tm_20_X_getParamId(outTm, i);
					push64UBit(&tempId, buffer, cont);

					tempValue = pus_tc_tm_20_2_3_getParamValue(outTm, i);
					push64DBit(&tempValue, buffer, cont);
				}

				return PUS_GET_ERROR();
			}
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 21: //pus_ST21_requestSequencing
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 22: //pus_ST22_partitionBasedScheduling
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 23: //pus_ST23_fileManagement
		{
			switch (pus_getTmSubtype(outTm))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			case 2:
			case 3:
			case 5:
			case 6:
			case 7:
			case 9:
			case 10:
			case 11:
			case 12:
			case 14:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TM);
			case 4:
			{
				if (pus_getTmDataKind(outTm) != st_23_4_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TM_KIND);
				}

				pusSt23RepositoryPath_t packetRepos;

				pus_tc_tm_23_X_getRepositoryPath(&packetRepos, outTm);

				int tempCount = packetRepos.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = (char)(packetRepos.arr[i]);
					*cont += 1;
				}

				pusSt23FileName_t packetFile;

				pus_tc_tm_23_X_getFileName(&packetFile, outTm);

				tempCount = packetFile.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = (char)(packetFile.arr[i]);
					*cont += 1;
				}

				pusSt23MaximumSize_t packetSize;

				pus_tc_tm_23_1_4_getMaximumSize(&packetSize, outTm);

				push64UBit(&packetSize, buffer, cont);

				bool packetLock;

				pus_tc_tm_23_1_4_getLock(&packetLock, outTm);

				if (packetLock)
				{
					buffer[*cont] = 1;
				}
				else
				{
					buffer[*cont] = 0;
				}
				*cont += 1;

				return PUS_GET_ERROR();
			}

			case 8:
			{
				if (pus_getTmDataKind(outTm) != st_23_8_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TM_KIND);
				}

				pusSt23RepositoryPath_t packetRepos;

				pus_tc_tm_23_X_getRepositoryPath(&packetRepos, outTm);

				int tempCount = packetRepos.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = (char)(packetRepos.arr[i]);
					*cont += 1;
				}

				asn1SccPusByteArray packetSearch;

				pus_tc_tm_23_7_8_getSearchPattern(&packetSearch, outTm);

				tempCount = packetSearch.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = (char)(packetSearch.arr[i]);
					*cont += 1;
				}

				int packetMatches;

				pus_tm_23_8_getMatches(&packetMatches, outTm);

				push32Bit(&packetMatches, buffer, cont);

				asn1SccPusByteArray packetPaths[30];

				pus_tm_23_8_getFilePaths(packetPaths, outTm);

				for (int i = 0; i < packetMatches; i++)
				{
					tempCount = packetPaths[i].nCount;

					push32Bit(&tempCount, buffer, cont);

					for (int j = 0; j < tempCount; j++)
					{
						buffer[*cont] = (char)(packetPaths[i].arr[j]);
						*cont += 1;
					}
				}

				return PUS_GET_ERROR();
			}
			case 13:
			{
				if (pus_getTmDataKind(outTm) != st_23_13_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TM_KIND);
				}

				pusSt23RepositoryPath_t packetRepos;

				pus_tc_tm_23_X_getRepositoryPath(&packetRepos, outTm);

				int tempCount = packetRepos.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = (char)(packetRepos.arr[i]);
					*cont += 1;
				}

				int packetNumber;

				pus_tm_23_13_getNumberObjects(&packetNumber, outTm);

				push32Bit(&packetNumber, buffer, cont);

				asn1SccPusSt23ObjectType packetTypes[30];

				pus_tm_23_13_getObjectTypes(packetTypes, outTm);

				asn1SccPusByteArray packetObjects[30];

				pus_tm_23_13_getObjectNames(packetObjects, outTm);

				for (int i = 0; i < packetNumber; i++)
				{
					buffer[*cont] = (char)(packetTypes[i]);
					*cont += 1;

					tempCount = packetObjects[i].nCount;

					push32Bit(&tempCount, buffer, cont);

					for (int j = 0; j < 30; j++)
					{
						buffer[*cont] = (char)(packetObjects[i].arr[j]);
						*cont += 1;
					}
					buffer[*cont] = 0;
					*cont += 1;
				}

				return PUS_GET_ERROR();
			}
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		default:
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		}
	}
	else
	{
		if (pus_getTmNoHeaderDataKind(outTm) != st_9_2_PRESENT)
		{
			return PUS_SET_ERROR(PUS_ERROR_TM_KIND);
		}

		pusSt09ExponentialRate_t packetExp;
		pusTime_t packetTime;

		pus_tm_9_2_3_getDataField(outTm, &packetTime, &packetExp);

		push64UBit(&packetExp, buffer, cont);
		push64SBit(&(packetTime.tv_sec), buffer, cont);
		push64SBit(&(packetTime.tv_nsec), buffer, cont);

		return PUS_GET_ERROR();
	}
}

pusError_t encodeTm(pusPacket_t* outTm, char* buffer, unsigned int* cont)
{
	//	-	-	-	Encodes TM packet header into the the buffer at offset cont

	if (NULL == outTm || NULL == buffer)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (*cont != 6)
	{
		return PUS_SET_ERROR(PUS_ERROR_LIMIT);
	}

	char temp = 0;

	temp = (pus_getTmPusVersion(outTm) << 4) | pus_getTmTimeReferenceStatus(outTm);
	buffer[*cont] = temp;
	*cont += 1;

	temp = pus_getTmService(outTm);
	buffer[*cont] = temp;
	*cont += 1;

	temp = pus_getTmSubtype(outTm);
	buffer[*cont] = temp;
	*cont += 1;

	temp = (outTm->data.u.tmData.header.msgTypeCount >> 8);
	buffer[*cont] = temp;
	*cont += 1;

	temp = (pus_getTmMessageTypeCounter(outTm) & 0xff);
	buffer[*cont] = temp;
	*cont += 1;

	temp = (pus_getTmMessageTypeCounter(outTm) >> 8);
	buffer[*cont] = temp;
	*cont += 1;

	temp = (pus_getTmDestination(outTm) & 0xff);
	buffer[*cont] = temp;
	*cont += 1;

	pusTime_t packetTime;
	pus_getTmPacketTime(&packetTime, outTm);

	push64SBit(&(packetTime.tv_sec), buffer, cont);
	push64SBit(&(packetTime.tv_nsec), buffer, cont);

	return encodeTmNoHeader(outTm, buffer, cont);
}

pusError_t encodeTcNoHeader(pusPacket_t* outTc, char* buffer, unsigned int* cont)
{
	//	-	-	-	Encodes TC packet user data into the the buffer at offset cont

	if (NULL == outTc || NULL == buffer)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (*cont < 6)
	{
		return PUS_SET_ERROR(PUS_ERROR_LIMIT);
	}

	if (pus_getSecondaryHeaderFlag(outTc))
	{
		switch (pus_getTcService(outTc))
		{
		case 0: //pusService_NONE
		{
			return PUS_SET_ERROR(PUS_ERROR_TC_SERVICE);
		}
		case 1: //pus_ST01_requestVerification
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 10:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TC);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 2: //pus_ST02_deviceAccess
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 3: //pus_ST03_housekeeping
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 5:
			case 6:
			case 7:
			case 8:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_3_X_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				int tempCount;

				pus_tc_3_X_getNumParameters(outTc, &tempCount);

				push32Bit(&tempCount, buffer, cont);

				pusSt03HousekeepingReportId_t tempId;

				for (int i = 0; i < tempCount; i++)
				{
					pus_tc_3_X_getParamId(outTc, i, &tempId);

					push64UBit(&tempId, buffer, cont);
				}

				return PUS_GET_ERROR();
			}
			case 25:
			case 26:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TC);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 4: //pus_ST04_parameterStatisticsReporting
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 5: //pus_ST05_eventReporting
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			case 2:
			case 3:
			case 4:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TC);
			case 5:
			case 6:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_5_X_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				int tempCount = pus_tc_tm_5_X_getNCount(outTc);

				push32Bit(&tempCount, buffer, cont);

				pusSt05Event_t packetEvents[20];

				for (int i = 0; i < tempCount; i++)
				{
					pus_tc_tm_5_X_getEvent(outTc, &(packetEvents[i]), i);

					push64UBit(&(packetEvents[i].eventId), buffer, cont);
					push64UBit(&(packetEvents[i].data.data1), buffer, cont);
					push64UBit(&(packetEvents[i].data.data1), buffer, cont);
				}

				return PUS_GET_ERROR();
			}
			case 7:
				return PUS_GET_ERROR();
			case 8:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TC);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 6: //pus_ST06_memoryManagement
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 7: //reserved
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 8: //pus_ST08_functionManagement
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_8_1_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				pusSt08FunctiontId_t packetId;

				pus_tc_8_1_getFunctionId(&packetId, outTc);

				push64UBit(&packetId, buffer, cont);

				return PUS_GET_ERROR();
			}
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 9: //pus_ST09_timeManagement
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_9_1_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				pusSt09ExponentialRate_t packetExp;

				pus_tc_9_1_getExponentialRate(&packetExp, outTc);

				push64UBit(&packetExp, buffer, cont);

				return PUS_GET_ERROR();
			}
			case 2:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TC);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 10: //reserved
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 11: //pus_ST11_timeBasedScheduling
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			case 2:
			case 3:
				return PUS_GET_ERROR();
			case 4:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_11_4_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				int32_t tempCount = pus_tc_11_4_getNCount(outTc);

				push32Bit(&tempCount, buffer, cont);

				pusSt11ScheduledActivity_t packetActivities[5];

				pus_tc_11_4_getActivities(&tempCount, packetActivities, outTc);

				for (int i = 0; i < tempCount; i++)
				{
					push64SBit(&(packetActivities[i].time.tv_sec), buffer, cont);
					push64SBit(&(packetActivities[i].time.tv_nsec), buffer, cont);

					pusError_t error;
					error = encodeReduced(&(packetActivities[i].packetReduced), buffer, cont);

					if (error != PUS_NO_ERROR) return error;
				}

				return PUS_GET_ERROR();
			}
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 12: //pus_ST12_onBoardMonitoring
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			case 2:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_12_1_2_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				int tempCount = pus_tc_12_1_2_getNCount(outTc);

				push32Bit(&tempCount, buffer, cont);

				pusSt12PmonId_t packetIds[20];

				pus_tc_12_1_2_getPmonId(packetIds, outTc);

				for (int i = 0; i < tempCount; i++)
				{
					push64UBit(&(packetIds[i]), buffer, cont);
				}

				return PUS_GET_ERROR();
			}
			case 15:
			case 16:
				return PUS_GET_ERROR();

			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 13: //pus_ST13_largePacketTransfer
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			case 2:
			case 3:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TC);
			case 9:
			case 10:
			case 11:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_13_X_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TM_KIND);
				}

				int packetId;

				pus_tc_tm_13_X_getTransactionID(&packetId, outTc);

				push32Bit(&packetId, buffer, cont);

				int packetSeq;

				pus_tc_tm_13_X_getPartSeqNumber(&packetSeq, outTc);

				push32Bit(&packetSeq, buffer, cont);

				char packetContent[pus_ST13_MAX_PART_SIZE];

				pus_tc_tm_13_X_getContent(packetContent, outTc);

				for (int i = 0; i < pus_ST13_MAX_PART_SIZE; i++)
				{
					buffer[*cont] = packetContent[i];
					*cont += 1;
				}

				return PUS_GET_ERROR();
			}
			case 16:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TC);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 14: //pus_ST14_realTimeForwardingControl
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 15: //pus_ST15_onBoardStorageAndRetrieval
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 16: //reserved
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 17: //pus_ST17_test
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
				return PUS_GET_ERROR();
			case 2:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TC);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 18: //pus_ST18_onBoardControlProcedure
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_18_1_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				pusSt18ObcpId_t packetId;

				pus_tc_18_X_getObcpId(&packetId, outTc);

				for (int i = 0; i < 10; i++)
				{
					buffer[*cont] = packetId.arr[i];
					*cont += 1;
				}

				pusSt18ObcpCode_t packetCode;

				pus_tc_18_1_getObcpCode(&packetCode, outTc);

				int tempCount = packetCode.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = packetCode.arr[i];
					*cont += 1;
				}

				asn1SccPusSt18ObcpChecksum packetChecksum;

				pus_tc_18_1_getObcpChecksum(&packetChecksum, outTc);

				push64UBit(&packetChecksum, buffer, cont);

				return PUS_GET_ERROR();
			}
			case 2:
			case 6:
			case 12:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_18_2_6_12_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				pusSt18ObcpId_t packetId;

				pus_tc_18_X_getObcpId(&packetId, outTc);

				for (int i = 0; i < 10; i++)
				{
					buffer[*cont] = packetId.arr[i];
					*cont += 1;
				}

				return PUS_GET_ERROR();
			}
			case 3:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_18_3_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				pusSt18ObcpId_t packetId;

				pus_tc_18_X_getObcpId(&packetId, outTc);

				for (int i = 0; i < 10; i++)
				{
					buffer[*cont] = packetId.arr[i];
					*cont += 1;
				}

				pusSt18ObservabilityLevel_t packetObserv;

				pus_tc_18_3_getObservabilityLevel(&packetObserv, outTc);

				int tempObserv = packetObserv;

				push32Bit(&tempObserv, buffer, cont);

				return PUS_GET_ERROR();
			}
			case 4:
			case 5:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_18_4_5_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				pusSt18ObcpId_t packetId;

				pus_tc_18_X_getObcpId(&packetId, outTc);

				for (int i = 0; i < 10; i++)
				{
					buffer[*cont] = packetId.arr[i];
					*cont += 1;
				}

				pusSt18ObcpStepId_t packetStep;

				pus_tc_18_4_5_getObcpStepId(&packetStep, outTc);

				push64UBit(&packetStep, buffer, cont);

				return PUS_GET_ERROR();
			}
			case 13:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_18_13_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				pusSt18ObcpId_t packetId;

				pus_tc_18_X_getObcpId(&packetId, outTc);

				for (int i = 0; i < 10; i++)
				{
					buffer[*cont] = packetId.arr[i];
					*cont += 1;
				}

				pusSt23RepositoryPath_t packetRepos;

				pus_tc_18_13_getRepositoryPath(&packetRepos, outTc);

				int tempCount = packetRepos.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = packetRepos.arr[i];
					*cont += 1;
				}

				pusSt23FileName_t packetFile;

				pus_tc_18_13_getFileName(&packetFile, outTc);

				tempCount = packetFile.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = packetFile.arr[i];
					*cont += 1;
				}

				return PUS_GET_ERROR();
			}
			case 21:
			case 22:
				return PUS_GET_ERROR();
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 19: //pus_ST19_eventAction
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_19_1_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				int tempCount = pus_tc_19_X_getNCount(outTc);

				push32Bit(&tempCount, buffer, cont);

				pusSt05EventId_t packetIds[20];
				pusPacketReduced_t packetPackets[20];

				for (int i = 0; i < tempCount; i++)
				{
					pus_tc_19_X_getEventId(&(packetIds[i]), outTc, i);
					pus_tc_19_1_getAction(&(packetPackets[i]), outTc, i);

					push64UBit(&(packetIds[i]), buffer, cont);
					encodeReduced(&(packetPackets[i]), buffer, cont);
				}

				return PUS_GET_ERROR();
			}
			case 2:
			case 4:
			case 5:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_19_X_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				int tempCount = pus_tc_19_X_getNCount(outTc);

				push32Bit(&tempCount, buffer, cont);

				pusSt05EventId_t packetIds[20];

				for (int i = 0; i < tempCount; i++)
				{
					pus_tc_19_X_getEventId(&(packetIds[i]), outTc, i);

					push64UBit(&(packetIds[i]), buffer, cont);
				}

				return PUS_GET_ERROR();
			}
			case 3:
				return PUS_GET_ERROR();
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 20: //pus_ST20_parameterManagement
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_20_1_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				int tempCount = pus_tc_tm_20_X_getNCount(outTc);

				push32Bit(&tempCount, buffer, cont);

				pusSt20OnBoardParamId_t packetIds[20];

				for (int i = 0; i < tempCount; i++)
				{
					packetIds[i] = pus_tc_tm_20_X_getParamId(outTc, i);

					push64UBit(&(packetIds[i]), buffer, cont);
				}

				return PUS_GET_ERROR();
			}
			case 2:
				return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
			case 3:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_20_3_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				int tempCount = pus_tc_tm_20_X_getNCount(outTc);

				push32Bit(&tempCount, buffer, cont);

				pusSt20OnBoardParamId_t packetIds[20];
				pusStoredParam_t packetValues[20];

				for (int i = 0; i < tempCount; i++)
				{
					packetIds[i] = pus_tc_tm_20_X_getParamId(outTc, i);
					packetValues[i] = pus_tc_tm_20_2_3_getParamValue(outTc, i);

					push64UBit(&(packetIds[i]), buffer, cont);
					push64DBit(&(packetValues[i]), buffer, cont);
				}

				return PUS_GET_ERROR();
			}
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		case 21: //pus_ST21_requestSequencing
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 22: //pus_ST22_partitionBasedScheduling
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		case 23: //pus_ST23_fileManagement
		{
			switch (pus_getTcSubtype(outTc))
			{
			case 0:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			case 1:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_23_1_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				pusSt23RepositoryPath_t packetRepos;

				pus_tc_tm_23_X_getRepositoryPath(&packetRepos, outTc);

				int tempCount = packetRepos.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = (char)(packetRepos.arr[i]);
					*cont += 1;
				}

				pusSt23FileName_t packetFile;

				pus_tc_tm_23_X_getFileName(&packetFile, outTc);

				tempCount = packetFile.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = (char)(packetFile.arr[i]);
					*cont += 1;
				}

				pusSt23MaximumSize_t packetSize;

				pus_tc_tm_23_1_4_getMaximumSize(&packetSize, outTc);

				push64UBit(&packetSize, buffer, cont);

				bool packetLock;

				pus_tc_tm_23_1_4_getLock(&packetLock, outTc);

				if (packetLock)
				{
					buffer[*cont] = 1;
				}
				else
				{
					buffer[*cont] = 0;
				}
				*cont += 1;

				return PUS_GET_ERROR();
			}
			case 2:
			case 3:
			case 5:
			case 6:
			case 7:
			case 9:
			case 10:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_23_X_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				pusSt23RepositoryPath_t packetRepos;

				pus_tc_tm_23_X_getRepositoryPath(&packetRepos, outTc);

				int tempCount = packetRepos.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = (char)(packetRepos.arr[i]);
					*cont += 1;
				}

				asn1SccPusByteArray packetFile;

				pus_tc_tm_23_X_getFileName(&packetFile, outTc);

				tempCount = packetFile.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = (char)(packetFile.arr[i]);
					*cont += 1;
				}

				return PUS_GET_ERROR();
			}
			case 11:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_23_11_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				pusSt23RepositoryPath_t packetRepos;

				pus_tc_tm_23_X_getRepositoryPath(&packetRepos, outTc);

				int tempCount = packetRepos.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = (char)(packetRepos.arr[i]);
					*cont += 1;
				}

				pusSt23DirectoryName_t packetDirOld;

				pus_tc_23_11_getOldDirectoryName(&packetDirOld, outTc);

				tempCount = packetDirOld.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = (char)(packetDirOld.arr[i]);
					*cont += 1;
				}

				pusSt23DirectoryName_t packetDirNew;

				pus_tc_23_11_getOldDirectoryName(&packetDirNew, outTc);

				tempCount = packetDirNew.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = (char)(packetDirNew.arr[i]);
					*cont += 1;
				}

				return PUS_GET_ERROR();
			}
			case 12:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_23_12_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				pusSt23RepositoryPath_t packetRepos;

				pus_tc_tm_23_X_getRepositoryPath(&packetRepos, outTc);

				int tempCount = packetRepos.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = (char)(packetRepos.arr[i]);
					*cont += 1;
				}

				return PUS_GET_ERROR();
			}
			case 14:
			{
				if (pus_getTcDataKind(outTc) != PusTcApplicationData_st_23_14_PRESENT)
				{
					return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
				}

				pusSt23RepositoryPath_t packetReposSource;

				pus_tc_23_14_getSourceRepositoryPath(&packetReposSource, outTc);

				int tempCount = packetReposSource.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = (char)(packetReposSource.arr[i]);
					*cont += 1;
				}

				pusSt23FileName_t packetFileSource;

				pus_tc_23_14_getSourceFileName(&packetFileSource, outTc);

				tempCount = packetFileSource.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = (char)(packetFileSource.arr[i]);
					*cont += 1;
				}

				pusSt23RepositoryPath_t packetReposTarget;

				pus_tc_23_14_getSourceRepositoryPath(&packetReposTarget, outTc);

				tempCount = packetReposTarget.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = (char)(packetReposTarget.arr[i]);
					*cont += 1;
				}

				pusSt23FileName_t packetFileTarget;

				pus_tc_23_14_getSourceFileName(&packetFileTarget, outTc);

				tempCount = packetFileTarget.nCount;

				push32Bit(&tempCount, buffer, cont);

				for (int i = 0; i < tempCount; i++)
				{
					buffer[*cont] = (char)(packetFileTarget.arr[i]);
					*cont += 1;
				}

				return PUS_GET_ERROR();
			}
			case 4:
			case 8:
			case 13:
				return PUS_SET_ERROR(PUS_ERROR_NOT_TC);
			default:
				return PUS_SET_ERROR(PUS_ERROR_TC_SUBTYPE);
			}
		}
		default:
		{
			return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
		}
		}
	}
	else {
		if (pus_getTcService(outTc) != pus_ST02_deviceAccess)
		{
			return PUS_SET_ERROR(PUS_ERROR_HEADER_MISMATCH);
		}

		return PUS_SET_ERROR(PUS_ERROR_NOT_IMPLEMENTED);
	}
}

pusError_t encodeTc(pusPacket_t* outTc, char* buffer, unsigned int* cont)
{
	//	-	-	-	Encodes TC packet header into the the buffer at offset cont

	if (NULL == outTc || NULL == buffer)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (*cont != 6)
	{
		return PUS_SET_ERROR(PUS_ERROR_LIMIT);
	}

	char temp = 0;
	temp = (pus_getTcPusVersion(outTc) << 4) | (pus_getTcAckFlagAcceptance(outTc) << 3) | (pus_getTcAckFlagStart(outTc) << 2) |
			(pus_getTcAckFlagProgress(outTc) << 1) | pus_getTcAckFlagCompletion(outTc);
	buffer[*cont] = temp;

	temp = pus_getTcService(outTc);
	buffer[*cont + 1] = temp;
	

	temp = pus_getTcSubtype(outTc);
	buffer[*cont + 2] = temp;

	temp = (pus_getTcSource(outTc) >> 8);
	buffer[*cont + 3] = temp;

	temp = (pus_getTcSource(outTc) & 0xff);
	buffer[*cont + 4] = temp;
	*cont += 5;

	return encodeTcNoHeader(outTc, buffer, cont);
}