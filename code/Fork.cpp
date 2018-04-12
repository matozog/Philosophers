#include "Fork.h"

Fork::Fork(int _id)
{
	id = _id;
}

Fork::Fork()
{

}

int Fork::getID()
{
	return id;
}

bool Fork::getAvailable()
{
	return available;
}

void Fork::setAvailable(bool _available)
{
	available = _available;
}
