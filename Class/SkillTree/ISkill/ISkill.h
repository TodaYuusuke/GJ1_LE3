#pragma once


class ISkill {

public:

	ISkill() = default;
	~ISkill() = default;

	virtual void Initialize()=0;

	virtual void Update() = 0;
public:

	bool isUsed_ = false;

};