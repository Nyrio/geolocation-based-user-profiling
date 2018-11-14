#include "../include/dj-cluster.h"
services::DJCluster::DJCluster(){

}
services::DJCluster::~DJCluster(){

}
vector<clusterTmp*> services::DJCluster::run(float epsilon,int minPts,LocPoint & center, float radius){
  idCurrentClustering++;
  vector<clusterTmp*> clusters;
  double min[] = {center->lat-radius,center->longi-radius};
  double max[] = {center->lat+radius,center->longi+radius};
  vector<LocPoint*> points;
  tree->Search(min,max,[=](LocPoint*p){
    points.add(p);
    return true;
  });
  for(int i = 0;i<;i++){
    p[i]->idClustering=idCurrentClustering;
    vector<LocPoint> neighbours;
    neighbours(p[i],epsilon,minPts,neighbours);
    //no neighbours => noise
    if(neighbours.size()==0){
      //do nothing
    }else {
      //TODO do constructor
      //clusterTmp * cluster = new clusterTmp(neighbours);
      //clusters.push_back(cluster);
      if(getClusterJoinable(neighbours,clusters)){
        joinClusters(clusters);
      }
    }
  }
  return clusters;
}
void services::DJCluster::load(){

}
void services::DJCluster::neighbours(LocPoint * point,float epsilon,int minPts,vector<LocPoint> &neighbours){
  double min[] = {point->lat-epsilon,point->longi-epsilon};
  double max[] = {point->lat+epsilon,point->longi+epsilon};
  neighbours.push_back(*point);
  tree->Search(min,max,[=](LocPoint*p){
    if(espilon>=math::sqrt(math::pow(p->lat-point->lat)+math::pow(p->longi-point->longi))){
      neighbours.push_back(*p);
    }
    return true;
  });

  if(neighbours.size()<minPts){
    neighbours.clear();
  }
}
clusterTmp* services::DJCluster::getClusterJoinable(vector<LocPoint>& neighbours,vector<clusterTmp*>& clusters,float epsilon){
  double min[] = {0,0};
  double max[] = {0,0};
  clusterTmp* found=nullptr;
  for(int i = 0;i<clusters.size();i++){
    found=false;
    min[0]=clusters[i]->center.lat-epsilon;
    min[1]=clusters[i]->center.longi-epsilon;
    max[0]=clusters[i]->center.lat+epsilon;
    max[1]=clusters[i]->center.longi+epsilon;
    for(int j = 0;j<neighbours.size();j++){
      tree->Search(min,max,[=](LocPoint*p){
        if(espilon>=math::sqrt(math::pow(p->lat-clusters[i]->center.lat)+math::pow(p->longi-clusters[i]->center.longi)) && neighbours[j]==*p){
          found=clusters[i];
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
void services::DJCluster::joinClusters(vector<clusterTmp*>& clusters){
  for(int i = 0;i<clusters.size();i++){
      clusterTmp *c=getClusterJoinable(clusters[i]->points,clusters);
      if(c){
        //TODO do the function in cluster class
        //c->fusion(clusters[i]);
        delete clusters[i];
        clusters[i]=clusters[clusters.size()-1];
        clusters.pop_back();
        i--;
      }

    }
  }
}
