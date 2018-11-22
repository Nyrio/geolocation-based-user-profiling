#pragma once
#include <vector>
#include "RTree.h"
#include "datatypes.h"
#include "workflow.h"

using namespace std;
using namespace data;

namespace services
{
    inline double sqr(double a)
    {
        return a*a;
    }

    // wrapper for TimeLoc (used to add attributes to timeloc)
    typedef struct{
        TimeLoc point;
    } TimeLocWrapper;

    inline bool operator==(const TimeLocWrapper& lp1,
                           const TimeLocWrapper& lp2)
    {
        return (lp1.point.loc == lp2.point.loc);
    }

    // wrapper for cluster, adds a center
    class ClusterWrapper
    {
    public:
        Cluster cluster;
        LocPoint center;

        // create a cluster from a list of timeloc
        ClusterWrapper(vector<TimeLocWrapper>& points);

        // adds all not-already in timelocs from the cluster other in this
        void fusion(ClusterWrapper * other);

        virtual ~ClusterWrapper();
    };

/**
*class containing the struct for dj-cluster
*/
    class DJCluster
    {
    public:
        DJCluster();
        virtual ~DJCluster();

        // loads data into the rtree
        void load(data::PointSet points);

        // run the algo, returns a list with the clusters found
        // no noise cluster in the list
        vector<Cluster> run(const data::WorkflowParam& wp);

    private:
        // function to preprocess data before the algorithm
        // it's O(n) though
        void preprocessing(data::PointSet &points);

        // reset the tree
        void cleanTree();

        // gets the neighbours of point, within epsilon and with a minimum
        // of minPts neighbours
        void neighbours(TimeLocWrapper * point, float epsilon, uint minPts,
                        vector<TimeLocWrapper>& neighbours);

        // gets a cluster joinable with the neighbours list (aka each one share at least a point)
        ClusterWrapper* getClusterJoinable(vector<services::TimeLocWrapper>& neighbours,
                                           vector<services::ClusterWrapper*>& clusters,
                                           float epsilon);

        // gets a cluster joinable with the cluster (aka each one share at least a point)
        ClusterWrapper* getClusterJoinable(Cluster& cluster,
            vector<services::ClusterWrapper*>& clusters,float epsilon);

        // joins all the clusters
        void joinClusters(vector<ClusterWrapper*>& clusters,float epsilon);


        /*
           Attributes
        */

        // rtree structure to increase the speed of the algo
        RTree<TimeLocWrapper*,double,2> * tree;

        // id of the current application of the algorithm
        long idCurrentClustering=-1;
    };
}
