/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial GUIlications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     GUIreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Mark Page
*/
#include "precomp.h"
#include "gui.h"

int GUI::main(const std::vector<std::string> &args)
{
	main_menu = true;
	std::string theme;
	if (clan::FileHelp::file_exists("../Resources/GUIThemeAero/theme.css"))
		theme = "../Resources/GUIThemeAero";
	else if (clan::FileHelp::file_exists("../Resources/GUIThemeBasic/theme.css"))
		theme = "../Resources/GUIThemeBasic";
	else
		throw clan::Exception("No themes found");

	clan::GUIManager gui(theme);

	clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("Main Menu");
	win_desc.set_position(clan::Rect(200, 200, 540, 440), false);
	clan::GUIComponent window(&gui, win_desc, "Window");
	window.func_close().set(this, &GUI::on_close, &window);

	clan::Rect client_area = window.get_content_box();

	clan::PushButton button(&window);
	button.set_geometry(clan::Rect(client_area.left + 10, client_area.top + 10, clan::Size(140, 40)));
	button.func_clicked().set(this, &GUI::on_button_clicked, &window);
	button.set_text("Start application");

	clan::PushButton button_2(&window);
	button_2.set_geometry(clan::Rect(client_area.left + 10, client_area.top + 60, clan::Size(140, 40)));
	button_2.func_clicked().set(this, &GUI::on_button_clicked_exit, &window);
	button_2.set_text("Exit");


	/*label = new clan::Label(&window);
	label->set_geometry(clan::Rect(client_area.left + 10, client_area.top + 160, clan::Size(330, 20)));
	label->set_text("Click the button");*/

	gui.exec();

	if (main_menu)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void GUI::on_button_clicked(clan::GUIComponent *win)
{
	//label->set_text("You clicked " + button->get_text());
	main_menu = false;
	win->exit_with_code(0);
}

void GUI::on_button_clicked_exit(clan::GUIComponent *win)
{
	//label->set_text("You clicked " + button->get_text());
	win->exit_with_code(0);
}

bool GUI::on_close(clan::GUIComponent *win)
{
	win->exit_with_code(0);
	return true;
}