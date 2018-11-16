#include "../include/dj-cluster.h"
#include <cmath>
#include "../include/clusters.h"

services::ClusterWrapper::ClusterWrapper(vector<services::TimeLocWrapper> & points){
  for(int i = 0;i<points.size();i++){
    this->cluster.insert(points[i].point);
  }
}
services::ClusterWrapper::~ClusterWrapper(){
}
void services::ClusterWrapper::fusion(services::ClusterWrapper* other){
  for(auto data: other->cluster){
    this->cluster.insert(data);
  }
  this->center=services::cluster_centroid(other->cluster);
}
services::DJCluster::DJCluster(){
  tree = new RTree<TimeLocWrapper*,double,2>();
}
services::DJCluster::~DJCluster(){
  cleanTree();
  delete tree;
}
void services::DJCluster::cleanTree(){
  RTree<TimeLocWrapper*,double,2>::Iterator iterator;
  tree->GetFirst(iterator);
  while(!tree->IsNull(iterator)){
    delete tree->GetAt(iterator);
    tree->GetNext(iterator);
  }
  tree->RemoveAll();
}
vector<services::ClusterWrapper*> services::DJCluster::run(float epsilon,int minPts){
  idCurrentClustering++;
  vector<services::ClusterWrapper*> clusters;
  RTree<TimeLocWrapper*,double,2>::Iterator iterator;
  tree->GetFirst(iterator);
  while(!tree->IsNull(iterator)){
    TimeLocWrapper* point= tree->GetAt(iterator);
    point->idClustering=idCurrentClustering;
    vector<services::TimeLocWrapper> neighbours;
    this->neighbours(point,epsilon,minPts,neighbours);
    //no neighbours => noise
    if(neighbours.size()==0){
      //do nothing
    }else {
      services::ClusterWrapper * cluster = new services::ClusterWrapper(neighbours);
      clusters.push_back(cluster);
      if(this->getClusterJoinable(neighbours,clusters,epsilon)){
        joinClusters(clusters,epsilon);
      }
    }
    tree->GetNext(iterator);
  }

  return clusters;
}
void services::DJCluster::load(PointSet points){
  preprocessing(points);
  tree->RemoveAll();
  double min[] = {0,0};
  double max[] = {0,0};

  for(auto point: points){
    min[0]=point.loc.lat;
    min[1]=point.loc.lon;
    max[0]=point.loc.lat;
    max[1]=point.loc.lon;
    TimeLocWrapper * wrapper = new TimeLocWrapper();
    wrapper->point = point;
    tree->Insert(min,max,wrapper);
  }
}
void services::DJCluster::preprocessing(PointSet &points){

}
void services::DJCluster::neighbours(services::TimeLocWrapper * point,float epsilon,int minPts,vector<services::TimeLocWrapper> &neighbours){
  double min[] = {point->point.loc.lat-epsilon,point->point.loc.lon-epsilon};
  double max[] = {point->point.loc.lat+epsilon,point->point.loc.lon+epsilon};
  neighbours.push_back(*point);
  float * epsilonPts= &epsilon;
  vector<services::TimeLocWrapper>* nPtr= & neighbours;
  tree->Search(min,max,[=](services::TimeLocWrapper*p){
    if((*epsilonPts)*(*epsilonPts)>=services::sqr(p->point.loc.lat-point->point.loc.lat)+services::sqr(p->point.loc.lon-point->point.loc.lon)){
      (*nPtr).push_back(*p);
    }
    return true;
  });

  if(neighbours.size()<minPts){
    neighbours.clear();
  }
}
services::ClusterWrapper* services::DJCluster::getClusterJoinable(vector<services::TimeLocWrapper>& neighbours,vector<services::ClusterWrapper*>& clusters,float epsilon){
  double min[] = {0,0};
  double max[] = {0,0};
  services::ClusterWrapper* found=nullptr;
  for(int i = 0;i<clusters.size();i++){
    min[0]=clusters[i]->center.lat-epsilon;
    min[1]=clusters[i]->center.lon-epsilon;
    max[0]=clusters[i]->center.lat+epsilon;
    max[1]=clusters[i]->center.lon+epsilon;
    float * epsPtr=&epsilon;
    services::ClusterWrapper** foundPtr= &found;
    for(int j = 0;j<neighbours.size();j++){
      tree->Search(min,max,[=](services::TimeLocWrapper*p){
        if((*epsPtr)*(*epsPtr)>=services::sqr(p->point.loc.lat-clusters[i]->center.lat)+services::sqr(p->point.loc.lon-clusters[i]->center.lon) && neighbours[j]==*p){
          *foundPtr=clusters[i];
          return false;//stop the search
        }
        return true;
      });

      if(found!=nullptr){
        return found;
      }
    }
  }
  return found;
}
services::ClusterWrapper* services::DJCluster::getClusterJoinable(Cluster& neighbours,vector<services::ClusterWrapper*>& clusters,float epsilon){
  double min[] = {0,0};
  double max[] = {0,0};
  services::ClusterWrapper* found=nullptr;
  for(int i = 0;i<clusters.size();i++){
    min[0]=clusters[i]->center.lat-epsilon;
    min[1]=clusters[i]->center.lon-epsilon;
    max[0]=clusters[i]->center.lat+epsilon;
    max[1]=clusters[i]->center.lon+epsilon;
    float * epsPtr=&epsilon;
    services::ClusterWrapper** foundPtr= &found;
    for(TimeLoc tl : neighbours){
      tree->Search(min,max,[=](services::TimeLocWrapper*p){
        if((*epsPtr)*(*epsPtr)>=services::sqr(p->point.loc.lat-clusters[i]->center.lat)+
        services::sqr(p->point.loc.lon-clusters[i]->center.lon) && tl.loc==p->point.loc){
          *foundPtr=clusters[i];
          return false;//stop the search
        }
        return true;
      });

      if(found!=nullptr){
        return found;
      }
    }
  }
  return found;
}
void services::DJCluster::joinClusters(vector<services::ClusterWrapper*>& clusters,float epsilon){
  for(int i = 0;i<clusters.size();i++){
    services::ClusterWrapper *c=getClusterJoinable(clusters[i]->cluster,clusters,epsilon);
    if(c){
      c->fusion(clusters[i]);
      delete clusters[i];
      clusters[i]=clusters[clusters.size()-1];
      clusters.pop_back();
      i--;
    }

  }
}
