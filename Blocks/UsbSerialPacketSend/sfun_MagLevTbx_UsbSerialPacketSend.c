/*
 * Riccardo Antonello (riccardo.antonello@unipd.it)
 *
 * February 17, 2026
 *
 * Dept. of Information Engineering, University of Padova
 */

#define S_FUNCTION_NAME  sfun_MagLevTbx_UsbSerialPacketSend
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

/*  #define NUM_INPUTS      0   determined according to block dialog parameters  */
#define NUM_OUTPUTS         0
#define NUM_PARAMS          5
#define NUM_CONT_STATES     0
#define NUM_DISC_STATES     0
#define NUM_DWORKS          1

/* Input Port X (generic) */
#define INPUT_X_WIDTH       DYNAMICALLY_SIZED
#define INPUT_X_DTYPE       DYNAMICALLY_TYPED
#define INPUT_X_COMPLEX     COMPLEX_NO
#define INPUT_X_FEEDTHROUGH 1

/*  DWork 0 (pointers to payload and output packet buffers)  */
#define DWORK_0_WIDTH       2
#define DWORK_0_DTYPE       SS_POINTER       
#define DWORK_0_COMPLEX     COMPLEX_NO   


/*===========================*
 * Auxiliary data structures *
 *===========================*/

const mwSize validDTypeNum = 9;
const char *validDTypeNames[] = {
    "double",   //     SS_DOUBLE
    "single",   //     SS_SINGLE
    "int8",     //     SS_INT8
    "uint8",    //     SS_UINT8
    "int16",    //     SS_INT16
    "uint16",   //     SS_UINT16
    "int32",    //     SS_INT32
    "uint32",   //     SS_UINT32
    "boolean",  //     SS_BOOLEAN 
};

const DTypeId validDTypeId[] = {
    SS_DOUBLE,
    SS_SINGLE,
    SS_INT8,
    SS_UINT8,
    SS_INT16,
    SS_UINT16,
    SS_INT32,
    SS_UINT32,
    SS_BOOLEAN
};

const mwSize validDTypeSize[] = {
    sizeof(double),
    sizeof(float),
    sizeof(int8_T),
    sizeof(uint8_T),
    sizeof(int16_T),
    sizeof(uint16_T),
    sizeof(int32_T),
    sizeof(uint32_T),
    sizeof(boolean_T),
};


/*=====================*
 * Auxiliary functions *
 *=====================*/

/* parse single cell parameter of the specified cell array */
boolean_T parseCellPar(const mxArray *pCellArray, mwIndex n, 
        DTypeId *pPortDTypeId, int_T *pPortWidth)
{
    mwIndex nd;
    mwSize N;
    mxArray *pCell;
    char *str, portDTypeName[50];
    
    /* get number of elements in cell array */
    N = mxGetNumberOfElements(pCellArray);
    
    /* exit if specified index exceeds bounds */
    if( n < 0 || n >= N )
        return false; 
    
    /* get specified cell parameter */
    pCell = mxGetCell(pCellArray, n);
    
    /* convert cell content to char string */
    str = mxArrayToString(pCell);
    
    /* get corresponding port width and data-type id */
    if( str != NULL)
    {
        /* extract port width */
        if( sscanf(str, "%d*%s", pPortWidth, portDTypeName) == 2  )
        {
            if( *pPortWidth <= 0 )
                return false;
        }
        else
        {
            if( sscanf(str, "%s", portDTypeName) == 1 )
                *pPortWidth = 1;
            else
                return false;
        }
        
        /* extract port data-type id */
        for(nd = 0; nd < validDTypeNum; nd++)
            if( strcmp(portDTypeName, validDTypeNames[nd]) == 0 )
                break;         
        
         if(nd < validDTypeNum)
         {
             *pPortDTypeId = nd;
             return true;
         }
    }    
    
    return false;
}  


/*====================*
 * S-function methods *
 *====================*/

