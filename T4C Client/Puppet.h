#ifndef __MASTER_OF_PUPPET
#define __MASTER_OF_PUPPET

#include "Icon3D.h"
#include "BasicSprite.h"
#include "TFCObject.h"

#define MAX_PUP_PART 460

// BODY PART
#define PUP_HAND_LEFT  0
#define PUP_ARM_LEFT   1
#define PUP_FOOT       2
#define PUP_LEGS       3
#define PUP_BODY       4 
#define PUP_HEAD       5
#define PUP_HAND_RIGHT 6
#define PUP_ARM_RIGHT  7
#define PUP_WEAPON	  8
#define PUP_SHIELD	  9
#define PUP_BOOT		  10
#define PUP_HAT		  11
#define PUP_CAPE		  12
#define PUP_BACKBODY   13
#define PUP_CAPE_2     14
#define PUP_HAIR       15
#define PUP_ROBELEGS   16
#define PUP_MASK       17
#define PUP_WEAPON2 	  18

// ARMOR
#define PUPEQ_NACKED       0
#define PUPEQ_LEATHER      1
#define PUPEQ_PLATE        2
#define PUPEQ_CHAIN	      3
#define PUPEQ_HORNED       4
#define PUPEQ_NECROROBE    5
#define PUPEQ_GOLDENCROWN  6
#define PUPEQ_ELVEN		   7
#define PUPEQ_STUDDED	   8
#define PUPEQ_SET1		   9
#define PUPEQ_BLACKLEATHER	10
#define PUPEQ_REDCAPE		11
#define PUPEQ_WHITEROBE    12
#define PUPEQ_MAGEROBE     13
#define PUPEQ_REDROBE      14 
#define PUPEQ_SHAMAN       15
#define PUPEQ_SKEL         16
#define PUPEQ_BLACKWINGS   17
#define PUPEQ_WHITEWINGS   18
//Dialsoft new armor 
#define PUPEQ_REDFEATHER		19
#define PUPEQ_SPIKEDLEATHER		20
#define PUPEQ_DARKWINGS			21
#define PUPEQ_NEUTRALWINGS		22
#define PUPEQ_REDWINGS			23
#define PUPEQ_GOLDWINGS			24
#define PUPEQ_DARKREDWINGS		25
#define PUPEQ_DARKGOLDWINGS		26
#define PUPEQ_LIGHTBLUEWINGS	27
#define PUPEQ_ORANGEWINGS		28
#define PUPEQ_FWHITEROBE_BLUE	29
#define PUPEQ_FWHITEROBE_GREEN	30
#define PUPEQ_FWHITEROBE_DARKBLUE	31
#define PUPEQ_FWHITEROBE_VIOLET	32
#define PUPEQ_FWHITEROBE_PURPLE	33
#define PUPEQ_FWHITEROBE_YELLOW	34
#define PUPEQ_FWHITEROBE_GRAY	35
#define PUPEQ_FWHITEROBE_MAUVE	36
#define PUPEQ_FWHITEROBE_ORANGE	37
#define PUPEQ_FWHITEROBE_BRUN	38
#define PUPEQ_FWHITEROBE_DARK	39
//Tiamat Start
#define PUPEQ_PLATE_GOLD        40
#define PUPEQ_PLATE_SILVER      41
#define PUPEQ_PLATE_BLACK       42
#define PUPEQ_PLATE_BLUE        43
#define PUPEQ_PLATE_GREEN       44
#define PUPEQ_PLATE_RED         45
#define PUPEQ_PLATE_PURPLE      46
#define PUPEQ_PLATE_EMERALD     47
//Tiamat End
//Lucifer Start
#define PUPEQ_GREENCAPE 48
#define PUPEQ_YELLOWCAPE 49
#define PUPEQ_SILVERCAPE 50
#define PUPEQ_PURPLECAPE 51
#define PUPEQ_BLACKCAPE 52
#define PUPEQ_AQUACAPE 53
#define PUPEQ_BLUECAPE 54
#define PUPEQ_WITCHHAT1 55
#define PUPEQ_WITCHHAT2 56
#define PUPEQ_WITCHHAT3 57

