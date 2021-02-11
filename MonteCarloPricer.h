#ifndef MONTECARLOPRICER_H
#define MONTECARLOPRICER_H

#ifndef PATHSIMULATOR_H
#include "PathSimulator.h"
#endif 


class MonteCarloPricer
{
public:
	MonteCarloPricer(const PathSimulator& path_simulator, size_t number_of_simulations, double discount_rate);

	MonteCarloPricer(const MonteCarloPricer& pricer);
	MonteCarloPricer& operator=(const MonteCarloPricer& pricer);

	virtual ~MonteCarloPricer(); 
	
	virtual double path_price(const std::vector<double>& path) const = 0; 
	double price() const;												 


protected:
	const PathSimulator* _path_simulator; 
	size_t _number_of_simulations;
	double _discount_rate;												
};


class MonteCarloOptionPricer : public MonteCarloPricer
{
public:
	MonteCarloOptionPricer(const PathSimulator& path_simulator, size_t number_of_simulations, double discount_rate, double strike, bool is_call);

protected:
	double _strike;
	bool _is_call;
};

class MonteCarloEuropeanOptionPricer : public MonteCarloOptionPricer
{
public:
	MonteCarloEuropeanOptionPricer(const PathSimulator& path_simulator, size_t number_of_simulations, double discount_rate, double strike, bool is_call);
	~MonteCarloEuropeanOptionPricer() = default;

	double path_price(const std::vector<double>& path) const override;
													
};

class MonteCarloAmericanOptionPricer : public MonteCarloOptionPricer
{
public:
	MonteCarloAmericanOptionPricer(const PathSimulator& path_simulator, size_t number_of_simulations, double discount_rate, double strike, bool is_call);

	double path_price(const std::vector<double>& path) const override;
};

class MonteCarloAsianEuropeanOptionPricer : public MonteCarloOptionPricer
{
public:
	MonteCarloAsianEuropeanOptionPricer(const PathSimulator& path_simulator, size_t number_of_simulations, double discount_rate, double strike, bool is_call);

	double path_price(const std::vector<double>& path) const override;
};

#endif