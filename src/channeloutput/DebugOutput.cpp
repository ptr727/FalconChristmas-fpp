/*
 * This file is part of the Falcon Player (FPP) and is Copyright (C)
 * 2013-2022 by the Falcon Player Developers.
 *
 * The Falcon Player (FPP) is free software, and is covered under
 * multiple Open Source licenses.  Please see the included 'LICENSES'
 * file for descriptions of what files are covered by each license.
 *
 * This source file is covered under the GPL v2 as described in the
 * included LICENSE.GPL file.
 */

#include "fpp-pch.h"

#include "DebugOutput.h"

extern "C" {
DebugOutput* createOutputDebug(unsigned int startChannel,
                               unsigned int channelCount) {
    return new DebugOutput(startChannel, channelCount);
}
}

/*
 *
 */
DebugOutput::DebugOutput(unsigned int startChannel, unsigned int channelCount) :
    ChannelOutputBase(startChannel, channelCount) {
    LogDebug(VB_CHANNELOUT, "DebugOutput::DebugOutput(%u, %u)\n",
             startChannel, channelCount);
}

/*
 *
 */
DebugOutput::~DebugOutput() {
    LogDebug(VB_CHANNELOUT, "DebugOutput::~DebugOutput()\n");
}

/*
 *
 */
int DebugOutput::Init(Json::Value v) {
    LogDebug(VB_CHANNELOUT, "DebugOutput::Init()\n");

    // Call the base class' Init() method, do not remove this line.
    return ChannelOutputBase::Init(v);
}

/*
 *
 */
int DebugOutput::Close(void) {
    LogDebug(VB_CHANNELOUT, "DebugOutput::Close()\n");

    return ChannelOutputBase::Close();
}

/*
 *
 */
int DebugOutput::SendData(unsigned char* channelData) {
    LogExcess(VB_CHANNELOUT, "DebugOutput::RawSendData(%p)\n", channelData);

    HexDump("DebugOutput::RawSendData", channelData, m_channelCount, VB_CHANNELOUT);

    return m_channelCount;
}

/*
 *
 */
void DebugOutput::DumpConfig(void) {
    LogDebug(VB_CHANNELOUT, "DebugOutput::DumpConfig()\n");

    // Call the base class' DumpConfig() method, do not remove this line.
    ChannelOutputBase::DumpConfig();
}
