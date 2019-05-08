#include "Planetoid.h"
#include "Definitions.h"

Planetoid::Planetoid( PlanetoidParams params )
{
	objectType = objectTypes::PLANETOID;
	integrity = params.area;
	layer = params.layer;
	shell = params.shell;
	index = params.index;
}

void Planetoid::appendChild(shared_ptr<Planetoid> newChild)
{
	children.get()->push_back(newChild);
}

Object* Planetoid::update(float windowWidth, float windowHeight) {
	position.x = pos_mod(position.x + velocity.x, windowWidth);
	position.y = pos_mod(position.y + velocity.y, windowHeight);

	position.angle += velocity.angle;


	return this;
}
void Planetoid::draw()
{
	img->draw(&position, &center, &srcRect);
}

string Planetoid::values() {
	return "index: " + to_string( index ) +
		" integrity: " + to_string( integrity ) +
		" layer: " + to_string( layer ) +
		" shell: " + ( shell ? "true" : "false" ) +
		" x: " + to_string( position.x ) +
		" y: " + to_string( position.y ) +
		" angle: " + to_string( position.angle ) +
		" vx: " + to_string( velocity.x ) +
		" vy: " + to_string( velocity.y ) +
		" vangle: " + to_string( velocity.angle ) +
		" cx: " + to_string( center.x ) +
		" cy: " + to_string( center.y ) +
		" sx: " + to_string( sourceX ) +
		" sy: " + to_string( sourceY ) +
		" sw: " + to_string( sourceW ) +
		" sh: " + to_string( sourceH );
}

shared_ptr<vector<shared_ptr<Planetoid>>> Planetoid::damage(float dmg)
{
	integrity -= dmg;
	cout << dmg << " dmg - integrity at " << integrity << endl;
	if (integrity <= 0) return children;
	return nullptr;
}