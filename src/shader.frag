#version 330 core

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) in vec2 fragUV;
layout(location = 0) out vec4 outColor;

uniform sampler2D colorAttachment;

uniform vec2 resolution;
uniform float time;
uniform uint frames;

uniform float _cameraFov;

uniform vec3 lookAt;

uniform vec3 _cameraWorldPos = vec3(0.0, 1.6, 2.8);
uniform vec3 _cameraUp = vec3(0.0, 1.0, 0.0);

layout(std140, location = 0) uniform World {
	float test;
};

const int BOUNCES = 8;
const int SAMPLES = 8;

const int OBJECT_TYPE_INVALID = -1;
const int OBJECT_TYPE_SPHERE = 0;
const int OBJECT_TYPE_PLANE = 1;
const int OBJECT_TYPE_TRIANGLE = 2;

const float PI = 3.14159265;
const float TAU = 6.2831853071;
const float RAY_T_MAX = 1e31;
const float EPSILON = 0.01;

float iSphere( in vec3 ro, in vec3 rd, in vec4 sph )
{
	vec3 oc = ro - sph.xyz;
	float b = dot( oc, rd );
	float c = dot( oc, oc ) - sph.w*sph.w;
	float h = b*b - c;
	if( h<0.0 ) return -1.0;
	return -b - sqrt( h );
}


vec3 nSphere(in vec3 p, in vec4 s) {
	return (p-s.xyz)/s.w;
}

float iPlane(in vec3 o, in vec3 d, in vec4 p) {
    return dot((p.xyz*p.w)-o, p.xyz)/dot(d,p.xyz);
}

struct Material {
	vec3 albedo;
	vec3 specular;
	float roughness;
	bool emissive;
};

struct Sphere {
	vec3 center;
	float radius;
	Material mat;
};

struct Plane {
	vec3 normal;
	float height;
	Material mat;
};

struct HitRecord {
	int type;	
	float t;
	vec3 p;
	vec3 normal;
	
	Material mat;
};

void spherical(in vec3 p, out float theta, out float phi) {
	theta = atan(p.y/p.x) + PI;
	phi = acos(p.z/length(p));
}

float OrenNeyarBRDF(in vec3 N, in vec3 L, in vec3 V, in float roughness) {
	float r2 = roughness*roughness;
	float A = 1.0 - 0.5*(r2/(r2+0.33));
    float B = 0.45*(r2/(r2+0.09));
    float dotNL = dot(N, L);
    float dotNV = dot(N, V);
    float ga = dot(V - N*dotNV , N - N*dotNL);
	return max(dotNL,0.0) * (A + B * max(0.0,ga) * sqrt((1.0 - dotNV*dotNV) * (1.0 - dotNL*dotNL)) / max(dotNL, dotNV));
}

const Sphere[6] spheres = Sphere[6](Sphere(vec3(0.0, 0.5, 0.0), 0.5, Material(vec3(1.0), vec3(0.0), 0.2, false)),
                                    Sphere(vec3(1.25, 0.25, 0.0), 0.25, Material(vec3(1.25, 0.0, 0.0), vec3(0.0), 0.01, false)),
                                    Sphere(vec3(-1.5, 0.7, -1.5), 0.7, Material(vec3(0.8, 1.5, 5.6), vec3(0.0), 0.95, false)),
                                    Sphere(vec3(1.0, 0.5, -2.0), 0.5, Material(vec3(2.8, 10.36, 5.7), vec3(0.0), 0.9999, false)),
                                    Sphere(vec3(1.0, 1.6, 1.0), 0.1, Material(vec3(50.0, 55, 60), vec3(0.0), 0.2, true)),
                                    Sphere(vec3(0.0, 1.5, 0.0), 0.15, Material(vec3(45.0, 47.0, 50.0), vec3(0.0), 0.2, true)));

const Plane[1] planes = Plane[1](Plane(vec3(0.0, 1.0, 0.0), 0.0, Material(vec3(0.7), vec3(0.0), 0.5, false)));

