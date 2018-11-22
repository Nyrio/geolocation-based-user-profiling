#include "dj-cluster.h"
#include <cmath>
#include <iostream>
#include "clusters.h"
#include <ctime>

#define DEBUG_MODE 0


services::ClusterWrapper::ClusterWrapper(vector<services::TimeLocWrapper*> & points,int idCluster)
{
    this->idCluster=idCluster;
    for(uint i = 0;i<points.size();i++){
        this->cluster.insert(points[i]->point);
        this->mapRefs.insert(pair<TimeLoc,TimeLocWrapper*>(points[i]->point,points[i]));
        points[i]->idCluster = idCluster;

    }
    this->center = services::cluster_centroid(this->cluster);
}


services::ClusterWrapper::~ClusterWrapper()
{
}


void services::ClusterWrapper::fusion(services::ClusterWrapper* other)
{
    for(auto data: other->cluster)
    {
        if(this->cluster.count(data)==0)
        {
            this->cluster.insert(data);
            this->mapRefs.insert(pair<TimeLoc,TimeLocWrapper*>(data,other->mapRefs.at(data)));
            this->mapRefs.at(data)->idCluster=this->idCluster;
        }
    }
    this->center = services::cluster_centroid(this->cluster);
}


services::DJCluster::DJCluster()
{
    tree = new RTree<TimeLocWrapper*,double,2>();
}


services::DJCluster::~DJCluster()
{
    cleanTree();
    delete tree;
}


void services::DJCluster::cleanTree()
{
    RTree<TimeLocWrapper*,double,2>::Iterator iterator;
    tree->GetFirst(iterator);
    while(!tree->IsNull(iterator))
    {
        delete tree->GetAt(iterator);
        tree->GetNext(iterator);
    }
    tree->RemoveAll();
}
void services::DJCluster::reset(){
    RTree<TimeLocWrapper*,double,2>::Iterator iterator;
    tree->GetFirst(iterator);
    while(!tree->IsNull(iterator))
    {
        tree->GetAt(iterator)->idCluster=-2;
        tree->GetNext(iterator);
    }
}


vector<data::Cluster> services::DJCluster::run(const data::WorkflowParam& wp)
{
    if(DEBUG_MODE)
        cout << "run dj clustering" << endl;
    reset();
    idCurrentClustering++;
    int idCluster=0;
    vector<services::ClusterWrapper*> clusters;
    RTree<TimeLocWrapper*,double,2>::Iterator iterator;
    tree->GetFirst(iterator);
    while(!tree->IsNull(iterator))
    {
        TimeLocWrapper* point= tree->GetAt(iterator);
        if(DEBUG_MODE) cout << "point: " << point->point.t << endl;
        if(point->idCluster==-2){
            vector<services::TimeLocWrapper*> neighbours;
            this->neighbours(point, wp.eps, wp.minPts, neighbours);
            //no neighbours => noise
            if(neighbours.size()==0){
                //do nothing
                if(DEBUG_MODE) cout << "noise" << endl;
            }else {
                if(DEBUG_MODE) cout << "new cluster: " << neighbours.size() << endl;

                services::ClusterWrapper * cluster = new services::ClusterWrapper(neighbours,idCluster);
                if(this->getClusterJoinable(clusters,wp.eps)){
                    if(DEBUG_MODE) cout << "cluster joinables" << endl;
                    clusters.push_back(cluster);
                    joinClusters(clusters,wp.eps,idCluster);
                }else{
                    if(DEBUG_MODE) cout << "append cluster" << endl;
                    clusters.push_back(cluster);
                    idCluster++;
                }
            }
        }
        tree->GetNext(iterator);
    }
    vector<data::Cluster> output;
    for(uint i = 0; i < clusters.size(); i++)
    {
        output.push_back(clusters[i]->cluster);
        delete clusters[i];
    }
    return output;
}

void services::DJCluster::load(data::PointSet points)
{
    if(DEBUG_MODE) cout << "load in rtree" << endl;
    preprocessing(points);
    tree->RemoveAll();
    double min[] = {0,0};
    double max[] = {0,0};

    for(auto point: points){
        min[0] = point.loc.lat;
        min[1] = point.loc.lon;
        max[0] = point.loc.lat;
        max[1] = point.loc.lon;
        TimeLocWrapper * wrapper = new TimeLocWrapper();
        wrapper->point = point;
        wrapper->idCluster=-2;
        tree->Insert(min,max,wrapper);
    }
}

void services::DJCluster::preprocessing(data::PointSet &points)
{
}

