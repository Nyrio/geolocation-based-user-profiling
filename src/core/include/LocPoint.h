#pragma once

namespace data
{
	class LocPoint
	{
		friend class DJCluster;
	public:
		LocPoint(double lat, double longi);
		~LocPoint();

    protected:
        double lat;
        double longi;
			private:
				long idClustering=-1;

	};

}