bool hit(in vec3 o, in vec3 d, out HitRecord rec) {
	rec.t = RAY_T_MAX;
	rec.type = OBJECT_TYPE_INVALID;
	int index;
	for(int i = 0; i < spheres.length(); ++i) {
		float t = iSphere(o, d, vec4(spheres[i].center, spheres[i].radius));
		if(t > 0.0 && min(rec.t, t) == t) {
			index = i;
			rec.type = OBJECT_TYPE_SPHERE;
			rec.t = t;		
		}
	}
	
	for(int i = 0; i < planes.length(); ++i) {
		float t = iPlane(o, d, vec4(planes[i].normal, planes[i].height));
		if(t > 0.0 && min(rec.t, t) == t) {
			index = i;
			rec.type = OBJECT_TYPE_PLANE;
			rec.t = t;
		}
	}

	if(rec.t > 0.0 && rec.t < RAY_T_MAX &&
       rec.type != OBJECT_TYPE_INVALID) {
		rec.p = o + d * rec.t;
		switch(rec.type) {
			case OBJECT_TYPE_SPHERE:
				Sphere s = spheres[index];
				rec.normal = nSphere(rec.p, vec4(s.center, s.radius));
				rec.mat = s.mat;
			break;
			case OBJECT_TYPE_PLANE:
				Plane p = planes[index];
				rec.normal = p.normal;
				rec.mat = s.mat;
			break;
		}
		return true;
	}
	
	return false;
}

#define HASHSCALE1 443.8975
#define HASHSCALE3 vec3(443.897, 441.423, 437.195)
#define HASHSCALE4 vec4(443.897, 441.423, 437.195, 444.129)

float seed = 0.0;

float hash11(float p)
{
	vec3 p3  = fract(vec3(p) * HASHSCALE1);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.x + p3.y) * p3.z);
}

float hash13(vec3 p3) {
	p3 = fract(p3 * HASHSCALE1);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.x + p3.y) * p3.z);
}

float Rand() {
	seed++;
	return hash13(vec3(fragUV, hash11(seed*1e-3)));
}

vec3 randDir(vec3 N) {
	float theta = acos(Rand()*2.0-1.0);
    float phi = TAU*Rand();
	vec3 T = normalize(cross(N.yzx, N));
    vec3 BN = cross(N, T);
	return normalize((T * cos(phi) + BN * sin(phi)) * sin(theta) + N * cos(theta));
}

vec3 shade(in vec3 o, in vec3 d) {
	vec3 color = vec3(0.0);
	vec3 weight = vec3(1.0);

	for(int i = 0; i < BOUNCES; ++i) {
		HitRecord rec;
		if(!hit(o,d,rec)) {
			color = vec3(0.0);
			break;		
		} else if(rec.mat.emissive) {
			color = rec.mat.albedo * weight;
			break;
		} else {
			vec3 V = d;
			d = randDir(rec.normal);
			o = rec.p + d * EPSILON;
			weight *= rec.mat.albedo * max(OrenNeyarBRDF(rec.normal, d, V, rec.mat.roughness),0.0);
			//weight *= max(dot(rec.normal, d), 0.0);
		}
	}
	
	return color;
}

void main(void) {
	seed = time;
	vec2 fragCoord = gl_FragCoord.xy;

	vec3 o = _cameraWorldPos;
	vec3 forward = normalize(lookAt - o);
	vec3 cameraRight = normalize(cross(forward, _cameraUp));
	float aspect = resolution.y/resolution.x;

	vec3 color = vec3(0.0);

	for(int s = 0; s < SAMPLES; s++) {
    	vec2 uv = (fragCoord + Rand())/resolution.xy;
		vec2 p = uv*2.0-1.0;
		p.y *= aspect;
		vec3 d = normalize(forward + cameraRight * p.x +
		                   _cameraUp * p.y);
    	color += shade(o, d);
    }

	color /= float(SAMPLES);

	outColor = vec4(mix(texture(colorAttachment, fragUV).rgb, color, 1.0 / float(frames)), 1.0);
}
