#include "Engine.h"
#include <string>
#include <vector>
#include "Definitions.h"
#include "GameObject.h"
#include "PhysicsDevice.h"
#include "RidgidBodyComponent.h"
#include "SpriteComponent.h"
#include "MiniMap.h"
#include "SDL_ttf.h"

Engine::Engine(string path)
{
	current_map = path;

	library->imgPath = imgPath;
	factory->xmlPath = xmlPath;
	sound->musicPath = musicPath;
	sound->soundPath = soundPath;

	loadLevel(path);

	// hide cursor
	SDL_ShowCursor(SDL_DISABLE);
	SDL_StopTextInput();

	TTF_Init();

	// establish boundaries
	int w = gDevice->getWidth();
	int h = gDevice->getHeight();

	physics->createBoundary(leftBound.get(), -20, 0, 20, h);
	physics->createBoundary(topBound.get(), 0, -20, w, 20);
	physics->createBoundary(rightBound.get(), w + 20, 0, 20, h);
	physics->createBoundary(bottomBound.get(), 0, h + 20, w, 20);

	// Apply settings
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
		printf("Bad File Path");
		exit(1);
	}
	//Get root
	tinyxml2::XMLElement* root = doc.FirstChildElement("Game");
	root = root->FirstChildElement("Settings");
	tinyxml2::XMLElement* volume = root->FirstChildElement("Volume");
	int music;
	if (volume->QueryIntAttribute("music", &music) == XML_SUCCESS) {
		sound->setMusicVolume(music);
	}
	int noise;
	if (volume->QueryIntAttribute("sound", &noise) == XML_SUCCESS) {
		sound->setMusicVolume(noise);
	}
}


Engine::~Engine() {
	vector<shared_ptr<GameObject>>().swap(objects);
}


void Engine::loadLevel(string path)
{
	current_map = path;
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
		printf("Bad File Path");
		exit(1);
	}

	//Get root
	tinyxml2::XMLElement* root = doc.FirstChildElement("Game");
	root = root->FirstChildElement("Level");

	// Find the current Level in the XML
	for (int l = 0; l < level; l++)root = root->NextSiblingElement("Level");

	// check if END
	if (root == nullptr) {
		SDL_SetRenderDrawColor(gDevice->getRenderer(), 0, 64, 0, 255);
		cout << "CONGRATULATIONS!!!" << endl;
		system("pause");
		exit(0);
		// END OF GAME
		return;
	}

	// Load BackgroundMusic
		const char* bgm;
		if (root->QueryStringAttribute("bgMusic", &bgm) == XML_SUCCESS)
			sound->setAsBackground((string)bgm);

	// Load GameObjects from XML
	for (tinyxml2::XMLElement* obj = root->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		int at = objects.size();
		factory->applyXML(obj, &objects);
		objects.at(at)->initialize();
	}
}

void Engine::update()
{
	while (deathRow.size()) {
		if(objects[deathRow[0]] != nullptr)
			objects.erase(objects.begin() + deathRow[0]);
		deathRow.erase(deathRow.begin());
	}
	deathRow.clear();

	int w = gDevice->getWidth();
	int h = gDevice->getHeight();

	Uint32 it;
	bool win = true;

	for (it = 0; it < objects.size(); it++ )
	{
		if (it == objects.size() - 1)objects[it]->draw();
		if (objects[it]->live) {
			if (objects[it]->type == objectTypes::PLANETOID || objects[it]->type == objectTypes::ENEMY)
				win = false;
			std::shared_ptr<GameObject> newObject = objects[it]->update();
			if (objects[it]->type == objectTypes::SHIP && objects[it]->score > 0)
			{
				score += objects[it]->score;
				objects[it]->score = 0;
			}
			if (newObject != nullptr) {
				nursery.push_back(newObject);
			}
		}
		else if (objects[it]->type == objectTypes::SHIP) {
			// the ship is dead, the screen is red
			win = false;
			SDL_SetRenderDrawColor(gDevice->getRenderer(), 192, 0, 0, 255);
		}
		else {
			deathRow.push_back(it);
		}
	}
	if (win && !lobby) {
		level++;
		lobby = true;
		SDL_SetRenderDrawColor(gDevice->getRenderer(), 0, 0, 64, 255);
		// Display final score
	}
	for (it = 0; it < nursery.size(); it++) {
		objects.push_back(nursery[it]);
		nursery[it]->initialize();
	}
	nursery.clear();
	physics->update(1.0f / gDevice->FPS);
}

