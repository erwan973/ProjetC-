#include <iostream>

#ifndef RANDOMNORMALGENERATOR_H
#include "RandomNormalGenerator.h"
#endif

#ifndef MONTECARLOPRICER_H
#include "MonteCarloPricer.h"
#endif

#ifndef MONTECARLOPRICER2D_H
#include "MonteCarloPricer2D.h"
#endif

#include "MonteCarloPricerTemplate.h"
#include "MonteCarloPricerTemplate.cpp"


Vector create_discretization_time_points()
{
	Vector time_points;
	size_t number_time_points = 50;
	double maturity = 2.;

	for (size_t time_index = 0; time_index < number_time_points; ++time_index)
	{
		double time_index_double = (double)time_index;
		double number_time_points_double = (double)number_time_points;
		time_points.push_back(time_index_double * maturity / (number_time_points_double - 1.));
	}

	return time_points;
}

PathSimulator create_pathsimulator_bs()
{
	// Defines initial value of asset
	double initial_asset_price = 100.;

	double drift = 0.06;
	double volatility = 0.20;
	BlackScholesModel model_BS(drift, volatility);

	// Defines the discretization of time space
	Vector time_points = create_discretization_time_points();

	// Defines the path simulator
	PathSimulator path_simulator_BS(initial_asset_price, time_points, model_BS);

	return path_simulator_BS;
}

PathSimulator create_pathsimulator_cir()
{
	// Defines initial value of asset
	double initial_asset_price = 100.;

	double volatility = 0.20;
	double mean_reversion_speed = 3.0;
	double mean_reversion_level = 100.;

	CIRModel model_CIR(mean_reversion_speed, mean_reversion_level, volatility);

	// Defines the discretization of time space
	Vector time_points = create_discretization_time_points();

	PathSimulator path_simulator_CIR(initial_asset_price, time_points, model_CIR);
	return path_simulator_CIR;
}

PathSimulator2D create_pathsimulator_heston()
{
	// Defines initial value of asset
	Pair initial_factors(100., 0.20); // (A0, B0) initial factors

	double drift = 0.06;
	double mean_reversion_speed = 3.0;
	double mean_reversion_level = 0.20;
	double vol_of_vol = 0.2;
	double correlation = -0.5;

	HestonModel model_Heston(correlation, drift, mean_reversion_speed, mean_reversion_level, vol_of_vol);

	// Defines the discretization of time space
	Vector time_points = create_discretization_time_points();

	// Defines the path simulator
	PathSimulator2D path_simulator_Heston(initial_factors, time_points, model_Heston);
	return path_simulator_Heston;
}

PathSimulator2D create_pathsimulator_sabr()
{
	// Defines initial value of asset
	Pair initial_factors(100., 0.20); // (A0, B0) initial factors

	double beta = 0.5;
	double alpha = 0.1;
	double correlation = -0.5;

	SabrModel model_Sabr(correlation, beta, alpha);

	// Defines the discretization of time space
	Vector time_points = create_discretization_time_points();

	// Defines the path simulator
	PathSimulator2D path_simulator_Sabr(initial_factors, time_points, model_Sabr);
	return path_simulator_Sabr;
}


