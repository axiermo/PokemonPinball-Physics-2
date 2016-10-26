#ifndef _J1MAP_H
#define _J1MAP_H
#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Physics.h"
#include "j1Scene.h"
#include "Animation.h"

class App;
class j1Map : public j1Module
{
public:
	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool Start();

	// Called each loop iteration
	void DrawChainsBoard();
	void Draw();
	void NewBall();
	// Called before quitting
	bool CleanUp();


public:

	class element {
	public:
		SDL_Texture* texture = nullptr;
		PhysBody* physbody = nullptr;
		SDL_Rect box;
		iPoint position;
		Animation animation;
	public:
		element() {}
		//call a constructure	
		element(SDL_Texture* texture, uint rect_x, uint rect_y, uint width, uint height, uint pos_x, uint pos_y)
		{
			(this)->texture = texture;
			box.x = rect_x;
			box.y = rect_y;
			box.w = width;
			box.h = height;
			position.x = pos_x;
			position.y = pos_y;
		}
	/*	element(SDL_Texture* texture, uint rect_x, uint rect_y, uint width, uint height)
		{
			(this)->texture = texture;
			box.x = rect_x;
			box.y = rect_y;
			box.w = width;
			box.h = height;
		}
		*/
		~element() {}

		

	};
	//
	b2RevoluteJoint* l_flipper_joint;
	b2RevoluteJoint* r_flipper_joint;

	// launcher
	b2PrismaticJoint*    ball_launcher_joint;

	SDL_Texture* map = nullptr;
	element left_flipper;
	element right_flipper;
	element ball;
	element ball_launcher;

	Animation cyndaquil;
	Animation chikorita;
	Animation makuhita;
	Animation sharpedo;
	Animation spoink;
	Animation chinchou1;
	Animation chinchou2;
	Animation chinchou3;

	element* egg;
	element* background;
	element* overlay;
	element* overlay2;
	element* cyndaquilcave;
	element* triangle_left;
	element* triangle_right;
	element* chinchou_bumper_1;
	element* chinchou_bumper_2;
	element* chinchou_bumper_3;

	iPoint ball_point;
};











#endif // !J1MAP_H

