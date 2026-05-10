#pragma once
class Person
{
private:
	int id;
	char name[20];
	char password[20];
public:
	virtual void displaymenu() = 0;
	Person();
	Person(int id, const char* n, const char* p);
    const char* getName();
	int getid();
	const char* getpass();
};
