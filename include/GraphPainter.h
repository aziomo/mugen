//
// Created by alberto on 12/23/21.
//

#ifndef MUGEN_GRAPHPAINTER_H
#define MUGEN_GRAPHPAINTER_H

#include "MainWindow.h"


class GraphPainter {
private:


    MainWindow* window;

public:
    GraphPainter(MainWindow *window);
    void renderWaveformGraph(WaveformType wavetype, bool drawIntegral = false, bool fullscreen = false);

    double YofX(double x, WaveformType function);
};


#endif //MUGEN_GRAPHPAINTER_H
