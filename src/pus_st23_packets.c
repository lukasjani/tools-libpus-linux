/*
 * H2020 ESROCOS Project
 * Company: GMV Aerospace & Defence S.A.U.
 * Licence: GPLv2
 */


#include "pus_st23_packets.h"
#ifdef PUS_CONFIGURE_ST23_ENABLED


pusError_t pus_tc_23_X_createDefaultRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount, pusSubservice_t subtype)
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
		pus_setTcService(outTc, pus_ST23_fileManagement);
		pus_setTcSubtype(outTc, subtype);
		pus_setTcSource(outTc, apid);

		return PUS_GET_ERROR();
	}

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tc_23_1_createCreateFileRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
		const pusSt23RepositoryPath_t* repository, const pusSt23FileName_t* fileName, pusSt23MaximumSize_t maxSize, bool lock)
{
	if ( NULL == outTc || NULL == repository || NULL == fileName )
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pusError_t error = pus_tc_23_X_createDefaultRequest(outTc, apid, sequenceCount, pus_TC_23_1_createFile);
	if( PUS_NO_ERROR != error )
	{
		return PUS_GET_ERROR();
	}

	//Set data field
	pus_setTcDataKind(outTc, pus_TC_DATA_ST_23_1);
	pus_tc_tm_23_X_setRepositoryPath(outTc, repository);
	pus_tc_tm_23_X_setFileName(outTc, fileName);
	pus_tc_tm_23_1_4_setMaximumSize(outTc, maxSize);
	pus_tc_tm_23_1_4_setLock(outTc, lock);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tc_23_2_createDeleteFileRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
		const pusSt23RepositoryPath_t* repository, const pusSt23FileName_t* fileName)
{
	if ( NULL == outTc || NULL == repository || NULL == fileName )
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pusError_t error = pus_tc_23_X_createDefaultRequest(outTc, apid, sequenceCount, pus_TC_23_2_deleteFile);
	if( PUS_NO_ERROR != error )
	{
		return PUS_GET_ERROR();
	}

	//Set data field
	pus_setTcDataKind(outTc, pus_TC_DATA_ST_23_X);
	pus_tc_tm_23_X_setRepositoryPath(outTc, repository);
	pus_tc_tm_23_X_setFileName(outTc, fileName);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tc_23_3_createFileAtributesRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
		const pusSt23RepositoryPath_t* repository, const pusSt23FileName_t* fileName)
{
	if ( NULL == outTc || NULL == repository || NULL == fileName )
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pusError_t error = pus_tc_23_X_createDefaultRequest(outTc, apid, sequenceCount, pus_TC_23_3_reportFileAttributes);
	if( PUS_NO_ERROR != error )
	{
		return PUS_GET_ERROR();
	}

	//Set data field
	pus_setTcDataKind(outTc, pus_TC_DATA_ST_23_X);
	pus_tc_tm_23_X_setRepositoryPath(outTc, repository);
	pus_tc_tm_23_X_setFileName(outTc, fileName);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tm_23_4_createFileAtributesReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusApid_t destination,
		const pusSt23RepositoryPath_t* repository, const pusSt23FileName_t* fileName, pusSt23MaximumSize_t maxSize)
{
	if ( NULL == outTm || NULL == repository || NULL == fileName )
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
		pus_setTmService(outTm, pus_ST23_fileManagement);
		pus_setTmSubtype(outTm, pus_TM_23_4_reportFileAttributes);
		pus_setTmDestination(outTm, destination);

		// Timestamp
		pus_setTmPacketTimeNow(outTm);

		// Set data field
		pus_setTmDataKind(outTm, pus_TM_DATA_ST_23_4);
		pus_tc_tm_23_1_4_setMaximumSize(outTm, maxSize);
		pus_tc_tm_23_X_setRepositoryPath(outTm, repository);
		pus_tc_tm_23_X_setFileName(outTm, fileName);

		return PUS_GET_ERROR();
	}

	return PUS_NO_ERROR;
}

