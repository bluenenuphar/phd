#ifndef L2FSIM_Q_LEARNING_PILOT_HPP_
#define L2FSIM_Q_LEARNING_PILOT_HPP_

#include <cstdio>
#include <cstdlib>
#include <L2Fsim/pilot/pilot.hpp>

/**
 * An online implementation of a Q-Learning algorithm
 * @version 1.0
 * @since 1.0
 *
 * @note compatibility: 'beeler_glider.hpp'; 'beeler_glider_state.hpp'; 'beeler_glider_command.hpp'
 * @note action space defined in method 'get_available_actions'
 * @note feature vector defined in method 'get_feature_vector'
 * @note reward function defined in method 'get_reward'
 */

namespace L2Fsim{

class q_learning_pilot : public pilot {
public:
    /**
     * Attributes
     * @param {beeler_glider_state} prev_s; previous state
     * @param {beeler_glider_command} prev_a; previous command
     * @param {double} angle_rate_magnitude; magnitude of the increment that one can apply to the angles
     * @param {double} epsilon; for epsilon-greedy policy
     * @param {double} lr; learning rate (aka alpha in the literature)
     * @param {double} df; discount factor (aka gamma in the literature)
     * @param {std::vector<double>} parameters; parameters vector
     */
    beeler_glider_state prev_s;
    beeler_glider_command prev_a;
    double angle_rate_magnitude;
    double epsilon;
    double lr;
    double df;
    std::vector<double> parameters;

    q_learning_pilot(
        double _angle_rate_magnitude=.0,
        double _epsilon=.01,
        double _lr=.01,
        double _df=.9) :
        prev_s(),
        prev_a(),
        angle_rate_magnitude(_angle_rate_magnitude),
        epsilon(_epsilon),
        lr(_lr),
        df(_df)
    {
        std::vector<double> phi = get_feature_vector(prev_s,prev_a);
        parameters.resize(phi.size(),0.);
    }

    /**
     * Sort the indices of the input vector
     * @param {const std::vector<double> &} v; input vector
     * @param {std::vector<unsigned int> &} up_ind; indices of the maximum values inside v
     * @param {std::vector<unsigned int> &} dw_ind; indices of the other values inside v
     */
    void sort_ind(const std::vector<double> &v,
        std::vector<unsigned int> &up_ind,
        std::vector<unsigned int> &dw_ind)
    {
        double max = *std::max_element(v.begin(),v.end());
        for (unsigned int j=0; j<v.size(); ++j) {
            if(v[j]<max) {dw_ind.push_back(j);}
            else {up_ind.push_back(j);}
        }
    }

    /**
     * Pick a random indice of the input vector, not depending on its content type
     * @param {const std::vector<content_type> &} v; input vector
     * @return {int} a random indice
     * @note initialize a random seed when executing your program
     */
    template <class content_type>
    int rnd_ind(const std::vector<content_type> &v)
    {
        return rand() % v.size();
    }

    /**
     * Evaluate the feature vector
     * @param {const beeler_glider_state &} s; state input
     * @param {const beeler_glider_command &} a; command input
     * @return {std::vector<double>} feature vector
     */
    std::vector<double> get_feature_vector(const beeler_glider_state &s, const beeler_glider_command &a)
    {
        std::vector<double> buffer, phi;
        phi.push_back(s.zdot);
        phi.push_back(s.gammadot);
        phi.push_back(s.alpha);
        phi.push_back(s.sigma);
        phi.push_back(a.dalpha);
        phi.push_back(a.dsigma);
        buffer = phi;
        phi.insert(phi.begin(),1.);
        for(unsigned int i=0; i<buffer.size(); ++i) {
            for(unsigned int j=i; j<buffer.size(); ++j) {
                phi.push_back(buffer.at(i)*buffer.at(j));
            }
        }
        return phi;
    }

    /**
     * Evaluate the Q function
     * @param {const beeler_glider_state &} s; state input
     * @param {const beeler_glider_command &} a; command input
     * @return {double} Q value
     */
    double get_q_value(const beeler_glider_state &s, const beeler_glider_command &a)
    {
        double score;
        std::vector<double> phi = get_feature_vector(s,a);
        for(unsigned int i=0; i<phi.size(); ++i) {
            score += phi.at(i) * parameters.at(i);
        }
        return score;
    }

