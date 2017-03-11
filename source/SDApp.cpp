//
//  SDApp.cpp
//  Ship Demo
//
//  This is the root class for your game.  The file main.cpp accesses this class
//  to run the application.  While you could put most of your game logic in
//  this class, we prefer to break the game up into player modes and have a
//  class for each mode.
//
//  Author: Walker White
//  Version: 1/10/17
//
#include "SDApp.h"

using namespace cugl;


#pragma mark -
#pragma mark Gameplay Control

/**
 * The method called after OpenGL is initialized, but before running the application.
 *
 * This is the method in which all user-defined program intialization should
 * take place.  You should not create a new init() method.
 *
 * When overriding this method, you should call the parent method as the
 * very last line.  This ensures that the state will transition to FOREGROUND,
 * causing the application to run.
 */
#define TIME_STEP 60
 // This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024
 //Try putting in gl commands (drl224)
static const GLfloat g_vertex_buffer_data[] = {
	-0.5f, -0.50f, -0.50f, // triangle 1 : begin
	-0.50f, -0.50f, 0.50f,
	-0.50f, 0.50f, 0.50f, // triangle 1 : end
	0.50f, 0.50f, -0.50f, // triangle 2 : begin
	-0.50f, -0.50f, -0.50f,
	-0.50f, 0.50f, -0.50f, // triangle 2 : end
	0.50f, -0.50f, 0.50f,
	-0.50f, -0.50f, -0.50f,
	0.50f, -0.50f, -0.50f,
	0.50f, 0.50f, -0.50f,
	0.50f, -0.50f, -0.50f,
	-0.50f, -0.50f, -0.50f,
	-0.50f, -0.50f, -0.50f,
	-0.50f, 0.50f, 0.50f,
	-0.50f, 0.50f, -0.50f,
	0.50f, -0.50f, 0.50f,
	-0.50f, -0.50f, 0.50f,
	-0.50f, -0.50f, -0.50f,
	-0.50f, 0.50f, 0.50f,
	-0.50f, -0.50f, 0.50f,
	0.50f, -0.50f, 0.50f,
	0.50f, 0.50f, 0.50f,
	0.50f, -0.50f, -0.50f,
	0.50f, 0.50f, -0.50f,
	0.50f, -0.50f, -0.50f,
	0.50f, 0.50f, 0.50f,
	0.50f, -0.50f, 0.50f,
	0.50f, 0.50f, 0.50f,
	0.50f, 0.50f, -0.50f,
	-0.50f, 0.50f, -0.50f,
	0.50f, 0.50f, 0.50f,
	-0.50f, 0.50f, -0.50f,
	-0.50f, 0.50f, 0.50f,
	0.50f, 0.50f, 0.50f,
	-0.50f, 0.50f, 0.50f,
	0.50f, -0.50f, 0.50f
};

static const GLfloat g_uv_buffer_data[] = {
	//0.000059f, 1.0f - 0.000004f,
	//0.000103f, 1.0f - 0.336048f,
	//0.335973f, 1.0f - 0.335903f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	//1.000023f, 1.0f - 0.000013f,
	//0.667979f, 1.0f - 0.335851f,
	//0.999958f, 1.0f - 0.336064f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	//0.667979f, 1.0f - 0.335851f,
	//0.336024f, 1.0f - 0.671877f,
	//0.667969f, 1.0f - 0.671889f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	//1.000023f, 1.0f - 0.000013f,
	//0.668104f, 1.0f - 0.000013f,
	//0.667979f, 1.0f - 0.335851f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	//0.000059f, 1.0f - 0.000004f,
	//0.335973f, 1.0f - 0.335903f,
	//0.336098f, 1.0f - 0.000071f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	//0.667979f, 1.0f - 0.335851f,
	//0.335973f, 1.0f - 0.335903f,
	//0.336024f, 1.0f - 0.671877f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	//1.000004f, 1.0f - 0.671847f,
	//0.999958f, 1.0f - 0.336064f,
	//0.667979f, 1.0f - 0.335851f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	//0.668104f, 1.0f - 0.000013f,
	//0.335973f, 1.0f - 0.335903f,
	//0.667979f, 1.0f - 0.335851f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	//0.335973f, 1.0f - 0.335903f,
	//0.668104f, 1.0f - 0.000013f,
	//0.336098f, 1.0f - 0.000071f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	//0.000103f, 1.0f - 0.336048f,
	//0.000004f, 1.0f - 0.671870f,
	//0.336024f, 1.0f - 0.671877f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	//0.000103f, 1.0f - 0.336048f,
	//0.336024f, 1.0f - 0.671877f,
	//0.335973f, 1.0f - 0.335903f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	//0.667969f, 1.0f - 0.671889f,
	//1.000004f, 1.0f - 0.671847f,
	//0.667979f, 1.0f - 0.335851f
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f
};
// This will identify our vertex buffer
GLuint vertexbuffer;
// This will identify our color buffer
GLuint colorbuffer;
// This will identify our uv buffer
GLuint uvbuffer;
//because I'm too lazy to put it in the header file
int _countdown;

