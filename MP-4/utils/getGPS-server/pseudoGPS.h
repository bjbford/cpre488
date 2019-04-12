#ifndef PSEUDOGPS_H
#define PSEUDOGPS_H

class pseudoGPS
{
    public:
        pseudoGPS();
        ~pseudoGPS();

        double getxPos(){return xPos;}
        void setxPos(double x){xPos = x;}

        double getyPos(){return yPos;}
        void setyPos(double y){yPos = y;}

        double getzPos(){return zPos;}
        void setzPos(double z){zPos = z;}

        double getdirPos(){return dirPos;}
        void setdirPos(double d){dirPos = d;}

    protected:

    private:

        double xPos;
        double yPos;
        double zPos;
        double dirPos;

};

#endif // PSEUDOGPS_H
