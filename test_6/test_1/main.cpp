#include "precomp.h"
#include "main.h"
#include "game.h"
#include "towers.h"

int Program::main(const std::vector<std::string> &args)
{
	SetupCore setup_core;
	SetupDisplay setup_display;
	SetupD3D setup_d3d;
	SetupGL setup_gl;
	clan::SetupSWRender setup_swrender;
	clan::SetupGUI setup_gui;

	SetupSound setup_sound;
	SoundOutput output(44100);

	game game;
	game.run();

	return 0;
}

// Create global application object:
// You MUST include this line or the application start-up will fail to locate your application object.
Application app(&Program::main);