void services::DJCluster::neighbours(
    services::TimeLocWrapper * point, float epsilon,
    uint minPts, vector<services::TimeLocWrapper*> &neighbours)
{
    double min[] = { point->point.loc.lat-epsilon,point->point.loc.lon-epsilon };
    double max[] = { point->point.loc.lat+epsilon,point->point.loc.lon+epsilon };
    float * epsilonPts= &epsilon;
    vector<services::TimeLocWrapper*>* nPtr= & neighbours;
    tree->Search(min, max,
        [=](services::TimeLocWrapper*p)
        {
            if((*epsilonPts)*(*epsilonPts)
                >= services::sqr(p->point.loc.lat-point->point.loc.lat)
                + services::sqr(p->point.loc.lon-point->point.loc.lon))
            {
                (*nPtr).push_back(p);
                if(DEBUG_MODE) cout << "is in espilon(" << epsilon << "): "
                                    << p->point.loc << endl;
            }
            return true;
        }
    );

    if(neighbours.size() < minPts){
        neighbours.clear();
    }else{
        for(TimeLocWrapper* tlw: neighbours){
            tlw->idCluster=-1;
        }
    }
}


services::ClusterWrapper* services::DJCluster::getClusterJoinable(
    vector<services::ClusterWrapper*>& clusters, float epsilon)
{
    double min[] = {0,0};
    double max[] = {0,0};
    services::ClusterWrapper* found=nullptr;
    for(uint i = 0;i<clusters.size();i++)
    {
        if(DEBUG_MODE) cout << "cluster(" << i << "): " << clusters[i]->cluster.size()
                            << " center:" << clusters[i]->center << endl;
        min[0]=clusters[i]->center.lat-epsilon;
        min[1]=clusters[i]->center.lon-epsilon;
        max[0]=clusters[i]->center.lat+epsilon;
        max[1]=clusters[i]->center.lon+epsilon;
        if(DEBUG_MODE) cout << "min: " << min[0] << " " << min[1] << endl;
        if(DEBUG_MODE) cout << "max: " << max[0] << " " << max[1] << endl;

        float * epsPtr=&epsilon;
        services::ClusterWrapper** foundPtr= &found;
        tree->Search(min, max,
            [=](services::TimeLocWrapper*p)
            {
                if(DEBUG_MODE)
                    cout << "point found: " << p->point.loc << endl;
                if((*epsPtr)*(*epsPtr)
                    >= services::sqr(p->point.loc.lat-clusters[i]->center.lat)
                    + services::sqr(p->point.loc.lon-clusters[i]->center.lon)
                    && p->idCluster==-1)//is a neighbour
                {
                    *foundPtr=clusters[i];
                    if(DEBUG_MODE) cout << "found cluster" << endl;
                    return false; //stop the search
                }
                if(DEBUG_MODE) cout << "continue"<<endl;
                return true;
            }
        );

        if(found != nullptr){
            return found;
        }

    }
    return found;
}

services::ClusterWrapper* services::DJCluster::getClusterJoinable(
    ClusterWrapper* cluster, vector<services::ClusterWrapper*>& clusters, float epsilon)
{
    double min[] = {0, 0};
    double max[] = {0, 0};
    services::ClusterWrapper* found = nullptr;
    int currentId = cluster->idCluster;
    for(uint i = 0; i<clusters.size(); i++)
    {
        min[0] = clusters[i]->center.lat-epsilon;
        min[1] = clusters[i]->center.lon-epsilon;
        max[0] = clusters[i]->center.lat+epsilon;
        max[1] = clusters[i]->center.lon+epsilon;
        float * epsPtr=&epsilon;
        services::ClusterWrapper** foundPtr= &found;
        tree->Search(min,max,
            [=](services::TimeLocWrapper*p)
            {
                if((*epsPtr)*(*epsPtr)
                    >= services::sqr(p->point.loc.lat-clusters[i]->center.lat)
                    + services::sqr(p->point.loc.lon-clusters[i]->center.lon)
                    && p->idCluster==currentId)
                {
                    *foundPtr=clusters[i];
                    return false; //stop the search
                }
                return true;
            }
        );

        if(found != nullptr){
            return found;
        }

    }
    return found;
}


void services::DJCluster::joinClusters(
    vector<services::ClusterWrapper*>& clusters,float epsilon,int & lastIdCluster)
{
    lastIdCluster=0;
    for(uint i = 0;i<clusters.size();i++)
    {
        services::ClusterWrapper *c = getClusterJoinable(
            clusters[i], clusters, epsilon);

        if(c && c != clusters[i])
        {
            c->fusion(clusters[i]);
            delete clusters[i];
            clusters[i] = clusters[clusters.size() - 1];
            clusters.pop_back();
            i--;
        }else{
            lastIdCluster++;
        }
    }
}