void testing_pricer_1D()
{
	// Defines the pricer
	size_t number_of_simulations = 5000;
	double discount_rate = 0.03;
	double strike = 90.;
	bool isCall = true;

	///////////////////////// Black Scholes ///////////////////////////
	PathSimulator path_simulator_BS = create_pathsimulator_bs();

	MonteCarloEuropeanOptionPricer* pricer_BS_euro = new MonteCarloEuropeanOptionPricer(path_simulator_BS, number_of_simulations, discount_rate, strike, isCall);
	MonteCarloAmericanOptionPricer* pricer_BS_amer = new MonteCarloAmericanOptionPricer(path_simulator_BS, number_of_simulations, discount_rate, strike, isCall);
	MonteCarloAsianEuropeanOptionPricer* pricer_BS_asian = new MonteCarloAsianEuropeanOptionPricer(path_simulator_BS, number_of_simulations, discount_rate, strike, isCall);

	// Testing convergence ...
	size_t number_of_tests = 10;
	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_euro = pricer_BS_euro->price();
		std::cout << "European option price with BS model for test number " << test_index << " is " << pv_euro << "\n";
	}

	std::cout << "\n";

	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_amer = pricer_BS_amer->price();
		std::cout << "American option price with BS model for test number " << test_index << " is " << pv_amer << "\n";
	}

	std::cout << "\n";

	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_asian = pricer_BS_asian->price();
		std::cout << "Asian option price with BS model for test number " << test_index << " is " << pv_asian << "\n";
	}

	std::cout << "\n";

	///////////////////////// Cox Ingersoll Ross ///////////////////////////
	PathSimulator path_simulator_CIR = create_pathsimulator_cir();

	MonteCarloEuropeanOptionPricer* pricer_CIR_euro = new MonteCarloEuropeanOptionPricer(path_simulator_CIR, number_of_simulations, discount_rate, strike, isCall);
	MonteCarloAmericanOptionPricer* pricer_CIR_amer = new MonteCarloAmericanOptionPricer(path_simulator_CIR, number_of_simulations, discount_rate, strike, isCall);
	MonteCarloAsianEuropeanOptionPricer* pricer_CIR_asian = new MonteCarloAsianEuropeanOptionPricer(path_simulator_CIR, number_of_simulations, discount_rate, strike, isCall);

	// Testing convergence ...
	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_euro = pricer_CIR_euro->price();
		std::cout << "European option price with CIR model for test number " << test_index << " is " << pv_euro << "\n";
	}

	std::cout << "\n";

	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_amer = pricer_CIR_amer->price();
		std::cout << "American option price with CIR model for test number " << test_index << " is " << pv_amer << "\n";
	}

	std::cout << "\n";

	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_asian = pricer_CIR_asian->price();
		std::cout << "Asian option price with CIR model for test number " << test_index << " is " << pv_asian << "\n";
	}

	std::cout << "\n";
}

void testing_pricer_2D()
{
	// Defines the pricer
	size_t number_of_simulations = 5000;
	double discount_rate = 0.03;
	double strike = 90.;
	bool isCall = true;

	///////////////////////// Heston ///////////////////////////	
	PathSimulator2D path_simulator_Heston = create_pathsimulator_heston();

	MonteCarloEuropeanOptionPricer2D* pricer_Heston_euro = new MonteCarloEuropeanOptionPricer2D(path_simulator_Heston, number_of_simulations, discount_rate, strike, isCall);
	MonteCarloAmericanOptionPricer2D* pricer_Heston_amer = new MonteCarloAmericanOptionPricer2D(path_simulator_Heston, number_of_simulations, discount_rate, strike, isCall);
	MonteCarloAsianEuropeanOptionPricer2D* pricer_Heston_asian = new MonteCarloAsianEuropeanOptionPricer2D(path_simulator_Heston, number_of_simulations, discount_rate, strike, isCall);

	// Testing convergence ...
	size_t number_of_tests = 10;
	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_euro = pricer_Heston_euro->price();
		std::cout << "European option price with Heston model for test number " << test_index << " is " << pv_euro << "\n";
	}

	std::cout << "\n";

	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_amer = pricer_Heston_amer->price();
		std::cout << "American option price with Heston model for test number " << test_index << " is " << pv_amer << "\n";
	}

	std::cout << "\n";

	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_asian = pricer_Heston_asian->price();
		std::cout << "Asian option price with Heston model for test number " << test_index << " is " << pv_asian << "\n";
	}

	std::cout << "\n";

	///////////////////////// SABR ///////////////////////////
	PathSimulator2D path_simulator_Sabr = create_pathsimulator_sabr();

	MonteCarloEuropeanOptionPricer2D* pricer_Sabr_euro = new MonteCarloEuropeanOptionPricer2D(path_simulator_Sabr, number_of_simulations, discount_rate, strike, isCall);
	MonteCarloAmericanOptionPricer2D* pricer_Sabr_amer = new MonteCarloAmericanOptionPricer2D(path_simulator_Sabr, number_of_simulations, discount_rate, strike, isCall);
	MonteCarloAsianEuropeanOptionPricer2D* pricer_Sabr_asian = new MonteCarloAsianEuropeanOptionPricer2D(path_simulator_Sabr, number_of_simulations, discount_rate, strike, isCall);

	// Testing convergence ...
	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_euro = pricer_Sabr_euro->price();
		std::cout << "European option price with SABR model for test number " << test_index << " is " << pv_euro << "\n";
	}

	std::cout << "\n";

	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_amer = pricer_Sabr_amer->price();
		std::cout << "American option price with SABR model for test number " << test_index << " is " << pv_amer << "\n";
	}

	std::cout << "\n";

	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_asian = pricer_Sabr_asian->price();
		std::cout << "Asian option price with SABR model for test number " << test_index << " is " << pv_asian << "\n";
	}

	std::cout << "\n";
}


