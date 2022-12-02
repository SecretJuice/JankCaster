#include <SFML/Graphics.hpp>
#include <list>
// #include "materials.h"

class Sphere {

    public:

    sf::Vector3f worldSpacePosition;
    float radius;
    Material surfaceMaterial;

    Sphere() = default;

    Sphere(sf::Vector3f wsPos, float r, Material material){

        worldSpacePosition = wsPos;
        radius = r;
        surfaceMaterial = material;

    }

};

class Light {

    public:
    sf::Vector3f worldSpacePosition;
    sf::Color lightColor;
    float lightStrength;

    Light() = default;

    Light(sf::Vector3f worldPos, sf::Color color, float strength){

        worldSpacePosition = worldPos;
        lightColor = color;
        lightStrength = strength;

    }

};

class Options {

    public: 
    sf::Vector2i screenResolution = sf::Vector2i(600, 400);
    sf::Color skyColor = sf::Color(255, 255, 255);
    sf::Color floorColor = sf::Color(50, 50, 50);
    float floorDepth = 3.f;
    int maxReflections = 3;
    float ambientLightMultiplier = 0.05f;

    void SetScreenResolution (sf::Vector2i resolution){
        screenResolution = resolution;
    }

    void SetFloorPosition (float depth){
        floorDepth = depth;
    }

    void SetMaxReflections (int max){
        maxReflections = max;
    }

};

class World {

    public:
    std::vector<Sphere> spheres;
    std::vector<Light> lights;
    Options options;

    void MakeSphere(Sphere sph){

        spheres.push_back(sph);

    }

    void MakeLight(Light light){

        lights.push_back(light);

    }


};