void ShipApp::onStartup() {
	Size size = getDisplaySize();
	size *= GAME_WIDTH / size.width;

	// Create a scene graph the same size as the window
	//_scene = Scene::alloc(size.width, size.height);
	// get the camera from the scene
	//_camera = PerspectiveCamera::alloc(1, 90);
	// fill the dummy texture
	//_texture = SpriteBatch::getBlankTexture();
	//_texture->initWithFile("texture.JPG");
	//_texture->bind();

	// Create a sprite batch (and background color) to render the scene
	_batch = SpriteBatch::alloc();
	setClearColor(Color4(20, 5, 35, 40));
	_shader = SpriteShader::alloc();
	//_shader->init();
	// Create an asset manager to load all assets
	_assets = AssetManager::alloc();

	// You have to attach the individual loaders for each asset type
	_assets->attach<Texture>(TextureLoader::alloc()->getHook());
	_assets->attach<Font>(FontLoader::alloc()->getHook());

	// This reads the given JSON file and uses it to load all other assets
	_assets->loadDirectory("json/assets.json");

	// Activate mouse or touch screen input as appropriate
	// We have to do this BEFORE the scene, because the scene has a button
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// Generate 1 buffer, put the resulting identifier in uvbuffer
	glGenBuffers(1, &colorbuffer);
	// The following commands will talk about our 'uvbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

	// Generate 1 buffer, put the resulting identifier in uvbuffer
	glGenBuffers(1, &uvbuffer);
	// The following commands will talk about our 'uvbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);


#if defined (CU_TOUCH_SCREEN)
	Input::activate<Touchscreen>();
#else
	Input::activate<Mouse>();
#endif

	

	Application::onStartup(); // YOU MUST END with call to parent
}

/**
 * The method called when the application is ready to quit.
 *
 * This is the method to dispose of all resources allocated by this
 * application.  As a rule of thumb, everything created in onStartup()
 * should be deleted here.
 *
 * When overriding this method, you should call the parent method as the
 * very last line.  This ensures that the state will transition to NONE,
 * causing the application to be deleted.
 */
void ShipApp::onShutdown() {
    _loading.dispose();
    _gameplay.dispose();
    _assets = nullptr;
    _batch = nullptr;
    
    // Shutdown input
#ifdef CU_TOUCH_SCREEN
    Input::deactivate<Touchscreen>();
#else
    Input::deactivate<Mouse>();
#endif

    Application::onShutdown();  // YOU MUST END with call to parent
}

/**
 * The method called to update the application data.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should contain any code that is not an OpenGL call.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void ShipApp::update(float timestep) {
	if (_countdown == 0) {
		// Move the logo about the screen
		Size size = getDisplaySize();
		size *= GAME_WIDTH / size.width;
		float x = (float)(std::rand() % (int)(size.width / 2)) + size.width / 4;
		float y = (float)(std::rand() % (int)(size.height / 2)) + size.height / 8;
		//_logo->setPosition(Vec2(x, y));
		_countdown = TIME_STEP * 6;
	}
	else {
		_countdown--;
	}
}

/**
 * The method called to draw the application to the screen.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should OpenGL and related drawing calls.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 */
void ShipApp::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// This takes care of begin/end
	// An array of 3 vectors which represents 3 vertices
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);

	glVertexAttribPointer(
		1,                  // attribute 2. No particular reason for 2, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);



	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);

	glVertexAttribPointer(
		2,                  // attribute 2. No particular reason for 2, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glUseProgram(_shader->getProgram());

	//Set the camera transform
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	int width = getDisplayWidth();
	int height = getDisplayHeight();
	Mat4 ortho = Mat4::createOrthographic(4, 3, 0.1f, 100.0f);
	Mat4 Projection = Mat4::createPerspective(90, (float)width / (float)height, 0.1f, 100.0f);
	Projection = ortho;
	// Camera matrix
	float camerabump = (TIME_STEP * 6.0f - _countdown) / (float)TIME_STEP / 6.0f;
	float theta = 2.0f * M_PI * camerabump;
	float otherbump = ((float)_countdown / (float)TIME_STEP * 6) - 1;
	Mat4 View = Mat4::createLookAt(
		Vec3(2 * cosf(theta), 1.0f, 2 * sinf(theta)), // Camera is at (4,3,3), in World Space
													  //Vec3(-cosf(theta), 0, -(sinf(theta))), // and looks at the origin
		Vec3(0.0f, 0.0f, 0.0f),
		Vec3(0, 1.0f, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	//View = Mat4::IDENTITY;
	//Mat4 Dummy = Mat4()
	// Model matrix : an identity matrix (model will be at the origin)
	Mat4 Model = Mat4::IDENTITY;
	// Our ModelViewProjection : multiplication of our 3 matrices
	Mat4 mvp = Projection * View *Model;
	mvp = Model * View * Projection;
	std::shared_ptr<Texture> texture = _assets->get<Texture>("claw");
	//std::shared_ptr<Texture> texture;
	texture->bind();
	GLuint MatrixID = glGetUniformLocation(_shader->getProgram(), "uPerspective");
	// Send our transformation to the currently bound shader, in the "uPerspective" uniform
	// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp.m[0]);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	texture->unbind();

}


