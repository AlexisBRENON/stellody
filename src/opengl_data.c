#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include <gtk/gtk.h>

#if defined(__linux)
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif
#if defined(__APPLE__)
	#include <openGL/gl.h>
	#include <openGL/glu.h>
#endif


#include "opengl_data.h"
#include "camera.h"
#include "image.h"

static unsigned int LoadTexture(char const* pcFileName)
{
    int iDimX, iDimY;
    unsigned char* strData;
    GdkPixbuf* Texture = NULL;
    GError* loadError = NULL;
    unsigned int iTexture ;


    Texture = gdk_pixbuf_new_from_file(pcFileName,
									&loadError);
	if (Texture == NULL)
	{
		fprintf(stderr, "Impossible de charger la texture : %s",
				pcFileName);
		return 0;
	}

    /* Creation de la texture */
    glGenTextures(1, & iTexture) ;

    /* Texture mipmappé */
    glBindTexture(GL_TEXTURE_2D, iTexture) ;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	strData = gdk_pixbuf_get_pixels(Texture);
    iDimY = gdk_pixbuf_get_height(Texture);
	iDimX = gdk_pixbuf_get_width(Texture);
    switch (gdk_pixbuf_get_n_channels(Texture))
    {
    case 1:
        glTexImage2D(GL_TEXTURE_2D,
					0,
					1,
					iDimX,
					iDimY,
					0,
					GL_LUMINANCE,
					GL_UNSIGNED_BYTE,
					strData) ;
        break;
    case 3:
        glTexImage2D(GL_TEXTURE_2D,
					0,
					3,
					iDimX,
					iDimY,
					0,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					strData) ;
        break;
    case 4:
        glTexImage2D(GL_TEXTURE_2D,
					0,
					4,
					iDimX,
					iDimY,
					0,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					strData) ;
        break;
    default:
        fprintf(stderr, "drawingGLLoadTexture : impossible de charger l'image,\
						nombre de couleur non géré!\n");
        break;
    }

	gdk_pixbuf_unref(Texture);

    return iTexture;
}



OpenGLData* openGLDataCreate ()
{
	OpenGLData* Data = NULL;

	Data = (OpenGLData*) malloc(sizeof(OpenGLData));
	openGLDataInit(Data);

	return Data;
}
void openGLDataDestroy (OpenGLData** ppData)
{
	assert (ppData != NULL);
	assert (*ppData != NULL);

	openGLDataRelease(*ppData);
	free(*ppData); *ppData = NULL;
}

void openGLDataInit (OpenGLData* pData)
{
	assert (pData != NULL);

	pData->CubeMapID = 0;
	pData->PlanetTextureID = 0;
	pData->Ratio = 4/3;
	pData->Precision = 10;
	pData->DispGrid = 0;
}
void openGLDataRelease (OpenGLData* pData)
{
	assert (pData != NULL);

	pData->CubeMapID = 0;
	pData->PlanetTextureID = 0;
	pData->Ratio = 4/3;
	pData->Precision = 10;
	pData->DispGrid = 0;
}

unsigned int openGLDataGetCubeMapID (OpenGLData const* const pData)
{
	assert (pData != NULL);

	return pData->CubeMapID;
}
void openGLDataSetCubeMap (OpenGLData* const pData,
							char const* const strFilename)
{
	assert (pData != NULL);

	if (pData->CubeMapID != 0)
	{
		glDeleteTextures(1, &(pData->CubeMapID));
	}

	pData->CubeMapID = LoadTexture(strFilename);
}
unsigned int openGLDataGetPlanetTexture(OpenGLData const* const pData)
{
	assert (pData != NULL);

	return pData->PlanetTextureID;
}
void openGLDataSetPlanetTexture (OpenGLData* const pData,
								char const* const strFilename)
{
	assert (pData != NULL);

	if (pData->PlanetTextureID != 0)
	{
		glDeleteTextures(1, &(pData->PlanetTextureID));
	}

	pData->PlanetTextureID = LoadTexture(strFilename);
}

double openGLDataGetRatio (OpenGLData const* const pData)
{
	assert (pData != NULL);

	return pData->Ratio;
}
void openGLDataSetRatio (OpenGLData* const pData,
						double dWidth, double dHeight)
{
	double dRatio = 0;

	assert (pData != NULL);
	assert (dHeight > 0);

	dRatio = dWidth/dHeight;
	pData->Ratio = dRatio;
	glViewport(0, 0, dWidth, dHeight);

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
    gluPerspective(70,
				pData->Ratio,
				0.001,
				1000);
}
int openGLDataGetPrecision (OpenGLData const* const pData)
{
	assert (pData != NULL);

	return pData->Precision;
}
void openGLDataSetPrecision (OpenGLData* const pData,
							int iPrecision)
{
	assert (pData != NULL);
	assert (iPrecision >= 3 && iPrecision <= 30);

	pData->Precision = iPrecision;
}

int openGLDataGetDispGrid (OpenGLData const* const pData)
{
	assert (pData != NULL);

	return pData->DispGrid;
}
void openGLDataSetDispGrid (OpenGLData* const pData,
							int iDispGrid)
{
	assert (pData != NULL);
	assert (iDispGrid == 0 || iDispGrid == 1);

	pData->DispGrid = iDispGrid;
}

