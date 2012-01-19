#include <stdlib.h>
#include <assert.h>
#include <math.h>

#if defined(__linux)
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#if defined(__APPLE__)
#include <openGL/gl.h>
#include <openGL/glu.h>
#endif

#include "3ddisplay.h"
#include "opengl_data.h"
#include "camera.h"
#include "dynamic_array.h"
#include "3dvector.h"
#include "analyzed_track.h"

#define MAP_RADIUS 300
#define MAP_FILENAME "data/images/cubemap.ppm"
#define PLANET_FILENAME "data/images/jupiter.jpg"

static void displayDraw (Display const* const pDisp,
                         int Type)
{
    int i;
    GLUquadric* Params = NULL;

    Params = gluNewQuadric();

    if (Type == GL_RENDER)
        /* Creation du CubeMap */
    {
        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, openGLDataGetCubeMapID(pDisp->GLData));
        gluQuadricTexture(Params, GL_TRUE);
        glColor3f(1, 1, 1);
        gluSphere (Params, MAP_RADIUS,
                   openGLDataGetPrecision(pDisp->GLData),
                   openGLDataGetPrecision(pDisp->GLData));

        glDisable(GL_TEXTURE_2D);
    }

    if (openGLDataGetDispGrid(pDisp->GLData) == 1 &&
            Type == GL_RENDER)
        /* Création de la grille */
    {
        glEnable(GL_BLEND) ;
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
        glColor4f(1, 1, 1, 0.05);
        for (i = -10; i <= 10; i = i+2)
        {
            glBegin(GL_LINES);

            glVertex3d(i, 0, 10);
            glVertex3d(i, 0, -10);
            glVertex3d(-10, 0, i);
            glVertex3d(10, 0, i);

            glVertex3d(i, 10, 0);
            glVertex3d(i, -10, 0);
            glVertex3d(-10, i, 0);
            glVertex3d(10, i, 0);

            glVertex3d(0, i, 10);
            glVertex3d(0, i, -10);
            glVertex3d(0, -10, i);
            glVertex3d(0, 10, i);

            glEnd();
        }
        glDisable(GL_BLEND);
    }

    /*  	dynamicArrayGetSize(pDisp->Stars, &iNbStar);
          for (i = 0; i < iNbStar; i++)
          {
      		glPushMatrix();
      		glLoadIdentity();

      		if (Type == GL_SELECT)
      		{
      			glPushName(i);
      		}
      		dynamicArrayGet(pDisp->Stars, i, (void**) &StarPosition);
      		glTranslatef(vectorGetX(StarPosition),
      					vectorGetY(StarPosition),
      					vectorGetZ(StarPosition));
      		glColor3ub(255, 255, 255);
      		gluSphere(Params,
      					1,
      					openGLDataGetPrecision(pDisp->GLData),
      					openGLDataGetPrecision(pDisp->GLData));

      		if (Type == GL_SELECT)
      		{
      			glPopName();
      		}

      		glPopMatrix();
          }

          if (pDisp->PlayingStar != NULL &&
      		Type == GL_RENDER)
      	{
      		glPushMatrix();
      		glLoadIdentity();

      		gluQuadricDrawStyle(Params, GLU_POINT);
      		glTranslatef(vectorGetX(pDisp->PlayingStar),
      					vectorGetY(pDisp->PlayingStar),
      					vectorGetZ(pDisp->PlayingStar));
      		gluSphere(Params,
      					1,
      					openGLDataGetPrecision(pDisp->GLData),
      					openGLDataGetPrecision(pDisp->GLData));
      		glPopMatrix();
      	}*/

    if (Type == GL_RENDER)
        /* Création du bulbe central */
    {
        glEnable(GL_BLEND) ;
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
        for (i = 0; i <= 50; i++)
        {
            float Alpha = pow(1.0-((float)i/100.0), 7.0);
            glColor4f(1.0f, 1.0f, 1.0f, Alpha);
            gluSphere(Params,
                      (float)i/10,
                      openGLDataGetPrecision(pDisp->GLData),
                      openGLDataGetPrecision(pDisp->GLData));
        }
        glDisable(GL_BLEND) ;
    }
    else /* GL_SELECT */
        /* Création du bulbe simplifié */
    {
        glColor3f(1.0, 1.0, 1.0);
        gluSphere(Params,
                  4,
                  3,
                  3);
    }

    glFlush();
}

Display* displayCreate(int iWidth, int iHeight,
                       int iPrecision, int iDispGrid)
{
    Display* pDisp = NULL;

    assert(iWidth > 0);
    assert(iHeight > 0);
    assert(iPrecision > 2);
    assert (iDispGrid == 0 || iDispGrid == 1);

    pDisp = (Display*) malloc(sizeof(Display));

    displayInit(pDisp,
                iWidth, iHeight,
                iPrecision, iDispGrid);

    return pDisp;
}
void displayDestroy(Display** ppDisp)
{
    assert (ppDisp != NULL);
    assert (*ppDisp != NULL);

    displayRelease(*ppDisp);
    free(*ppDisp);
    *ppDisp = NULL;
}

