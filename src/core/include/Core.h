#pragma once

#include "workflow.h"

namespace services 
{
	class Core
	{
	public:
		Core(std::string parameterFile = "");
		~Core();

		void test_cluster_features();

	protected:
		data::WorkflowParam wp;
	};

}
