#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "3dvector.h"

struct _Camera
{
	double _speed;
	double _transSensivity;
	double _rotSensivity;

	unsigned int _sphereMapRadius;

	Vector _position;
	Vector _target;
	Vector _forward;
	Vector _left;

	double _theta;
	double _phi;
};
typedef struct _Camera Camera;


Camera* cameraCreate ();
Camera* cameraCreateWithCoord (double dX, double dY, double dZ);
Camera* cameraCreateWithVector (Vector const* const pVect);
void cameraDestroy (Camera** ppCam);

void cameraInit (Camera* pCam);
void cameraInitWithCoord (Camera* pCam,
						double dX, double dY, double dZ);
void cameraInitWithVector (Camera* pCam,
						Vector const* const pVect);
void cameraRelease (Camera* pCam);

void cameraSetSpereMapRadius (Camera* pCam,
							unsigned int iRadius);

void cameraRotate (Camera* pCam,
					double dXRelative, double dYRelative);
void cameraTranslate (Camera* pCam,
						double dXRelative, double dYRelative);
void cameraZoom (Camera* pCam,
				signed char cDirection,
				double dXRotate, double dYRotate);

void cameraSetDirection (Camera* pCam,
						double dX, double dY, double dZ);
void cameraAnimate (Camera* pCam);

void cameraLook (Camera* pCam);



#endif
