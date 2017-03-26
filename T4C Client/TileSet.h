// **************************************************************************
// ***                                                                    ***
//      File Name: TileSet.H
//      Project:   The Fourth Coming
//      Creation:  May 30th, 1997
//      Author:    Benoit Thomas (TH)
// ***                                                                    ***
// **************************************************************************
// ***                                                                    ***
//      Change History
//
//         Date            Ver.	  Author	     Purpose
//         ----            ----    ------      -------
//         05-30-1997      1.0     TH          Initial development.
//
//		  Fonctions:
//			  Provide TileSet Basic.
//
// ***                                                                    ***
//	**************************************************************************
// *** Copyright (c) 1996-1997 Vritual Dreams (r). All rights reserved.   ***
 
#ifndef __Virtual_Dreams_TileSet_092819053097
#define __Virtual_Dreams_TileSet_092819053097
 
#include "Sprite.h"
#include "Random.h" 
#include "V2Sprite.h"
#include "cs.h"
#include "Icon3D.h"
#include "TFCLandDef.h"

//#define DONE 4
//#define MOVX 8
//#define MOVY 4
//#define FPS  16

#define DONE 8
#define MOVX 4
#define MOVY 2
#define FPS  32

#define SKILLS 0
#define STATS 1
#define OBJECTS 2
#define EQUIPMENTS 3
#define TRAIN_SKILL 4

enum TileSetError { NOT_DEFINED_VIEW, FILE_NOT_FOUND };

template <class type>
class TestArray {
   private:
       type *Array;
       DWORD MaxAddr;

   public:
      TestArray(void) {
         Array = NULL;
      }

      ~TestArray(void) {
         if (Array)
            delete Array;
      }
      
      void Alloc(DWORD alloc) {
         Array = new type [alloc];
         MaxAddr = alloc;
      }
      
      type &operator [] (DWORD addr) {
         if (addr > MaxAddr) {
            LOG << "\r\n Address out of bound. \r\n";
            exit(0);
         }
         return Array[addr];
      }

};

class TileSetException {
	public:
		TileSetException(TileSetError);
};

class GraphicStructure {
   public:
      int iCorner, iBorder, iWall, iDarkWall, 
          iExcp1, iDarkExcp1,
          iExcp2, iDarkExcp2,
          iExcp3, iDarkExcp3,
          iExcp4, iDarkExcp4,
          iExcp5, iDarkExcp5;
      int iG;
      int iE;
      int WallSet;
      char *Name;
      void Load(void);
      void Draw(int, int, int, RECT *, RECT *, RECT *, LPDIRECTDRAWSURFACE, int);
};

const int MAXCUSTOMCORNER    = 10;
const int MAXCUSTOMBORDER    = 10;
const int MAXCUSTOMWALL      = 10;
const int MAXCUSTOMDARKWALL  = 10;
const int MAXCUSTOMEXCP1     = 10;
const int MAXCUSTOMDARKEXCP1 = 10;
const int MAXCUSTOMEXCP2     = 10;
const int MAXCUSTOMDARKEXCP2 = 10;
const int MAXCUSTOMEXCP3     = 10;
const int MAXCUSTOMDARKEXCP3 = 10;
const int MAXCUSTOMEXCP4     = 10;
const int MAXCUSTOMDARKEXCP4 = 10;
const int MAXCUSTOMEXCP5     = 10;
const int MAXCUSTOMDARKEXCP5 = 10;

class TileSet {
	private:
      bool StaticSave;
      bool StaticState;
   public:
      BYTE FadeLevel;
	  CriticalSection csLock;
	  bool SetFading;
	  bool RealFading;
		char *XPString, *MPString, *HPString;
	private:
	
		RandomTable<215> Rnd;
		
		int Middle;

		int a, b;
		
		char  TileWidth;
		char  TileHeight; 
		char  ViewWidth;
		char  ViewHeight;
		char  SizeWidth;
		char  SizeHeight;
   public:
      short VirtualWidth;
		short VirtualHeight;
   private:
		short WorldWidth;
		short WorldHeight;
	
		//unsigned short *CompiledView;
      TestArray<unsigned short> CompiledView;
		unsigned short *VirtualView;

