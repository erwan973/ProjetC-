#include "MonteCarloPricer.h"
#include "MonteCarloPricer2D.h"

#include <algorithm> 

MonteCarloPricer2D::MonteCarloPricer2D(const PathSimulator2D & path_simulator, size_t number_of_simulations, double discount_rate)
	: _path_simulator(new PathSimulator2D(path_simulator)), _number_of_simulations(number_of_simulations), _discount_rate(discount_rate)
{
}

MonteCarloPricer2D::MonteCarloPricer2D(const MonteCarloPricer2D & pricer)
	: _path_simulator(new PathSimulator2D(*(pricer._path_simulator))), _number_of_simulations(pricer._number_of_simulations), _discount_rate(pricer._discount_rate)
{
}

MonteCarloPricer2D & MonteCarloPricer2D::operator=(const MonteCarloPricer2D & pricer)
{
	if (this == &pricer)
		return *this;
	else
	{
		delete _path_simulator;												
		_path_simulator = new PathSimulator2D(*(pricer._path_simulator));		

		_number_of_simulations = pricer._number_of_simulations;
		_discount_rate = pricer._discount_rate;
	}
	return *this;
}

MonteCarloPricer2D::~MonteCarloPricer2D()
{
	delete _path_simulator; 
}

double MonteCarloPricer2D::price() const
{
	double price = 0.;

	for (size_t simulation_index = 0; simulation_index < _number_of_simulations; ++simulation_index)
	{
		Vector path = _path_simulator->path();
		price += path_price(path);
	}
	price /= _number_of_simulations;
	return price;
}


MonteCarloOptionPricer2D::MonteCarloOptionPricer2D(const PathSimulator2D & path_simulator, size_t number_of_simulations, double discount_rate, double strike, bool is_call)
	: MonteCarloPricer2D(path_simulator, number_of_simulations, discount_rate), _strike(strike), _is_call(is_call)
{
}


MonteCarloEuropeanOptionPricer2D::MonteCarloEuropeanOptionPricer2D(const PathSimulator2D & path_simulator, size_t number_of_simulations, double discount_rate, double strike, bool isCall)
	: MonteCarloOptionPricer2D(path_simulator, number_of_simulations, discount_rate, strike, isCall)
{
}

double MonteCarloEuropeanOptionPricer2D::path_price(const Vector& path) const
{
	double spot_at_maturity = path.at(path.size() - 1);
	double path_payoff = std::max(_is_call ? spot_at_maturity - _strike : _strike - spot_at_maturity, 0.);

	double maturity = _path_simulator->getTimePoints().at(_path_simulator->getTimePoints().size() - 1);
	double path_price = std::exp(-_discount_rate * maturity) * path_payoff;

	return path_price;
}

MonteCarloAmericanOptionPricer2D::MonteCarloAmericanOptionPricer2D(const PathSimulator2D & path_simulator, size_t number_of_simulations, double discount_rate, double strike, bool isCall)
	:MonteCarloOptionPricer2D(path_simulator, number_of_simulations, discount_rate, strike, isCall)
{
}

double MonteCarloAmericanOptionPricer2D::path_price(const Vector& path) const
{

	Vector payoffs;
	for (int time_index = 0; time_index < path.size(); ++time_index)
		payoffs.push_back(std::max(_is_call ? path[time_index] - _strike : _strike - path[time_index], 0.)); 

	Vector::const_iterator payoff_max_iterator = std::max_element<Vector::const_iterator>(payoffs.begin(), payoffs.end());

	int payoff_max_iterator_index = payoff_max_iterator - payoffs.begin();

	double maturity = _path_simulator->getTimePoints()[payoff_max_iterator_index];
	double path_price = std::exp(-_discount_rate * maturity) * *payoff_max_iterator; 

	return path_price;
}

MonteCarloAsianEuropeanOptionPricer2D::MonteCarloAsianEuropeanOptionPricer2D(const PathSimulator2D & path_simulator, size_t number_of_simulations, double discount_rate, double strike, bool is_call)
	: MonteCarloOptionPricer2D(path_simulator, number_of_simulations, discount_rate, strike, is_call)
{
}

double MonteCarloAsianEuropeanOptionPricer2D::path_price(const Vector& path) const
{
	double average = 0.;

	size_t number_of_points = path.size();
	Vector time_points = _path_simulator->getTimePoints();
	for (size_t index = 0; index <= number_of_points - 2; ++index)
	{
		double mid_point_S_t = 0.5* (path[index] + path[index + 1]); 
		double delta_t = time_points[index + 1] - time_points[index];
		average += mid_point_S_t * delta_t;
	}

	double maturity = time_points[number_of_points - 1];
	average /= maturity;


	double payoff = _is_call ? std::max(average - _strike, 0.) : std::max(_strike - average, 0.);
	double pv = std::exp(-_discount_rate * maturity) * payoff;

	return pv;
}