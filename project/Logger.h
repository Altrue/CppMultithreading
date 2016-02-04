#pragma once

#include <iostream>

class Logger
{
private:
	void insert_log(std::string p_content, int p_level);
	int log_level;

public:
	Logger();
	~Logger();

	void log_info(std::string p_content);
	void log_warn(std::string p_content);
	void log_alert(std::string p_content);
	void log_error(std::string p_content);
	void set_log_level(int p_level);
};

