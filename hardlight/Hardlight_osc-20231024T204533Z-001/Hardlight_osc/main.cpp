#include <iostream>
#include <vector>
#include <string>
#include "include/HLVR.h"
#include <cassert>
#include <chrono>
#include <thread>

using namespace std;

HLVR_System* system_vest = nullptr;
HLVR_Event* event = nullptr;
HLVR_Effect* effect = nullptr;
HLVR_Timeline* timeline = nullptr;


void init_click(void){
    std::vector<uint32_t> regions = {hlvr_region_chest_left, hlvr_region_chest_right};

    HLVR_Event_Create(&event, HLVR_EventType_DiscreteHaptic);

    //set waveform property. Choice of SetInt matches the _Int suffix on the HLVR_EventKey
    HLVR_Event_SetInt(event, HLVR_EventKey_DiscreteHaptic_Waveform_Int, HLVR_Waveform_Hum);

    //set target property, which comes in two flavors: regions or nodes. 
    //here we target regions because we don't care about any particular device
    HLVR_Event_SetUInt32s(event, HLVR_EventKey_Target_Regions_UInt32s, regions.data(), regions.size());
    
    HLVR_Timeline_Create(&timeline);

    //add foo event at 0 seconds
    HLVR_Timeline_AddEvent(timeline, 0.0 /*seconds*/, event);
    
    HLVR_Effect_Create(&effect);

    //transmit the timeline, creating an effect in the system
    HLVR_Timeline_Transmit(timeline, system_vest, effect);
}

void init_vest(void){
    //make sure supplied headers are compatible with our lib
    assert(HLVR_Version_IsCompatibleDLL()); 

    HLVR_System_Create(&system_vest);
    this_thread::sleep_for(std::chrono::seconds(1));
    init_click();
}

void transmit_click(void){
    HLVR_Effect_Play(effect);
    HLVR_EffectInfo info;
    HLVR_Effect_GetInfo(effect,&info);
    while(info.PlaybackState == HLVR_EffectInfo_State_Playing){
        HLVR_Effect_GetInfo(effect,&info);
        
    }
    HLVR_Effect_Reset(effect);
}

void destroy_everything(void){
    HLVR_Timeline_Destroy(timeline);
 
    HLVR_Event_Destroy(event);

    HLVR_System_Destroy(system_vest);
}

int main()
{
    HLVR_Result sc;

    init_vest();
    transmit_click();
    this_thread::sleep_for(std::chrono::milliseconds(1000));
    transmit_click();
    this_thread::sleep_for(std::chrono::milliseconds(1000));
    transmit_click();
    this_thread::sleep_for(std::chrono::milliseconds(1000));
    transmit_click();
    this_thread::sleep_for(std::chrono::milliseconds(1000));
   
    destroy_everything();
    vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};

    for (const string& word : msg)
    {
        cout << word << " ";
    }
    cout << endl;
}