#pragma once
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

#include <vector>

#include "overlays/PixelOverlayModel.h"
#include "PixelString.h"
#include "ThreadedChannelOutputBase.h"

class ModelPixelStringsOutput : public ThreadedChannelOutputBase {
public:
    ModelPixelStringsOutput(unsigned int startChannel, unsigned int channelCount);
    ~ModelPixelStringsOutput();

    virtual int Init(Json::Value config) override;
    virtual int Close(void) override;

    virtual void PrepData(unsigned char* channelData) override;
    virtual int RawSendData(unsigned char* channelData) override;

    virtual void DumpConfig(void) override;

    virtual void GetRequiredChannelRanges(const std::function<void(int, int)>& addRange) override;

private:
    std::string modelName;
    PixelOverlayModel *model = nullptr;

    unsigned char* buffer = nullptr;

    std::vector<PixelString*> strings;
};

