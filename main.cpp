// raylib_test1.cpp : Defines the entry point for the application.
//

#include "dependencies.h"
using namespace std;


int main()
{
	struct Composition {
		string namw;
		double percentage;
	};

	struct Alloy {
		string name;
		double density; //kg per 1 m^3
		Color color;
		vector<Composition> compostiton;
	};

	//alloy metals
	vector<Alloy> alloys = {
		//Aerospace & High Performance alloys
		{ "Titanium 6Al-4V", 4430, GRAY, {{"Titanium", 90.0}, {"Aluminum", 6.0}, {"Vanadium", 4.0}} },
		{ "Hastelloy C276", 8890, LIGHTGRAY,{{"Nickel", 57.0}, {"Molybdenum", 16.0}, {"Chromium", 15.5}} },
		
		//Industrial alloys
		{ "Stainless Steel 304", 8000, DARKGRAY,{{"Iron", 70.0}, {"Chromium", 19.0}, {"Nickel", 10.0}} },
		{ "Cast Iron", 7200, DARKGRAY, {{"Iron", 96.0}, {"Carbon", 3.0}, {"Silicon", 1.0}} },
		{ "Manganese Steel", 7870, LIGHTGRAY,{{"Iron", 86.0}, {"Manganese", 13.0}, {"Carbon", 1.0}} },

		//Copper & Non-ferous alloys
		{ "Brass", 8470, BROWN, {{"Copper", 65.0}, {"Zinc", 35.0}} },
		{ "Bronze", 8800, DARKBROWN, {{"Copper", 95}, {"Tin", 5.0}} },
		{ "Cupronickel", 8940, RED,{{"Copper", 75.0}, {"Nickel", 25.0}} },
		{ "Monel 400", 8800, LIGHTGRAY, {{"Nickel", 67.0}, {"Copper", 30.0}, {"Iron", 2.0}} },

		//low melting point & Soft alloys
		{ "Pewter", 7260, GRAY,{{"Tin", 91.0}, {"Antimony", 7.5}, {"Copper", 1.5}} },
		{ "Wood's Metal", 9700, BROWN, {{"Bismuth", 50.0}, {"Lead", 26.7}, {"Tin", 13.3}, {"Cadmium", 10.0}} },
		{ "Solder", 8400, LIGHTGRAY, {{"Tin", 63.0}, {"Lead", 37.0}} },

		//Jewelry alloys
		{ "Sterling Silver", 10360,	LIGHTGRAY, {{"Silver", 92.5}, {"Copper", 7.5}} },
		{ "18K Yellow Gold", 15580, GOLD, {{"Gold", 75.0}, {"Copper", 12.5}, {"Silver", 12.5}} },
		{ "Alnico V", 7300, LIGHTGRAY, {{"Iron", 51.0}, {"Cobalt", 24.0}, {"Nickel", 14.0}, {"Aluminum", 8.0}} },
		{ "Magnalium", 2000, LIGHTGRAY, {{"Aluminum", 95.0}, {"Magnesium", 5.0}} }
	};


	//Define window parameters
	const int width = 1000;
	const int height = 750;
	const char* title = "Projectile Physics engine";

	//initialize window
	InitWindow(width, height, title);

	struct Ball {
		Vector2 position;
		Color color;
	};

	Vector2 ballposition = { (float)width/2, 0.0f}; //intial ball postion
	Vector2 linepoint{}; //end point for magnitude measurement
	Vector2 current{}; //current point of cursor
	Vector2 objVelocity = { 0.0f, 0.0f }; //ball velocity
	Vector2 ballspeed = { 0.0f, 0.0f }; //ballspeed
	string screentext = "Select an alloy";
	bool isDragging = false;
	float avgVelocity = 2.0f;
	float gravity = 0.5f; //gravity for ball
	float launchMultiplier = 0.0f; //launch multiplier
	float friction = 0.98f; //friction to floor
	float dist = 0.0f; //magnitude
	float res = 0.0f;

	SetTargetFPS(60); // set target fps of game

	//main loop for game
	while (!WindowShouldClose()) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			linepoint = GetMousePosition();
			dist = 0.0f;
			isDragging = true;
		};

		if (isDragging && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) { //launch ball after mouse button is released
			current = GetMousePosition();
			res = Vector2Distance(linepoint, current);
			Vector2 DragVector = Vector2Subtract(linepoint, current);
			objVelocity = Vector2Scale(DragVector, launchMultiplier);
			isDragging = false;
		};

		if (!isDragging) { //applys gravity and launch vectors to ball
			objVelocity.y += gravity;
			ballspeed.x += objVelocity.x;
			ballspeed.y += objVelocity.y;
			objVelocity.x *= friction;
			objVelocity.y *= friction;
		};

		if (ballposition.y > height - 50 || ballposition.x < -50 || ballposition.x > width + 50) { //check collisions on the horizontal
			ballposition = { 20.0f, (float)height / 2};
			ballspeed = { 0.0f, 0.0f };
		};

		ballposition.y += ballspeed.y;
		ballposition.x += ballspeed.x;

		if (ballposition.y >= (710 - 10)) { //collision check
			ballposition.y = (710 - 10); //reset ball position to ground
			ballspeed = { 0.0f, 0.0f };
		} else if (ballposition.y <= 10) ballposition.y = 10;
		dist += ballspeed.x;
		BeginDrawing(); //start drawing on window

		int mX = width/18; //x coordinate for each metal ball
		int mY = height / 18; //y coordiante for each metal ball
		for (int i = 0; i < alloys.size(); i++) { //Display each metal option
			Ball ball;
			ball.color = alloys[i].color;
			ball.position.x = mX;
			ball.position.y = mY * 0.8f;
			
			DrawCircle(ball.position.x, ball.position.y, 10, ball.color);
			
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { //selecting a circle
				if (CheckCollisionPointCircle(GetMousePosition(), ball.position, 20)) { //launch according to metal weigth
					launchMultiplier = 0.012f;
					screentext = alloys[i].name;
					ballposition.x = ball.position.x;
					ballposition.y = ball.position.y;
					launchMultiplier *= (10000/alloys[i].density);
				}
			};
			
			mX += width/18;
			mY += height / 18;
		};

		ClearBackground(RAYWHITE); //clear background white avoid flickering

		DrawText(screentext.c_str(), 700, 100, 20, DARKBLUE); //Text for window

		DrawText(TextFormat("Distance: %0.2fcm", dist * 2.54f / 96.0f), 700, 200, 20, DARKBLUE); //travel distance text;

		DrawText(TextFormat("Velocity: %0.2fm/s", res/(100.0f*GetFrameTime())), 700, 300, 20, DARKBLUE); //velocity text

		DrawCircle(ballposition.x, ballposition.y, 10, DARKBLUE); //draw ball

		DrawRectangle(0, 710, width, 20, BLACK); //Draw rectangle

		EndDrawing(); //end drawing mode
	};
	CloseWindow(); //close window and OpenGL context

	return 0;
}
