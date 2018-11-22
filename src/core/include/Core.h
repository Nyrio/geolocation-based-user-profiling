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

		vector<data::Cluster> clusterize(uint id, time_t t1 = 0, time_t t2 = 0);
		void find_house(uint id, time_t t1 = 0, time_t t2 = 0);
		void show_clusters(uint id, time_t t1 = 0, time_t t2 = 0);

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
	};

}
