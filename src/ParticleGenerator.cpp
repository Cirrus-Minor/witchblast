#include "ParticleGenerator.h"

#include "sfml_game/ImageManager.h"
#include "sfml_game/CollidingSpriteEntity.h"
#include "Constants.h"
#include "WitchBlastGame.h"

const float drift = 25.0f;

ParticleGenerator::ParticleGenerator()
: velocityDistribution(std::normal_distribution<float>(0.0f, 1.0f))
, driftDistribution(std::normal_distribution<float>(drift, drift))
, lifetimeDistribution(std::normal_distribution<float>(0.3f, 0.1f))
, sizeDistribution(std::uniform_int_distribution<>(10, 20))
, colorDistribution(std::uniform_int_distribution<>(0, 20))
, dice6Distribution(std::uniform_int_distribution<>(0,6))
{}

void ParticleGenerator::GenerateParticles(int frame, int imageId, float posX, float posY, int width, int height, const Vector2D & velocity, int nOfParticles, float scale)
{

	for (int i = 0; i < nOfParticles; ++i)
	{
		//int sizeFactor = sizeDistribution(randomGenerator);
		Vector2D newVelocity = velocity;
		RandomizeVelocity(newVelocity);
		float lifeTime = lifetimeDistribution(randomGenerator);
		if (scale == 0.7f) lifeTime *= 0.3f;

		GenerateBoltParticle(frame, newVelocity, posX, posY, lifeTime, scale );
	}
}

void ParticleGenerator::RandomShuffle(float & val1, float & val2)
{
	float factor1 = velocityDistribution(randomGenerator) * 15.0f;
	//float factor2 = driftDistribution(randomGenerator);
	val1 += factor1;
	val2 += velocityDistribution(randomGenerator) * 15.0f;
}

void ParticleGenerator::RandomizeVelocity(Vector2D & velocity)
{
	if (velocity.x == 0.0f)
		RandomShuffle(velocity.x, velocity.y);
	else
		RandomShuffle(velocity.y, velocity.x);
}

void ParticleGenerator::RandomizeColor(SpriteEntity* particle)
{
	int myRandNumber = colorDistribution(randomGenerator);
	if (myRandNumber < 3)
		particle->setColor(sf::Color(200, 150, 200));
	else if (myRandNumber<7)
		particle->setColor(sf::Color(200, 200, 100));
	else if (myRandNumber < 13)
		particle->setColor(sf::Color(150, 200, 255));
}

void ParticleGenerator::GenerateBoltParticle(int frame, const Vector2D & velocity, float posX, float posY, float lifeTime, float scale)
{
    float particleScale = scale - 0.2f;

    // low particles -> lifetime / 2
    if (game().getParameters().lowParticles) lifeTime *= 0.5f;

    if (game().getParameters().particlesBatching)
    {
      // "background" particle
      // not for ice shots
      if (frame != 2)
        game().getCurrentMapEntity()->generateBoltParticle(posX, posY, velocity, true, BOLT_PRO_LINE + frame, particleScale, lifeTime);

      // "blend" particle
      // same size for illusion shots
      if (frame != 3) particleScale -= 0.1f;
      game().getCurrentMapEntity()->generateBoltParticle(posX, posY, velocity, false, BOLT_PRO_LINE * 2 + frame, particleScale, lifeTime);
    }
    else
    {
      // "background" particle
      // not for ice shots
      if (frame != 2)
      {
        SpriteEntity* particle = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_BOLT), posX, posY, BOLT_WIDTH, BOLT_HEIGHT);
        particle->setFading(true);
        particle->setImagesProLine(BOLT_PRO_LINE);
        particle->setZ(10);
        particle->setLifetime(lifeTime);
        particle->setVelocity(velocity);
        particle->setType(ENTITY_EFFECT);
        particle->setFrame(BOLT_PRO_LINE + frame);
        particle->setShrinking(true, particleScale, particleScale);
      }

      // "blend" particle
      // same size for illusion shots
      if (frame != 3) particleScale -= 0.1f;

      SpriteEntity* particle = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_BOLT), posX, posY, BOLT_WIDTH, BOLT_HEIGHT);
      particle->setFading(true);
      particle->setImagesProLine(BOLT_PRO_LINE);
      particle->setZ(11);
      particle->setLifetime(lifeTime);
      particle->setVelocity(velocity);
      particle->setType(ENTITY_EFFECT);
      particle->setFrame(BOLT_PRO_LINE * 2 + frame);
      particle->setShrinking(true, particleScale, particleScale);
      particle->setRenderAdd();
    }
}
