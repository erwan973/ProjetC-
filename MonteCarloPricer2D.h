#ifndef MONTECARLOPRICER2D_H
#define MONTECARLOPRICER2D_H

#ifndef PATHSIMULATOR2D_H
#include "PathSimulator2D.h"
#endif 

class MonteCarloPricer2D
{
public:
	MonteCarloPricer2D(const PathSimulator2D& path_simulator, size_t number_of_simulations, double discount_rate);

	MonteCarloPricer2D(const MonteCarloPricer2D& pricer);
	MonteCarloPricer2D& operator=(const MonteCarloPricer2D& pricer);

	virtual ~MonteCarloPricer2D(); 

	virtual double path_price(const Vector& path) const = 0; 
	double price() const;												 


protected:
	const PathSimulator2D* _path_simulator;
	size_t _number_of_simulations;
	double _discount_rate;
};


class MonteCarloOptionPricer2D : public MonteCarloPricer2D
{
public:
	MonteCarloOptionPricer2D(const PathSimulator2D& path_simulator, size_t number_of_simulations, double discount_rate, double strike, bool is_call);

protected:
	double _strike;
	bool _is_call;
};

class MonteCarloEuropeanOptionPricer2D : public MonteCarloOptionPricer2D
{
public:
	MonteCarloEuropeanOptionPricer2D(const PathSimulator2D& path_simulator, size_t number_of_simulations, double discount_rate, double strike, bool is_call);
	~MonteCarloEuropeanOptionPricer2D() = default;

	double path_price(const Vector& path) const override;

};

class MonteCarloAmericanOptionPricer2D : public MonteCarloOptionPricer2D
{
public:
	MonteCarloAmericanOptionPricer2D(const PathSimulator2D& path_simulator, size_t number_of_simulations, double discount_rate, double strike, bool is_call);

	double path_price(const Vector& path) const override;
};

class MonteCarloAsianEuropeanOptionPricer2D : public MonteCarloOptionPricer2D
{
public:
	MonteCarloAsianEuropeanOptionPricer2D(const PathSimulator2D& path_simulator, size_t number_of_simulations, double discount_rate, double strike, bool is_call);

	double path_price(const Vector& path) const override;
};

#endif