#ifndef OPENMM_VARIABLEVERLETINTEGRATOR_H_
#define OPENMM_VARIABLEVERLETINTEGRATOR_H_

/* -------------------------------------------------------------------------- *
 *                                   OpenMM                                   *
 * -------------------------------------------------------------------------- *
 * This is part of the OpenMM molecular simulation toolkit originating from   *
 * Simbios, the NIH National Center for Physics-Based Simulation of           *
 * Biological Structures at Stanford, funded under the NIH Roadmap for        *
 * Medical Research, grant U54 GM072970. See https://simtk.org.               *
 *                                                                            *
 * Portions copyright (c) 2008-2009 Stanford University and the Authors.      *
 * Authors: Peter Eastman                                                     *
 * Contributors:                                                              *
 *                                                                            *
 * Permission is hereby granted, free of charge, to any person obtaining a    *
 * copy of this software and associated documentation files (the "Software"), *
 * to deal in the Software without restriction, including without limitation  *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,   *
 * and/or sell copies of the Software, and to permit persons to whom the      *
 * Software is furnished to do so, subject to the following conditions:       *
 *                                                                            *
 * The above copyright notice and this permission notice shall be included in *
 * all copies or substantial portions of the Software.                        *
 *                                                                            *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    *
 * THE AUTHORS, CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,    *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR      *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE  *
 * USE OR OTHER DEALINGS IN THE SOFTWARE.                                     *
 * -------------------------------------------------------------------------- */

#include "Integrator.h"
#include "openmm/Kernel.h"
#include "internal/windowsExport.h"

namespace OpenMM {

/**
 * This is an error contolled, variable time step Integrator that simulates a System using the
 * velocity Verlet algorithm.  It compares the result of the Verlet integrator to that of an
 * explicit Euler integrator, takes the difference between the two as a measure of the integration
 * error in each time step, and continuously adjusts the step size to keep the error below a
 * specified tolerance.  This allows it to take larger steps on average than a fixed step size
 * integrator, while still maintaining comparable accuracy and stability.
 *
 * It is best not to think of the value of the accuracy parameter as having any absolute meaning.
 * It is just an adjustable parameter that affects the step size and integration accuracy.  You
 * should try different values to find the largest value that produces a trajectory sufficiently
 * accurate for your purposes.  0.001 is often a good starting point.
 *
 * Unlike a fixed step size Verlet integrator, variable step size Verlet is not symplectic.  This
 * means that at a given accuracy level, energy is not as precisely conserved over long time periods.
 * This makes it most appropriate for constant temperate simulations.  In constant energy simulations
 * where precise energy conservation over long time periods is important, a fixed step size Verlet
 * integrator may be more appropriate.
 */

class OPENMM_EXPORT VariableVerletIntegrator : public Integrator {
public:
    /**
     * Create a VariableVerletIntegrator.
     *
     * @param stepSize the initial step size to use (in picoseconds)
     * @param accuracy the required accuracy
     */
    VariableVerletIntegrator(double stepSize, double accuracy);
    /**
     * Get the required accuracy.
     */
    double getAccuracy() const {
        return accuracy;
    }
    /**
     * Set the required accuracy.
     */
    void setAccuracy(double acc) {
        accuracy = acc;
    }
   /**
     * Advance a simulation through time by taking a series of time steps.
     *
     * @param steps   the number of time steps to take
     */
    void step(int steps);
protected:
    /**
     * This will be called by the OpenMMContext when it is created.  It informs the Integrator
     * of what context it will be integrating, and gives it a chance to do any necessary initialization.
     * It will also get called again if the application calls reinitialize() on the OpenMMContext.
     */
    void initialize(OpenMMContextImpl& context);
    /**
     * Get the names of all Kernels used by this Integrator.
     */
    std::vector<std::string> getKernelNames();
private:
    double accuracy;
    OpenMMContextImpl* context;
    Kernel kernel;
};

} // namespace OpenMM

#endif /*OPENMM_VARIABLEVERLETINTEGRATOR_H_*/