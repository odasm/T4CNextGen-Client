#include "pch.h"
#include "LightMap.h"
#include "IDirectXDraw.h"
#include "stdio.h"
#include "MacroHandler.h"
#include "PowerDebug.h"
#include "Global.h"
#include "SaveGame.h"
extern CSaveGame g_SaveGame;


static WORD *lpwLightMap = NULL;
//static WORD *lpwLightMap = new WORD[g_Global.GetScreenWDiv2() * g_Global.GetScreenHDiv2()];

//WORD LightMap::lpwLightMap[512 * 384];

LightMap::LightMap(void) 
{
   lpwMap = NULL;
}

LightMap::~LightMap(void) {
   Release();
}

void LightMap::Release(void) {
   if (lpwMap) 
   {
      delete lpwMap;
      lpwMap = NULL;
   }
   if(lpwLightMap)
   {
      delete []lpwLightMap;
      lpwLightMap = NULL;
   }
}

void LightMap::Create(LPTSTR lpszID, int w, int h) 
{
   if(lpwLightMap == NULL)
      lpwLightMap = new WORD[g_Global.GetScreenWDiv2() * g_Global.GetScreenHDiv2()];
   memset(lpwLightMap,0x00,g_Global.GetScreenWDiv2() * g_Global.GetScreenHDiv2()*2);

   WORD wRed;
   WORD wGreen;
   WORD wBlue;
   WORD wColor;
   char szTemp[256];

   BYTE *bTempBuffer = new BYTE[w*h*3];
   
   wWidth = w;
   wHeight = h;

   lpwMap = new WORD [wWidth*wHeight];


   strcpy(szTemp, "Game Files\\");
   strcat(szTemp, lpszID);
   
   FILE *f = fopen(szTemp, "rb");

   int dwtoto = fread(bTempBuffer, 1, w*h*3, f);
   fclose(f);

   for (int i = 0; i < w*h; i++) 
   {
      wRed = bTempBuffer[(i*3)] >> 3;
      wGreen = bTempBuffer[(i*3)+1] >> 3;
      wBlue = bTempBuffer[(i*3)+2] >> 3;

      // Brightness Correction
      //int caca = g_SaveGame.GetOptionsParam()->dwBrightness;//BLBLBL 29 mars 2009 suppr. lignes inutilisée
      
	  wRed = wRed + (g_SaveGame.GetOptionsParam()->dwBrightness*1);
      if (wRed > 31)
         wRed = 31;
      wGreen = wGreen + (g_SaveGame.GetOptionsParam()->dwBrightness-1);
      if (wGreen > 31)
         wGreen = 31;
      wBlue = wBlue + (g_SaveGame.GetOptionsParam()->dwBrightness-1);
      if (wBlue > 31)
         wBlue = 31;

      wColor = (wRed << 10) + (wGreen << 5) + (wBlue);

      lpwMap[i] = wColor;
   }

   delete bTempBuffer;
}

void LightMap::MakeBaseLightMap(int r, int g, int b) 
{
   if (g_SaveGame.GetOptionsParam()->bLightHightGraph) 
   {
      MakeBaseLightMap2(r, g, b);
      return;
   }

   // Brightness Correction
   r = r + ((g_SaveGame.GetOptionsParam()->dwBrightness-1));
   if (r > 31)
      r = 31;
   g = g + ((g_SaveGame.GetOptionsParam()->dwBrightness-1));
   if (g > 31)
      g = 31;
   b = b + ((g_SaveGame.GetOptionsParam()->dwBrightness-1));
   if (b > 31)
      b = 31;

   int i, j;
   DWORD dwColor = (r << 26) + (g << 21) + (b << 16) + (r << 10) + (g << 5) + (b);
   LPDWORD lpdwLightMap = (LPDWORD)lpwLightMap;
   
   for (j = 0; j < g_Global.GetScreenHDiv2(); j++) 
   {
      for (i = 0; i < g_Global.GetScreenWDiv2()/2; i++) 
      {
         *lpdwLightMap = dwColor;
         lpdwLightMap++;
      }
   }
}


