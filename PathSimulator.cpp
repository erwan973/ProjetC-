#ifndef PATHSIMULATOR_H
#include "PathSimulator.h"
#endif

#ifndef RANDOMNORMALGENERATOR_H
#include "RandomNormalGenerator.h"
#endif


PathSimulator::PathSimulator(double initial_value, const std::vector<double>& time_points, const Model& model)
	: _initial_value(initial_value), _time_points(time_points), _model(model.clone())
{
}


PathSimulator::PathSimulator(const PathSimulator & path_simulator)
	: _initial_value(path_simulator._initial_value), 
	_time_points(path_simulator._time_points), 
	_model(path_simulator._model->clone())
{
}

PathSimulator& PathSimulator::operator=(const PathSimulator & path_simulator)
{
	
	if (this == &path_simulator) return *this;
	else {
		delete _model;								
		_model = path_simulator._model->clone();	

		
		_initial_value = path_simulator._initial_value;
		_time_points = path_simulator._time_points;
		}

		return *this;								 
}


PathSimulator::~PathSimulator()
{
	delete _model;
}

std::vector<double> PathSimulator::path() const
{
	std::vector<double> path {_initial_value};
	for (int index = 0; index < _time_points.size() - 1; ++index)
		path.push_back(nextStep(index, path[index]));

	return path;
}

std::vector<double> PathSimulator::getTimePoints() const
{
	return _time_points;
}


double PathSimulator::nextStep(int current_index, double current_asset_price) const
{
	double cur_time = _time_points[current_index];
	double time_gap = _time_points[current_index + 1] - cur_time;

	
	double drift_increment = _model->drift_term(cur_time, current_asset_price) * time_gap;

	
	double volatility_increment = _model->volatility_term(cur_time, current_asset_price) * sqrt(time_gap) * RandomNormalGenerator::normalRandom();  

	return current_asset_price + drift_increment + volatility_increment;
}
