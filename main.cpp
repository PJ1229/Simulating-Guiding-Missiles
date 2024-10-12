#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"
#include "Player.h"
#include "PathFinding.h"

using namespace std;

int main() 
{
    Game game;

    while (game.getWindowIsOpen())
    {
        //update
        game.update();

        //render
        game.render();
    }

    return 0;
}

/*
    std::vector<sf::Vertex> a = { sf::Vertex(sf::Vector2f(0,3)), sf::Vertex(sf::Vector2f(3,0)), sf::Vertex(sf::Vector2f(1,0)),sf::Vertex(sf::Vector2f(0,0)),sf::Vertex(sf::Vector2f(2,0)), sf::Vertex(sf::Vector2f(1,2)), sf::Vertex(sf::Vector2f(-1,2)), sf::Vertex(sf::Vector2f(-1,-1)), sf::Vertex(sf::Vector2f(1,-1)), sf::Vertex(sf::Vector2f(-2,0)) };
    PathFinding *test = new PathFinding();
    test->calculateConvexHullPoints(a);
*/