#define PUPEQ_BUTTERFLYWINGS1  58
#define PUPEQ_BUTTERFLYWINGS2  59
#define PUPEQ_BUTTERFLYWINGS3  60
#define PUPEQ_BUTTERFLYWINGS4  61
#define PUPEQ_ARCHWINGSW       62

#define PUPEQ_LICHROBE         63
#define PUPEQ_GOBMASK          64
#define PUPEQ_ARMORED_ROBE     65

#define PUPEQ_NOEL_HAT         66

#define PUPEQ_V2_HAUME01       67
#define PUPEQ_V2_HAUME02       68
#define PUPEQ_V2_HAUME03       69
#define PUPEQ_V2_HAUME04       70
#define PUPEQ_V2_HAT01         71
#define PUPEQ_V2_MASK01        72
#define PUPEQ_V2_GOBMASK_ICE   73

#define PUPEQ_WITCHHAT4 74
#define PUPEQ_WITCHHAT5 75

#define PUPEQ_V2_MAN_ARMOR01GR 76
#define PUPEQ_V2_MAN_ARMOR01BL 77
#define PUPEQ_V2_HAUME04_BL    78

#define PUPEQ_NM_DECHUWINGS    79
#define PUPEQ_T4CP_CASQUE      80
#define PUPEQ_T4CP_CASQUETTE   81
#define PUPEQ_WHITELEATHER     82
#define PUPEQ_NM2_DECHUWINGS   83

// Lucifer End
//Dialsoft END

