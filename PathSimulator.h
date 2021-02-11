#ifndef PATHSIMULATOR_H
#define PATHSIMULATOR_H

#ifndef MODEL_H
#include "Model.h"
#endif

#include <vector>

class PathSimulator final
{
public:
	PathSimulator(double initial_value, const std::vector<double>& time_points, const Model& model);
	PathSimulator(const PathSimulator& path_simulator);
	PathSimulator& operator=(const PathSimulator& path_simulator);
	~PathSimulator();

	std::vector<double> path() const;
	std::vector<double> getTimePoints() const;

private:
	double nextStep(int current_index, double current_asset_price) const; 

	double _initial_value;					
	std::vector<double> _time_points;		
	const Model* _model;
						 
};

#endif