#include <cmath>
#include <cstdint>
#include <cstring>

#include <algorithm>
#include <array>
#include <optional>             // NOLINT
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "./color.hpp"
#include "./mat4x4.hpp"
#include "./vec.hpp"

struct Scene;

#include <glad/glad.h>
#include <SFML/OpenGL.hpp>

std::vector<char> readFile(const std::string &filename) {
	std::ifstream file(filename, std::ios::ate);
	if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }
    
    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    
    return buffer;
}


GLuint createShader(const char *string, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, static_cast<const GLchar **>(&string), nullptr);
	glCompileShader(shader);
	
	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		glDeleteShader(shader);
		std::cout << "Shader error log:" << std::endl
                  << errorLog.data()     << std::endl;
	}

	return shader;
}


GLuint createShaderProgram(const std::vector<GLuint>& shaders) {
	GLuint program = glCreateProgram();
	for(const GLuint &shader: shaders) {
		glAttachShader(program, shader);
	}
	glLinkProgram(program);
	
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
		glDeleteProgram(program);				
	}
	
	for(const GLuint &shader: shaders) {
		glDetachShader(program, shader);
	}
	return program;
}

double radians(double x) {
	return x * 180.0 / M_PI;
}

void previewModeGL(sf::VideoMode videoMode, const Scene &scene) {
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	
	sf::Window window(videoMode, "OpenGL", sf::Style::Default, settings);

	window.setVerticalSyncEnabled(false);
	window.setActive(true);

	if(!gladLoadGL()) {
		throw std::runtime_error("failed to create shader module!");
	}
	
	std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << std::endl;
	
	glViewport(0, 0, videoMode.width, videoMode.height);
	glEnable(GL_TEXTURE_2D);

	std::vector<GLuint> shaders;
	std::vector<char> vertexShaderString = readFile(std::string("shader.vert"));
	vertexShaderString.push_back('\0');
	std::vector<char> fragmentShaderString = readFile(std::string("shader.frag"));
	fragmentShaderString.push_back('\0');	
	
	shaders.push_back(createShader(vertexShaderString.data(), GL_VERTEX_SHADER));
	shaders.push_back(createShader(fragmentShaderString.data(), GL_FRAGMENT_SHADER));

	GLuint program = createShaderProgram(shaders);

	for(GLuint const& shader: shaders)
		glDeleteShader(shader);

	glUseProgram(program);

	// Getting shader locations
	GLint positionAttribLocation = glGetAttribLocation(program, "inPosition");
	GLint texcoordAttribLocation = glGetAttribLocation(program, "inTexcoord");
//	int worldBlockIndex = glGetUniformBlockIndex(program, "World");

	GLint lookAtUniformLocation = glGetUniformLocation(program, "lookAt");
	GLint cameraFovUniformLocation = glGetUniformLocation(program, "_cameraFov");
	GLint cameraAspectUniformLocation = glGetUniformLocation(program, "_cameraAspect");

	// Settings up fullscreen triangle
	float positions[6] = {-1.0f, -1.0f, 3.0f, -1.0f, -1.0f, 3.0f};
	float texcoords[6] = {0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 2.0f};

	GLuint vao, vbo[2];
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // positions
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), (const GLvoid*)positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(positionAttribLocation);
	glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // texcoords
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), (const GLvoid*)texcoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(texcoordAttribLocation);
	glVertexAttribPointer(texcoordAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	
	glClearColor(0.0, 0.0, 0.0, 1.0);

	bool running = true;
	while (running) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
                running = false;
			}
			else if (event.type == sf::Event::Resized) {
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUniform1f(cameraFovUniformLocation, (GLfloat)radians(75.0));
		glUniform1f(cameraAspectUniformLocation, (GLfloat)videoMode.width / (GLfloat)videoMode.height);
		glUniform3f(lookAtUniformLocation, 0.0, 0.0, 0.0);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		window.display();
	}
}

template <typename T>
using vec3 = vec<T, 3>;

using plane = vec<float, 4>;

struct Wall {
    plane p;
    color c;
};

