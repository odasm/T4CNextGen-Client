#ifndef __AUTOLOCK_H
#define __AUTOLOCH_H

class Autolock {
	public:
		Autolock(CRITICAL_SECTION *);
		~Autolock(void);
	private:
		CRITICAL_SECTION *m_cs;
};

#endif