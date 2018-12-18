#include <iostream>
#include "Point.h"
#include "Point.cpp"
#include "Segment.h"
#include "Segment.cpp"
#include <algorithm>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <set>
using namespace std;
int dim,traj_n;
vector<vector<Point>> db;
set<Segment*> partitionDB; // 线段的集合
map<int,set<int>> cluster_cnt;
double eplison = 10;
int minLins = 20;
/*
 * 测试一下距离公式
 */

void test_dist(){
//    Point pi_s(1,1),pi_e(3,1);
//    Point pj_s(1,2),pj_e(3,4);
//    Segment si(pi_s,pi_e),sj(pj_s,pj_e);
//    cout << min(1,2);/**/
//    cout << si.perpendicular_dist(sj) << "  " << si.parallel_dist(sj) <<" "<<si.angle_dist(sj)<< endl;
}

double MDL(int traj_id, int start_index,int currIndex,string t){
    Segment se = Segment(db[traj_id][start_index],db[traj_id][currIndex],traj_id);
    double l_h = 0.0,l_d_h_l = 0.0,l_d_h_r = 0.0;
    if(t == "par") {
        l_h = log2(se.getSegmentLength());
    }
    for(int i = start_index ; i < currIndex;i++){
        Segment sub = Segment(db[traj_id][i] , db[traj_id][i + 1], traj_id);
        if(t == "par") {
            l_d_h_l += se.perpendicular_dist(sub);
            l_d_h_r += se.angle_dist(sub);
        }
        else if (t == "nopar"){
            l_h += sub.getSegmentLength();
        }
    }
    if(t == "par"){
        return l_h + log2(l_d_h_l) + log2(l_d_h_r);
    }
    else{
        return log2(l_h);
    }
}
void approximate_trajectory_partitioning(){
//    cout << "db.size: "<< db.size()<<endl;
    for(int i =0 ;i< db.size();i++) {
        vector<Point> seq = db[i];
        int start_index = 0, length = 1;
        while (start_index + length < seq.size()) {
            int currIndex = start_index + length;
            double cost_par = MDL(i,start_index, currIndex, "par");
            double cost_nopar = MDL(i,start_index, currIndex, "nopar");
//            cout << "start_index：" << start_index << "  currIndex: " << currIndex << " " << cost_par
//                 << "  par and nopar  " << cost_nopar << endl;
            if (cost_par > cost_nopar) {
                Segment *seg = new Segment(db[i][start_index],db[i][currIndex - 1],i);
                partitionDB.insert(seg);
                start_index = currIndex - 1;
            } else {
                length++;
            }
        }
    }
//    for(int i=0;i<partitioningdb.size();i++){
//        for(int j=0;j<partitioningdb[i].size();j++){
//            if(j) cout << " ";
//            cout << partitioningdb[i][j].getSegment().first.getX() <<" "<<partitioningdb[i][j].getSegment().first.getY()<<" "<<
//            partitioningdb[i][j].getSegment().second.getX() <<" "<<partitioningdb[i][j].getSegment().second.getY();
//        }
//        cout<<endl;
//    }

}

queue<Segment*> n_eplison_l(Segment seg){
    queue<Segment*> tmp;
    Segment seg_i,seg_j;
    for(Segment *seg_tmp: partitionDB){ // 取较长的
        if(seg_tmp->getSegmentLength() > seg.getSegmentLength()){
            seg_i = *seg_tmp;
            seg_j = seg;
        }
        else{
            seg_i = seg;
            seg_j = *seg_tmp;
        }
        if(seg_i.getAllDistance(seg_j) <= eplison){
//            cout <<"all distance: "<< seg_i.getAllDistance(seg_j) << " "<<endl;
            tmp.push(seg_tmp);
        }
    }
    return tmp;
}

void expand_cluster(queue<Segment*> que,int cluster_id){
    while(!que.empty()){
        Segment * front = que.front();
        que.pop();
        front->setClusterId(cluster_id);
        queue<Segment*> tmp_que = n_eplison_l(*front);
        if(tmp_que.size() >= minLins){
            while(!tmp_que.empty()){
                Segment * tmp_front = tmp_que.front();
                tmp_que.pop();
                if(tmp_front->getClusterId() == -1){
                    que.push(tmp_front);
                    tmp_front->setClusterId(cluster_id);
                }
            }
        }
    }
}
void line_segment_clustering(){
    int cluster_id = 0 ;
//    cout << "db_size: "<<partitionDB.size()<<endl;
    for(Segment *seg : partitionDB){
//        cout<< seg->getClusterId()<<" --- "<<endl;
        if(seg->getClusterId() == -1){ // 未被分类
            seg->setClusterId(cluster_id);
            queue<Segment*> que = n_eplison_l(*seg);

            if(que.size() >= minLins){
                expand_cluster(que,cluster_id);
                cluster_id ++;
            }
        }
//        cout <<cluster_id << " "<< seg->getClusterId()<<endl;
        cluster_cnt[seg->getClusterId()].insert(seg->getTrajId());
    }
//    map<int,set<int>>::iterator iter;
    set<int>remove_cluster;
    for(int i=0;i<cluster_cnt.size();i++){
        if(cluster_cnt[i].size() < minLins){
            remove_cluster.insert(i);
        }
    }
    //remove o
    for(Segment *tmp:partitionDB) {
        if (remove_cluster.find(tmp->getClusterId()) == remove_cluster.end()) {
            cout << tmp->getSegment().first.getX() << " " << tmp->getSegment().first.getY() << " "
                 << tmp->getSegment().second.getX() << " " << tmp->getSegment().second.getY() << " "
                 << tmp->getClusterId() << endl;
        }
    }
}
int main() {
//    freopen("deer1995.tra","r",stdin);
//    freopen("elk_1993.tra","r",stdin);
//    freopen("deer1995.tra","r",stdin);
//    freopen("mini_data.in","r",stdin);
    freopen("hurricane1950_2006.tra","r",stdin);
    freopen("data.out","w",stdout);
    cin>> dim >> traj_n;
    for(int i=0;i<traj_n;i++){
        int traj_id,point_num;
        cin>>traj_id>>point_num;
        vector<Point> tmp;
        for(int j =0 ;j< point_num;j++){
            double x,y;
            cin >> x >> y;
            tmp.push_back(Point(x,y,i));
        }
        db.push_back(tmp);
    }
    approximate_trajectory_partitioning();
    line_segment_clustering();
    return 0;
}