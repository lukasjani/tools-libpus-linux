/*! \file pus_st23_packets.h
 *  \brief PUS service ST[23] File management.
 *
 *  This module contains the functions to manage the file management
 *
 *  Only a subset of the standard requests and reports are implemented.
 *  * File handling
 *   - TC[23,1] Create file
 *   - TC[23,2] Delete file
 *   - TC[23,3] Request file attributes
 *   - TM[23,4] Report file attributes
 *   - TC[23,5] Lock file
 *   - TC[23,6] Unlock file
 *   - TC[23,7] Find files
 *   - TM[23,8] Found files
 *   - TC[23,9] Create directory
 *   - TC[23,10] Delete directory
 *   - TC[23,11] Rename directory
 *   - TC[23,12] Request repository summary
 *   - TM[23,13] Report repository summary
 *   - TC[23,14] Copy file
 *
 *  H2020 ESROCOS Project
 *  \author Company: GMV Aerospace & Defence S.A.U.
 *  \copyright Licence: GPLv2
 */

#ifndef PUS_ST23_PACKETS_H
#define PUS_ST23_PACKETS_H

#include "pus_types.h"
#ifdef PUS_CONFIGURE_ST23_ENABLED

#include "pus_apid.h"
#include "pus_error.h"
#include "pus_threads.h"
#include "pus_packet.h"
#include <string.h>


#ifdef  __cplusplus
extern "C" {
#endif


/*! Builds a default TC[23,X] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \param[in] subtype Subtype of the service
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_23_X_createDefaultRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSubservice_t subtype);

/*! Builds a TC[23,1] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \param[in] repository Path of the repository
 *  \param[in] fileName Name of the file
 *  \param[in] maxSize Maximum size of the file
 *  \param[in] lock Lock status of the file
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_23_1_createCreateFileRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
    const pusSt23RepositoryPath_t* repository, const pusSt23FileName_t* fileName, pusSt23MaximumSize_t maxSize, bool lock);

/*! Builds a TC[23,2] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \param[in] repository Path of the repository
 *  \param[in] fileName Name of the file
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_23_2_createDeleteFileRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
		const pusSt23RepositoryPath_t* repository, const pusSt23FileName_t* fileName);

/*! Builds a TC[23,3] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] repository Path of the repository
 *  \param[in] fileName Name of the file
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_23_3_createFileAtributesRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
		const pusSt23RepositoryPath_t* repository, const pusSt23FileName_t* fileName);

/*! Builds a TM[23_4] packet in the packet passed as parameter.
 *  \param[out] outTm Packet variable to build the TM
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \param[in] destination Destination of the TM report
 *  \param[in] repository Path of the repository
 *  \param[in] fileName Name of the file
 *  \param[in] maxSize Maximum size of the file
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tm_23_4_createFileAtributesReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusApid_t destination,
		const pusSt23RepositoryPath_t* repository, const pusSt23FileName_t* fileName, pusSt23MaximumSize_t maxSize);

/*! Builds a TC[23,5] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \param[in] repository Path of the repository
 *  \param[in] fileName Name of the file
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_23_5_createLockFileRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
    const pusSt23RepositoryPath_t* repository, const pusSt23FileName_t* fileName);

/*! Builds a TC[23,6] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \param[in] repository Path of the repository
 *  \param[in] fileName Name of the file
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_23_6_createUnlockFileRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
    const pusSt23RepositoryPath_t* repository, const pusSt23FileName_t* fileName);

/*! Builds a TC[23,7] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \param[in] repository Path of the repository
 *  \param[in] fileName Name of the file
 *  \param[in] searchPattern String to compare to file names
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_23_7_createFindFileRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
    const pusSt23RepositoryPath_t* repository, const pusSt23FileName_t* fileName, const asn1SccPusByteArray* searchPattern);

/*! Builds a TM[23_8] packet in the packet passed as parameter.
 *  \param[out] outTm Packet variable to build the TM
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \param[in] destination Destination of the TM report
 *  \param[in] repository Path of the repository
 *  \param[in] searchPattern String to compare to file names
 *  \param[in] matches Number of files found
 *  \param[in] filePaths Path to each found file
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tm_23_8_createFoundFileReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusApid_t destination,
    const pusSt23RepositoryPath_t* repository, const asn1SccPusByteArray* searchPattern, int matches, const asn1SccPusByteArray* filePaths);

/*! Builds a TC[23,9] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \param[in] repository Path of the repository
 *  \param[in] directory Directory name
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_23_9_createCreateDirectoryRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
    const pusSt23RepositoryPath_t* repository, const pusSt23DirectoryName_t* directory);

/*! Builds a TC[23,10] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \param[in] repository Path of the repository
 *  \param[in] directory Directory name
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_23_10_createDeleteDirectoryRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
    const pusSt23RepositoryPath_t* repository, const pusSt23DirectoryName_t* directory);

/*! Builds a TC[23,11] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \param[in] repository Path of the repository
 *  \param[in] directory_old Old directory name
 *  \param[in] directory_new New directory name
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_23_11_createRenameDirectoryRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
    const pusSt23RepositoryPath_t* repository, const pusSt23DirectoryName_t* directory_old, const pusSt23DirectoryName_t* directory_new);

/*! Builds a TC[23,12] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \param[in] repository Path of the repository
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_23_12_createSummaryRepositoryRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
    const pusSt23RepositoryPath_t* repository);

/*! Builds a TM[23_13] packet in the packet passed as parameter.
 *  \param[out] outTm Packet variable to build the TM
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \param[in] destination Destination of the TM report
 *  \param[in] repository Path of the repository
 *  \param[in] numberObjects Number of objects in the repository
 *  \param[in] objectTypes Types of the objects in the repository
 *  \param[in] objectNames Names of the objects in the repository
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tm_23_13_createSummaryRepositoryReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusApid_t destination,
    const pusSt23RepositoryPath_t* repository, const int numberObjects, const asn1SccPusSt23ObjectType* objectTypes, const asn1SccPusByteArray* objectNames);

/*! Builds a TC[23,14] packet in the packet passed as parameter.
 *  \param[out] outTc Packet variable to build the TC
 *  \param[in] apid APID of the process sending the report
 *  \param[in] sequenceCountcount TC sequence counter for the source APID
 *  \param[in] sourceRepository Path of the repository of the source file
 *  \param[in] sourceFileName Name of the source file
 *  \param[in] targetRepository Path of the destination repository
 *  \param[in] targetFileName Name of the destination file
 *  \return Error code (PUS_NO_ERROR if success)
 */
pusError_t pus_tc_23_14_createCopyFileRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
		const pusSt23RepositoryPath_t* sourceRepository, const pusSt23FileName_t* sourceFileName,
		const pusSt23RepositoryPath_t* targetRepository, const pusSt23FileName_t* targetFileName);

