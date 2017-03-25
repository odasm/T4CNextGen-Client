// EffectStatusUI.h
//

#if !defined(EffectStatusUI_h)
#define EffectStatusUI_h

#include "BoxUI.h"
#include "GraphUI.h"
#include "../ScrollUI.h"
#include <string>
#include <list>
#include <vector>

class EffectStatusUI : public BoxUI, public ScrollUI
{
public:
    virtual void DrawMinimized( V2SPRITEFX *vsfFX );

    static EffectStatusUI *GetInstance();


    void Enable( bool newState )
        { enabled = newState; };

    bool IsEnabled()
        { return enabled; }

    void AddEffect( DWORD id, DWORD time, DWORD totalTime, DWORD iconId, std::string desc );
    void DispellEffect( DWORD id );    

    bool LeftMouseDown( MousePos mousePos ); // For scroll buttons.

private:
    EffectStatusUI();

    class Effect : public GraphUI{
    public:
        bool operator < ( const Effect &eff ) const{
            return expireTime < eff.expireTime;
        }

        DWORD id;
        DWORD expireTime;
        DWORD initialTime;
        std::string desc;
        bool infinite;
    };

    class EffectDisplay : public GraphUI{
    public:
        EffectDisplay() : effect( NULL ){};

        // Delegate drawing operations to the underlying effect object.
        virtual void Draw( V2SPRITEFX *vsfFX );

        virtual bool LeftMouseUp( MousePos mousePos );
        
        Effect *effect;

        GraphUI emptyGauge;
    };

    void CalcVisibleEffects( void );

    void CalcEffectInfo( void );

    // Called by ScrollUI when the scroll bar has changed.
    virtual void ScrollChanged( void );

    std::vector< EffectDisplay > displayedEffects;
    std::list< Effect > effects;

    class ScrollUpEvent : public EventVisitor{
    public:
        void LeftClicked();
    } scrollUpEvent;

    class ScrollDownEvent : public EventVisitor{
    public:
        void LeftClicked();
    } scrollDownEvent;

    friend ScrollDownEvent;
    friend ScrollUpEvent;

    ButtonUI scrollUpButton;
    ButtonUI scrollDownButton;


    bool scrollEnabled;
    bool enabled;
};

#endif /* EffectStatusUI_h */
