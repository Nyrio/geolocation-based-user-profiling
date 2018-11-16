#pragma once
#include <vector>
#include "RTree.h"
#include "datatypes.h"

using namespace std;
using namespace data;

  namespace services{
    inline double sqr(double a){
      return a*a;
    }
    typedef struct{
      LocPoint point;
      long idClustering;
    }LocPointWrapper;
    inline bool operator==(const LocPointWrapper& lp1, const LocPointWrapper& lp2){
      return (lp1.point==lp2.point);
    }
    class ClusterWrapper{

    public:
      Cluster cluster;
      LocPoint center;
      ClusterWrapper(vector<LocPointWrapper>& points);
      void fusion(ClusterWrapper * other);
      virtual ~ClusterWrapper();
    };

    class DJCluster{
    public:
      DJCluster();
      virtual ~DJCluster();
      void load();
      /**
      *no noise in clusters
      */
      vector<ClusterWrapper*> run(float epsilon,int minPts,LocPointWrapper & center, float radius);
    private:
      void neighbours(LocPointWrapper * point,float epsilon,int minPts,vector<LocPointWrapper>& neighbours);
      ClusterWrapper* getClusterJoinable(vector<services::LocPointWrapper>& neighbours,vector<services::ClusterWrapper*>& clusters,float epsilon);
      ClusterWrapper* getClusterJoinable(Cluster& neighbours,vector<services::ClusterWrapper*>& clusters,float epsilon);

      void joinClusters(vector<ClusterWrapper*>& clusters,float epsilon);
      RTree<LocPointWrapper*,double,2> * tree;
      long idCurrentClustering=-1;

    };
}
