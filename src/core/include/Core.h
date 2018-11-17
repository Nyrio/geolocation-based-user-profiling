#pragma once

#include "workflow.h"

namespace services
{
	class Core
	{
	public:
		Core(std::string parameterFile = "");
		~Core();


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
