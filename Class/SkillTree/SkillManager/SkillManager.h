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
		

		//下層のデータ
		std::vector<SkillData>child;
	};

};


