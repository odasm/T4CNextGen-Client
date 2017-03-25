/* this file is a part of amp software, (C) tomislav uzelac 1996,1997
*/

/* audio.h  some global variables
 *
 * Created by: tomislav uzelac Mar/Apr, Jul 96
 * Last modified by:
 */

#include <stdio.h>
#include <Windows.h>

extern unsigned char buffer[];
extern int append;
extern int data;

/* buffer for the 'bit reservoir'
*/
#define BUFFER_SIZE     4096
#define BUFFER_AUX      2048


struct AUDIO_HEADER {
	int ID;
	int layer;
	int protection_bit;
	int bitrate_index;
	int SamplingFrequency;
	int padding_bit;
	int private_bit;
	int mode;
	int mode_extension;
	int copyright;
	int original;
	int emphasis;
};

struct SIDE_INFO {
	int main_data_begin;
	int scfsi[2][4];
	int part2_3_length[2][2];
	int big_values[2][2];
	int global_gain[2][2];
	int scalefac_compress[2][2];
	int window_switching_flag[2][2];
	int block_type[2][2];
	int mixed_block_flag[2][2];
	int table_select[2][2][3];
	int subblock_gain[2][2][3];
	int region0_count[2][2];
	int region1_count[2][2];
	int preflag[2][2];
	int scalefac_scale[2][2];
	int count1table_select[2][2];
};


/* global stuff 
*/

extern FILE *in_file,*out_file;
extern char *in_buffer;

extern int scalefac_l[2][2][22];
extern int scalefac_s[2][2][13][3];
extern int t_b8_l[2][3][22];
extern int t_b8_s[2][3][13];
extern short t_bitrate[2][3][15];

extern int is[2][578];
extern double xr[2][32][18];

extern int *t_l,*t_s;
extern int nch;
extern int t_sampling_frequency[2][3];

extern int SHOW_HEADER,SHOW_HEADER_DETAIL;
extern int SHOW_SIDE_INFO,SHOW_SIDE_INFO_DETAIL;
extern int SHOW_MDB,SHOW_MDB_DETAIL;
extern int SHOW_HUFFMAN_ERRORS,SHOW_HUFFBITS;
extern int SHOW_SCFSI,SHOW_BLOCK_TYPE,SHOW_TABLES,SHOW_BITRATE;
extern int A_QUIET,A_SHOW_CNT,A_FORMAT_WAVE;
extern int A_WRITE_TO_AUDIO,A_WRITE_TO_FILE;
extern short pcm_sample[64];
extern int A_SET_VOLUME,A_SHOW_TIME;
extern int A_MSG_STDOUT;
extern int A_DONT_PLAY;

#define i_sq2   0.707106781188
#define IS_ILLEGAL 0xfeed

extern const short t_reorder[2][3][576];
extern const double t_is[7];
extern const double t_is2[2][32];

/* ...
*/

void bread(void *buffer, size_t size, size_t count);
void bopen(char *);
extern char *our_buffer;

#ifdef AUDIO

FILE *in_file,*out_file;
char *in_buffer;
 
int scalefac_l[2][2][22];
int scalefac_s[2][2][13][3];

int is[2][578];
double xr[2][32][18];

int *t_l,*t_s;
int nch;
int t_sampling_frequency[2][3] = {
{ 22050 , 24000 , 16000},
{ 44100 , 48000 , 32000}
};

int SHOW_HEADER,SHOW_HEADER_DETAIL;
int SHOW_SIDE_INFO,SHOW_SIDE_INFO_DETAIL;
int SHOW_MDB,SHOW_MDB_DETAIL;
int SHOW_HUFFMAN_ERRORS,SHOW_HUFFBITS;
int SHOW_SCFSI,SHOW_BLOCK_TYPE,SHOW_TABLES,SHOW_BITRATE;
int A_QUIET,A_SHOW_CNT,A_FORMAT_WAVE,A_DUMP_BINARY;
int A_WRITE_TO_FILE;
int A_SET_VOLUME, A_SHOW_TIME;
int A_MSG_STDOUT;
int A_DONT_PLAY;
short pcm_sample[64];

short t_bitrate[2][3][15] = {{
{0,32,48,56,64,80,96,112,128,144,160,176,192,224,256},
{0,8,16,24,32,40,48,56,64,80,96,112,128,144,160},
{0,8,16,24,32,40,48,56,64,80,96,112,128,144,160}
},{
{0,32,64,96,128,160,192,224,256,288,320,352,384,416,448},
{0,32,48,56,64,80,96,112,128,160,192,224,256,320,384},
{0,32,40,48,56,64,80,96,112,128,160,192,224,256,320}
}};

/* the last sfb is given implicitly on pg.28. of the standard. scalefactors 
 * for that one are 0, pretab also 
 */