// WEAPON
#define PUPEQ_AXE				1
#define PUPEQ_DARK			2
#define PUPEQ_FLAIL			3
#define PUPEQ_NORMALSWORD  4
#define PUPEQ_BATTLESWORD  5
#define PUPEQ_REALDARK		6
#define PUPEQ_MORNINGSTAR  7
#define PUPEQ_GOLDENSTAR	8
#define PUPEQ_BATTLEDAGGER 9
#define PUPEQ_WARHAMMER    10
#define PUPEQ_MACE			11
#define PUPEQ_STAFF1			12
#define PUPEQ_STAFF2			13
#define PUPEQ_STAFF3			14
#define PUPEQ_STAFF4			15
#define PUPEQ_SMALL_BOW    16
#define PUPEQ_LARGE_BOW    17
#define PUPEQ_OGRECLUB     18
#define PUPEQ_SKAVENCLUB   19
#define PUPEQ_SKAVENKNIFE  20
#define PUPEQ_SKELAXE      21
#define PUPEQ_SKELSWORD    22
#define PUPEQ_HALBERD      23
//Dialsoft new weapon
#define PUPEQ_FIREFLAIL    24
#define PUPEQ_TIGERAXE			25
#define PUPEQ_ANCIENTAXE		26
#define PUPEQ_FLOWERS			27
#define PUPEQ_NECROSTAFF		28
#define PUPEQ_NEW_BOW01       29
#define PUPEQ_DWARF_HAMMER    30
#define PUPEQ_AXE_DESTRUCTION 31
#define PUPEQ_SWORD_ANGEL     32
#define PUPEQ_HAMM            33
#define PUPEQ_CLAY_2BLADES    34
#define PUPEQ_SWORD_HERO      35
#define PUPEQ_V2_DAGGER_VIP   36
#define PUPEQ_V2_DAGGER_01    37
#define PUPEQ_V2_DAGGER_02    38
#define PUPEQ_V2_DAGGER_03    39
#define PUPEQ_V2_DAGGER_04    40
#define PUPEQ_V2_DAGGER_05    41
//NM_TAG_ADD_SWORD
#define PUPEQ_V2_BUSSWORD01       42
#define PUPEQ_V2_BUSSWORD02       43
#define PUPEQ_V2_BUSSWORD03       44
#define PUPEQ_V2_CLAYSWORD01      45
#define PUPEQ_V2_CLAYSWORD02      46
#define PUPEQ_V2_GLAIVESWORD01    47
#define PUPEQ_V2_GLAIVESWORD02    48
#define PUPEQ_V2_SWORD01          49
#define PUPEQ_V2_SWORD02          50
#define PUPEQ_V2_SWORD03          51
#define PUPEQ_V2_SWORD04          52
#define PUPEQ_V2_SWORD05          53
#define PUPEQ_V2_SWORD06          54
#define PUPEQ_V2_SWORD07          55
#define PUPEQ_V2_SWORD08          56
#define PUPEQ_V2_2BUSSWORD01      57
#define PUPEQ_V2_2BUSSWORD02      58
#define PUPEQ_V2_2BUSSWORD03      59
#define PUPEQ_V2_2CLAYSWORD01     60
#define PUPEQ_V2_2CLAYSWORD02     61
#define PUPEQ_V2_2GLAIVESWORD01   62
#define PUPEQ_V2_2GLAIVESWORD02   63
#define PUPEQ_V2_2SWORD01         64
#define PUPEQ_V2_2SWORD02         65
#define PUPEQ_V2_2SWORD03         66
#define PUPEQ_V2_2SWORD04         67
#define PUPEQ_V2_2SWORD05         68
#define PUPEQ_V2_2SWORD06         69
#define PUPEQ_V2_2SWORD07         70
#define PUPEQ_V2_2SWORD08         71
#define PUPEQ_V2_HACHE01          72
#define PUPEQ_V2_HACHE02          73
#define PUPEQ_V2_HACHE03          74
#define PUPEQ_V2_HACHE04          75
#define PUPEQ_V2_HACHE05          76
#define PUPEQ_V2_HACHE06          77
#define PUPEQ_V2_HACHE07          78
#define PUPEQ_V2_HACHE08          79
#define PUPEQ_V2_HACHE09          80
#define PUPEQ_V2_HACHE10          81
#define PUPEQ_V2_HACHE11          82
#define PUPEQ_V2_2HACHE01         83
#define PUPEQ_V2_2HACHE02         84
#define PUPEQ_V2_2HACHE03         85
#define PUPEQ_V2_2HACHE04         86
#define PUPEQ_V2_2DAGUE01         87
#define PUPEQ_V2_2DAGUE02         88
#define PUPEQ_V2_2DAGUE03         89
#define PUPEQ_V2_2DAGUE04         90
#define PUPEQ_V2_2DAGUE05         91
#define PUPEQ_V2_2DAGUE06         92
#define PUPEQ_V2_BOW01            93
#define PUPEQ_V2_BOW02            94
#define PUPEQ_V2_BOW03            95
#define PUPEQ_V2_BOW04            96
#define PUPEQ_V2_BOW05            97
#define PUPEQ_V2_BOW06            98
#define PUPEQ_V2_BOW07            99
#define PUPEQ_V2_BOW08           100
#define PUPEQ_V2_BOW09           101
#define PUPEQ_V2_BOW10           102
#define PUPEQ_V2_SCEPTRE01       103
#define PUPEQ_V2_SCEPTRE02       104
#define PUPEQ_V2_SCEPTRE03       105
#define PUPEQ_V2_SP01            106
#define PUPEQ_V2_SP02            107
#define PUPEQ_V2_SP03            108
#define PUPEQ_V2_SP04            109
#define PUPEQ_V2_SP05            110
#define PUPEQ_V2_SP06            111
#define PUPEQ_V2_SP07            112
#define PUPEQ_V2_2SP01           113
#define PUPEQ_V2_2SP02           114
#define PUPEQ_V2_2SP03           115
#define PUPEQ_V2_2SP04           116
#define PUPEQ_V2_2SP05           117
#define PUPEQ_V2_2SP06           118
#define PUPEQ_V2_HAMMER01        119
#define PUPEQ_V2_HAMMER02        120
#define PUPEQ_V2_HAMMER03        121
#define PUPEQ_V2_HAMMER04        122
#define PUPEQ_V2_HAMMER05        123
#define PUPEQ_V2_HAMMER06        124
#define PUPEQ_V2_HAMMER07        125
#define PUPEQ_V2_2HAMMER01       126
#define PUPEQ_V2_2HAMMER02       127
#define PUPEQ_V2_2HACHE05        128
#define PUPEQ_V2_2HACHE06        129
#define PUPEQ_V2_GLAIVESWORD02_ICE   130
#define PUPEQ_V2_2GLAIVESWORD02_ICE  131
#define PUPEQ_V2_BOW05_ICE           132
#define PUPEQ_T4CP_MATRAQUE          133
#define PUPEQ_T4CP_BAT               134
#define PUPEQ_T4CP_12                135





