// SPDX-FileCopyrightText: Copyright (c) 2021-2024 The Lethe Authors
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception OR LGPL-2.1-or-later

/*
 * This file defines the parameters in the parameter namespace
 * that pertain to multiphysics simulations
 */


#ifndef lethe_parameters_multiphysics_h
#define lethe_parameters_multiphysics_h

#include <core/parameters.h>
#include <core/utilities.h>

#include <deal.II/base/parameter_handler.h>

using namespace dealii;

namespace Parameters
{
  /** @brief Class to account for different sharpening types:
   *  - constant: the sharpening threshold is the same throughout the
   * simulation,
   *  - adaptive: the sharpening threshold is determined by binary search, to
   * ensure mass conservation of the monitored phase
   */
  enum class SharpeningType
  {
    constant,
    adaptive
  };

  /** @brief Class to account for different phase fraction filtering types:
   * - none: no filter wil be applied on the calculated phase fraction
   * - tanh: the tanh filter function will be applied to the phase fraction,
   * a \f$\beta\f$ parameter influencing the interface definition must be
   * defined
   */
  enum class FilterType
  {
    none,
    clip,
    tanh
  };

  enum class EpsilonSetMethod
  {
    automatic,
    manual
  };

  /**
   * @brief Verbosity options for the epsilon parameter.
   */
  enum class EpsilonVerbosity
  {
    /// Epsilon related information will not be displayed on terminal
    quiet,
    /// Epsilon value will be displayed on terminal for every steady and
    /// transient iteration.
    verbose
  };


  /**
   * @brief CahnHilliard_PhaseFilter - Defines the parameters for the phase filtration of CahnHilliard physics
   */
  struct CahnHilliard_PhaseFilter
  {
    // Type of filter
    Parameters::FilterType type;

    // \f$beta\f$ value for the tanh filter
    double beta;

    // Type of verbosity for the phase filter
    Parameters::Verbosity verbosity;

    void
    declare_parameters(ParameterHandler &prm);
    void
    parse_parameters(ParameterHandler &prm);
  };

  /**
   * @brief VOF_InterfaceSharpening - Defines the parameters for
   * interface sharpening in the VOF solver.
   */
  struct VOF_InterfaceSharpening
  {
    // Interface sharpening parameters. The sharpening method and parameters are
    // explained in the dam break VOF example:
    // https://chaos-polymtl.github.io/lethe/examples/multiphysics/dam-break-VOF/dam-break-VOF.html

    bool enable;

    Parameters::SharpeningType type;

    // Parameters for constant sharpening
    double threshold;

    // Parameters for adaptive sharpening
    double threshold_max_deviation;
    int    max_iterations;

    // Other sharpening parameters
    double interface_sharpness;
    int    frequency;

    // Type of verbosity for the interface sharpening calculation
    Parameters::Verbosity verbosity;

    bool monitoring;

    /// Conservation tolerance on the fluid monitored,
    /// used with adaptive sharpening
    double tolerance;

    Parameters::FluidIndicator monitored_fluid;

    void
    declare_parameters(ParameterHandler &prm);
    void
    parse_parameters(ParameterHandler &prm);
  };

  /**
   * @brief SurfaceTensionForce - Defines the parameters for
   * the calculation of surface tension force in the VOF solver.
   */
  struct VOF_SurfaceTensionForce
  {
    bool enable;

    double phase_fraction_gradient_diffusion_factor;
    double curvature_diffusion_factor;

    bool output_vof_auxiliary_fields;

    // Type of verbosity for the surface tension force calculation
    Parameters::Verbosity verbosity;

    // Enable marangoni effect
    bool enable_marangoni_effect;

    void
    declare_parameters(ParameterHandler &prm);
    void
    parse_parameters(ParameterHandler &prm);
  };

  /**
   * @brief VOF_PhaseFilter - Defines the parameters for the phase filtration
   */
  struct VOF_PhaseFilter
  {
    // Type of filter
    Parameters::FilterType type;

    // $$\beta$$ value for the tanh filter
    double beta;

    // Type of verbosity for the phase filter
    Parameters::Verbosity verbosity;

    void
    declare_parameters(ParameterHandler &prm);
    void
    parse_parameters(ParameterHandler &prm);
  };


  /**
   * @brief VOF - Defines the parameters for free surface simulations
   * using the VOF method.
   * Has to be declared before member creation in Multiphysics structure.
   */
  struct VOF
  {
    Parameters::VOF_InterfaceSharpening sharpening;
    Parameters::VOF_SurfaceTensionForce surface_tension_force;
    Parameters::VOF_PhaseFilter         phase_filter;

    Parameters::FluidIndicator viscous_dissipative_fluid;

    // artificial diffusivity (diffusion coefficient) (in L^2/s) added to the
    // VOF transport equation. This parameter is zero by default, and can be
    // increased to improve the wetting of the phases in the vicinity of
    // boundaries
    double diffusivity;

    bool compressible;

    void
    declare_parameters(ParameterHandler &prm);
    void
    parse_parameters(ParameterHandler &prm);
  };

  struct CahnHilliard
  {
    // Smoothing parameter \f$\xi\f$ in the Cahn-Hilliard equations (potential
    // equation)
    double potential_smoothing_coefficient;

    // Epsilon set strategy (automatic|manual)
    Parameters::EpsilonSetMethod epsilon_set_method;

    // Epsilon verbosity
    Parameters::EpsilonVerbosity epsilon_verbosity;

    // Epsilon value in the Cahn-Hilliard equations
    double epsilon;

    // Phase filtration parameters
    Parameters::CahnHilliard_PhaseFilter cahn_hilliard_phase_filter;

    void
    declare_parameters(ParameterHandler &prm);
    void
    parse_parameters(ParameterHandler &prm, const Dimensionality dimensions);
  };

  /**
   * @brief Multiphysics - the parameters for multiphysics simulations
   * and handles sub-physics parameters.
   */
  struct Multiphysics
  {
    bool fluid_dynamics;
    bool heat_transfer;
    bool tracer;
    bool VOF;
    bool cahn_hilliard;

    // subparameters for heat_transfer
    bool viscous_dissipation;
    bool buoyancy_force;

    Parameters::VOF          vof_parameters;
    Parameters::CahnHilliard cahn_hilliard_parameters;

    void
    declare_parameters(ParameterHandler &prm);
    void
    parse_parameters(ParameterHandler &prm, const Dimensionality dimensions);
  };
} // namespace Parameters
#endif
