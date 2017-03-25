/* From: util.c */
void die(char *, ...);
void warn(char *, ...);
void msg(char *, ...);
void debugSetup(char *);
void debugOptions();

/* From: audioIO_<OSTYPE>.c */
void audioOpen(int frequency, int stereo, int volume);
void audioSetVolume(int);
void audioClose();
int  audioWrite(char *, int);
int  getAudioFd();

/* From: buffer.c */
void printout(void);

/* From: audio.c */
void displayUsage();

