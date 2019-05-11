#include "Engine.h"
#include <string>
#include <vector>
#include "Definitions.h"
#include "GameObject.h"
#include "PhysicsDevice.h"
#include "RidgidBodyComponent.h"
#include "SpriteComponent.h"
#include "ParticleEmitter.h"
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
	vector<textOutput>().swap(textout);
}


void Engine::loadLevel(string path)
{
	clearText();
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
		gameover = true;
		sound->setAsBackground(string("win.wav"));
		SDL_SetRenderDrawColor(gDevice->getRenderer(), 0, 0, 0, 255);
		int center = gDevice->getWidth() / 2;
		queueText({ "CONGRATULATIONS", center, 50, 45, 255, 0, 255, CENTER });
		queueText({ "YOU WIN!!", center, 150, 35, 0, 192, 255, CENTER });
		queueText({ "Final score: " + std::to_string(final_score), center, 200, 20, 255, 255, 255, CENTER });
		queueText({ "Team Rocket Spring 2019", center, 280, 20, 255, 255, 255, CENTER });
		queueText({ "Finis Martin", center, 330, 20, 128, 128, 255, CENTER });
		queueText({ "Josh Pyburn", center, 360, 20, 128, 128, 255, CENTER });
		queueText({ "Thanks for playing!", center, 450, 30, 196, 255, 64, CENTER });
		shared_ptr<GameObject> fireworks = make_shared<GameObject>();
		fireworks->type = objectTypes::COMPONENT;
		shared_ptr<Emitter> emitter = make_shared<Emitter>(fireworks);
		shared_ptr<ParticleParams> params = make_shared<ParticleParams>();
		params->x = center;
		params->y = 200;
		params->texture = imgPath + "puff.png";
		params->cx = params->cy = 2.5f;
		params->rx = params->ry = 0;
		params->rw = params->rh = 5;
		params->lifespan = -1;
		params->parttime = 20;
		params->ppf = 50;
		params->range = 360.f;
		params->speed = 20.f;
		params->gravity = { 0.f, 1.5f };
		params->color.setRGB(0xaaff60);
		params->endcol.setRGB(0xaa3030);
		emitter->initialize(gDevice.get(), params);
		fireworks->addComponent(emitter);
		fireworks->initialize();
		objects.push_back(fireworks);
		return;
	}

	// Load BackgroundMusic
		const char* bgm;
		if (root->QueryStringAttribute("bgMusic", &bgm) == XML_SUCCESS && currentsong != bgm) {
			currentsong = bgm;
			sound->setAsBackground(currentsong);
		}

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
		if (!gameover) {
			final_score += score;
			score = 0;
			int center = gDevice->getWidth() / 2;
			int middle = gDevice->getHeight() / 2;
			queueText({ "Level " + to_string(level) + " completed", center, middle - 20, 40, 255, 255, 255, CENTER });
			queueText({ "Score: " + to_string(final_score), center, middle + 40, 20, 128, 128, 255, CENTER });
			queueText({ "Press [Enter] to continue", center, middle + 70, 12, 128, 128, 196, CENTER });
			SDL_SetRenderDrawColor(gDevice->getRenderer(), 0, 0, 64, 255);
		}
		lobby = true;
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
	
	if(batchText()) drawScore();
	;
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
		if (Event.type == SDL_KEYDOWN && gameover)exit(0);
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
			score = 0;
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

void Engine::queueText(textOutput output)
{
	textout.push_back(output);
}

bool Engine::batchText()
{
	for (auto t : textout) drawText(t);
	return textout.size() == 0;
}

void Engine::clearText()
{
	vector<textOutput>().swap(textout);
}

void Engine::drawText(textOutput output)
{
	TTF_Font* normalFont = TTF_OpenFont((fontPath + "PixelOperator8-Bold.ttf").c_str(), output.size);
	SDL_Color textColor = { output.red, output.green, output.blue };
	SDL_Texture* textSheetTexture = SDL_CreateTextureFromSurface(gDevice.get()->getRenderer(), TTF_RenderText_Solid(normalFont, output.text.c_str(), textColor));
	int width, height;
	SDL_QueryTexture(textSheetTexture, NULL, NULL, &width, &height);
	int rx;
	switch (output.align) {
	case LEFT: rx = output.x; break;
	case RIGHT: rx = output.x - width; break;
	case CENTER: rx = output.x - width / 2; break;
	}
	SDL_Rect renderQuad = { rx, output.y, width, height };
	//Render to screen
	SDL_RenderCopy(gDevice.get()->getRenderer(), textSheetTexture, NULL, &renderQuad);
}

void Engine::drawScore()
{
	scoreText.text = to_string(final_score + score);
	drawText(scoreText);
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
