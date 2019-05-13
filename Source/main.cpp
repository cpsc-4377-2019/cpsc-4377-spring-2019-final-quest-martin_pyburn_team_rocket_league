#include "Definitions.h"
#include "engine.h"

int main(int argc, char *argv[])
{
	string path = "./Assets/Config/level1.xml";
	Engine* e = new Engine(path);
	while (e->run()) {}
	return 0;
}