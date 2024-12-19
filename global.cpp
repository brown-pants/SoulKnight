#include "global.h"
QSize Global::RenderSize(600, 400);
QSize Global::GameWindowSize(600, 400);
Global::Key Global::KeyPressed;
Global::Mouse Global::MouseState;
double Global::elapsed = 0.0;
QPointF Global::CameraPos(0.0, 0.0);
bool Global::Touched(false);