std::ostream& operator<<(std::ostream& os, const Wall&wall) {
    return os << "Wall{ p" << wall.p << " c" << wall.c << "}";
}

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

struct Scene {
    vec3<float> eye;
    std::vector<Sphere> spheres;
    std::vector<Wall> walls;
};

std::ostream& operator<<(std::ostream& os, const Scene&scene) {
    os << "Scene{" << std::endl
        << "  Eye:" << scene.eye << std::endl;

    for (const auto&sphere : scene.spheres)
          std::cout << "  " << sphere << std::endl;

    for (const auto&wall : scene.walls)
          std::cout << "  " << wall << std::endl;

    return os << "}" << std::endl;
}

float dot(const plane &p, const vec3<float> &v) {
    return p.v[0] * v.v[0] + p.v[1] * v.v[1] + p.v[2] * v.v[2] + p.v[3];
}

float dot(const vec3<float> &v1, const vec3<float> &v2) {
    return v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2];
}

vec3<float> normalize(const vec3<float> &v) {
    return 1.0f / sqrtf(sqr_norm(v)) * v;
}

void save_display_to_file(const color *display,
                          size_t width,
                          size_t height,
                          const std::string &filepath) {
    std::ofstream fout(filepath, std::ofstream::out | std::ostream::binary);

    fout << "P6" << std::endl;
    fout << width << " " << height << std::endl;
    fout << 255 << std::endl;

    for (size_t row = 0; row < height; ++row) {
        for (size_t col = 0; col < width; ++col) {
            for (size_t k = 0; k < 3; ++k) {
                const float x = 255.0f * display[row * width + col].v[k];
                fout.put(static_cast<char>(x));
            }
        }
    }
}

float color_factor(size_t steps, size_t step_count) {
    const float x =
        1.0f - static_cast<float>(steps) / static_cast<float>(step_count);
    return x;
}

color march(float x, float y,
            const Scene &scene,
            vec3<float> dir) {
    vec3<float> ray = {x, y, 0.0f};
    size_t step_count = 600;

    for (size_t i = 0; i < step_count; ++i) {
        ray += dir;

        for (const auto &sphere : scene.spheres) {
            if (sqr_norm(sphere.center - ray) < sphere.radius * sphere.radius) {
                vec3<float> norm = normalize(ray - sphere.center);
                dir = normalize(dir - (2 * dot(dir, norm)) * norm);
            }
        }

        for (const auto &wall : scene.walls) {
            if (std::abs(dot(wall.p, ray)) <= 0.5f) {
                return wall.c * color_factor(i, step_count);
            }
        }
    }

    return {0.0f, 0.0f, 0.0f};
}

void render_scene(color *display, size_t width, size_t height,
                  const Scene &scene) {
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

const Scene load_scene_from_file(const std::string &filename) {
    std::ifstream infile(filename);
    Scene scene = {};
    std::string type, line;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        iss >> type;

        if (type == "e") {  // eye
            float eye1, eye2, eye3;
            iss >> eye1 >> eye2 >> eye3;
            scene.eye = { eye1, eye2, eye3 };
        } else if (type == "s") {  // sphere
            float c1, c2, c3, radius;
            iss >> c1 >> c2 >> c3 >> radius;
            scene.spheres.push_back({ {c1, c2, c3}, radius });
        } else if (type == "w") {  // walls
            float plane1, plane2, plane3, plane4;
            std::string color_hex;
            iss >> plane1 >> plane2 >> plane3 >> plane4 >> color_hex;

            scene.walls.push_back({
                    {plane1, plane2, plane3, plane4},
                    color_from_hex(color_hex).value_or(color{1.0f, 1.0f, 1.0f})
                });
        }
    }

    return scene;
}

void file_render_mode(const size_t width,
                      const size_t height,
                      const std::string &output_file,
                      const Scene &scene) {
    std::unique_ptr<color[]> display(new color[width * height]);
    render_scene(display.get(), width, height, scene);
    save_display_to_file(display.get(), width, height, output_file);
}

