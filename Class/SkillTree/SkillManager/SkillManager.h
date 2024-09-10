#pragma once
#include<Adapter.h>
#include"../ISkill/ISkill.h"

class SkillManager {

public:
	SkillManager();
	~SkillManager();

	void Initialize();

	void Update();

private:

	struct SkillData
	{
		

		//‰º‘w‚Ìƒf[ƒ^
		std::vector<SkillData>child;
	};

};


