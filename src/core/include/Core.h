#pragma once

#include "datatypes.h"

#include "workflow.h"

namespace services
{
	class Core
	{
	public:
		Core(std::string parameterFile = "");
		~Core();

		void clusterize(uint id, time_t t1 = 0, time_t t2 = 0);

		pair<data::Cluster,data::Cluster> find_place_hour_range(int h1, int h2);
		
		void analyze_tags();
		void print_house();
		void print_work();

		void show_clusters();

		/*
			Benchmarks
		*/
		void benchmark_clustering(uint id, time_t t1, time_t t2,
	                              uint nbmax, uint nbmes);

		/*
			Some hardcoded tests
		*/
		void test_cluster_features();
		void testDJClustering();
		void test_clusters_visits();

	protected:
		data::WorkflowParam wp;
		vector<data::Cluster> clusters;
	};

}
