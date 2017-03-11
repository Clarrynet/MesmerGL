//
//  SDGameMode.h
//  Ship Demo
//
//  This is the most important class in this demo.  This class manages the
//  gameplay for this demo.  It is a relativeluy simple class as we are not
//  worried about collisions.
//
//  WARNING: There are a lot of shortcuts in this design that will do not adapt
//  well to data driven design.  This demo has a lot of simplifications to make
//  it a bit easier to see how everything fits together.  However, the model
//  classes and how they are initialized will need to be changed if you add
//  dynamic level loading.
//
//  Author: Walker White
//  Version: 1/10/17
//
#include <cugl/cugl.h>
#include <iostream>
#include <sstream>

#include "SDGameMode.h"
//#include "SDInputController.h"
//#include "SDShipModel.h"

using namespace cugl;
using namespace std;

#pragma mark -
#pragma mark Level Layout

/** This is adjusted by screen aspect ratio to get the height */
#define GAME_WIDTH 1024

/** The number of space tiles to use */
#define TILE_AMOUNT  5
/** The amount to scale the planets */
#define PLANET_SCALE 0.25f
/** The parallax for each layer */
#define PARALLAX_AMT 0.1f

/** The offset of planet 1 from the ship position (in world coordinates) */
float PLANET1_OFF[] = {-325,-200};
/** The offset of planet 2 from the ship position (in world coordinates) */
float PLANET2_OFF[] = { 325, 200};
/** The position of the HUD in screen coordinates */
float HUD_OFFSET[]  = {10.0f,10.f};

/** The name of the ship texture */
#define SHIP_TEXTURE        "ship"
/** The name of the space texture */
#define SPACE_TEXTURE       "space"
/** The name of the first planet texture */
#define PLANET1_TEXTURE     "planet1"
/** The name of the second planet texture */
#define PLANET2_TEXTURE     "planet2"
/** The name of the HUD font */
#define HUD_FONT_NAME       "felt32"


/** Ship Frame Sprite numbers */
#define SHIP_IMG_LEFT   0   // Left bank frame
#define SHIP_IMG_FLAT   9   // Neutral frame
#define SHIP_IMG_RIGHT 17   // Right bank frame
/** Number of rows and cols in film strip */
#define SHIP_IMG_ROWS   4
#define SHIP_IMG_COLS   5
#define SHIP_IMG_SIZE  18


#pragma mark -
#pragma mark Constructors


/**
 * Initializes the controller contents, and starts the game
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool GameMode::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    if (assets == nullptr) {
        return false;
    }
    _assets = assets;
    
    // Start up the input handler
    _input.init();

    // Create the scene graph
    Size dimen = Application::get()->getDisplaySize();
    dimen *= GAME_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    
    _scene = Scene::alloc(dimen);
    
    // Get the space background.  Its size determines all scaling.
    auto bkgd = _assets->get<Texture>(SPACE_TEXTURE);
    Size tileSize = bkgd->getSize();
    
    // Compute the ship position from this file
    Vec2 shipPos = (Vec2)(tileSize*TILE_AMOUNT/2.0f);
    
    // Create the master node.  This, unlike the scene graph, is resolution independent.
    // If we do not do this, the children will not all line up correctly on different devices.
    Vec2 center(dimen.width/2.0f,dimen.height/2.0f);
    _allSpace = Node::alloc();
    _allSpace->setContentSize(tileSize*TILE_AMOUNT);
    _allSpace->setAnchor(Vec2::ANCHOR_MIDDLE);
    _allSpace->setPosition(center);
    
    // Everything else is relative to all space, not the screen!
    center.set(_allSpace->getContentSize().width/2.0f,_allSpace->getContentSize().height/2.0f);
    _farSpace = Node::alloc();
    _farSpace->setContentSize(_allSpace->getContentSize());
    _farSpace->setAnchor(Vec2::ANCHOR_MIDDLE);
    _farSpace->setPosition(center);
    
    // Tile the background with deep space
    Vec2 rivet;
    for(int ii = -TILE_AMOUNT/2; ii <= TILE_AMOUNT/2; ii++) {
        for(int jj = -TILE_AMOUNT/2; jj <= TILE_AMOUNT/2; jj++) {
            // Create a new autorelease sprite for each tile
            auto tile = PolygonNode::allocWithTexture(bkgd);
            rivet.x = _farSpace->getContentSize().width/2.0f+ii*tileSize.width;
            rivet.y = _farSpace->getContentSize().height/2.0f+jj*tileSize.height;
            tile->setAnchor(Vec2::ANCHOR_MIDDLE);
            tile->setPosition(rivet);
            _farSpace->addChild(tile);
        }
    }
    
    // Put planets in the foreground.
    _nearSpace = Node::alloc();
    _nearSpace->setContentSize(_allSpace->getContentSize());
    _nearSpace->setAnchor(Vec2::ANCHOR_MIDDLE);
    _nearSpace->setPosition(center);
    
    // Planet positions are absolute.
    std::shared_ptr<Texture> image = _assets->get<Texture>(PLANET1_TEXTURE);
    std::shared_ptr<PolygonNode> planet = PolygonNode::allocWithTexture(image);
    planet->setScale(PLANET_SCALE,PLANET_SCALE);
    planet->setPosition(shipPos+((Vec2)PLANET1_OFF));
    planet->setAnchor(Vec2::ANCHOR_MIDDLE);
    _nearSpace->addChild(planet);
    
    image = _assets->get<Texture>(PLANET2_TEXTURE);
    planet = PolygonNode::allocWithTexture(image);
    planet->setScale(PLANET_SCALE,PLANET_SCALE);
    planet->setPosition(shipPos+((Vec2)PLANET2_OFF));
    planet->setAnchor(Vec2::ANCHOR_MIDDLE);
    _nearSpace->addChild(planet);
    
    image = _assets->get<Texture>(SHIP_TEXTURE);
    _shipNode = AnimationNode::alloc(image,SHIP_IMG_ROWS,SHIP_IMG_COLS,SHIP_IMG_SIZE);
    
    auto font = _assets->get<Font>(HUD_FONT_NAME);
    _coordHUD = Label::alloc(positionText(shipPos)+"  ",font);
    _coordHUD->setForeground(Color4::WHITE);
    
    // We want the HUD in the lower left corner in screen coordinates
    _coordHUD->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _coordHUD->setPosition((Vec2)HUD_OFFSET);
    
    // Remove the welcome screen and display the game.
    _allSpace->addChild(_farSpace,0);
    _allSpace->addChild(_nearSpace,1);
    _allSpace->addChild(_shipNode,2);
    _scene->addChild(_allSpace,0);
    _scene->addChild(_coordHUD,1);  // On top of scene graph.
    
    // Create the ship model
    _shipModel = ShipModel::alloc(shipPos);
    _shipModel->setSprite(_shipNode);
    
    _active = true;
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void GameMode::dispose() {
    if (_active) {
        _input.dispose();
        _scene = nullptr;
        _allSpace = nullptr;
        _farSpace = nullptr;
        _nearSpace = nullptr;
        _shipNode = nullptr;
        _shipModel = nullptr;
        _active = false;
    }
}


#pragma mark -
#pragma mark Gameplay Handling

/**
 * Resets the status of the game so that we can play again.
 */
