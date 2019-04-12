#ifndef FRUIT_H
#define FRUIT_H

#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <string>

using namespace std;
using namespace cv;

class Fruit
{
  public:
    Fruit();
    ~Fruit();

    double Area;

    int getxPos();
    void setxPos(int x);

    int getyPos();
    void setyPos(int y);

    Scalar getHSVmin();
    Scalar getHSVmax();

    void setHSVmin(Scalar min);
    void setHSVmax(Scalar max);

    double getArea(){return Area;}
    void setArea(double a){Area = a;}

  protected:

  private:

    int xPos, yPos; // X and Y position of Fruit

    string type;
    Scalar HSVmin, HSVmax;

};

#endif // FRUIT_H