void Engine::draw()
{
	vector<GameObject*> planetoids	= getPlanetoids();
	vector<GameObject*> powerups	= getPowerUps();
	vector<GameObject*> enemies		= getEnemies();
	vector<GameObject*> munitions	= getMunitions();
	vector<GameObject*> components	= getComponents();
	GameObject* ship				= getShip();

	gDevice->begin();
	for (Uint32 it = 0; it < planetoids.size(); it++)
		planetoids[it]->draw();
	for (Uint32 it = 0; it < powerups.size(); it++)
		powerups[it]->draw();
	for (Uint32 it = 0; it < enemies.size(); it++)
		enemies[it]->draw();
	for (Uint32 it = 0; it < munitions.size(); it++)
		munitions[it]->draw();
	if(ship != nullptr)ship->draw();
	for (Uint32 it = 0; it < components.size(); it++)
		components[it]->draw();

	// draw mini map
	if(drawMap) map->draw(planetoids, powerups, enemies, ship);

	if(debug)physics->debugDraw();
	
	drawScore();
	gDevice->present();
}

bool Engine::run()
{
	//Construct Frame Timer
	std::unique_ptr<Timer> timer = make_unique<Timer>();
	if (!timer->Initialize(gDevice->FPS))
	{
		printf("Frame Timer could not intialize! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}


	SDL_Event Event;

	while (SDL_PollEvent(&Event))
	{
		if (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_RETURN && lobby) {
			lobby = false;
			paused = true;
			SDL_SetRenderDrawColor(gDevice->getRenderer(), 0, 0, 0, 255);
			objects.clear();
			loadLevel(current_map);
			paused = false;
		}
		input->update( &Event );
		if ( input->keyStates[ InputHandler::Inputs::QUIT ] ) return false;
		if (input->keyStates[InputHandler::Inputs::PAUSE]) {
			paused = !paused;
		}
		if ( input->keyStates[ InputHandler::Inputs::RELOAD] ){
			paused = true;
			SDL_SetRenderDrawColor(gDevice->getRenderer(), 0, 0, 0, 255);
			objects.clear();
			loadLevel(current_map);
			paused = false;
		}
		if (input->keyStates[InputHandler::Inputs::MAP]) drawMap = !drawMap;
		if (input->keyStates[InputHandler::Inputs::DEBUG]) debug = !debug;

		// set up input for debug
	}

	if (!paused) {
		//Start Frame Timer
		timer->start();

		// update objects
		update();

		// draw objects
		draw();

		//pauses until proper refresh time has passed.
		timer->fpsRegulate();
	}
	else physics->update(0.f);

	return true;
}

void Engine::drawScore()
{
	TTF_Font* normalFont = TTF_OpenFont((fontPath + "PixelOperator8-Bold.ttf").c_str(), 20);
	if(normalFont == nullptr) printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
	SDL_Color textColor = { 225, 225, 225 };
	int tempScore = score;
	string scoreText = std::to_string(score);
	SDL_Texture* textSheetTexture = SDL_CreateTextureFromSurface(gDevice.get()->getRenderer(), TTF_RenderText_Solid(normalFont, scoreText.c_str(), textColor));
	int width, height;
	int textX, textY;
	SDL_QueryTexture(textSheetTexture, NULL, NULL, &width, &height);
	textX = (gDevice->getWidth() - width - 5);
	textY = 5;
	SDL_Rect renderQuad = { textX, textY, width, height };
	//Render to screen
	SDL_RenderCopy(gDevice.get()->getRenderer(), textSheetTexture, NULL, &renderQuad);
}

GameObject* Engine::getShip()
{
	for (Uint32 it = 0; it < objects.size(); it++)
		if (objects[it]->type == objectTypes::SHIP && objects[it]->live) return objects[it].get();
	return nullptr;
}

vector<GameObject*> Engine::getComponents()
{
	unique_ptr<vector<GameObject*>> list = make_unique<vector<GameObject*>>();
	for (Uint32 it = 0; it < objects.size(); it++)
		if (objects[it]->type == objectTypes::COMPONENT)
			list->push_back(objects[it].get());
	return *list;
}

vector<GameObject*> Engine::getPlanetoids()
{
	unique_ptr<vector<GameObject*>> list = make_unique<vector<GameObject*>>();
	for (Uint32 it = 0; it < objects.size(); it++)
		if (objects[it]->type == objectTypes::PLANETOID)
			list->push_back( objects[it].get() );
	return *list;
}

vector<GameObject*> Engine::getEnemies()
{
	unique_ptr<vector<GameObject*>> list = make_unique<vector<GameObject*>>();
	for (Uint32 it = 0; it < objects.size(); it++)
		if (objects[it]->type == objectTypes::ENEMY)
			list->push_back( objects[it].get() );
	return *list;
}

vector<GameObject*> Engine::getPowerUps()
{
	unique_ptr<vector<GameObject*>> list = make_unique<vector<GameObject*>>();
	for (Uint32 it = 0; it < objects.size(); it++)
		if (objects[it]->type == objectTypes::POWERUP)
			list->push_back(objects[it].get());
	return *list;
}

vector<GameObject*> Engine::getMunitions()
{
	unique_ptr<vector<GameObject*>> list = make_unique<vector<GameObject*>>();
	for (Uint32 it = 0; it < objects.size(); it++)
		if (objects[it]->type == objectTypes::BULLET || objects[it]->type == objectTypes::MISSILE)
			list->push_back(objects[it].get());
	return *list;
}
