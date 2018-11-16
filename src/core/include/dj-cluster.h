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
      TimeLoc point;
      long idClustering;
    }TimeLocWrapper;
    inline bool operator==(const TimeLocWrapper& lp1, const TimeLocWrapper& lp2){
      return (lp1.point.loc==lp2.point.loc);
    }
    class ClusterWrapper{

    public:
      Cluster cluster;
      LocPoint center;
      ClusterWrapper(vector<TimeLocWrapper>& points);
      void fusion(ClusterWrapper * other);
      virtual ~ClusterWrapper();
    };

    class DJCluster{
    public:
      DJCluster();
      virtual ~DJCluster();
      void load(PointSet points);
      /**
      *no noise in clusters
      */
      vector<ClusterWrapper*> run(float epsilon,int minPts,TimeLocWrapper & center, float radius);
    private:
      void neighbours(TimeLocWrapper * point,float epsilon,int minPts,vector<TimeLocWrapper>& neighbours);
      ClusterWrapper* getClusterJoinable(vector<services::TimeLocWrapper>& neighbours,vector<services::ClusterWrapper*>& clusters,float epsilon);
      ClusterWrapper* getClusterJoinable(Cluster& neighbours,vector<services::ClusterWrapper*>& clusters,float epsilon);

      void joinClusters(vector<ClusterWrapper*>& clusters,float epsilon);
      RTree<TimeLocWrapper*,double,2> * tree;
      long idCurrentClustering=-1;

    };
}