//Dialsoft END

// SHIELD
#define PUPEQ_ROMANSHIELD     1
#define PUPEQ_BAROSSASHIELD   2
#define PUPEQ_ORCSHIELD		   3
#define PUPEQ_CENTAURSHIELD1  4
#define PUPEQ_CENTAURSHIELD2  5
#define PUPEQ_SKAVENSHIELD1   6
#define PUPEQ_SKAVENSHIELD2   7
#define PUPEQ_SKAVENSHIELD3   8
#define PUPEQ_SKELSHIELD      9
#define PUPEQ_ORCSHIELD2     10
#define PUPEQ_SKSHIELD       11
#define PUPEQ_V2_SHIELD01    12
#define PUPEQ_V2_SHIELD02    13
#define PUPEQ_T4CP_SHIELD    14
#define PUPEQ_V2_ICESHIELD01 15

// HAIR
#define PUPEQ_HAIR1         1



class Puppet : public BasicSprite {
public:
   BYTE     BodyOrder[10][32][13];
   BYTE     BodyOrderA[10][32][9];
   BYTE     BodyOrderAR[10][32][9];
   BYTE     BodyOrderR[10][32][13];
   
   static UINT Pow2[32];
   static BOOL Pow2Init;
   
   Sprite3D PuppetPart[MAX_PUP_PART];
   
   Puppet(void);
   ~Puppet(void);
   
   void DrawSprite3D(int Direction, int Frame, int X, int Y, BYTE Stand, RECT *Clip, BOOL boOutline, BYTE bOutlineColor, LPDIRECTDRAWSURFACE Surface, signed char Format, TFCObject *);
   void DrawSprite3DA(int Direction, int Frame, int X, int Y, RECT *Clip, BOOL boOutline, BYTE bOutlineColor, LPDIRECTDRAWSURFACE Surface, signed char Format, TFCObject *);
   void DrawSprite3DR(int Direction, int Frame, int X, int Y, RECT *Clip, BOOL boOutline, BYTE bOutlineColor, LPDIRECTDRAWSURFACE Surface, signed char Format, TFCObject *);
   void DrawCorpse(int, int X, int Y, RECT *Clip, RECT *OutBound, RECT *Wrap, LPDIRECTDRAWSURFACE Surface, signed char Format, int AbsX, int AbsY, TFCObject *);
   void LoadBodyPart(void);
   void SetPuppet(TFCObject *);
   
   // Pas implementer
   INFO *GetCurrentVSF(int Direction, int Frame, BYTE Stand) { LastAccess = timeGetTime(); INFO *x = new INFO; ZeroMemory(x, sizeof(INFO)); return x; };
   int  GetTextCorrection(void) { LastAccess = timeGetTime(); return -20; };
   
   void Release(void);
   BOOL Release(DWORD);
   
   /*
   int MaxWidth;
   int MaxHeight;
   int MaxxCor;
   int MaxyCor;
   signed char StAtt, StMov;
   
     char Type, Faces, FacesA, FacesC, FacesStA, FacesStM;
   */
   
};

#endif