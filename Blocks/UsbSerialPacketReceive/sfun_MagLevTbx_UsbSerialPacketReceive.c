/*
 * Riccardo Antonello (riccardo.antonello@unipd.it)
 * 
 * February 18, 2026
 *
 * Dept. of Management and Engineering, University of Padova 
 *
 */

#define S_FUNCTION_NAME  sfun_MagLevTbx_UsbSerialPacketReceive
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include <string.h>
#include <math.h>

#define NUM_INPUTS          0
/* #define NUM_OUTPUTS      0   determined according to block dialog parameters  */
#define NUM_PARAMS          6
#define NUM_CONT_STATES     0
#define NUM_DISC_STATES     0
#define NUM_DWORKS          1

#define SAMPLE_TIME_0       INHERITED_SAMPLE_TIME

/*  Output Port - Data Port X (generic)  */
#define OUTPUT_DATA_PORT_X_WIDTH       DYNAMICALLY_SIZED
#define OUTPUT_DATA_PORT_X_DTYPE       DYNAMICALLY_TYPED
#define OUTPUT_DATA_PORT_X_COMPLEX     COMPLEX_NO 

/*  Output Port - Status Port (1 = packet received; 0 otherwise)  */
#define OUTPUT_STATUS_PORT_WIDTH    1
#define OUTPUT_STATUS_PORT_DTYPE    SS_UINT8 
#define OUTPUT_STATUS_PORT_COMPLEX  COMPLEX_NO

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
    
    int_T outputWidth = 1;
    char outputDTypeName[50];
    
    /*  get block dialog parameters  */
    const mxArray *pa0 = ssGetSFcnParam(S, 0);     /*  Packet Specifier  */
    const mxArray *pa1 = ssGetSFcnParam(S, 1);     /*  COBS Encoding flag */
    const mxArray *pa2 = ssGetSFcnParam(S, 2);     /*  NULL Terminator flag */
    const mxArray *pa3 = ssGetSFcnParam(S, 3);     /*  Status Port show flag */
    const mxArray *pa4 = ssGetSFcnParam(S, 4);     /*  Output mode */
    const mxArray *pa5 = ssGetSFcnParam(S, 5);     /*  Sample time */
    
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
            if( !(sscanf(str, "%d*%s", &outputWidth, outputDTypeName) == 2 ||
                    sscanf(str, "%s", outputDTypeName) == 1) )
            {
                sprintf(errMsg, "Invalid element '%s' at position %d of Packet Specifier.", 
                        str, (int)n+1);
                ssSetErrorStatus(S, errMsg);
                return;
            }
            
            if( outputWidth <= 0 )
            {
                sprintf(errMsg, "Invalid output width for element '%s' at position %d of Packet Specifier.", 
                        str, (int)n+1);
                ssSetErrorStatus(S, errMsg);                
                return;
            }
            
            /* check port data-type id */
            for(nd = 0; nd < validDTypeNum; nd++)
                if( strcmp(outputDTypeName, validDTypeNames[nd]) == 0 )
                    break;
            
            if(nd < validDTypeNum)
                continue;
            else
            {
                sprintf(errMsg, "Invalid output data-type for element '%s' at position %d of Packet Specifier.", 
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
     *  4th parameter check (Status Port enable flag) 
     * ----------------------------------------------------------------- */
    
    if(  mxIsEmpty(pa3) || 
        !mxIsLogicalScalar(pa3) )
    {
        ssSetErrorStatus(S, "4th parameter must be a valid Status Port enable flag (true/false).");
        return;
    }  
    
    /* ----------------------------------------------------------------- * 
     *  5th parameter check (Output mode) 
     * ----------------------------------------------------------------- */
    
    if( mxIsEmpty(pa4) ||
            !mxIsScalar(pa4) ||
            !mxIsNumeric(pa4) ||
            mxIsComplex(pa4) )
    {
        ssSetErrorStatus(S, "5th parameter must be a valid output mode (1 or 2).");
        return;        
    }
    
    mxDouble *dt4 = mxGetDoubles(pa4);
    if( ((int)*dt4) < 1 || ((int)*dt4) > 2)
    {
        ssSetErrorStatus(S, "5th parameter must be a valid output mode (1 or 2).");
        return;
    }
    
    /* ----------------------------------------------------------------- *
     *  6th parameter check (Sample time)
     * ----------------------------------------------------------------- */

    if( mxIsEmpty(pa5) ||
       !mxIsScalar(pa5) ||
       !mxIsNumeric(pa5) ||
       mxIsComplex(pa5) )
    {
        ssSetErrorStatus(S, "6th parameter must be a valid sample time (positive scalar, or -1).");
        return;
    }

    mxDouble *dt5 = mxGetDoubles(pa5);
    if( *dt5 < 0 && *dt5 != -1 )
    {
        ssSetErrorStatus(S, "6th parameter must be a valid sample time (positive scalar, or -1).");
        return;
    }    

}
#endif
          


/* Function: mdlInitializeSizes ======================================== */
static void mdlInitializeSizes(SimStruct *S)
{
    int_T n;
    int_T numOutputDataPorts;
    boolean_T showStatusPort;

    /*  get block dialog parameters  */
    const mxArray *pa0 = ssGetSFcnParam(S, 0);     /*  Packet Specifier  */
    const mxArray *pa1 = ssGetSFcnParam(S, 1);     /*  COBS Encoding flag */
    const mxArray *pa2 = ssGetSFcnParam(S, 2);     /*  NULL Terminator flag */
    const mxArray *pa3 = ssGetSFcnParam(S, 3);     /*  Status Port show flag */
    const mxArray *pa4 = ssGetSFcnParam(S, 4);     /*  Output mode */
    const mxArray *pa5 = ssGetSFcnParam(S, 5);     /*  Sample time */
   
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
    
    /* set number of input ports */
    if (!ssSetNumInputPorts(S, NUM_INPUTS)) 
        return;
    
    /* get number of output Data Ports */  
    numOutputDataPorts = (int_T)mxGetNumberOfElements( pa0 ); 
    
    /* check for possible output Status Port */
    showStatusPort = mxIsLogicalScalarTrue( pa3 );
    
    /* set number of output ports */
    if(showStatusPort)
    {
        if (!ssSetNumOutputPorts(S, numOutputDataPorts + 1))
            return;
    }
    else
    {
        if (!ssSetNumOutputPorts(S, numOutputDataPorts))
            return;
    }
    
    /* set output Data Ports properties */
    for(n=0; n<numOutputDataPorts; n++)
    {
        ssSetOutputPortWidth(S, n, OUTPUT_DATA_PORT_X_WIDTH);
        ssSetOutputPortDataType(S, n, OUTPUT_DATA_PORT_X_DTYPE);
        ssSetOutputPortComplexSignal(S, n, OUTPUT_DATA_PORT_X_COMPLEX);
    }
    
    /* set output Status Port properties */
    if(showStatusPort)
    {
        ssSetOutputPortWidth(S, numOutputDataPorts, OUTPUT_STATUS_PORT_WIDTH);
        ssSetOutputPortDataType(S, numOutputDataPorts, OUTPUT_STATUS_PORT_DTYPE);
        ssSetOutputPortComplexSignal(S, numOutputDataPorts, OUTPUT_STATUS_PORT_COMPLEX);        
    }
     
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



/* Function: mdlSetOutputPortDimensionInfo ============================== */
#if defined(MATLAB_MEX_FILE)
#define MDL_SET_OUTPUT_PORT_DIMENSION_INFO
void mdlSetOutputPortDimensionInfo(SimStruct *S,
        int_T port,
        const DimsInfo_T *dimsInfo)
{   
    DTypeId parDTypeId;
    int_T parWidth;
    int_T outputWidth;
    static char errMsg[80];
    
    boolean_T isNotVector = ( dimsInfo->numDims > 2 ||
                             ( (dimsInfo->numDims == 2 ) &&
                             (dimsInfo->dims[0] > 1 && dimsInfo->dims[1] > 1) ) );

    /*  get block dialog parameters  */
    const mxArray *pa0 = ssGetSFcnParam(S, 0);     /*  Packet Specifier  */
    const mxArray *pa1 = ssGetSFcnParam(S, 1);     /*  COBS Encoding flag */
    const mxArray *pa2 = ssGetSFcnParam(S, 2);     /*  NULL Terminator flag */
    const mxArray *pa3 = ssGetSFcnParam(S, 3);     /*  Status Port show flag */
    const mxArray *pa4 = ssGetSFcnParam(S, 4);     /*  Output mode */
    const mxArray *pa5 = ssGetSFcnParam(S, 5);     /*  Sample time */

    /*  exclude output signals different from scalars and vectors  */
    if( isNotVector )
    {
        ssSetErrorStatus(S, "The block only allows scalar or vector output signals.");
        return;
    }   
        
    /*  Data port case  */
    
    /* verify compatibility with specified output width */
    parseCellPar( pa0, port, &parDTypeId, &parWidth); 
    
    if( parWidth != dimsInfo->width )
    {
        sprintf(errMsg, "Width of output Data Port %d does not match the specified size (%d).\n",
                port+1, parWidth);
        ssSetErrorStatus(S, errMsg);
        return;
    }
    
    /* set output port width */
    outputWidth = ssGetOutputPortWidth(S, port);
    if( outputWidth == DYNAMICALLY_SIZED )
    {
        if( !ssSetOutputPortDimensionInfo(S, port, dimsInfo) ) 
        {
            sprintf(errMsg, "Unable to set width of output Data Port %d.\n",
                    port+1);
            ssSetErrorStatus(S, errMsg);
            return;
        }
    } 
    else
    {
        if( outputWidth != dimsInfo->width )
        {
            ssSetErrorStatus(S, "Incompatible output port width.");
            return;
        }
    }    
    
    /*  Status port case  */
    if( port == ssGetNumOutputPorts(S) &&
            mxIsLogicalScalarTrue(pa3) )
    {
        outputWidth = ssGetOutputPortWidth(S, port);
        if( outputWidth != dimsInfo->width )
        {
            ssSetErrorStatus(S, "Incompatible Status Port width.");
            return;
        }
        
    }
      
} /* MDL_SET_OUTPUT_PORT_DIMENSION_INFO */
#endif 



/* Function: mdlSetDefaultPortDimensionInfo ============================== */
#if defined(MATLAB_MEX_FILE)
#define MDL_SET_DEFAULT_PORT_DIMENSION_INFO
void mdlSetDefaultPortDimensionInfo(SimStruct *S)
{        
    mwIndex n;
    int_T outputNum;
    int_T outputWidth;
    DTypeId outputDTypeId;
    boolean_T showStatusPort;
   
    /*  get block dialog parameters  */
    const mxArray *pa0 = ssGetSFcnParam(S, 0);     /*  Packet Specifier  */
    const mxArray *pa1 = ssGetSFcnParam(S, 1);     /*  COBS Encoding flag */
    const mxArray *pa2 = ssGetSFcnParam(S, 2);     /*  NULL Terminator flag */
    const mxArray *pa3 = ssGetSFcnParam(S, 3);     /*  Status Port show flag */
    const mxArray *pa4 = ssGetSFcnParam(S, 4);     /*  Output mode */
    const mxArray *pa5 = ssGetSFcnParam(S, 5);     /*  Sample time */
    
    /* get show Status Port flag */
    showStatusPort = mxIsLogicalScalarTrue(pa3); 
    
    /*  get number of output data ports  */
    outputNum = ssGetNumOutputPorts(S);
    if(showStatusPort)
        outputNum--;

    /* set output Data Ports width (if not already set) */ 
    for(n=0; n<outputNum; n++)
    {
        parseCellPar( pa0, n, &outputDTypeId, &outputWidth );
        
        if( ssGetOutputPortWidth(S, n) == DYNAMICALLY_SIZED )
            ssSetOutputPortWidth(S, n, outputWidth);
    }
    
} /* MDL_SET_DEFAULT_PORT_DIMENSION_INFO */
#endif 



/* Function: mdlSetOutputPortDataType ============================== */
#if defined(MATLAB_MEX_FILE)
#define MDL_SET_OUTPUT_PORT_DATA_TYPE
void mdlSetOutputPortDataType(SimStruct *S, int_T port, DTypeId id)
{  
    DTypeId parDTypeId;
    int_T parWidth;
    static char errMsg[80];

    /*  get block dialog parameters  */
    const mxArray *pa0 = ssGetSFcnParam(S, 0);     /*  Packet Specifier  */
    const mxArray *pa1 = ssGetSFcnParam(S, 1);     /*  COBS Encoding flag */
    const mxArray *pa2 = ssGetSFcnParam(S, 2);     /*  NULL Terminator flag */
    const mxArray *pa3 = ssGetSFcnParam(S, 3);     /*  Status Port show flag */
    const mxArray *pa4 = ssGetSFcnParam(S, 4);     /*  Output mode */
    const mxArray *pa5 = ssGetSFcnParam(S, 5);     /*  Sample time */
       
    /*  Data port case  */
    
    /* get data port DTypeId parameter */
    parseCellPar( pa0, port, &parDTypeId, &parWidth );
    
    /* set output data type */
    if( id == parDTypeId )
        ssSetOutputPortDataType(S, port, id);
    else
    {
        sprintf(errMsg, "Data-type of output Data Port %d does not match the specified type (%s).\n",
                port+1, validDTypeNames[parDTypeId]);
        ssSetErrorStatus(S, errMsg);
        return;        
    }
    
    /*  Status port case  */
    if( port == ssGetNumOutputPorts(S) &&
            mxIsLogicalScalarTrue(pa3) )
    {
        if( id == 3 ) /* SS_UINT8 type */
            ssSetOutputPortDataType(S, port, id);
        else
        {
            ssSetErrorStatus(S, "Data-type of Status Port does not match the required type (uint8).\n");
            return;        
        }
        
    }
}
#endif



/* Function: mdlSetDefaultPortDataTypes ============================== */
#if defined(MATLAB_MEX_FILE)
#define MDL_SET_DEFAULT_PORT_DATA_TYPES
void mdlSetDefaultPortDataTypes(SimStruct *S)
{           
    mwIndex n;
    int_T outputNum;
    int_T outputWidth;
    DTypeId outputDTypeId;
    boolean_T showStatusPort;
   
    /*  get block dialog parameters  */
    const mxArray *pa0 = ssGetSFcnParam(S, 0);     /*  Packet Specifier  */
    const mxArray *pa1 = ssGetSFcnParam(S, 1);     /*  COBS Encoding flag */
    const mxArray *pa2 = ssGetSFcnParam(S, 2);     /*  NULL Terminator flag */
    const mxArray *pa3 = ssGetSFcnParam(S, 3);     /*  Status Port show flag */
    const mxArray *pa4 = ssGetSFcnParam(S, 4);     /*  Output mode */
    const mxArray *pa5 = ssGetSFcnParam(S, 5);     /*  Sample time */
    
    /* get show Status Port flag */
    showStatusPort = mxIsLogicalScalarTrue(pa3); 
    
    /*  get number of output data ports  */
    outputNum = ssGetNumOutputPorts(S);
    if(showStatusPort)
        outputNum--;
    
    /* set output Data Ports type (if not already set)*/ 
    for(n=0; n<outputNum; n++)
    {
        parseCellPar( pa0, n, &outputDTypeId, &outputWidth);
        
        if( ssGetOutputPortDataType(S, n) == DYNAMICALLY_TYPED )
            ssSetOutputPortDataType(S, n, outputDTypeId);
    }
    
}
#endif



/* Function: mdlInitializeSampleTimes ================================== */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    /*  get block dialog parameters  */
    const mxArray *pa0 = ssGetSFcnParam(S, 0);     /*  Packet Specifier  */
    const mxArray *pa1 = ssGetSFcnParam(S, 1);     /*  COBS Encoding flag */
    const mxArray *pa2 = ssGetSFcnParam(S, 2);     /*  NULL Terminator flag */
    const mxArray *pa3 = ssGetSFcnParam(S, 3);     /*  Status Port show flag */
    const mxArray *pa4 = ssGetSFcnParam(S, 4);     /*  Output mode */
    const mxArray *pa5 = ssGetSFcnParam(S, 5);     /*  Sample time */

    time_T sampleTime = (time_T)mxGetScalar(pa5);

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


#if defined(MATLAB_MEX_FILE)
#define MDL_RTW
/* Function: mdlRTW ==================================================== */
static void mdlRTW(SimStruct *S)
{   
    mwIndex n;
    int_T outputNum;
    int8_T *outputDTypeId;
    int8_T *outputDTypeSize, *outputWidth;
    uint32_T blockSize, payloadSize, packetSize;
    
    /*  get block dialog parameters  */
    const mxArray *pa0 = ssGetSFcnParam(S, 0);     /*  Packet Specifier  */
    const mxArray *pa1 = ssGetSFcnParam(S, 1);     /*  COBS Encoding flag */
    const mxArray *pa2 = ssGetSFcnParam(S, 2);     /*  NULL Terminator flag */
    const mxArray *pa3 = ssGetSFcnParam(S, 3);     /*  Status Port show flag */
    const mxArray *pa4 = ssGetSFcnParam(S, 4);     /*  Output mode */
    const mxArray *pa5 = ssGetSFcnParam(S, 5);     /*  Sample time */
    
    /*  extract data  */
	boolean_T cobsEncoded = mxIsLogicalScalarTrue(pa1);       
	boolean_T nullTerminated = mxIsLogicalScalarTrue(pa2); 
    boolean_T showStatusPort = mxIsLogicalScalarTrue(pa3); 
    mxUint8 outputMode = (mxUint8) mxGetScalar(pa4); 
    mxDouble sampleTime = (mxDouble) mxGetScalar(pa5);
    
    /*  get number of output data ports  */
    outputNum = ssGetNumOutputPorts(S);
    if(showStatusPort)
        outputNum--;
 
    /*  create vectors with output port width, DType id, DType (byte) size */
    outputWidth = (int8_T *)mxMalloc(outputNum);
    outputDTypeId = (int8_T *)mxMalloc(outputNum);
    outputDTypeSize = (int8_T *)mxMalloc(outputNum);
    
    /*  store data into vectors */
    payloadSize = 0;
    for(n=0; n<outputNum; n++)
    {
        outputWidth[n] = (int8_T)ssGetOutputPortWidth(S, n);
        outputDTypeId[n] = (int8_T)ssGetOutputPortDataType(S, n);
        outputDTypeSize[n] = (int8_T)validDTypeSize[outputDTypeId[n]];
        blockSize = outputWidth[n]*outputDTypeSize[n];
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
            10,  /*  number of parameters to write on model.rtw  */
            SSWRITE_VALUE_DTYPE_NUM, "CobsEncoded", (void *)&cobsEncoded, DTINFO(SS_BOOLEAN, 0),
            SSWRITE_VALUE_DTYPE_NUM, "NullTerminated", (void *)&nullTerminated, DTINFO(SS_BOOLEAN, 0), 
            SSWRITE_VALUE_DTYPE_VECT, "OutputWidth", (void *)outputWidth, outputNum, DTINFO(SS_INT8, 0), 
            SSWRITE_VALUE_DTYPE_VECT, "OutputDTypeId", (void *)outputDTypeId, outputNum, DTINFO(SS_INT8, 0),
            SSWRITE_VALUE_DTYPE_VECT, "OutputDTypeSize", (void *)outputDTypeSize, outputNum, DTINFO(SS_INT8, 0), 
            SSWRITE_VALUE_DTYPE_NUM, "PayloadSize", (void *)&payloadSize, DTINFO(SS_UINT32, 0),
            SSWRITE_VALUE_DTYPE_NUM, "PacketSize", (void *)&packetSize, DTINFO(SS_UINT32, 0), 
            SSWRITE_VALUE_DTYPE_NUM, "ShowStatusPort", (void *)&showStatusPort, DTINFO(SS_BOOLEAN, 0),
            SSWRITE_VALUE_DTYPE_NUM, "OutputMode", (void *)&outputMode, DTINFO(SS_UINT8, 0), 
            SSWRITE_VALUE_DTYPE_NUM, "SampleTime", (void *)&sampleTime, DTINFO(SS_DOUBLE, 0) ) ) 
    {
        ssSetErrorStatus(S, "Unable to write block dialog parameters to model.rtw file.");
        return; /* An error occurred. */
    }     
    
    /* remove data from heap space */
    mxFree(outputWidth);
    mxFree(outputDTypeId);
    mxFree(outputDTypeSize);    
    
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
