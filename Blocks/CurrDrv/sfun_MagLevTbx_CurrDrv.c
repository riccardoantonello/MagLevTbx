/*
 * Riccardo Antonello (riccardo.antonello@unipd.it)
 * 
 * February 16, 2026
 *
 * Dept. of Information Engineering, University of Padova 
 *
 */

#define S_FUNCTION_NAME  sfun_MagLevTbx_CurrDrv
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

#define NUM_INPUTS          1
#define NUM_OUTPUTS         0
#define NUM_PARAMS          2
#define NUM_CONT_STATES     0
#define NUM_DISC_STATES     0
#define NUM_DWORKS          0

/* Input Port  0 */
#define INPUT_0_WIDTH       1
#define INPUT_0_DTYPE       SS_INT16
#define INPUT_0_COMPLEX     COMPLEX_NO
#define INPUT_0_FEEDTHROUGH 1


/*====================*
 * S-function methods *
 *====================*/

/* Function: mdlCheckParameters ======================================== */
#define MDL_CHECK_PARAMETERS
#if defined(MDL_CHECK_PARAMETERS) && defined(MATLAB_MEX_FILE)
static void mdlCheckParameters(SimStruct *S)
{
    /* get block dialog parameters */
    const mxArray *pa0 = ssGetSFcnParam(S, 0);
    const mxArray *pa1 = ssGetSFcnParam(S, 1);
            
    /* ----------------------------------------------------------------- *
     *  1st parameter check (Current driver number)
     * ----------------------------------------------------------------- */
    if( mxIsEmpty(pa0) ||
       !mxIsScalar(pa0) ||
       !mxIsNumeric(pa0) ||
       mxIsComplex(pa0) )
    {
        ssSetErrorStatus(S, "1st parameter must be a valid current driver number (0..3).");
        return;
    }

    mxDouble *dt0 = mxGetDoubles(pa0);
    if( *dt0 < 0 || *dt0 > 3 || *dt0 != (int)(*dt0) )
    {
        ssSetErrorStatus(S, "1st parameter must be a valid current driver number (0..3).");
        return;
    }

    /* ----------------------------------------------------------------- *
     *  2nd parameter check (Sample time)
     * ----------------------------------------------------------------- */
    if( mxIsEmpty(pa1) ||
       !mxIsScalar(pa1) ||
       !mxIsNumeric(pa1) ||
       mxIsComplex(pa1) )
    {
        ssSetErrorStatus(S, "2nd parameter must be a valid sample time (positive scalar, or -1).");
        return;
    }

    mxDouble *dt1 = mxGetDoubles(pa1);
    if( *dt1 < 0 && *dt1 != -1 )
    {
        ssSetErrorStatus(S, "2nd parameter must be a valid sample time (positive scalar, or -1).");
        return;
    }      
}
#endif



/* Function: mdlInitializeSizes ======================================== */
static void mdlInitializeSizes(SimStruct *S)
{
    int_T n;

    /* check for expected number of parameters */
    ssSetNumSFcnParams(S, NUM_PARAMS);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S))
    {
        /* Return if number of expected != number of actual parameters */
        ssSetErrorStatus(S, "Invalid number of parameters (2 parameters required: current driver id, sample time).");
        return;
    }

    /* set parameters as non-tunable */
    for(n = 0; n < NUM_PARAMS; n++)
        ssSetSFcnParamTunable(S, n, 0);
    
    /* check parameters */
    mdlCheckParameters(S);

    /* set number of input ports */
    if (!ssSetNumInputPorts(S, NUM_INPUTS)) 
        return;

    /* set number of output ports */
    if (!ssSetNumOutputPorts(S, NUM_OUTPUTS))
        return;
    
    /* set input ports properties */
    ssSetInputPortWidth(S, 0, INPUT_0_WIDTH);
    ssSetInputPortDataType(S, 0, INPUT_0_DTYPE);
    ssSetInputPortComplexSignal(S, 0, INPUT_0_COMPLEX);
    ssSetInputPortDirectFeedThrough(S, 0, INPUT_0_FEEDTHROUGH);
    ssSetInputPortRequiredContiguous(S, 0, 1); /*direct input signal access*/
    
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

    /* Specify the sim state compliance to be same as a built-in block */
    ssSetSimStateCompliance(S, USE_DEFAULT_SIM_STATE);

    ssSetOptions(S, 0);
}



/* Function: mdlInitializeSampleTimes ================================== */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    const mxArray *pa1 = ssGetSFcnParam(S, 1);     /*  Sample time  */
    time_T sampleTime = (time_T)mxGetScalar(pa1);

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
    /*  get params  */
    mxUint8 driverId = (mxUint8) mxGetScalar(ssGetSFcnParam(S, 0));
    mxDouble sampleTime = (mxDouble) mxGetScalar(ssGetSFcnParam(S, 1)); 
  
    /* write ParamSettings to rtw file */
    if(!ssWriteRTWParamSettings(S, 
            2,  /*  number of parameters to write on model.rtw  */
            SSWRITE_VALUE_DTYPE_NUM, "driverId", (void *)&driverId, DTINFO(SS_UINT8, 0),
            SSWRITE_VALUE_DTYPE_NUM, "sampleTime", (void *)&sampleTime, DTINFO(SS_DOUBLE, 0) ) ) 
    {
        ssSetErrorStatus(S, "Unable to write block dialog parameters to model.rtw file.");
        return; /* An error occurred. */
    } 
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
