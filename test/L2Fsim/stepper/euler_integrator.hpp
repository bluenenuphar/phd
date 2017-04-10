#ifndef L2FSIM_EULER_INTEGRATOR_HPP_
#define L2FSIM_EULER_INTEGRATOR_HPP_

#include <L2Fsim/stepper/stepper.hpp>
#include <L2Fsim/utils/utils.hpp>
#include <cstdio>
#include <cstdlib>

namespace L2Fsim {

class euler_integrator : public stepper {
public:
    /**
     * Attributes
     * @param {double} time; current time of the simulation
     * @param {double} dt; integration step
     * @param {std::string} file_name1; file name for the log
     * @param {std::string} file_name2; file name for the log
     */
    double time;
    double dt;
    std::string state_log_filename = "data/data_plane.txt";

	/** Constructor */
	euler_integrator(double _dt=.001) :
        dt(_dt) {
        time = 0.;
    }

    /**
     * Euler update operator
     * Modifies the static variables of the state according to the corresponding simulation configuration
     * @warning The dynamic part of the state is set to the value used for the update (cf Euler, RK4, etc.)
     * @param {aircraft &} ac; aircraft
     * @param {flight_zone &} fz; flight zone
     * @param {const double} time; time step
     * @param {const double} dt; integration step
     */
    void euler_update(
        aircraft &ac,
        flight_zone &fz,
        const double time,
        const double dt)
    {
        ac.update_state_dynamic(fz,time,ac.get_state());
        ac.get_state().apply_dynamic(dt);
        //ac.get_state().print(); // TODO remove
    }

    /**
     * Stepping operator
     * @param {flight_zone &} fz; flight zone
     * @param {aircraft &} ac; aircraft
     * @param {pilot &} pl; pilot
     * @param {double} period; period of time during which we perform integration
     */
    void operator()(
        flight_zone &fz,
        aircraft &ac,
        pilot &pl,
        double period) override
    {
        /// 1. Apply the policy
        if (ac.get_distance_to_center() > 1200.) {
            pl.out_of_range(ac.get_state(),ac.get_command());
        } else {
            pl(ac.get_state(),ac.get_command());
        }

        /// 2. Save the data
        ac.get_state().save(state_log_filename,time);

        /// 3. Apply the command to the aircraft i.e. modify its state attribute
        ac.apply_command();

        /// 4. Perform Euler updates
        for(int n=0; n<period/dt; ++n) {
            euler_update(ac,fz,time,dt);
            time += dt;
        }

        /// 5. Check aircraft's configuration validity
        ac.is_in_model();
    }
};

}

#endif