void displayInit (Display* pDisp,
                  int iWidth, int iHeight,
                  int iPrecision, int iDispGrid)
{
    assert (pDisp != NULL);
    assert(iWidth > 0);
    assert(iHeight > 0);
    assert(iPrecision > 2);
    assert (iDispGrid == 0 || iDispGrid == 1);

    pDisp->GLData = openGLDataCreate();
    pDisp->Cam = cameraCreateWithCoord(0.0, -10.0, 0.0);
    pDisp->Stars = dynamicArrayCreate();
    pDisp->PlayingStar = NULL;

    openGLDataSetRatio(pDisp->GLData, iWidth, iHeight);
    openGLDataSetPrecision(pDisp->GLData, iPrecision);
    openGLDataSetDispGrid(pDisp->GLData, iDispGrid);
    openGLDataSetCubeMap(pDisp->GLData, MAP_FILENAME);

    cameraSetSpereMapRadius(pDisp->Cam, MAP_RADIUS);

    glClearColor(0, 1, 0, 0);
    glEnable(GL_DEPTH_TEST) ;
}
void displayRelease(Display* pDisp)
{
    assert (pDisp != NULL);

    openGLDataDestroy(&(pDisp->GLData));
    cameraDestroy(&(pDisp->Cam));
    dynamicArrayDestroy(&(pDisp->Stars));
}
void displayReinit(Display* pDisp)
{
	cameraRelease(pDisp->Cam);
	cameraInitWithCoord(pDisp->Cam, 0.0, -10.0, 0.0);
	cameraSetSpereMapRadius(pDisp->Cam, MAP_RADIUS);
}

void displayResize(Display* pDisp,
                   int iWidth, int iHeight)
{
    assert (pDisp != NULL);
    assert(iWidth > 0);
    assert(iHeight > 0);

    openGLDataSetRatio(pDisp->GLData, iWidth, iHeight);
}
void displayTranslate(Display* pDisp,
                      double dXTranslate, double dYTranslate)
{
    assert (pDisp != NULL);

    cameraTranslate(pDisp->Cam,
                    dXTranslate, dYTranslate);
}
void displayRotate(Display* pDisp,
                   double dXTranslate, double dYTranslate)
{
    assert (pDisp != NULL);

    cameraRotate(pDisp->Cam,
                 dXTranslate, dYTranslate);
}
void displayZoom(Display* pDisp,
                 char bDirection,
                 double dXDirection,double dYDirection)
{
    assert (pDisp != NULL);
    assert (bDirection != 0);

    cameraZoom(pDisp->Cam,
               bDirection,
               dXDirection, dYDirection);
}

void displayDisp (Display const* pDisp)
{
    assert (pDisp != NULL);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    cameraLook(pDisp->Cam);
    displayDraw(pDisp, GL_RENDER);
}
int displaySelect (Display const* pDisp,
                   int iXPosition, int iYPosition)
{
    unsigned int HitBuffer[4] = {0};
    int Viewport[4] = {0};

    assert (pDisp != NULL);

    glSelectBuffer(4, HitBuffer);
    glRenderMode(GL_SELECT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glGetIntegerv(GL_VIEWPORT, Viewport);
    gluPickMatrix(iXPosition,
                  Viewport[3] - iYPosition,
                  5, 5, Viewport);
    gluPerspective(70,
                   openGLDataGetRatio(pDisp->GLData),
                   0.001, 1000);
    glMatrixMode(GL_MODELVIEW);
    glInitNames();

    displayDraw(pDisp, GL_SELECT);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glFlush();

    if (glRenderMode(GL_RENDER) == 1)
    {
    	if (HitBuffer[0] == 1)
    	{
    		return HitBuffer[3];
    	}
    }

    return -1;
}

void displayAddStar (Display* pDisp,
                     AnalyzedTrack const* pTrack)
{
    Vector* NewStarCoord = NULL;
    int TID = 0;

    assert (pDisp != NULL);
    assert (pTrack != NULL);

    TID = analyzedTrackGetTID(pTrack);
    NewStarCoord = vectorCreate(TID, TID, TID);

    dynamicArrayPush(pDisp->Stars, NewStarCoord);
}
void displayRemoveStar (Display* pDisp,
                        AnalyzedTrack const* pTrack)
{
    int TID = 0;

    assert (pDisp != NULL);
    assert (pTrack != NULL);

    TID = analyzedTrackGetTID(pTrack);
    dynamicArrayRemove(pDisp->Stars, TID);
}
void displayRemoveStarByID (Display* pDisp,
                            int iTID)
{
    assert (pDisp != NULL);
    dynamicArrayRemove(pDisp->Stars, iTID);
}


void displaySetPrecision (Display* pDisp,
						int iPrecision)
{
	assert (pDisp != NULL);
	assert (iPrecision > 2 && iPrecision < 31);

	openGLDataSetPrecision(pDisp->GLData, iPrecision);
}
void displaySetDispGrid (Display* pDisp,
						int DispGrid)
{
	assert (pDisp != NULL);
	assert (DispGrid == 0 || DispGrid == 1);

	openGLDataSetDispGrid(pDisp->GLData, DispGrid);
}
void displaySetPlayedTrack (Display* pDisp,
                            AnalyzedTrack const* pTrack)
{
    int TID = 0;

    assert (pDisp != NULL);
    assert (pTrack != NULL);

    TID = analyzedTrackGetTID(pTrack);
    assert (dynamicArrayGet(pDisp->Stars,
                            TID,
                            (void**) &(pDisp->PlayingStar)) == ARRAY_OK);
}











