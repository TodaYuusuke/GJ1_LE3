#pragma once
#include<Adapter.h>


class SkillTree {
public:

	SkillTree();
	~SkillTree();

	void Initialize();

	void Update();

private:

	enum BaseSkilllType {
		Body,
		SG,
		Drone,
		_countBaseSkill
	};

	//スキルデータ
	struct SkillData {
		LWP::Resource::RigidModel model;
		bool isGet=false;

		std::vector<SkillData>data;
	};

	LWP::Object::TransformQuat baseposition_;

	SkillData datas_[_countBaseSkill];
};