/* Function: mdlCheckParameters ======================================== */
#define MDL_CHECK_PARAMETERS
#if defined(MDL_CHECK_PARAMETERS) && defined(MATLAB_MEX_FILE)
static void mdlCheckParameters(SimStruct *S)
{
    mwSize N;
    mwIndex n, nd;
    mxArray *pCell;
    char *str;
    static char errMsg[128];
    
    int_T inputWidth = 1;
    char inputDTypeName[50];
    
    /*  get block dialog parameters  */
    const mxArray *pa0 = ssGetSFcnParam(S, 0);     /*  Packet Specifier  */
    const mxArray *pa1 = ssGetSFcnParam(S, 1);     /*  COBS Encoding flag  */
    const mxArray *pa2 = ssGetSFcnParam(S, 2);     /*  NULL Terminator flag  */
    const mxArray *pa3 = ssGetSFcnParam(S, 3);     /*  Wait for DTR signal flag  */ 
    const mxArray *pa4 = ssGetSFcnParam(S, 4);     /*  Sample time  */
   
    /* ----------------------------------------------------------------- * 
     *  1st parameter check (Packet Specifier) 
     * ----------------------------------------------------------------- */
    
    if( mxIsEmpty(pa0) ||
            !mxIsCell(pa0) )
    {
        ssSetErrorStatus(S, "1st parameter must be a valid Packet Specifier (cell array of valid data-types).");
        return;
    }
    
    /* get number of elements of cell array */
    N = mxGetNumberOfElements(pa0);
    
    /* check for proper port width and data types in cell array */
    for(n=0; n<N; n++)
    {
        /* get specified cell parameter */
        pCell = mxGetCell(pa0, n);
        
        /* convert cell content to char string */
        str = mxArrayToString(pCell);
        
        /* check for valid port width and data-type id */
        if( str != NULL )
        {
            /* check port width */
            if( !(sscanf(str, "%d*%s", &inputWidth, inputDTypeName) == 2 ||
                    sscanf(str, "%s", inputDTypeName) == 1) )
            {
                sprintf(errMsg, "Invalid element '%s' at position %d of Packet Specifier.", 
                        str, (int)n+1);
                ssSetErrorStatus(S, errMsg);
                return;
            }
            
            if( inputWidth <= 0 )
            {
                sprintf(errMsg, "Invalid input width for element '%s' at position %d of Packet Specifier.", 
                        str, (int)n+1);
                ssSetErrorStatus(S, errMsg);                
                return;
            }
            
            /* check port data-type id */
            for(nd = 0; nd < validDTypeNum; nd++)
                if( strcmp(inputDTypeName, validDTypeNames[nd]) == 0 )
                    break;
            
            if(nd < validDTypeNum)
                continue;
            else
            {
                sprintf(errMsg, "Invalid input data-type for element '%s' at position %d of Packet Specifier.", 
                        str, (int)n+1);
                ssSetErrorStatus(S, errMsg);                
                return; 
            }              
        }
        else
        {
            sprintf(errMsg, "Invalid empty element at position %d of Packet Specifier.",
                    (int)n+1);
            ssSetErrorStatus(S, errMsg);
            return;
        }
    }     
    
    /* ----------------------------------------------------------------- * 
     *  2nd parameter check (COBS Encoding flag) 
     * ----------------------------------------------------------------- */
    
    if(  mxIsEmpty(pa1) || 
        !mxIsLogicalScalar(pa1) )
    {
        ssSetErrorStatus(S, "2nd parameter must be a valid COBS Enable flag (true/false).");
        return;
    }  
    
    /* ----------------------------------------------------------------- * 
     *  3rd parameter check (NULL Terminator flag) 
     * ----------------------------------------------------------------- */
    
    if(  mxIsEmpty(pa2) || 
        !mxIsLogicalScalar(pa2) )
    {
        ssSetErrorStatus(S, "3rd parameter must be a valid NULL Terminator flag (true/false).");
        return;
    }  
    
    /* ----------------------------------------------------------------- * 
     *  4th parameter check (Wait for DTR flag) 
     * ----------------------------------------------------------------- */
    
    if(  mxIsEmpty(pa3) || 
        !mxIsLogicalScalar(pa3) )
    {
        ssSetErrorStatus(S, "4th parameter must be a valid Wait for Host Tx Cmd flag (true/false).");
        return;
    }      
    
    /* ----------------------------------------------------------------- *
     *  5th parameter check (Sample time)
     * ----------------------------------------------------------------- */

    if( mxIsEmpty(pa4) ||
       !mxIsScalar(pa4) ||
       !mxIsNumeric(pa4) ||
       mxIsComplex(pa4) )
    {
        ssSetErrorStatus(S, "5th parameter must be a valid sample time (positive scalar, or -1).");
        return;
    }

    mxDouble *dt4 = mxGetDoubles(pa4);
    if( *dt4 < 0 && *dt4 != -1 )
    {
        ssSetErrorStatus(S, "5th parameter must be a valid sample time (positive scalar, or -1).");
        return;
    }    
}
#endif


