// Rain.cpp: implementation of the Rain class.
// By:       Scotch for Dialsoft
// Date:     02/20/2005
// Purpose:  Raining system & client / server communication
//////////////////////////////////////////////////////////////////////
#include <windows.h>           
#include <stdio.h>
#include "pch.h"
#include "Weather.h"
#include "TFCPlayer.h"
#include "CombatCursor.h"
#include "IDirectXImage.h"
#include "Comm.h"
#include "NewSound.h"

///////////////////////////////////////////////////////////////////////
/////Ophise Add' For Sound Effect//////////////////////////////////////
/*namespace{
    T3VSBSound *LightningSound = NULL;

    void InitSound(){
        if( !LightningSound ){
            LightningSound = new T3VSBSound;
            //LightningSound->Create( "Thunder", TS_MEMORY );
			LightningSound->Create( "Lightning", TS_MEMORY );
        }
    }

};*/
//////////////////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWeather::CWeather()
{
   //Rain
   m_bRainStatus = FALSE;
   m_dwRainIntensity = 20;
   m_pRainX = new UINT[m_dwRainIntensity*10];
   m_pRainY = new UINT[m_dwRainIntensity*10];

   //Snow
   m_bSnowStatus = FALSE;
   m_dwSnowIntensity = 20;
   m_pSnowX = new UINT[m_dwSnowIntensity*10];
   m_pSnowY = new UINT[m_dwSnowIntensity*10];
   m_pSnowN = new UINT[m_dwSnowIntensity*10];
  
}

CWeather::~CWeather()
{
   if(m_pRainX)
      delete []m_pRainX;
   if(m_pRainY)
      delete []m_pRainY;

   m_pRainX = NULL;
   m_pRainY = NULL;


   if(m_pSnowX)
      delete []m_pSnowX;
   if(m_pSnowY)
      delete []m_pSnowY;
   if(m_pSnowN)
      delete []m_pSnowN;

   m_pSnowX = NULL;
   m_pSnowY = NULL;
   m_pSnowN = NULL;
}

CWeather *CWeather::GetInstance(void)
{
    static CWeather m_pInstance;
    return &m_pInstance;
}

void CWeather::DrawRain(CV2Sprite *pVSFSpriteR,CV2Sprite *pVSFSpriteL,V2SPRITEFX *fx)
{
	//InitSound(); // Ophise Add'
   static BOOL bShowLight1 = FALSE;
   static BOOL bShowLight2 = FALSE;
   static UINT oldIntensity = 1;
   static UINT dwCnt        = 0;
   UINT dwNbrGouttes = 0;

   if(oldIntensity != m_dwRainIntensity)
   {
      dwCnt = 0;
      if(m_dwRainIntensity >10)
         m_dwRainIntensity = 10;
      oldIntensity = m_dwRainIntensity;

      if(m_pRainX)
         delete []m_pRainX;
      if(m_pRainY)
         delete []m_pRainY;
      m_pRainX = new UINT[m_dwRainIntensity*10];
      m_pRainY = new UINT[m_dwRainIntensity*10];
   }
   dwNbrGouttes = m_dwRainIntensity*10;

   if((dwCnt == 0 || dwCnt%2) || m_dwRainIntensity>3)
   {
      for(UINT i=0;i<dwNbrGouttes;i++)
      {
         m_pRainX[i] = rand()%g_Global.GetScreenW();
         m_pRainY[i] = rand()%g_Global.GetScreenH();
         pVSFSpriteR->DrawSpriteN(m_pRainX[i], m_pRainY[i],fx);
      }
   }
   else 
   {
      for(UINT i=0;i<dwNbrGouttes;i++)
         pVSFSpriteR->DrawSpriteN(m_pRainX[i], m_pRainY[i],fx);
   }
   
   if(m_dwRainIntensity >3 && dwCnt%3)
   {
      //Add LightNing...
      bShowLight1 = FALSE;
      bShowLight2 = FALSE;
      if(rand()%1000 <5)
      {
         pVSFSpriteL[0].DrawSpriteN(0, 0,fx);
		 //LightningSound->Play( TRUE ); //Ophise Add'
         bShowLight1 = TRUE;
      }
      if(rand()%1000 <2)
      {
         pVSFSpriteL[1].DrawSpriteN(0, 0,fx);
		 //LightningSound->Play( TRUE ); // Ophise Add'
         bShowLight2 = TRUE;
      }
   }
   else
   {
      if(bShowLight1)
	  {
         pVSFSpriteL[0].DrawSpriteN(0, 0,fx);
		 //LightningSound->Play( TRUE ); // Ophise Add'
	  }
      if(bShowLight2)
	  {
         pVSFSpriteL[1].DrawSpriteN(0, 0,fx);
	     //LightningSound->Play( TRUE ); // Ophise Add'
	  }
   }
   dwCnt++;
}


