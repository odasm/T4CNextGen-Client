#pragma warning( disable : 4291 )

#include "pch.h"
#include "NewFont.h"
#include "PowerDebug.h"
#include "IDirectXDraw.h"
#include "FormatText.h"
#include "Packet.h"
#include "MemoryManager.h"
#include "Global.h"
#include "SaveGame.h"
extern CSaveGame g_SaveGame;
 
int CALLBACK FontManagerCallBack
//**************************************************************************************//
// Load all the Current System Font.
// 
(
 LPLOGFONT lplf,    // Logical Description of a Font.
 LPTEXTMETRIC lptm, // Physical Description of a Font.
 DWORD FontType,    // FontType.
 LPARAM lParam      // User-Defined Parameter.
 )
 // Return: int, 1 Continue enumeration, 0 Stop enumeration 
 //**************************************************************************************//
{
    FontList *fl = new FontList;
    
    memcpy(fl->lplfLogical, lplf, sizeof(LOGFONT));
    fl->lpszFontName = new TCHAR [strlen(lplf->lfFaceName)+1];
    strcpy(fl->lpszFontName, lplf->lfFaceName);
    
    FontManager::GetInstance()->AddToTail(fl);
    return 1; 
} 

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

FontManager::FontManager(void) {
    // Constructor
    lpFont = NULL;
}

FontManager::~FontManager(void) {
    // Destructor
    Release();
}

void FontManager::Release(void) {   
    ToHead();
    while(QueryNext()) {
        FontList *flObject;
        flObject = GetObject();
        RemoveObject();
        delete flObject;
    }
    if (lpFont) {
        delete [] lpFont;
        lpFont = NULL;
    }
}

FontManager *FontManager::GetInstance(void) {
    // Instance
    static FontManager Instance;
    return &Instance;
}

void FontManager::Initialize(void) {
    // Initialize Font Manager.
    HDC hdc;
    
    DXDGetDC(&hdc, 14);
    
    EnumFonts(hdc, NULL, (FONTENUMPROC) FontManagerCallBack, NULL);
    
    lpFont = new FontObject [10];
    
    CreateFont( "Tahoma"       , &lpFont[0], 1);//BLBLBL System=>Tahoma
    CreateFont( "Verdana"      , &lpFont[1], 14); 
    CreateFont( "T4C Beaulieux", &lpFont[2], 22 );
    CreateFont( "Verdana"      , &lpFont[3], 18); 
    CreateFont( "T4C Beaulieux", &lpFont[4], 26 );
    
    
    DXDReleaseDC(hdc, 14);
}

void FontManager::CreateFont(LPTSTR lpszFontName, FontObject *foReturn, double Pc) {
    FontList *flObject;
    bool Found = false;
    
    ToHead();
    while(QueryNext()) {
        flObject = GetObject();
        if (!strcmp(flObject->lpszFontName, lpszFontName)) {
            foReturn->Create(flObject->lplfLogical, Pc);
            Found = true;
            SetQueryState(STOP_QUERY);
        }
    }
    
    if (!Found) {
        ToHead();
        while(QueryNext()) {
            flObject = GetObject();
            if (!strcmp(flObject->lpszFontName, "System")) {
                foReturn->Create(flObject->lplfLogical, Pc);
                Found = true;
                SetQueryState(STOP_QUERY);
            }
        }
    }
}

TextObject *FontManager::GetSmallFont(BOOL bValidSize,LPCTSTR szDefaultText) {
    TextObject *toReturn = new TextObject;
    

    if(bValidSize && g_SaveGame.GetOptionsParam()->bHighFont)
       toReturn->foFont = &lpFont[3];
    else
       toReturn->foFont = &lpFont[1];
    

    
    if( szDefaultText ){
        toReturn->SetText( const_cast< char * >( szDefaultText ) );
    }
    
    return toReturn;
};