		VOID (* flpLoadSprite[LASTONE])(VOID);
		GraphicStructure *flpLoadSprite2[LASTONE];
		GraphicStructure *flpDeletePtr[1000];
		int iNbGSPtr;

		char **WorldFileName;
   public:
		char NumberofWorld;
   private:
		char CurrentWorld;

      int    m_dwFogX[15];
      int    m_dwFogY[15];
      float  m_fFoyY[15];
      WORD   m_wFogStatus;
      WORD   m_wFogAlphaVal;
      ULONG  m_dwFogCntGlobal;

      WORD   m_wRainStatus;
      WORD   m_wRainAlphaVal;
      ULONG  m_dwRainCntGlobal;

      WORD   m_wSnowStatus;
      WORD   m_wSnowAlphaVal;
      ULONG  m_dwSnowCntGlobal;

	public:
		LPDIRECTDRAWSURFACE lpPreLightSurface;
		LPDIRECTDRAWSURFACE lpSmoothSurface2;
		LPDIRECTDRAWSURFACE lpSmoothTemp;
	private:
		static LPDIRECTDRAWSURFACE lpFractalSurface;

		RECT ScreenDest;
	public:
		RECT SmoothSource, SmoothSource2;
	private:
		RECT FractalSource[1];
		RECT SurfaceDest;
		RECT SurfaceSource;
		RECT SourceRect[4];
		RECT DestRect[4];

	public:
		int Done, Done2;
	private:
	public:
		int MovX,    MovY, MovX2, MovY2;
		int SepX,    SepY, SepX2, SepY2;
		int OldSepX, OldSepY;
		int xPos,    yPos, wPos; 

      #define T_EARTH_X  32 // 11
      #define T_EARTH_Y  32 // 10
      #define T_NGRASS_X 32 // 13
      #define T_NGRASS_Y 32 // 15
	   #define T_DGRASS_X 32 // 11
      #define T_DGRASS_Y 32 // 10
      #define T_WATER1_X 32 // 10
      #define T_WATER1_Y 32 // 9
	   #define T_WATER2_X 32 // 10
      #define T_WATER2_Y 32 // 9
	   #define T_MOUNT_X  32 // 10
      #define T_MOUNT_Y  32 // 9
      #define T_FOREST_X 32 // 11
      #define T_FOREST_Y 32 // 11
      #define T_SAND_X   32 // 2
      #define T_SAND_Y   32 // 2


      #define T_ROCK_X 16 // 3
      #define T_ROCK_Y 16 // 3
      #define T_OLDROCK_X 4 // 2
      #define T_OLDROCK_Y 4 // 2
      #define T_SNOW_X 16 
      #define T_SNOW_Y 16 
      #define T_EARTHI_X 16 
      #define T_EARTHI_Y 16 
      #define T_CAVERNG_X 16 
      #define T_CAVERNG_Y 16 
      #define T_ICE_X 16 
      #define T_ICE_Y 16 

      #define T_2WOODEN_X 8 
      #define T_2WOODEN_Y 8 

      #define T_3WOODEN_X 8 
      #define T_3WOODEN_Y 8 
      
      #define T_TUILE1_X 16 
      #define T_TUILE1_Y 16 
      #define T_TUILE2_X 16 
      #define T_TUILE2_Y 16 

      #define T_STUILE1_X 8 
      #define T_STUILE1_Y 8 
      #define T_STUILE2_X 8 
      #define T_STUILE2_Y 8 

      #define T_ROCK2_X 16 
      #define T_ROCK2_Y 16 

      #define T_AWATER01_N 6//17 
      #define T_AWATER01_X 16 
      #define T_AWATER01_Y 16 

      //#define T_ALAVA01_N 63
      //#define T_ALAVA01_X 16 
      //#define T_ALAVA01_Y 16 

      #define T_CENDRE_X 32 
      #define T_CENDRE_Y 32 

      #define T_NLAVA_X 32 
      #define T_NLAVA_Y 32 

