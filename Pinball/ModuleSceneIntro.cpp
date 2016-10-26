#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleWindow.h"
#include "p2String.h"
ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

bool ModuleSceneIntro::Start()
{

	App->scene_intro->DrawChainsBoard();
	App->audio->PlayMusic("music/SoundTrack.ogg");
	
	
	LOG("Loading Intro assets");

	//ball_point = { 475,600 };
	top = false;

	map = (App->textures->Load("sprites/sprites.png"));
	SDL_Texture* pokeball = App->textures->Load("sprites/PokeBall_std.png");
	SDL_Texture* LFlipper = App->textures->Load("sprites/left_flipper.png");
	SDL_Texture* RFlipper = App->textures->Load("sprites/right_flipper.png");
	overlay2 = new element(map, 554, 32, 500, 450, 20, 10);
	overlay = new element(map, 24, 32, 500, 500, 0, 10);

	ball.texture = pokeball;
	ball.box = { 0,0,24,24 };
	left_flipper.texture = LFlipper;
	left_flipper.box = { 0,0,61,23 };
	right_flipper.texture = RFlipper;
	right_flipper.box = { 0,0,61,23 };

	cyndaquilcave = new element(map, 375, 1736, 82, 72, 135, 220);
	background.texture=map;
	background.box = { 1059, 32, 500, 829 };
	background.position = { 0,1 };

	egg = new element(map, 655, 2837, 32, 38, 158, 194);

	chinchou1.PushBack({ 349,1544,51,41 });
	chinchou1.PushBack({ 402,1540,47,45 });
	chinchou1.speed = 0.05;

	chinchou2.PushBack({ 349,1544,51,41 });
	chinchou2.PushBack({ 402,1540,47,45 });
	chinchou2.speed = 0.06;

	chinchou3.PushBack({ 349,1544,51,41 });
	chinchou3.PushBack({ 402,1540,47,45 });
	chinchou3.speed = 0.04;

	cyndaquil.PushBack({ 37,1731,66,76 });
	cyndaquil.PushBack({ 103,1731,66,76 });
	cyndaquil.PushBack({ 169,1731,70,76 });
	cyndaquil.PushBack({ 235,1731,70,76 });
	cyndaquil.speed = 0.025f;

	chikorita.PushBack({ 397,2052,49,88 });
	chikorita.PushBack({ 452,2052,49,88 });
	chikorita.speed = 0.015f;

	makuhita.PushBack({ 690,1963,56,68 });
	makuhita.PushBack({ 749,1963,62,66 });
	makuhita.PushBack({ 749,1963,62,66 });
	makuhita.speed = 0.02f;

	sharpedo.PushBack({ 300,1851,71,80 });
	sharpedo.PushBack({ 375,1851,72,80 });
	sharpedo.speed = 0.025f;

	spoink.PushBack({ 23,1609,40,100 });
	spoink.PushBack({ 66,1609,40,100 });
	spoink.speed = 0.025f;
	
	
	return true;
}
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}
update_status ModuleSceneIntro::Update(){
	//Activate the correct chains
	


	TopOrUnder();

	// GAMEPLAY INPUTS
	// Left flippers
	if ((App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) || (App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT))
		l_flipper_joint->SetMotorSpeed(-25);
	else
		l_flipper_joint->SetMotorSpeed(25);

	//// Right flippers
	if ((App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) || (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT))
		r_flipper_joint->SetMotorSpeed(25);
	else
		r_flipper_joint->SetMotorSpeed(-25);

	//// Ball Launcher 
	if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) || (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT))
		ball_launcher_joint->SetMotorSpeed(123.1);
	else
		ball_launcher_joint->SetMotorSpeed(-123.0);


	// Create a new ball
	if ((App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN))
	{
		ball.physbody = App->physics->CreateCircle(475, 600, 11, b2BodyType::b2_dynamicBody);
		ball.position = { 475,600 };
		ball.physbody->listener = this;
	}

	// Draw ----------
	
		Draw();

	// ---------------
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = { x - App->renderer->camera.x, y - App->renderer->camera.y };
	p2SString title("Mouse: %d, x -- %d y", map_coordinates.x, map_coordinates.y);



	App->window->SetTitle(title.GetString());
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::DrawChainsBoard()
{
	//ball
	//ball = new element(App->tex->Load("maps/PokeBall_std.png"), 0, 0, 36, 36, ball_point.x, ball_point.y);
	
	//sensors
	RigthEntrance = App->physics->CreateRectangleSensor(345, 398, 18, 0);
	RigthEntrance->body->SetActive(true);
	LeftEntrance = App->physics->CreateRectangleSensor(45, 321, 25, 0);
	LeftEntrance->body->SetActive(true);
	//walls

	Points_Chicorita = App->physics->CreateRectangleSensor(150, 535, 5, 0);
	Points_Chicorita->body->SetActive(true);
	Points_Cynda = App->physics->CreateRectangleSensor(222, 370, 5, 0);
	Points_Cynda->body->SetActive(true);
	int bumper_rigth[8] = {
		296, 702,
		332, 648,
		331, 647,
		294, 701
	};

	bumperright = App->physics->CreateChain(0, 0, bumper_rigth, 8, b2BodyType::b2_staticBody);
	// Pivot 0, 0
	int bumper_left[8] = {
		134, 647,
		172, 700,
		173, 699,
		134, 646
	};
	bumperleft = App->physics->CreateChain(0, 0, bumper_left, 8, b2BodyType::b2_staticBody);

	
	int Tunel_Interior[84] = {
		103, 388,
		80, 364,
		64, 330,
		55, 292,
		54, 246,
		60, 212,
		76, 180,
		98, 157,
		125, 142,
		152, 129,
		168, 125,
		252, 125,
		282, 136,
		307, 150,
		326, 170,
		341, 190,
		354, 224,
		362, 269,
		362, 305,
		346, 344,
		324, 396,
		313, 410,
		300, 402,
		318, 371,
		357, 292,
		355, 250,
		334, 196,
		306, 162,
		273, 144,
		252, 138,
		157, 138,
		116, 157,
		96, 174,
		79, 191,
		68, 211,
		61, 236,
		59, 262,
		59, 285,
		66, 316,
		78, 345,
		92, 362,
		102, 373
	};
	PhysBody* TunelInterior = App->physics->CreateChain(0, 0, Tunel_Interior, 84, b2BodyType::b2_staticBody);
	TunelInterior->body->SetActive(false);
	int tunel_exterior[118] = {
		114, 436,
		65, 388,
		44, 355,
		32, 324,
		24, 291,
		22, 274,
		22, 237,
		30, 205,
		43, 174,
		61, 152,
		83, 132,
		107, 117,
		132, 107,
		161, 99,
		257, 99,
		288, 110,
		322, 131,
		352, 160,
		371, 188,
		384, 224,
		389, 261,
		401, 238,
		410, 212,
		414, 192,
		448, 192,
		440, 222,
		420, 277,
		395, 328,
		370, 376,
		337, 438,
		337, 447,
		343, 444,
		404, 333,
		427, 278,
		441, 241,
		453, 204,
		453, 184,
		408, 184,
		402, 215,
		392, 242,
		382, 195,
		371, 169,
		359, 151,
		328, 121,
		291, 101,
		261, 89,
		155, 89,
		125, 98,
		95, 112,
		71, 130,
		41, 163,
		25, 202,
		17, 230,
		17, 275,
		21, 305,
		31, 343,
		44, 375,
		61, 399,
		90, 423
	};
	TunelExterior = App->physics->CreateChain(0, 0, tunel_exterior, 118, b2BodyType::b2_staticBody);
	TunelExterior->body->SetActive(false);
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
	background.physbody = App->physics->CreateChain(0, 0, board, 176, b2BodyType::b2_staticBody);

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
	PhysBody* rightriangle = App->physics->CreateChain(0, 0, Right_triangle, 20, b2BodyType::b2_kinematicBody);
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
	PhysBody* LRight = App->physics->CreateChain(0, 0, L_right, 20, b2BodyType::b2_staticBody);

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
	PhysBody* Triangleleft = App->physics->CreateChain(0, 0, triangle_left, 20, b2BodyType::b2_kinematicBody);
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
	PhysBody* LLeft = App->physics->CreateChain(0, 0, L_left, 20, b2BodyType::b2_staticBody);

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
	SharkWalls = App->physics->CreateChain(0, 0, SharkpedoWalls, 68, b2BodyType::b2_staticBody);

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
	PhysBody* CyndaWalls = App->physics->CreateChain(0, 0, CyndaquilCaveWalls, 56, b2BodyType::b2_staticBody);

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
	PhysBody* WallOverBridg = App->physics->CreateChain(0, 0, WallOverBridge, 26, b2BodyType::b2_staticBody);

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
	PhysBody* WallUnderBridg = App->physics->CreateChain(0, 0, WallUnderBridge, 22, b2BodyType::b2_staticBody);

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


	iPoint left_flipper_pos = { 168,768 };
	PhysBody* Ball_l_A = App->physics->CreateCircle(left_flipper_pos.x, left_flipper_pos.y - 5, 8, b2BodyType::b2_staticBody);
	left_flipper.physbody = App->physics->CreateRectangle(left_flipper_pos.x, left_flipper_pos.y - 25, 58, 15, b2BodyType::b2_dynamicBody);

	l_flipper_joint = App->physics->CreateRevoluteJoint(Ball_l_A, left_flipper.physbody, -24.0f, 0.0f, 30, -13, 300, 0);

	//l_flipper_joint = App->physics->CreateRevoluteJoint(10, left_flipper, 22, 169, 765, 10, 10, 200, 150, 10, -90);

	iPoint right_flipper_pos = { 302,768 };
	PhysBody* Ball_r_A = App->physics->CreateCircle(right_flipper_pos.x, right_flipper_pos.y - 5, 8, b2BodyType::b2_staticBody);
	right_flipper.physbody = App->physics->CreateRectangle(right_flipper_pos.x, right_flipper_pos.y - 25, 58, 15, b2BodyType::b2_dynamicBody);
	//PhysBody* Chain_r_B = App->physics->CreateChain(right_flipper_pos.x, right_flipper_pos.y, right_flipper, 10, b2BodyType::b2_dynamicBody);

	r_flipper_joint = App->physics->CreateRevoluteJoint(Ball_r_A, right_flipper.physbody, 24.0f, 0.0f, 13, -30, 300, 0);

	//ball launcher
	iPoint ball_launcher_pos = { 475,810 };
	PhysBody* Launcher_A = App->physics->CreateRectangle(ball_launcher_pos.x, ball_launcher_pos.y, 10, 20, b2BodyType::b2_staticBody);
	PhysBody* Launcher_B = App->physics->CreateRectangle(ball_launcher_pos.x, ball_launcher_pos.y, 20, 40, b2BodyType::b2_dynamicBody);

	ball_launcher_joint = App->physics->CreatePrismaticJoint(Launcher_A, Launcher_B, b2Vec2(1, -10), b2Vec2(1, 30), -20, -5, 500, 0);

}