pusError_t pus_tc_23_5_createLockFileRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
		const pusSt23RepositoryPath_t* repository, const pusSt23FileName_t* fileName)
{
	if (NULL == outTc || NULL == repository || NULL == fileName)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pusError_t error = pus_tc_23_X_createDefaultRequest(outTc, apid, sequenceCount, pus_TC_23_5_lockFile);
	if (PUS_NO_ERROR != error)
	{
		return PUS_GET_ERROR();
	}

	//Set data field
	pus_setTcDataKind(outTc, pus_TC_DATA_ST_23_X);
	pus_tc_tm_23_X_setRepositoryPath(outTc, repository);
	pus_tc_tm_23_X_setFileName(outTc, fileName);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tc_23_6_createUnlockFileRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
		const pusSt23RepositoryPath_t* repository, const pusSt23FileName_t* fileName)
{
	if (NULL == outTc || NULL == repository || NULL == fileName)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pusError_t error = pus_tc_23_X_createDefaultRequest(outTc, apid, sequenceCount, pus_TC_23_6_unlockFile);
	if (PUS_NO_ERROR != error)
	{
		return PUS_GET_ERROR();
	}

	//Set data field
	pus_setTcDataKind(outTc, pus_TC_DATA_ST_23_X);
	pus_tc_tm_23_X_setRepositoryPath(outTc, repository);
	pus_tc_tm_23_X_setFileName(outTc, fileName);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tc_23_7_createFindFileRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
		const pusSt23RepositoryPath_t* repository, const pusSt23FileName_t* fileName, const asn1SccPusByteArray* searchPattern)
{
	if (NULL == outTc || NULL == repository || NULL == fileName)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pusError_t error = pus_tc_23_X_createDefaultRequest(outTc, apid, sequenceCount, pus_TC_23_7_requestFindFile);
	if (PUS_NO_ERROR != error)
	{
		return PUS_GET_ERROR();
	}

	//Set data field
	pus_setTcDataKind(outTc, pus_TC_DATA_ST_23_X);
	pus_tc_tm_23_X_setRepositoryPath(outTc, repository);
	pus_tc_tm_23_7_8_setSearchPattern(outTc, searchPattern);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tm_23_8_createFoundFileReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusApid_t destination,
		const pusSt23RepositoryPath_t* repository, const asn1SccPusByteArray* searchPattern, int matches, const asn1SccPusByteArray* filePaths)
{
	if (NULL == outTm || NULL == repository || NULL == searchPattern)
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
		pus_setTmService(outTm, pus_ST23_fileManagement);
		pus_setTmSubtype(outTm, pus_TM_23_8_reportFoundFile);
		pus_setTmDestination(outTm, destination);

		// Timestamp
		pus_setTmPacketTimeNow(outTm);

		// Set data field
		pus_setTmDataKind(outTm, pus_TM_DATA_ST_23_8);
		pus_tc_tm_23_X_setRepositoryPath(outTm, repository);
		pus_tc_tm_23_7_8_setSearchPattern(outTm, searchPattern);
		pus_tm_23_8_setMatches(outTm, matches);
		pus_tm_23_8_setFilePaths(outTm, filePaths);

		return PUS_GET_ERROR();
	}

	return PUS_NO_ERROR;
}

pusError_t pus_tc_23_9_createCreateDirectoryRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
		const pusSt23RepositoryPath_t* repository, const pusSt23DirectoryName_t* directory)
{
	if (NULL == outTc || NULL == repository || NULL == directory)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pusError_t error = pus_tc_23_X_createDefaultRequest(outTc, apid, sequenceCount, pus_TC_23_9_createDirectory);
	if (PUS_NO_ERROR != error)
	{
		return PUS_GET_ERROR();
	}

	//Set data field
	pus_setTcDataKind(outTc, pus_TC_DATA_ST_23_X);
	pus_tc_tm_23_X_setRepositoryPath(outTc, repository);
	pus_tc_23_9_10_setDirectoryName(outTc, directory);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tc_23_10_createDeleteDirectoryRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
		const pusSt23RepositoryPath_t* repository, const pusSt23DirectoryName_t* directory)
{
	if (NULL == outTc || NULL == repository || NULL == directory)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pusError_t error = pus_tc_23_X_createDefaultRequest(outTc, apid, sequenceCount, pus_TC_23_10_deleteDirectory);
	if (PUS_NO_ERROR != error)
	{
		return PUS_GET_ERROR();
	}

	//Set data field
	pus_setTcDataKind(outTc, pus_TC_DATA_ST_23_X);
	pus_tc_tm_23_X_setRepositoryPath(outTc, repository);
	pus_tc_23_9_10_setDirectoryName(outTc, directory);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tc_23_11_createRenameDirectoryRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
		const pusSt23RepositoryPath_t* repository, const pusSt23DirectoryName_t* directory_old, const pusSt23DirectoryName_t* directory_new)
{
	if (NULL == outTc || NULL == repository || NULL == directory_new || NULL == directory_old)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pusError_t error = pus_tc_23_X_createDefaultRequest(outTc, apid, sequenceCount, pus_TC_23_11_renameDirectory);
	if (PUS_NO_ERROR != error)
	{
		return PUS_GET_ERROR();
	}

	//Set data field
	pus_setTcDataKind(outTc, pus_TC_DATA_ST_23_11);
	pus_tc_tm_23_X_setRepositoryPath(outTc, repository);
	pus_tc_23_11_setOldDirectoryName(outTc, directory_old);
	pus_tc_23_11_setNewDirectoryName(outTc, directory_new);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tc_23_12_createSummaryRepositoryRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
		const pusSt23RepositoryPath_t* repository)
{
	if (NULL == outTc || NULL == repository)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pusError_t error = pus_tc_23_X_createDefaultRequest(outTc, apid, sequenceCount, pus_TC_23_12_requestSummaryRepository);
	if (PUS_NO_ERROR != error)
	{
		return PUS_GET_ERROR();
	}

	//Set data field
	pus_setTcDataKind(outTc, pus_TC_DATA_ST_23_12);
	pus_tc_tm_23_X_setRepositoryPath(outTc, repository);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

pusError_t pus_tm_23_13_createSummaryRepositoryReport(pusPacket_t* outTm, pusApid_t apid, pusSequenceCount_t sequenceCount, pusApid_t destination,
		const pusSt23RepositoryPath_t* repository, const int numberObjects, const asn1SccPusSt23ObjectType* objectTypes, const asn1SccPusByteArray* objectNames)
{
	if (NULL == outTm || NULL == repository || NULL == numberObjects)
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
		pus_setTmService(outTm, pus_ST23_fileManagement);
		pus_setTmSubtype(outTm, pus_TM_23_13_reportSummaryRepository);
		pus_setTmDestination(outTm, destination);

		// Timestamp
		pus_setTmPacketTimeNow(outTm);

		// Set data field
		pus_setTmDataKind(outTm, pus_TM_DATA_ST_23_13);
		pus_tc_tm_23_X_setRepositoryPath(outTm, repository);
		pus_tm_23_13_setNumberObjects(outTm, numberObjects);
		pus_tm_23_13_setObjectTypes(outTm, objectTypes);
		pus_tm_23_13_setObjectNames(outTm, objectNames);

		return PUS_GET_ERROR();
	}

	return PUS_NO_ERROR;
}

