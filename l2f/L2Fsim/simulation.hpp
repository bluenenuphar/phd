#ifndef L2FSIM_SIMULATION_HPP_
#define L2FSIM_SIMULATION_HPP_

#include <L2Fsim/flight_zone/flight_zone.hpp>
#include <L2Fsim/aircraft/aircraft.hpp>
#include <L2Fsim/stepper/stepper.hpp>
#include <L2Fsim/pilot/pilot.hpp>

/**
 * Simulation environment
 * @version 1.1
 * @since 1.0
 *
 * Every choices concerning the simulation environment e.g. aircraft, pilot, environment etc.
 * are made in the initialization of a 'simulation' object
 */

namespace L2Fsim {

class simulation{
public:
	/**
	 * Attributes
	 * @param {flight_zone *} fz; pointer to a flight zone
	 * @param {aircraft *} ac; pointer to an aircraft
	 * @param {stepper *} st; pointer to a stepper
	 * @param {pilot *} pl; pointer to a pilot
	 */
	flight_zone *fz;
	aircraft *ac;
	stepper *st;
	pilot *pl;

	/**
	 * Stepping function
	 */
	void step(double dt) {
		(*st)(*fz, *ac, *pl, dt);
	}
};

}

#endif