void CWeather::DrawRain2(CV2Sprite *pVSFSpriteR1,V2SPRITEFX *fx, WORD Alpha)
{
   int dwOffset = rand()%16;

   static int dwCnt = 0;
   if(dwCnt ==1)
     dwOffset+=16;
   else if(dwCnt ==2)
     dwOffset+=32;
   else if(dwCnt ==3)
     dwOffset+=48;
   
   pVSFSpriteR1->DrawSpriteNSemiTrans((dwOffset*2)-64-64, dwOffset-32-32,fx,TRUE,(BYTE)Alpha);
   dwCnt ++;
   if(dwCnt >3)
      dwCnt = 0;
 }

void CWeather::SetSnowState(BOOL bSnow)        
{
   m_bSnowStatus     = bSnow;
}

void CWeather::DrawSnow(CV2Sprite *pVSFSprite1,CV2Sprite *pVSFSprite2,V2SPRITEFX *fx, WORD Alpha)
{
   static UINT oldSnowIntensity = 1;
   static UINT dwSnowCnt        = 0;
   UINT dwNbrFlocon = 0;

   
   if(oldSnowIntensity != m_dwSnowIntensity)
   {
      dwSnowCnt = 0;
      if(m_dwSnowIntensity >12)
         m_dwSnowIntensity = 12;
      oldSnowIntensity = m_dwSnowIntensity;

      if(m_pSnowX)
         delete []m_pSnowX;
      if(m_pSnowY)
         delete []m_pSnowY;
      if(m_pSnowN)
         delete []m_pSnowN;
      
      m_pSnowX = new UINT[m_dwSnowIntensity*10];
      m_pSnowY = new UINT[m_dwSnowIntensity*10];
      m_pSnowN = new UINT[m_dwSnowIntensity*10];
   }
   dwNbrFlocon = m_dwSnowIntensity*10;

   if(dwSnowCnt == 0)
   {
      for(UINT i=0;i<dwNbrFlocon;i++)
      {
         m_pSnowX[i] = rand()%g_Global.GetScreenW();
         m_pSnowY[i] = rand()%g_Global.GetScreenH();
         m_pSnowN[i] = rand()%2;
         if(m_pSnowN[i])
            pVSFSprite1->DrawSpriteN/*SemiTrans*/(m_pSnowX[i], m_pSnowY[i],fx/*,TRUE,(BYTE)Alpha*/);
         else
            pVSFSprite2->DrawSpriteN/*SemiTrans*/(m_pSnowX[i], m_pSnowY[i],fx/*,TRUE,(BYTE)Alpha*/);

         //pVSFSpriteR->DrawSpriteNSemiTrans(m_pSnowX[i], m_pSnowY[i],fx,TRUE,(BYTE)Alpha);
      }
   }
   else
   {
      for(UINT i=0;i<dwNbrFlocon;i++)
      {
         if(m_pSnowN[i])
            pVSFSprite1->DrawSpriteN/*SemiTrans*/(m_pSnowX[i], m_pSnowY[i],fx/*,TRUE,(BYTE)Alpha*/);
         else
            pVSFSprite2->DrawSpriteN/*SemiTrans*/(m_pSnowX[i], m_pSnowY[i],fx/*,TRUE,(BYTE)Alpha*/);

      }
         //pVSFSpriteR->DrawSpriteNSemiTrans(m_pSnowX[i], m_pSnowY[i],fx,TRUE,(BYTE)Alpha);
   }
  
   dwSnowCnt++;
   if(dwSnowCnt >2)
      dwSnowCnt = 0;

}
