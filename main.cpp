#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <list>
#include <chrono>

enum MaterialType{

    Matte = 0, Reflective = 1, Transparent = 2

};

class Material{

    public:
    sf::Color materialColor;
    MaterialType materialType;

};

class MatteMaterial: public Material {

    public:

    //materialType = Matte;

    MatteMaterial(sf::Color matColor) :Material{} {
        materialColor = matColor;
        materialType = Matte;
        
    }

};

class ReflectiveMaterial: public Material {

    public:

    float materialSmoothness;
    

    ReflectiveMaterial() = default;

    ReflectiveMaterial(sf::Color matColor, float smoothness):Material{}{
        materialColor = matColor;
        materialSmoothness = smoothness;
        materialType = Reflective;
    }

};

class TransparentMaterial: public ReflectiveMaterial {

    public:

    float refractionIndex;
    float materialOpacity;
    

    TransparentMaterial(sf::Color matColor, float smoothness, float rIndex, float opacity){
        materialColor = matColor;
        
        materialSmoothness = smoothness;
        refractionIndex = rIndex;
        materialOpacity = opacity;
        materialType = Transparent;
    }

};

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
    sf::Vector2i screenResolution = sf::Vector2i(1200, 800);
    sf::Color skyColor = sf::Color(180, 180, 255);
    sf::Color floorColor = sf::Color(255, 100, 100);
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

class Hit {
    public:
    Sphere sphere;
    sf::Vector3f hitPosition;
    sf::Vector3f hitNormal;
    float rayLength;
    bool hitFloor;
    bool hitNothing = true;

