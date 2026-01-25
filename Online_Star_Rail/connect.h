#pragma once

int start_socket();

class SQL {
public:
		static void init();
		static void close();
};
class Connestion {
public:
    Connestion();
	~Connestion();
};
