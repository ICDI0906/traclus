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
vector<vector<Point*>> representive_point;
vector<vector<Segment*>> seg_global;
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
    cout << partitionDB.size() << "size :" <<endl;
}

set<Segment*> n_eplison_l(Segment seg){
    set<Segment*> tmp;
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
            cout <<"all distance: "<< seg_i.getAllDistance(seg_j) << " "<<endl;
            tmp.insert(seg_tmp);
        }
    }
    return tmp;
}

void expand_cluster(queue<Segment*> que,int cluster_id){
    cout<< "start  -------- "<<endl;
    while(!que.empty()){

        Segment * front = que.front();
        que.pop();
        set<Segment*> tmp_set = n_eplison_l(*front);
        if(tmp_set.size() -1 >= minLins){
            for(Segment *seg: tmp_set){
                if(seg->getClusterId() == -1){
                    que.push(seg);
                    seg->setClusterId(cluster_id);
                }
            }
        }
    }
    cout<< "end  -------- "<<endl;
}
struct cmp{
    bool operator()( Point p1, Point p2){
        if(p1.getX() != p2.getX()){
            return p1.getX() < p2.getX();
        }
        else{
            return p1.getY() < p2.getY();
        }
    }
};

void representative_trajectory_generation(){
    representive_point.resize(seg_global.size());
    for(int i = 0; i < seg_global.size() ; i++){

        int point_size = seg_global[i].size() * 2;
        Point *sort_point = new Point[point_size];
        Point *rep_point = new Point(0,0,-1),*zero_point = new Point(1,0,-1);
        for(int j = 0;j < seg_global[i].size();j++) {
            *rep_point = *rep_point + (seg_global[i][j]->getSegment().second - seg_global[i][j]->getSegment().first);
        }
        *rep_point = *rep_point / seg_global[i].size();
        double cos_theta = rep_point->dot(*zero_point) / rep_point->dist(Point(0,0,-1)); // cos(theta)
        double sin_theta = sqrt(1 - pow(cos_theta , 2)); // sin(theta)

        for(int j = 0;j < seg_global[i].size();j++){
            Point s =  seg_global[i][j]->getSegment().first,e =  seg_global[i][j]->getSegment().second;
            seg_global[i][j]->setSegment(Point(s.getX() * cos_theta + s.getY() * sin_theta, s.getY() * cos_theta - s.getX() * sin_theta, -1),
                                  Point(e.getX() * cos_theta + e.getY() * sin_theta, e.getY() * cos_theta - e.getX() * sin_theta, -1));
            sort_point[2 * j] = seg_global[i][j]->getSegment().first;
            sort_point[2 * j + 1] = seg_global[i][j]->getSegment().second;
        }
        // sort by x
        sort(sort_point,sort_point + point_size,cmp());

//        for(int i=0;i<point_size;i++){
//            cout << sort_point[i].getX() << " "<< sort_point[i].getY() << endl;
//        }
        for(int p=0; p < point_size * 2; p++){ //
            int intersect_cnt = 0;
            Point *y = new Point(0, 0 , -1);
            for(int q =0 ;q < seg_global[i].size(); q++){
                Point s = seg_global[i][q]->getSegment().first,e = seg_global[i][q]->getSegment().second;
                if(sort_point[p].getX() <= e.getX() && sort_point[p].getX() >= s.getX()){ // 在一个线段的中间
                    // 计算该点在线段上的点
                    if(s.getX() == e.getX()){
                        continue;
                    }
                    else if(s.getY() == e.getY()){
                        intersect_cnt ++;
                        *y = *y + Point(sort_point[p].getX(), s.getY(), -1); //
                    }
                    else{
                        intersect_cnt ++;
                        *y = *y + Point(sort_point[p].getX(),(e.getY() - s.getY()) / (e.getX() - s.getX()) * (sort_point[p].getX() - s.getX()) + s.getY(), -1);
                    }
                }
            }
            if(intersect_cnt >= minLins){ // 取均值
                *y = *y / intersect_cnt;
                Point *tmp = new Point(y->getX() * cos_theta - sin_theta * y->getY(), sin_theta * y->getX() + cos_theta * y->getY(),-1);
                representive_point[i].push_back(tmp);
            }
//            delete y;
        }
//        delete []sort_point;
//        delete rep_point,zero_point;
    }
    cout <<"representive_size : "<<representive_point.size() << endl;
    for(int i=0;i<representive_point.size();i++){
        int len = ((int)representive_point[i].size()) - 1;
        for(int j = 0;j< len ;j++){
//            if (j) cout << " ";
            cout << representive_point[i][j]->getX() << " "<< representive_point[i][j]->getY() << " ";
            cout << representive_point[i][j + 1]->getX() << " "<< representive_point[i][j + 1]->getY() << " " << i <<endl;
        }
//        cout << endl;
    }
}
void line_segment_clustering(){
    int cluster_id = 0;
    cout << "db_size: "<<partitionDB.size()<<endl;
    for (Segment *seg : partitionDB) {
        queue<Segment*> que;
        while(!que.empty()) que.pop();
        if (seg->getClusterId() == -1) { // 未被分类
            set<Segment *> se = n_eplison_l(*seg);
            if ((int)se.size() -1 >= minLins) { // include itself
                for(Segment *seg1 : se){
                    seg1->setClusterId(cluster_id);
                    if(seg1 != seg){
                        que.push(seg1);
                    }
                }
                expand_cluster(que, cluster_id);
                cluster_id++;
            }

        }
        cluster_cnt[seg->getClusterId()].insert(seg->getTrajId());
    }
    set<int> remove_cluster;
    for (int i = 0; i < cluster_cnt.size(); i++) {
        if (cluster_cnt[i].size() < minLins) {
            remove_cluster.insert(i);
        }
    }
    cout << "cluster_id : "<< cluster_id<<"remove_cluster: " << remove_cluster.size() << endl;
//    map<int,set<int>> ::iterator it;
//    for(it = cluster_cnt.begin();it != cluster_cnt.end() ;it++){
//        cout << it->first<<endl;
//    }
    seg_global.resize(cluster_id);
    int cluster_cnt = 0;
    map<int, int> map_cluster;
    //remove o
    try {
        for (Segment *tmp:partitionDB) {
            if (remove_cluster.find(tmp->getClusterId()) == remove_cluster.end()) {
                cout << tmp->getSegment().first.getX() << " " << tmp->getSegment().first.getY() << " "
                     << tmp->getSegment().second.getX() << " " << tmp->getSegment().second.getY() << " "
                     << tmp->getClusterId() << endl;
                int id = tmp->getClusterId(); // cluster_id
                if (map_cluster.find(id) == map_cluster.end()) {  // not in
                    map_cluster.insert(make_pair(id,cluster_cnt));
                    seg_global[cluster_cnt].push_back(tmp);
                    cluster_cnt++;
                } else {
                    seg_global[map_cluster[id]].push_back(tmp);
                }
            }
        }
    }
    catch(exception e){
        cout << " --------------------- "<<endl;
        cout <<e.what()<<endl;
    }


//    cout << "seg.size : "<< seg_global.size()<< endl;
    representative_trajectory_generation();
}

int main() {
//    freopen("deer1995.tra","r",stdin);
//    freopen("elk_1993.tra","r",stdin);
//    freopen("deer1995.tra","r",stdin);
//    freopen("mini_data.in","r",stdin);
    freopen("hurricane1950_2006.tra","r",stdin);
//    freopen("data.out","w",stdout);
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