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
	ball = new element(App->tex->Load("maps/PokeBall_std.png"), 0, 0, 36, 36);

	//name of the variable = App->tex->Load()
	//ASIER TODO
	//here we will load every sprite

	return true;
}

void j1Map::DrawChainsBoard() {
	//flippers

	//ball
	ball->physbody = App->physics->CreateCircle(20, 30, 11, b2BodyType::b2_dynamicBody);
	ball->physbody->body->SetBullet(1);
	balls.add(ball->physbody);
	
	//walls
	int board[182] = {
		0, 957,
		225, 957,
		225, 945,
		167, 915,
		125, 890,
		88, 865,
		88, 893,
		54, 893,
		54, 749,
		56, 741,
		60, 734,
		65, 730,
		72, 727,
		91, 727,
		98, 726,
		103, 722,
		107, 716,
		108, 708,
		108, 643,
		88, 623,
		72, 599,
		58, 572,
		45, 544,
		34, 509,
		27, 472,
		26, 458,
		26, 415,
		30, 381,
		37, 338,
		46, 308,
		62, 274,
		80, 247,
		109, 217,
		145, 194,
		185, 176,
		217, 167,
		248, 164,
		301, 164,
		331, 168,
		360, 177,
		389, 188,
		417, 205,
		439, 218,
		457, 230,
		483, 251,
		512, 279,
		525, 297,
		537, 321,
		546, 344,
		553, 366,
		560, 396,
		564, 429,
		564, 920,
		532, 920,
		532, 468,
		528, 418,
		523, 376,
		515, 342,
		501, 313,
		493, 306,
		493, 324,
		502, 353,
		508, 381,
		511, 414,
		512, 462,
		508, 495,
		501, 527,
		490, 556,
		472, 592,
		458, 614,
		448, 628,
		432, 642,
		432, 716,
		434, 721,
		439, 725,
		447, 728,
		468, 728,
		475, 730,
		480, 733,
		484, 739,
		486, 750,
		486, 892,
		452, 892,
		452, 865,
		397, 901,
		349, 927,
		317, 943,
		317, 957,
		577, 957,
		577, 0,
		0, 0

	};
	PhysBody* background = App->physics->CreateChain(0, 0, board, 182, b2BodyType::b2_staticBody);

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
	// Pivot 0, 0
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
	// Pivot 0, 0
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
	int left_flipper[24] = {
		221, 884,
		207, 873,
		196, 873,
		191, 879,
		188, 890,
		192, 899,
		202, 903,
		241, 920,
		250, 921,
		256, 917,
		255, 907,
		230, 890
	};
	
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