pusError_t pus_tc_23_14_createCopyFileRequest(pusPacket_t* outTc, pusApid_t apid, pusSequenceCount_t sequenceCount,
		const pusSt23RepositoryPath_t* sourceRepository, const pusSt23FileName_t* sourceFileName,
		const pusSt23RepositoryPath_t* targetRepository, const pusSt23FileName_t* targetFileName)
{
	if ( NULL == outTc || NULL == sourceRepository || NULL == sourceFileName || NULL == targetRepository || NULL == targetFileName )
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	pusError_t error = pus_tc_23_X_createDefaultRequest(outTc, apid, sequenceCount, pus_TC_23_14_copyFile);
	if( PUS_NO_ERROR != error )
	{
		return PUS_GET_ERROR();
	}

	//Set data field
	pus_setTcDataKind(outTc, pus_TC_DATA_ST_23_14);
	pus_tc_23_14_setSourceRepositoryPath(outTc, sourceRepository);
	pus_tc_23_14_setTargetRepositoryPath(outTc, targetRepository);
	pus_tc_23_14_setSourceFileName(outTc, sourceFileName);
	pus_tc_23_14_setTargetFileName(outTc, targetFileName);

	return PUS_SET_ERROR(PUS_NO_ERROR);
}

//
//Getter & Setter
//

