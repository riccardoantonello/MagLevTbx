/*
 * Riccardo Antonello (riccardo.antonello@unipd.it)
 * 
 * February 26, 2026
 *
 * Dept. of Information Engineering, University of Padova 
 *
 */

#define S_FUNCTION_NAME  sfun_MagLevTbx_UserLED
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

#define NUM_INPUTS          1
#define NUM_OUTPUTS         0
#define NUM_PARAMS          1
#define NUM_CONT_STATES     0
#define NUM_DISC_STATES     0
#define NUM_DWORKS          0

/* Input Port  0 */
#define INPUT_0_WIDTH       1
#define INPUT_0_DTYPE       SS_BOOLEAN
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

    /* ----------------------------------------------------------------- *
     *  1st parameter check (Sample time)
     * ----------------------------------------------------------------- */
    if( mxIsEmpty(pa0) ||
       !mxIsScalar(pa0) ||
       !mxIsNumeric(pa0) ||
       mxIsComplex(pa0) )
    {
        ssSetErrorStatus(S, "1st parameter must be a valid sample time (positive scalar, or -1).");
        return;
    }

    mxDouble *dt0 = mxGetDoubles(pa0);
    if( *dt0 < 0 && *dt0 != -1 )
    {
        ssSetErrorStatus(S, "1st parameter must be a valid sample time (positive scalar, or -1).");
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
        ssSetErrorStatus(S, "Invalid number of parameters (1 parameter required: sample time).");
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
    const mxArray *pa0 = ssGetSFcnParam(S, 0);     /*  Sample time  */
    time_T sampleTime = (time_T)mxGetScalar(pa0);

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
    /*  get params  */
    mxDouble sampleTime = (mxDouble) mxGetScalar(ssGetSFcnParam(S, 0)); 
  
    /* write ParamSettings to rtw file */
    if(!ssWriteRTWParamSettings(S, 
            1,  /*  number of parameters to write on model.rtw  */
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
