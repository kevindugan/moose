//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ComputeNodalAuxVarsThread.h"

// MOOSE includes
#include "AuxiliarySystem.h"
#include "AuxKernel.h"
#include "FEProblem.h"
#include "MooseMesh.h"

#include "libmesh/threads.h"

ComputeNodalAuxVarsThread::ComputeNodalAuxVarsThread(
    FEProblemBase & fe_problem, const MooseObjectWarehouse<AuxKernel> & storage)
  : ThreadedNodeLoop<ConstNodeRange, ConstNodeRange::const_iterator>(fe_problem),
    _aux_sys(fe_problem.getAuxiliarySystem()),
    _storage(storage)
{
}

// Splitting Constructor
ComputeNodalAuxVarsThread::ComputeNodalAuxVarsThread(ComputeNodalAuxVarsThread & x,
                                                     Threads::split split)
  : ThreadedNodeLoop<ConstNodeRange, ConstNodeRange::const_iterator>(x, split),
    _aux_sys(x._aux_sys),
    _storage(x._storage)
{
}

void
ComputeNodalAuxVarsThread::subdomainChanged()
{
  std::set<TagID> needed_vector_tags;
  std::set<TagID> needed_matrix_tags;

  const auto & block_kernels = _storage.getActiveBlockObjects(_tid);

  for (const auto & block : _block_ids)
  {
    const auto iter = block_kernels.find(block);

    if (iter != block_kernels.end())
      for (const auto & aux : iter->second)
      {
        auto & matrix_tags = aux->getFEVariableCoupleableMatrixTags();
        needed_matrix_tags.insert(matrix_tags.begin(), matrix_tags.end());
        auto & vector_tags = aux->getFEVariableCoupleableVectorTags();
        needed_vector_tags.insert(vector_tags.begin(), vector_tags.end());
      }
  }

  _fe_problem.setActiveFEVariableCoupleableMatrixTags(needed_matrix_tags, _tid);
  _fe_problem.setActiveFEVariableCoupleableVectorTags(needed_vector_tags, _tid);
}

void
ComputeNodalAuxVarsThread::onNode(ConstNodeRange::const_iterator & node_it)
{
  const Node * node = *node_it;

  const auto & block_ids = _aux_sys.mesh().getNodeBlockIds(*node);

  if (_block_ids != block_ids)
  {
    _block_ids.clear();
    _block_ids.insert(block_ids.begin(), block_ids.end());
    subdomainChanged();
  }

  // prepare variables
  for (const auto & it : _aux_sys._nodal_vars[_tid])
  {
    MooseVariable * var = it.second;
    var->prepareAux();
  }

  _fe_problem.reinitNode(node, _tid);

  // Get a map of all active block restricted AuxKernel objects
  const auto & block_kernels = _storage.getActiveBlockObjects(_tid);

  // Loop over all SubdomainIDs for the current node, if an AuxKernel is active on this block then
  // compute it.
  for (const auto & block : block_ids)
  {
    const auto iter = block_kernels.find(block);

    if (iter != block_kernels.end())
      for (const auto & aux : iter->second)
        aux->compute();
  }

  // We are done, so update the solution vector
  {
    Threads::spin_mutex::scoped_lock lock(Threads::spin_mtx);
    for (const auto & it : _aux_sys._nodal_vars[_tid])
    {
      MooseVariable * var = it.second;
      var->insert(_aux_sys.solution());
    }
  }
}

void
ComputeNodalAuxVarsThread::post()
{
  _fe_problem.clearActiveFEVariableCoupleableVectorTags(_tid);
  _fe_problem.clearActiveFEVariableCoupleableMatrixTags(_tid);
}

void
ComputeNodalAuxVarsThread::join(const ComputeNodalAuxVarsThread & /*y*/)
{
}
