// GraphUI.h
//

#if !defined(GraphUI_h)
#define GraphUI_h

#include "ControlUI.h"


class EventVisitor;

// A graph UI provides a way to use external CV2Sprite sprites.
class GraphUI : public ControlUI
{
public:
    GraphUI() : vsfGraphSprite( NULL ){
        
    }
    
    void SetVSF( CV2Sprite *vsfSprite )
    {
        vsfGraphSprite = vsfSprite;
    }

    void Draw(  V2SPRITEFX *vsfFX)
    {
        // If a graph sprite was provided.
        if( vsfGraphSprite != NULL )
        {
            // Draw it.
            vsfGraphSprite->DrawSpriteN( GetPos().x, GetPos().y, vsfFX );
        }
        else
        {
            // Draw the normal sprite otherwise.
            GameUI::Draw( vsfFX);
        }
    };

  
    virtual CV2Sprite *GetVSF( void )
    {
       if(vsfGraphSprite == NULL)
           return GameUI::GetVSF();
       else
           return vsfGraphSprite;
    }

    virtual bool IsSpriteDefined(){
        if( vsfGraphSprite == NULL ){
            return GameUI::IsSpriteDefined();
        }
        return true;
    }


private:
    CV2Sprite *vsfGraphSprite;
};

#endif /* GraphUI_h */
