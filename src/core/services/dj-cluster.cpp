#include "../include/dj-cluster.h"
#include <cmath>
services::ClusterWrapper::ClusterWrapper(vector<services::LocPointWrapper> & points){

}
services::ClusterWrapper::~ClusterWrapper(){

}
void services::ClusterWrapper::fusion(services::ClusterWrapper* other){
  this->cluster.putAll(other->cluster);
}
services::DJCluster::DJCluster(){

}
services::DJCluster::~DJCluster(){

}
vector<services::ClusterWrapper*> services::DJCluster::run(float epsilon,int minPts,services::LocPointWrapper & center, float radius){
  idCurrentClustering++;
  vector<services::ClusterWrapper*> clusters;
  double min[] = {center.point.lat-radius,center.point.lon-radius};
  double max[] = {center.point.lat+radius,center.point.lon+radius};
  vector<services::LocPointWrapper*> * points = new vector<services::LocPointWrapper*>();
  tree->Search(min,max,[=](services::LocPointWrapper*p){
    points->push_back(p);
    return true;
  });
  for(int i = 0;i<points->size();i++){
    (*points)[i]->idClustering=idCurrentClustering;
    vector<services::LocPointWrapper> neighbours;
    this->neighbours((*points)[i],epsilon,minPts,neighbours);
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
  }
  delete points;
  return clusters;
}
void services::DJCluster::load(){

}
void services::DJCluster::neighbours(services::LocPointWrapper * point,float epsilon,int minPts,vector<services::LocPointWrapper> &neighbours){
  double min[] = {point->point.lat-epsilon,point->point.lon-epsilon};
  double max[] = {point->point.lat+epsilon,point->point.lon+epsilon};
  neighbours.push_back(*point);
  float * epsilonPts= &epsilon;
  vector<services::LocPointWrapper>* nPtr= & neighbours;
  tree->Search(min,max,[=](services::LocPointWrapper*p){
    if(*epsilonPts>=sqrt(services::sqr(p->point.lat-point->point.lat)+services::sqr(p->point.lon-point->point.lon))){
      (*nPtr).push_back(*p);
    }
    return true;
  });

  if(neighbours.size()<minPts){
    neighbours.clear();
  }
}
services::ClusterWrapper* services::DJCluster::getClusterJoinable(vector<services::LocPointWrapper>& neighbours,vector<services::ClusterWrapper*>& clusters,float epsilon){
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
      tree->Search(min,max,[=](services::LocPointWrapper*p){
        if(*epsPtr>=sqrt(services::sqr(p->point.lat-clusters[i]->center.lat)+services::sqr(p->point.lon-clusters[i]->center.lon)) && neighbours[j]==*p){
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
      tree->Search(min,max,[=](services::LocPointWrapper*p){
        if(*epsPtr>=sqrt(services::sqr(p->point.lat-clusters[i]->center.lat)+
        services::sqr(p->point.lon-clusters[i]->center.lon)) && tl.loc==p->point){
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