//
//Getter & Setter
//

//! Setter for the file name of a TC[23,X] and TM[23,4] packet
pusError_t pus_tc_tm_23_X_setFileName(pusPacket_t* outTc, const pusSt23FileName_t* file);

//! Getter for the file name of a TC[23,X] packet
pusError_t pus_tc_tm_23_X_getFileName(pusSt23FileName_t* file, const pusPacket_t* outTc);

//! Setter for the repository path of a TC[23,X] packet
pusError_t pus_tc_tm_23_X_setRepositoryPath(pusPacket_t* outTc, const pusSt23RepositoryPath_t* repository);

//! Getter for the repository path of a TC[23,X] packet
pusError_t pus_tc_tm_23_X_getRepositoryPath(pusSt23RepositoryPath_t* repository, const pusPacket_t* outTc);

//! Setter for the maximum size of a TC[23,1] and TM[23,4] packet
pusError_t pus_tc_tm_23_1_4_setMaximumSize(pusPacket_t* outTc, pusSt23MaximumSize_t size);

//! Getter for the maximum size of a TC[23,1] and TM[23,4] packet
pusError_t pus_tc_tm_23_1_4_getMaximumSize(pusSt23MaximumSize_t* size, const pusPacket_t* outTc);

//! Setter for the lock status of a TC[23,1] and TM[23,4] packet
pusError_t pus_tc_tm_23_1_4_setLock(pusPacket_t* outTc, const bool lock);

//! Getter for the lock status of a TC[23,1] and TM[23,4] packet
pusError_t pus_tc_tm_23_1_4_getLock(bool* lock, const pusPacket_t* outTc);

//! Setter for the search pattern of a TC[23,7] and TM[23,8] packet
pusError_t pus_tc_tm_23_7_8_setSearchPattern(pusPacket_t* outTc, const asn1SccPusByteArray* searchPattern);

//! Getter for the search pattern of a TC[23,7] and TM[23,8] packet
pusError_t pus_tc_tm_23_7_8_getSearchPattern(asn1SccPusByteArray* searchPattern, const pusPacket_t* outTc);

//! Setter for the repository path of a TM[23,8] packet
pusError_t pus_tm_23_8_setMatches(pusPacket_t* outTm, const int matches);

//! Getter for the repository path of a TM[23,8] packet
pusError_t pus_tm_23_8_getMatches(int* matches, const pusPacket_t* outTm);

//! Setter for the repository path of a TM[23,8] packet
pusError_t pus_tm_23_8_setFilePaths(pusPacket_t* outTm, const asn1SccPusByteArray* filePaths);

//! Getter for the repository path of a TM[23,8] packet
pusError_t pus_tm_23_8_getFilePaths(asn1SccPusByteArray* filePaths, const pusPacket_t* outTm);

//! Setter for the repository path of a TC[23,9] and TC[23,10] packet
pusError_t pus_tc_23_9_10_setDirectoryName(pusPacket_t* outTc, const pusSt23DirectoryName_t* directory);

//! Getter for the repository path of a TC[23,9] and TC[23,10] packet
pusError_t pus_tc_23_9_10_getDirectoryName(pusSt23DirectoryName_t* directory, const pusPacket_t* outTc);

