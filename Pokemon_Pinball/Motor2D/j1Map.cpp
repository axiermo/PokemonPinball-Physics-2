#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module()
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
}

bool j1Map::Start()
{
	SDL_Texture* map = (App->tex->Load("maps/sprites.png"));
	overlay = new element(map, 24, 32, 500, 500, 0, 10);
	overlay2 = new element(map, 552, 32, 500, 450, 22, 10);
	cyndaquilcave = new element(map, 375, 1736, 82, 72, 135, 220);
	background = new element(map,1058 , 32, 500, 829, 1, 1);
	ball = new element(App->tex->Load("maps/PokeBall_std.png"), 0, 0, 36, 36,450,750);

	//name of the variable = App->tex->Load()
	//ASIER TODO
	//here we will load every sprite

	return true;
}

void j1Map::DrawChainsBoard() 
{
	//ball
	ball->physbody = App->physics->CreateCircle(400, 200, 11, b2BodyType::b2_dynamicBody);
	ball->physbody->body->SetBullet(1);
	balls.add(ball->physbody);
	
	//walls
	int board[176] = {
		0, 829,
		195, 829,
		195, 820,
		152, 796,
		105, 768,
		76, 748,
		76, 775,
		46, 775,
		46, 650,
		48, 643,
		51, 638,
		55, 634,
		60, 632,
		81, 632,
		88, 630,
		91, 626,
		93, 622,
		93, 558,
		77, 541,
		65, 523,
		53, 502,
		42, 481,
		32, 454,
		26, 426,
		23, 406,
		23, 338,
		29, 304,
		38, 273,
		52, 241,
		67, 218,
		94, 190,
		123, 171,
		151, 158,
		185, 148,
		212, 144,
		260, 144,
		296, 150,
		330, 161,
		363, 179,
		388, 196,
		409, 211,
		445, 242,
		462, 266,
		475, 304,
		483, 330,
		485, 342,
		488, 370,
		488, 798,
		461, 798,
		461, 408,
		460, 385,
		458, 362,
		454, 329,
		448, 303,
		439, 280,
		432, 269,
		427, 266,
		427, 284,
		432, 297,
		437, 317,
		442, 340,
		444, 357,
		444, 403,
		441, 430,
		436, 456,
		424, 484,
		410, 513,
		395, 539,
		374, 558,
		374, 619,
		376, 625,
		381, 629,
		390, 632,
		407, 632,
		413, 635,
		418, 640,
		420, 646,
		421, 654,
		421, 775,
		393, 775,
		393, 750,
		354, 776,
		318, 797,
		275, 819,
		275, 829,
		500, 829,
		500, 0,
		0, 0

	};
	PhysBody* background = App->physics->CreateChain(0, 0, board, 176, b2BodyType::b2_staticBody);
	int L_left[24] = {
		195, 902,
		100, 842,
		96, 834,
		96, 751,
		101, 744,
		108, 744,
		112, 750,
		112, 806,
		112, 813,
		116, 817,
		208, 876,
		208, 890
	};

	int L_right[24]{
		347, 901,
		438, 841,
		443, 834,
		442, 748,
		432, 746,
		429, 753,
		428, 812,
		422, 823,
		408, 832,
		334, 880,
		333, 887,
		338, 894

	};

	int triangle_right[18] = {
		342, 822,
		343, 808,
		381, 750,
		388, 745,
		393, 752,
		392, 792,
		388, 796,
		381, 801,
		352, 820
	};
	
	int triangle_left[16] = {
		148, 793,
		146, 753,
		150, 745,
		155, 747,
		196, 807,
		194, 817,
		190, 822,
		157, 800
	};

	int topwall[38] = {
		120, 446,
		98, 424,
		79, 394,
		71, 367,
		78, 334,
		86, 305,
		102, 272,
		115, 252,
		131, 240,
		131, 267,
		126, 273,
		121, 285,
		116, 298,
		112, 316,
		110, 330,
		110, 363,
		112, 389,
		115, 407,
		120, 429
	};
	
	int balls_warehouse[30] = {
		220, 956,
		156, 997,
		150, 1052,
		182, 1108,
		234, 1141,
		310, 1143,
		363, 1140,
		385, 1086,
		384, 1021,
		363, 972,
		325, 959,
		408, 962,
		430, 1167,
		139, 1159,
		123, 954
	};

	//flippers
	int left_flipper[22] = {
		169, 783,
		161, 780,
		158, 775,
		158, 762,
		163, 757,
		172, 757,
		214, 786,
		215, 791,
		211, 797,
		204, 797,
		183, 789
	};
	l_flipper_joint = App->physics->CreateRevoluteJoint(15, left_flipper, 22, 0, 0, 100, 100, 200, 150, 250, -90);
	
	int right_flipper[22] = {
		305, 896,
		289, 909,
		288, 914,
		294, 920,
		301, 921,
		353, 900,
		356, 889,
		354, 879,
		351, 874,
		338, 872,
		315, 888
	};
	l_flipper_joint = App->physics->CreateRevoluteJoint(15, right_flipper, 22, 0, 0, 100, 100, 200, 150, 250, -90);

	//ball launcher
	int pos_x = 450; int pos_y = 800;
	PhysBody* A = App->physics->CreateRectangle(pos_x, pos_y, 10, 100, b2BodyType::b2_staticBody);
	PhysBody* B = App->physics->CreateRectangle(pos_x, pos_y-30, 50, 70, b2BodyType::b2_kinematicBody);
	ball_launcher_joint = App->physics->CreatePrismaticJoint(A, B, b2Vec2(1, 10), b2Vec2(1, -10), -40, -120, 248, 200);

}

void j1Map::Draw()
{
	//App->render->Blit() utilitzar aquesta funció per totes les textures
	App->render->Blit(background->texture, background->position.x, background->position.y, &background->box);
	App->render->Blit(overlay->texture, overlay->position.x, overlay->position.y, &overlay->box);
	App->render->Blit(overlay2->texture, overlay2->position.x, overlay2->position.y, &overlay2->box);
	App->render->Blit(cyndaquilcave->texture, cyndaquilcave->position.x, cyndaquilcave->position.y, &cyndaquilcave->box);
	App->render->Blit(ball->texture, ball->position.x, ball->position.y, &ball->box);
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	return true;
}

