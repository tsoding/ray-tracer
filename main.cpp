#include <cmath>
#include <cstdint>

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "vec.hpp"

template <typename T>
using vec3 = vec<T, 3>;

using color = vec3<float>;

using plane = vec<float, 4>;

struct Wall
{
    plane p;
    color c;

    friend std::ostream& operator<<(std::ostream& os, const Wall&wall) {
		return os << "Wall{ p" << wall.p << " c" << wall.c << "}";
	 }
};

struct Sphere
{
    vec3<float> center;
    float radius;

    friend std::ostream& operator<<(std::ostream& os, const Sphere&sphere) {
		return os << "Sphere{ center " << sphere.center << " radius " << sphere.radius << "}";
	 }
};

struct Scene
{
    vec3<float> eye;
    std::vector<Sphere> spheres;
    std::vector<Wall> walls;

    friend std::ostream& operator<<(std::ostream& os, const Scene&scene) {
		os << "Scene{" << std::endl
		   << "  Eye:" << scene.eye << std::endl;
		std::for_each(scene.spheres.begin(), scene.spheres.end(), [](Sphere const& sphere) {
			  std::cout << "  " << sphere << std::endl;
		});
		std::for_each(scene.walls.begin(), scene.walls.end(), [](Wall const& wall) {
			  std::cout << "  " << wall << std::endl;
		});
		return os << "}" << std::endl;
    }
};

float dot(const plane &p, const vec3<float> &v)
{
    return p.v[0] * v.v[0] + p.v[1] * v.v[1] + p.v[2] * v.v[2] + p.v[3];
}

float dot(const vec3<float> &v1, const vec3<float> &v2)
{
    return v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2];
}

vec3<float> normalize(const vec3<float> &v)
{
    return 1.0f / sqrtf(sqr_norm(v)) * v;
}

void save_display_to_file(const color *display,
                          size_t width,
                          size_t height,
                          const std::string &filepath)
{
    std::ofstream fout(filepath, std::ofstream::out | std::ostream::binary);

    fout << "P6" << std::endl;
    fout << width << " " << height << std::endl;
    fout << 255 << std::endl;

    for (size_t row = 0; row < height; ++row) {
        for (size_t col = 0; col < width; ++col) {
            for (size_t k = 0; k < 3; ++k) {
                fout.put(static_cast<char>(255.0f * display[row * width + col].v[k]));
            }
        }
    }
}

color march(float x, float y,
            const Scene &scene,
            vec3<float> dir)
{
    vec3<float> ray = {x, y, 0.0f};
    size_t step_count = 600;

    for (size_t i = 0; i < step_count; ++i) {
        ray += dir;

        for (const auto &sphere: scene.spheres) {
            if (sqr_norm(sphere.center - ray) < sphere.radius * sphere.radius) {
                vec3<float> norm = normalize(ray - sphere.center);
                dir = normalize(dir - (2 * dot(dir, norm)) * norm);
            }
        }

        for (const auto &wall: scene.walls) {
            if (std::abs(dot(wall.p, ray)) <= 0.5f) {
                return wall.c * (1.0f - static_cast<float>(i) / static_cast<float>(step_count));
            }
        }
    }

    return {0.0f, 0.0f, 0.0f};
}

void render_scene(color *display, size_t width, size_t height,
                  const Scene &scene)
{
    const float half_width = static_cast<float>(width) * 0.5f;
    const float half_height = static_cast<float>(height) * 0.5f;

    for (size_t row = 0; row < height; ++row) {
        std::cout << "Row " << row << std::endl;
        for (size_t col = 0; col < width; ++col) {
            const vec3<float> p = { static_cast<float>(col) - half_width,
                                    static_cast<float>(row) - half_height,
                                    0.0f };

            display[row * width + col] =
                march(static_cast<float>(col) - half_width,
                      static_cast<float>(row) - half_height,
                      scene,
                      normalize(p - scene.eye));
        }
    }
}

const Scene load_scene_from_file(const char* filename)
{
   std::ifstream infile(filename);
   Scene scene = {};
   std::string type, line;

   while(std::getline(infile, line)) {
	  std::istringstream iss(line);
	  iss >> type;

	  if(type.compare("e") == 0) // eye
	  {
	    float eye1, eye2, eye3;
		 iss >> eye1 >> eye2 >> eye3;
		 scene.eye = { eye1, eye2, eye3 };
	  }
	  else if(type.compare("s") == 0) // sphere
	  {
		 float c1, c2, c3, radius;
		 iss >> c1 >> c2 >> c3 >> radius;
		 scene.spheres.push_back({ {c1, c2, c3}, radius });
	  }
	  else if(type.compare("w") == 0) // walls
	  {
		 float plane1, plane2, plane3, plane4, r, g, b;
		 iss >> plane1 >> plane2 >> plane3 >> plane4 >> r >> g >> b;

		 scene.walls.push_back({
		    {plane1, plane2, plane3, plane4},
			 {r, g, b}
		 });
	  }
   }

   return scene;
}

int main(int argc, char *argv[])
{
    const size_t width = 800, height = 600;
    std::array<color, width * height> display;

    if (argc < 4) {
        std::cerr << "./ray-tracer <sphere-x> <sphere-y> <file-name>" << std::endl;
        return -1;
    }

    const float x = static_cast<float>(std::atof(argv[1]));
    const float y = static_cast<float>(std::atof(argv[2]));
    const std::string file_name(argv[3]);

    auto scene = load_scene_from_file("./scene.txt");
    scene.spheres.push_back({ { x, y, 200.0f }, 100.0f });
    scene.spheres.push_back({ { x + 100.0f, y, 100.0f }, 100.0f } );
    std::cout << scene << std::endl;

    render_scene(display.data(), width, height, scene);
    save_display_to_file(display.data(), width, height, file_name);

    return 0;
}
