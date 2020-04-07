#include <android_native_app_glue.h>
#include <jni.h>

#include "core/Application.h"
#include "Game.h"
#include "core/AndroidInputHandler.h"

#include "core/AndroidDataProvider.h"





extern "C" {

void android_main(struct android_app *pApp)
{
    // create main instance of logger singleton
    core::Logger logger;

    Game *engine = new Game(pApp);
    core::InputManager *inputManager = new core::InputManager();
    engine->setInputManager(inputManager);

    core::Application app(pApp, engine, inputManager );


    app.activate();

    app.run();

    app.deactivate();


    //delete logger;
}

}
