#include <SFML/Graphics.hpp>

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