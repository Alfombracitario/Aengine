#include "engine/scenemanager.h"

// escena actual
extern const Scene* currentScene;

void sceneChange(const Scene* newScene) {
    // ejecutar exit de la escena actual (si existe)
    if (currentScene && currentScene->sceneExit) {
        currentScene->sceneExit();
    }

    // cambiar escena
    currentScene = newScene;

    // ejecutar init de la nueva escena
    if (currentScene && currentScene->sceneInit) {
        currentScene->sceneInit();
    }
}

void sceneEvent(SceneEvent event) {
    if (!currentScene) return;

    switch (event) {
        case SCENE_EVENT_INIT:
            if (currentScene->sceneInit) currentScene->sceneInit();
            break;
        case SCENE_EVENT_UPDATE:
            if (currentScene->sceneUpdate) currentScene->sceneUpdate();
            break;
        case SCENE_EVENT_DRAW:
            if (currentScene->sceneDraw) currentScene->sceneDraw();
            break;
        case SCENE_EVENT_EXIT:
            if (currentScene->sceneExit) currentScene->sceneExit();
            break;
    }
}

const Scene* getCurrentScene() {
    return currentScene;
}
