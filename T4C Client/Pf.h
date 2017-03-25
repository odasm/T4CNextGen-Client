#ifndef __PF_H
#define __PF_H

void  pfSetPosition(int x, int y);
BOOL  pfIsAtPosition(void);
short pfGetNextMovement(void);
void  pfStopMovement(char *);
void  pfNearPosition(void);

extern int  pfSaveXPosition;
extern int  pfSaveYPosition;
extern BOOL pfBoolStop;

#endif