void GameMode::reset() {
    // Reset the ships and input
    _shipModel->reset();
    _input.clear();
    
    // Reset the parallax
    Vec2 position = _farSpace->getPosition();
    _farSpace->setAnchor(Vec2::ANCHOR_MIDDLE);
    _farSpace->setPosition(position);
    _farSpace->setAngle(0.0f);
    position = _nearSpace->getPosition();
    _nearSpace->setAnchor(Vec2::ANCHOR_MIDDLE);
    _nearSpace->setPosition(position);
    _nearSpace->setAngle(0.0f);
}

/**
 * The method called to update the game mode.
 *
 * This method contains any gameplay code that is not an OpenGL call.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void GameMode::update(float timestep) {
    _input.update(timestep);
    
    // Reset the game if necessary
    if (_input.didReset()) { reset(); }

    Vec2 thrust = _input.getThrust();
    
    // Move the ship (MODEL ONLY)
    _shipModel->setForward(thrust.y);
    _shipModel->setTurning(thrust.x);
    _shipModel->update(timestep);
    
    // "Drawing" code.  Move everything BUT the ship
    // Update the HUD
    _coordHUD->setText(positionText(_shipModel->getPosition()));
    
    Vec2 offset = _shipModel->getPosition()-_farSpace->getPosition();
    
    // Anchor points are in texture coordinates (0 to 1). Scale it.
    offset.x = offset.x/_allSpace->getContentSize().width;
    offset.y = offset.y/_allSpace->getContentSize().height;
    
    // Reanchor the node at the center of the screen and rotate about center.
    Vec2 position = _farSpace->getPosition();
    _farSpace->setAnchor(offset*PARALLAX_AMT+Vec2::ANCHOR_MIDDLE);
    _farSpace->setPosition(position); // Reseting the anchor changes the position
    _farSpace->setAngle(_shipModel->getAngle());
    
    // Reanchor the node at the center of the screen and rotate about center.
    position = _nearSpace->getPosition();
    _nearSpace->setAnchor(offset+Vec2::ANCHOR_MIDDLE);
    _nearSpace->setPosition(position); // Reseting the anchor changes the position
    _nearSpace->setAngle(_shipModel->getAngle());
}


/**
 * The method called to draw the application to the screen.
 *
 * This method should contain OpenGL and related drawing calls.
 */
void GameMode::draw(const std::shared_ptr<SpriteBatch>& _batch) {
    _scene->render(_batch);
}

/**
 * Returns an informative string for the position
 *
 * This function is for writing the current ship position to the HUD.
 *
 * @param coords The current ship coordinates
 *
 * @return an informative string for the position
 */
std::string GameMode::positionText(const cugl::Vec2& coords) {
    stringstream ss;
    ss << "Coords: (" << (int)coords.x/10 << "," << (int)coords.y/10 << ")";
    return ss.str();
}
