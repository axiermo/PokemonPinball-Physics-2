#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}
PhysBody * ModulePhysics::CreateCircleSensor(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 0.5f;
	fixture.restitution = 0.33f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

// Destructor
ModulePhysics::~ModulePhysics()
{}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// Needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	return true;
}

update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, b2BodyType type)
{
	b2BodyDef body;

	
		body.type = type;
	

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

bool ModulePhysics::DeleteBody(b2Body* body)
{
	if (body != NULL) 
	{
		world->DestroyBody(body);
		body->SetUserData(NULL);
		body = NULL;
	}

	return true;
}

PhysBody* ModulePhysics::CreatePolygon(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape polygon;

	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	polygon.Set(p, size / 2);

	b2FixtureDef fixture;
	fixture.density = 1.0f;
	fixture.shape = &polygon;
	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}
b2PrismaticJoint* ModulePhysics::CreatePrismaticJoint(PhysBody* bodyA, PhysBody* bodyB, b2Vec2 anchorA, b2Vec2 anchorB, float low_trams, float upp_trans, float max_motor_force, float speed)
{
	b2PrismaticJointDef def;

	def.bodyA = bodyA->body;
	def.bodyB = bodyB->body;
	def.collideConnected = false;
	def.localAxisA.Set(0, 1);
	def.localAnchorA.Set(PIXEL_TO_METERS(anchorA.x), PIXEL_TO_METERS(anchorA.y));
	def.localAnchorB.Set(PIXEL_TO_METERS(anchorB.x), PIXEL_TO_METERS(anchorB.y));

	//jointDef.Initialize(myBodyA, myBodyB, myBodyA->GetWorldCenter(), worldAxis);
	def.enableLimit = true;
	def.lowerTranslation = PIXEL_TO_METERS(low_trams);
	def.upperTranslation = PIXEL_TO_METERS(upp_trans);
	def.type = e_prismaticJoint;

	def.enableMotor = true;
	def.maxMotorForce = max_motor_force;
	def.motorSpeed = speed*DEGTORAD;



	b2PrismaticJoint* prismatic_joint = (b2PrismaticJoint*)world->CreateJoint(&def);

	return prismatic_joint;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, b2BodyType bodytype)
{
	b2BodyDef body;
	body.type = bodytype;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

update_status ModulePhysics::PostUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (!debug)
		return UPDATE_CONTINUE;

	bool check = false;
	PhysBody body;
	mouse_pos.x = PIXEL_TO_METERS(App->input->GetMouseX());
	mouse_pos.y = PIXEL_TO_METERS(App->input->GetMouseY());

	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
			// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();
				App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
			}
			break;

			// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->GetVertexCount();
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->GetVertex(i));
					if (i > 0)
						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->GetVertex(0));
				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
			}
			break;

			// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
				b2Vec2 prev, v;

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)
						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
					prev = v;
				}

				v = b->GetWorldPoint(shape->m_vertices[0]);
				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
			}
			break;
			}

			// TODO 1: If mouse button 1 is pressed ...
			// App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
			// test if the current body contains mouse position

			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && check == false)
			{
				if (f->GetShape()->TestPoint(b->GetTransform(), mouse_pos))
				{
					check = true;
					body.body = b;
				}
				else body.body = NULL;
			}
		}
	}

	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	// TODO 2: If a body was selected, create a mouse joint
	// using mouse_joint class property

	if (body.body != NULL)
	{
		b2MouseJointDef def;
		def.bodyA = ground;
		def.bodyB = body.body;
		def.target = mouse_pos;
		def.dampingRatio = 0.5f;
		def.frequencyHz = 2.0f;
		def.maxForce = 50.0f * body.body->GetMass();
		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
	}

	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && mouse_joint != nullptr)
	{
		mouse_joint->SetTarget(mouse_pos);
		App->renderer->DrawLine(METERS_TO_PIXELS(mouse_joint->GetBodyB()->GetPosition().x), METERS_TO_PIXELS(mouse_joint->GetBodyB()->GetPosition().y), METERS_TO_PIXELS(mouse_pos.x), METERS_TO_PIXELS(mouse_pos.y), 255, 0, 0);
	}

	// TODO 4: If the player releases the mouse button, destroy the joint

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && mouse_joint != nullptr)
	{
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}
	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}
b2RevoluteJoint* ModulePhysics::CreateRevoluteJoint(PhysBody* bodyA, PhysBody* bodyB, float anchor_x, float anchor_y, int upper_angle, int lower_angle, int max_torque, int speed)
{

	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = bodyA->body;
	revoluteJointDef.bodyB = bodyB->body;
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.type = e_revoluteJoint;
	//el anchor de A es per defecte 0,0
	revoluteJointDef.localAnchorB.Set(PIXEL_TO_METERS(anchor_x), PIXEL_TO_METERS(anchor_y));

	if (lower_angle != NULL && upper_angle != NULL)
	{
		revoluteJointDef.enableLimit = true;
		revoluteJointDef.lowerAngle = lower_angle * DEGTORAD;
		revoluteJointDef.upperAngle = upper_angle * DEGTORAD;
	}
	else
		revoluteJointDef.enableLimit = false;

	if (max_torque != 0)
	{
		revoluteJointDef.enableMotor = true;
		revoluteJointDef.maxMotorTorque = max_torque;
		revoluteJointDef.motorSpeed = speed * DEGTORAD; //90 degrees per second
	}
	else
		revoluteJointDef.enableMotor = false;


	b2RevoluteJoint* revolute_joint = (b2RevoluteJoint*)world->CreateJoint(&revoluteJointDef);

	return revolute_joint;
}

b2RevoluteJoint* ModulePhysics::CreateRevoluteJoint(PhysBody* A, PhysBody* B, float anchor_x, float anchor_y, int reference_angle, int lower_angle, int upper_angle)
{
	b2RevoluteJointDef def;
	def.bodyA = A->body;
	def.bodyB = B->body;
	def.collideConnected = false;

	def.localAnchorA.Set(PIXEL_TO_METERS(anchor_x), PIXEL_TO_METERS(anchor_y));
	def.localAnchorB.Set(0, 0); 

	def.enableLimit = true;
	def.referenceAngle = reference_angle * DEGTORAD;
	def.lowerAngle = lower_angle * DEGTORAD;
	def.upperAngle = upper_angle * DEGTORAD;

	def.enableMotor = true;
	def.maxMotorTorque = 180;

	return (b2RevoluteJoint*)world->CreateJoint(&def);
}