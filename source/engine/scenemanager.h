#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

typedef enum {
    SCENE_EVENT_INIT = 0,
    SCENE_EVENT_UPDATE,
    SCENE_EVENT_DRAW,
    SCENE_EVENT_EXIT
} SceneEvent;

// Estructura base de escena (funciones de evento)
typedef struct {
    void (*sceneInit)();
    void (*sceneUpdate)();
    void (*sceneDraw)();
    void (*sceneExit)();
} Scene;

void sceneChange(const Scene* newScene);
void sceneEvent(SceneEvent event);
const Scene* getCurrentScene(); // debug opcional

#endif