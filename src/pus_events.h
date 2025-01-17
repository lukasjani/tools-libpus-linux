/*! \file pus_events.h
 *  \brief Management of the events table used by several services
 *
 *  Defines the parameters table used by the service ST[05], ST[18] and ST[19].
 *
 *  The table contains the last N events. This allows that each of the services that
 *  have to process the event can do it in turn, provided that there are not too
 *  many events. Each service passes a counter of the last event processed when
 *  requesting the next event available. It is possible that several activations of
 *  the same event are in the table at the same time.
 *
 *  Access to the table is optionally protected by a mutex. This allows implementing
 *  the service using external synchronization (by TASTE) or not.
 *
 *  H2020 ESROCOS Project
 *  \author Company: GMV Aerospace & Defence S.A.U.
 *  \copyright Licence: GPLv2
 */

#ifndef PUS_EVENTS_H
#define PUS_EVENTS_H
#pragma once

#include "pus_types.h"
#ifdef PUS_CONFIGURE_EVENTS_TABLE


#include "pus_error.h"
#include "pus_apid.h"
#include "pus_threads.h"
#include "pus_stored_param.h"
#include "pus_packet.h"
#include <string.h>

#ifdef  __cplusplus
extern "C" {
#endif

// Severity Type enum
typedef enum
{
	PUS_ST05_SEVERITY_INFO,
	PUS_ST05_SEVERITY_LOW,
	PUS_ST05_SEVERITY_MEDIUM,
	PUS_ST05_SEVERITY_HIGH
} pusSt05SeverityType_t;


//! Type to define the data types of each parameter of an event
typedef struct
{
	pusParamType_t dataType1; //!< Datatype of param 1
	pusParamType_t dataType2; //!< Datatype of param 2
} pusSt05EventReportData_t;

//! Type to describe a event: label, severity, dataTypes (the parameter ID gives its location in the array)
typedef struct
{
	const char* label; //!< Name of the event, for debugging purposes
	pusSt05SeverityType_t defaultSeverity; //!< Severity type
	pusSt05EventReportData_t data; //!< Data type
} pusSt05EventInfo_t;

//! Type to describe the structure that the events' table save
typedef struct {
	pusSt05Event_t event; //!< Event in table
    size_t eventBufferCounter; //!< Counter of the events number in table.
} pusSt05EventInBuffer_t;


//! Pointer(index) for event buffer in
extern size_t pus_st05_eventBufferIn;

#ifdef __sparc__
	#define PUS_ST05_EVENT_BUFFER_COUNTER_LIMIT 65535
#else
	#define PUS_ST05_EVENT_BUFFER_COUNTER_LIMIT 65535
#endif


//! Counter of the events that has been inserted
extern size_t pus_st05_eventBufferCounter;


//
// Service initialization and tear down
//

//! Function to initialize the event manager used by service ST[05] and several others
/*! This function is generated from the mission database, and it should be declared
 *  in \ref pus_st05_config.h
 */
extern pusError_t pus_events_configure();

//! Initialize the data structures of the event manager
/*! Create the parameter tables from the configuration and initializes a mutex.
 *  Alternatively, can pass NULL as mutex if access protection is provided externally,
 *  for instance, by TASTE.
 *  \param[inout] mutex Mutex to be initialized; if NULL, access protection is disabled
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_events_initialize(pusMutex_t* mutex);

//! Release the resources allocated by the event manager
/*! \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_events_finalize();

//! Check if the event manager is initialized
bool pus_events_isInitialized();



bool pus_events_isInInfoList(pusSt05Event_t * event);

size_t pus_events_getLastEventCounter();

//
// Buffer control
//

//! Function to push an event into the buffer
/*! Insert an event into the buffer, preparing the pointer and counter for the next insert.
 * \param[in] event Event that is going to be inserted into the buffer
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_st05_pushBufferEvent(const pusSt05Event_t * event);


//! Function that return the next event in buffer.
/*! Look for the next event in buffer
 * \param[out] event The next event in buffer.
 * \param[inout] event Event that is going to be inserted into the buffer
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_st05_getNextBufferEvent(pusSt05Event_t *next, uint64_t *actualCounter);

//! getter pus_st05_eventBufferIn
size_t pus_st05_getEventBufferIn();

//! setter pus_st05_eventBufferIn
void pus_st05_setEventBufferIn(size_t bufferIn);

//! getter pus_st05_eventBufferIn
size_t pus_st05_getEventBufferCounter();

//! setter pus_st05_eventBufferIn
void pus_st05_setEventBufferCounter(size_t counter);

//! getter pus_st05_eventBufferLength
size_t pus_st05_getEventBufferLength();

//! getter pus_st05_eventDestination
pusApid_t pus_st05_getEventDestination() ;

//! getter pus_st05_eventInfoListLength
size_t pus_st05_getEventInfoListLength();

// Getters/Setters pusSt05Event_t
//! Getter for eventId in event structure
pusSt05EventId_t pus_events_getEventId(const pusSt05Event_t* event);

//! Setter for eventId in event structure
void pus_events_setEventId(pusSt05Event_t* event, pusSt05EventId_t eventId);

//! Getter for data in event structure
asn1SccPusSt05EventAuxData pus_events_getEventAuxData(const pusSt05Event_t* event);

//! Setter for data in event structure
void pus_events_setEventAuxData(pusSt05Event_t* event, asn1SccPusSt05EventAuxData data);

#ifdef  __cplusplus
}
#endif

#endif

#endif // PUS_EVENTS_H
