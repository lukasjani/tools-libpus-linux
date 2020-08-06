#ifndef GENERATED_ASN1SCC_dataview_uniq_H
#define GENERATED_ASN1SCC_dataview_uniq_H
/*
Code automatically generated by asn1scc tool
*/

#include "asn1crt.h"

#ifdef  __cplusplus
extern "C" {
#endif

#pragma once

typedef asn1SccUint asn1SccPusUInt3;
typedef asn1SccUint asn1SccPusUInt4;
typedef asn1SccPusUInt4 asn1SccPusTimeReferenceStatus;
typedef asn1SccUint asn1SccPusUInt8;
typedef asn1SccPusUInt8 asn1SccPusServiceType;
typedef asn1SccPusUInt8 asn1SccPusSubserviceType;
typedef asn1SccUint asn1SccPusUInt11;
typedef asn1SccPusUInt11 asn1SccPusApid;
typedef asn1SccUint asn1SccPusUInt14;
typedef asn1SccPusUInt14 asn1SccPusSequenceCount;
typedef asn1SccUint asn1SccPusUInt16;
typedef asn1SccPusUInt16 asn1SccPusSt18ObcpChecksum;
typedef asn1SccPusUInt16 asn1SccPusPacketDataLength;
typedef asn1SccPusUInt16 asn1SccPusMessageTypeCounter;
typedef asn1SccPusUInt16 asn1SccPusChecksum;
typedef asn1SccUint asn1SccPusUInt32;
typedef asn1SccPusUInt32 asn1SccPusStepId;
typedef asn1SccPusUInt32 asn1SccPusSt20OnBoardParameterId;
typedef asn1SccPusUInt32 asn1SccPusSt05EventId;
typedef asn1SccPusUInt32 asn1SccPusSt09ExponentialRate;
typedef asn1SccPusUInt32 asn1SccPusSt03HousekeepingReportId;
typedef asn1SccPusUInt32 asn1SccPusSt03ParameterId;
typedef asn1SccPusUInt32 asn1SccPusSt08FunctionId;
typedef asn1SccPusUInt32 asn1SccPusSt01FailureCode;
typedef asn1SccPusUInt32 asn1SccPusConfigDummyType;
typedef asn1SccUint asn1SccPusUInt64;
typedef asn1SccPusUInt64 asn1SccPusMemAddr;
typedef asn1SccPusUInt64 asn1SccPusSt20ParamPTC;
typedef asn1SccPusUInt64 asn1SccPusSt20ParamPFC;
typedef asn1SccSint asn1SccPusInt32;
typedef asn1SccSint asn1SccPusInt64;
typedef double asn1SccPusDouble;
typedef asn1SccPusDouble asn1SccPusSt20StoredParam;
typedef asn1SccPusDouble asn1SccPusSt03StoredParam;
typedef flag asn1SccPusSecondaryHeaderFlag;
typedef asn1SccSint asn1SccT_Int32;
typedef asn1SccUint asn1SccT_UInt32;
typedef asn1SccSint asn1SccT_Int8;
typedef asn1SccUint asn1SccT_UInt8;
typedef flag asn1SccT_Boolean;


typedef struct {
    int nCount;
    byte arr[256];
} asn1SccPusByteArray;

typedef struct {
    int nCount;
    asn1SccPusSt05EventId eventId[20];
} asn1SccPusTC_19_X_Data;

typedef struct {
    asn1SccPusSt09ExponentialRate exponentialRate;
} asn1SccPusTC_9_1_Data;

typedef asn1SccPusUInt32 asn1SccPusSt12pmonId;

typedef struct {
    int nCount;
    asn1SccPusSt12pmonId pmonId[20];
} asn1SccPusTC_12_1_2_Data;


typedef struct {
    asn1SccPusSt08FunctionId functionId;
} asn1SccPusTC_8_1_Data;


typedef struct {
    asn1SccPusSt20OnBoardParameterId paramId;
    asn1SccPusSt20StoredParam value;
} asn1SccPusSt20ParamValue;

typedef struct {
    asn1SccPusSt20OnBoardParameterId paramId;
} asn1SccPusSt20Param;

typedef struct {
    int nCount;
    asn1SccPusSt20OnBoardParameterId arr[20];
    asn1SccPusSt20StoredParam value[20];
} asn1SccPusTC_TM_20_2_3_Data;

typedef struct {
    int nCount;
    asn1SccPusSt20OnBoardParameterId arr[20];
} asn1SccPusTC_20_1_Data;

typedef struct {
    asn1SccPusUInt64 data1;
    asn1SccPusUInt64 data2;
} asn1SccPusSt05EventAuxData;

typedef struct {
    asn1SccPusSt05EventId eventId;
    asn1SccPusSt05EventAuxData data;
} asn1SccPusSt05Event;

typedef asn1SccPusSt05Event asn1SccPusTM_5_X_Data;

typedef struct {
    int nCount;
    asn1SccPusSt05Event arr[20];
} asn1SccPusTC_TM_5_X_Data;

typedef struct {    
    int nCount; 
    asn1SccPusSt03StoredParam arr[20];
} asn1SccPusSt03ParameterArray;

typedef struct {
    asn1SccPusSt03HousekeepingReportId reportId;
    asn1SccPusSt03ParameterArray parameters;
} asn1SccPusTM_3_X_Data;

typedef struct {
    int nCount;
    asn1SccPusSt20OnBoardParameterId arr[20];
} asn1SccPusTC_3_X_Data;

typedef struct {
    asn1SccPusInt32 subcode;
    asn1SccPusInt32 data;
    asn1SccPusMemAddr address;
} asn1SccPusSt01FailureInfo;

typedef struct {
    asn1SccPusSt01FailureCode code;
    asn1SccPusSt01FailureInfo info;
} asn1SccPusSt01Failure;

typedef struct {
    asn1SccPusInt64 tv_sec;
    asn1SccPusInt64 tv_nsec;
} asn1SccPusTime;

typedef struct {
    asn1SccPusSt09ExponentialRate exponentialRate;
    asn1SccPusTime time;
} asn1SccPusTM_9_2_3_Data;

typedef struct {
    byte arr[10];
} asn1SccPusSt18ObcpId;

typedef struct {
    asn1SccPusSt18ObcpId obcpId;
} asn1SccPusTC_18_2_6_12_Data;

typedef struct {
    asn1SccPusSt18ObcpId obcpId;
    asn1SccPusUInt32 stepId;
} asn1SccPusTC_18_4_5_Data;

typedef struct {
    int nCount; 
    byte arr[15000];
} asn1SccPusSt18ObcpCode;

typedef struct {
    asn1SccPusSt18ObcpId obcpId;
    asn1SccPusSt18ObcpCode obcpCode;
    asn1SccPusSt18ObcpChecksum checksum;
} asn1SccPusTC_18_1_Data;

typedef enum {
    asn1Sccnone = 0,
    asn1Sccprocedure = 1,
    asn1Sccstep = 2,
    asn1Sccdetailed = 3
} asn1SccPusSt18ObservabilityLevel;

typedef struct {
    asn1SccPusSt18ObcpId obcpId;
    asn1SccPusSt18ObservabilityLevel observabilityLevel;
} asn1SccPusTC_18_3_Data;

typedef enum {
    asn1Sccpus_CCSDS_V1 = 0
} asn1SccPusPacketVersion;

typedef enum {
    asn1Sccpus_TM = 0,
    asn1Sccpus_TC = 1
} asn1SccPusPacketType;

typedef enum {
    asn1Sccpus_STANDALONE_PACKET = 3
} asn1SccPusSequenceFlags;

typedef struct {
    asn1SccPusPacketVersion packetVersion;
    asn1SccPusPacketType packetType;
    asn1SccPusSecondaryHeaderFlag secondaryHeaderFlag;
    asn1SccPusApid apid;
    asn1SccPusSequenceFlags sequenceFlags;
    asn1SccPusSequenceCount sequenceCount;
} asn1SccPusSt01RequestId;

typedef struct {
    asn1SccPusSt01RequestId request;
    asn1SccPusStepId step;
    asn1SccPusSt01Failure failure;
} asn1SccPusTM_1_X_Data;

typedef enum {
    asn1Sccpus_V0 = 0,
    asn1Sccpus_V1 = 1,
    asn1Sccpus_V2 = 2,
    asn1Sccpus_INVALID_VERSION = 15
} asn1SccPusVersionNumber;

typedef struct {
    asn1SccPusVersionNumber pusVersion;
    asn1SccPusTimeReferenceStatus timeRefStatus;
    asn1SccPusServiceType serviceId;
    asn1SccPusSubserviceType subtypeId;
    asn1SccPusMessageTypeCounter msgTypeCount;
    asn1SccPusApid destination;
    asn1SccPusTime time;
} asn1SccPusTmHeader;

typedef struct {
    flag acceptanceReportRequested;
    flag startReportRequested;
    flag progressReportRequested;
    flag completionReportRequested;
} asn1SccPusAcknowledgementFlags;

typedef struct {
    asn1SccPusVersionNumber pusVersion;
    asn1SccPusAcknowledgementFlags ackFlags;
    asn1SccPusServiceType serviceId;
    asn1SccPusSubserviceType subtypeId;
    asn1SccPusApid source;
} asn1SccPusTcHeader;

typedef struct {
    asn1SccPusSt18ObcpId obcpId;
    asn1SccPusByteArray repository;
    asn1SccPusByteArray fileName;
} asn1SccPusTC_18_13_Data;

typedef struct {
    asn1SccPusByteArray repository;
    asn1SccPusByteArray fileName;
    asn1SccPusUInt64 maxSize;
    asn1SccT_Boolean locked;
} asn1SccPusTC_TM_23_1_4_Data;

typedef struct {
    asn1SccPusByteArray repository;
    asn1SccPusByteArray searchPattern;
    int matches;
    asn1SccPusByteArray filePaths[30];
} asn1SccPusTM_23_8_Data;

typedef enum {
    st_23_13_directory = 'D',
    st_23_13_file = 'F'
} asn1SccPusSt23ObjectType;

typedef struct {
    asn1SccPusByteArray repository;
    int numberObjects;
    asn1SccPusSt23ObjectType objectTypes[30];
    asn1SccPusByteArray objectNames[30];
} asn1SccPusTM_23_13_Data;

typedef struct {
    asn1SccPusUInt64 transactionID;
    asn1SccPusUInt64 part_seq_number;
    char* content;
} asn1SccPusTC_TM_13_X_Data;

typedef struct {
    asn1SccPusUInt64 transactionID;
    asn1SccPusUInt64 failure_reason;
} asn1SccPusTM_13_16_Data;

typedef enum {
    PusTmSourceData_NONE,
    st_1_X_PRESENT,
    st_3_X_PRESENT,
    st_5_X_PRESENT,
    st_5_8_PRESENT,
    st_9_2_PRESENT,
    st_13_X_PRESENT,
    st_13_16_PRESENT,
    st_20_2_PRESENT,
    st_23_4_PRESENT,
    st_23_8_PRESENT,
    st_23_13_PRESENT
} pusTmDataKind;

typedef pusTmDataKind pusTmDataKind_t;

typedef struct {
    pusTmDataKind_t kind;
    union {
        asn1SccUint32 st_0_0;
        asn1SccPusTM_1_X_Data st_1_X;
        asn1SccPusTM_3_X_Data st_3_X;
        asn1SccPusTM_5_X_Data st_5_X;
        asn1SccPusTC_TM_5_X_Data st_5_8;
        asn1SccPusTM_9_2_3_Data st_9_2_3;
        asn1SccPusTC_TM_13_X_Data st_13_X;
        asn1SccPusTM_13_16_Data st_13_16;
        asn1SccPusTC_TM_20_2_3_Data st_20_2_3;
        asn1SccPusTC_TM_23_1_4_Data st_23_4;
        asn1SccPusTM_23_8_Data st_23_8;
        asn1SccPusTM_23_13_Data st_23_13;
    } u; 
} asn1SccPusTmSourceData;

typedef struct {
    asn1SccPusTmHeader header;
    asn1SccPusTmSourceData data;
} asn1SccPusTmHeaderData;

typedef struct {
    asn1SccPusByteArray repository;
    asn1SccPusByteArray name;
} asn1SccPusTC_23_X_Data;

typedef struct {
    asn1SccPusByteArray repository;
    asn1SccPusByteArray directory_old;
    asn1SccPusByteArray directory_new;
} asn1SccPusTC_23_11_Data;

typedef struct {
    asn1SccPusByteArray repository;
} asn1SccPusTC_23_12_Data;

typedef struct {
    asn1SccPusByteArray sourceRepository;
    asn1SccPusByteArray sourceFileName;
    asn1SccPusByteArray targetRepository;
    asn1SccPusByteArray targetFileName;
} asn1SccPusTC_23_14_Data;


typedef enum {
    PusTcApplicationDataReduced_NONE,
    PusTcApplicationDataReduced_st_8_1_PRESENT,
    PusTcApplicationDataReduced_st_9_1_PRESENT,
    PusTcApplicationDataReduced_st_12_1_2_PRESENT,
    PusTcApplicationDataReduced_st_13_X_PRESENT,
    PusTcApplicationDataReduced_st_18_2_6_12_PRESENT,
    PusTcApplicationDataReduced_st_18_3_PRESENT,
    PusTcApplicationDataReduced_st_18_4_5_PRESENT,
    PusTcApplicationDataReduced_st_18_13_PRESENT,
    PusTcApplicationDataReduced_st_19_X_PRESENT,
    PusTcApplicationDataReduced_st_20_X_PRESENT,
    PusTcApplicationDataReduced_st_23_X_PRESENT,
    PusTcApplicationDataReduced_st_23_1_PRESENT,
    PusTcApplicationDataReduced_st_23_14_PRESENT
} pusTcReducedDataKind;

typedef pusTcReducedDataKind pusTcReducedDataKind_t;

typedef struct {
    pusTcReducedDataKind_t kind;
    union {
        asn1SccUint32 st_0_0;
        asn1SccPusTC_TM_5_X_Data st_5_X;
        asn1SccPusTC_8_1_Data st_8_1;
        asn1SccPusTC_9_1_Data st_9_1;
        asn1SccPusTC_12_1_2_Data st_12_1_2;
        asn1SccPusTC_TM_13_X_Data st_13_X;
        asn1SccPusTC_18_1_Data st_18_1;
        asn1SccPusTC_18_2_6_12_Data st_18_2_6_12;
        asn1SccPusTC_18_3_Data st_18_3;
        asn1SccPusTC_18_4_5_Data st_18_4_5;
        asn1SccPusTC_18_13_Data st_18_13;
        asn1SccPusTC_19_X_Data st_19_X;
        asn1SccPusTC_20_1_Data st_20_1;
        asn1SccPusTC_TM_20_2_3_Data st_20_2_3;
        asn1SccPusTC_23_X_Data st_23_X;
        asn1SccPusTC_TM_23_1_4_Data st_23_1;
        asn1SccPusTC_23_11_Data st_23_11;
        asn1SccPusTC_23_12_Data st_23_12;
        asn1SccPusTC_23_14_Data st_23_14;
    } u; 
} asn1SccPusTcApplicationDataReduced;

typedef struct {
    asn1SccPusTcHeader header;
    asn1SccPusTcApplicationDataReduced data;
} asn1SccPusPacketDataReduced_tcData;

typedef struct {
    enum {
        PusPacketDataReduced_NONE,
        PusPacketDataReduced_tmData_PRESENT,
        PusPacketDataReduced_tmDataNoHeader_PRESENT,
        PusPacketDataReduced_tcData_PRESENT,
        PusPacketDataReduced_tcDataNoHeader_PRESENT 
    } kind;
    union {
        asn1SccPusTmHeaderData tmData;
        asn1SccPusTmSourceData tmDataNoHeader;
        asn1SccPusPacketDataReduced_tcData tcData;
        asn1SccPusTcApplicationDataReduced tcDataNoHeader;
    } u; 
} asn1SccPusPacketDataReduced;

typedef struct {
    asn1SccPusPacketVersion packetVersion;
    asn1SccPusPacketType packetType;
    asn1SccPusSecondaryHeaderFlag secondaryHeaderFlag;
    asn1SccPusApid apid;
    asn1SccPusSequenceFlags sequenceFlags;
    asn1SccPusSequenceCount sequenceCount;
    asn1SccPusPacketDataLength dataLength;
    asn1SccPusPacketDataReduced data;
} asn1SccPusPacketReduced;

typedef struct {
    int nCount;
    asn1SccPusSt05EventId eventId[20];
    asn1SccPusPacketReduced packetReduced[20];
} asn1SccPusTC_19_1_Data;

typedef struct {
    asn1SccPusTime time;
    asn1SccPusPacketReduced packetReduced;
} asn1SccPusTcScheduledActivity;

typedef struct {
    int nCount;
    asn1SccPusTcScheduledActivity arr[5];
} asn1SccPusTC_11_4_Data;

typedef enum {
    PusTcApplicationData_NONE,
    PusTcApplicationData_st_3_X_PRESENT,
    PusTcApplicationData_st_5_X_PRESENT,
    PusTcApplicationData_st_8_1_PRESENT,
    PusTcApplicationData_st_9_1_PRESENT,
    PusTcApplicationData_st_11_4_PRESENT,
    PusTcApplicationData_st_12_1_2_PRESENT,
    PusTcApplicationData_st_13_X_PRESENT,
    PusTcApplicationData_st_18_1_PRESENT,
    PusTcApplicationData_st_18_2_6_12_PRESENT,
    PusTcApplicationData_st_18_3_PRESENT,
    PusTcApplicationData_st_18_4_5_PRESENT,
    PusTcApplicationData_st_18_13_PRESENT,
    PusTcApplicationData_st_19_1_PRESENT,
    PusTcApplicationData_st_19_X_PRESENT,
    PusTcApplicationData_st_20_1_PRESENT,
    PusTcApplicationData_st_20_3_PRESENT,
    PusTcApplicationData_st_23_X_PRESENT,
    PusTcApplicationData_st_23_1_PRESENT,
    PusTcApplicationData_st_23_11_PRESENT,
    PusTcApplicationData_st_23_12_PRESENT,
    PusTcApplicationData_st_23_14_PRESENT
} pusTcDataKind;

typedef pusTcDataKind pusTcDataKind_t;

typedef struct {
    pusTcDataKind_t kind;
    union {
        asn1SccUint32 st_0_0;
        asn1SccPusTC_3_X_Data st_3_X;
        asn1SccPusTC_TM_5_X_Data st_5_X;
        asn1SccPusTC_8_1_Data st_8_1;
        asn1SccPusTC_9_1_Data st_9_1;
        asn1SccPusTC_11_4_Data st_11_4;
        asn1SccPusTC_12_1_2_Data st_12_1_2;
        asn1SccPusTC_TM_13_X_Data st_13_X;
        asn1SccPusTM_13_16_Data st_13_16;
        asn1SccPusTC_18_1_Data st_18_1;
        asn1SccPusTC_18_2_6_12_Data st_18_2_6_12;
        asn1SccPusTC_18_3_Data st_18_3;
        asn1SccPusTC_18_4_5_Data st_18_4_5;
        asn1SccPusTC_18_13_Data st_18_13;
        asn1SccPusTC_19_1_Data st_19_1;
        asn1SccPusTC_19_X_Data st_19_X;
        asn1SccPusTC_20_1_Data st_20_1;
        asn1SccPusTC_TM_20_2_3_Data st_20_2_3;
        asn1SccPusTC_23_X_Data st_23_X;
        asn1SccPusTC_TM_23_1_4_Data st_23_1;
        asn1SccPusTC_23_11_Data st_23_11;
        asn1SccPusTC_23_12_Data st_23_12;
        asn1SccPusTC_23_14_Data st_23_14;
    } u; 
} asn1SccPusTcApplicationData;

typedef struct {
    asn1SccPusTcHeader header;
    asn1SccPusTcApplicationData data;
} asn1SccPusPacketData_tcData;


typedef enum {
    PusPacketData_NONE,
    PusPacketData_tmData_PRESENT,
    PusPacketData_tmDataNoHeader_PRESENT,
    PusPacketData_tcData_PRESENT,
    PusPacketData_tcDataNoHeader_PRESENT
} pusPacketDataKind;

typedef pusPacketDataKind pusPacketDataKind_t;

typedef struct {
    pusPacketDataKind_t kind;
    union {
        asn1SccPusTmHeaderData tmData;
        asn1SccPusTmSourceData tmDataNoHeader;
        asn1SccPusPacketData_tcData tcData;
        asn1SccPusTcApplicationData tcDataNoHeader;
    } u; 
} asn1SccPusPacketData;

typedef struct {
    asn1SccPusPacketVersion packetVersion;
    asn1SccPusPacketType packetType;
    asn1SccPusSecondaryHeaderFlag secondaryHeaderFlag;
    asn1SccPusApid apid;
    asn1SccPusSequenceFlags sequenceFlags;
    asn1SccPusSequenceCount sequenceCount;
    asn1SccPusPacketDataLength dataLength;
    asn1SccPusPacketData data;
} asn1SccPusPacket;

extern const asn1SccPusPacketVersion pus_PACKET_VERSION_CURRENT;
extern const asn1SccPusApid pus_APID_TIME;
extern const asn1SccPusApid pus_APID_IDLE;
extern const asn1SccPusVersionNumber pus_VERSION_CURRENT;
extern const asn1SccPusTimeReferenceStatus pus_TIME_REFERENCE_STATUS_NONE;
extern const asn1SccPusMessageTypeCounter pus_MESSAGE_TYPE_COUNTER_NONE;
extern const asn1SccPusStepId pus_STEP_NONE;
extern const asn1SccPusSt05EventId pus_EVENT_ID_NONE;
extern const asn1SccPusSt05EventAuxData pus_EVENT_AUXDATA_NONE;
extern const asn1SccPusUInt64 pus_EVENT_DATASTORED_NONE;

extern const asn1SccPusServiceType pusService_NONE;
extern const asn1SccPusServiceType pus_ST01_requestVerification;
extern const asn1SccPusServiceType pus_ST02_deviceAccess;
extern const asn1SccPusServiceType pus_ST03_housekeeping;
extern const asn1SccPusServiceType pus_ST04_parameterStatisticsReporting;
extern const asn1SccPusServiceType pus_ST05_eventReporting;
extern const asn1SccPusServiceType pus_ST06_memoryManagement;
extern const asn1SccPusServiceType pus_ST08_functionManagement;
extern const asn1SccPusServiceType pus_ST09_timeManagement;
extern const asn1SccPusServiceType pus_ST11_timeBasedScheduling;
extern const asn1SccPusServiceType pus_ST12_onBoardMonitoring;
extern const asn1SccPusServiceType pus_ST13_largePacketTransfer;
extern const asn1SccPusServiceType pus_ST14_realTimeForwardingControl;
extern const asn1SccPusServiceType pus_ST15_onBoardStorageAndRetrieval;
extern const asn1SccPusServiceType pus_ST17_test;
extern const asn1SccPusServiceType pus_ST18_onBoardControlProcedure;
extern const asn1SccPusServiceType pus_ST19_eventAction;
extern const asn1SccPusServiceType pus_ST20_parameterManagement;
extern const asn1SccPusServiceType pus_ST21_requestSequencing;
extern const asn1SccPusServiceType pus_ST22_partitionBasedScheduling;
extern const asn1SccPusServiceType pus_ST23_fileManagement;

extern const asn1SccPusSubserviceType pusSubtype_NONE;
extern const asn1SccPusSubserviceType pus_TM_1_1_successfulAcceptance;
extern const asn1SccPusSubserviceType pus_TM_1_2_failedAcceptance;
extern const asn1SccPusSubserviceType pus_TM_1_3_successfulStart;
extern const asn1SccPusSubserviceType pus_TM_1_4_failedStart;
extern const asn1SccPusSubserviceType pus_TM_1_5_successfulProgress;
extern const asn1SccPusSubserviceType pus_TM_1_6_failedProgress;
extern const asn1SccPusSubserviceType pus_TM_1_7_successfulCompletion;
extern const asn1SccPusSubserviceType pus_TM_1_8_failedCompletion;
extern const asn1SccPusSubserviceType pus_TM_1_10_failedRouting;
extern const asn1SccPusSubserviceType pus_TC_3_5_enableHousekeeping;
extern const asn1SccPusSubserviceType pus_TC_3_6_disableHousekeeping;
extern const asn1SccPusSubserviceType pus_TC_3_7_enableDiagnostic;
extern const asn1SccPusSubserviceType pus_TC_3_8_disableDiagnostic;
extern const asn1SccPusSubserviceType pus_TM_3_25_housekeepingReport;
extern const asn1SccPusSubserviceType pus_TM_3_26_diagnosticReport;
extern const asn1SccPusSubserviceType pus_TM_5_1_eventInformative;
extern const asn1SccPusSubserviceType pus_TM_5_2_anomalyLowSeverity;
extern const asn1SccPusSubserviceType pus_TM_5_3_anomalyMediumSeverity;
extern const asn1SccPusSubserviceType pus_TM_5_4_anomalyHighSeverity;
extern const asn1SccPusSubserviceType pus_TC_5_5_enableEventReport;
extern const asn1SccPusSubserviceType pus_TC_5_6_disableEventReport;
extern const asn1SccPusSubserviceType pus_TC_5_7_reportDisabledEvents;
extern const asn1SccPusSubserviceType pus_TM_5_8_disabledEventsReport;
extern const asn1SccPusSubserviceType pus_TC_8_1_performFunction;
extern const asn1SccPusSubserviceType pus_TC_9_1_setTimeReportRate;
extern const asn1SccPusSubserviceType pus_TM_9_2_cucTimeReport;
extern const asn1SccPusSubserviceType pus_TC_11_1_enableTimeBasedScheduling;
extern const asn1SccPusSubserviceType pus_TC_11_2_disableTimeBasedScheduling;
extern const asn1SccPusSubserviceType pus_TC_11_3_resetTimeBasedSchedule;
extern const asn1SccPusSubserviceType pus_TC_11_4_insertActivity;
extern const asn1SccPusSubserviceType pus_TC_12_1_enableParameterMonitoringDefinitions;
extern const asn1SccPusSubserviceType pus_TC_12_2_disableParameterMonitoringDefinitions;
extern const asn1SccPusSubserviceType pus_TC_12_15_enableParameterMonitoring;
extern const asn1SccPusSubserviceType pus_TC_12_16_disableParameterMonitoring;
extern const asn1SccPusSubserviceType pus_TM_13_1_createFirstDownlinkPartReport;
extern const asn1SccPusSubserviceType pus_TM_13_2_createIntermediateDownlinkPartReport;
extern const asn1SccPusSubserviceType pus_TM_13_3_createLastDownlinkPartReport;
extern const asn1SccPusSubserviceType pus_TC_13_9_createFirstUplinkPartReport;
extern const asn1SccPusSubserviceType pus_TC_13_10_createIntermediateUplinkPartReport;
extern const asn1SccPusSubserviceType pus_TC_13_11_createLastUplinkPartReport;
extern const asn1SccPusSubserviceType pus_TM_13_16_createLargePacketUplinkAbortReport;
extern const asn1SccPusSubserviceType pus_TC_17_1_connectionTest;
extern const asn1SccPusSubserviceType pus_TM_17_2_connectionTest;
extern const asn1SccPusSubserviceType pus_TC_18_1_loadObcpDirect;
extern const asn1SccPusSubserviceType pus_TC_18_2_unloadObcp;
extern const asn1SccPusSubserviceType pus_TC_18_3_activateObcp;
extern const asn1SccPusSubserviceType pus_TC_18_4_stopObcp;
extern const asn1SccPusSubserviceType pus_TC_18_5_suspendObcp;
extern const asn1SccPusSubserviceType pus_TC_18_6_resumeObcp;
extern const asn1SccPusSubserviceType pus_TC_18_12_abortObcp;
extern const asn1SccPusSubserviceType pus_TC_18_13_loadObcpByReference;
extern const asn1SccPusSubserviceType pus_TC_18_21_startObcpEngine;
extern const asn1SccPusSubserviceType pus_TC_18_22_stopObcpEngine;
extern const asn1SccPusSubserviceType pus_TC_19_1_addEventActionDefinitions;
extern const asn1SccPusSubserviceType pus_TC_19_2_deleteEventActionDefinitions;
extern const asn1SccPusSubserviceType pus_TC_19_4_enableEventActionDefinitions;
extern const asn1SccPusSubserviceType pus_TC_19_5_disableEventActionDefinitions;
extern const asn1SccPusSubserviceType pus_TC_20_1_reportParameterValues;
extern const asn1SccPusSubserviceType pus_TM_20_2_reportParameterValues;
extern const asn1SccPusSubserviceType pus_TC_20_3_setParameterValues;
extern const asn1SccPusSubserviceType pus_TC_23_1_createFile;
extern const asn1SccPusSubserviceType pus_TC_23_2_deleteFile;
extern const asn1SccPusSubserviceType pus_TC_23_3_reportFileAttributes;
extern const asn1SccPusSubserviceType pus_TM_23_4_reportFileAttributes;
extern const asn1SccPusSubserviceType pus_TC_23_5_lockFile;
extern const asn1SccPusSubserviceType pus_TC_23_6_unlockFile;
extern const asn1SccPusSubserviceType pus_TC_23_7_requestFindFile;
extern const asn1SccPusSubserviceType pus_TM_23_8_reportFoundFile;
extern const asn1SccPusSubserviceType pus_TC_23_9_createDirectory;
extern const asn1SccPusSubserviceType pus_TC_23_10_deleteDirectory;
extern const asn1SccPusSubserviceType pus_TC_23_11_renameDirectory;
extern const asn1SccPusSubserviceType pus_TC_23_12_requestSummaryRepository;
extern const asn1SccPusSubserviceType pus_TM_23_13_reportSummaryRepository;
extern const asn1SccPusSubserviceType pus_TC_23_14_copyFile;

extern const asn1SccPusSt01FailureCode pus_ST01_NO_ERROR;
extern const asn1SccPusSt01FailureCode pus_ST01_ERROR_APID_UNAVAILABLE;
extern const asn1SccPusSt01FailureCode pus_ST01_ERROR_SERVICE_UNAVAILABLE;
extern const asn1SccPusSt01FailureCode pus_ST01_ERROR_SUBTYPE_UNAVAILABLE;
extern const asn1SccPusSt01FailureCode pus_ST01_ERROR_APID_UNKNOWN;
extern const asn1SccPusSt01FailureCode pus_ST01_ERROR_SERVICE_UNKNOWN;
extern const asn1SccPusSt01FailureCode pus_ST01_ERROR_SUBTYPE_UNKNOWN;
extern const asn1SccPusSt01FailureCode pus_ST01_ERROR_WRONG_FORMAT;
extern const asn1SccPusSt01FailureCode pus_ST01_ERROR_CHECKSUM;

extern const asn1SccPusUInt32 pus_ST03_MAX_REPORT_LENGTH;
extern const asn1SccPusUInt32 pus_ST11_MAX_SCHEDULED_ACTIVITIES;
extern const asn1SccPusUInt32 pus_ST13_MAX_PART_SIZE;
extern const asn1SccPusUInt32 pus_ST18_SIZE_OBCP_ID;
extern const asn1SccPusUInt32 pus_ST18_MAX_SIZE_OBCP_CODE;
extern const asn1SccPusUInt32 pus_ST18_SIZE_OBCP_CHECKSUM;
extern const asn1SccPusUInt32 pus_ST23_MAX_SIZE_FILE_PATH;
extern const asn1SccPusUInt32 pus_ST23_MAX_SIZE_REPOSITORY_PATH;
extern const asn1SccPusUInt32 pus_ST23_MAX_SIZE_REPOSITORY_DOMAIN;
extern const asn1SccPusUInt32 pus_ST23_MAX_SIZE_FILE; 

/* ================= Encoding/Decoding function prototypes =================
 * These functions are placed at the end of the file to make sure all types
 * have been declared first, in case of parameterized ACN encodings
 * ========================================================================= */

 


#ifdef  __cplusplus
}

#endif

#endif