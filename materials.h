enum MaterialType{};
class Material {

    public:
    sf::Color materialColor;
    MaterialType materialType;

};
class MatteMaterial : public Material {

    public:
    MatteMaterial(sf::Color matColor){};
        // materialColor(matColor) {};

};

class ReflectiveMaterial : public Material {

    public:
    float materialSmoothness;

    ReflectiveMaterial(sf::Color matColor, float smoothness):
        // materialColor(matColor),
        materialSmoothness(smoothness) {};

};
class TransparentMaterial : public Material {

    public:

    float materialSmoothness;
    float refractionIndex;
    float materialOpacity;

    TransparentMaterial(sf::Color matColor, float smoothness, float rIndex, float opacity):
        materialSmoothness(smoothness),
        refractionIndex(rIndex),
        materialOpacity(opacity) {};

};