/* leftmost index denotes ID, so first three tables are for MPEG2 (header->ID==0)
 * and the other three are for MPEG1 (header->ID==1)
 */
/* 22.05, 24, 16 */
int t_b8_l[2][3][22]={{ /* table B.8b ISO/IEC 11172-3 */
{5,11,17,23,29,35,43,53,65,79,95,115,139,167,199,237,283,335,395,463,521,575},
{5,11,17,23,29,35,43,53,65,79,95,113,135,161,193,231,277,331,393,463,539,575},
{5,11,17,23,29,35,43,53,65,79,95,115,139,167,199,237,283,335,395,463,521,575}
},{
{3,7,11,15,19,23,29,35,43,51,61,73,89,109,133,161,195,237,287,341,417,575},
{3,7,11,15,19,23,29,35,41,49,59,71,87,105,127,155,189,229,275,329,383,575},
{3,7,11,15,19,23,29,35,43,53,65,81,101,125,155,193,239,295,363,447,549,575}
}};   
int t_b8_s[2][3][13]={{ /* table B.8b ISO/IEC 11172-3 */
{3,7,11,17,23,31,41,55,73,99,131,173,191},
{3,7,11,17,25,35,47,61,79,103,135,179,191},
{3,7,11,17,25,35,47,61,79,103,133,173,191}
},{
{3,7,11,15,21,29,39,51,65,83,105,135,191},
{3,7,11,15,21,27,37,49,63,79,99,125,191},
{3,7,11,15,21,29,41,57,77,103,137,179,191}
}};

#endif /* AUDIO */

class VSoundBuffer {
   private:
   public:
	  static SIDE_INFO *Info;
      static AUDIO_HEADER *Header;
      
      static HRESULT GetHdr(void); 
      static void _FillBuffer(int);
      static unsigned int _GetBits(int);
      static unsigned int GetBits(int);
      static unsigned int ViewBits(int);
      static void SackBits(int);
      static void FillBuffer(int);
      static void GetCrc(void);
      static void GetInfo(void);
      static int  Layer3Decompress(int);
      static int HuffmanDecode(int, int *, int *);
      static int DecodeScaleFactors(int,int);
      static HRESULT ProcessHeader(int);
      static int DecodeHuffmanData(int,int,int);
      static int _QSign(int,int *);
	  static void RequantizeMS(int gr);
	  static int FindIsBound(int isbound[3],int gr);
	  static void Stereo_S(int l,double a[2],int pos,int ms_flag,int is_pos);
	  static void Stereo_L(int l,double a[2],int ms_flag,int is_pos);
	  static void AliasReduction(int ch);
	  static void RequantizeMono(int gr,int ch);
	  static double Fras_L(int sfb,int global_gain,int scalefac_scale,int scalefac,int preflag);
	  static double Fras_S(int global_gain,int subblock_gain,int scalefac_scale,int scalefac);
	  static double Fras2(int is,double a);
	  static void Premultiply(void);
	  static void Poly(const int ch,int f);
	  static void IMdct(int win_type,int sb,int ch);
	  static void IMdctInit(void);

   public:
      static void decodeMPEG(char *, char **Decompress, DWORD *Len);
      VSoundBuffer(void);
};

extern int non_zero[2];
extern int is_max[21]; /* the maximum value of is_pos. for short blocks is_max[sfb=0] == is_max[6],
	   	 * it's sloppy but i'm sick of waisting storage. blaah...
		 */
extern int intensity_scale;

/* these should not be touched
*/
#define		SYNCWORD	0xfff
#define		TRUE		1
#define		FALSE		0
/* version 
*/
#define		MAJOR		0
#define		MINOR		7
#define		PATCH		3

#define MAX(a,b)  ((a) > (b) ? (a) : (b))
#define MIN(a,b)  ((a) < (b) ? (a) : (b))


/* Debugging flags */

struct debugFlags_t {
  int audio,args,buffer,buffer2,misc,misc2;
};

struct debugLookup_t {
  char *name; int *var;
};

extern struct debugFlags_t debugFlags;

/* This is only here to keep all the debug stuff together */
#ifdef AMP_UTIL
struct debugLookup_t debugLookup[] = {
  {"audio", &debugFlags.audio},
  {"args",  &debugFlags.args},
  {"buffer",  &debugFlags.buffer},
  {"buffer2",  &debugFlags.buffer2},
  {"misc",  &debugFlags.misc},
  {"misc2",  &debugFlags.misc2},
  {0,0}
};
#endif /* AMP_UTIL */

extern struct debugLookup_t debugLookup[];


#ifdef DEBUG
  #define DB(type,cmd) if (debugFlags.type) { cmd ; }	
#else
  #define DB(type,cmd)
#endif


#include "proto.h"

extern short stereo_samples[18][32][2];
extern short mono_samples[18][32];
extern double res[32][18];
extern double s[2][32][18];

