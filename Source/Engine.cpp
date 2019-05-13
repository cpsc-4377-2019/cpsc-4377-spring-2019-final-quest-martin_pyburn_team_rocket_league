#include "Engine.h"
#include <string>
#include <vector>
#include "Definitions.h"
#include "GameObject.h"
#include "PhysicsDevice.h"
#include "RidgidBodyComponent.h"
#include "SpriteComponent.h"
#include "FireworksLauncher.h"
#include "MiniMap.h"
#include "SDL_ttf.h"

Engine::Engine(string path)
{
	// Apply settings
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
		printf("Bad File Path");
		exit(1);
	}
	//Get root
	tinyxml2::XMLElement* root = doc.FirstChildElement("Game");

	int width = 800, height = 800;

	root = root->FirstChildElement("Settings");
	if (root != nullptr) {
		tinyxml2::XMLElement* volume = root->FirstChildElement("Volume");
		if (volume != nullptr) {
			int music;
			if (volume->QueryIntAttribute("music", &music) == XML_SUCCESS) {
				sound->setMusicVolume(music);
			}
			int noise;
			if (volume->QueryIntAttribute("sound", &noise) == XML_SUCCESS) {
				sound->setSoundVolume(noise);
			}
		}
		tinyxml2::XMLElement* resolution = root->FirstChildElement("Resolution");
		if (resolution != nullptr) {
			resolution->QueryIntAttribute("width", &width);
			resolution->QueryIntAttribute("height", &height);
		}
	}

	gDevice = make_shared<GraphicsDevice>(width, height);
	input = make_shared<InputHandler>();
	library = make_shared<TextureLibrary>(resources);
	sound = make_shared<SoundDevice>();
	physics = make_shared<PhysicsDevice>(Vector2D{ 0.f, 0.f }, gDevice.get());
	map = make_unique<MiniMap>(gDevice.get(), imgPath);
	scoreText = { "0", gDevice->getWidth() - 5, 5, 20, 255, 255, 255, RIGHT };

	resources->engine = this;
	resources->graphics = gDevice;
	resources->physics = physics;
	resources->library = library;
	resources->sounds = sound;
	resources->inputs = input;

	factory = make_shared<ObjectFactory>(resources);

	resources->factory = factory;

	resources->imgPath = imgPath;
	resources->xmlPath = xmlPath;
	sound->soundPath = soundPath;
	sound->musicPath = musicPath;

	current_map = path;

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
		queueText({ "CONGRATULATIONS", center, 50, 45, 255, 32, 32, CENTER });
		queueText({ "YOU WIN!!", center, 150, 35, 0, 192, 255, CENTER });
		queueText({ "Final score: " + std::to_string(final_score), center, 200, 20, 255, 255, 255, CENTER });
		queueText({ "Team Rocket Spring 2019", center, 280, 20, 255, 255, 255, CENTER });
		queueText({ "Finis Martin", center, 330, 20, 128, 128, 255, CENTER });
		queueText({ "Josh Pyburn", center, 360, 20, 128, 128, 255, CENTER });
		queueText({ "Thanks for playing!", center, 450, 30, 255, 196, 64, CENTER });
		
		// fireworks show
		for (int l = 0; l < 8; l++) {
			eFloat angle = -PI / 2;
			angle += PI / 24 * (l - 3);
			shared_ptr<GameObject> gob = make_shared<GameObject>();
			shared_ptr<ObjectTemplate> bt = make_shared<ObjectTemplate>();
			ObjectParams* bp = (*bt)[BODY].get();
			bp->set = true;
			bp->x = (eFloat)center + cosf(angle) * 200.f;
			bp->y = 850.f + sinf(angle) * 200.f;
			factory->applyTemplate(bt, gob);
			shared_ptr<RidgidBody> body = gob->getComponent<RidgidBody>();
			body->setAngle(angle * RAD_TO_DEG);
			body->setType(BodyType::Static);
			shared_ptr<FWLauncher> launcher = make_shared<FWLauncher>(gob);
			launcher->initialize(resources);
			gob->addComponent(launcher);
			gob->initialize();
			gob->type = objectTypes::COMPONENT;
			objects.push_back(gob);
		}
		
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
	gDevice->begin();

	if (gameover) {
		shared_ptr<Texture> background = library->getArtAsset("background.png");
		Vector2D tl = {
			gDevice->getWidth() / 2 - background->width / 2,
			gDevice->getHeight() / 2 - background->height / 2
		};
		background->draw(tl);
	}

	vector<GameObject*> planetoids	= getPlanetoids();
	vector<GameObject*> powerups	= getPowerUps();
	vector<GameObject*> enemies		= getEnemies();
	vector<GameObject*> munitions	= getMunitions();
	vector<GameObject*> components	= getComponents();
	GameObject* ship				= getShip();

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
	SDL_Color textColor = { (Uint8)output.red, (Uint8)output.green, (Uint8)output.blue };
	SDL_Surface* textSurface = TTF_RenderText_Solid(normalFont, output.text.c_str(), textColor);
	TTF_CloseFont(normalFont);
	SDL_Texture* textSheetTexture = SDL_CreateTextureFromSurface(gDevice.get()->getRenderer(), textSurface);
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
	SDL_FreeSurface(textSurface);
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
			list->push_back( objects[it].get());
	return *list;
}

vector<GameObject*> Engine::getEnemies()
{
	unique_ptr<vector<GameObject*>> list = make_unique<vector<GameObject*>>();
	for (Uint32 it = 0; it < objects.size(); it++)
		if (objects[it]->type == objectTypes::ENEMY)
			list->push_back( objects[it].get());
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
