#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <list>
#include <chrono>
//#include "materials.h"
#include "world.h"
#include "raycast.h"


int main(){

    World world;

    int xResolution = world.options.screenResolution.x;
    int yResolution = world.options.screenResolution.y;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(xResolution, yResolution), "Jank Tracer", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    bool finished = false; 

    // std::cout << GetDotProduct(sf::Vector3f(0.83205f, 0.554701f, 0), sf::Vector3f(0, -1, 0)) << ": Dot Product";

    int totalPixels =  xResolution * yResolution;
    int currentPixel = 0;

    world.MakeSphere(Sphere(sf::Vector3f(-1, -1, 1), 1.3f, MatteMaterial(sf::Color(180, 20, 220)))); 
    world.MakeSphere(Sphere(sf::Vector3f(-1, 2, 3), 1.4f, MatteMaterial(sf::Color(20, 20, 220))));
    world.MakeSphere(Sphere(sf::Vector3f(-3, 0, 7), 3.f, MatteMaterial(sf::Color(20, 220, 20))));
    world.MakeSphere(Sphere(sf::Vector3f(1, -1, 6), 1.3f, ReflectiveMaterial(sf::Color(180, 20, 220), 1.f)));
    world.MakeSphere(Sphere(sf::Vector3f(0, 0, 5), 1.4f, ReflectiveMaterial(sf::Color(20, 20, 220), 1.f)));
    world.MakeSphere(Sphere(sf::Vector3f(3, 0, 7), 3.f, MatteMaterial(sf::Color(20, 220, 20))));

    world.MakeLight(Light(sf::Vector3f(-4, -5, 6), sf::Color(255, 255, 255), 20.f));
    world.MakeLight(Light(sf::Vector3f(8, -3, 2), sf::Color(255, 255, 255), 20.f));
    world.MakeLight(Light(sf::Vector3f(0, -3, -4), sf::Color(255, 255, 255), 20.f));

    // world.MakeSphere(Sphere(sf::Vector3f(0, 0, 5), 2.f, sf::Color(255, 255, 255)));
    // world.MakeLight(Light(sf::Vector3f(30, 0, 5), sf::Color(255, 255, 255), 20.f));

    // std::cout << world.lights.size();


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (!finished){

            auto start = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < xResolution; i++)
            {
                for (int j = 0; j < yResolution; j++)
                {
                    sf::RectangleShape shape(sf::Vector2f(1, 1));
                    shape.setPosition(sf::Vector2f(i, j));

                    //Do something to get Color (ie Cast Ray)

                    shape.setFillColor(CastRay(i, j, world));

                    window.draw(shape);

                    currentPixel++;

                    
                }
                std::cout << currentPixel << " / " << totalPixels << ": %" << ((float)currentPixel / (float)totalPixels) * 100.f << "\r";
                
            }

            window.display();
            finished = true;

            auto end = std::chrono::high_resolution_clock::now();

            std::cout << "Finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds" <<std::endl;

        }
    
    }

}

