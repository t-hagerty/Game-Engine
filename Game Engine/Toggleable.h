#pragma once
class Toggleable
{
public:
	Toggleable();
	~Toggleable();
	virtual void toggle() = 0;
};

