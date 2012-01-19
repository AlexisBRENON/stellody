#ifndef __3DDISPLAY_H__
#define __3DDISPLAY_H__

#include "opengl_data.h"
#include "camera.h"
#include "dynamic_array.h"
#include "3dvector.h"
#include "analyzed_track.h"

struct _Display
{
	OpenGLData* GLData;
	Camera* Cam;
	DynamicArray* Stars;
	Vector* PlayingStar;
};
typedef struct _Display Display;

Display* displayCreate(int iWidth, int iHeight,
					int iPrecision, int iDispGrid);
void displayDestroy(Display** ppDisp);

void displayInit(Display* pDisp,
				int iWidth, int iHeight,
				int iPrecision, int iDispGrid);
void displayRelease(Display* pDisp);
void displayReinit(Display* pDisp);

void displayResize(Display* pDisp,
				int iWidth, int iHeight);
void displayTranslate(Display* pDisp,
					double dXTranslate, double dYTranslate);
void displayRotate(Display* pDisp,
					double dXTranslate, double dYTranslate);
void displayZoom(Display* pDisp,
				char bDirection,
				double dXDirection,double dYDirection);

void displayDisp(Display const* pDisp);
int displaySelect (Display const* pDisp,
					int iXPosition, int iYPosition);

void displayAddStar (Display* pDisp,
					AnalyzedTrack const* pTrack);
void displayRemoveStar (Display* pDisp,
						AnalyzedTrack const* pTrack);
void displayRemoveStarByID (Display* pDisp,
							int iTID);

void displaySetPrecision (Display* pDisp,
						int iPrecision);
void displaySetDispGrid (Display* pDisp,
						int DispGrid);
void displaySetPlayedTrack (Display* pDisp,
							AnalyzedTrack const* pTrack);


#endif
