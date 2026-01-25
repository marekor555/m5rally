#include "particle.h"


void Particle::tick(const float delta) {
	time += delta;
}

void Particle::draw(M5Canvas *display, const float camposX, const float camposY) const {
	display->fillSmoothCircle(position.x - camposX, position.y - camposY, radius * ((lifetime - time) / lifetime),
							color);
}

void ParticleSpawner::spawn(const int x, const int y, const int radius, const float lifetime, const uint16_t color) {
	if (particles.size() >= maxParticles) return;
	Particle particle(x, y, lifetime, radius, color);
	particles.emplace_back(particle);
}

bool ParticleSpawner::tick(const float delta) {
	for (int i = 0; i < particles.size(); i++) {
		particles[i].tick(delta);
		if (particles[i].time > particles[i].lifetime) {
			particles.erase(particles.begin() + i);
		}
	}
	sinceSpawned += delta;
	if (sinceSpawned > spawnDelay) {
		sinceSpawned = 0;
		return true;
	}
	return false;
}

void ParticleSpawner::draw(M5Canvas *display, const float camposX, const float camposY) const {
	for (const Particle &particle: particles) {
		particle.draw(display, camposX, camposY);
	}
}

void ParticleSpawner::drawLine(M5Canvas *display, const float camposX, const float camposY, const float width) const {
	for (int i = 1; i < particles.size(); i++) {
		display->drawWideLine(particles[i - 1].position.x - camposX, particles[i - 1].position.y - camposY,
							particles[i].position.x - camposX, particles[i].position.y - camposY,
							width * ((particles[i].lifetime - particles[i].time) / particles[i].lifetime),
							particles[i].color);
	}
}
