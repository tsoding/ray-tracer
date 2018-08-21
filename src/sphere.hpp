#ifndef SPHERE_HPP_
#define SPHERE_HPP_

struct Sphere {
    vec3<float> center;
    float radius;
};

std::ostream& operator<<(std::ostream& os, const Sphere&sphere) {
    return os << "Sphere{ center "
              << sphere.center
              << " radius "
              << sphere.radius
              << "}";
}


#endif  // SPHERE_HPP_