void ModuleSceneIntro::Draw()
{
	if (ball.physbody != NULL)
	{
		ball.position.x = ball.physbody->body->GetPosition().x;
		ball.position.y = ball.physbody->body->GetPosition().y;
		App->renderer->Blit(ball.texture, ball.position.x, ball.position.y, &ball.box, 50.0f, ball.physbody->GetRotation(), ball.physbody->body->GetLocalCenter().x + 11, ball.physbody->body->GetLocalCenter().y + 11);

	}

	//ball.physbody->body->GetPosition(, ball.position.y);


	App->renderer->Blit(background.texture, background.position.x, background.position.y, &background.box);
	App->renderer->Blit(overlay2->texture, overlay2->position.x, overlay2->position.y, &overlay2->box);
	App->renderer->Blit(overlay->texture, overlay->position.x, overlay->position.y, &overlay->box);
	App->renderer->Blit(cyndaquilcave->texture, cyndaquilcave->position.x, cyndaquilcave->position.y, &cyndaquilcave->box);
			  
	App->renderer->Blit(egg->texture, egg->position.x, egg->position.y, &egg->box);
			   
			   
	App->renderer->Blit(map, 259, 304, &chinchou1.GetCurrentFrame(), -0.1f);
	App->renderer->Blit(map, 223, 239, &chinchou2.GetCurrentFrame(), -0.1f);
	App->renderer->Blit(map, 293, 247, &chinchou3.GetCurrentFrame(), -0.1f);
	App->renderer->Blit(map, 134, 272, &cyndaquil.GetCurrentFrame(), -0.1f);
	App->renderer->Blit(map, 105, 421, &chikorita.GetCurrentFrame(), -0.1f);
	App->renderer->Blit(map, 375, 537, &makuhita.GetCurrentFrame(), -0.1f);
	App->renderer->Blit(map, 353, 332, &sharpedo.GetCurrentFrame(), -0.1f);
	App->renderer->Blit(map, 456, 724, &spoink.GetCurrentFrame(), -0.1f);
	App->renderer->Blit(left_flipper.texture, 165, 750, &left_flipper.box, 50.0f, left_flipper.physbody->GetRotation(), left_flipper.physbody->body->GetLocalCenter().x, left_flipper.physbody->body->GetLocalCenter().y);
	App->renderer->Blit(right_flipper.texture, 295, 744, &right_flipper.box, 50.0f, (r_flipper_joint->GetJointAngle()*RADTODEG) + 180, right_flipper.physbody->body->GetWorldCenter().x, right_flipper.physbody->body->GetWorldCenter().y);

}

void ModuleSceneIntro::NewBall()
{
	ball.position = { 475,600 };
	ball.physbody = ball.physbody = App->physics->CreateCircle(ball.position.x, ball.position.y, 11, b2BodyType::b2_dynamicBody);
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB) {
	if (bodyB == RigthEntrance || bodyB == LeftEntrance)
	{
		top = !top;
	}
	if (bodyB == bumperright) {

		bodyA->body->SetLinearVelocity(b2Vec2(-8, -8));
	}
	if (bodyB == bumperleft) {

		bodyA->body->SetLinearVelocity(b2Vec2(8, -8));
	}
	if (bodyB == Points_Chicorita || bodyB == Points_Cynda)
	{
		score += 500;
	}
}
void ModuleSceneIntro:: TopOrUnder()
{
	if (top)
	{
		//p2List_item<PhysBody*>* items = limits_background.getFirst();
		TunelExterior->body->SetActive(true);
		//if(background.physbody !=nullptr)
		background.physbody->body->SetActive(false);
		SharkWalls->body->SetActive(false);
	}
	else
	{
		TunelExterior->body->SetActive(false);
		//if (background.physbody != nullptr)
		background.physbody->body->SetActive(true);
		SharkWalls->body->SetActive(true);
	}
}