		CV2Sprite Grass[T_NGRASS_Y][T_NGRASS_X], Earth[T_EARTH_Y][T_EARTH_X], Rock[7][7],RockFloor2[T_ROCK2_Y][T_ROCK2_X], Desert[T_SAND_Y][T_SAND_X], Floor[6],Floor2[T_2WOODEN_Y][T_2WOODEN_X],Floor3[T_3WOODEN_Y][T_3WOODEN_Y], GraRoc2[4], GraRoc[4], Tree[43], 
          Snow[T_SNOW_Y][T_SNOW_X],EarthI[T_EARTHI_Y][T_EARTHI_X],CavernG[T_CAVERNG_Y][T_CAVERNG_X],IceFloor[T_ICE_Y][T_ICE_X],
          TuileTmps1 [T_TUILE1_Y] [T_TUILE1_X] ,TuileTmps2 [T_TUILE2_Y] [T_TUILE2_X],
          STuileTmps1[T_STUILE1_Y][T_STUILE1_X],STuileTmps2[T_STUILE2_Y][T_STUILE2_X],
         /*GrassBreak1[9][8],*/ Lava[10], Ice[10], Forest[T_FOREST_Y][T_FOREST_X], Object[257], Grave[10], Rock2[T_ROCK_Y][T_ROCK_X], FlipDungeonRockWall[5],Moulin[2],
         SapinNoel,SapinNoelA,
         Smoothing2[32], WatSmooth[32], WhiteWall[31], Haie[7], Cavern[14][14], Rampart[7], 
			CavernWall[44],GCavernWall[44], RivWat[6], BrickWall[31], DGrass[T_DGRASS_Y][T_DGRASS_X], DungeonRockWall[42], Water1[T_WATER1_Y][T_WATER1_X],Water2[T_WATER2_Y][T_WATER2_X],
			SRockFloor[4],SRockFloor2[4],SRockFloor3[4],SRockFloor4[4],SRockFloor5[4],SRockFloor6[4], Mountain[6], MountainRock[T_MOUNT_Y][T_MOUNT_X], DungeonFloor[5][5], DungeonWall[28],
         AnimWater01[T_AWATER01_N][T_AWATER01_Y][T_AWATER01_X],
         //AnimLava01[T_ALAVA01_N][T_ALAVA01_Y][T_ALAVA01_X],
         NewLava[T_NLAVA_Y][T_NLAVA_X],
         Cendre[T_CENDRE_Y][T_CENDRE_X],
         DrakeSkeleton[4],
         EscalierObject[6],
         DungeonWallSpec[5], FlipDungeonWall[5], FlipDungeonWallSpec[5], DungeonLight[5][5],
         FlipDungeonLight[5][5], DungeonWallSpec2[5], FlipDungeonWallSpec2[5], Smoothing[32],
         DungeonWallSpec3[5], FlipDungeonWallSpec3[5], DungeonOver1[10][10], DungeonOver2[15][15],
			DungeonRockFloor[6][7], DungeonRockWallSpec[5], FlipDungeonRockWallSpec[5],
			CastleWall[18],V2CastleWall[28],V2CastleWallS[21],V2CastleRampart[11], FlipCastleWall[5], DungeonRockWall2[42], FlipDungeonRockWall2[5], 
         NStucoWall[45],NMurPoutre[7],NTourGarde[4],
         DungeonRockWall3[42], FlipDungeonRockWall3[5], DungeonRockWall4[42], /*GrassBreak2[7][5],*/
         FlipDungeonRockWall4[5], DungeonRockWall2Spec[5], FlipDungeonRockWall2Spec[5],
         DungeonRockWall3Spec[5], FlipDungeonRockWall3Spec[5], DungeonRockWall4Spec[5], 
         FlipDungeonRockWall4Spec[5], Smoothing3[4], OldRock[T_OLDROCK_Y][T_OLDROCK_X], Troll[5], RuinedBrickWall[31],
		   RuinedWhiteWall[43],WoodWall[19],NBrickWall[17],NBrickWallS[21],NBrickWallS2[17],NBrickWallS3[17],NBrickWallS4[17],NBrickWallS5[17],NBrickWallS6[17],
         NBrickWallRS3[5],NBrickWallRS4[5],NBrickWallRS5[5],NBrickWallRS6[5],
         *CustomCorner[MAXCUSTOMCORNER], *CustomWall[MAXCUSTOMWALL], *CustomDarkWall[MAXCUSTOMDARKWALL],
         *CustomExcp1[MAXCUSTOMEXCP1],  *CustomDarkExcp1[MAXCUSTOMDARKEXCP1], *CustomBorder[MAXCUSTOMBORDER],
         *CustomExcp2[MAXCUSTOMEXCP2],  *CustomDarkExcp2[MAXCUSTOMDARKEXCP2],
         *CustomExcp3[MAXCUSTOMEXCP3],  *CustomDarkExcp3[MAXCUSTOMDARKEXCP3],
         *CustomExcp4[MAXCUSTOMEXCP4],  *CustomDarkExcp4[MAXCUSTOMDARKEXCP4],
         *CustomExcp5[MAXCUSTOMEXCP5],  *CustomDarkExcp5[MAXCUSTOMDARKEXCP5];
 
