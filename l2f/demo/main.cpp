#include <L2Fsim/simulation.hpp>
#include <L2Fsim/aircraft/beeler_glider/beeler_glider.hpp>
#include <L2Fsim/aircraft/beeler_glider/beeler_glider_state.hpp>
#include <L2Fsim/aircraft/beeler_glider/beeler_glider_command.hpp>
#include <L2Fsim/pilot/passive_pilot.hpp>
#include <L2Fsim/pilot/q_learning/q_learning_pilot.hpp>
#include <L2Fsim/flight_zone/flat_thermal_soaring_zone.hpp>
#include <L2Fsim/flight_zone/flat_zone.hpp>
#include <L2Fsim/stepper/euler_integrator.hpp>
#include <L2Fsim/stepper/rk4_integrator.hpp>
#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace L2Fsim;

int main() {
    /**
     * Time parameters
     * @param {double} current_time; current time (s)
     * @param {double} limit_time; limit time (s)
     * @param {double} time_step; time step (s)
     */
    srand(time(NULL));
    double limit_time = 1000.;
	double time_step = 1e-3;

    /** Choice of the environment */
    double windx=0., windy=0.;
    // read config1.txt
    //flat_zone my_zone(windx,windy);
    flat_thermal_soaring_zone my_zone("data/config1.txt");

    /** Choice of the initial state */
	double x0 = -500.; // m
	double y0 = 0.; // m
	double z0 = 1000.; // m
	double V0 = 20.; // m/s
	double gamma0 = 0.; // rad
	double khi0 = 0.; // rad
	double alpha0 = 0.; // rad
	double beta0 = 0.; // rad
	double sigma0 = 0.; // rad
	double xdot0 = 0., ydot0 = 0., zdot0 = 0., Vdot0 = 0., gammadot0 = 0., khidot0 = 0.; //unrealistic but updated at the begining of the simulation
    beeler_glider_state my_state(x0,y0,z0,V0,gamma0,khi0,alpha0,beta0,sigma0,xdot0,ydot0,zdot0,Vdot0,gammadot0,khidot0);

    /** Choice of the initial command */
    beeler_glider_command my_command;

    /** Choice of the aircraft */
	double m = 1.35;
	double ws = 2.;
	double ar = 16.;
	beeler_glider my_glider(my_state,my_command,m,ws,ar);

    /**
     * Choice of the pilot
     * @param {double} angle_rate_magnitude; rate at which the pilot can modify the angles
     * @param {double} ep; epsilon for Q-learning
     * @param {double} lr; 'learning rate' for Q-learning
     * @param {double} df; discount factor for Q-learning
     */
    double angle_rate_magnitude = .01;
    double ep = .01, lr = .1, df = .95;
    //passive_pilot my_pilot(angle_rate_magnitude);
    q_learning_pilot my_pilot(angle_rate_magnitude,ep,lr,df);

    /**
     * Choice of the stepper
     * @param {double} dt; substeps-width during an integration of width time_step
     * @warning time_step must be a multiple of dt
     */
    double dt = time_step / 1.;
	euler_integrator my_stepper(dt);
	//rk4_integrator my_stepper(dt);

    /** Initialize the simulation */
    simulation mysim;
	mysim.fz = &my_zone;
	mysim.ac = &my_glider;
	mysim.st = &my_stepper;
	mysim.pl = &my_pilot;

	/** Run the simulation */
    for(double current_time=0.; current_time<limit_time; current_time+=time_step) {
		mysim.step(time_step);
        std::cout << "t = " << current_time << std::endl;
	}
}