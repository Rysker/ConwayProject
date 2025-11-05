#ifndef APP_STATE_H
#define APP_STATE_H

// Available tools
enum class TOOL {
    BRUSH,
    ERASER
};

// Struct for application status
struct AppState
{
    bool isPaused = true;
    // Time for next generation to be displayed in seconds.
    float generationTimeSec = 1.0f;
    TOOL currentTool = TOOL::BRUSH;
    int brushSize = 1;
};

#endif