void LightMap::MergeLightMap(int x, int y) 
{
   if (g_SaveGame.GetOptionsParam()->bLightHightGraph) 
   {
      MergeLightMap2(x, y);
      return;
   }

   // Set 1 Mini Buffer
   if ((x+wWidth) <= 0)
      return;
   if (x >= g_Global.GetScreenWDiv2())
      return;
   if ((y+wHeight) <= 0)
      return;
   if (y >= g_Global.GetScreenHDiv2())
      return;

   int j;
   int  xCut = x < 0 ? -x : 0;
   int  yCut = y < 0 ? -y : 0;
   int  xEnd = (x+wWidth)  > g_Global.GetScreenWDiv2() ? (x+wWidth) -g_Global.GetScreenWDiv2() : 0;
   int  yEnd = (y+wHeight) > g_Global.GetScreenHDiv2() ? (y+wHeight)-g_Global.GetScreenHDiv2() : 0;

   DWORD dwW = (DWORD)wWidth-(xEnd+xCut);
   WORD wH = wHeight;


   UINT dwOffset = ((x+xCut) + ((y+yCut)*g_Global.GetScreenWDiv2()));
   //if(dwOffset > (UINT)((g_Global.GetScreenWDiv2()*g_Global.GetScreenHDiv2())-(wWidth*wHeight)))
   //   dwOffset = (g_Global.GetScreenWDiv2()*g_Global.GetScreenHDiv2())-(wWidth*wHeight);

   if(dwOffset > (UINT)((g_Global.GetScreenWDiv2()*g_Global.GetScreenHDiv2())-(wWidth*(wH-yEnd-yCut))))
      dwOffset = (g_Global.GetScreenWDiv2()*g_Global.GetScreenHDiv2())-(wWidth*(wH-yEnd-yCut));

   LPWORD lpwL = (lpwLightMap + dwOffset);
   LPWORD lpwM = (lpwMap + xCut + yCut*wWidth);

   int dwWDiv2 = g_Global.GetScreenWDiv2();
   for (j = 0; j < wH-yEnd-yCut; j++) 
   {
         // Get MiniBuffer Color
         __asm {
            // ebx = M, edx = L;
            mov   ecx, dword ptr [dwW]

i_MergeLoop:           
            // wColor = *lpwM;
            mov   eax, dword ptr [lpwM]                  ; // 1
            mov   ebx, dword ptr [eax]                   ; // 1
            // wColor2 = *lpwL;
            mov   eax, dword ptr [lpwL]                  ; // 1
            mov   edx, dword ptr [eax]                   ; // 1
            // wSRed   = wColor & 0x7C00;
            mov   esi, ebx                               ; // 1/2
            and   esi, 7C00h                             ; // 1
            // wDRed   = wColor2 & 0x7C00;
            mov   edi, edx
            and   edi, 7C00h
            // if (wDRed < wSRed)
            //    wDRed = wSRed;
            mov   eax, edi
            cmp   esi, edi
            jl    Make_Green
            mov   eax, esi
Make_Green:
            // wSGreen = wColor & 0x3E0;
            mov   esi, ebx                               ; // 1/2
            and   esi, 3E0h                             ; // 1
            // wDGreen = wColor2 & 0x3E0;
            mov   edi, edx
            and   edi, 3E0h
            // if (wDGreen < wSGreen)
            //    wDGreen = wSGreen;
            cmp   esi, edi
            jl    Add_Green
            add   eax, esi
            jmp   Make_Blue
Add_Green:
            add   eax, edi
Make_Blue:
            // wSBlue = wColor & 0x1F;
            mov   esi, ebx                               ; // 1/2
            and   esi, 1Fh                            ; // 1
            // wDBlue = wColor2 & 0x1F;
            mov   edi, edx
            and   edi, 1Fh
            // if (wDBlue < wSBlue)
            //    wDBlue = wSBlue;
            cmp   esi, edi
            jl    Add_Blue
            add   eax, esi
            jmp   End
Add_Blue:
            add   eax, edi
End:
            // *lpwL = wDRed + wDGreen + wDBlue;
            mov ebx, dword ptr [lpwL]
            mov word ptr [ebx], ax

            // lpwM++
            add dword ptr [lpwM], 2
            // lpwL++
            add dword ptr [lpwL], 2

            dec ecx
            jnz i_MergeLoop

            //lpwL += (320-wWidth);
            mov eax, dwWDiv2
            sub eax, dword ptr [dwW]
            shl eax, 1
            add dword ptr [lpwL], eax
            
            mov eax, dword ptr [xEnd]
            add eax, dword ptr [xCut]
            shl eax, 1
            add dword ptr [lpwM], eax
         }
   }
}



