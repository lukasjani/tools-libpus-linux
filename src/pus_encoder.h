#ifndef PUS_ENCODER_H
#define PUS_ENCODER_H

#include <cstdlib>
#include <bitset>
#include <cstring>
#include <string.h>
#include "pus_error.h"
#include "pus_packet.h"
#include "dataview-uniq.h"
#include "pus_types.h"
#include "pus_st01_packets.h"
#include "pus_st03_packets.h"
#include "pus_st05_packets.h"
#include "pus_st08_packets.h"
#include "pus_st09_packets.h"
#include "pus_st11_packets.h"
#include "pus_st12_packets.h"
#include "pus_st13_packets.h"
#include "pus_st17_packets.h"
#include "pus_st18_packets.h"
#include "pus_st19_packets.h"
#include "pus_st20_packets.h"
#include "pus_st23_packets.h"

unsigned int getDataLength(char* buffer);

void push16Bit(short* var, char* buffer, unsigned int* cont);

unsigned short pop16Bit(char* buffer, unsigned int* cont);

void push32Bit(int* var, char* buffer, unsigned int* cont);

unsigned int pop32Bit(char* buffer, unsigned int* cont);

void push64UBit(asn1SccPusUInt64* var, char* buffer, unsigned int* cont);

void push64SBit(asn1SccPusInt64* var, char* buffer, unsigned int* cont);

void push64DBit(asn1SccPusDouble* var, char* buffer, unsigned int* cont);

asn1SccPusUInt64 pop64UBit(char* buffer, unsigned int* cont);

asn1SccPusInt64 pop64SBit(char* buffer, unsigned int* cont);

asn1SccPusDouble pop64DBit(char* buffer, unsigned int* cont);

pusError_t decodeReduced(char* buffer, asn1SccPusPacketReduced* outTm, unsigned int* cont);

pusError_t decodePacket(char* buffer, pusPacket_t* outTm);

pusError_t decodeTmNoHeader(char* buffer, pusPacket_t* outTm, unsigned int* cont);

pusError_t decodeTm(char* buffer, pusPacket_t* outTm, unsigned int cont);

pusError_t decodeTcNoHeader(char* buffer, pusPacket_t* outTc, unsigned int* cont);

pusError_t decodeTc(char* buffer, pusPacket_t* outTc, unsigned int cont);

pusError_t encodeReduced(asn1SccPusPacketReduced* outTm, char* buffer, unsigned int* cont);

pusError_t encodePacket(pusPacket_t* outTm, char* buffer, unsigned int* cont);

pusError_t encodeTmNoHeader(pusPacket_t* outTm, char* buffer, unsigned int* cont);

pusError_t encodeTm(pusPacket_t* outTm, char* buffer, unsigned int* cont);

pusError_t encodeTcNoHeader(pusPacket_t* outTc, char* buffer, unsigned int* cont);

pusError_t encodeTc(pusPacket_t* outTc, char* buffer, unsigned int* cont);

#endif