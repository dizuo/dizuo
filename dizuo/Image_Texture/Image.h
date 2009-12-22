/****************************************************************************
*                   image.h
*
*  This module contains all defines, typedefs, and prototypes for IMAGE.CPP.
*
*  from Persistence of Vision(tm) Ray Tracer
*  Copyright 1996-2002 Persistence of Vision Team
*---------------------------------------------------------------------------
*  NOTICE: This source code file is provided so that users may experiment
*  with enhancements to POV-Ray and to port the software to platforms other
*  than those supported by the POV-Ray Team.  There are strict rules under
*  which you are permitted to use this file.  The rules are in the file
*  named POVLEGAL.DOC which should be distributed with this file.
*  If POVLEGAL.DOC is not available it may be found online at -
*
*    http://www.povray.org/povlegal.html.
*
* This program is based on the popular DKB raytracer version 2.12.
* DKBTrace was originally written by David K. Buck.
* DKBTrace Ver 2.0-2.12 were written by David K. Buck & Aaron A. Collins.
*
*##############################################################################
*  This file is part of MegaPOV, which is a modified and unofficial version of POV-Ray
*
* $RCSfile: image.h,v $
* $Revision: 1.3 $
* $Author: smellenbergh $
* $Log: image.h,v $
* Revision 1.3  2002/12/07 16:48:32  smellenbergh
* Adds listed pattern, displacement warp
* Header CVS keywords fix
*
* Revision 1.2  2002/11/14 12:15:55  abx
* PoPOV 0.3 core + windows
*
*
*****************************************************************************/


#ifndef IMAGE_H
#define IMAGE_H

#include "hfield.h"

/*****************************************************************************
* Global preprocessor defines
******************************************************************************/

/* Image/Bump Map projection types. */

#define PLANAR_MAP      0
#define SPHERICAL_MAP   1
#define CYLINDRICAL_MAP 2
#define PARABOLIC_MAP   3
#define HYPERBOLIC_MAP  4
#define TORUS_MAP       5
#define PIRIFORM_MAP    6
#define OLD_MAP         7

/* Bit map interpolation types. */

#define NO_INTERPOLATION 0
#define NEAREST_NEIGHBOR 1
#define BILINEAR         2
#ifdef BICUBIC_INTERPOLATION_PATCH
#define BICUBIC     3
#else
#define CUBIC_SPLINE     3
#endif
#define NORMALIZED_DIST  4


/* NK 1998 - instead of Use_Colour_Flag */
#define USE_INDEX  0
#define USE_COLOUR 1
#define USE_ALPHA  2
/* USE_IMAGE = TRUE = 1
USE_INDEX = FALSE = 0 */
/* NK ---- */


/*****************************************************************************
* Global typedefs
******************************************************************************/




/*****************************************************************************
* Global variables
******************************************************************************/




/*****************************************************************************
* Global functions
******************************************************************************/

DBL image_pattern (VECTOR EPoint, TPATTERN *TPattern);
int image_map (VECTOR EPoint, PIGMENT *Pigment, COLOUR colour);
TEXTURE *material_map (VECTOR IPoint, TEXTURE *Texture);
void bump_map (VECTOR EPoint, TNORMAL *Tnormal, VECTOR normal);
HF_VAL image_height_at(IMAGE *Image, int x, int y);
bool is_image_opaque(IMAGE *Image);
IMAGE *Copy_Image (IMAGE *Old);
IMAGE *Create_Image (void);
void Destroy_Image (IMAGE *Image);

#endif
