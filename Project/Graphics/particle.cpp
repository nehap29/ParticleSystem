#include <glm/glm.hpp>


class Particle
{
public:
    glm::vec3 pos, velocity;
    unsigned char r,g,b,a;
    float size, angle, weight;
    float lifeTime;
    float theta;
    float cameraDistance;
    //bool  bounced;
    
};
