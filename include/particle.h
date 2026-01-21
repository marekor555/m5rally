#ifndef M5RALLY_PARTICLE_H
#define M5RALLY_PARTICLE_H
#include "collision.h"
#include <M5Cardputer.h>
#include <vector>

class Particle {
public:
	Pos2D position;
	float lifetime;
	float time = 0;
	int radius;
	uint16_t color;

	Particle(const int x, const int y, const float lifetime, const int radius, const uint16_t color) : position(x,y), lifetime(lifetime), radius(radius), color(color) {
	}
	void tick(float delta);
	void draw(M5Canvas *display, float camposX, float camposY) const;
};

class ParticleSpawner {
public:
	std::vector<Particle> particles;
	int maxParticles;
	float sinceSpawned = 0;
	float spawnDelay;

	ParticleSpawner(const int maxParticles, const float spawnDelay) : maxParticles(maxParticles), spawnDelay(spawnDelay) {
		particles.reserve(maxParticles);
	}

	void spawn(int x, int y, int radius, float lifetime, uint16_t color);
	bool tick(float delta);
	void draw(M5Canvas *display, float camposX, float camposY) const;
};

#endif //M5RALLY_PARTICLE_H