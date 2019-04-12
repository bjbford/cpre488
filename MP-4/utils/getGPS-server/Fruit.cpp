#include "Fruit.h"

Fruit::Fruit()
{
    //ctor
}

Fruit::~Fruit()
{
    //dtor
}


int Fruit::getxPos()
{
    return Fruit::xPos;
}

void Fruit::setxPos(int x)
{
  Fruit::xPos = x;
}

int Fruit::getyPos()
{
    return Fruit::yPos;
}

void Fruit::setyPos(int y)
{
  Fruit::yPos = y;
}

Scalar Fruit::getHSVmin()
{
  return Fruit::HSVmin;
}

Scalar Fruit::getHSVmax()
{
  return Fruit::HSVmax;
}

void Fruit::setHSVmin(Scalar min)
{
  Fruit::HSVmin = min;
}

void Fruit::setHSVmax(Scalar max)
{
  Fruit::HSVmax = max;
}

