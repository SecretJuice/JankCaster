#include <SFML/Graphics.hpp>

class Hit {};
float GetDistanceVector3f();
sf::Vector3f GetDirectionVector3f();
float GetDotProduct();
sf::Color AddWeightedColors();
sf::Color ApplyFactorToColor();
Hit DoRayCast();
float CastLightRay();
sf::Color CastRay(int x, int y, World world) {};