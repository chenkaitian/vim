/*
 * HardwareInfo.h
 *
 *  Created on: 2016年10月18日
 *      Author: ravizhang
 */

#ifndef HARDWARE_INFO_N_H_
#define HARDWARE_INFO_N_H_


#include "servant/Application.h"
#include <servant/Communicator.h>
#include <servant/CommunicatorFactory.h>

#include <unistd.h>



class FZHardwareInfoMan :
public TC_Singleton<FZHardwareInfoMan, CreateUsingNew, DefaultLifetime>
{
public:
    int getCpuNum();
    int getMemSize();
    int getFreeMemSize();    
};


#endif