    void SetHitObject(Sphere sph){
        sphere = sph;
        hitNothing = false;
    }
    void SetHitPosition(sf::Vector3f position){
        hitPosition = position;
        hitNothing = false;
    }
    void SetHitNormal(sf::Vector3f normal){
        hitNormal = normal;
        hitNothing = false;
    }
    void SetRayLength(float length){
        rayLength = length;
    }
    void SetHitFloor(bool b){
        hitFloor = b;
        hitNothing = false;
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

float GetDistanceVector3f(sf::Vector3f v1, sf::Vector3f v2){

    float distance = pow(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2), .5); 
    return distance;

}

sf::Vector3f GetDirectionVector3f(sf::Vector3f v1, sf::Vector3f v2, bool normalized){


    sf::Vector3f direction (v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);

    if (!normalized){
        return direction;
    }

    float distance = GetDistanceVector3f(v1, v2);

    if (distance == 0){
        return direction;
    }

    direction = sf::Vector3f(direction.x / distance, direction.y / distance, direction.z / distance);
    return direction;

}

float GetDotProduct(sf::Vector3f v1, sf::Vector3f v2){

    float dotProduct = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
    return dotProduct;

}

sf::Color AverageColors(sf::Color c1, sf::Color c2, float c2Weight){

    int colorInts1 [3] = {c1.r, c1.g, c1.b};
    int colorInts2 [3] = {c2.r, c2.g, c2.b};

    colorInts1[0] = ((colorInts1[0] * c2Weight) + colorInts2[0]) / 2;
    colorInts1[1] = ((colorInts1[1] * c2Weight) + colorInts2[1]) / 2;
    colorInts1[2] = ((colorInts1[2] * c2Weight) + colorInts2[2]) / 2;

    for (int k = 0; k < 3; k++)
    {
        if (colorInts1[k] < 0){ colorInts1[k] = 0;}
        if (colorInts1[k] > 255){ colorInts1[k] = 255;} 
    }

    return sf::Color(colorInts1[0], colorInts1[1], colorInts1[2]);

}

sf::Color ApplyFactorToColor(sf::Color color, float lightingFactor){

    int colorInts [3] = {color.r, color.g, color.b};

    colorInts[0] *= lightingFactor;
    colorInts[1] *= lightingFactor;
    colorInts[2] *= lightingFactor;

    for (int k = 0; k < 3; k++)
    {
        if (colorInts[k] < 0){ colorInts[k] = 0;}
        if (colorInts[k] > 255){ colorInts[k] = 255;} 
    }
            
    return sf::Color(colorInts[0], colorInts[1], colorInts[2]);

}

Hit DoRayCast(sf::Vector3f startPosition, sf::Vector3f rayDir, float maxDistance, World world){

    sf::Vector3f currentPosition = startPosition;
    float distanceTraveled = 0.f;

    std::vector<Sphere> spheres = world.spheres;
    float floorDepth = world.options.floorDepth;

    Hit hit;

    while(distanceTraveled < maxDistance){

        float closestDistance = maxDistance;
        bool floorIsClosest = false;
        Sphere closestSphere;

        for (int i = 0; i < spheres.size(); i++)
        {
            float distance = GetDistanceVector3f(currentPosition, spheres[i].worldSpacePosition) - spheres[i].radius;
            
            if (distance < closestDistance){
                closestDistance = distance;
                closestSphere = spheres[i];
            }
        }

        if (rayDir.y > 0){

            float floorDistance = GetDistanceVector3f(currentPosition, sf::Vector3f(currentPosition.x, floorDepth, currentPosition.z));

            if (floorDistance < closestDistance){
                closestDistance = floorDistance;
                floorIsClosest = true;
            }

        }

        currentPosition = sf::Vector3f(currentPosition.x + (rayDir.x * closestDistance), currentPosition.y + (rayDir.y * closestDistance), currentPosition.z + (rayDir.z * closestDistance));
        
        if (closestDistance < 0.001f){

            if (floorIsClosest){

                //currentPosition.y = floorDepth;

                hit.SetHitPosition(currentPosition);
                hit.SetHitNormal(sf::Vector3f(0, -1, 0));
                hit.SetHitFloor(true);
                hit.SetRayLength(distanceTraveled);
                break;

            }

            sf::Vector3f finalPos = GetDirectionVector3f(closestSphere.worldSpacePosition, currentPosition, true);
            // finalPos = sf::Vector3f(finalPos.x * closestSphere.radius + closestSphere.worldSpacePosition.x, 
            //                         finalPos.y * closestSphere.radius + closestSphere.worldSpacePosition.y,
            //                         finalPos.z * closestSphere.radius + closestSphere.worldSpacePosition.z);

            hit.SetHitObject(closestSphere);
            hit.SetHitPosition(currentPosition);
            hit.SetHitNormal(finalPos);
            hit.SetHitFloor(false);
            hit.SetRayLength(distanceTraveled);

            break;

        }
        distanceTraveled += closestDistance;

    }

    return hit;

}


float CastLightRay(sf::Vector3f startPosition, World world, sf::Vector3f normal){

    float lightMultiplier = 0.f;
    float floorDepth = 3.f;

    std::vector<Light> lights = world.lights; 
    std::vector<Sphere> spheres = world.spheres;

    for (int i = 0; i < lights.size(); i++ ){

        Light light = world.lights[i];

        sf::Vector3f currentPosition = startPosition;

        sf::Vector3f direction = GetDirectionVector3f(startPosition, light.worldSpacePosition, true);
        float distance = GetDistanceVector3f(startPosition, light.worldSpacePosition);
        float distanceTraveled = 0.f;

        bool hit = false;
        bool firstRun = true;

        float dotProduct = GetDotProduct(normal, direction);

        if (dotProduct < 0){
            dotProduct = 0;
            lightMultiplier += dotProduct;
            continue;
        }

        currentPosition += normal * 0.0011f;

        Hit rayCastHit = DoRayCast(currentPosition, direction, distance, world);

        if (rayCastHit.hitNothing){
            lightMultiplier += dotProduct;
        }
    }

    if (lightMultiplier < world.options.ambientLightMultiplier){
        lightMultiplier = world.options.ambientLightMultiplier;
    }

    return lightMultiplier;

}

sf::Color CastRay(int x, int y, World world) {

    float maxDistance = 40.f;
    float focalPointDistance = 0.4f;
    sf::Color color = world.options.skyColor;

    std::vector<Sphere> spheres = world.spheres;

    sf::Vector3f rayStartPos ((float)x / world.options.screenResolution.x, (float)y / world.options.screenResolution.x, 0);

    sf::Vector3f rayDir(GetDirectionVector3f(sf::Vector3f(0.5f, 0.3f, -focalPointDistance), rayStartPos, true));

    sf::Vector3f currentPosition = rayStartPos;

    Hit rayCastHit = DoRayCast(currentPosition, rayDir, maxDistance, world);

    if (rayCastHit.hitNothing){

        return color;

    }
    
    if (rayCastHit.hitFloor){

        color = world.options.floorColor;

        return ApplyFactorToColor(color, CastLightRay(rayCastHit.hitPosition, world, rayCastHit.hitNormal));

    }

    switch (rayCastHit.sphere.surfaceMaterial.materialType)
    {
    case Matte:
        color = rayCastHit.sphere.surfaceMaterial.materialColor;

        return ApplyFactorToColor(color, CastLightRay(rayCastHit.hitPosition, world, rayCastHit.hitNormal));
        break;
    case Reflective:
        {
            Material material = rayCastHit.sphere.surfaceMaterial;
            color = material.materialColor;

            sf::Vector3f reflection = (-2 * GetDotProduct(rayDir, rayCastHit.hitNormal) * rayCastHit.hitNormal) + rayDir;

            sf::Vector3f newStartPos = rayCastHit.hitPosition + (rayCastHit.hitNormal * 0.01f);

            Hit reflectionHit = DoRayCast(newStartPos, reflection, maxDistance, world);

            sf::Color reflectionColor;

            if (reflectionHit.hitNothing){
                reflectionColor = world.options.skyColor;
                reflectionColor = AverageColors(reflectionColor, ApplyFactorToColor(color, CastLightRay(rayCastHit.hitPosition, world, rayCastHit.hitNormal)), 0.1f);
                return reflectionColor;
            }
            if (reflectionHit.hitFloor){
                reflectionColor = world.options.floorColor;
                reflectionColor = AverageColors(ApplyFactorToColor(reflectionColor, CastLightRay(reflectionHit.hitPosition, world, reflectionHit.hitNormal)),
                                  ApplyFactorToColor(color, CastLightRay(rayCastHit.hitPosition, world, rayCastHit.hitNormal)), 0.1f);
                return reflectionColor;
            }

            if (!(reflectionHit.hitNothing || reflectionHit.hitFloor)){
                reflectionColor = reflectionHit.sphere.surfaceMaterial.materialColor;

                reflectionColor = AverageColors(ApplyFactorToColor(reflectionColor, CastLightRay(reflectionHit.hitPosition, world, reflectionHit.hitNormal)),
                                  ApplyFactorToColor(color, CastLightRay(rayCastHit.hitPosition, world, rayCastHit.hitNormal)), 0.1f);
                return reflectionColor;
            }

            return reflectionColor;
            break;
        }
        

    case Transparent:

        color = rayCastHit.sphere.surfaceMaterial.materialColor;

        return ApplyFactorToColor(color, CastLightRay(rayCastHit.hitPosition, world, rayCastHit.hitNormal));
        break;

    
    // default:
    // color = rayCastHit.sphere.surfaceMaterial.materialColor;

    //     return ApplyFactorToColor(color, CastLightRay(rayCastHit.hitPosition, world, rayCastHit.hitNormal));
    //     break;
    }
}

int main(){

    World world;

    int xResolution = world.options.screenResolution.x;
    int yResolution = world.options.screenResolution.y;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(xResolution, yResolution), "Jank Tracer", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    bool finished = false; 

    std::cout << GetDotProduct(sf::Vector3f(0.83205f, 0.554701f, 0), sf::Vector3f(0, -1, 0)) << ": Dot Product";


    world.MakeSphere(Sphere(sf::Vector3f(-1, -1, 1), 1.3f, MatteMaterial(sf::Color(180, 20, 220)))); 
    world.MakeSphere(Sphere(sf::Vector3f(-1, 2, 3), 1.4f, MatteMaterial(sf::Color(20, 20, 220))));
    world.MakeSphere(Sphere(sf::Vector3f(-3, 0, 7), 3.f, MatteMaterial(sf::Color(20, 220, 20))));
    world.MakeSphere(Sphere(sf::Vector3f(1, -1, 6), 1.3f, TransparentMaterial(sf::Color(180, 20, 220), 1.f, 1.1f, 0.2f)));
    world.MakeSphere(Sphere(sf::Vector3f(0, 0, 5), 1.4f, ReflectiveMaterial(sf::Color(20, 20, 220), 1.f)));
    world.MakeSphere(Sphere(sf::Vector3f(3, 0, 7), 3.f, MatteMaterial(sf::Color(20, 220, 20))));

    world.MakeLight(Light(sf::Vector3f(-4, -5, 6), sf::Color(255, 255, 255), 20.f));
    world.MakeLight(Light(sf::Vector3f(8, -3, 2), sf::Color(255, 255, 255), 20.f));
    world.MakeLight(Light(sf::Vector3f(0, -3, -4), sf::Color(255, 255, 255), 20.f));

    // world.MakeSphere(Sphere(sf::Vector3f(0, 0, 5), 2.f, sf::Color(255, 255, 255)));
    // world.MakeLight(Light(sf::Vector3f(30, 0, 5), sf::Color(255, 255, 255), 20.f));

    std::cout << world.lights.size();


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
                }
                
            }

            window.display();
            finished = true;

            auto end = std::chrono::high_resolution_clock::now();

            std::cout << "Finished the thing, in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

        }
    
    }

}

