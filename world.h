#include <list>
#include <SFML/Graphics.hpp>
#include "materials.h"

class Sphere {

    public:

    sf::Vector3f worldSpacePosition;
    float radius;
    Material surfaceMatieral;

    Sphere(sf::Vector3f position, float rad, Material material):
        worldSpacePosition(position),
        radius(rad),
        surfaceMatieral(material) {};

};
class Light {

    public:

    sf::Vector3f worldSpacePosition;
    sf::Color lightColor;
    float lightStrength;

    Light(sf::Vector3f position, sf::Color color, float strength):
        worldSpacePosition(position),
        lightColor(color),
        lightStrength(strength) {};

};
class Options {

    public:

    sf::Vector2i screenResolution = sf::Vector2i(600, 400);
    sf::Color skyColor;
    sf::Color floorColor;

    float floorDepth;
    int maxReflections;
    float ambientLightMultiplier;

    void SetScreenResolution(sf::Vector2i resolution) {};
    void SetFloorPosition(float depth) {};
    void SetMaxReflections(int max) {};

};
class World {

    public:

    std::vector<Sphere> spheres;
    std::vector<Light> lights;
    Options options;

    void MakeSphere(Sphere sphere) {};
    void MakeLight(Light light) {};

};