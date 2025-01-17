/*
 * H2020 ESROCOS Project
 * Company: GMV Aerospace & Defence S.A.U.
 * Licence: GPLv2
 */

#include "pus_error.h"

// Error variables
volatile pusError_t lastErrorCode = PUS_NO_ERROR;
volatile pusErrorFunction_t lastErrorFunction = "none";
volatile pusErrorData_t lastErrorData;


pusError_t pus_setError(pusError_t error, const char* function, pusErrorData_t data)
{
    lastErrorCode = error;
    lastErrorFunction = function;
    lastErrorData = data;
    return error;
}


pusError_t pus_getError(pusError_t* outError, pusErrorFunction_t* outFunction, pusErrorData_t* outData)
{
    pusError_t code = lastErrorCode;
    
    if (NULL != outError)
    {
        *outError = code;
    }
    
    if (NULL != outFunction)
    {
        *outFunction = lastErrorFunction;
    }
    
    if (NULL != outData)
    {
        *outData = lastErrorData;
    }
    
    return code;
}


void pus_clearError()
{
    lastErrorCode = PUS_NO_ERROR;
    lastErrorFunction = "none";
    lastErrorData = (pusErrorData_t){.integer=0};
}


