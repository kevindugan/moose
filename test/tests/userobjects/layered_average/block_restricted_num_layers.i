[Mesh]
  type = GeneratedMesh
  nx = 10
  ny = 10
  dim = 2
[]

[Variables]
  [u]
  []
[]

[AuxVariables]
  [master_app_var]
    order = CONSTANT
    family = MONOMIAL
    block = '1'
  []
[]

[AuxKernels]
  [layered_aux]
    type = SpatialUserObjectAux
    variable = master_app_var
    execute_on = 'timestep_end'
    user_object = master_uo
    block = '1'
  []
[]

[UserObjects]
  [master_uo]
    type = LayeredAverage
    direction = x
    variable = 'u'
    block = '1'
    num_layers = 2
  []
[]

[Kernels]
  [diff]
    type = Diffusion
    variable = u
  []
[]

[BCs]
  [left]
    type = DirichletBC
    variable = u
    boundary = 'left'
    value = 0
  []
  [right]
    type = DirichletBC
    variable = u
    boundary = 'right'
    value = 100
  []
[]

[Executioner]
  type = Transient
  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-10
  petsc_options_iname = '-pc_type -pc_hypre_type'
  num_steps = 1
  petsc_options_value = 'hypre boomeramg'
  l_tol = 1e-8
[]

[Postprocessors]
  [u_avg]
    type = ElementAverageValue
    variable = 'u'
    execute_on = 'initial timestep_end'
  []
  [final_avg]
    type = ElementAverageValue
    variable = 'master_app_var'
    execute_on = 'initial timestep_end'
    block = '1'
  []
[]

[Outputs]
  exodus = true
[]

[MeshModifiers]
  [middle]
    type = SubdomainBoundingBox
    block_id = 1
    top_right = '0.6 0.6 0'
    bottom_left = '0.4 0.4 0'
  []
[]
