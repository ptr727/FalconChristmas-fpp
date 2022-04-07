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

#include <string>

#include "Matrix.h"
#include "PanelMatrix.h"

#include "led-matrix.h"

using rgb_matrix::Canvas;
using rgb_matrix::FrameCanvas;
using rgb_matrix::RGBMatrix;

#include "ChannelOutputBase.h"

class RGBMatrixOutput : public ChannelOutputBase {
public:
    RGBMatrixOutput(unsigned int startChannel, unsigned int channelCount);
    virtual ~RGBMatrixOutput();

    virtual int Init(Json::Value config) override;
    virtual int Close(void) override;

    virtual void PrepData(unsigned char* channelData) override;

    virtual int SendData(unsigned char* channelData) override;

    virtual void DumpConfig(void) override;

    virtual void GetRequiredChannelRanges(const std::function<void(int, int)>& addRange) override;

private:
    FrameCanvas* m_canvas;
    RGBMatrix* m_rgbmatrix;
    std::string m_layout;
    std::string m_colorOrder;

    int m_panelWidth;
    int m_panelHeight;
    int m_panels;
    int m_width;
    int m_height;
    int m_rows;
    int m_outputs;
    int m_longestChain;
    int m_invertedData;

    Matrix* m_matrix;
    PanelMatrix* m_panelMatrix;

    uint8_t m_gammaCurve[256];
};
