#ifndef PARTICLE_H
#define PARTICLE_H

#include "sfml_game/MyTools.h"

#include <random>

class SpriteEntity;

class ParticleGenerator
{

public:
	ParticleGenerator();
	void GenerateParticles(int frame, int imageId, float posX, float posY, int width, int height, const Vector2D & velocity, int nOfParticles, float scale);

private:

	void RandomizeVelocity(Vector2D & velocity);
	void RandomizeColor(SpriteEntity* particle);
	void RandomShuffle(float & val1, float & val2);
	void GenerateBoltParticle(int frame, const Vector2D & velocity, float posX, float posY, float lifeTime, float scale);

private:

	std::default_random_engine randomGenerator;
	std::normal_distribution<float> velocityDistribution;
	std::normal_distribution<float> driftDistribution;
	std::normal_distribution<float> lifetimeDistribution;
	std::uniform_int_distribution<> sizeDistribution;
	std::uniform_int_distribution<> colorDistribution;
	std::uniform_int_distribution<> dice6Distribution;

};

#endif
