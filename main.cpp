#include <iostream>
#include "Point.h"
#include "Point.cpp"
#include "Segment.h"
#include "Segment.cpp"
#include <algorithm>

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
int main() {

    return 0;
}