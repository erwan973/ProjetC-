#ifndef PATHSIMULATOR2D_H
#define PATHSIMULATOR2D_H

#ifndef MODEL2D_H
#include "Model2D.h"
#endif

#include <vector>
#include <cmath>

using Vector = std::vector<double>;
using Vector_Pair = std::vector<std::pair<double, double> >;

class PathSimulator2D final
{
public:
	PathSimulator2D(Pair initial_factors, 
                    const Vector& time_points, 
                    const Model2D& model);
	PathSimulator2D(const PathSimulator2D& path_simulator);
	PathSimulator2D& operator=(const PathSimulator2D& path_simulator);
	~PathSimulator2D();

	Vector path() const;
	Vector getTimePoints() const;

private:
	Pair nextStep(int current_index, Pair current_factors) const; 

	Pair _initial_factors;					
	Vector _time_points;		
	const Model2D* _model;
};

#endif