/* Function: mdlInitializeSizes ======================================== */
static void mdlInitializeSizes(SimStruct *S)
{
    int_T n;
    int_T numInputPorts;
    
    /* check for expected number of parameters */
    ssSetNumSFcnParams(S, NUM_PARAMS); 
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) 
    {
        /* Return if number of expected != number of actual parameters */
        ssSetErrorStatus(S, "Invalid number of parameters.");
        return;
    }
    
    /* check parameters */
    mdlCheckParameters(S);
    
    /* set parameters as non-tunable */
    for(n = 0; n < NUM_PARAMS; n++)
        ssSetSFcnParamTunable(S, n, 0);
    
    /* get number of elements of Packet Specifier cell array (with rounding to integer) */
    numInputPorts = (int_T)mxGetNumberOfElements( ssGetSFcnParam(S, 0) );  
    
    /* set number of input ports */
    if (!ssSetNumInputPorts(S, numInputPorts)) 
        return;
    
    /* set input ports properties */
    for(n=0; n<numInputPorts; n++)
    {
        ssSetInputPortWidth(S, n, INPUT_X_WIDTH);
        ssSetInputPortDataType(S, n, INPUT_X_DTYPE);
        ssSetInputPortComplexSignal(S, n, INPUT_X_COMPLEX);
        ssSetInputPortDirectFeedThrough(S, n, INPUT_X_FEEDTHROUGH);
        ssSetInputPortRequiredContiguous(S, n, 1);  /*direct input signal access*/
    }
    
    /* set number of output ports */
    if (!ssSetNumOutputPorts(S, NUM_OUTPUTS))
        return;
    
    /* set number of states */
    ssSetNumContStates(S, NUM_CONT_STATES);
    ssSetNumDiscStates(S, NUM_DISC_STATES);

    /* set elementary work vectors */
    ssSetNumSampleTimes(S, 1);
    ssSetNumDWork(S, NUM_DWORKS);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    /*  set DWork properties  */
    ssSetDWorkWidth(S, 0, DWORK_0_WIDTH);
    ssSetDWorkDataType(S, 0, DWORK_0_DTYPE);
    ssSetDWorkUsageType(S, 0, SS_DWORK_USED_AS_DWORK);
    
    /* Specify the sim state compliance to be same as a built-in block */
    ssSetSimStateCompliance(S, USE_DEFAULT_SIM_STATE);

    ssSetOptions(S, 0);
}



/* Function: mdlSetInputPortDimensionInfo ============================== */
#if defined(MATLAB_MEX_FILE)
#define MDL_SET_INPUT_PORT_DIMENSION_INFO
void mdlSetInputPortDimensionInfo(SimStruct *S,
        int_T port,
        const DimsInfo_T *dimsInfo)
{  
    DTypeId parDTypeId;
    int_T parWidth;
    static char errMsg[80];
    mwSize inputWidth;
    
    boolean_T isNotVector = ( dimsInfo->numDims > 2 ||
                             ( (dimsInfo->numDims == 2 ) &&
                             (dimsInfo->dims[0] > 1 && dimsInfo->dims[1] > 1) ) );

    /*  get cell array parameters */
    const mxArray *pa0 = ssGetSFcnParam(S, 0);     /*  Packet Specifier  */

    /* exclude input signals different from scalars and vectors */
    if( isNotVector )
    {
        ssSetErrorStatus(S, "The block only accepts scalar or vector input signals.");
        return;
    }   
    
    /* verify compatibility with specified input width */
    parseCellPar( pa0, port, &parDTypeId, &parWidth);
    
    if( parWidth != dimsInfo->width )
    {
        sprintf(errMsg, "Width of input port %d does not match the specified size (%d).\n",
                port+1, parWidth);
        ssSetErrorStatus(S, errMsg);
        return;
    }
    
    /* set input port width */
    inputWidth = ssGetInputPortWidth(S, port);
    if( inputWidth == DYNAMICALLY_SIZED )
    {
        if( !ssSetInputPortDimensionInfo(S, port, dimsInfo) ) 
            return;
    } 
    else
    {
        if( inputWidth != dimsInfo->width )
        {
            ssSetErrorStatus(S, "Incompatible input port width.");
            return;
        }
    }

} /* MDL_SET_INPUT_PORT_DIMENSION_INFO */
#endif



/* Function: mdlSetDefaultPortDimensionInfo ============================== */
#if defined(MATLAB_MEX_FILE)
#define MDL_SET_DEFAULT_PORT_DIMENSION_INFO
void mdlSetDefaultPortDimensionInfo(SimStruct *S)
{        
    mwIndex n;
    int_T inputNum;
    int_T inputWidth;
    DTypeId inputDTypeId;
   
    /*  get cell array parameters */
    const mxArray *pa0 = ssGetSFcnParam(S, 0);     /*  Packet Specifier  */
    
    /*  get number of input ports  */
    inputNum = ssGetNumInputPorts(S);

    /* set input ports width (if not already set) */ 
    for(n=0; n<inputNum; n++)
    {
        parseCellPar( pa0, n, &inputDTypeId, &inputWidth);
        
        if( ssGetInputPortWidth(S, n) == DYNAMICALLY_SIZED )
            ssSetInputPortWidth(S, n, inputWidth);
    }
    
} /* MDL_SET_DEFAULT_PORT_DIMENSION_INFO */
#endif 



