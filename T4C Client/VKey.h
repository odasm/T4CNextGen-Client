// VKey.h: interface for the VKey class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VKEY_H__79380E38_9C3D_11D3_85BE_00E02922FA40__INCLUDED_)
#define AFX_VKEY_H__79380E38_9C3D_11D3_85BE_00E02922FA40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

namespace vir{

// This class stores keyboard key combinations.
class VKey{
public:
    VKey( void );
    VKey( const VKey &vKey );
    VKey( unsigned short virKey, bool ctrl = false, bool shift = false, bool alt = false );
    
    // Returns the VKey class as a single 'long' variable (for storage).
    long AsLong( void ) const;

    // Sets the value from a 'long' value returned by AsLong().
    // (for storage retreival).
    void SetLong( long val );

    // Returns the raw VK_ (without keyboard states).
    unsigned short GetKey( void ) const;

    // Sets the key, also look for CTRL, ALT and SHIFT keyboard states.
    void SetKey( unsigned short virKey, bool ctrl = false, bool shift = false, bool alt = false );
    // Sets the key but doesn't look for other keyboard states.
    void SetRawKey( unsigned short virKey );

    bool operator == ( const VKey &vKey ) const;

    std::string GetName() const;

    bool LCTRL() const;
    bool RCTRL() const;
    bool LSHIFT() const;
    bool RSHIFT() const;
    bool LMENU() const;
    bool RMENU() const;
    bool CTRL( void ) const;
    bool ALT( void ) const;
    bool SHIFT( void ) const;
    bool MENU( void ) const;

    void SetLCTRL( bool state = true );
    void SetRCTRL( bool state = true );
    void SetLSHIFT( bool state = true );
    void SetRSHIFT( bool state = true );
    void SetLMENU( bool state = true );
    void SetRMENU( bool state = true );
    void SetALT( bool state = true );

private:
    long key;
};

};

#endif // !defined(AFX_VKEY_H__79380E38_9C3D_11D3_85BE_00E02922FA40__INCLUDED_)
