//
// Created by 张开顺 on 2018/12/13.
//

#ifndef TRACLUS_POINT_H
#define TRACLUS_POINT_H
#include <iostream>
#include <ostream>
class Point{
    private:
        double x;
        double y;
        int traj_id;
    public:
        Point();
        Point(double x,double y,int traj_id);
        double const getX();
        double const getY();
        std::pair<double,double>getPoint();
        double const dist(Point p);
        // operator
        friend const Point operator + (const Point &a, const Point &b);
        friend const Point operator - (const Point &a, const Point &b);
        friend const Point operator * (const double &a,const Point &p);
        friend const Point operator * (const Point &p,const double &a);
        friend const Point operator / (const Point &p,const double &a);
        double const dot(const Point &p);
        friend const bool operator == (const Point &p1,const Point &p2);
//        std::ostream& operator<<(std::ostream& cout,const Point& p)
//        {
//            cout << p.x << " "<< p.y << std::endl;
//            return cout;
//        }
        ~Point();

};
#endif //TRACLUS_POINT_H
