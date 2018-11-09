#pragma once

namespace data 
{
	class LocPoint
	{
	public:
		LocPoint(double lat, double longi);
		~LocPoint();

    protected:
        double lat;
        double longi;
	};

}
