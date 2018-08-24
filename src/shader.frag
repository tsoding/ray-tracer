#version 330 core

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) in vec2 fragUV;
layout(location = 0) out vec4 outColor;

uniform float _cameraFov;
uniform float _cameraAspect;

uniform vec3 lookAt = vec3(0.0, 0.0, -1.0);

uniform vec3 _cameraWorldPos = vec3(0.0, 0.0, 200.0);
uniform vec3 _cameraUp = vec3(0.0, 1.0, 0.0);

layout(std140, location = 0) uniform World {
	vec4 spheres[];	
};

float iSphere(in vec3 o, in vec3 d, in vec4 s) {
    vec3 v = o-s.xyz;
    float b = 2.0*dot(d,v), c = dot(v,v) - s.w*s.w,
    delta = b*b - 4.0*c;
    if(delta < 0.) return -1.0;
    else if(delta == 0.0 && -b >= 0.0) return -b*0.5;
    else if(delta > 0.0) {
        float h = sqrt(delta);
        float t0 = -b+h, t1 = -b-h;
        float tmin = min(t0, t1), tmax = max(t0, t1);
        return (tmin > 0.0 ? tmin : tmax > 0.0 ? tmax : -1.0)*0.5;
    }
}

vec3 nSphere(in vec3 p, in vec4 s) {
	return (p-s.xyz)/s.w;
}

float iPlane(in vec3 o, in vec3 d, in vec4 p) {
    return dot((p.xyz*p.w)-o, p.xyz)/dot(d,p.xyz);
}

const int MAT_COMMON = 0;
const int MAT_SPECULAR = 1;
const int MAT_EMISSIVE = 2;

struct Material {
	vec3 albedo;
	vec3 specular;
	float roughness;
	float metalic;
	int type;
};

struct Sphere {
	vec3 center;
	float radius;
	int mat_id;
};

struct Plane {
	vec3 normal;
	float height;
	int mat_id;
};

struct HitInfo {
	float t;
	vec3 p;
	vec3 normal;
	
	int mat_id;
};

void main(void) {
	vec2 p = fragUV * 2.0 - 1.0;
	vec3 o = _cameraWorldPos;
	vec3 forward = normalize(lookAt - o);
	vec3 cameraRight = normalize(cross(forward, _cameraUp));
	float halfHeight = tan(_cameraFov * 0.5);
	float halfWidth = halfHeight * _cameraAspect;
	vec3 d = normalize(forward + cameraRight * p.x * halfWidth +
                       _cameraUp * p.y * halfHeight);

	vec4 s = vec4(0.0, 0.0, -200.0, 100.0);
	float t = iSphere(o, d, s);
	if(t > 0.0) {
		vec3 point = o+d*t;
		vec3 normal = nSphere(point, s);
		outColor = vec4(normal * 0.5 + 0.5, 1.0);
	} else {
		outColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}