void LightMap::MakeLightingFX(LPBYTE lpbDest, LPBYTE lpbSource, long lDPitch, long lSPitch, int xPos, int yPos) 
{
   if (g_SaveGame.GetOptionsParam()->bLightHightGraph) 
   {
      MakeLightingFX2(lpbDest, lpbSource, lDPitch, lSPitch, xPos, yPos);
      return;
   }

   //DWORD   dwColor;
   int j;
   LPWORD  lpwDest    = (LPWORD)lpbDest;
   LPWORD  lpwSource  = (LPWORD)(lpbSource+2);
   LPWORD  lpwLight   = lpwLightMap;
   DWORD dwRMask = lpDXDirectDraw->wRMask;
   DWORD dwGMask = lpDXDirectDraw->wGMask;
   DWORD dwBMask = lpDXDirectDraw->wBMask;

   lpwDest += (xPos);
   lpwSource += (xPos/2) + ((lSPitch/2)*(-yPos/2));
   lpwLight += (xPos/4) + (g_Global.GetScreenWDiv2()*(-yPos/4));

   long lValue1 = (lSPitch)+2;
   long lValue2 = (lDPitch);
   long lValue3 = (lSPitch)-6;
   long lValue4 = (lDPitch)-4;

   int dwWDiv2 = g_Global.GetScreenWDiv2();

   for (j = 0; j < g_Global.GetScreenHDiv2()+(yPos/2); j++) 
   {
         __asm 
         {
            // eax -> temp register
            // ebx -> low part:  word de la pixel 1
            // ecx -> loop register
            // edx -> low part:  Current Mask
            // esi -> low part:  word de la lumiere de la pixel 1, 2
            // edi -> wColor

            mov   ecx, dwWDiv2//140h
i_Loop:

            // pixel 1
            // wWord = *(LPWORD)(lpwSource);
            mov   eax, dword ptr [lpwSource]             ; // 1
            mov   ebx, dword ptr [eax]                   ; // 1
            // wWord = *(LPWORD)(lpwLight)
            mov   edx, dword ptr [lpwLight]              ; // 1
            mov   esi, dword ptr [edx]                   ; // 1
            // iRLight = wWord & 0x7C00
            mov   edx, esi                               ; // 1/2
            and   edx, 7C00h                             ; // 1
            // dwRed = wWord & wRMask;
            mov   eax, ebx                               ; // 1/2
            and   eax, dword ptr [dwRMask]                ; // 1
            // dwRed = ((dwRed * iRlight) >> 15) & wRMask
            mul   edx                                    ; // ?
            shr   eax, 15                                ; // 1
            and   eax, dword ptr [dwRMask]               ; // 1
            // wColor = dwRed    
            mov   edi, eax                               ; // 1/2
            // iGLight = wWord & 0x3E0
            mov   edx, esi                               ; // 1/2
            // dwGreen  =  wWord & wGMask;
            mov   eax, ebx                               ; // 1/2
            and   eax, dword ptr [dwGMask]                ; // 1
            and   edx, 3E0h                              ; // 1
            // dwGreen = ((dwGreen * iGlight) >> 10) & wGMask
            mul   edx                                    ; // ?
            shr   eax, 10                                 ; // 1
            and   eax, dword ptr [dwGMask]                ; // 1
            // wColor += dwGreen
            add   edi, eax                               ; // 1
            // iBLight = wWord & 0x1F
            mov   edx, esi                               ; // 1/2
            and   edx, 1Fh                               ; // 1
            // dwBlue  =  wWord & wBMask;
            mov   eax, ebx                               ; // 1/2
            and   eax, dword ptr [dwBMask]                ; // 1
            // dwBlue = ((dwBlue * iBlight) >> 5)
            mul   edx                                    ; // ?
            shr   eax, 5                                 ; // 1
            and   eax, dword ptr [dwBMask]
            // wColor += dwBlue
            add   edi, eax                               ; // 1
            shl   edi, 16

            // lpwSource++;
            sub dword ptr [lpwSource], 2;                ; // ?

            // pixel 2
            // wWord = *(LPWORD)(lpwSource);
            mov   eax, dword ptr [lpwSource]             ; // 1
            mov   ebx, dword ptr [eax]                   ; // 1
            // dwRed = wWord & wRMask;
            mov   eax, ebx                               ; // 1/2
            and   eax, dword ptr [dwRMask]                ; // 1
            // iRLight = wWord & 0x7C00
            mov   edx, esi                               ; // 1/2
            and   edx, 7C00h                             ; // 1
            // dwRed = ((dwRed * iRlight) >> 15) & wRMask
            mul   edx                                    ; // ?
            shr   eax, 15                                ; // 1
            and   eax, dword ptr [dwRMask]                ; // 1
            // wColor = dwRed    
            add   edi, eax
            // dwGreen  =  wWord & wGMask;
            mov   eax, ebx                               ; // 1/2
            and   eax, dword ptr [dwGMask]                ; // 1
            // iGLight = wWord & 0x3E0
            mov   edx, esi                               ; // 1/2
            and   edx, 3E0h                              ; // 1
            // dwGreen = ((dwGreen * iGlight) >> 10) & wGMask
            mul   edx                                    ; // ?
            shr   eax, 10                                ; // 1
            and   eax, dword ptr [dwGMask]                ; // 1
            // wColor += dwGreen
            add   edi, eax                               ; // 1
            // dwBlue  =  wWord & wBMask;
            mov   eax, ebx                               ; // 1/2
            and   eax, dword ptr [dwBMask]                ; // 1
            // iBLight = wWord & 0x1F
            mov   edx, esi                               ; // 1/2
            and   edx, 1Fh                               ; // 1
            // dwBlue = ((dwBlue * iBlight) >> 5)
            mul   edx                                    ; // ?
            shr   eax, 5                                 ; // 1
            and   eax, dword ptr [dwBMask]
            // wColor += dwBlue
            add   edi, eax                               ; // 1
            // *lpwDest = WColor
            mov   edx, dword ptr [lpwDest];              ; // 1
            mov   dword ptr [edx], edi;                  ; // 1

            // lpwSource++;
            mov eax, dword ptr [lValue1]
			add dword ptr [lpwSource], eax               ; // ?
            // lpwDest++;
            mov eax, dword ptr [lValue2]
            add dword ptr [lpwDest], eax                 ; // ?

            // pixel 3
            // wWord = *(LPWORD)(lpwSource);
            mov   eax, dword ptr [lpwSource]             ; // 1
            mov   ebx, dword ptr [eax]                   ; // 1
            // iRLight = wWord & 0x7C00
            mov   edx, esi                               ; // 1/2
            and   edx, 7C00h                             ; // 1
            // dwRed = wWord & wRMask;
            mov   eax, ebx                               ; // 1/2
            and   eax, dword ptr [dwRMask]                ; // 1
            // dwRed = ((dwRed * iRlight) >> 15) & wRMask
            mul   edx                                    ; // ?
            shr   eax, 15                                ; // 1
            and   eax, dword ptr [dwRMask]                ; // 1
            // wColor = dwRed    
            mov   edi, eax
            // iGLight = wWord & 0x3E0
            mov   edx, esi                               ; // 1/2
            and   edx, 3E0h                              ; // 1
            // dwGreen  =  wWord & wGMask;
            mov   eax, ebx                               ; // 1/2
            and   eax, dword ptr [dwGMask]                ; // 1
            // dwGreen = ((dwGreen * iGlight) >> 10) & wGMask
            mul   edx                                    ; // ?
            shr   eax, 10                                ; // 1
            and   eax, dword ptr [dwGMask]                ; // 1
            // wColor += dwGreen
            add   edi, eax                               ; // 1
            // iBLight = wWord & 0x1F
            mov   edx, esi                               ; // 1/2
            and   edx, 1Fh                               ; // 1
            // dwBlue  =  wWord & wBMask;
            mov   eax, ebx                               ; // 1/2
            and   eax, dword ptr [dwBMask]                ; // 1
            // dwBlue = ((dwBlue * iBlight) >> 5)
            mul   edx                                    ; // ?
            shr   eax, 5                                 ; // 1
            and   eax, dword ptr [dwBMask]
            // wColor += dwBlue
            add   edi, eax                               ; // 1
            // *lpwDest = WColor
            shl   edi, 16

            // lpwSource++;
            sub dword ptr [lpwSource], 2                 ; // ?

            // pixel 4
            // wWord = *(LPWORD)(lpwSource);
            mov   eax, dword ptr [lpwSource]             ; // 1
            mov   ebx, dword ptr [eax]                   ; // 1
            // iRLight = wWord & 0x7C00
            mov   edx, esi                               ; // 1/2
            and   edx, 7C00h                             ; // 1
            // dwRed = wWord & wRMask;
            mov   eax, ebx                               ; // 1/2
            and   eax, dword ptr [dwRMask]                ; // 1
            // dwRed = ((dwRed * iRlight) >> 15) & wRMask
            mul   edx                                    ; // ?
            shr   eax, 15                                ; // 1
            and   eax, dword ptr [dwRMask]                ; // 1
            // wColor = dwRed    
            add   edi, eax
            // iGLight = wWord & 0x3E0
            mov   edx, esi                               ; // 1/2
            and   edx, 3E0h                              ; // 1
            // dwGreen  =  wWord & wGMask;
            mov   eax, ebx                               ; // 1/2
            and   eax, dword ptr [dwGMask]                ; // 1
            // dwGreen = ((dwGreen * iGlight) >> 10) & wGMask
            mul   edx                                    ; // ?
            shr   eax, 10                                ; // 1
            and   eax, dword ptr [dwGMask]                ; // 1
            // wColor += dwGreen
            add   edi, eax                               ; // 1
            // iBLight = wWord & 0x1F
            mov   edx, esi                               ; // 1/2
            and   edx, 1Fh                               ; // 1
            // dwBlue  =  wWord & wBMask;
            mov   eax, ebx                               ; // 1/2
            and   eax, dword ptr [dwBMask]                ; // 1
            // dwBlue = ((dwBlue * iBlight) >> 5)
            mul   edx                                    ; // ?
            shr   eax, 5                                 ; // 1
            and   eax, dword ptr [dwBMask]
            // wColor += dwBlue
            add   edi, eax                               ; // 1
            // *lpwDest = WColor
            mov   edx, dword ptr [lpwDest];              ; // 1
            mov   dword ptr [edx], edi;                  ; // 1

            // lpwSource++;
            mov eax, dword ptr [lValue3]
            sub dword ptr [lpwSource], eax              ; // ?
            // lpwDest++;
            mov eax, dword ptr [lValue4]
            sub dword ptr [lpwDest], eax                ; // ?
            // lpwLight++;
            add dword ptr [lpwLight], 2                  ; // ?

            dec ecx
            jnz i_Loop
            
         } 
      lpwSource += (lSPitch-g_Global.GetScreenW());
      lpwDest += (lDPitch-g_Global.GetScreenW());
   } 
}



