#include "ParticleManager.h"

void ParticleManager::Init() {
	ceilingFragment_.model.LoadShortPath("Debris/Debris.gltf");
}

void ParticleManager::Ceiling(LWP::Math::Vector3 pos) {
	ceilingFragment_.Add(10, pos);
}
