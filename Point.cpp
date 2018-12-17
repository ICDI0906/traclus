//
// Created by 张开顺 on 2018/12/13.
//
#include "Point.h"
#include <math.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>

Point::Point() {

}
Point::Point(double a, double b,int traj_id) {
    this->x = a;     // this should be ->
    this->y = b;
    this->traj_id = traj_id;
}
double const Point::dist(Point p){
    return sqrt(pow(x - p.x,2) + pow(y - p.y,2));
}
Point::~Point() {

}
double Point::getX() {
    return x;
}
double Point::getY() {
    return y;
}
std::pair<double ,double> Point::getPoint() {
    return std::make_pair(x,y);
}
Point const operator + (const Point &a, const Point &b) {

    return Point(a.x + b.x, a.y + b.y,-1);
}

Point const operator - (const Point &a, const Point &b) {

    return Point(a.x - b.x, a.y - b.y,-1);
}

Point const operator * (const double &a,const Point &p){
    return Point(p.x * a, p.y* a,-1);
}


Point const operator * (const Point &p,const double &a){
    return Point(p.x * a, p.y* a,-1);
}

bool const operator == (const Point &p1,const Point &p2){
    return (p1.x == p2.x) && (p2.y == p2.y);
}

double const Point::dot(const Point &p){
    return x * p.x + y * p.y;
}