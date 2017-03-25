#ifndef __LIGHTMAP_H
#define __LIGHTMAP_H

#include <windows.h>
#include "Global.h"

//#define LM_MAX_X  SCREEN_W_DIV2
//#define LM_MAX_Y  SCREEN_H_DIV2

class LightMap {

   public:
      LightMap(void);
      ~LightMap(void);
      void Create(LPTSTR, int, int);
      void Release(void);
      void MergeLightMap(int, int);
      void MergeLightMap2(int, int);
      static void MakeBaseLightMap(int, int, int);
      static void MakeBaseLightMap2(int, int, int);
      static void MakeLightingFX(LPBYTE, LPBYTE, long, long, int, int);
      static void MakeLightingFX2(LPBYTE, LPBYTE, long, long, int, int);

   private:

      LPWORD lpwMap;

      WORD wWidth;
      WORD wHeight;
};

#endif