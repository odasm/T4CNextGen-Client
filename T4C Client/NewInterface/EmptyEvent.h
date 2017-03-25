// EmptyEvent.h
//

#if !defined(EmptyEvent_h)
#define EmptyEvent_h

#include "EventVisitor.h"


class EmptyEvent : public EventVisitor
{
public:
    // Empty class. Simply unabstract EventVisitor.
    static EmptyEvent &GetInstance();
};

#endif /* EmptyEvent_h */
