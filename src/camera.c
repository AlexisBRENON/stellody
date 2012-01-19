#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#ifndef M_PI
	#define M_PI 3.14159
#endif

#if defined(__linux)
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif
#if defined(__APPLE__)
	#include <openGL/gl.h>
	#include <openGL/glu.h>
#endif

#include "camera.h"
#include "3dvector.h"


#define SPEED 0.01
#define TRANS_SENSIVITY 0.01
#define ROT_SENSIVITY 0.5

static void cameraVectorsFromAngles (Camera* const pCam)
{
	Vector Vertical;
	double dRTemp;

	assert (pCam != NULL);

	if (pCam->_phi > 89)
	{
		pCam->_phi = 89;
	}
	else if (pCam->_phi < -89)
	{
		pCam->_phi = -89;
	}

	dRTemp = cos(pCam->_phi * M_PI/180);
	(pCam->_forward).z = sin(pCam->_phi * M_PI/180);
	(pCam->_forward).x = dRTemp*cos(pCam->_theta * M_PI/180);
	(pCam->_forward).y = dRTemp*sin(pCam->_theta * M_PI/180);

	vectorInit(&Vertical, 0, 0, 1);
	pCam->_left = vectorCrossProduct(&Vertical, &(pCam->_forward));
	vectorNormalize(&(pCam->_left));

	pCam->_target = vectorSum(&(pCam->_position), &(pCam->_forward));
}

Camera* cameraCreate ()
{
	Camera* pCam = NULL;

	pCam = (Camera*) malloc(sizeof(Camera));
	cameraInit(pCam);

	return pCam;
}
Camera* cameraCreateWithCoord (double dX, double dY, double dZ)
{
	Camera* pCam = NULL;

	pCam = (Camera*) malloc(sizeof(Camera));
	cameraInitWithCoord(pCam,
						dX, dY, dZ);

	return pCam;
}
Camera* cameraCreateWithVector (Vector const* const pVect)
{
	Camera* pCam = NULL;

	pCam = (Camera*) malloc(sizeof(Camera));
	cameraInitWithVector(pCam,
						pVect);

	return pCam;
}
void cameraDestroy (Camera** ppCam)
{
	assert (ppCam != NULL);
	assert (*ppCam != NULL);

	free(*ppCam);
	*ppCam = NULL;
}

void cameraInit (Camera* pCam)
{
	assert (pCam != NULL);

	vectorInit(&(pCam->_position), 0.0, 0.0, 0.0);
	pCam->_phi = 0;
	pCam->_theta = 90;
	cameraVectorsFromAngles(pCam);

	pCam->_speed = SPEED;
	pCam->_transSensivity = TRANS_SENSIVITY;
	pCam->_rotSensivity = ROT_SENSIVITY;
}
void cameraInitWithCoord (Camera* pCam,
						double dX, double dY, double dZ)
{
	assert (pCam != NULL);

	vectorInit(&(pCam->_position), dX, dY, dZ);
	pCam->_phi = 0;
	pCam->_theta = 90;
	cameraVectorsFromAngles(pCam);

	pCam->_speed = SPEED;
	pCam->_transSensivity = TRANS_SENSIVITY;
	pCam->_rotSensivity = ROT_SENSIVITY;
}
void cameraInitWithVector (Camera* pCam,
						Vector const* const pVect)
{
	assert (pCam != NULL);

	pCam->_position = vectorCopy(pVect);
	pCam->_phi = 0;
	pCam->_theta = 90;
	cameraVectorsFromAngles(pCam);

	pCam->_speed = SPEED;
	pCam->_transSensivity = TRANS_SENSIVITY;
	pCam->_rotSensivity = ROT_SENSIVITY;
}
void cameraRelease (Camera* pCam)
{
	assert (pCam != NULL);

	cameraInit(pCam);
}

void cameraSetSpereMapRadius (Camera* pCam,
							unsigned int iRadius)
{
	assert (pCam != NULL);

	pCam->_sphereMapRadius = iRadius-50;
}


void cameraRotate (Camera* pCam,
					double dXRelative, double dYRelative)
{
	assert (pCam != NULL);

	pCam->_phi += dYRelative*pCam->_rotSensivity;
	pCam->_theta += dXRelative*pCam->_rotSensivity;
	cameraVectorsFromAngles(pCam);
}
void cameraTranslate (Camera* pCam,
						double dXRelative, double dYRelative)
{
	Vector CamUp, XTranslate, YTranslate;
	unsigned int CamDistance = 0;

	assert (pCam != NULL);

	CamDistance = vectorGetLength(&(pCam->_position));
	CamUp = vectorCrossProduct(&(pCam->_forward), &(pCam->_left));
	XTranslate = vectorProduct(
						&(pCam->_left),
						dXRelative*pCam->_transSensivity*CamDistance);
	YTranslate = vectorProduct(&CamUp,
							-dYRelative*pCam->_transSensivity*CamDistance);

	pCam->_position = vectorSum(&(pCam->_position), &XTranslate);
	pCam->_position = vectorSum(&(pCam->_position), &YTranslate);

	if (vectorGetLength(&(pCam->_position)) > (pCam->_sphereMapRadius))
	{
		vectorNormalize(&(pCam->_position));
		pCam->_position = vectorProduct(&(pCam->_position), (pCam->_sphereMapRadius));
	}

	pCam->_target = vectorSum(&(pCam->_position), &(pCam->_forward));
}
void cameraZoom (Camera* pCam,
				signed char cDirection,
				double dXRotate, double dYRotate)
{
	Vector YTranslate;
	unsigned int CamDistance = 0;

	assert (pCam != NULL);

	CamDistance = vectorGetLength(&(pCam->_position));
	YTranslate = vectorProduct(&(pCam->_forward),
							cDirection*pCam->_transSensivity*CamDistance);
	pCam->_position = vectorSum(&(pCam->_position), &YTranslate);

	if (vectorGetLength(&(pCam->_position)) > (pCam->_sphereMapRadius)-2)
	{
		vectorNormalize(&(pCam->_position));
		pCam->_position = vectorProduct(&(pCam->_position), (pCam->_sphereMapRadius));
	}

	/*cameraRotate(pCam,
				dXRotate*0.5*pCam->_rotSensivity,
				dYRotate*0.5*pCam->_rotSensivity);*/

	pCam->_target = vectorSum(&(pCam->_position), &(pCam->_forward));
}

void cameraLook (Camera* pCam)
{
	gluLookAt(pCam->_position.x, pCam->_position.y, pCam->_position.z,
			pCam->_target.x, pCam->_target.y, pCam->_target.z,
			0, 0, 1);
}



