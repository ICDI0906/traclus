//
// Created by 张开顺 on 2018/12/17.
//

#include "Segment.h"
const double eps = 1e-12;
Segment::Segment(Point p1, Point p2, int traj_id) {
    s = p1;e = p2;cluster_id = -1;
    this->traj_id = traj_id;
}
std::pair<Point, Point> Segment::getSegment() {
    return std::make_pair(s,e);
}

void const Segment::setClusterId(int cluster_id){
    this->cluster_id = cluster_id;
}

int const Segment::getClusterId(){
    return this->cluster_id;
}

double const Segment::perpendicular_dist(Segment seg) {
    //
    Point tmp = (seg.s - s);
    double u1 = tmp.dot(e - s) / pow(e.dist(s), 2);
    Point ps = s + u1 * (e - s);
    tmp = seg.e - s;
    double u2 = tmp.dot(e - s) / pow(e.dist(s) , 2);
    Point pe = s + u2 * (e - s);
    double l1 = seg.s.dist(ps);
    double l2 = seg.e.dist(pe);
    if (l1 < eps && l2 < eps){
        return 0;
    }
    else {
        return (pow(l1, 2) + pow(l2, 2)) / (l1 + l2);
    }
}
double const Segment::parallel_dist(Segment seg) {
    //
    Point tmp = (seg.s - s);
    double u1 = tmp.dot(e - s) / pow(e.dist(s), 2);
    Point ps = s + u1 * (e - s);
    tmp = seg.e - s;
    double u2 = tmp.dot(e - s) / pow(e.dist(s) , 2);
    Point pe = s + u2 * (e - s);
    double l1 = s.dist(ps);
    double l2 = e.dist(pe);
    return std::min(l1,l2); // min is in namespace std;
}
double const Segment::angle_dist(Segment seg) {
    //
    Point tmp = e - s;

    double cos_theta = tmp.dot(seg.e - seg.s) / (e.dist(s) * seg.e.dist(seg.s));
    if (cos_theta > eps){ // theta < 90
        return seg.getSegmentLength() * sqrt(1 - pow(cos_theta ,2));
    }
    else{
        return seg.getSegmentLength();
    }
}
double Segment::getSegmentLength() {
    return sqrt(pow(s.getX() - e.getX(), 2) +pow(s.getY() - e.getY(), 2));
}

double const Segment::getAllDistance(Segment seg){
    double result = this->angle_dist(seg);// 论文中定义两个相连线段之间的平行距离为0
    if(!(this->getSegment().second == seg.getSegment().first || this->getSegment().first == seg.getSegment().second)){
//        std::cout << "in parallel_dist"<<std::endl;
        result += this->parallel_dist(seg);
    }
    if(this->getTrajId()!=seg.getTrajId()){
//        std::cout << this->getTrajId() <<" "<< seg.getTrajId() <<std::endl;
        result += this->perpendicular_dist(seg);
    }
    return  result;
}
int const Segment::getTrajId(){
    return this->traj_id;
}
void const Segment::setSegment(Point p, Point q) {
    this->s = p;this->e = q;
}