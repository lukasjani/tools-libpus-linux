#pragma once

#define PORT 1025 
#define IS_SERVER 1
//#define SERVER_IP "172.16.201.38" //zebra
#define SERVER_IP "172.16.201.192" //rpi2

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
#include "pus_parameter_management.h"
#include "pus_encoder.h"

void print_packet(pusPacket_t* outTm);

void assert_primary_header(pusPacket_t* outTm1, pusPacket_t* outTm2);

void assert_secondary_tm_header(pusPacket_t* outTm1, pusPacket_t* outTm2);

void assert_secondary_tc_header(pusPacket_t* outTm1, pusPacket_t* outTm2);

int test_tm_service_one();

int test_tc_service_one();

int test_tm_service_two();

int test_tc_service_two();

int test_tm_service_three();

int test_tc_service_three();

int test_tm_service_four();

int test_tc_service_four();

int test_tm_service_five();

int test_tc_service_five();

int test_tm_service_six();

int test_tc_service_six();

int test_tm_service_seven();

int test_tc_service_seven();

int test_tm_service_eight();

int test_tc_service_eight();

int test_tm_service_nine();

int test_tc_service_nine();

int test_tm_service_ten();

int test_tc_service_ten();

int test_tm_service_eleven();

int test_tc_service_eleven();

int test_tm_service_twelve();

int test_tc_service_twelve();

int test_tm_service_thirteen();

int test_tc_service_thirteen();

int test_tm_service_fourteen();

int test_tc_service_fourteen();

int test_tm_service_fifteen();

int test_tc_service_fifteen();

int test_tm_service_sixteen();

int test_tc_service_sixteen();

int test_tm_service_seventeen();

int test_tc_service_seventeen();

int test_tm_service_eighteen();

int test_tc_service_eighteen();

int test_tm_service_nineteen();

int test_tc_service_nineteen();

int test_tm_service_twenty();

int test_tc_service_twenty();

int test_tm_service_twentyone();

int test_tc_service_twentyone();

int test_tm_service_twentytwo();

int test_tc_service_twentytwo();

int test_tm_service_twentythree();

int test_tc_service_twentythree();

int test_server();

int test_client();