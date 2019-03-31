#include "levelEntity.h"
#include<iostream>
pery::LevelEntity::LevelEntity(b2World* &World, int x, int y, int width, int height)
{
	this->World = World;

	this->x = x;
	this->y = y;
	this->width  = width  * 0.5f;
	this->height = height * 0.5f;
}

pery::LevelEntity::~LevelEntity()
{
}


//Collision 
void pery::LevelEntity::CreateCollider(bool isStatic)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(x + width, y + height);
	isStatic ? BodyDef.type = b2_dynamicBody: BodyDef.type = b2_staticBody ;

	Body = World->CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox(width, height); // Creates a box shape. Divide your desired width and height by 2.

	b2FixtureDef FixtureDef;
	FixtureDef.density = isStatic ? 1.0f : 0.0f;  // Sets the density of the body
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &Shape; // Sets the shape

	Body->CreateFixture(&FixtureDef); // Apply the fixture definition
}

void pery::LevelEntity::CreateSprite(Texture2D & tileset)
{
	sprite = new Sprite(tileset, 15, 2, 16, 16);
}

void pery::LevelEntity::DrawCollider(int cameraX, int cameraY)
{
	float angle = 180 / b2_pi * Body->GetAngle();
	Rectangle r = { Body->GetPosition().x - cameraX, Body->GetPosition().y - cameraY,
		(float)width * 2, (float)height * 2 };

	Color col = { 0,255,0,130 };
	if (Body->GetType() == b2_staticBody) col = {255,0,0,80 };

	DrawRectanglePro(r, { (float)width,(float)height },
		angle, col);

}

void pery::LevelEntity::DrawSprite(int cameraX, int cameraY)
{
	if (sprite != NULL) 
	{
		float angle = 180 / b2_pi * Body->GetAngle();
		Rectangle r = { Body->GetPosition().x - cameraX, Body->GetPosition().y - cameraY,
			(float)width * 2, (float)height * 2 };

		DrawTexturePro(sprite->tilesetTexture, sprite->source, r, sprite->origin, angle, WHITE);

	}
		
}
