// SfmlTemplate.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <conio.h>
#include <vector>
#include "UtilsCpp.h"

#define RECT_SIZE std::make_pair(400.0f,200.0f)
#define DEBUG_CIRCLE_RADIUS 4.0f
#define ROTATION_SPEED 400.0f
#define COMPUTE_ERROR_TOLERANCE 0.2f


int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");

	//texte d'instructions
	sf::Font* font = new Font();
	font->loadFromFile("resources/arial.ttf");
	sf::Text instructionTxt = sf::Text("Fleche droite pour tourner le rectangle \n outline sur le rect si la souris est dans la boundingBox \n le rect passe en vert si on est dedans", *font, 20);
	instructionTxt.setOrigin(sf::Vector2f(instructionTxt.getGlobalBounds().width / 2, instructionTxt.getGlobalBounds().height));
	instructionTxt.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y -20));

	//rectangle de test
	sf::RectangleShape testRect = sf::RectangleShape(sf::Vector2f(RECT_SIZE.first, RECT_SIZE.second));
	testRect.setFillColor(sf::Color::Red);
	testRect.setOrigin(sf::Vector2f(sf::Vector2f(RECT_SIZE.first / 2, RECT_SIZE.second / 2)));
	testRect.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));



	// tableau ou on stocke les positions des 4 coins du rectangle
	// ordre des points, on rentre les points par rapport a l'origine du rectangle (centre)
	// et comme si il était en position x:0 y:0

	//                x:0
	//                 V
	//        0---------------- 1
	// y:0 >  |        X        |
	//        3-----------------2
	//
	sf::Vector2f orginalCornersPos[4];
	orginalCornersPos[0] = { -(RECT_SIZE.first / 2),-(RECT_SIZE.second / 2) };
	orginalCornersPos[1] = { RECT_SIZE.first / 2,-(RECT_SIZE.second / 2) };
	orginalCornersPos[2] = { RECT_SIZE.first / 2,RECT_SIZE.second / 2 };
	orginalCornersPos[3] = { -(RECT_SIZE.first / 2),RECT_SIZE.second / 2 };

	//tableau ou on stockera les nouveaux points calculés
	sf::Vector2f newCornersPos[4];
	for (int i = 0; i < 4; i++)
	{
		newCornersPos[i] = testRect.getTransform().transformPoint(orginalCornersPos[i] + testRect.getOrigin());
	}

	///DEBUG
	//tableau de cercle pour verifier que les nouvelles positions des 4 coins sont calculées correctement
	sf::CircleShape debugCircles[4];
	for (int i = 0; i < 4; i++)
	{
		debugCircles[i].setRadius(DEBUG_CIRCLE_RADIUS);
		debugCircles[i].setOrigin(sf::Vector2f(DEBUG_CIRCLE_RADIUS, DEBUG_CIRCLE_RADIUS));
		debugCircles[i].setPosition(orginalCornersPos[i] + testRect.getPosition());
		debugCircles[i].setFillColor(sf::Color::Yellow);
	}
	///END_DEBUG

	//variable pour stocker l'aire du rectangle
	float rectArea = RECT_SIZE.first * RECT_SIZE.second;

	//tableau pour stocker l'aire des 4 triangles formés entre les 4 coins du rectangle et la position de la souris
	float triangleArea[4] = { 0,0,0,0 };

	//variable pour stocker la valeur du demi perimetre du triangle (nescessaire au calcul de l'aire)
	float triangleHalfP = 0;

	//variable pour stocker le total des aires des 4 triangles
	float trianglesAreaSum = 0;

	//tableau pour calculer la longueur de chaque côtés du triangle
	float hedgeLenght[3] = { 0,0,0 };

	//variable pour calculer la difference d'aire entre le total des 4 triangles et de l'aire du rectangle
	float triangleAreaDif = 0;


	//variable pour stocker la rotation actuelle
	float actualAngle = 0;

	//position de la souris
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);

	//timer pour la rotation
	sf::Clock rotationTimer;
	rotationTimer.restart();


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//on recupére la position de la souris a chaque frame
		mousePos = sf::Mouse::getPosition(window);


		//on fait tourner le rectangle avec la flêche droite du clavier
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			actualAngle += ROTATION_SPEED * rotationTimer.getElapsedTime().asSeconds();
			if (actualAngle >= 360)
			{
				actualAngle = 0;
			}

			//on transforme les points d'origine avec le transform du rect  et on les stocke dans les nouveau points;
			for (int i = 0; i < 4; i++)
			{
				newCornersPos[i] = testRect.getTransform().transformPoint(orginalCornersPos[i] + testRect.getOrigin());
				debugCircles[i].setPosition(newCornersPos[i]);
			}
			testRect.setRotation(actualAngle);

		}

		//si la souris est dans les globals bounds on set la outline du rectangle en vert 
		if (testRect.getGlobalBounds().contains((sf::Vector2f)mousePos))
		{
			testRect.setOutlineColor(sf::Color::Yellow);
			testRect.setOutlineThickness(4);

			//boucle pour le calcul du total des aires de chaques triangles formés par les coins du rectangle et la position de la souris
			for (int i = 0; i < 4; i++)
			{
				//variable pour stocker l'indice du coin suivant (si l'indice passe a 4 alors il s'agit du coin 0
				int indexPlus = i + 1;
				if (indexPlus >= 4)
				{
					indexPlus = 0;
				}
				//on calcule la longueur de chaque côtés du triangle
				hedgeLenght[0] = Magnitude(newCornersPos[i] - newCornersPos[indexPlus]);
				hedgeLenght[1] = Magnitude(newCornersPos[i] - (sf::Vector2f)mousePos);
				hedgeLenght[2] = Magnitude(newCornersPos[indexPlus] - (sf::Vector2f)mousePos);

				//on calcule le demi perimetre du triangle
				triangleHalfP = (hedgeLenght[0] + hedgeLenght[1] + hedgeLenght[2]) / 2;
				triangleArea[i] = sqrtf(triangleHalfP * (triangleHalfP - hedgeLenght[0])* (triangleHalfP - hedgeLenght[1])* (triangleHalfP - hedgeLenght[2]));
				trianglesAreaSum += triangleArea[i];
			}

			//variable pour calculer la difference de taille , pour set un offset de verif
			triangleAreaDif = trianglesAreaSum - rectArea;
			//si la valeur est negative on la repasse en positif
			if (triangleAreaDif <= 0.0f)
			{
				triangleAreaDif *= -1.0f;
			}
			//si la difference entre le total des aires des 4 triangles et l'aire du rectangle est dans la tolérance d'écart de calcul, la souris est sur le rectangle
			if (triangleAreaDif <= COMPUTE_ERROR_TOLERANCE)
			{
				//set la couleur a vert
				testRect.setFillColor(sf::Color::Green);
				//reset des variables (juste par sécurité)
				hedgeLenght[0] = 0;
				hedgeLenght[1] = 0;
				hedgeLenght[2] = 0;
				triangleHalfP = 0;
				trianglesAreaSum = 0;
				triangleAreaDif = 0;
				for (int i = 0; i < 4; i++)
				{
					triangleArea[i] = 0;
				}
			}
			else
			{
				testRect.setFillColor(sf::Color::Red);
				//reset des variables (juste par sécurité)
				hedgeLenght[0] = 0;
				hedgeLenght[1] = 0;
				hedgeLenght[2] = 0;
				triangleHalfP = 0;
				trianglesAreaSum = 0;
				for (int i = 0; i < 4; i++)
				{
					triangleArea[i] = 0;
				}
			}

		}
		//si on est pas dans la bounding box on reset les propriétés graphiques du rectangle
		else
		{
			testRect.setOutlineColor(sf::Color::Transparent);
			testRect.setOutlineThickness(0);
			triangleAreaDif = 0;
			testRect.setFillColor(sf::Color::Red);
		}


		window.clear();
		window.draw(testRect);
		for (int i = 0; i < 4; i++)
		{
			window.draw(debugCircles[i]);
		}
		window.draw(instructionTxt);
		window.display();

		rotationTimer.restart();
	}

	return 0;
}