      //////////////////////
      // NightMare --- Add  2005-03-17
      // Add Weather CV2Sprite Sprite Attribute
      CV2Sprite m_VSFWeatherRain;
      CV2Sprite m_VSFWeatherLightning[2];

      CV2Sprite m_VSFWeatherRain1;
      CV2Sprite m_VSFWeatherSemiTransFog;

      CV2Sprite m_VSFWeatherSnow1;
      CV2Sprite m_VSFWeatherSnow2;

      //END NightMare
      //////////////////////

	  // Dialsoft new floors
	  CV2Sprite FloorLava[4][4];
	  CV2Sprite FloorIce[10][9];
		 

         
		int iCustomCorner, iCustomWall, iCustomDarkWall, iCustomBorder, 
          iCustomExcp1, iCustomDarkExcp1,
          iCustomExcp2, iCustomDarkExcp2,
          iCustomExcp3, iCustomDarkExcp3,
          iCustomExcp4, iCustomDarkExcp4,
          iCustomExcp5, iCustomDarkExcp5;
      Sprite
			Border[6];
		Sprite
			Pp1, Pp2;

		bool DrawTileSet (int xScreen, int yScreen, int, RECT *Clip, RECT *OutBound, RECT *Wrap, bool Chk, LPDIRECTDRAWSURFACE lpSurface, bool Overlap, BOOL ParamDither,UINT uiID=0,UINT uiAnimCount=0);
		void LoadVirtualMap(int, int, int);
		void ReloadMap(void);
		void CompileView(int);
   protected :
      void ValidFogMoving();

	public:
		Sprite Bug[1];				 
		
		TileSet(void);
		~TileSet(void);

		void SetTileDimension   (int, int);
		void SetViewDimension   (int, int);
		void SetVirtualDimension(int, int);
		void SetWorldDimension  (int, int);
		void SetWorldFileName   (char *);
		void SetNumberofWorld   (char);

		void SetPosition        (int, int, int, BOOL = TRUE,BOOL ForceName = TRUE);
		int  MoveToPosition     (int, int);

		void UpdateView (bool Draw, bool, int, int);
		void CreateView (void);


		void Resize(int x, int y);

		void DrawOverlap(int x, int y, int XC, int YC, int xPos, int yPos, Sprite *Over, int ID, INFO *Sp,UINT uiID);
      void DecorDrawOverlap(int x, int y, int XC, int YC, int xPos, int yPos, LPDIRECTDRAWSURFACE lpBlitSurface, int ID, INFO *Sp,UINT uiID,RECT *pRc);
      void DecorDrawOverlapID(int x, int y, int XC, int YC, int xPos, int yPos, LPDIRECTDRAWSURFACE lpBlitSurface, int ID, INFO *Sp,UINT uiID,RECT *pRc);
		void Switch(void);
      void Switch2(void);

		void Lock(int);
		void Unlock(int);

		bool Block(signed char x, signed char y);

		void GetHeight(int, int, int *, int *, signed char *);
		void DrawWaterLevel(bool bOffset,int XCor, int YCor,LPDIRECTDRAWSURFACE);

      void ChangeStatic(void);
      void Save(void);
      void Restore(void);
      BOOL GetWallType(int, int, int);

      void Register(int, char *, int, int);
      void Mathieu(void);
};

extern TileSet World;

#endif