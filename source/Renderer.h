#pragma once
#include "Direct3D.h"

void InitRenderer(void);
void DrawFrame(void);
void UninitRenderer(void);
bool ResetRenderer(void);
LPD3DXFONT GetFont(void);