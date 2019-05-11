#ifndef ENGINE_H
#define ENGINE_H
#include "Timer.h"
#include "GraphicsDevice.h"
#include "ObjectFactory.h"
#include "TextureLibrary.h"
#include "SoundDevice.h"
#include "PhysicsDevice.h"
#include "InputHandler.h"

#include <vector>
using namespace std;

class MiniMap;

class Engine
{

public:
	Engine(std::string path);
	~Engine();

	void loadLevel(std::string levelPath);
	void update();
	void draw();
	bool run();

	enum textAlignment { LEFT, RIGHT, CENTER };
	struct textOutput {
		string text{ "" };
		int x{ 0 };
		int y{ 0 };
		int size{ 20 };
		int red{ 255 };
		int green{ 255 };
		int blue{ 255 };
		textAlignment align{ LEFT };
	};

	textAlignment t_align{ LEFT };
	void queueText(textOutput);
	bool batchText();
	void clearText();
	void drawText(textOutput);
	void drawScore();

	GameObject* getShip();


private:
	// get stuff
	vector<GameObject*> getPlanetoids();
	vector<GameObject*> getEnemies();
	vector<GameObject*> getPowerUps();
	vector<GameObject*> getMunitions();
	vector<GameObject*> getComponents();

	vector<shared_ptr<GameObject>> objects;
	unique_ptr<GraphicsDevice> gDevice = make_unique<GraphicsDevice>();

	// create input handler
	shared_ptr<InputHandler> input = make_shared<InputHandler>();

	// create texture library
	unique_ptr<TextureLibrary> library =
		make_unique<TextureLibrary>(gDevice.get());

	// create sound device
	unique_ptr<SoundDevice> sound = make_unique<SoundDevice>();

	// create physics device
	unique_ptr<PhysicsDevice> physics =
		make_unique<PhysicsDevice>(Vector2D{ 0.f, 0.f }, gDevice.get());	// there is no gravity

	// create factory
	unique_ptr<ObjectFactory> factory =
		make_unique<ObjectFactory>(this, gDevice->getRenderer(), input.get(), library.get(), physics.get(), gDevice.get());

	// paths
	string imgPath = "./Assets/Images/";
	string xmlPath = "./Assets/Config/";
	string musicPath = "./Assets/Music/";
	string soundPath = "./Assets/Sounds/";
	string fontPath = "./Assets/Fonts/";
	
	// MiniMap
	unique_ptr<MiniMap> map = make_unique<MiniMap>(gDevice.get(), imgPath);
	bool drawMap{ true };

	vector<Uint32> deathRow;
	vector<shared_ptr<GameObject>> nursery;

	///////////////

	bool paused{ false };
	bool lobby{ false };
	bool debug{ false };
	bool gameover{ false };
	int level{ 0 };
	int final_score = 0;
	int score = 0;

	GameObject* ship;

	string current_map{ "" };
	string currentsong;

	unique_ptr<RidgidBody> leftBound	= make_unique<RidgidBody>(make_unique<GameObject>());
	unique_ptr<RidgidBody> rightBound	= make_unique<RidgidBody>(make_unique<GameObject>());
	unique_ptr<RidgidBody> topBound		= make_unique<RidgidBody>(make_unique<GameObject>());
	unique_ptr<RidgidBody> bottomBound	= make_unique<RidgidBody>(make_unique<GameObject>());

	// text rendering
	vector<textOutput> textout;
	textOutput scoreText{ "0", gDevice->getWidth() - 5, 5, 20, 255, 255, 255, RIGHT };
};


#endif