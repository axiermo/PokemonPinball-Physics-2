#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Physics.h"
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
	ball_point = { 475,600 };

	map = (App->tex->Load("maps/sprites.png"));
	SDL_Texture* pokeball = App->tex->Load("maps/PokeBall_std.png");
	overlay2 = new element(map, 554, 32, 500, 450, 20, 10);
	overlay = new element(map, 24, 32, 500, 500, 0, 10);
	
	ball.texture = pokeball;
	ball.box = { 0,0,24,24 };

	cyndaquilcave = new element(map, 375, 1736, 82, 72, 135, 220);
	background = new element(map,1059 , 32, 500, 829, 0, 1);
	egg = new element(map, 655, 2837, 32, 38, 158, 194);
	
	//-------BUMPERS------
	chinchou_bumper_1 = new element();
	
	chinchou_bumper_1->animation.PushBack({ 349,1544,51,41 });
	chinchou_bumper_1->animation.PushBack({ 402,1540,47,45 });
	chinchou_bumper_1->animation.speed = 0.0005;

	chinchou_bumper_2 = new element();

	chinchou_bumper_2->animation.PushBack({ 349,1544,51,41 });
	chinchou_bumper_2->animation.PushBack({ 402,1540,47,45 });
	chinchou_bumper_2->animation.speed = 0.0006;

	chinchou_bumper_3 = new element();

	chinchou_bumper_3->animation.PushBack({ 349,1544,51,41 });
	chinchou_bumper_3->animation.PushBack({ 402,1540,47,45 });
	chinchou_bumper_3->animation.speed = 0.0004;

	cyndaquil.PushBack({ 37,1731,66,76 });
	cyndaquil.PushBack({ 103,1731,66,76 });
	cyndaquil.PushBack({ 169,1731,70,76 });
	cyndaquil.PushBack({ 235,1731,70,76 });
	cyndaquil.speed = 0.0025f;

	chikorita.PushBack({ 397,2052,49,88 });
	chikorita.PushBack({ 452,2052,49,88 });
	chikorita.speed = 0.0015f;

	makuhita.PushBack({ 690,1963,56,68 });
	makuhita.PushBack({ 749,1963,62,66 });
	makuhita.PushBack({ 749,1963,62,66 });
	makuhita.speed = 0.002f;

	sharpedo.PushBack({ 300,1851,71,80 });
	sharpedo.PushBack({ 375,1851,72,80 });
	sharpedo.speed = 0.0025f;

	spoink.PushBack({ 23,1609,40,100 });
	spoink.PushBack({ 66,1609,40,100 });
	spoink.speed = 0.0025f;


	return true;
}

