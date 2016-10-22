#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = background = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;
	//loading textures 
	circle = App->textures->Load("pinball/PokeBall_std.png"); //MUST change it for the sprite of our ball
	background = App->textures->Load("pinball/background.png");
													   
	//loading audio
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	chains.add(App->physics->CreateChain(0, 0, board, 182 ,b2BodyType::b2_staticBody));
	chains.add(App->physics->CreateChain(0, 0, L_left, 24, b2BodyType::b2_staticBody));
	chains.add(App->physics->CreateChain(0, 0, L_right, 24, b2BodyType::b2_staticBody));
	chains.add(App->physics->CreateChain(0, 0, triangle_right, 18, b2BodyType::b2_staticBody));
	chains.add(App->physics->CreateChain(0, 0, triangle_left, 16, b2BodyType::b2_staticBody));
	chains.add(App->physics->CreateChain(0, 0, topwall, 38, b2BodyType::b2_staticBody));
	chains.add(App->physics->CreateChain(0, 0, balls_warehouse, 30, b2BodyType::b2_staticBody));

	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	//we have to add the first ball in the map
	circles.add(App->physics->CreateCircle(550,800 , 9));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 9));
		circles.getLast()->data->listener = this;
	}
	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();
	
	App->renderer->Blit(background, 0, 0, NULL, 0.0f, NULL);

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(circle, x-5, y-5, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}
	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
