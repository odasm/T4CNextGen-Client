// SliderUI.cpp: implementation of the SliderUI class.
//
//////////////////////////////////////////////////////////////////////
#include "../pch.h"
#include "SliderUI.h"

namespace{

    T3VSBSound *buttonDownSound = NULL;
    T3VSBSound *buttonUpSound = NULL;

    void InitSound(){
        if( !buttonDownSound ){
            buttonDownSound = new T3VSBSound;
            buttonUpSound = new T3VSBSound;

            buttonDownSound->Create( "Generic Drop Item", TS_MEMORY );
            buttonUpSound->Create( "Button Release Sound", TS_MEMORY );
        }
    }
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SliderUI::SliderUI( EventVisitor &iEvent )  
:
    eventVisitor( iEvent ),
    step( 1 ),
    minRange( 0 ),
    maxRange( 100 ),
    curPos( 0 ),
    dragging( false ),
    rightArrowDown( false ),
    leftArrowDown( false ),
    soundEnabled( true )
{
    InitSound();

    prevPos = curPos;

    qsStart.SetPos( FramePos( 0, 0, 8, 7 ) );
    //qsStart.SetSpriteId( "64kOptionSliderEnd1" );

    qsEnd.SetPos( FramePos( 0, 0, 9, 8 ) );
    //qsEnd.SetSpriteId( "64kOptionSliderEnd2" );

    qsMiddle.SetPos( FramePos( 0, 0, 1, 5 ) );
    //qsMiddle.SetSpriteId( "64kOptionSliderMiddle" );

    sliderGraph.SetPos( FramePos( 0, 0, 24, 22 ) );
    sliderGraph.SetSpriteId( "GUI_ScrollTick" );
}

SliderUI::~SliderUI()
{

}

//////////////////////////////////////////////////////////////////////////////////////////
void SliderUI::SetRange
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the new slider range
// 
(
 int iminRange,
 int imaxRange,
 FramePos framePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    minRange = iminRange;
    maxRange = imaxRange;
    sliderArea = framePos;
}
//////////////////////////////////////////////////////////////////////////////////////////
void SliderUI::SetStep
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the step.
// 
(
 int istep
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    step = istep;
}
//////////////////////////////////////////////////////////////////////////////////////////
void SliderUI::SetSliderPos
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the slider's position 
// 
(
 int ipos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    curPos = ipos;
    prevPos = curPos;
}

//////////////////////////////////////////////////////////////////////////////////////////
int SliderUI::GetSliderPos( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Return: int, The position
//////////////////////////////////////////////////////////////////////////////////////////
{
    return curPos;
}

//////////////////////////////////////////////////////////////////////////////////////////
void SliderUI::SetSliderGraph
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the graphic to display as slider button
// 
(
 GraphUI &graphUI
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    sliderGraph.SetSpriteId( graphUI.GetSpriteId() );
    sliderGraph.SetPos( graphUI.GetPos() );
}


//////////////////////////////////////////////////////////////////////////////////////////
void SliderUI::Draw
//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the sliderUI.
// 
(
 V2SPRITEFX *vsfFX
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If in drag mode and left mouse is down.
    if( dragging && IsLeftMouseDown() ){
        MousePos mousePos( MouseDevice::xPos, MouseDevice::yPos );
        
        // Need to be within the slider's width
        if( mousePos.x >= sliderArea.x && mousePos.x <= sliderArea.maxx ){
            // Do as a click.
            LeftMouseDown( mousePos );
        }
    }else{
        dragging = false;
    }
    
    if( rightArrowDown && IsLeftMouseDown() ){
        scrollRightSprite.Draw( vsfFX );
    }else{
        rightArrowDown = false;
    }
    if( leftArrowDown && IsLeftMouseDown() ){
        scrollLeftSprite.Draw( vsfFX );
    }else{
        leftArrowDown = false;
    }

    
    // If a graph was set (not a quicksilver style slider).
    if( sliderGraph.IsSpriteDefined() ){
        int x, y;

        FramePos sliderPos = sliderGraph.GetPos();

        // Set the slider's y position.
        y = sliderArea.Height() / 2 + sliderArea.y - sliderPos.Height() / 2;


        double relPos = static_cast< double >( curPos - minRange ) / 
                        static_cast< double >( maxRange - minRange );

        x = (int)(relPos * sliderArea.Width() - relPos * ( sliderPos.Width() / 2 ) + sliderArea.x);

        sliderPos.SetNewOffset( MousePos( x, y ) );

        sliderGraph.SetPos( sliderPos );

        sliderGraph.Draw( vsfFX );
    }else{
        FramePos thisPos = sliderArea;
        FramePos midPos = qsMiddle.GetPos();
        FramePos endPos = qsEnd.GetPos();
        FramePos startPos = qsStart.GetPos();
        int endX, y;

        
        // Set the slider's y position.
        y = thisPos.Height() / 2 + thisPos.y - midPos.Height() / 2;
        
        double relPos = static_cast< double >( curPos - minRange ) / 
                        static_cast< double >( maxRange - minRange );

        // Display the start part
        FramePos oldPos = startPos;
        oldPos.SetNewOffset( MousePos( thisPos.x, y ) );
        
        qsStart.SetPos( oldPos );
        qsStart.Draw( vsfFX );

        // Calculate the ending part
        endX = (int)(relPos * sliderArea.Width() - relPos * 
            ( midPos.Width() / 2 ) + sliderArea.x - endPos.Width());

        

        // Display the middle part in between.
        int i;
        for( i = thisPos.x + startPos.Width(); i < endX; i += midPos.Width() ){
            oldPos = midPos;
            oldPos.SetNewOffset( MousePos( i, y ) );
            qsMiddle.SetPos( oldPos );
            qsMiddle.Draw( vsfFX );
        }

        // Display the ending part.
        oldPos = endPos;
        oldPos.SetNewOffset( MousePos( endX, y ) );
        qsEnd.SetPos( oldPos );

        qsEnd.Draw( vsfFX );


    }
}

//////////////////////////////////////////////////////////////////////////////////////////
bool SliderUI::LeftMouseUp
//////////////////////////////////////////////////////////////////////////////////////////
// Mouse up
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
   if( panLeft.MousePosInFrame( mousePos ) ){
      InitSound();
      
      if( soundEnabled ){
         buttonUpSound->Play( FALSE );
      }
      
      curPos -= step;
      if( curPos < minRange ){
         curPos = minRange;
      }
      
      // If the positions changed.
      if( prevPos != curPos ){
         // Call the event visitor.
         eventVisitor.LeftClicked();
      }
      prevPos = curPos;
      
      return true;
   }else if( panRight.MousePosInFrame( mousePos ) ){
      InitSound();
      
      if( soundEnabled ){
         buttonUpSound->Play( FALSE );
      }
      
      curPos += step;
      if( curPos > maxRange ){
         curPos = maxRange;
      }
      
      // If the positions changed.
      if( prevPos != curPos ){
         // Call the event visitor.
         eventVisitor.LeftClicked();
      }
      prevPos = curPos;
      
      return true;
   }
   return false;
}
//////////////////////////////////////////////////////////////////////////////////////////
bool SliderUI::LeftMouseDown
//////////////////////////////////////////////////////////////////////////////////////////
//  Mouse down on the slider UI.
// 
(
 MousePos mousePos // The position
)
//////////////////////////////////////////////////////////////////////////////////////////
{
   if( !sliderArea.MousePosInFrame( mousePos ) ){
      if( panLeft.MousePosInFrame( mousePos ) ){
         InitSound();
         
         if( soundEnabled ){
            buttonDownSound->Play( FALSE );
         }
         
         leftArrowDown = true;
         return true;
      }else if( panRight.MousePosInFrame( mousePos ) ){
         InitSound();
         
         if( soundEnabled ){
            buttonDownSound->Play( FALSE );
         }
         
         rightArrowDown = true;
         return true;
      }        
      return false;
   }
   
   int halfStep = sliderArea.Width() / ( maxRange - minRange ) * step / 2;
   
   double relPos = static_cast< double >( mousePos.x - sliderArea.x + halfStep ) / 
      static_cast< double >( sliderArea.Width() );
   
   double absRange = static_cast< double >( maxRange - minRange ) / 
      static_cast< double >( step );
   
   int newAbsPos = static_cast< int >( relPos * absRange );
   
   curPos = ( newAbsPos * step ) + minRange;
   
   dragging = true;
   
   // If the positions changed.
   if( prevPos != curPos ){
      InitSound();
      
      if( soundEnabled ){
         buttonUpSound->Play( FALSE );
      }

      // Call the event visitor.
      eventVisitor.LeftClicked();
   }
   prevPos = curPos;
   
   return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void SliderUI::SetQuickSilverGraph
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the graph UI for a 'quick silver' style slider.
// 
(
 std::string startId,   // The starting part of the bar.
 std::string endId,      // The end part of the bar.
 std::string middleId   // The middle part of the bar.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    qsStart.SetSpriteId( startId );
    qsEnd.SetSpriteId( endId );
    qsMiddle.SetSpriteId( middleId );
}

//////////////////////////////////////////////////////////////////////////////////////////
void SliderUI::SetScrollRegions
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the scroll regions
// 
(
 FramePos scrollLeft, // <- 
 FramePos scrollRight, // ->
 std::string leftSpriteId, // Id of the pressed left arrow sprite
 std::string rightSpriteId // If of the pressed right arrow sprite
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    panLeft = scrollLeft;
    panRight = scrollRight;

    if(leftSpriteId.c_str()[0] != NULL)
    {
       scrollLeftSprite.SetSpriteId( leftSpriteId );
       scrollRightSprite.SetSpriteId( rightSpriteId );
    
       scrollLeftSprite.SetPos( scrollLeft );
       scrollRightSprite.SetPos( scrollRight );

       scrollLeftSprite.Show();
       scrollRightSprite.Show();
    }
}