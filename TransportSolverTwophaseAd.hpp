/*
  Copyright 2013 SINTEF ICT, Applied Mathematics.

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OPM_TRANSPORTSOLVERTWOPHASEAD_HEADER_INCLUDED
#define OPM_TRANSPORTSOLVERTWOPHASEAD_HEADER_INCLUDED

#include "AutoDiffBlock.hpp"
#include "AutoDiffHelpers.hpp"
#include <opm/core/transport/TransportSolverTwophaseInterface.hpp>
#include <vector>

struct UnstructuredGrid;

namespace Opm
{

    class IncompPropertiesInterface;
    class LinearSolverInterface;
    namespace parameter { class ParameterGroup; }

    /// Implements an implicit transport solver for incompressible two-phase flow,
    /// using automatic differentiation.
    class TransportSolverTwophaseAd : public TransportSolverTwophaseInterface
    {
    public:
        /// Construct solver.
        /// \param[in] grid       A 2d or 3d grid.
        /// \param[in] props      Rock and fluid properties.
        /// \param[in] linsolver  Linear solver for Newton-Raphson scheme.
        /// \param[in] param      Parameters for the solver.
        TransportSolverTwophaseAd(const UnstructuredGrid& grid,
                                  const IncompPropertiesInterface& props,
                                  const LinearSolverInterface& linsolver,
                                  const parameter::ParameterGroup& param);

        // Virtual destructor.
        virtual ~TransportSolverTwophaseAd();

        /// Solve for saturation at next timestep.
        /// Note that this only performs advection by total velocity, and
        /// no gravity segregation.
        /// \param[in]      porevolume   Array of pore volumes.
        /// \param[in]      source       Transport source term. For interpretation see Opm::computeTransportSource().
        /// \param[in]      dt           Time step.
        /// \param[in, out] state        Reservoir state. Calling solve() will read state.faceflux() and
        ///                              read and write state.saturation().
        virtual void solve(const double* porevolume,
                           const double* source,
                           const double dt,
                           TwophaseState& state);

    private:
        typedef AutoDiff::ForwardBlock<double> ADB;
        typedef ADB::V V;
        typedef ADB::M M;

    private:
        const UnstructuredGrid& grid_;
        const IncompPropertiesInterface& props_;
        const LinearSolverInterface& linsolver_;
        const HelperOps ops_;
        double tol_;
        int maxit_;
        std::vector<int> allcells_;


        #if 0
        const double* visc_;
        std::vector<double> smin_;
        std::vector<double> smax_;

        const double* darcyflux_;   // one flux per grid face
        const double* porevolume_;  // one volume per cell
        const double* source_;      // one source per cell
        double dt_;
        std::vector<double> saturation_;        // one per cell, only water saturation!
        std::vector<double> fractionalflow_;  // = m[0]/(m[0] + m[1]) per cell
        std::vector<int> reorder_iterations_;
        //std::vector<double> reorder_fval_;
        // For gravity segregation.
        std::vector<double> gravflux_;
        std::vector<double> mob_;
        std::vector<double> s0_;
        std::vector<std::vector<int> > columns_;

        // Storing the upwind and downwind graphs for experiments.
        std::vector<int> ia_upw_;
        std::vector<int> ja_upw_;
        std::vector<int> ia_downw_;
        std::vector<int> ja_downw_;

        struct Residual;
        double fracFlow(double s, int cell) const;

        struct GravityResidual;
        void mobility(double s, int cell, double* mob) const;
#endif
    };

} // namespace Opm

#endif // OPM_TRANSPORTSOLVERTWOPHASEAD_HEADER_INCLUDED