void LightMap::MakeBaseLightMap2(int r, int g, int b) 
{
   LPDWORD lpdwColor = (LPDWORD)lpwLightMap;
   
   int iColor = ((r + g + b) / 3);
   DWORD iStep;
   
   if (iColor > 16)
      iStep = 0;
   else if (iColor > 8)
      iStep = 1;
   else if (iColor > 4)
      iStep = 2;
   else if (iColor > 2)
      iStep = 3;
   else if (iColor > 1)
      iStep = 4;
   else 
      iStep = 5;
   
   DWORD dwRMask = (lpDXDirectDraw->wRMask);
   DWORD dwGMask = (lpDXDirectDraw->wGMask);
   DWORD dwBMask = (lpDXDirectDraw->wBMask);
   DWORD dwMask = ((dwRMask >> iStep) & dwRMask) + ((dwGMask >> iStep) & dwGMask) + ((dwBMask >> iStep) & dwBMask);
   dwMask = (dwMask << 16) + dwMask;
   
   *lpdwColor = dwMask;
   lpdwColor++;
   *lpdwColor = iStep;
}

void LightMap::MergeLightMap2(int x, int y) 
{
   if (!x && !y && wWidth == g_Global.GetMainTorchW() && wHeight == g_Global.GetMainTorchH()) 
   {
      DWORD dwLight = *(lpwMap + (wWidth/2)+(wHeight/2)*wWidth);
      LPDWORD lpdwColor = (LPDWORD)lpwLightMap;
      
      DWORD dwRMask = (lpDXDirectDraw->wRMask);
      DWORD dwGMask = (lpDXDirectDraw->wGMask);
      DWORD dwBMask = (lpDXDirectDraw->wBMask);

      int r, g, b;
      
      r = (dwLight & dwRMask) >> lpDXDirectDraw->bRDown;
      g = (dwLight & dwGMask) >> lpDXDirectDraw->bGDown;
      b = (dwLight & dwBMask) >> lpDXDirectDraw->bBDown;
      
      int iColor = ((r + g + b) / 3);
      DWORD iStep;
      
      if (iColor > 16)
         iStep = 0;
      else if (iColor > 8)
         iStep = 1;
      else if (iColor > 4)
         iStep = 2;
      else if (iColor > 2)
         iStep = 3;
      else if (iColor > 1)
         iStep = 4;
      else 
         iStep = 5;
      
      DWORD dwMask = ((dwRMask >> iStep) & dwRMask) + ((dwGMask >> iStep) & dwGMask) + ((dwBMask >> iStep) & dwBMask);
      dwMask = (dwMask << 16) + dwMask;
      
      *lpdwColor = dwMask;
      lpdwColor++;
      *lpdwColor = iStep;
   }
}