/* Function: mdlSetInputPortDataType ============================== */
#if defined(MATLAB_MEX_FILE)
#define MDL_SET_INPUT_PORT_DATA_TYPE
void mdlSetInputPortDataType(SimStruct *S, 
        int_T port,
        DTypeId id)
{  
    DTypeId parDTypeId;
    int_T parWidth;
    static char errMsg[80];

    /*  get cell array parameters */
    const mxArray *pa0 = ssGetSFcnParam(S, 0);     /*  Packet Specifier  */
    
    /*  get input port DTypeId parameter */
    parseCellPar( pa0, port, &parDTypeId, &parWidth );
    
    /*  set input data type */
    if( id == parDTypeId )
        ssSetInputPortDataType(S, port, id);
    else
    {
        sprintf(errMsg, "Data-type of input port %d does not match the specified type (%s).\n",
                port+1, validDTypeNames[parDTypeId]);
        ssSetErrorStatus(S, errMsg);
        return;        
    }
}
#endif



/* Function: mdlSetDefaultPortDataTypes ============================== */
#if defined(MATLAB_MEX_FILE)
#define MDL_SET_DEFAULT_PORT_DATA_TYPES
void mdlSetDefaultPortDataTypes(SimStruct *S)
{           
    mwIndex n;
    int_T inputNum;
    int_T inputWidth;
    DTypeId inputDTypeId;
   
    /*  get cell array parameters */
    const mxArray *pa0 = ssGetSFcnParam(S, 0);     /*  Packet Specifier  */
    
    /*  get number of input ports  */
    inputNum = ssGetNumInputPorts(S);
    
    /* set input Data Ports type (if not already set) */ 
    for(n=0; n<inputNum; n++)
    {
        parseCellPar( pa0, n, &inputDTypeId, &inputWidth);
        
        if( ssGetInputPortDataType(S, n) == DYNAMICALLY_TYPED )
            ssSetInputPortDataType(S, n, inputDTypeId);
    }
    
}
#endif



/* Function: mdlInitializeSampleTimes ================================== */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    /*  get sample time param  */
    const mxArray *pa4 = ssGetSFcnParam(S, 4);     /*  Sample time  */
    time_T sampleTime = (time_T)mxGetScalar(pa4);

    /*  set sample time  */
    if(sampleTime > 0)
    {
        ssSetSampleTime(S, 0, sampleTime);
        ssSetOffsetTime(S, 0, 0.0);
    }
    else
    {
        ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
        ssSetOffsetTime(S, 0, 0.0);
    }
}



#undef MDL_INITIALIZE_CONDITIONS   /* Change to #undef to remove function */
#if defined(MDL_INITIALIZE_CONDITIONS)
  /* Function: mdlInitializeConditions ================================= */
  static void mdlInitializeConditions(SimStruct *S)
  {
  }
#endif /* MDL_INITIALIZE_CONDITIONS */



#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START) 
  /* Function: mdlStart ================================================ */
  static void mdlStart(SimStruct *S)
  {
  }
#endif /*  MDL_START */



/* Function: mdlOutputs ================================================ */
static void mdlOutputs(SimStruct *S, int_T tid)
{
}



#undef MDL_UPDATE  /* Change to #undef to remove function */
#if defined(MDL_UPDATE)
  /* Function: mdlUpdate =============================================== */
  static void mdlUpdate(SimStruct *S, int_T tid)
  {
  }
#endif /* MDL_UPDATE */



#undef MDL_DERIVATIVES  /* Change to #undef to remove function */
#if defined(MDL_DERIVATIVES)
  /* Function: mdlDerivatives ========================================== */
  static void mdlDerivatives(SimStruct *S)
  {
  }
#endif /* MDL_DERIVATIVES */



/* Function: mdlTerminate ============================================== */
static void mdlTerminate(SimStruct *S)
{
}



#define MDL_ENABLE  /* Change to #undef to remove function */
#if defined(MDL_ENABLE) 
  /* Function: mdlEnable =============================================== */
  static void mdlEnable(SimStruct *S)
  {
  }
#endif /*  MDL_ENABLE */
  
  
  
