
#include <android_native_app_glue.h>
#include <jni.h>

#include "core/Application.h"
#include "core/Engine.h"
#include "core/AndroidInputHandler.h"

#include "core/Logger.h"

#include "core/AndroidDataProvider.h"
#include "core/ResourceSystem.h"
#include "core/TextureManager.h"
#include "core/ShaderManager.h"
#include "core/ShadingProgramManager.h"
#include "core/ImageSpriteManager.h"
#include "core/SpritedFontManager.h"

#include "core/ShadingProgram.h"
#include "core/SpritedFont.h"


#include "core/XMLScriptParser.h"
#include "core/_2d/MaterialManager.h"



extern "C" {

void android_main(struct android_app *pApp)
{
    core::Logger logger;
    logger.write(std::string("C++ version: ") + std::to_string( __cplusplus));


    core::Engine *engine = new core::Engine(pApp);
    core::InputManager *inputManager = new core::InputManager();
    engine->setInputManager(inputManager);

    core::Application app(pApp, engine, inputManager );

    /*


    core::DataProviderPtr dataProvider(new core::AndroidDataProvider(pApp->activity->assetManager));

    core::ScriptLoader loader;
    {
        core::ScriptParserPtr parser(new core::XmlScriptParser());
        loader.registerParser(parser);
    }


    core::TextureManager texManager;
    core::ShaderManager shaderManager;
    core::ShadingProgramManager shadingProgManager;
    core::ImageSpriteManager spriteManager;
    core::SpritedFontManager fontManager;
    core::ResourceSystem resSystem;

    core::_2d::MaterialManager manager;

    resSystem.registerDataProvider(dataProvider);

    resSystem.registerResourceManager("texture", core::TextureManager::getSingletonPtr());
    resSystem.registerResourceManager("shader", core::ShaderManager::getSingletonPtr());
    resSystem.registerResourceManager("shading_program", core::ShadingProgramManager::getSingletonPtr());
    resSystem.registerResourceManager("sprite", core::ImageSpriteManager::getSingletonPtr());
    resSystem.registerResourceManager("font", core::SpritedFontManager::getSingletonPtr());

    resSystem.parseConfiguration("GameResourceCfg.xml");


*/

    app.activate();

    app.run();

    app.deactivate();


    //delete logger;
}

}
