#include <iostream>
#include "Point.h"
#include "Point.cpp"
#include "Segment.h"
#include "Segment.cpp"
#include <algorithm>
#include <string>
#include <vector>
using namespace std;
/*
 * 测试一下距离公式
 */
void test_dist(){
    Point pi_s(1,1),pi_e(3,1);
    Point pj_s(1,2),pj_e(3,4);
    Segment si(pi_s,pi_e),sj(pj_s,pj_e);
//    cout << min(1,2);/**/
    cout << si.perpendicular_dist(sj) << "  " << si.parallel_dist(sj) <<" "<<si.angle_dist(sj)<< endl;
}
vector<Point> seq;
double MDL(int start_index,int currIndex,string t){
    Segment se = Segment(seq[start_index],seq[currIndex]);
    double l_h = 0.0,l_d_h = 0.0;
    if(t == "par") {
        l_h = log2(se.getSegmentLength());
    }
    for(int i = start_index ; i < currIndex;i++){
        Segment sub = Segment(seq[i], seq[i + 1]);
        if(t == "par") {
            l_d_h += se.perpendicular_dist(sub);
            l_d_h += se.angle_dist(sub);
        }
        else if (t == "nopar"){
            l_h += sub.getSegmentLength();
        }
    }
    if(t == "par"){
        return l_h + l_d_h;
    }
    else{
        return l_h;
    }
}
void approximate_trajectory_partitioning(){
    int start_index = 0,length = 1;
    cout << "0"<<endl;
    while(start_index + length < seq.size()){
        int currIndex = start_index + length;
        double cost_par = MDL(start_index,currIndex,"par");
        double cost_nopar = MDL(start_index,currIndex,"nopar");
        cout << "start_index：" <<start_index << "  currIndex: "<< currIndex <<" "<<cost_par << "  par and nopar  "<< cost_nopar<<endl;
        if(cost_par > cost_nopar){
            cout << "index : "<< currIndex - 1 << endl;
            start_index = start_index + length;
        }
        else{
            length ++;
        }
    }
}
int main() {
    seq.push_back(Point(1,1));
    seq.push_back(Point(2,2));
    seq.push_back(Point(3,2.5));
    seq.push_back(Point(4,1));
    seq.push_back(Point(5,0.5));
    approximate_trajectory_partitioning();
    return 0;
}