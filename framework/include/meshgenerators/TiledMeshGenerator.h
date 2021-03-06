//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef TILEDMESHGENERATOR_H
#define TILEDMESHGENERATOR_H

#include "MeshGenerator.h"

// Forward declarations
class TiledMeshGenerator;

template <>
InputParameters validParams<TiledMeshGenerator>();

/**
 * Take an input mesh and repeat it in the x,y and z directions
 */
class TiledMeshGenerator : public MeshGenerator
{
public:
  TiledMeshGenerator(const InputParameters & parameters);

  std::unique_ptr<MeshBase> generate();

protected:
  /// Mesh that possibly comes from another generator
  std::unique_ptr<MeshBase> & _input;

  /// The mesh width in the x, y and z directions
  const Real _x_width;
  const Real _y_width;
  const Real _z_width;
};

#endif // TILEDMESHGENERATOR_H
