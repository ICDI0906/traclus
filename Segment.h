//
// Created by 张开顺 on 2018/12/17.
//

#ifndef TRACLUS_SEGMENT_H
#define TRACLUS_SEGMENT_H
#include "Point.h"
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <math.h>
class Segment{
private:
    Point s,e;
public:
    Segment(){}
    Segment(Point p1,Point p2);
    std::pair<Point,Point> getSegment();
    double const perpendicular_dist(Segment seg); // 两个线段的垂直距离
    double const parallel_dist(Segment seg);
    double const angle_dist(Segment seg);
    double getSegmentLength();
};
#endif //TRACLUS_SEGMENT_H
