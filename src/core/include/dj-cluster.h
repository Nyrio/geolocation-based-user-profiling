#pragma once
#include <vector>
#include "./RTree.h"
#include "./LocPoint.h"

using namespace std;
  namespace services{
    struct clusterTmp{
      LocPoint center;
      multiset<LocPoint> points;

    }
    class DJCluster{
    public:
      DJCluster();
      virtual ~DJCluster();
      void load();
      /**
      *no noise in clusters
      */
      vector<clusterTmp*> run(float epsilon,int minPts,LocPoint & center, float radius);
    private:
      void neighbours(LocPoint * point,float epsilon,int minPts,vector<LocPoint> neighbours,vector<clusterTmp*> clusters);
      bool isDensityJoinable(vector<LocPoint*> neighbours,vector<clusterTmp*> clusters);
      void getClusterJoinable(vector<cluster*> clusters);
      RTree * tree;
      long idCurrentClustering=-1;

    };
}
