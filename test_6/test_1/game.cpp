#include "game.h"
#include "scene_cache.h"
#include "elapsed_timer.h"
#include <ClanLib/application.h>
#include <sstream>
#include "gui.h"
#include "SoundSession.h"

#include "game_component.h"
#include "toolbar.h"
#include "towers.h"
#include "Missile.h"
#include "Node.h"

#define left -50
#define right 50
#define top 20
#define down -90
#define mouse_move_speed 0.7

game::game()
{
}

game::~game()
{
	return;
}

void game::play_sample(int ID)
{
	//	0 - Explosion1
	//  1 - Explosion2
	//  2 - Hurt1
	//  3 - Hurt2
	//  4 - Powerup1
	//  5 - Shoot1

	if(ID>=0&&ID<total_samples)
	{
		if(current_channel==1) 
		{
			sound_session1 = samples[ID].prepare();
			sound_session1.play();
		}
		else if(current_channel==2) 
		{
			sound_session2 = samples[ID].prepare();
			sound_session2.play();
		}
		else if(current_channel==3) 
		{
			sound_session3 = samples[ID].prepare();
			sound_session3.play();
		}
		
		current_channel++;
		
		if(current_channel>total_channels)
		{
			current_channel=1;
		}
	}
}

void game::on_key_down(const InputEvent &key)
{
	if (key.id == keycode_escape)
		quit = true;
	/*if(key.id==keycode_w) playerP->up(true);
	else if(key.id==keycode_s) playerP->down(true);
	else if(key.id==keycode_a) playerP->left(true);
	else if(key.id==keycode_d) playerP->right(true);
	else if(key.id==keycode_m) mute_music();
	else if(key.id==keycode_escape) on_quit();*/

}
void game::on_key_up(const InputEvent &key)
{
	/*if(key.id==keycode_w) playerP->up(false);
	else if(key.id==keycode_s) playerP->down(false);
	else if(key.id==keycode_a) playerP->left(false);
	else if(key.id==keycode_d) playerP->right(false);*/

}
void game::on_pointer_move(const InputEvent &key)
{
	//playerP->aim_at(key.mouse_pos.x,key.mouse_pos.y);
}

void game::on_pointer_down(const InputEvent &key)
{
	cameraRestricted = true;
	mouse_pos =  Point(key.mouse_pos);
	if(key.id == mouse_left)
		show_radial_menu(mouse_pos);
}
void game::on_pointer_up(const InputEvent &key)
{
	cameraRestricted = false;
}

void game::show_radial_menu(Point &position)
{
	radial_menu->clear_items();
	radial_menu->add_item("Tower 1");
	radial_menu->add_item("Tower 2");
	radial_menu->add_item("Tower 3");
	//radial_menu->add_item("Push");
	//radial_menu->add_item("Pick up"); 
	//radial_menu->add_item("Kick");
	//radialMenu->add_item("Item 6");
	//radialMenu->add_item("Item 7");
	//radialMenu->add_item("Item 8");

	radial_menu->show(position);
}

void game::show_radial_menu_2(Point &position)
{
	radial_menu->clear_items();
	radial_menu->add_item("Sell");
	radial_menu->add_item("Upgrade");
	//radial_menu->add_item("Close");
	//radial_menu->add_item("Push");
	//radial_menu->add_item("Pick up"); 
	//radial_menu->add_item("Kick");
	//	radialMenu->add_item("Item 6");
	//	radialMenu->add_item("Item 7");
	//	radialMenu->add_item("Item 8");

	radial_menu->show(position);
}

void game::on_radial_menu_itemselected(int selected_index)
{
	selected_tower = radial_menu->get_item(selected_index);
	if (selected_tower != "")
		mouseUp = true;
}

