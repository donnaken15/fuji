#include "Common.h"
#include "Material.h"
#include "Model.h"
#include "Animation.h"
#include "View.h"

extern Material *pCurrentMaterial;
extern View *pCurrentView;
extern uint32 renderSource;
extern uint32 currentRenderFlags;

void Renderer_SetRenderer(uint32 rendererFlags, uint32 flags, uint32 newRenderSource)
{
	Material *pMat = Material::GetCurrent();

	if(pCurrentMaterial != pMat || (currentRenderFlags&RT_Untextured) != (rendererFlags&RT_Untextured))
	{
		// set some render states
		if(pMat->pTextures[pMat->diffuseMapIndex] && !(rendererFlags & RT_Untextured))
		{
			// set the texture unit
			pMat->pTextures[pMat->diffuseMapIndex]->SetTexture(0);
		}
		else
		{
			Texture::UseNone(0);
		}
	}

	if(pCurrentMaterial != pMat)
	{
		// apply material settings
	}

	pCurrentMaterial = pMat;
	renderSource = newRenderSource;
	currentRenderFlags = rendererFlags;
}


