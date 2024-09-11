#pragma once
#include"../IUpgrade/IUpgrade.h"

class ActiveJump : public IUpgrade {

	void Initialize() override {};

	void Apply(Player* player_, Drone* drone)override { player_->parameters_.activeFlag.jump = true; }

};