void testing_pricer_template()
{
	// Defines the pricer
	size_t number_of_simulations = 5000;
	double discount_rate = 0.03;
	double strike = 90.;
	bool isCall = true;

	///////////////////////// Black Scholes ///////////////////////////
	PathSimulator path_simulator_BS = create_pathsimulator_bs();

	MonteCarloEuropeanOptionPricerTemplate<PathSimulator>* pricer_BS_euro = new MonteCarloEuropeanOptionPricerTemplate<PathSimulator>(path_simulator_BS, number_of_simulations, discount_rate, strike, isCall);
	MonteCarloAmericanOptionPricerTemplate<PathSimulator>* pricer_BS_amer = new MonteCarloAmericanOptionPricerTemplate<PathSimulator>(path_simulator_BS, number_of_simulations, discount_rate, strike, isCall);
	MonteCarloAsianEuropeanOptionPricerTemplate<PathSimulator>* pricer_BS_asian = new MonteCarloAsianEuropeanOptionPricerTemplate<PathSimulator>(path_simulator_BS, number_of_simulations, discount_rate, strike, isCall);

	// Testing convergence ...
	size_t number_of_tests = 10;
	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_euro = pricer_BS_euro->price();
		std::cout << "European option price with BS model for test number " << test_index << " is " << pv_euro << "\n";
	}

	std::cout << "\n";

	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_amer = pricer_BS_amer->price();
		std::cout << "American option price with BS model for test number " << test_index << " is " << pv_amer << "\n";
	}

	std::cout << "\n";

	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_asian = pricer_BS_asian->price();
		std::cout << "Asian option price with BS model for test number " << test_index << " is " << pv_asian << "\n";
	}

	std::cout << "\n";

	///////////////////////// Cox Ingersoll Ross ///////////////////////////
	PathSimulator path_simulator_CIR = create_pathsimulator_cir();

	MonteCarloEuropeanOptionPricerTemplate<PathSimulator>* pricer_CIR_euro = new MonteCarloEuropeanOptionPricerTemplate<PathSimulator>(path_simulator_CIR, number_of_simulations, discount_rate, strike, isCall);
	MonteCarloAmericanOptionPricerTemplate<PathSimulator>* pricer_CIR_amer = new MonteCarloAmericanOptionPricerTemplate<PathSimulator>(path_simulator_CIR, number_of_simulations, discount_rate, strike, isCall);
	MonteCarloAsianEuropeanOptionPricerTemplate<PathSimulator>* pricer_CIR_asian = new MonteCarloAsianEuropeanOptionPricerTemplate<PathSimulator>(path_simulator_CIR, number_of_simulations, discount_rate, strike, isCall);

	// Testing convergence ...
	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_euro = pricer_CIR_euro->price();
		std::cout << "European option price with CIR model for test number " << test_index << " is " << pv_euro << "\n";
	}

	std::cout << "\n";

	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_amer = pricer_CIR_amer->price();
		std::cout << "American option price with CIR model for test number " << test_index << " is " << pv_amer << "\n";
	}

	std::cout << "\n";

	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_asian = pricer_CIR_asian->price();
		std::cout << "Asian option price with CIR model for test number " << test_index << " is " << pv_asian << "\n";
	}

	std::cout << "\n";

	///////////////////////// Heston ///////////////////////////	
	PathSimulator2D path_simulator_Heston = create_pathsimulator_heston();

	MonteCarloEuropeanOptionPricerTemplate<PathSimulator2D>* pricer_Heston_euro = new MonteCarloEuropeanOptionPricerTemplate<PathSimulator2D>(path_simulator_Heston, number_of_simulations, discount_rate, strike, isCall);
	MonteCarloAmericanOptionPricerTemplate<PathSimulator2D>* pricer_Heston_amer = new MonteCarloAmericanOptionPricerTemplate<PathSimulator2D>(path_simulator_Heston, number_of_simulations, discount_rate, strike, isCall);
	MonteCarloAsianEuropeanOptionPricerTemplate<PathSimulator2D>* pricer_Heston_asian = new MonteCarloAsianEuropeanOptionPricerTemplate<PathSimulator2D>(path_simulator_Heston, number_of_simulations, discount_rate, strike, isCall);

	// Testing convergence ...
	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_euro = pricer_Heston_euro->price();
		std::cout << "European option price with Heston model for test number " << test_index << " is " << pv_euro << "\n";
	}

	std::cout << "\n";

	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_amer = pricer_Heston_amer->price();
		std::cout << "American option price with Heston model for test number " << test_index << " is " << pv_amer << "\n";
	}

	std::cout << "\n";

	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_asian = pricer_Heston_asian->price();
		std::cout << "Asian option price with Heston model for test number " << test_index << " is " << pv_asian << "\n";
	}

	std::cout << "\n";

	///////////////////////// SABR ///////////////////////////
	PathSimulator2D path_simulator_Sabr = create_pathsimulator_sabr();

	MonteCarloEuropeanOptionPricerTemplate<PathSimulator2D>* pricer_Sabr_euro = new MonteCarloEuropeanOptionPricerTemplate<PathSimulator2D>(path_simulator_Sabr, number_of_simulations, discount_rate, strike, isCall);
	MonteCarloAmericanOptionPricerTemplate<PathSimulator2D>* pricer_Sabr_amer = new MonteCarloAmericanOptionPricerTemplate<PathSimulator2D>(path_simulator_Sabr, number_of_simulations, discount_rate, strike, isCall);
	MonteCarloAsianEuropeanOptionPricerTemplate<PathSimulator2D>* pricer_Sabr_asian = new MonteCarloAsianEuropeanOptionPricerTemplate<PathSimulator2D>(path_simulator_Sabr, number_of_simulations, discount_rate, strike, isCall);

	// Testing convergence ...
	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_euro = pricer_Sabr_euro->price();
		std::cout << "European option price with SABR model for test number " << test_index << " is " << pv_euro << "\n";
	}

	std::cout << "\n";

	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_amer = pricer_Sabr_amer->price();
		std::cout << "American option price with SABR model for test number " << test_index << " is " << pv_amer << "\n";
	}

	std::cout << "\n";

	for (int test_index = 0; test_index < number_of_tests; ++test_index)
	{
		double pv_asian = pricer_Sabr_asian->price();
		std::cout << "Asian option price with SABR model for test number " << test_index << " is " << pv_asian << "\n";
	}

	std::cout << "\n";
}



int main()
{
	// Testing of the PRICERS

	//testing_pricer_1D();
	//testing_pricer_2D();

	testing_pricer_template();

	return 0;
}