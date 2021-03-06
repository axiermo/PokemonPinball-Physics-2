#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	PhysBody * CreateCircleSensor(int x, int y, int radius);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	PhysBody * CreateCircle(int x, int y, int radius, b2BodyType type);
	PhysBody * CreateChain(int x, int y, int * points, int size, b2BodyType type);

	update_status PostUpdate();
	bool CleanUp();

	
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	b2PrismaticJoint * CreatePrismaticJoint(PhysBody * bodyA, PhysBody * bodyB, b2Vec2 anchorA, b2Vec2 anchorB, float low_trams, float upp_trans, float max_motor_force, float speed);
	PhysBody * CreateRectangle(int x, int y, int width, int height, b2BodyType bodytype);
	b2RevoluteJoint* CreateRevoluteJoint(PhysBody* A, PhysBody* B, float anchor_x, float anchor_y, int reference_Angle, int lower_angle, int upper_angle);
	PhysBody* CreatePolygon(int x, int y, int* points, int size);

	void BeginContact(b2Contact* contact);
	b2RevoluteJoint * CreateRevoluteJoint(PhysBody * bodyA, PhysBody * bodyB, float anchor_x, float anchor_y, int upper_angle, int lower_angle, int max_torque, int speed);
	bool DeleteBody(b2Body* circle);

private:

	bool debug;	
	b2World* world;
	b2Body* ground;

	// Joints
	b2MouseJoint* mouse_joint;
	b2Vec2 mouse_pos;
};