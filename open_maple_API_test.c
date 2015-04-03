    /* ***********************************************************************
     * OpenMaple Example Program
     * Copyright (c) Maplesoft, a division of Waterloo Maple Inc. 2003
     * All rights reserved.
     *
     * This example program illustrates how to use the OpenMaple API
     * to initialize the Maple kernel and compute with it.
     * Users are encouraged to use and modify  this code as a starting
     * point for learning the OpenMaple API. 
     *
     *********************************************************************** */
   
    #include <stdio.h>
    #include <stdlib.h>
   
    #include "/opt/maplesoft/maple9.5/extern/include/maplec.h"
    #include "/opt/maplesoft/maple9.5/extern/include/mplshlib.h"
    #include "/opt/maplesoft/maple9.5/extern/include/mpltable.h"
/*
  typedef struct {
      void (M_DECL *textCallBack)( void *data, int tag, char *output );
     
      void (M_DECL *errorCallBack)( void *data, M_INT offset,
                                    char *msg );
     
      void (M_DECL *statusCallBack)( void *data, long kilobytesUsed,
                                     long kilobytesAlloc, double cpuTime );
                             
      char * (M_DECL *readLineCallBack)( void *data, M_BOOL debug );
     
      M_BOOL (M_DECL *redirectCallBack)( void *data, char *name,
                                         char *mode );
                                 
      char * (M_DECL *streamCallBack)( void *data, char *name,
                                       M_INT nargs, char **args );
     
      M_BOOL (M_DECL *queryInterrupt)( void *data );
      char * (M_DECL *callBackCallBack)( void *data, char *output );
  } MCallBackVector, *MCallBack;
*/
   
    /* callback used for directing result output */
    static void M_DECL textCallBack( void *data, int tag, char *output )
    {
        printf("%s\n",output);
    }
   
    int main( int argc, char *argv[] )
    {
        char err[2048];  /* command input and error string buffers */
        MKernelVector kv;  /* Maple kernel handle */
        MCallBackVectorDesc cb = {  textCallBack,
                                    0,   /* errorCallBack not used */
                                    0,   /* statusCallBack not used */
                                    0,   /* readLineCallBack not used */
                                    0,   /* redirectCallBack not used */
                                    0,   /* streamCallBack not used */
                                    0,   /* queryInterrupt not used */
                                    0    /* callBackCallBack not used */
                                };
        ALGEB r, l;  /* Maple data-structures */
   
        /* initialize Maple */
        if( (kv=StartMaple(argc,argv,&cb,NULL,NULL,err)) == NULL ) {
            printf("Fatal error, %s\n",err);
            return( 1 );
        }
    
        /* example 1: find out where Maple is installed */
        r = MapleKernelOptions(kv,"mapledir",NULL);
        if( IsMapleString(kv,r) )
            printf("Maple directory = \"%s\"\n\n",MapleToString(kv,r));
   
        /* example 2: compute an integral */
        /* output goes through the textCallBack */
        printf("Evaluate an integral: \n\t");
        r = EvalMapleStatement(kv,"int(1/(x^4+1),x);");
   
        /* example 3: assign x a value and reevaluate the integral */
        MapleAssign(kv,
            ToMapleName(kv,"x",TRUE),
            ToMapleInteger(kv,0));
        r = MapleEval(kv,r);
        MapleALGEB_Printf(kv,"\nEvaluated at x=0, the integral is: %a\n",r);
   
        /* example 4: create a list with 3 elements */
        l = MapleListAlloc(kv,3);
        MapleListAssign(kv,l,1,r);
        MapleListAssign(kv,l,2,ToMapleBoolean(kv,1));
        MapleListAssign(kv,l,3,ToMapleFloat(kv,3.14));
        MapleALGEB_Printf(kv,"\nHere is the list: %a\n",l);
   
        return( 0 );
    }
