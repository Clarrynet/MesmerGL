//
//  SDLoadingMode.h
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
#ifndef __SD_LOADING_MODE_H__
#define __SD_LOADING_MODE_H__
#include <cugl/cugl.h>

#pragma mark -
#pragma mark GameController
/**
 * This class is a simple loading screen for asychronous asset loading.
 *
 * The screen will display a very minimal progress bar that displays the
 * status of the asset manager.  Make sure that all asychronous load requests
 * are issued BEFORE calling update for the first time, or else this screen
 * will think that asset loading is complete. 
 *
 * Once asset loading is completed, it will display a play button.  Clicking
 * this button will inform the application root to switch to the gameplay mode.
 */
class LoadingMode {
protected:
    /** The asset manager for loading. */
    std::shared_ptr<cugl::AssetManager> _assets;

    // NO CONTROLLER (ALL IN SEPARATE THREAD)
    
    // VIEW
    /** The root of our scene graph. */
    std::shared_ptr<cugl::Scene>  _scene;
    /** The animated progress bar */
    std::shared_ptr<cugl::ProgressBar>  _bar;
    /** The "play" button */
    std::shared_ptr<cugl::Button> _button;

    // MODEL
    /** The progress displayed on the screen */
    float _progress;
    /** Whether or not the player has pressed play to continue */
    bool  _completed;

    
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new loading mode with the default values.
     *
     * This constructor does not allocate any objects or start the game.
     * This allows us to use the object without a heap pointer.
     */
    LoadingMode() : _progress(0.0f), _completed(false) {}
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~LoadingMode() { dispose(); }
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    void dispose();
    
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
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);

    
#pragma mark -
#pragma mark Progress Monitoring
    /**
     * The method called to update the game mode.
     *
     * This method updates the progress bar amount.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    void update(float timestep);
    
    /**
     * The method called to draw the application to the screen.
     *
     * This method should contain OpenGL and related drawing calls.
     */
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);
    
    /**
     * Returns true if loading is complete and the player pressed play
     *
     * @return true if loading is complete and the player pressed play
     */
    bool isComplete( ) const { return _completed; }

    /**
     * Returns true if loading is complete, but the player has not pressed play
     *
     * @return true if loading is complete, but the player has not pressed play
     */
    bool isPending( ) const;
};

#endif /* __SD_LOADING_MODE_H__ */
