/*
 * kernlcsf.c
 *    Functions to create  and maintain the csf-kernel
 *     runtime structures
 */
#ifndef lint
 static const char *rcs_id = 
 "$Header: /home/pcrtree/SRC.RCS/libs/csf/RCS/kernlcsf.c,v 2.2 1997/03/14 10:04:28 cees Exp $";
#endif


/**************************************************************************/
/*  KERNLCSF.C                                                            */ 
/*                         						  */
/*                                                                        */
/*                                                                        */
/**************************************************************************/

/********/
/* USES */
/********/
#include "csf.h" 
#include "csfimpl.h" 

#include <stdlib.h>

/***************/
/* EXTERNALS   */
/***************/

static MAP **mapList = NULL;

/*********************/ 
/* LOCAL DEFINITIONS */
/*********************/ 

/**********************/ 
/* LOCAL DECLARATIONS */
/**********************/ 

/******************/
/* IMPLEMENTATION */
/******************/


/* close all open maps at exit  (LIBRARY_INTERNAL)
 * passed through atexit to c-library
 * exit code
 */
static void CsfCloseCsfKernel(void)
{
	int i;
	
	for(i = 0; i < MAX_MAPS; i++)
	 if(mapList[i] != NULL)
		if(Mclose(mapList[i]))
			(void)fprintf(stderr,"CSF_INTERNAL_ERROR: unable to close %s at exit\n",
				mapList[i]->fileName);
	CSF_FREE(mapList);
	mapList = NULL;
}

/* boot the CSF runtime library (LIBRARY_INTERNAL)
 * CsfBootCsfKernel creates the mapList and adds the function to 
 *  close all files at a system exit 
 * 
 * NOTE
 * Note that CsfBootCsfKernel never returns if there isn't enough
 * memory to allocate an array of MAX_MAPS pointers, or if 
 * the atexit() call fails
 */
void CsfBootCsfKernel(void)
{
	int i;

	POSTCOND(mapList == NULL);

	mapList = (MAP **)CSF_MALLOC(MAX_MAPS * sizeof(MAP *));
	if (mapList == NULL)
	{
		(void)fprintf(stderr,"CSF_INTERNAL_ERROR: Not enough memory to use CSF-files\n");
		exit(1);
	}

	for(i = 0; i < MAX_MAPS; i++)
		mapList[i] = NULL;

	if (atexit(CsfCloseCsfKernel))
	{
		(void)fprintf(stderr,"CSF_INTERNAL_ERROR: Impossible to close CSF-files automatically at exit\n");
		exit(1);
	}
}

/* check if the kernel is booted (LIBRARY_INTERNAL)
 * returns 0 if not, nonzero if already booted 
 */
int CsfIsBootedCsfKernel(void)
{
	return(mapList != NULL);
}

/* put map in run time structure (LIBRARY_INTERNAL)
 * Every map opened or created is
 * registered in a list for verification
 * if functions get a valid map handle
 * passed and for automatic closing
 * at exit if the application forgets it.
 */
void CsfRegisterMap(
	MAP *m) /* map handle to be registered, the field m->mapListId is
	         * initialized
	         */
{
	int i=0;

	while (mapList[i] != NULL && i < MAX_MAPS) 	
		i++;

	if(i == MAX_MAPS)
	{
	        /* this should never happen if MAX_MAPS is 
	         * greater or equal to the FOPEN_MAX
	         * since an fopen() should already have
	         * failed before registering.
	         */ 
		(void)fprintf(stderr,
		 "CSF_INTERNAL_ERROR: unable to register map (MAX_MAPS=%u)\n", MAX_MAPS);
		exit(1);
	}

	mapList[i] =   m;
	m->mapListId = i;
}

/* remove map from run time structure (LIBRARY_INTERNAL)
 * The map handle will become invalid. 
 */ 
void CsfUnloadMap(
	MAP *m) /* map handle */
{
	POSTCOND(CsfIsValidMap(m));
	
	mapList[m->mapListId] = NULL;
	m->mapListId = -1;
}
	
/* check if the map handle is created via the csf kernel (LIBRARY_INTERNAL)
 */
int CsfIsValidMap(
	const MAP *m) /* map handle */
{
	return(CsfIsBootedCsfKernel() && m != NULL 
	        && m->mapListId >= 0 && m->mapListId < MAX_MAPS
		&& mapList[m->mapListId] == m);
}
