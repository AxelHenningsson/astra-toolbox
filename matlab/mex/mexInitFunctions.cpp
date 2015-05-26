#include <mex.h>
#include "astra/Logging.h"

bool mexIsInitialized=false;

/**
 * Callback to print log message to Matlab window.
 *
 */
void logCallBack(const char *msg, size_t len){
    mexPrintf(msg);
}

/**
 * Initialize mex functions.
 *
 */
void initASTRAMex(){
    if(mexIsInitialized) return;
    if(!astra::CLogger::setCallbackScreen(&logCallBack)){
        mexErrMsgTxt("Error initializing mex functions.");
    }
    mexIsInitialized=true;
}
