#pragma once

#include "workflow.h"

namespace services 
{
	class Core
	{
	public:
		Core(std::string parameterFile = "");
		~Core();

	protected:
		data::WorkflowParam wp;
	};

}
