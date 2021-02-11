#ifndef MODEL_H
#define MODEL_H

#include <vector>


class Model
{
public:
	virtual Model* clone() const = 0;

	virtual double drift_term(double time, double asset_price) const =0;
	virtual double volatility_term(double time, double asset_price) const =0;

private:
};

class BlackScholesModel: public Model
{
public:
	BlackScholesModel(double drift, double volatility);					
	BlackScholesModel(const BlackScholesModel& model);					
	~BlackScholesModel() = default;										
	BlackScholesModel& operator=(const BlackScholesModel& model);		

	BlackScholesModel* clone() const;
	double drift_term(double time, double asset_price) const override;
	double volatility_term(double time, double asset_price) const override;

private:
	double _drift;
	double _volatility;
};

class CIRModel : public Model
{
public:
	CIRModel(double mean_reversion_speed, double mean_reversion_level, double volatility);		
	CIRModel(const CIRModel& model);															
	~CIRModel() = default;																        
	CIRModel& operator=(const CIRModel& model);													

public:
	CIRModel* clone() const;
	double drift_term(double time, double asset_price) const override;
	double volatility_term(double time, double asset_price) const override;

private:
	double _mean_reversion_speed;
	double _mean_reversion_level;
	double _volatility;
};

#endif