#ifndef MODEL2D_H
#define MODEL2D_H

#include <utility>
using Pair = std::pair<double, double>;

class Model2D
{
public:
	virtual ~Model2D() = default;

	Model2D(double correlation);
    virtual Model2D* clone() const = 0;

	virtual Pair drift_term(double time, Pair factors) const =0;
	virtual Pair volatility_term(double time, Pair factors) const =0;

    double get_correlation() const;
protected:
    double _correlation;
};

class HestonModel final: public Model2D 
{
public:
    HestonModel(double correlation,
                double drift, 
                double mean_reversion_speed, 
                double mean_reversion_level,
                double vol_of_vol);
    HestonModel* clone() const override;
	Pair drift_term(double time, Pair factors) const override;
	Pair volatility_term(double time, Pair factors) const override;
private:
    double _drift;
    double _mean_reversion_speed;
    double _mean_reversion_level;
    double _vol_of_vol;
};

class SabrModel final : public Model2D
{
public:
	SabrModel(double correlation, double beta, double alpha);
	SabrModel* clone() const override;
	Pair drift_term(double time, Pair factors) const override;
	Pair volatility_term(double time, Pair factors) const override;
private:
	double _beta;
	double _alpha;
};


#endif