#include "PowerDebug.h"

#ifndef __Vircom_Bitmap_084503071597
#define __Vircom_Bitmap_084503071597

class Bitmap {
   public:
      unsigned long *BitmapData;
		unsigned long Temp;
      int  xMap;
		int  yMap;

      Bitmap(void) {
         BitmapData = NULL;
      }

      ~Bitmap(void) {
         if (BitmapData)
            delete BitmapData;
      }
      
      void SetBitmap(int x, int y) {
         xMap = x;
			yMap = y;
         BitmapData = new unsigned long [x*y];
      	memset(BitmapData, 0, x*y*sizeof(long));
      }

		void Fill(int t) {
			memset(BitmapData, t, xMap*yMap*sizeof(long));
		}

      unsigned long &operator() (int x, int y);
};

#endif