TextObject *FontManager::GetTalkFont(BOOL bValidSize){
    TextObject *toReturn = new TextObject;
    
    if(bValidSize && g_SaveGame.GetOptionsParam()->bHighFont)
       toReturn->foFont = &lpFont[4];
    else
       toReturn->foFont = &lpFont[2];
    
    
    return toReturn;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

FontObject::FontObject(void) {
    // Constructor
}

FontObject::~FontObject(void) {
    // Destructor
}

void FontObject::Create(LPLOGFONT lplf, double Pc) {
    //HDC hdc;
    //DXDGetDC(&hdc, 15);
    
    LPLOGFONT lplfLogical = new LOGFONT;
    memcpy(lplfLogical, lplf, sizeof(LOGFONT));
    
    //LOG.Lock();
    //LOG << "\r\nLOGPIXELSY=" << GetDeviceCaps(hdc, LOGPIXELSY) << ".!";
    //LOG.Unlock();
    
    //pdp->lf.lfPitchAndFamily = 
    //    (0xF0 & pdp->lf.lfPitchAndFamily) | VARIABLE_PITCH ;  
    lplfLogical->lfWidth  = 0;//(long)(lplfLogical->lfWidth  * Pc);
    lplfLogical->lfHeight = Pc;//-MulDiv(5, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    //(long)(lplfLogical->lfHeight * Pc);
    lplfLogical->lfQuality = NONANTIALIASED_QUALITY;    
    
    hf = CreateFontIndirect(lplfLogical);
    delete lplfLogical;
    //DXDReleaseDC(hdc, 15);
}

double FontObject::GetHeight(HDC hdc) {
    SIZE sz;
    
    SelectObject(hdc, hf);
    GetTextExtentPoint32(hdc, "Xy", 1, &sz); 
    
    return sz.cy;
}

double FontObject::GetWidth(HDC hdc, LPCTSTR s) {
    SIZE sz;
    
    SelectObject(hdc, hf);
    GetTextExtentPoint32(hdc, s, lstrlen(s), &sz); 
    return sz.cx;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

TextObject::TextObject(void) :
m_TruncatedTextLineOffset( 0 ),
bRelease( false ),
allowNewLine( true ),
dwFlag( TO_DEFAULT ),
indentNewLine( false ),
m_LineSpacing( 0 ),
m_MaxWidth( g_Global.GetScreenW()),
m_BlackBackground( true ),
m_MaxLines( INFINITE ),
m_StartLineOffset( 0 ),
m_Outline(false),
m_CenterJustify(false)
{
}

TextObject::~TextObject(void) {
    // Destructor
    TextSprite.Release();
}

bool isempty(const char *s) {
    int l = strlen(s);
    for (int i = 0; i < l; i++)
        if (s[i] != ' ')
            return false;
        return true;
}

void TextObject::SetFlag(DWORD dwF) {
    dwFlag = dwF;
}

void TextObject::Format( int maxX, int maxY ){
    bRelease = false;
    dwUse = timeGetTime();
    
    m_MaxWidth = maxX;
    
    if( maxY != -1 ){
        m_LineSpacing = maxY;
    }
    
    if( !m_Text.empty() && !isempty(m_Text.c_str()) ){
        Redo();
    }
}

void TextObject::SetText(LPCTSTR s, DWORD color, int maxWidth, int lineSpacing, bool blackBackground, bool iallowNewLine, bool indentNewLine ) {
    // Variable
    // If the provided string is not empty, and the string or color has changed.
    if (s && s[0] && !isempty(s) && ( strcmp( s, m_Text.c_str() ) != 0 || color != oldColor ) ) {
        
        bRelease = false;
        dwUse = timeGetTime();
        oldColor = color;
        allowNewLine = iallowNewLine;
        
        m_MaxWidth = maxWidth;
        m_LineSpacing = lineSpacing;
        m_BlackBackground = blackBackground;
        this->indentNewLine = indentNewLine;
        
        // Set the String.
        m_Text = s;
        
        // Format the String for Multiple Line
        Redo();
    } else if( s ){
        m_Text = s;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void TextObject::ResetText
//////////////////////////////////////////////////////////////////////////////////////////
//  Only changes the text without any onther settings changes
// 
(
 LPCTSTR text // The text.
 )
 //////////////////////////////////////////////////////////////////////////////////////////
{
    SetText( text, oldColor, m_MaxWidth, m_LineSpacing, m_BlackBackground );
}

void TextObject::Release(void) {
    //   ShowSystemMessage("Delete Text Object", RGB(255,255,255), 0);
    bRelease = true;
    TextSprite.Release();
}

//////////////////////////////////////////////////////////////////////////////////////////
void TextObject::TextColor
//////////////////////////////////////////////////////////////////////////////////////////
(
 HDC hdc,
 const char *s,
 int xPos,
 int yPos,
 DWORD &CurrentColor,
 DWORD &NewColor,
 bool skipColorCoding,
 bool transparentBackground
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    char Text[1000];
    char *ptr;
    int j;
    bool bDraw = false;   
    
    SelectObject(hdc, foFont->hf);
    
    if( transparentBackground ){
        SetBkMode(hdc, TRANSPARENT);
    }else{
        SetBkMode(hdc, OPAQUE);
    
        if (!m_BlackBackground) {
            SetBkColor(hdc, RGB(127, 127, 0));
        } else {
            SetBkColor(hdc, RGB(0, 0, 0));
        }
    }
    SetTextColor(hdc, CurrentColor);
    
    ptr = Text;
    strcpy(Text, s);
    DWORD colorSwitch = 0;
    
    for (j = 0; j < strlen(s); j++) {
        switch (Text[j]) {
        case '{':
            if (!(dwFlag & TO_DRAW_QUOTES)) {
                Text[j] = 0;
                if( !skipColorCoding ){
                    NewColor = RGB(150, 150, 150);
                }
                bDraw = true;
            }
            break;
        case '}':
            if (!(dwFlag & TO_DRAW_QUOTES)) {
                Text[j] = 0;
                if( !skipColorCoding ){
                    NewColor = oldColor;
                }
                bDraw = true;
            }
            break;
        case '\"':
            if (!(dwFlag & TO_DRAW_QUOTES)) {
                colorSwitch++;
                Text[j] = 0;
                if( !skipColorCoding ){
                    if (colorSwitch & 1){
                        NewColor = RGB(255,255,255);
                    }else{
                        NewColor = oldColor;
                    }
                }
                bDraw = true;
            }
            break;
        } 
        if (bDraw || j == (strlen(s)-1)) {
            bDraw = false;
            char szTemp[1000];
            sprintf(szTemp, "|%s|", ptr);
            SetTextColor(hdc, CurrentColor);
            ::TextOut(hdc, xPos, yPos, ptr, strlen(ptr));
            
            if( !skipColorCoding ){
                CurrentColor = NewColor;
            }
            
            xPos += foFont->GetWidth(hdc, ptr);
            ptr = &Text[j+1];
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
const char *GetMoreLinesToken(){    
    // Add "More" character.
    static char tmp[4];
    _snprintf( tmp, 4, "\"%c\"", 254 );
    return tmp;
    
}

//////////////////////////////////////////////////////////////////////////////////////////
void TextObject::Redo(void) {
    // If the provided string is not empty, and the string or color has changed.
    HDC hdc;
    int iW = 0, iH = 0;
    int i;
    
    //      ShowSystemMessage("Redo Text Object", RGB(255,255,255), 0);
    bRelease = false;

    static const char *MoreLinesToken = GetMoreLinesToken();
    
    // Format the String for Multiple Line
    if (m_MaxWidth != 0) {
        FormatText ft;
        if( !allowNewLine ){
            ft.SetSystem( FALSE );
        }
        ft.SetText(m_Text.c_str());
        ft.Format(m_MaxWidth, foFont, indentNewLine);
        
        // Create the DD Surface
        DXDGetDC(&hdc, 16);
        iH = (int)foFont->GetHeight(hdc);
        iW = (int)foFont->GetWidth(hdc, ft.GetText(0));

        iNbLine = ft.GetnbLine();
        
        // If the start offset is beyond the text's farthest line.
        int startOffset = m_StartLineOffset;
        if( startOffset >= iNbLine ){
            DXDReleaseDC(hdc, 16);
            
            Release();
            m_Text = "";
            return;
        }         
        
        bool moreLines = false;
        if( iNbLine - startOffset > m_MaxLines ){
            iNbLine = startOffset + m_MaxLines;
            moreLines = true;
        }
        
        for (i = startOffset; i < iNbLine; i++)  {
            int iT = (int)foFont->GetWidth(hdc, ft.GetText(i));
            if (iW < iT)
                iW = iT;
        }
        
        m_TruncatedTextLineOffset = iNbLine;
        
        int spriteHeight = (iH + m_LineSpacing) * (iNbLine - startOffset);
        
        const int BorderOutlineWidth = 2;
        if( m_Outline ){
            iW += BorderOutlineWidth;
            spriteHeight += BorderOutlineWidth;
            if( moreLines ){
                iW += foFont->GetWidth( hdc, MoreLinesToken );
            }
        }        
        DXDReleaseDC(hdc, 16);
        TextSprite.Release(); 
        
        if(iW < 0)
           iW = m_MaxWidth+50;

        TextSprite.Create(iW, spriteHeight, !m_BlackBackground, !m_BlackBackground ? ((lpDXDirectDraw->wGMask >> 1) & lpDXDirectDraw->wGMask) + ((lpDXDirectDraw->wRMask >> 1) & lpDXDirectDraw->wRMask) : 0);
        
        // Draw the Text
        TextSprite.GetSpriteDC(&hdc);
        
        DWORD CurrentColor = oldColor;
        DWORD NewColor = oldColor;

        if( m_Outline ){
            DWORD Black = RGB(15,15,15);

            for (i = startOffset; i < iNbLine; i++) {
                int xPos = 0;
                if( m_CenterJustify ){
                    xPos = (iW / 2) - (foFont->GetWidth( hdc, ft.GetText(i) ) / 2);
                }
                
                string text( ft.GetText(i) );
                if( moreLines && i + 1 >= iNbLine ){
                    text += MoreLinesToken;
                }
                
                int yPos = (i - startOffset) * (iH + m_LineSpacing);
                TextColor(hdc, text.c_str(), xPos + 0, yPos, Black, Black, true, false);
                TextColor(hdc, text.c_str(), xPos + 2, yPos + 2, Black, Black, true, true);
                TextColor(hdc, text.c_str(), xPos + 1, yPos + 1, CurrentColor, NewColor, false, true);
            }             
        }else{
            for (i = startOffset; i < iNbLine; i++) {
                TextColor(hdc, ft.GetText(i), 0, i*(iH + m_LineSpacing), CurrentColor, NewColor, false, false);
            }
        }
        TextSprite.ReleaseSpriteDC(hdc);
    } else {
        DXDGetDC(&hdc, 17);
        iH = (int)foFont->GetHeight(hdc);
        iW = (int)foFont->GetWidth(hdc, m_Text.c_str());
        DXDReleaseDC(hdc, 17);
        TextSprite.Release();
        TextSprite.Create(iW, iH, !m_BlackBackground, !m_BlackBackground ? ((lpDXDirectDraw->wGMask >> 1) & lpDXDirectDraw->wGMask) + ((lpDXDirectDraw->wRMask >> 1) & lpDXDirectDraw->wRMask) : 0);
        TextSprite.GetSpriteDC(&hdc);
        DWORD CurrentColor = oldColor;
        DWORD NewColor = oldColor;
        TextColor(hdc, m_Text.c_str(), 0, 0, CurrentColor, NewColor, false,false);
        TextSprite.ReleaseSpriteDC(hdc);
    }
}

void TextObject::Draw(int x, int y, V2SPRITEFX *fx) {
    if( m_Text.empty() ){
        return;
    }

    RECT r = {0, 0, g_Global.GetScreenW(), g_Global.GetScreenH()};
    if (fx) {
        r.left   = fx->lpClipRect->left;
        r.top    = fx->lpClipRect->top;
        r.right  = fx->lpClipRect->right;
        r.bottom = fx->lpClipRect->bottom;
    }
    // Out of the Clipping Border.
    if (x > r.right)         return;
    if (y > r.bottom)        return;
    if (x + TextSprite.GetWidth() < r.left) return;
    if (y + TextSprite.GetWidth() < r.top) return;
    
    if (bRelease){
        Redo();
    }
    
    dwUse = timeGetTime();
    TextSprite.DrawSprite(x, y, &r);    
}

void TextObject::Test(void) {
    if (!bRelease) {
        if (timeGetTime() - dwUse > 10000)
            Release();
    }
}

int TextObject::GetFontSize(void) {
    HDC hdc;
    DXDGetDC(&hdc, 18);
    int R = (int)foFont->GetHeight(hdc);
    DXDReleaseDC(hdc, 18);
    return R;
}

int TextObject::GetHeight(void) {
    if (bRelease)
        Redo();
    
    return TextSprite.GetHeight();
}

int TextObject::GetWidth(LPCTSTR s) {
    if( s != NULL ){
        HDC hdc;
        DXDGetDC(&hdc, 19);
        int R = (int)foFont->GetWidth( hdc, const_cast< char * >( s ) );
        DXDReleaseDC(hdc, 19);
        return R;
    }
    
    if (bRelease)
        Redo();
    
    return TextSprite.GetWidth();
}

void TextObject::SetMaxLines( DWORD maxLines, DWORD startLineOffset )
{
    m_MaxLines = maxLines;
    m_StartLineOffset = startLineOffset;
}

int TextObject::GetTruncatedTextLineOffset()
{
    return m_TruncatedTextLineOffset;
}

void TextObject::EnableOutline()
{
    m_Outline = true;
}

void TextObject::EnableCenterJustification()
{
    m_CenterJustify = true;
}