void j1Map::DrawChainsBoard() 
{
	//ball
	//ball = new element(App->tex->Load("maps/PokeBall_std.png"), 0, 0, 36, 36, ball_point.x, ball_point.y);
	ball.physbody = App->physics->CreateCircle(ball_point.x, ball_point.y, 11, b2BodyType::b2_dynamicBody,0.33,0x0002,0x0001);
	ball.physbody->listener = App->physics;
	ball.position = { ball_point.x,ball_point.y };
	//the 0x0000 things: 0x0001 es el background, el 2 es la pilota i el 3 son esl flippers i els launchers,
	//el primer terme es el que identifica el objecte que crearà i lo segon es amb quin tipus de bodys colisiona
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
	PhysBody* background = App->physics->CreateChain(0, 0, board, 176, b2BodyType::b2_staticBody,0x0001,0x0002);

	int Right_triangle[20] = {
		300, 698,
		328, 655,
		332, 649,
		336, 647,
		340, 648,
		341, 651,
		341, 690,
		305, 712,
		299, 709,
		298, 704
	};
	PhysBody* rightriangle = App->physics->CreateChain(0, 0, Right_triangle, 20, b2BodyType::b2_kinematicBody, 0x0001, 0x0002);
	int L_right[20] = {
		308, 750,
		321, 769,
		379, 732,
		384, 726,
		384, 662,
		370, 662,
		370, 701,
		368, 710,
		360, 716,
		324, 739
	};
	PhysBody* LRight = App->physics->CreateChain(0, 0, L_right, 20, b2BodyType::b2_staticBody, 0x0001, 0x0002);
	
	int triangle_left[20] = {
		127, 691,
		127, 654,
		128, 649,
		132, 647,
		135, 649,
		170, 701,
		171, 706,
		169, 710,
		163, 712,
		136, 696
	};
	PhysBody* Triangleleft = App->physics->CreateChain(0, 0, triangle_left, 20, b2BodyType::b2_kinematicBody, 0x0001, 0x0002);
	int L_left[20] = {
		152, 773,
		163, 752,
		106, 717,
		99, 710,
		95, 701,
		95, 662,
		82, 662,
		82, 724,
		86, 730,
		102, 741
	};
	PhysBody* LLeft = App->physics->CreateChain(0, 0, L_left, 20, b2BodyType::b2_staticBody, 0x0001, 0x0002);

	int SharkpedoWalls[68] = {
		323, 189,
		323, 227,
		344, 237,
		359, 249,
		360, 284,
		329, 349,
		300, 400,
		300, 417,
		315, 426,
		335, 389,
		347, 357,
		355, 340,
		374, 345,
		374, 371,
		338, 440,
		338, 469,
		347, 462,
		358, 432,
		368, 413,
		376, 402,
		389, 402,
		394, 409,
		372, 478,
		372, 504,
		389, 472,
		400, 448,
		408, 412,
		411, 391,
		412, 352,
		407, 315,
		390, 263,
		368, 229,
		354, 211,
		334, 196
	};
	PhysBody* SharkWalls = App->physics->CreateChain(0, 0, SharkpedoWalls, 68, b2BodyType::b2_staticBody, 0x0001, 0x0002);
	
	int CyndaquilCaveWalls[56] = {
		144, 361,
		138, 337,
		136, 285,
		136, 255,
		153, 212,
		177, 187,
		197, 175,
		215, 170,
		214, 221,
		209, 228,
		209, 273,
		213, 295,
		219, 314,
		228, 335,
		234, 345,
		233, 361,
		206, 362,
		197, 355,
		201, 285,
		201, 267,
		150, 267,
		148, 299,
		150, 317,
		153, 336,
		159, 352,
		160, 378,
		155, 383,
		148, 377
	};
	PhysBody* CyndaWalls = App->physics->CreateChain(0, 0, CyndaquilCaveWalls, 56, b2BodyType::b2_staticBody, 0x0001, 0x0002);
	
	int WallOverBridge[26] = {
		63, 312,
		72, 269,
		85, 244,
		97, 225,
		112, 210,
		113, 231,
		105, 247,
		100, 264,
		97, 283,
		96, 316,
		97, 336,
		102, 371,
		84, 346
	};
	PhysBody* WallOverBridg = App->physics->CreateChain(0, 0, WallOverBridge, 26, b2BodyType::b2_staticBody, 0x0001, 0x0002);
	
	int WallUnderBridge[22] = {
		61, 400,
		66, 428,
		76, 457,
		88, 485,
		103, 512,
		131, 547,
		153, 529,
		153, 507,
		140, 487,
		128, 460,
		119, 441
	};
	PhysBody* WallUnderBridg = App->physics->CreateChain(0, 0, WallUnderBridge, 22, b2BodyType::b2_staticBody, 0x0001, 0x0002);
	
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

	//--------BUMPERS---------------

	chinchou_bumper_1->physbody = App->physics->CreateCircle(250, 261,20, b2BodyType::b2_kinematicBody,0.8 , 0x0003, 0x0001);
	
	chinchou_bumper_2->physbody = App->physics->CreateCircle(316, 267,20, b2BodyType::b2_kinematicBody,0.8 ,0x0003, 0x0001);
	
	chinchou_bumper_3->physbody = App->physics->CreateCircle(283, 325,20, b2BodyType::b2_kinematicBody,0.8 ,0x0003, 0x0001);	
	/*big_bumper1->pb = App->physics->CreateCircle(370, 576, 40, 0.7f, 0x0003, 0x0002); 
	big_bumper1->pb->body->SetType(b2_kinematicBody);
	big_bumper1->pb->coll_name = collider_names::big_bumper_1;
	big_bumper2->pb = App->physics->CreateCircle(527, 440, 40, 0.7f, 0x0003, 0x0002); big_bumper2->pb->body->SetType(b2_kinematicBody);
	big_
*/


	//-------FLIPPERS & LAUNCHERS--------
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
	int right_flipper[10] = {
		192, 769,
		216, 786,
		215, 793,
		208, 794,
		182, 784
	};


	iPoint left_flipper_pos = {170,763};
	PhysBody* Ball_l_A = App->physics->CreateCircle(left_flipper_pos.x, left_flipper_pos.y, 10, b2BodyType::b2_staticBody,0.0,0x0000,0x0000);
	PhysBody* Chain_l_B = App->physics->CreateRectangle(left_flipper_pos.x, left_flipper_pos.y, 58, 15, b2BodyType::b2_dynamicBody,0x0001,0x0002);

	l_flipper_joint = App->physics->CreateRevoluteJoint(Ball_l_A, Chain_l_B, -24.0f, 0.0f, 30,-15, 300, 0);

	//l_flipper_joint = App->physics->CreateRevoluteJoint(10, left_flipper, 22, 169, 765, 10, 10, 200, 150, 10, -90);
	
	iPoint right_flipper_pos = {305,763};
	PhysBody* Ball_r_A = App->physics->CreateCircle(right_flipper_pos.x, right_flipper_pos.y,10,b2BodyType::b2_staticBody,0.0, 0x0000, 0x0000);
	PhysBody* Chain_r_B = App->physics->CreateRectangle(right_flipper_pos.x , right_flipper_pos.y, 58, 15, b2BodyType::b2_dynamicBody, 0x0001, 0x0002);
	//PhysBody* Chain_r_B = App->physics->CreateChain(right_flipper_pos.x, right_flipper_pos.y, right_flipper, 10, b2BodyType::b2_dynamicBody);

	r_flipper_joint = App->physics->CreateRevoluteJoint(Ball_r_A, Chain_r_B,24.0f, 0.0f, 15, -30, 300, 0);
		
	//ball launcher
	iPoint ball_launcher_pos = { 475,810 };
	PhysBody* Launcher_A = App->physics->CreateRectangle(ball_launcher_pos.x, ball_launcher_pos.y, 10, 20, b2BodyType::b2_staticBody, 0x0001, 0x0001);
	PhysBody* Launcher_B = App->physics->CreateRectangle(ball_launcher_pos.x, ball_launcher_pos.y, 20, 40, b2BodyType::b2_dynamicBody, 0x0001, 0x0001);

	ball_launcher_joint = App->physics->CreatePrismaticJoint(Launcher_A, Launcher_B, b2Vec2(1,-10), b2Vec2(1, 30), -20, -5, 500, 0);

}

