//
//  SDLoadingMode.cpp
//  Ship Demo
//
//  This module provides a very barebones loading screen.  Most of the time you
//  will not need a loading screen, because the assets will load so fast.  But
//  just in case, this is a simple example you can use in your games.
//
//  We know from 3152 that you all like to customize this screen.  Therefore,
//  we have kept it as simple as possible so that it is easy to modify.
//
//  Author: Walker White
//  Version: 1/10/17
//
#include "SDLoadingMode.h"

using namespace cugl;

#pragma mark -
#pragma mark Loading Assets

/** This is adjusted by screen aspect ratio to get the height */
#define GAME_WIDTH 1024

/** The reference to the claw texture */
#define CLAW_TEXTURE    "textures/Claw.png"
#define CLAW_KEY        "claw"
#define CLAW_SCALE      0.4f
#define CLAW_VOFF       0.6f

/** The reference to the play button (which is "inside" the claw)*/
#define PLAY_TEXTURE    "textures/play.png"
#define PLAY_KEY        "play"
#define PLAY_SCALE      1.875f
#define PLAY_HOFF       0.535f
#define PLAY_VOFF       0.3f

/** The reference to the progress bar textire */
#define PROGRESS_TEXTURE "textures/progressbar.png"
#define PROGRESS_KEY     "progress"
#define PROGRESS_VOFF   0.15f

float BACK_RECT[] = {0.0f,  1.0f,  0.0625f, 0.4375f};
float FORE_RECT[] = {0.04f, 0.96f, 0.5625f, 0.9375f};
float LEFT_RECT[] = {0.00f, 0.04f, 0.5625f, 0.9375f};
float RGHT_RECT[] = {0.96f, 1.00f, 0.5625f, 0.9375f};


#pragma mark -
#pragma mark Constructors

/**
 * Initializes the controller contents, making it ready for loading
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool LoadingMode::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    // IMMEDIATELY load three assets
    _assets = assets;
    assets->load<Texture>(CLAW_KEY,CLAW_TEXTURE);
    assets->load<Texture>(PLAY_KEY,PLAY_TEXTURE);
    assets->load<Texture>(PROGRESS_KEY,PROGRESS_TEXTURE);
    
    Size size = Application::get()->getDisplaySize();
    size *= GAME_WIDTH/size.width;
    _scene = Scene::alloc(size);
    
    // The "logo".  Make the edges smooth
    auto claw = PolygonNode::allocWithTexture(assets->get<Texture>(CLAW_KEY));
    claw->setAnchor(Vec2::ANCHOR_MIDDLE);
    claw->setPosition(Vec2(size.width/2.0f,size.height*CLAW_VOFF));
    claw->setScale(CLAW_SCALE);

    // The play button
    auto play = PolygonNode::allocWithTexture(assets->get<Texture>(PLAY_KEY));
    _button = Button::alloc(play);
    _button->setName(PLAY_KEY);
    _button->setAnchor(Vec2::ANCHOR_MIDDLE);
    _button->setPosition(claw->getContentSize()*Vec2(PLAY_HOFF,PLAY_VOFF));
    _button->setScale(PLAY_SCALE);
    _button->setListener([=](const std::string& name, bool down) {
        this->_completed = !down;
    });
    _button->setVisible(false);
    
    // The progress bar
    auto texture = assets->get<Texture>(PROGRESS_KEY);
    auto background = texture->getSubTexture(BACK_RECT[0], BACK_RECT[1], BACK_RECT[2], BACK_RECT[3]);
    auto foreground = texture->getSubTexture(FORE_RECT[0], FORE_RECT[1], FORE_RECT[2], FORE_RECT[3]);
    auto begincap   = texture->getSubTexture(LEFT_RECT[0], LEFT_RECT[1], LEFT_RECT[2], LEFT_RECT[3]);
    auto finalcap   = texture->getSubTexture(RGHT_RECT[0], RGHT_RECT[1], RGHT_RECT[2], RGHT_RECT[3]);

    _bar = ProgressBar::allocWithCaps(background,foreground,begincap,finalcap);
    _bar->setAnchor(Vec2::ANCHOR_MIDDLE);
    _bar->setPosition(Vec2(size.width/2.0f,size.height*PROGRESS_VOFF));

    _scene->addChild(claw);
    claw->addChild(_button);
    _scene->addChild(_bar);
    
    Application::get()->setClearColor(Color4(192,192,192,255));
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void LoadingMode::dispose() {
    // Deactivate the button (platform dependent)
    if (isPending()) {
        _button->deactivate();
    }
    _button = nullptr;
    _bar = nullptr;
    _scene = nullptr;
    _assets = nullptr;
    _progress = 0.0f;
    _completed = false;
}


#pragma mark -
#pragma mark Progress Monitoring
/**
 * The method called to update the game mode.
 *
 * This method updates the progress bar amount.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void LoadingMode::update(float progress) {
    if (_progress < 1) {
        _progress = _assets->progress();
        if (_progress >= 1) {
            _progress = 1.0f;
            _button->setVisible(true);
            _button->activate(1);
        }
        _bar->setProgress(_progress);
    }
}

/**
 * The method called to draw the application to the screen.
 *
 * This method should contain OpenGL and related drawing calls.
 */
void LoadingMode::draw(const std::shared_ptr<cugl::SpriteBatch>& batch) {
    _scene->render(batch);
}

/**
 * Returns true if loading is complete, but the player has not pressed play
 *
 * @return true if loading is complete, but the player has not pressed play
 */
bool LoadingMode::isPending( ) const {
    return _button != nullptr && _button->isVisible();
}

