// EventVisitor.h
//

#if !defined(EventVisitor_h)
#define EventVisitor_h

// Abstract base class.
class GameUI;
class EventVisitor
{
public:
    EventVisitor(){
        fatherUI = 0;
    }
    
    virtual ~EventVisitor() = 0;

    // Default null implementation.
    virtual void LeftClicked(){};
    virtual void RightClicked(){};
    virtual bool CanDrag( GameUI *dragItem ){ return true; };
    virtual void Dragged(){};
    virtual bool Dropping(){ return false; };   // Target event receives that.
    virtual void Dropped( EventVisitor *sourceEvent ){};                   // Source event receives that.
    virtual void LeftDblClicked(){};
    virtual void RightDblClicked(){};

    GameUI *GetUI( void ){
        return fatherUI;
    }
    void SetUI( GameUI *newUI ){
        fatherUI = newUI;
    }

private:
    EventVisitor *sourceEvent;
    GameUI *fatherUI;

};

#endif /* EventVisitor_h */
