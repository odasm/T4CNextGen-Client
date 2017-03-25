// Rain.h: interface for the Rain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_NM__WEATHER__INCLUDED_)
#define _NM__WEATHER__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TileSet.h"


class CWeather  
{
public:
	CWeather();
	virtual ~CWeather(void);
	static CWeather *GetInstance(void);

   // set Rain Option
   void SetRainIntensity(UINT dwInt = 1){m_dwRainIntensity = dwInt;};
   void SetRainState(BOOL bRain)        {m_bRainStatus     = bRain;};
   void SwitchRainState(void)           {m_bRainStatus     = !m_bRainStatus;};
   BOOL GetRainState(void)              {return m_bRainStatus;};
   void DrawRain(CV2Sprite *pVSFSpriteR,CV2Sprite *pVSFSpriteL,V2SPRITEFX *fx);
   void DrawRain2(CV2Sprite *pVSFSpriteR1,V2SPRITEFX *fx, WORD Alpha);


   // Snow Option and Draw
   void SetSnowState(BOOL bSnow);
   void SwitchSnowState(void)           {m_bSnowStatus     = !m_bSnowStatus;};
   BOOL GetSnowState(void)              {return m_bSnowStatus;};
   void DrawSnow(CV2Sprite *pVSFSprite1,CV2Sprite *pVSFSprite2,V2SPRITEFX *fx, WORD Alpha);
	
private:	
	//RAIn Attributes
   BOOL m_bRainStatus;
   UINT m_dwRainIntensity;
   UINT *m_pRainX;
   UINT *m_pRainY;

   BOOL m_bSnowStatus;
   UINT m_dwSnowIntensity;
   UINT *m_pSnowX;
   UINT *m_pSnowY;
   UINT *m_pSnowN;

  
};

#endif // !defined(_NM__WEATHER__INCLUDED_)
