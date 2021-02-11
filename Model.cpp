#include "Model.h"
#include <cmath>



BlackScholesModel::BlackScholesModel(double drift, double volatility)
	: _drift(drift), _volatility(volatility)
{}

BlackScholesModel::BlackScholesModel(const BlackScholesModel& model) // 
	:_drift(model._drift), _volatility(model._volatility)
{
}

BlackScholesModel * BlackScholesModel::clone() const
{
	return new BlackScholesModel(*this); 
}

double BlackScholesModel::drift_term(double time, double asset_price) const 
{
	return _drift * asset_price;
}

double BlackScholesModel::volatility_term(double time, double asset_price) const
{
	return _volatility * asset_price;
}

BlackScholesModel& BlackScholesModel::operator=(const BlackScholesModel & model)
{
	if (this == &model) 
		return *this;
	else
	{
		_drift = model._drift;
		_volatility = model._volatility;
	}
	return *this;
}


CIRModel::CIRModel(double mean_reversion_speed, double mean_reversion_level, double volatility)
	:_mean_reversion_speed(mean_reversion_speed), _mean_reversion_level(mean_reversion_level), _volatility(volatility)
{
}

CIRModel::CIRModel(const CIRModel & model)
	: _mean_reversion_speed(model._mean_reversion_speed), _mean_reversion_level(model._mean_reversion_level), _volatility(model._volatility)
{
}

CIRModel * CIRModel::clone() const
{
	return new CIRModel(*this);
}

double CIRModel::drift_term(double time, double asset_price) const
{
	return _mean_reversion_speed * (_mean_reversion_level - asset_price);
}

double CIRModel::volatility_term(double time, double asset_price) const
{
	return _volatility * std::sqrt(asset_price);
}

CIRModel& CIRModel::operator=(const CIRModel & model)
{
	if (this == &model) 
		return *this;
	else
	{
		_mean_reversion_speed = model._mean_reversion_speed;
		_mean_reversion_level = model._mean_reversion_level;
		_volatility = model._volatility;
	}
	return *this;
}