pusError_t pus_tc_tm_23_X_setFileName(pusPacket_t* outTc, const pusSt23FileName_t* file)
{
	if( NULL == outTc || file == NULL )
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (pus_TC_DATA_ST_23_X == pus_getTcDataKind(outTc))
	{
		outTc->data.u.tcData.data.u.st_23_X.name = *file;
		return PUS_NO_ERROR;
	}

	if( pus_TC_DATA_ST_23_1 == pus_getTcDataKind(outTc) )
	{
		outTc->data.u.tcData.data.u.st_23_1.fileName = *file;
		return PUS_NO_ERROR;
	}

	if (pus_TM_DATA_ST_23_4 == pus_getTmDataKind(outTc))
	{
		outTc->data.u.tmData.data.u.st_23_4.fileName = *file;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tc_tm_23_X_getFileName(pusSt23FileName_t* file, const pusPacket_t* outTc)
{
	if( NULL == outTc || file == NULL )
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if( PUS_NO_ERROR == PUS_EXPECT_ST23TM(outTc, pus_TM_1_4_failedStart))
	{
		*file = outTc->data.u.tmData.data.u.st_23_4.fileName;
		return PUS_NO_ERROR;
	}

	if( pus_TC_DATA_ST_23_1 == pus_getTcDataKind(outTc) )
	{
		*file = outTc->data.u.tcData.data.u.st_23_1.fileName;
		return PUS_NO_ERROR;
	}
	if( pus_TC_DATA_ST_23_X == pus_getTcDataKind(outTc) )
	{
		*file = outTc->data.u.tcData.data.u.st_23_X.name;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tc_tm_23_X_setRepositoryPath(pusPacket_t* outTc, const pusSt23RepositoryPath_t* repository)
{
	if (NULL == outTc || NULL == repository)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}

	if (pus_TC_DATA_ST_23_X == pus_getTcDataKind(outTc))
	{
		outTc->data.u.tcData.data.u.st_23_X.repository = *repository;
		return PUS_NO_ERROR;
	}

	if (pus_TC_DATA_ST_23_1 == pus_getTcDataKind(outTc))
	{
		outTc->data.u.tcData.data.u.st_23_1.repository = *repository;
		return PUS_NO_ERROR;
	}

	if (pus_TM_DATA_ST_23_4 == pus_getTmDataKind(outTc))
	{
		outTc->data.u.tmData.data.u.st_23_4.repository = *repository;
		return PUS_NO_ERROR;
	}

	if (pus_TM_DATA_ST_23_8 == pus_getTmDataKind(outTc))
	{
		outTc->data.u.tmData.data.u.st_23_8.repository = *repository;
		return PUS_NO_ERROR;
	}

	if (pus_TC_DATA_ST_23_11 == pus_getTcDataKind(outTc))
	{
		outTc->data.u.tcData.data.u.st_23_11.repository = *repository;
		return PUS_NO_ERROR;
	}

	if (pus_TC_DATA_ST_23_12 == pus_getTcDataKind(outTc))
	{
		outTc->data.u.tcData.data.u.st_23_12.repository = *repository;
		return PUS_NO_ERROR;
	}

	if (pus_TM_DATA_ST_23_13 == pus_getTmDataKind(outTc))
	{
		outTc->data.u.tmData.data.u.st_23_13.repository = *repository;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tc_tm_23_X_getRepositoryPath(pusSt23RepositoryPath_t* repository, const pusPacket_t* outTc)
{
	if (NULL == outTc || NULL == repository)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST23TM(outTc, pus_TM_1_4_failedStart))
	{
		//TM
		*repository = outTc->data.u.tmData.data.u.st_23_4.repository;
		return PUS_NO_ERROR;
	}
	if (PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pusSubtype_NONE))
	{
		return PUS_GET_ERROR();
	}

	if (pus_TC_DATA_ST_23_1 == pus_getTcDataKind(outTc))
	{
		*repository = outTc->data.u.tcData.data.u.st_23_1.repository;
		return PUS_NO_ERROR;
	}
	if (pus_TC_DATA_ST_23_X == pus_getTcDataKind(outTc))
	{
		*repository = outTc->data.u.tcData.data.u.st_23_X.repository;
		return PUS_NO_ERROR;
	}
	if (pus_TC_DATA_ST_23_11 == pus_getTcDataKind(outTc))
	{
		*repository = outTc->data.u.tcData.data.u.st_23_11.repository;
		return PUS_NO_ERROR;
	}
	if (pus_TC_DATA_ST_23_12 == pus_getTcDataKind(outTc))
	{
		*repository = outTc->data.u.tcData.data.u.st_23_12.repository;
		return PUS_NO_ERROR;
	}
	if (pus_TM_DATA_ST_23_13 == pus_getTcDataKind(outTc))
	{
		*repository = outTc->data.u.tmData.data.u.st_23_13.repository;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tc_tm_23_1_4_setMaximumSize(pusPacket_t* outTc, const pusSt23MaximumSize_t size)
{
	if (NULL == outTc)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST23TM(outTc, pus_TM_23_4_reportFileAttributes))
	{
		outTc->data.u.tmData.data.u.st_23_4.maxSize = size;
		return PUS_NO_ERROR;
	}
	else if (PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pus_TC_23_1_createFile))
	{
		return PUS_GET_ERROR();
	}
	if (pus_TC_DATA_ST_23_1 != pus_getTcDataKind(outTc))
	{
		return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
	}

	outTc->data.u.tcData.data.u.st_23_1.maxSize = size;
	return PUS_NO_ERROR;
}

pusError_t pus_tc_tm_23_1_4_getMaximumSize(pusSt23MaximumSize_t* size, const pusPacket_t* outTc)
{
	if (NULL == outTc || NULL == size)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST23TM(outTc, pus_TM_23_4_reportFileAttributes))
	{
		*size = outTc->data.u.tmData.data.u.st_23_4.maxSize;
		return PUS_NO_ERROR;
	}
	else if (PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pus_TC_23_1_createFile))
	{
		return PUS_GET_ERROR();
	}
	if (pus_TC_DATA_ST_23_1 != pus_getTcDataKind(outTc))
	{
		return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
	}

	*size = outTc->data.u.tcData.data.u.st_23_1.maxSize;
	return PUS_NO_ERROR;
}

pusError_t pus_tc_tm_23_1_4_setLock(pusPacket_t* outTc, const bool lock)
{
	if (NULL == outTc)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST23TM(outTc, pus_TM_23_4_reportFileAttributes))
	{
		outTc->data.u.tmData.data.u.st_23_4.locked = lock;
		return PUS_NO_ERROR;
	}
	else if (PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pus_TC_23_1_createFile))
	{
		return PUS_GET_ERROR();
	}
	if (pus_TC_DATA_ST_23_1 != pus_getTcDataKind(outTc))
	{
		return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
	}

	outTc->data.u.tcData.data.u.st_23_1.locked = lock;
	return PUS_NO_ERROR;
}

