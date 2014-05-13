#ifndef DESORPTIONTOPORESPACE
#define DESORPTIONTOPORESPACE

#include "Kernel.h"
#include "LangmuirMaterial.h"

// Forward Declarations
class DesorptionToPorespace;

template<>
InputParameters validParams<DesorptionToPorespace>();

/**
 * Mass flow rate of fluid to the porespace from the matrix
 * Add this to the DE for the porepressure variable to get
 * fluid flowing from the matrix to the porespace
 */
class DesorptionToPorespace : public Kernel
{
public:

  DesorptionToPorespace(const std::string & name,
                        InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  /// moose internal variable number corresponding to the concentration in the matrix (needed for OffDiagJacobian)
  unsigned int _conc_var;

  /// mass flow rate from matrix = mass flow rate to porespace
  MaterialProperty<Real> & _mass_rate_from_matrix;

  /// derivative of mass flow rate from matrix wrt concentration
  MaterialProperty<Real> & _dmass_rate_from_matrix_dC;

  /// derivative of mass flow rate from matrix wrt pressure
  MaterialProperty<Real> & _dmass_rate_from_matrix_dp;
};

#endif //DESORPTIONTOPORESPACE