    /**
     * Get every available actions
     * @param {std::vector<beeler_glider_command> &} av_a; a cleared vector
     */
    void get_available_actions(std::vector<beeler_glider_command> &av_a)
    {
        av_a.clear();
        av_a.push_back(beeler_glider_command(-angle_rate_magnitude,0.,-angle_rate_magnitude));
        av_a.push_back(beeler_glider_command(-angle_rate_magnitude,0.,0.));
        av_a.push_back(beeler_glider_command(-angle_rate_magnitude,0.,+angle_rate_magnitude));
        av_a.push_back(beeler_glider_command(0.,0.,-angle_rate_magnitude));
        av_a.push_back(beeler_glider_command(0.,0.,0.));
        av_a.push_back(beeler_glider_command(0.,0.,+angle_rate_magnitude));
        av_a.push_back(beeler_glider_command(+angle_rate_magnitude,0.,-angle_rate_magnitude));
        av_a.push_back(beeler_glider_command(+angle_rate_magnitude,0.,0.));
        av_a.push_back(beeler_glider_command(+angle_rate_magnitude,0.,+angle_rate_magnitude));
    }

    /**
     * Set the value of an input command accordingly to an epsilon-greedy policy given an input state
     * @param {const beeler_glider_state &} s; input state
     * @param {beeler_glider_command &} a; modified command
     */
    void epsilon_greedy_policy(const beeler_glider_state &s, beeler_glider_command &a)
    {
        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution(0.0,1.0);
        std::vector<beeler_glider_command> available_actions;
        std::vector<unsigned int> max_ind, non_max_ind;
        std::vector<double> scores;

        get_available_actions(available_actions);
        for(unsigned int i=0; i<available_actions.size(); ++i) {
            scores.push_back(get_q_value(s,available_actions.at(i)));
        }
        sort_ind(scores,max_ind,non_max_ind);

        double number = distribution(generator);
        if(number > epsilon) { // Greedy action
            a = available_actions.at(max_ind.at(rnd_ind(max_ind)));
        } else { // Random action
            a = available_actions.at(non_max_ind.at(rnd_ind(non_max_ind)));
        }
    }

    /**
     * Set the value of an input command accordingly to a greedy policy given an input state
     * @param {const beeler_glider_state &} s; input state
     * @param {beeler_glider_command &} a; modified command
     */
    void greedy_policy(const beeler_glider_state &s, beeler_glider_command &a)
    {
        std::vector<beeler_glider_command> available_actions;
        std::vector<unsigned int> max_ind, non_max_ind;
        std::vector<double> scores;

        get_available_actions(available_actions);
        for(unsigned int i=0; i<available_actions.size(); ++i) {
            scores.push_back(get_q_value(s,available_actions.at(i)));
        }
        sort_ind(scores,max_ind,non_max_ind);
        a = available_actions.at(max_ind.at(rnd_ind(max_ind)));
    }

    /**
     * Get the reward value for tuple (prev_s,prev_a,s)
     * @param {const beeler_glider_state &} s; state "t+1"
     * @param {double &} reward; reward
     * @note next state 's' not used so far
     */
    void get_reward(const beeler_glider_state &s, double &reward)
    {
        reward = prev_s.zdot + prev_s.V * prev_s.Vdot / 9.81;
    }

    /**
     * Update the parameters vector
     * @param {const double &} delta; cf q-learning update equation
     * @note update performed with tuple (prev_s,prev_a)
     */
    void update_parameters(const double &delta)
    {
        std::vector<double> phi = get_feature_vector(prev_s,prev_a);
        for (unsigned int i=0; i<parameters.size(); ++i) {
            parameters[i] += lr * delta * phi.at(i);
        }
    }

    /**
     * An online Q-Learning algorithm step and a command control are performed at each time step of the simulation
     * @param {state &} s; reference on the state
     * @param {command &} a; reference on the command
     * @warning dynamic cast of state and action
     */
	pilot & operator()(state &_s, command &_a) override
	{
        beeler_glider_state &s = dynamic_cast <beeler_glider_state &> (_s);
        beeler_glider_command &a = dynamic_cast <beeler_glider_command &> (_a);
        beeler_glider_command a_off;
        double reward, delta;

        greedy_policy(s,a_off);
        get_reward(s,reward);
        delta = reward + df * get_q_value(s,a_off) - get_q_value(prev_s,prev_a);
        update_parameters(delta);

        epsilon_greedy_policy(s,a);
        prev_s = s;
        prev_a = a;

		return *this;
	}

    /**
     * Policy for 'out of range' situations
     * @param {state &} s; reference on the state
     * @param {command &} a; reference on the command
     */
    pilot & out_of_range(state &_s, command &_a) override
    {
        beeler_glider_state &s = dynamic_cast <beeler_glider_state &> (_s);
        beeler_glider_command &a = dynamic_cast <beeler_glider_command &> (_a);
        a.dalpha = 0.;
        a.dbeta = 0.;
        if(s.sigma < 0.4) {
            a.dsigma = +angle_rate_magnitude;
        } else {
            a.dsigma = 0.;
        }
		return *this;
    }
};

}

#endif