void game::run()
{
	//MissileDesc projectile(*this);
	towerDesc towr(this);

	std::string txt;

	std::string txt2 = "Life:";

	std::string txt3 = "Money:";

	std::string main = "Cam Pos";

	std::string txt4 = "Game Over";

	std::list<SceneObject *> troll;

	endgame = false;
	mouseUp = false;

	float dir = 0.0f;

	float spot_dir = 45.0f;
    float aspect_time = 0.0f;

	towerCount = 0;
	cost = 0;

	life = 20;
	money = 10;
	quit = false;
	cameraRestricted = false;
	
	selected_tower = "";

	std::stringstream ss2 (std::stringstream::in | std::stringstream::out);
	std::stringstream ss3 (std::stringstream::in | std::stringstream::out);

	/*GUI app;
	int retval = app.main(args);

	if (retval == 0)
	{
		return retval;
	}*/


	Quaternionf camera_orientation(55.0f, 0.0f, 0.0f, angle_degrees, order_YXZ);
		
	clan::OpenGLWindowDescription opengl_desc;
	opengl_desc.set_version(3, 2, false);
	clan::OpenGLTarget::set_description(opengl_desc);

	//Scene test;
	//clan::SceneCamera camera(test);
	//DisplayWindowDescription innerwindow("Hello World", Size(640, 480), true);
	//innerwindow.set_allow_resize(true);
	//innerwindow.set_fullscreen(true);

	clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("Main");
	win_desc.set_size(clan::Size( 1100, 900 ), false);
	DisplayWindow window(win_desc);	
	//DisplayWindow window("main", 1200, 900, false, true);	
	//window.maximize();
	//DisplayWindow window(innerwindow);

	Rect viewport = window.get_viewport();

	GraphicContext gc = window.get_gc();

	ResourceManager resources;
	SceneCache::set(resources, std::shared_ptr<SceneCache>(new AppSceneCache()));

	ResourceManager sound_resources = XMLResourceManager::create(XMLResourceDocument("../Resources/resources.xml"));
	sound_resources_=&sound_resources;

	std::string shader_path = "../Resources/Scene3D";

	Scene scene(gc, resources, shader_path);

	sceneHolder = &scene;

	SceneCamera camera(scene);
	scene.set_camera(camera);
	scene.set_viewport(gc.get_size());

	camera.set_orientation(camera_orientation);
	camera.set_position(Vec3f(0.0f, 40.0f, down));

	Canvas canvas(window);

	image_hp = Image(canvas, "../Resources/Images/heart.png");
	image_hp.set_alignment(origin_center);
	image_hp.set_scale(0.07, 0.07);

	image_coin = Image(canvas, "../Resources/Images/coin.png");
	image_coin.set_alignment(origin_center);
	image_coin.set_scale(0.3, 0.3);


	GUIWindowManagerDirect direct(window, canvas);

	GUIManager maingui(direct, "../Resources");

	GUIComponent *win_comp = new GUIComponent(&maingui, win_desc, "");

	radial_menu = new RadialMenu(win_comp);

	radial_menu->func_selected.set(this, &game::on_radial_menu_itemselected);

	//GameComponent game_component(viewport, &maingui);

	/*Rect toolbar_rect = Rect((viewport.get_width() - 448) / 2, viewport.bottom - 56, (viewport.get_width() - 448) / 2 + 448, viewport.bottom);
	Toolbar toolbar(toolbar_rect, &game_component);	// GameComponent is the "desktop" that the toolbar sits on, as an owner

	toolbar.add_item(Sprite(canvas, "../Resources/Images/spell1.png"), Sprite(canvas, "../Resources/Images/spell1_selected.png"), Sprite(canvas, "../Resources/Images/spell1_clicked.png"));
	toolbar.add_item(Sprite(canvas, "../Resources/Images/spell2.png"), Sprite(canvas, "../Resources/Images/spell2_selected.png"), Sprite(canvas, "../Resources/Images/spell2_clicked.png"));
	toolbar.add_item(Sprite(canvas, "../Resources/Images/spell3.png"), Sprite(canvas, "../Resources/Images/spell3_selected.png"), Sprite(canvas, "../Resources/Images/spell3_clicked.png"));
	toolbar.add_item(Sprite(canvas, "../Resources/Images/spell4.png"), Sprite(canvas, "../Resources/Images/spell4_selected.png"), Sprite(canvas, "../Resources/Images/spell4_clicked.png"));
	toolbar.add_item(Sprite(canvas, "../Resources/Images/spell5.png"), Sprite(canvas, "../Resources/Images/spell5_selected.png"), Sprite(canvas, "../Resources/Images/spell5_clicked.png"));
	toolbar.add_item(Sprite(canvas, "../Resources/Images/spell6.png"), Sprite(canvas, "../Resources/Images/spell6_selected.png"), Sprite(canvas, "../Resources/Images/spell6_clicked.png"));
	toolbar.add_item(Sprite(canvas, "../Resources/Images/spell7.png"), Sprite(canvas, "../Resources/Images/spell7_selected.png"), Sprite(canvas, "../Resources/Images/spell7_clicked.png"));
	*/

	InputDevice keyboard = window.get_ic().get_keyboard();
	InputDevice mouse = window.get_ic().get_mouse();
	clan::Font font(canvas, "Tahoma", 30); // The clan prefix is required on linux due to a namespace conflict

	SceneModel plane(gc, scene, "plane");
	SceneModel box(gc, scene, "box");
	SceneModel tower(gc, scene, "tower");
	SceneModel gate(gc,scene,"gate");

	SceneObject object(scene, plane, Vec3f(0.0f, 0.0f, 0.0f));
	
	object.rotate(180.0f, 0.0f, 0.0f);

	scene.show_skybox_stars(false);

	/*SceneObject box0(scene, tower, Vec3f(20.0f, 5.0f, 0.0f), Quaternionf(0.0f, 0.0f, 0.0f, angle_degrees, order_YXZ));
	SceneObject box1(scene, tower, Vec3f(-20.0f, 5.0f, 0.0f), Quaternionf(0.0f, 0.0f, 0.0f, angle_degrees, order_YXZ));
	SceneObject box2(scene, tower, Vec3f(0.0f, 5.0f, 20.0f), Quaternionf(0.0f, 0.0f, 0.0f, angle_degrees, order_YXZ));
	SceneObject box3(scene, tower, Vec3f(0.0f, 5.0f, -20.0f), Quaternionf(0.0f, 0.0f, 0.0f, angle_degrees, order_YXZ));
	SceneObject tower[10];*/

	SceneObject theGate(scene, gate, Vec3f(-75.0f,5.0f,60.0f), Quaternionf(0.0f,0.0f,0.0f,angle_degrees,order_YXZ));
	theGate.set_scale(Vec3f(1.0f,2.0f,2.0f));

	std::vector<SceneLight> omni_lights;
	for (int i = 0; i < 4; i++)
	{
		SceneLight omni(scene);
		omni.set_type(SceneLight::type_omni);
		omni.set_color(Vec3f(0.05f));
		omni.set_position(Quaternionf(45.0f, 45.0f + i * 90.0f, 0.0f, angle_degrees, order_YXZ).rotate_vector(Vec3f(0.0f, 0.0f, -100.0f)));
		omni.set_attenuation_end(200.0f);
		omni.set_ambient_illumination(0.025f);

		omni_lights.push_back(omni);
	}

	SceneLight spot(scene);
    spot.set_type(SceneLight::type_spot);
    spot.set_orientation(Quaternionf(45.0f, 45.0f, 0.0f, angle_degrees, order_YXZ));
    spot.set_position(spot.get_orientation().rotate_vector(Vec3f(-55.0f, 90.0f, -550.0f)));
	//spot.set_position(Vec3f(-250, 100, -430));
    spot.set_color(Vec3f(1.0f, 1.0f, 0.8f));
    spot.set_falloff(45.0f);
    spot.set_hotspot(15.0f);
    spot.set_attenuation_start(150.0f);
    spot.set_attenuation_end(1500.0f);
    spot.set_shadow_caster(true);
    spot.set_rectangle_shape(false);
	//spot.set_ambient_illumination(true);
    spot.set_aspect_ratio(1.0f);

	/*SceneLight spot(scene);
    spot.set_type(SceneLight::type_spot);
    spot.set_orientation(Quaternionf(30.0f, 30.0f, 0.0f, angle_degrees, order_YXZ));
    spot.set_position(spot.get_orientation().rotate_vector(Vec3f(0.0f, 0.0f, -100.0f)));
    spot.set_color(Vec3f(1.0f, 1.0f, 0.8f));
    spot.set_falloff(45.0f);
    spot.set_hotspot(15.0f);
    spot.set_attenuation_start(20.0f);
    spot.set_attenuation_end(200.0f);
    spot.set_shadow_caster(true);
    spot.set_rectangle_shape(false);
    spot.set_aspect_ratio(1.0f);*/

    /*SceneLight spot2(scene);
    spot2.set_type(SceneLight::type_spot);
    spot2.set_position(Vec3f(0.0f, 100.0f, 0.0f));
    spot2.set_color(Vec3f(1.0f, 1.0f, 1.0f) * 3.0f);
    spot2.set_falloff(35.0f);
    spot2.set_hotspot(30.0f);
    spot2.set_attenuation_start(20.0f);
    spot2.set_attenuation_end(130.0f);
    spot2.set_shadow_caster(true);
    spot2.set_rectangle_shape(false);
    spot2.set_ambient_illumination(0.025f);*/

	wm.NodeArray[0].setPos(75.0f,1.25f,0.0f);
	wm.NodeArray[1].setPos(-60.0f,1.25f,0.0f);
	wm.NodeArray[2].setPos(-55.0f,1.25f,-65.0f);
	wm.NodeArray[3].setPos(60.0f,1.25f,-60.0f);
	wm.NodeArray[4].setPos(55.0f,1.25f,65.0f);
	wm.NodeArray[5].setPos(-80.0f,1.25f,60.0f);
	
	Physics3DShape box_shape = Physics3DShape::box(Vec3f(5.0f));
	Physics3DShape plane_shape = Physics3DShape::box(Vec3f(75.0f, 1.0f, 75.0f));
	Physics3DShape sphere_shape = Physics3DShape::sphere(2.0f);

	Physics3DObject phys_plane(physics_world, plane_shape, Vec3f(0.0f, -0.5f, 0.0f));

	/*Physics3DObject phys_box0(physics_world, box_shape, Vec3f(20.0f, 5.0f, 0.0f), box0.get_orientation());
	Physics3DObject phys_box1(physics_world, box_shape, Vec3f(-20.0f, 5.0f, 0.0f), box1.get_orientation());
	Physics3DObject phys_box2(physics_world, box_shape, Vec3f(0.0f, 5.0f, 20.0f), box2.get_orientation());
	Physics3DObject phys_box3(physics_world, box_shape, Vec3f(0.0f, 5.0f, -20.0f), box3.get_orientation());
	Physics3DObject phys_box[10];*/

	Physics3DSweepTest sweep_test(physics_world);

	Physics3DRayTest raycast(physics_world);

	Slot slot_keyboard_key_down	= (window.get_ic().get_keyboard()).sig_key_down()	.connect(this,&game::on_key_down);
	Slot slot_keyboard_key_up	= (window.get_ic().get_keyboard()).sig_key_up()		.connect(this,&game::on_key_up);
	Slot slot_mouse_moved		= (window.get_ic().get_mouse()).sig_pointer_move()	.connect(this,&game::on_pointer_move);
	Slot slot_mouse_down			= (window.get_ic().get_mouse()).sig_key_down()		.connect(this,&game::on_pointer_down);
	Slot slot_mouse_up			= (window.get_ic().get_mouse()).sig_key_up()		.connect(this,&game::on_pointer_up);

	//________________________________________________________________
	//											           S O U N D S

	total_channels=3;
	current_channel=1;
	SoundBuffer music = SoundBuffer::resource("Music1",sound_resources);
	music.set_volume(0.3f);

	sound_session1.play();
	sound_session2.play();
	sound_session3.play();

	total_samples = 6;
	samples.resize(total_samples);
	samples[0] = SoundBuffer::resource("Explosion1",sound_resources);
	samples[1] = SoundBuffer::resource("Explosion2",sound_resources);
	samples[2] = SoundBuffer::resource("Hurt1",sound_resources);
	samples[3] = SoundBuffer::resource("Hurt2",sound_resources);
	samples[4] = SoundBuffer::resource("Powerup1",sound_resources);
	samples[5] = SoundBuffer::resource("Shoot1",sound_resources);
	
	for(int i = 0; i<total_samples; i++)
	{
		samples[i].set_volume(0.3f);
	}
	
	SoundBuffer_Session music_session = music.prepare();
	music_session_ = &music_session;

	music_session.set_looping(true);
	music_session.play();
	is_music_muted = false;

	/********Enemies(Jasper)************************/
	SceneObject enemyobj[30];
	wm.spawnCreeps();
	for(int i = 0; i < 30; i ++)
	{
		enemyobj[i] = SceneObject(scene, box);
		enemyobj[i].set_scale(Vec3f(0.25f));
		wm.enemyArray[i].setEnemyObject(&enemyobj[i]);
	}
	/***************************************/

	ElapsedTimer elapsed_timer;

	while (!quit)
	{
		float time_elapsed = elapsed_timer.seconds_elapsed();

		/*spot_dir = std::fmod(spot_dir + time_elapsed * 30.0f, 90.0f);
		aspect_time = std::fmod(aspect_time + time_elapsed * 0.2f, 2.0f);

		spot2.set_aspect_ratio(clamp(aspect_time >= 1.0f ? 2.0f - aspect_time : aspect_time, 0.1f, 1.0f));
		spot2.set_orientation(Quaternionf(65.0f + (spot_dir >= 45.0f ? 90.0f - spot_dir : spot_dir), 60.0f, dir * 4.0f, angle_degrees, order_YXZ));
		*/
		// Draw with the canvas:
		/*canvas.clear(Colorf::cadetblue);
		canvas.draw_line(0, 110, 640, 110, Colorf::yellow);
		font.draw_text(canvas, 100, 100, "Hello World!", Colorf::lightseagreen);
		// Draw any remaining queued-up drawing commands oQAn canvas:
		canvas.flush();*/

		if (!endgame)
		{
			if (mouse.get_x() <= 0 && !cameraRestricted)
			{
				mouse.set_position(0, mouse.get_y());
				camera.set_position(camera.get_position()+Vec3f(-mouse_move_speed, 0.0f, 0.0f));
				/*canvas.clear();
				font.draw_text(canvas, 100, 100, "x=0", Colorf::lightseagreen);
				canvas.flush();*/
				//window.flip();
			}
			if (mouse.get_y() <= 0 && !cameraRestricted)
			{
				mouse.set_position(mouse.get_x(), 0);
				camera.set_position(camera.get_position()+Vec3f(0.0f, 0.0f, mouse_move_speed));
				/*canvas.clear();
				font.draw_text(canvas, 100, 100, "y=0", Colorf::lightseagreen);
				canvas.flush();*/
				//window.flip();
			}
			if (mouse.get_x() >= window.get_gc().get_width()-1 && !cameraRestricted)
			{
				mouse.set_position(window.get_gc().get_width()-1, mouse.get_y());
				camera.set_position(camera.get_position()+Vec3f(mouse_move_speed, 0.0f, 0.0f));
				/*canvas.clear();
				font.draw_text(canvas, 100, 100, "x=windowRight", Colorf::lightseagreen);
				canvas.flush();*/
				//window.flip();
			}
			if (mouse.get_y() >= window.get_gc().get_height()-1 && !cameraRestricted)
			{
				mouse.set_position(mouse.get_x(), window.get_gc().get_height()-1);
				camera.set_position(camera.get_position()+Vec3f(0.0f, 0.0f, -mouse_move_speed));
				/*canvas.clear();
				font.draw_text(canvas, 100, 100, "y=windowBottom", Colorf::lightseagreen);
				canvas.flush();*/
				//window.flip();
			}

			if (mouseUp)
			{
				if (selected_tower == "Tower 1")
				{
					towr.set_type(towr.t_bullet);
					cost = 10;
				}
				else if (selected_tower == "Tower 2")
				{
					towr.set_type(towr.t_energy);
					cost = 15;
				}
				else if (selected_tower == "Tower 3")
				{
					towr.set_type(towr.t_rocket);
					cost = 20;
				}

				scene.unproject(mouse_pos, start, end);
				end *= 151;
				test = start + end;

				if (raycast.test(start, test))
				{
					if (towerCount < 10 && money >= cost)
					{
						towr.set_pos(Vec3f(raycast.get_hit_position().x, 5.0f, raycast.get_hit_position().z));
						towr.create(scene, tower, physics_world);

						money -= cost;
						towerCount++;
						cost = 0;
					}
				}
				mouseUp = false;
				//float x = mouse.get_x() - scene.world_to_projection().get_origin_x();
				//tower[0] = SceneObject(scene, box, Vec3f(camera.get_position().x, 5.0f, camera.get_position().z));
				//tower[0].set_position(Vec3f(0.0f, 5.0f, 0.0f));
				//window.flip();
				//canvas.clear(Colorf::cadetblue);
				/*canvas.draw_line(0, 110, 640, 110, Colorf::yellow);
				font.draw_text(canvas, 100, 100, "Hello World!", Colorf::lightseagreen);
				// Draw any remaining queued-up drawing commands oQAn canvas:
				canvas.flush();
				// Present the frame buffer content to the user:
				window.flip();*/
			}

			if (mouse.get_keycode(0))
			{
				
				//maingui.process_messages(0);
				//test.update();
				//show_radial_menu(mouse.get_position());
			}

			/*if (mouse.get_keycode(1))
			{
				scene.unproject(mouse.get_position(), start, end);
				end *= 151;
				Vec3f test = start + end;
				if (raycast.test(start, test))
				{
					Physics3DObject hit = raycast.get_hit_object();
					font.draw_text(canvas, 400, 100, "hit", Colorf::lightseagreen);
					//hit.set_position(Vec3f(0.0f, -10.0f, 0.0f));
					std::stringstream ss3 (std::stringstream::in | std::stringstream::out);
					ss3 << hit.get_position().y;
					txt3 = ss3.str();

					font.draw_text(canvas, 550, 100, txt3, Colorf::lightseagreen);
				}
				
				//projectile.set_pos(Vec3f(raycast.get_hit_position().x, 5.0f, raycast.get_hit_position().z));
				//projectile.fire(scene, box, physics_world);
			
				//canvas.clear(Colorf::white);
				//window.flip();
			}*/
			if (keyboard.get_keycode(keycode_control))
			{
				if (window.is_fullscreen())
				{
					//innerwindow.set_fullscreen(false);
					window.set_size(640, 480, true);
					window.restore();
				}
				else
					window.set_size(640, 480, false);
			}

			if (camera.get_position().x <= left)
			{
				camera.set_position(Vec3f(left, camera.get_position().y, camera.get_position().z));
			}
		
			if (camera.get_position().x >= right)
			{
				camera.set_position(Vec3f(right, camera.get_position().y, camera.get_position().z));
			}
		
			if (camera.get_position().z >= top)
			{
				camera.set_position(Vec3f(camera.get_position().x, camera.get_position().y, top));
			}

			if (camera.get_position().z <= down)
			{
				camera.set_position(Vec3f(camera.get_position().x, camera.get_position().y, down));
			}

			/*if(objects_for_deletion.size()>0)
			{
				std::list<Gameobject *>::iterator it;
				for(it=objects_for_deletion.begin(); it!= objects_for_deletion.end(); ++it)
				{
					delete (*it);
					//towerCount--;
				}

				objects_for_deletion.clear();
			}*/

			if(towerObjects.size()>0)
			{
				std::vector<Gameobject *>::iterator it;
				int counter = 0;
				for(it=towerObjects.begin(); it!= towerObjects.end(); ++it)
				{
					towerObjects.at(counter)->update(time_elapsed, wm.enemyArray, 30);
					counter++;
				}
			}

			if(missileObjects.size()>0)
			{
				std::list<Gameobject *>::iterator it;
				for(it=missileObjects.begin(); it!= missileObjects.end();)
				{
					//if (!(*it)->checkActive())
					(*it)->update(time_elapsed, wm.enemyArray);

					if ((*it)->checkActive())
					{
						delete(*it);
						it = missileObjects.erase(it);						
					}
					else
						it++;
				}
			}

			/*******Enemies(Updates)************************/
			wm.Update(1);
			//time_lasttime = time_elapsed;

			if(wm.life > 0)
			{
				// Put a screen here
				life -= wm.life;
				wm.life = 0;
			}

			if (wm.currLevel == 3)
			{
				endgame = true;
			}
			/***********************************************/

			//scene.update(gc, time_elapsed);

			txt3 = ":";
			ss3.str("");
			ss3.clear();
			ss3 << money;
			txt3 += ss3.str();

			font.draw_text(canvas, 550, 50, txt3, Colorf::lightseagreen);

			if (life < 0)
			{
				endgame = true;
			}

			scene.render(gc);

			/*canvas.clear(clan::Colorf(0.0f,0.0f,0.0f));
			std::stringstream ss (std::stringstream::in | std::stringstream::out);
			ss << game_time.get_time_elapsed();
			std::string troll = ss.str();
			font.draw_text(canvas, 100, 100, troll, Colorf::lightseagreen);*/
			//canvas.clear(clan::Colorf(0.0f,0.0f,0.0f));
			std::stringstream ss (std::stringstream::in | std::stringstream::out);
			ss << scene.world_to_projection().get_origin_x();
			txt = ss.str();

			txt2 = ":";
			ss2.str("");
			ss2.clear();
			ss2 << life;
			txt2 += ss2.str();

			font.draw_text(canvas, 150, 50, txt2, Colorf::lightseagreen);

			//maingui.process_messages(0);

			/*font.draw_text(canvas, 0, 100, main, Colorf::lightseagreen);
			font.draw_text(canvas, 150, 100, txt, Colorf::lightseagreen);
			font.draw_text(canvas, 250, 100, txt2, Colorf::lightseagreen);
			canvas.draw_line(0, 110, 640, 110, Colorf::yellow);*/

			maingui.render_windows();
			
			image_hp.draw(canvas, 100, 45); 
			image_coin.draw(canvas, 500, 45);
			
			window.flip();

			// Read messages from the windowing system message queue, if any are available:
			KeepAlive::process();
		}

		else
		{
			//maingui.render_windows();
			canvas.clear(Colorf::black);

			font.draw_text(canvas, 450, 400, txt4, Colorf::lightseagreen);

			window.flip();

			// Read messages from the windowing system message queue, if any are available:
			KeepAlive::process();
		}
	}
}