void LightMap::MakeLightingFX2(LPBYTE lpbDest, LPBYTE lpbSource, long lDPitch, long lSPitch, int xPos, int yPos) 
{
   LPDWORD lpdwDest = (LPDWORD)lpbDest;
   LPDWORD lpdwSource = (LPDWORD)lpbSource;
   DWORD   dwMask = *(LPDWORD)lpwLightMap;
   DWORD   iStep = *(LPDWORD)(lpwLightMap+2);
   
   int i, j;
   
   int DAdd = ((lDPitch/4)-g_Global.GetScreenWDiv2()+(xPos/2));
   int SAdd = ((lSPitch/4)-g_Global.GetScreenWDiv2()+(xPos/2));
   
   //if (iStep != 0) 
   {
      for (j = 0; j < g_Global.GetScreenH()+yPos; j++) 
      {
         for (i = 0; i < g_Global.GetScreenWDiv2()-(xPos/2); i++) 
         {
            *lpdwDest = ((*lpdwSource) >> iStep) & dwMask;
            lpdwDest++;
            lpdwSource++;
         }
         lpdwDest += DAdd;
         lpdwSource += SAdd;
      }
   } 
   /*
   else 
   {
      for (j = 0; j < g_Global.GetScreenH()+yPos; j++) 
      {
         for (i = 0; i < g_Global.GetScreenWDiv2()-(xPos/2); i++) 
         {
            *lpdwDest = *lpdwSource;
            lpdwDest++;
            lpdwSource++;
         }
         lpdwDest += DAdd;
         lpdwSource += SAdd;
      }
   }
   */
}


