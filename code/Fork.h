#pragma once
#include <iostream>

class Fork{

int id;
bool available = true;

public:

bool getAvailable();
void setAvailable(bool _available);
Fork();
Fork(int _id);
int getID();

};
