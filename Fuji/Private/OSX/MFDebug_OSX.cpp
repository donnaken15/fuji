#include "Fuji.h"

#if !defined(_RETAIL)

#include "MFCallstack_Internal.h"
#include "MFSystem_Internal.h"
#include "Display_Internal.h"
#include "MFFont.h"
#include "MFView.h"
#include "MFPrimitive.h"
#include "MFMaterial.h"

#include <stdio.h>

// FIXME: OpenGL Includes

void MFSystem_HandleEventsPlatformSpecific();

extern int gQuit;

// Output a string to the debugger.
void MFDebug_Message(const char *pMessage)
{
	fprintf(stderr, MFStr("%s\n", pMessage));
}

#if defined(_FUJI_UTIL)

void MFDebug_DebugAssert(const char *pReason, const char *pMessage, const char *pFile, int line)
{
	MFDebug_Message(MFStr("%s(%d) : Assertion Failure.",pFile,line));
	MFDebug_Message(MFStr("Failed Condition: (%s)\n%s", pReason, pMessage));

	MFDebug_Breakpoint();
}

#else

void MFDebug_DebugAssert(const char *pReason, const char *pMessage, const char *pFile, int line)
{
	MFDebug_Message(MFStr("%s(%d) : Assertion Failure.",pFile,line));
	MFDebug_Message(MFStr("Failed Condition: %s\n%s", pReason, pMessage));
#if !defined(_RETAIL)
	MFCallstack_Log();
#endif

	while(!gQuit)
	{
		MFSystem_HandleEventsPlatformSpecific();

		MFSystem_UpdateTimeDelta();
		gFrameCount++;

		MFSystem_Update();
		MFSystem_PostUpdate();

		MFDisplay_BeginFrame();

		MFDisplay_SetClearColour(0,0,0,0);
		MFDisplay_ClearScreen();

		MFView_SetDefault();
		MFView_SetOrtho();

		// FIXME: Need Linux code for resetting the render states
		// Set some renderstates
#if defined(_WINDOWS) || defined(_XBOX)
		pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
		pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

		pd3dDevice->SetTexture(0, NULL);
#endif

		if(!(((uint32)gSystemTimer.GetSecondsF()) % 2))
		{
			MFMaterial_SetMaterial(MFMaterial_GetStockMaterial(MFMat_White));
			MFPrimitive(PT_QuadList);

			MFBegin(4);
			MFSetColour(1,0,0,1);
			MFSetPosition(50, 50, 0);
			MFSetPosition(590, 110, 0);

			MFSetColour(0,0,0,1);
			MFSetPosition(55, 55, 0);
			MFSetPosition(585, 105, 0);
			MFEnd();
		}

		MFFont_DrawTextf(MFFont_GetDebugFont(), 110, 60, 20, MakeVector(1,0,0,1), "Software Failure. Press left mouse button to continue.");
		MFFont_DrawTextf(MFFont_GetDebugFont(), 240, 80, 20, MakeVector(1,0,0,1), "Guru Meditation: ");

		MFFont_DrawTextf(MFFont_GetDebugFont(), 80, 120, 20, MakeVector(1,0,0,1), "Assertion Failure:");
		MFFont_DrawTextf(MFFont_GetDebugFont(), 80, 140, 20, MakeVector(1,0,0,1), MFStr("Failed Condition: %s", pReason));
		MFFont_DrawTextf(MFFont_GetDebugFont(), 80, 160, 20, MakeVector(1,0,0,1), MFStr("File: %s, Line: %d", pFile, line));
		MFFont_DrawTextf(MFFont_GetDebugFont(), 80, 190, 20, MakeVector(1,0,0,1), MFStr("Message: %s", pMessage));

#if !defined(_RETAIL)
		MFFont_DrawTextf(MFFont_GetDebugFont(), 80, 230, 20, MakeVector(1,0,0,1), "Callstack:");
		MFFont_DrawTextf(MFFont_GetDebugFont(), 100, 250.0f, 20, MakeVector(1,0,0,1), MFCallstack_GetCallstackString());
#else
		MFFont_DrawTextf(MFFont_GetDebugFont(), 80, 230, 20, MakeVector(1,0,0,1), "Callstack not available in _RETAIL builds");
#endif

//		MFSystem_Draw();
		MFDisplay_EndFrame();
	}
}

#endif

#endif // !defined(_RETAIL)
