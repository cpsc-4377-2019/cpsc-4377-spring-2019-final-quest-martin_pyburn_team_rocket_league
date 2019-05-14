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
	int drawText(textOutput);
	void drawHUD();

	GameObject* getShip();


private:
	// get stuff
	vector<GameObject*> getPlanetoids();
	vector<GameObject*> getEnemies();
	vector<GameObject*> getPowerUps();
	vector<GameObject*> getMunitions();
	vector<GameObject*> getComponents();


	shared_ptr<resource_map> resources = make_shared<resource_map>();

	vector<shared_ptr<GameObject>> objects;
	shared_ptr<GraphicsDevice> gDevice;

	// create input handler
	shared_ptr<InputHandler> input;

	// create texture library
	shared_ptr<TextureLibrary> library;

	// create sound device
	shared_ptr<SoundDevice> sound;

	// create physics device
	shared_ptr<PhysicsDevice> physics;

	// create factory
	shared_ptr<ObjectFactory> factory;

	// paths
	string imgPath = "./Assets/Images/";
	string xmlPath = "./Assets/Config/";
	string musicPath = "./Assets/Music/";
	string soundPath = "./Assets/Sounds/";
	string fontPath = "./Assets/Fonts/";
	
	// MiniMap
	unique_ptr<MiniMap> map;
	bool drawMap{ true };

	vector<Uint32> deathRow;
	vector<shared_ptr<GameObject>> nursery;

	///////////////

	bool paused{ false };
	bool lobby{ false };
	bool debug{ false };
	bool gameover{ false };
	bool playdead{ false };
	int level{ 0 };
	int final_score = 0;
	int score = 0;
	int HP_MAX{ 250 };
	GameObject* ship;

	string current_map{ "" };
	string currentsong;

	unique_ptr<RidgidBody> leftBound	= make_unique<RidgidBody>(make_unique<GameObject>());
	unique_ptr<RidgidBody> rightBound	= make_unique<RidgidBody>(make_unique<GameObject>());
	unique_ptr<RidgidBody> topBound		= make_unique<RidgidBody>(make_unique<GameObject>());
	unique_ptr<RidgidBody> bottomBound	= make_unique<RidgidBody>(make_unique<GameObject>());

	// text rendering
	vector<textOutput> textout;
	textOutput scoreText;
	textOutput hpText;
};


#endif