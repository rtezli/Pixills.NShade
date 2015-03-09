#pragma once
class Global
{
public:
	static Global& instance()
	{
		static Global _instance;
		return _instance;
	}
	~Global() {}
private:
	Global() {}
	Global(const Global&);
	Global & operator = (const Global &);
	static Global* _instance;
};

