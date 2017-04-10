#ifndef L2FSIM_STEPPER_HPP_
#define L2FSIM_STEPPER_HPP_

#include <L2Fsim/flight_zone/flight_zone.hpp>
#include <L2Fsim/aircraft/aircraft.hpp>
#include <L2Fsim/pilot/pilot.hpp>

namespace L2Fsim {

struct stepper {
public:
    virtual ~stepper() = default;

    /**
     * Temporal integrator of the model
     * @param {flight_zone &} fz; flight zone
     * @param {aircraft &} ac; aircraft
     * @param {pilot &} pl; pilot
     * @param {double} period; period of time during which we perform integration
     */
	virtual void operator()(flight_zone &fz,
							aircraft &ac,
							pilot &pl,
							double period) = 0;
};

}

#endif
