#ifndef TOOLS_LIBPUS_LINUX_H
#define TOOLS_LIBPUS_LINUX_H

#include <fstream>
#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/types.h>
#include <netdb.h> 
#include <cstdlib>
#include <cstring>
#include <poll.h>

#include "src/dataview-uniq.h"
#include "src/pus_apid.h"
#include "src/pus_enable_services.h"
#include "src/pus_encoder.h"
#include "src/pus_error.h"
#include "src/pus_event_action.h"
#include "src/pus_events.h"
#include "src/pus_file_management.h"
#include "src/pus_housekeeping.h"
#include "src/pus_notify.h"
#include "src/pus_packet.h"
#include "src/pus_packet_queues.h"
#include "src/pus_packet_reduced.h"
#include "src/pus_parameter_management.h"
#include "src/pus_parameter_monitoring.h"
#include "src/pus_st01_packets.h"
#include "src/pus_st03_packets.h"
#include "src/pus_st05_packets.h"
#include "src/pus_st08_packets.h"
#include "src/pus_st09_packets.h"
#include "src/pus_st11_packets.h"
#include "src/pus_st12_packets.h"
#include "src/pus_st13_packets.h"
#include "src/pus_st17_packets.h"
#include "src/pus_st18_packets.h"
#include "src/pus_st19_packets.h"
#include "src/pus_st20_packets.h"
#include "src/pus_st23_packets.h"
#include "src/pus_stored_param.h"
#include "src/pus_threads.h"
#include "src/pus_time.h"
#include "src/pus_timebased_scheduling.h"
#include "src/pus_types.h"
#include "src/pus_test.h"

pusError_t pus_tm_send_1_1(pusPacket_t* outTm, pusApid_t* apid, pusSequenceCount_t* seq, pusPacket_t* inTc, char* buffer, int* sockfd, int* pus_st13_transactionID);
pusError_t pus_tm_send_1_2(pusPacket_t* outTm, pusApid_t* apid, pusSequenceCount_t* seq, pusPacket_t* inTc, char* buffer, int* sockfd, int* pus_st13_transactionID,
	pusSt01FailureCode_t code, pusSt01FailureInfo_t* info);
pusError_t pus_tc_sendTCP(int socket, char* buffer, int packet_length, pusSequenceCount_t* sequenceCount, int* transactionID);
pusError_t pus_tm_sendTCP(int socket, char* buffer, int packet_length, pusSequenceCount_t* sequenceCount, int* transactionID, pusApid_t destination);
pusError_t pus_tc_recvTCP(int socket, char* buffer);
pusError_t pus_tm_recvTCP(int socket, char* buffer);

void start_server();
void start_client();

int main(int argc, const char* argv[]);

#endif