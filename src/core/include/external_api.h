#pragma once
#include <string>
#include <set>
#include "datatypes.h"
#include "workflow.h"

using namespace std;

namespace data
{
	set<string> get_amenities(const LocPoint& lp, const data::WorkflowParam& wp);
}