pusError_t pus_tc_tm_23_1_4_getLock(bool* lock, const pusPacket_t* outTc)
{
	if (NULL == outTc)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST23TM(outTc, pus_TM_23_4_reportFileAttributes))
	{
		*lock = outTc->data.u.tmData.data.u.st_23_4.locked;
		return PUS_NO_ERROR;
	}
	else if (PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pus_TC_23_1_createFile))
	{
		return PUS_GET_ERROR();
	}
	if (pus_TC_DATA_ST_23_1 != pus_getTcDataKind(outTc))
	{
		return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
	}

	*lock = outTc->data.u.tcData.data.u.st_23_1.locked;
	return PUS_NO_ERROR;
}

pusError_t pus_tc_tm_23_7_8_setSearchPattern(pusPacket_t* outTc, const asn1SccPusByteArray* searchPattern)
{
	if (NULL == outTc || searchPattern == NULL)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST23TC(outTc, pusSubtype_NONE))
	{
		return PUS_GET_ERROR();
	}
	if (pus_TC_DATA_ST_23_X == pus_getTcDataKind(outTc))
	{
		*(outTc->data.u.tcData.data.u.st_23_1.fileName.arr) = searchPattern;
		outTc->data.u.tcData.data.u.st_23_1.fileName.nCount = sizeof(searchPattern) / sizeof(char);
		return PUS_NO_ERROR;
	}
	if (pus_TM_DATA_ST_23_8 == pus_getTmDataKind(outTc)) {
		outTc->data.u.tmData.data.u.st_23_8.searchPattern = *searchPattern;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tc_tm_23_7_8_getSearchPattern(asn1SccPusByteArray* searchPattern, const pusPacket_t* outTc)
{
	if (NULL == outTc || searchPattern == NULL)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST23TC(outTc, pusSubtype_NONE))
	{
		return PUS_GET_ERROR();
	}
	if (pus_TC_DATA_ST_23_X == pus_getTcDataKind(outTc))
	{
		*searchPattern = outTc->data.u.tcData.data.u.st_23_1.fileName;
		return PUS_NO_ERROR;
	}
	if (pus_TM_DATA_ST_23_8 == pus_getTmDataKind(outTc)) {
		*searchPattern = outTc->data.u.tmData.data.u.st_23_8.searchPattern;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tm_23_8_setMatches(pusPacket_t* outTm, const int matches)
{
	if (NULL == outTm)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST23TM(outTm, pus_TM_23_8_reportFoundFile))
	{
		outTm->data.u.tmData.data.u.st_23_8.matches = matches;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tm_23_8_getMatches(int* matches, const pusPacket_t* outTm)
{
	if (NULL == outTm)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST23TM(outTm, pus_TM_23_8_reportFoundFile))
	{
		*matches = outTm->data.u.tmData.data.u.st_23_8.matches;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tm_23_8_setFilePaths(pusPacket_t* outTm, const asn1SccPusByteArray* filePaths)
{
	if (NULL == outTm)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (outTm->data.u.tmData.data.u.st_23_8.matches == 0)
	{
		return PUS_SET_ERROR(PUS_ERROR_BEFORE);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST23TM(outTm, pus_TM_23_8_reportFoundFile))
	{
		for (int i = 0; i < outTm->data.u.tmData.data.u.st_23_8.matches; i++)
		{
			outTm->data.u.tmData.data.u.st_23_8.filePaths[i] = filePaths[i];
		}
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tm_23_8_getFilePaths(asn1SccPusByteArray* filePaths, const pusPacket_t* outTm)
{
	if (NULL == outTm)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR == PUS_EXPECT_ST23TM(outTm, pus_TM_23_8_reportFoundFile))
	{
		filePaths = outTm->data.u.tmData.data.u.st_23_8.filePaths;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tc_23_9_10_setDirectoryName(pusPacket_t* outTc, const pusSt23DirectoryName_t* directory)
{
	if (NULL == outTc || directory == NULL)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pusSubtype_NONE))
	{
		return PUS_GET_ERROR();
	}

	if (pus_TC_DATA_ST_23_X == pus_getTcDataKind(outTc))
	{
		outTc->data.u.tcData.data.u.st_23_X.name = *directory;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tc_23_9_10_getDirectoryName(pusSt23DirectoryName_t* directory, const pusPacket_t* outTc)
{
	if (NULL == outTc || directory == NULL)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pusSubtype_NONE))
	{
		return PUS_GET_ERROR();
	}

	if (pus_TC_DATA_ST_23_X == pus_getTcDataKind(outTc))
	{
		*directory = outTc->data.u.tcData.data.u.st_23_X.name;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tc_23_11_setNewDirectoryName(pusPacket_t* outTc, const pusSt23DirectoryName_t* directory_new)
{
	if (NULL == outTc || directory_new == NULL)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pusSubtype_NONE))
	{
		return PUS_GET_ERROR();
	}

	if (pus_TC_DATA_ST_23_11 == pus_getTcDataKind(outTc))
	{
		outTc->data.u.tcData.data.u.st_23_11.directory_new = *directory_new;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tc_23_11_getNewDirectoryName(pusSt23DirectoryName_t* directory_new, const pusPacket_t* outTc)
{
	if (NULL == outTc || directory_new == NULL)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pusSubtype_NONE))
	{
		return PUS_GET_ERROR();
	}

	if (pus_TC_DATA_ST_23_11 == pus_getTcDataKind(outTc))
	{
		*directory_new = outTc->data.u.tcData.data.u.st_23_11.directory_new;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tc_23_11_setOldDirectoryName(pusPacket_t* outTc, const pusSt23DirectoryName_t* directory_old)
{
	if (NULL == outTc || directory_old == NULL)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pusSubtype_NONE))
	{
		return PUS_GET_ERROR();
	}

	if (pus_TC_DATA_ST_23_11 == pus_getTcDataKind(outTc))
	{
		outTc->data.u.tcData.data.u.st_23_11.directory_old = *directory_old;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tc_23_11_getOldDirectoryName(pusSt23DirectoryName_t* directory_old, const pusPacket_t* outTc)
{
	if (NULL == outTc || directory_old == NULL)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pusSubtype_NONE))
	{
		return PUS_GET_ERROR();
	}

	if (pus_TC_DATA_ST_23_11 == pus_getTcDataKind(outTc))
	{
		*directory_old = outTc->data.u.tcData.data.u.st_23_11.directory_old;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tm_23_13_setNumberObjects(pusPacket_t* outTm, const int numberObjects)
{
	if (NULL == outTm || numberObjects == NULL)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR != PUS_EXPECT_ST23TM(outTm, pusSubtype_NONE))
	{
		return PUS_GET_ERROR();
	}

	if (pus_TM_DATA_ST_23_13 == pus_getTmDataKind(outTm))
	{
		outTm->data.u.tmData.data.u.st_23_13.numberObjects = numberObjects;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tm_23_13_getNumberObjects(int* numberObjects, const pusPacket_t* outTm)
{
	if (NULL == outTm || numberObjects == NULL)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR != PUS_EXPECT_ST23TM(outTm, pusSubtype_NONE))
	{
		return PUS_GET_ERROR();
	}

	if (pus_TM_DATA_ST_23_13 == pus_getTmDataKind(outTm))
	{
		*numberObjects = outTm->data.u.tmData.data.u.st_23_13.numberObjects;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tm_23_13_setObjectTypes(pusPacket_t* outTm, const asn1SccPusSt23ObjectType* objectTypes)
{
	if (NULL == outTm || objectTypes == NULL)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR != PUS_EXPECT_ST23TM(outTm, pusSubtype_NONE))
	{
		return PUS_GET_ERROR();
	}
	if (outTm->data.u.tmData.data.u.st_23_13.numberObjects == 0)
	{
		return PUS_SET_ERROR(PUS_ERROR_BEFORE);
	}

	if (pus_TM_DATA_ST_23_13 == pus_getTmDataKind(outTm))
	{
		for (int i = 0; i < outTm->data.u.tmData.data.u.st_23_13.numberObjects; i++)
		{
			outTm->data.u.tmData.data.u.st_23_13.objectTypes[i] = objectTypes[i];
		}
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tm_23_13_getObjectTypes(asn1SccPusSt23ObjectType* objectTypes, const pusPacket_t* outTm)
{
	if (NULL == outTm || objectTypes == NULL)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR != PUS_EXPECT_ST23TM(outTm, pusSubtype_NONE))
	{
		return PUS_GET_ERROR();
	}

	if (pus_TM_DATA_ST_23_13 == pus_getTmDataKind(outTm))
	{
		objectTypes = outTm->data.u.tmData.data.u.st_23_13.objectTypes;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tm_23_13_setObjectNames(pusPacket_t* outTm, const asn1SccPusByteArray* objectNames)
{
	if (NULL == outTm || objectNames == NULL)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR != PUS_EXPECT_ST23TM(outTm, pusSubtype_NONE))
	{
		return PUS_GET_ERROR();
	}
	if (outTm->data.u.tmData.data.u.st_23_13.numberObjects == 0)
	{
		return PUS_SET_ERROR(PUS_ERROR_BEFORE);
	}

	if (pus_TM_DATA_ST_23_13 == pus_getTmDataKind(outTm))
	{
		for (int i = 0; i < outTm->data.u.tmData.data.u.st_23_13.numberObjects; i++)
		{
			outTm->data.u.tmData.data.u.st_23_13.objectNames[i] = objectNames[i];
		}
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tm_23_13_getObjectNames(asn1SccPusByteArray* objectNames, const pusPacket_t* outTm)
{
	if (NULL == outTm || objectNames == NULL)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if (PUS_NO_ERROR != PUS_EXPECT_ST23TM(outTm, pusSubtype_NONE))
	{
		return PUS_GET_ERROR();
	}

	if (pus_TM_DATA_ST_23_13 == pus_getTmDataKind(outTm))
	{
		objectNames = outTm->data.u.tmData.data.u.st_23_13.objectNames;
		return PUS_NO_ERROR;
	}

	return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
}

pusError_t pus_tc_23_14_setSourceFileName(pusPacket_t* outTc, const pusSt23FileName_t* fileName)
{
	if( NULL == outTc || NULL == fileName )
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if( PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pus_TC_23_14_copyFile))
	{
		return PUS_GET_ERROR();
	}
	if( pus_TC_DATA_ST_23_14 != pus_getTcDataKind(outTc) )
	{
		return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
	}

	outTc->data.u.tcData.data.u.st_23_14.sourceFileName = *fileName;
	return PUS_NO_ERROR;
}

pusError_t pus_tc_23_14_getSourceFileName(pusSt23FileName_t* fileName, const pusPacket_t* outTc)
{
	if( NULL == outTc || NULL == fileName)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if( PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pus_TC_23_14_copyFile))
	{
		return PUS_GET_ERROR();
	}
	if( pus_TC_DATA_ST_23_14 != pus_getTcDataKind(outTc) )
	{
		return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
	}

	*fileName = outTc->data.u.tcData.data.u.st_23_14.sourceFileName;
	return PUS_NO_ERROR;
}

pusError_t pus_tc_23_14_setTargetFileName(pusPacket_t* outTc, const pusSt23FileName_t* fileName)
{
	if( NULL == outTc || NULL == fileName )
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if( PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pus_TC_23_14_copyFile))
	{
		return PUS_GET_ERROR();
	}
	if( pus_TC_DATA_ST_23_14 != pus_getTcDataKind(outTc) )
	{
		return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
	}

	outTc->data.u.tcData.data.u.st_23_14.targetFileName = *fileName;
	return PUS_NO_ERROR;
}

pusError_t pus_tc_23_14_getTargetFileName(pusSt23FileName_t* fileName, const pusPacket_t* outTc)
{
	if( NULL == outTc || NULL == fileName )
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if( PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pus_TC_23_14_copyFile))
	{
		return PUS_GET_ERROR();
	}
	if( pus_TC_DATA_ST_23_14 != pus_getTcDataKind(outTc) )
	{
		return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
	}

	*fileName = outTc->data.u.tcData.data.u.st_23_14.targetFileName;
	return PUS_NO_ERROR;
}

pusError_t pus_tc_23_14_setSourceRepositoryPath(pusPacket_t* outTc, const pusSt23RepositoryPath_t* repository)
{
	if( NULL == outTc || NULL == repository )
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if( PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pus_TC_23_14_copyFile))
	{
		return PUS_GET_ERROR();
	}
	if( pus_TC_DATA_ST_23_14 != pus_getTcDataKind(outTc) )
	{
		return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
	}

	outTc->data.u.tcData.data.u.st_23_14.sourceRepository = *repository;
	return PUS_NO_ERROR;
}

pusError_t pus_tc_23_14_getSourceRepositoryPath(pusSt23RepositoryPath_t* repository, const pusPacket_t* outTc)
{
	if( NULL == outTc || NULL == repository )
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if( PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pus_TC_23_14_copyFile))
	{
		return PUS_GET_ERROR();
	}
	if( pus_TC_DATA_ST_23_14 != pus_getTcDataKind(outTc) )
	{
		return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
	}

	*repository = outTc->data.u.tcData.data.u.st_23_14.sourceRepository;
	return PUS_NO_ERROR;
}

pusError_t pus_tc_23_14_setTargetRepositoryPath(pusPacket_t* outTc, const pusSt23RepositoryPath_t* repository)
{
	if( NULL == outTc || NULL == repository )
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if( PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pus_TC_23_14_copyFile))
	{
		return PUS_GET_ERROR();
	}
	if( pus_TC_DATA_ST_23_14 != pus_getTcDataKind(outTc) )
	{
		return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
	}

	outTc->data.u.tcData.data.u.st_23_14.targetRepository = *repository;
	return PUS_NO_ERROR;
}

pusError_t pus_tc_23_14_getTargetRepositoryPath(pusSt23RepositoryPath_t* repository, const pusPacket_t* outTc)
{
	if( NULL == outTc || NULL == repository)
	{
		return PUS_SET_ERROR(PUS_ERROR_NULLPTR);
	}
	if( PUS_NO_ERROR != PUS_EXPECT_ST23TC(outTc, pus_TC_23_14_copyFile))
	{
		return PUS_GET_ERROR();
	}
	if( pus_TC_DATA_ST_23_14 != pus_getTcDataKind(outTc) )
	{
		return PUS_SET_ERROR(PUS_ERROR_TC_KIND);
	}

	*repository = outTc->data.u.tcData.data.u.st_23_14.targetRepository;
	return PUS_NO_ERROR;
}

//
// Parameter checking
//

pusError_t pus_expectSt23Tm(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function)
{
	pusError_t expectResult = pus_expectTmHeader(packet, function, (pusErrorData_t){.integer=0});
	if (PUS_NO_ERROR == expectResult)
	{
		pusService_t service = pus_getTmService(packet);
		pusSubservice_t subtype = pus_getTmSubtype(packet);

		if (service != pus_ST23_fileManagement)
		{
			//pus_setError(PUS_ERROR_TM_SERVICE, function, (pusErrorData_t){.integer=service});
			return PUS_ERROR_TM_SERVICE;
		}

		if (expectedSubtype == pusSubtype_NONE)
		{
			// Check that subtype corresponds to ST[23]
			if ((subtype != pus_TM_23_4_reportFileAttributes) && 
				(subtype != pus_TM_23_8_reportFoundFile) && 
				(subtype != pus_TM_23_13_reportSummaryRepository))
			{
				//pus_setError(PUS_ERROR_TM_SUBTYPE, function, (pusErrorData_t){.integer=subtype});
				return PUS_ERROR_TM_SUBTYPE;
			}
		}
		else
		{
			// Check that subtype is as expected
			if (subtype != expectedSubtype)
			{
				//pus_setError(PUS_ERROR_TM_SUBTYPE, function, (pusErrorData_t){.integer=subtype});
				return PUS_ERROR_TM_SUBTYPE;

			}
		}

		pusPacketDataKind_t kind = pus_getTmDataKind(packet);
		if ((kind != pus_TM_DATA_ST_23_4) && 
			(kind != pus_TM_DATA_ST_23_8) && 
			(kind != pus_TM_DATA_ST_23_13))
		{
			//pus_setError(PUS_ERROR_TM_KIND, function, (pusErrorData_t){.integer=kind});
			return PUS_ERROR_TM_KIND;
		}

		return PUS_NO_ERROR;
	}
	else
	{
		return expectResult;
	}
}

pusError_t pus_expectSt23Tc(const pusPacket_t* packet, pusSubservice_t expectedSubtype, const char* function)
{
	pusError_t expectResult = pus_expectTcHeader(packet, function, (pusErrorData_t){.integer=0});
	if (PUS_NO_ERROR == expectResult)
	{
		pusService_t service = pus_getTcService(packet);
		pusSubservice_t subtype = pus_getTcSubtype(packet);

		if (service != pus_ST23_fileManagement)
		{
			//pus_setError(PUS_ERROR_TC_SERVICE, function, (pusErrorData_t){.integer=service});
			return PUS_ERROR_TC_SERVICE;
		}

		if (expectedSubtype == pusSubtype_NONE)
		{
			// Check that subtype corresponds to ST[23]
			if ((subtype != pus_TC_23_1_createFile) &&
				(subtype != pus_TC_23_2_deleteFile) &&
				(subtype != pus_TC_23_3_reportFileAttributes) &&
				(subtype != pus_TC_23_5_lockFile) &&
				(subtype != pus_TC_23_6_unlockFile) &&
				(subtype != pus_TC_23_7_requestFindFile) &&
				(subtype != pus_TC_23_9_createDirectory) &&
				(subtype != pus_TC_23_10_deleteDirectory) &&
				(subtype != pus_TC_23_11_renameDirectory) &&
				(subtype != pus_TC_23_12_requestSummaryRepository) &&
				(subtype != pus_TC_23_14_copyFile))
			{
				//pus_setError(PUS_ERROR_TC_SUBTYPE, function, (pusErrorData_t){.integer=subtype});
				return PUS_ERROR_TC_SUBTYPE;
			}
		}
		else
		{
			// Check that subtype is as expected
			if (subtype != expectedSubtype)
			{
				//pus_setError(PUS_ERROR_TC_SUBTYPE, function, (pusErrorData_t){.integer=subtype});
				return PUS_ERROR_TC_SUBTYPE;
			}
		}

		pusPacketDataKind_t kind = pus_getTcDataKind(packet);
		if ((kind != pus_TC_DATA_ST_23_1) && 
			(kind != pus_TC_DATA_ST_23_X) &&
			(kind != pus_TC_DATA_ST_23_11) &&
			(kind != pus_TC_DATA_ST_23_12) &&
			(kind != pus_TC_DATA_ST_23_14))
		{
			//pus_setError(PUS_ERROR_TC_KIND, function, (pusErrorData_t){.integer=kind});
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