void preview_mode(const size_t width,
                  const size_t height,
                  const std::string &scene_file) {
    std::mt19937 gen { std::random_device {}() };
    std::vector<size_t> ns(height);
    std::iota(ns.begin(), ns.end(), 0);

    Scene scene = load_scene_from_file(scene_file);

    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(width),
                                          static_cast<unsigned int>(height),
                                          32),
                            "Ray Tracer");
    window.setFramerateLimit(0);
    window.setVerticalSyncEnabled(true);
    window.clear(sf::Color(0, 0, 0));
    window.display();

    const size_t textureSize = width * height * 4;

    std::vector<sf::Uint8> buffer(textureSize, 0);

    sf::Texture texture;
    texture.create(static_cast<unsigned int>(width), static_cast<unsigned int>(height));

    sf::Sprite sprite(texture, sf::IntRect(0, 0, static_cast<int>(width), static_cast<int>(height)));

    const float half_width = static_cast<float>(width) * 0.5f;
    const float half_height = static_cast<float>(height) * 0.5f;

    size_t k = 0, i = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed: {
                window.close();
                break;
            }
            case sf::Event::Resized: {
                sf::Vector2f size = static_cast<sf::Vector2f>(window.getSize());
                window.setView(sf::View(sf::FloatRect(0.f, 0.f, size.x, size.y)));
                break;
            }
            case sf::Event::KeyPressed: {
                switch (event.key.code) {
                case sf::Keyboard::Q:
                case sf::Keyboard::Escape:
                    window.close();
                    break;
                case sf::Keyboard::R:
                    scene = load_scene_from_file(scene_file);
                    std::fill(buffer.begin(), buffer.end(), 0);
                    i = 0;
                    break;
                default: break;
                }
                break;
            }
            default: break;
            }
        }

        if (i < ns.size()) {
            if (i == 0) {
                std::shuffle(ns.begin(), ns.end(), gen);
                std::cout << "\rRendering..." << std::flush;
            }

            std::cout << "\rRendering... "
                << std::fixed << std::setprecision(1)
                << static_cast<float>(100*i)/static_cast<float>(height)
                << std::left << std::setfill(' ') << std::setw(2)
                << "%" << std::flush;

            const size_t row = ns[i++];

            for (size_t col = 0; col < width; ++col) {
                const vec3<float> ray = { static_cast<float>(col) - half_width,
                                          static_cast<float>(row) - half_height,
                                          0.0f };
                const color pixel_color =
                    march(static_cast<float>(col) - half_width,
                          static_cast<float>(row) - half_height,
                          scene,
                          normalize(ray - scene.eye));

                buffer[row * width * 4 + col * 4 + 0] = static_cast<sf::Uint8>(pixel_color.v[0] * 255.0f);
                buffer[row * width * 4 + col * 4 + 1] = static_cast<sf::Uint8>(pixel_color.v[1] * 255.0f);
                buffer[row * width * 4 + col * 4 + 2] = static_cast<sf::Uint8>(pixel_color.v[2] * 255.0f);
                buffer[row * width * 4 + col * 4 + 3] = 255;
            }
        } else if (i == ns.size()) {
            ++i;
            std::cout << "\rRendering... 100.0%" << std::endl;
        }

        if (k == 0) {
            texture.update(buffer.data());
            sprite.setTexture(texture);

            window.clear();
            window.draw(sprite);
            window.display();
        }

        k = (k + 1) % 10;
    }
}

int main(int argc, char *argv[]) {
    const size_t width = 1280, height = 720;

    if (argc < 2) {
        std::cerr << "./ray-tracer <scene-file> [output-file]"
                  << std::endl;
        return -1;
    }

    const std::string scene_file(argv[1]);
    const std::optional<std::string> output_file =
        argc >= 3
        ? std::optional<std::string>(argv[2])
        : std::optional<std::string>();

    if (output_file) {
        const auto scene = load_scene_from_file(scene_file);
        file_render_mode(width, height, *output_file, scene);
    } else {
		previewModeGL(sf::VideoMode(width, height), load_scene_from_file(scene_file));	
//        preview_mode(width, height, scene_file);
    }

    return 0;
}
