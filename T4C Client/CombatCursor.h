#ifndef COMBATCURSOR_HEADER
#define COMBATCURSOR_HEADER


class CombatCursorVSF : public AnimV2Sprite{
public:
    
    // Delegate to the bow cursor if in range attack.
    virtual HRESULT DrawSpriteN(LONG x, LONG y, LPV2SPRITEFX vsfFX = NULL )
    {
        return AnimV2Sprite::DrawSpriteN( x, y, vsfFX );
        /*
        if( Player.rangedAttack )
        {
            return rangedCursor.DrawSpriteN( x, y, vsfFX );
        }
        else
        {
            return AnimV2Sprite::DrawSpriteN( x, y, vsfFX );
        }
        */
    }
    // Delegate to bow cursor if in range attack.
    virtual void DrawSprite(int xPos, int yPos, RECT *Clip, RECT *OutBound, RECT *Wrap, LPDIRECTDRAWSURFACE ToBlit = lpDXDirectDraw->GetBackBufferDirect(), char FX = 0, signed char Format = 0, BOOL ParamDither = FALSE)
    {
        AnimV2Sprite::DrawSprite( xPos, yPos, Clip, OutBound, Wrap, ToBlit, FX, Format, ParamDither );
        /*
        if( Player.rangedAttack )
        {
            rangedCursor.DrawSprite( xPos, yPos, Clip, OutBound, Wrap, ToBlit, FX, Format, ParamDither );
        }
        else
        {
            AnimV2Sprite::DrawSprite( xPos, yPos, Clip, OutBound, Wrap, ToBlit, FX, Format, ParamDither );
        }
        */
    }

    virtual WORD GetWidth(VOID)
    {
        return AnimV2Sprite::GetWidth();
        /*
        if( Player.rangedAttack )
        {
            return rangedCursor.GetWidth();
        }
        else
        {
           return AnimV2Sprite::GetWidth();
        } 
        */
    }
    virtual WORD GetHeight(VOID)
    {
       return AnimV2Sprite::GetHeight();
       /*
        if( Player.rangedAttack )
        {
            return rangedCursor.GetHeight();
        }
        else
        {
            return AnimV2Sprite::GetHeight();
        }
        */
    }

    AnimV2Sprite *GetRangedCursor()
    {
        return &rangedCursor;
    }
    

private:
    AnimV2Sprite rangedCursor;
};

#endif