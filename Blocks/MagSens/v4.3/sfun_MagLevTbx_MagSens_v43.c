/*
 * Riccardo Antonello (riccardo.antonello@unipd.it)
 * 
 * March 25, 2026
 *
 * Dept. of Information Engineering, University of Padova 
 *
 */

#define S_FUNCTION_NAME  sfun_MagLevTbx_MagSens_v43
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

#define NUM_INPUTS          0
#define NUM_OUTPUTS         1
#define NUM_PARAMS          1
#define NUM_CONT_STATES     0
#define NUM_DISC_STATES     0

/* Output Port  0 (XYZ readouts) */
#define OUTPUT_0_WIDTH      3
#define OUTPUT_0_DTYPE      SS_DOUBLE
#define OUTPUT_0_COMPLEX	COMPLEX_NO


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

    /* set output ports properties */
    ssSetOutputPortWidth(S, 0, OUTPUT_0_WIDTH);
    ssSetOutputPortDataType(S, 0, OUTPUT_0_DTYPE);
    ssSetOutputPortComplexSignal(S, 0, OUTPUT_0_COMPLEX);
    
    /* set number of states */
    ssSetNumContStates(S, NUM_CONT_STATES);
    ssSetNumDiscStates(S, NUM_DISC_STATES);

    /* set elementary work vectors */
    ssSetNumSampleTimes(S, 1);
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
