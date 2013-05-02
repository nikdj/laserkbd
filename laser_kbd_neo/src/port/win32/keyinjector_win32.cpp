/*
 *  Keyboard event injector for Win32
 *
 *  By Shikai Chen (csk@live.com)
 *  
 *  Copyright 2009 - 2013 RoboPeak Team
 *  http://www.robopeak.net
 */

#include "common.h"
#include "port/common/keyinjector.h"


class OSKeyInjector_Win32 : public OSKeyInjector
{
public :
    OSKeyInjector_Win32() {


    }

    virtual ~OSKeyInjector_Win32() {

    }


    virtual bool injectKeyEvents( const std::vector<KeyEventDesc> & intputlist)
    {
        if (!intputlist.size()) return false;


        INPUT * inputs = new INPUT[intputlist.size()];
        
        do
        {
            for (int pos=0; pos<intputlist.size(); ++pos)
            {
                inputs[pos].type = INPUT_KEYBOARD;
                inputs[pos].ki.wVk = intputlist[pos].keyval;
                
                if (intputlist[pos].type == KEY_EVENT_PRESSED) {
                
                    inputs[pos].ki.dwFlags =  0;
                } else {
                    inputs[pos].ki.dwFlags =  KEYEVENTF_KEYUP;
                }
                
                inputs[pos].ki.time = 0;
            }

            SendInput(intputlist.size(), inputs, sizeof(INPUT));

        }while(0);


        delete [] inputs;

        return true;
    }

};




//---

OSKeyInjector* OSKeyInjector::g_inst = NULL;

OSKeyInjector * OSKeyInjector::GetInstance() {
    // FIXME: race condition will occur when this function is used in 
    //        multi-thread envirnoment

    if (OSKeyInjector::g_inst) {
        return OSKeyInjector::g_inst;
    }

    OSKeyInjector::g_inst = new OSKeyInjector_Win32();
    return OSKeyInjector::g_inst;
}


void OSKeyInjector::ReleaseInstance()
{
    // FIXME: race condition will occur when this function is used in 
    //        multi-thread envirnoment

    if (OSKeyInjector::g_inst) {
        delete OSKeyInjector::g_inst;
        OSKeyInjector::g_inst = NULL;
    }
}