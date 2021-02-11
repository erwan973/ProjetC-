#ifndef MONTECARLOPRICERTEMPLATE_H
#define MONTECARLOPRICERTEMPLATE_H

#include <vector>
using Vector = std::vector<double>;

template<class PathSim>
class MonteCarloPricerTemplate
{
	public:
	MonteCarloPricerTemplate(const PathSim& path_simulator, size_t number_of_simulations, double discount_rate);

	MonteCarloPricerTemplate(const MonteCarloPricerTemplate& pricer);
	MonteCarloPricerTemplate& operator=(const MonteCarloPricerTemplate& pricer);

	virtual ~MonteCarloPricerTemplate(); 

	virtual double path_price(const Vector& path) const = 0; 
	double price() const;												 

protected:
	const PathSim* _path_simulator;
	size_t _number_of_simulations;
	double _discount_rate;
};

template<class PathSim>
class MonteCarloOptionPricerTemplate : public MonteCarloPricerTemplate<PathSim>
{
public:
	MonteCarloOptionPricerTemplate(const PathSim& path_simulator, size_t number_of_simulations, double discount_rate, double strike, bool is_call);

protected:
	double _strike;
	bool _is_call;
};

template<class PathSim>
class MonteCarloEuropeanOptionPricerTemplate : public MonteCarloOptionPricerTemplate<PathSim>
{
public:
	MonteCarloEuropeanOptionPricerTemplate(const PathSim& path_simulator, size_t number_of_simulations, double discount_rate, double strike, bool is_call);
	~MonteCarloEuropeanOptionPricerTemplate() = default;

	double path_price(const Vector& path) const override;
};

template<class PathSim>
class MonteCarloAmericanOptionPricerTemplate : public MonteCarloOptionPricerTemplate<PathSim>
{
public:
	MonteCarloAmericanOptionPricerTemplate(const PathSim& path_simulator, size_t number_of_simulations, double discount_rate, double strike, bool is_call);

	double path_price(const Vector& path) const override;
};

template<class PathSim>
class MonteCarloAsianEuropeanOptionPricerTemplate : public MonteCarloOptionPricerTemplate<PathSim>
{
public:
	MonteCarloAsianEuropeanOptionPricerTemplate(const PathSim& path_simulator, size_t number_of_simulations, double discount_rate, double strike, bool is_call);

	double path_price(const Vector& path) const override;
};



#endif