#define MDL_DISABLE  /* Change to #undef to remove function */
#if defined(MDL_DISABLE) 
  /* Function: mdlDisable=============================================== */
  static void mdlDisable(SimStruct *S)
  {
  }
#endif /*  MDL_DISABLE */
  
  

#if defined(MATLAB_MEX_FILE)
#define MDL_RTW
/* Function: mdlRTW ==================================================== */
static void mdlRTW(SimStruct *S)
{   
    mwIndex n;
    int_T inputNum;
    int8_T *inputDTypeId;
    int8_T *inputDTypeSize, *inputWidth;
    uint32_T blockSize, payloadSize, packetSize;
    
    /*  get block dialog parameters  */
    const mxArray *pa0 = ssGetSFcnParam(S, 0);  /*  Packet Specifier  */
    const mxArray *pa1 = ssGetSFcnParam(S, 1);  /*  COBS Encoding flag  */
    const mxArray *pa2 = ssGetSFcnParam(S, 2);  /*  NULL Terminator flag  */
    const mxArray *pa3 = ssGetSFcnParam(S, 3);  /*  Wait for DTR signal flag  */ 
    const mxArray *pa4 = ssGetSFcnParam(S, 4);  /*  Sample time  */ 

    /*  extract data  */
	bool cobsEncoded = mxIsLogicalScalarTrue(pa1);       
	bool nullTerminated = mxIsLogicalScalarTrue(pa2);  
    bool waitDTR = mxIsLogicalScalarTrue(pa3);
    mxDouble sampleTime = (mxDouble) mxGetScalar(pa4);
    
    /*  get number of inputs  */
    inputNum = ssGetNumInputPorts(S);
    
    /*  create vectors with input port width, DType id, DType (byte) size  */
    inputWidth = (int8_T *)mxMalloc(inputNum);
    inputDTypeId = (int8_T *)mxMalloc(inputNum);
    inputDTypeSize = (int8_T *)mxMalloc(inputNum);
    
    /*  fill vector with DTypeId of input ports, and computes the payload size  */
    payloadSize = 0;
    for(n=0; n<inputNum; n++)
    {
        inputWidth[n] = (int8_T)ssGetInputPortWidth(S, n);
        inputDTypeId[n] = (int8_T)ssGetInputPortDataType(S, n);
        inputDTypeSize[n] = (int8_T)validDTypeSize[inputDTypeId[n]];
        blockSize = inputWidth[n]*inputDTypeSize[n];
        payloadSize += blockSize;
    }
    
    /*  compute packet size  */
    packetSize = payloadSize;
    if(cobsEncoded)
        packetSize++;
    if(nullTerminated)
        packetSize++;
    
    /* write ParamSettings to rtw file */
    if(!ssWriteRTWParamSettings(S, 
            9,  /*  number of parameters to write on model.rtw  */
            SSWRITE_VALUE_DTYPE_NUM, "CobsEncoded", (void *)&cobsEncoded, DTINFO(SS_BOOLEAN, 0),
            SSWRITE_VALUE_DTYPE_NUM, "NullTerminated", (void *)&nullTerminated, DTINFO(SS_BOOLEAN, 0), 
            SSWRITE_VALUE_DTYPE_NUM, "WaitDTR", (void *)&waitDTR, DTINFO(SS_BOOLEAN, 0), 
            SSWRITE_VALUE_DTYPE_VECT, "InputWidth", (void *)inputWidth, inputNum, DTINFO(SS_INT8, 0), 
            SSWRITE_VALUE_DTYPE_VECT, "InputDTypeId", (void *)inputDTypeId, inputNum, DTINFO(SS_INT8, 0),
            SSWRITE_VALUE_DTYPE_VECT, "InputDTypeSize", (void *)inputDTypeSize, inputNum, DTINFO(SS_INT8, 0), 
            SSWRITE_VALUE_DTYPE_NUM, "PayloadSize", (void *)&payloadSize, DTINFO(SS_UINT32, 0),
            SSWRITE_VALUE_DTYPE_NUM, "PacketSize", (void *)&packetSize, DTINFO(SS_UINT32, 0),
            SSWRITE_VALUE_DTYPE_NUM, "SampleTime", (void *)&sampleTime, DTINFO(SS_DOUBLE, 0) ) ) 
    {
        ssSetErrorStatus(S, "Unable to write block dialog parameters to model.rtw file.");
        return; /* An error occurred. */
    }     
    
    /* remove data from heap space */
    mxFree(inputWidth);
    mxFree(inputDTypeId);
    mxFree(inputDTypeSize);
}
#endif /* mdlRTW */

/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
