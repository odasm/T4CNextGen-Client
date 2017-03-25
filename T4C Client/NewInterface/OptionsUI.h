// OptionsUI.h
//

#if !defined(OptionsUI_h)
#define OptionsUI_h

#include "BoxUI.h"
#include "ListUI.h"
#include "SliderUI.h"
#include "GraphUI.h"
#include "ButtonUI.h"
#include "StaticTextUI.h"
#include "EditUI.h"

class OptionEvents;

class OptionsUI : public BoxUI
{
public:
    ~OptionsUI();

    
    static OptionsUI *GetInstance();
    static unsigned int Display( void *null );
    void Hide();
    void Draw( V2SPRITEFX *vsfFX );
    int  TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise);
    void ShowQuitWindows();
    void HideQuitWindows();

private:
    OptionsUI( void );

    void LoadOptions();
    void UpdateGraphList();
    void UpdateLogList();
    void AddGraphparameters(char *pstrName,int dwID, int dwStatus);
    void AddLogParameters(char *pstrName,int dwID, int dwStatus);

    class MusicVolumeEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } musicVolumeEvent;
    class FxVolumeEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } fxVolumeEvent;
    class BrightnessEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } brightnessEvent;
    class CdMusicEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } cdMusicEvent;

    class QuitEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } quitButtonEvent;

    class GraphListEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } graphListEvent;
    friend GraphListEvent;

    class LogListEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } logListEvent;

    class LowCpuButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } lowCpuButtonEvent;

    friend LowCpuButtonEvent;
    friend LogListEvent;
    friend MusicVolumeEvent;
    friend FxVolumeEvent;
    friend BrightnessEvent;
    friend CdMusicEvent;

    class FileNamePopup : public BoxUI{
    public:
        class OkEvent : public EventVisitor{
        public:
            void LeftClicked();
        } okEvent;

        FileNamePopup();
        void Draw( V2SPRITEFX *vsfFX );
        int  TxtOff(Font *, HDC, char*,int) ;

        void SetFileName( std::string ifileName )
        {
            fileName.SetText( ifileName );
        }        
    private:
        EditUI   fileName;
        ButtonUI cancelButton;
        ButtonUI okButton;
        GraphUI  intlText;
        GraphUI  graphPatch;
        GraphUI  NameBack;

        friend OkEvent;
    } fileNamePopup;

    class QuitPopup : public BoxUI{
    public:
        QuitPopup();

    private:
        class QuitEvent : public EventVisitor{
        public:
            void LeftClicked();
        } quitEvent;

        void Draw( V2SPRITEFX *vsfFX );
        int  TxtOff(Font *, HDC, char*,int) ;

        ButtonUI cancelButton;
        ButtonUI okButton;
        GraphUI  intlText;
        GraphUI  graphPatch;
        GraphUI  quitBack;
        StaticTextUI textArea;
    } quitPopup;

    friend QuitPopup;
    friend QuitEvent;

    class FileNameEvent : public EventVisitor{
    public:
        void LeftClicked();
    } fileNameEvent;

    friend FileNamePopup;
    friend FileNameEvent;    
    friend FileNamePopup::OkEvent;

    class OptDlgUI : public StaticTextUI{
    public:
        std::string optionID;
        bool        bState;
        int         dwID;
    };
    class OptDlgToggleUI : public ButtonUI{
    public:
        OptDlgToggleUI() : ButtonUI( EmptyEvent::GetInstance() ){};

        std::string optionID;
        bool        bState;
    };

    SliderUI musicVolume;
    SliderUI fxVolume;
    SliderUI brightness;

    ListUI   graphList;
    GraphUI  graphListingSelectionGraphPatch;
    GraphUI  graphListScrollUp;
    GraphUI  graphListScrollDown;

    ListUI   logList;
    GraphUI  logListingSelectionGraphPatch;
    GraphUI  logListScrollUp;
    GraphUI  logListScrollDown;
    
    ButtonUI cdMusic;
    ButtonUI disabledCdMusic;
    
    ButtonUI quitButton;
    ButtonUI cancelButton;
    ButtonUI lowCPUButton;
    ButtonUI logFileNameButton;

    ButtonUI X_Button;
    StaticTextUI logFileName;

    StaticTextUI cdMusicText;

    GraphUI backOption;
};

#endif /* OptionsUI_h */
