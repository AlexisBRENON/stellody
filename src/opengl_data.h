#ifndef __OPENGL_DATA_H__
#define __OPENGL_DATA_H__

#include "camera.h"

struct _OpenGLData
{
	unsigned int CubeMapID;
	unsigned int PlanetTextureID;
	double Ratio;
	int Precision;
	int DispGrid;
};
typedef struct _OpenGLData OpenGLData;

OpenGLData* openGLDataCreate ();
void openGLDataDestroy (OpenGLData** ppData);

void openGLDataInit (OpenGLData* pData);
void openGLDataRelease (OpenGLData* pData);

unsigned int openGLDataGetCubeMapID (OpenGLData const* const pData);
void openGLDataSetCubeMap (OpenGLData* const pData,
							char const* const strFilename);

unsigned int openGLDataGetPlanetTexture (OpenGLData const* const pData);
void openGLDataSetPlanetTexture (OpenGLData* const pData,
								char const* const strFilename);


double openGLDataGetRatio (OpenGLData const* const pData);
void openGLDataSetRatio (OpenGLData* const pData,
						double dWidth, double dHeight);

int openGLDataGetPrecision (OpenGLData const* const pData);
void openGLDataSetPrecision (OpenGLData* const pData,
							int iPrecision);

int openGLDataGetDispGrid (OpenGLData const* const pData);
void openGLDataSetDispGrid (OpenGLData* const pData,
							int iDispGrid);


#endif
