#pragma once
#include <string>
#include <set>
#include "datatypes.h"
#include "workflow.h"

namespace data
{
	set<string> get_amenities(const LocPoint& lp, const data::WorkflowParam& wp);
}