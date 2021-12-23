//
// Created by alberto on 12/23/21.
//

#include "GraphPainter.h"

GraphPainter::GraphPainter(MainWindow* window){
    this->window = window;
}

void GraphPainter::renderWaveformGraph(WaveformType wavetype, bool drawIntegral, bool fullscreen){
    // have a rect destined for drawing
    SDL_Rect drawingArea;
    if (fullscreen){
        drawingArea = {window->borderSize*10, window->borderSize*10, window->w - window->borderSize*20, window->h - window->borderSize*20};
    }
    else {
        drawingArea = {window->w*5/9 + window->borderSize, window->borderSize*2, window->w*4/9 - window->borderSize*3, window->h/2 - window->borderSize*2};
    }

    double xIncrement = TWOPI / drawingArea.w;
    double startingX = 0.0;
    double currentX = startingX;
    double currentY = YofX(currentX, wavetype);

    double xAxisAltitude = drawingArea.y + (drawingArea.h * 0.5);

    SDL_Point point, lastPoint;
//    bool drawIntegral = true;

    SDL_SetRenderDrawColor(window->renderer, 0x00, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawLine(window->renderer, drawingArea.x, xAxisAltitude, drawingArea.x + drawingArea.w, xAxisAltitude);

    SDL_SetRenderDrawColor(window->renderer, 0xFF, 0xFF, 0xFF, 0xFF);


    if (drawIntegral)
    {
        double maxSample = 0.0;
        double minSample = abs(currentY);

        for (int i = 0; i < drawingArea.w; i++){
            currentX += xIncrement;
            currentY += YofX(currentX, wavetype);

            if (maxSample < abs(currentY)){
                maxSample = abs(currentY);
            }
            if (minSample > abs(currentY)){
                minSample = abs(currentY);
            }
        }

        currentX = startingX;
        currentY = YofX(currentX, wavetype);

        double normalizedY = (currentY - minSample)/(maxSample - minSample);

        point = {drawingArea.x, static_cast<int>(xAxisAltitude - (drawingArea.h * 0.5) * normalizedY)};
        lastPoint = {drawingArea.x, static_cast<int>(xAxisAltitude - (drawingArea.h * 0.5) * normalizedY)};

        for (int i = 0; i < drawingArea.w; i++){
            currentX += xIncrement;
            currentY += YofX(currentX, wavetype);

            normalizedY = (currentY - minSample)/(maxSample - minSample);
            point.x = drawingArea.x+i;
            point.y = static_cast<int>(xAxisAltitude - (drawingArea.h * 0.5) * normalizedY);

            SDL_RenderDrawLine(window->renderer, lastPoint.x, lastPoint.y, point.x, point.y);
            lastPoint = point;
        }
    }
    else
    {
        point = {drawingArea.x, static_cast<int>(xAxisAltitude - (drawingArea.h * 0.5) * currentY)};
        lastPoint = {drawingArea.x, static_cast<int>(xAxisAltitude - (drawingArea.h * 0.5) * currentY)};

        for (int i = 0; i < drawingArea.w; i++){
            currentX += xIncrement;
            currentY = YofX(currentX, wavetype);

            point.x = drawingArea.x+i;
            point.y = static_cast<int>(xAxisAltitude - (drawingArea.h * 0.5) * currentY);

            SDL_RenderDrawLine(window->renderer, lastPoint.x, lastPoint.y, point.x, point.y);

            lastPoint = point;
        }
    }
}

double GraphPainter::YofX(double x, WaveformType function){

    double value;

    switch (function) {
        case WaveformType::SINE:
            value = sin(x);
            break;
        case WaveformType::SQUARE:
            value = sin(x) > 0 ? 1.0 : -1.0;
            break;
        case WaveformType::TRIANGLE:
            value = asin(sin(x)) * 2.0/PI;
            break;
        case WaveformType::SAWTOOTHDOWN:
            value = 1.0 - 2.0 * (fmod(x, TWOPI) * (1.0 / TWOPI));
            break;
        case WaveformType::SAWTOOTHUP:
            value = (2.0 * (fmod(x, TWOPI) * (1.0 / TWOPI))) - 1.0;
            break;
        case WaveformType::NOISE:
            value = 2.0 * ((double)rand() / (double)(RAND_MAX)) - 1.0;
            break;
    }

    return value;
}