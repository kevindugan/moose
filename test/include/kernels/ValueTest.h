//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#ifndef VALUETEST_H_
#define VALUETEST_H_

#include "Kernel.h"

class ValueTest : public Kernel
{
public:
  ValueTest(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
};

template <>
InputParameters validParams<ValueTest>();

#endif /* VALUETEST_H_ */