void j1Map::Draw()
{	
	ball.physbody->GetPosition(ball.position.x, ball.position.y);

	
	App->render->Blit(background->texture, background->position.x, background->position.y, &background->box);
	App->render->Blit(overlay2->texture, overlay2->position.x, overlay2->position.y, &overlay2->box);
	App->render->Blit(overlay->texture, overlay->position.x, overlay->position.y, &overlay->box);
	App->render->Blit(cyndaquilcave->texture, cyndaquilcave->position.x, cyndaquilcave->position.y, &cyndaquilcave->box);

	App->render->Blit(egg->texture, egg->position.x, egg->position.y, &egg->box);

	
	App->render->Blit(map, 259, 304, &chinchou1.GetCurrentFrame(), -0.1f);
	App->render->Blit(map, 223, 239, &chinchou2.GetCurrentFrame(), -0.1f);
	App->render->Blit(map, 293, 247, &chinchou3.GetCurrentFrame(), -0.1f);
	App->render->Blit(map, 134, 272, &cyndaquil.GetCurrentFrame(), -0.1f);
	App->render->Blit(map, 105, 421, &chikorita.GetCurrentFrame(), -0.1f);
	App->render->Blit(map, 375, 537, &makuhita.GetCurrentFrame(), -0.1f);
	App->render->Blit(map, 353, 332, &sharpedo.GetCurrentFrame(), -0.1f);
	App->render->Blit(map, 456, 724, &spoink.GetCurrentFrame(), -0.1f);
	App->render->Blit(ball.texture, ball.position.x, ball.position.y, &ball.box, 5.0f, ball.physbody->GetRotation(), ball.physbody->body->GetLocalCenter().x+11, ball.physbody->body->GetLocalCenter().y+11);

}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	return true;
}

void j1Map::NewBall()
{
	ball.position = { 475,600 };
	ball.physbody  = App->physics->CreateCircle(ball.position.x, ball.position.y, 11, b2BodyType::b2_dynamicBody,0.2,0x0002,0x0001);
	ball.physbody->listener = App->physics;
}

