
/*
 * cellsize.c 
   $Log: cellsize.c,v $
   Revision 2.0  1996/05/23 13:16:26  cees
   csf2clean

   Revision 1.1  1996/05/23 13:11:49  cees
   Initial revision

   Revision 1.2  1995/11/01 17:23:03  cees
   .

 * Revision 1.1  1994/09/08  17:16:23  cees
 * Initial revision
 *
 */
#ifndef lint  
static const char *rcs_id = 
 "$Header: /home/pcrtree/SRC.RCS/libs/csf/RCS/cellsize.c,v 2.0 1996/05/23 13:16:26 cees Exp $";
#endif

/********/
/* USES */
/********/

/* libs ext. <>, our ""  */
#include "csf.h"
#include "csfimpl.h"

/* global header (opt.) and cellsize's prototypes "" */


/* headers of this app. modules called */ 

/***************/
/* EXTERNALS   */
/***************/

/**********************/ 
/* LOCAL DECLARATIONS */
/**********************/ 

/*********************/ 
/* LOCAL DEFINITIONS */
/*********************/ 

/******************/
/* IMPLEMENTATION */
/******************/


/* get cell size 
 * returns the cell size or -1 in case of an error
 *
 * Merrno
 *  ILL_CELLSIZE
 *  ILLHANDLE
 */
REAL8 RgetCellSize(
	const MAP *map) /* map handle */
{
	CHECKHANDLE(map);
	if ( map->raster.cellSizeX != map->raster.cellSizeY)
	{
		M_ERROR(ILL_CELLSIZE);
		return -1;
	}

	return(map->raster.cellSizeX);
}

/* put cell size
 * returns the new cell size or -1
 * in case of an error.
 *
 * Merrno
 * ILLHANDLE
 * NOACCESS
 * ILL_CELLSIZE
 */
REAL8 RputCellSize(
	MAP *map, /* map handle */
	REAL8 cellSize) /* new cell size */
{
	CHECKHANDLE_GOTO(map, error);
	if(! WRITE_ENABLE(map))
	{
		M_ERROR(NOACCESS);
		goto error;
	}
	if (cellSize <= 0.0)
	{
		M_ERROR(ILL_CELLSIZE);
		goto error;
	}
	map->raster.cellSizeX = cellSize;
	map->raster.cellSizeY = cellSize;
	return(cellSize);
error:  return(-1.0);
}
