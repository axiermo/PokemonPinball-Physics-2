#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"
#include "Box2D/Box2D/Box2D.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void DrawChainsBoard();
	void Draw();
	void NewBall();
	void TopOrUnder();

	
public:

	class element {
	public:
		SDL_Texture* texture = nullptr;
		PhysBody* physbody = nullptr;
		SDL_Rect box;
		iPoint position;
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
		~element() {}
	};
	//
	b2RevoluteJoint* l_flipper_joint;
	b2RevoluteJoint* r_flipper_joint;

	// launcher
	b2PrismaticJoint*    ball_launcher_joint;

	PhysBody* RigthEntrance;
	PhysBody* LeftEntrance;
	

	PhysBody* SharkWalls;
	PhysBody* TunelExterior;
	PhysBody* TunelInterior;
	PhysBody* bumperright;
	PhysBody* bumperleft;
	PhysBody* Points_Chicorita;
	PhysBody* Points_Cynda;

	SDL_Texture* map = nullptr;

	element left_flipper;
	element right_flipper;
	element ball;
	element ball_launcher;
	element background;

	Animation cyndaquil;
	Animation chikorita;
	Animation makuhita;
	Animation sharpedo;
	Animation spoink;
	Animation chinchou1;
	Animation chinchou2;
	Animation chinchou3;

	element* egg;
	
	element* overlay;
	element* overlay2;
	element* cyndaquilcave;
	element* triangle_left;
	element* triangle_right;

	iPoint ball_point;
	bool top;
	uint score;
};
