#pragma once

#include "workflow.h"

namespace services 
{
	class Core
	{
	public:
		Core(std::string parameterFile = "");
		~Core();

		void do_stuff();

	protected:
		data::WorkflowParam wp;
	};

}