//! Setter for the repository path of a TC[23,11] packet
pusError_t pus_tc_23_11_setNewDirectoryName(pusPacket_t* outTc, const pusSt23DirectoryName_t* directory_new);

//! Getter for the repository path of a TC[23,11] packet
pusError_t pus_tc_23_11_getNewDirectoryName(pusSt23DirectoryName_t* directory_new, const pusPacket_t* outTc);

//! Setter for the repository path of a TC[23,11] packet
pusError_t pus_tc_23_11_setOldDirectoryName(pusPacket_t* outTc, const pusSt23DirectoryName_t* directory_old);

//! Getter for the repository path of a TC[23,11] packet
pusError_t pus_tc_23_11_getOldDirectoryName(pusSt23DirectoryName_t* directory_old, const pusPacket_t* outTc);

//! Setter for the repository path of a TM[23,13] packet
pusError_t pus_tm_23_13_setNumberObjects(pusPacket_t* outTm, const int numberObjects);

//! Getter for the repository path of a TM[23,13] packet
pusError_t pus_tm_23_13_getNumberObjects(int* numberObjects, const pusPacket_t* outTm);

//! Setter for the repository path of a TM[23,13] packet
pusError_t pus_tm_23_13_setObjectTypes(pusPacket_t* outTm, const asn1SccPusSt23ObjectType* objectTypes);

//! Getter for the repository path of a TM[23,13] packet
pusError_t pus_tm_23_13_getObjectTypes(asn1SccPusSt23ObjectType* objectTypes, const pusPacket_t* outTm);

//! Setter for the repository path of a TM[23,13] packet
pusError_t pus_tm_23_13_setObjectNames(pusPacket_t* outTm, const asn1SccPusByteArray* objectNames);

//! Getter for the repository path of a TM[23,13] packet
pusError_t pus_tm_23_13_getObjectNames(asn1SccPusByteArray* objectNames, const pusPacket_t* outTm);

//! Setter for the source file name of a TC[23,14] packet
pusError_t pus_tc_23_14_setSourceFileName(pusPacket_t* outTc, const pusSt23FileName_t* fileName);

//! Getter for the source file name of a TC[23,14] packet
pusError_t pus_tc_23_14_getSourceFileName(pusSt23FileName_t* fileName, const pusPacket_t* outTc);

//! Setter for the target file name of a TC[23,14] packet
pusError_t pus_tc_23_14_setTargetFileName(pusPacket_t* outTc, const pusSt23FileName_t* fileName);

//! Getter for the target file name of a TC[23,14] packet
pusError_t pus_tc_23_14_getTargetFileName(pusSt23FileName_t* fileName, const pusPacket_t* outTc);

//! Setter for the source repository path of a TC[23,14] packet
pusError_t pus_tc_23_14_setSourceRepositoryPath(pusPacket_t* outTc, const pusSt23RepositoryPath_t* repository);

//! Getter for the source repository path of a TC[23,14] packet
pusError_t pus_tc_23_14_getSourceRepositoryPath(pusSt23RepositoryPath_t* repository, const pusPacket_t* outTc);

//! Setter for the target repository path of a TC[23,14] packet
pusError_t pus_tc_23_14_setTargetRepositoryPath(pusPacket_t* outTc, const pusSt23RepositoryPath_t* repository);

//! Getter for the target repository path of a TC[23,14] packet
pusError_t pus_tc_23_14_getTargetRepositoryPath(pusSt23RepositoryPath_t* repository, const pusPacket_t* outTc);

//
// Parameter checking
//

//! Check that a packet is of a PUS ST[23] kind
/*! \param[in] packet The PUS packet
 *  \param[in] expectedSubtype Check that the TC has this subtype; use pusSubtype_NONE to check for all TC types in ST[23]
 *  \param[in] function Function name to write as error information (use with the macro \ref PUS_EXPECT_ST23 to include the caller function's name)
 *  \return If valid TC, PUS_NO_ERROR; otherwise, an error code
 */
pusError_t pus_expectSt23Tc(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function);

//! Check that a packet is of a PUS ST[23] kind
/*! \param[in] packet The PUS packet
 *  \param[in] expectedSubtype Check that the TM has this subtype; use pusSubtype_NONE to check for all TM types in ST[23]
 *  \param[in] function Function name to write as error information (use with the macro \ref PUS_EXPECT_ST23 to include the caller function's name)
 *  \return If valid TM, PUS_NO_ERROR; otherwise, an error code
 */
pusError_t pus_expectSt23Tm(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function);

//! Helper macro for pus_expectSt20Tc; adds function name
#define PUS_EXPECT_ST23TC(packet, subtype) pus_expectSt23Tc((packet), (subtype), __func__)

//! Helper macro for pus_expectSt20Tm; adds function name
#define PUS_EXPECT_ST23TM(packet, subtype) pus_expectSt23Tm((packet), (subtype), __func__)

#ifdef  __cplusplus
}
#endif

#endif

#endif // PUS_ST23_PACKETS_H
