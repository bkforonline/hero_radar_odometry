#ifndef STEAMSOLVER_HPP
#define STEAMSOLVER_HPP

#include <vector>
#include "SteamPyHelper.hpp"

class SteamSolver {
public:
    SteamSolver(const double& dt, const unsigned int& window_size) : dt_(dt), window_size_(window_size) {
        // Make Qc_inv
        Eigen::Array<double, 1, 6> Qc_diag;
        Qc_diag << 0.3678912639416186958207788393338,
                   0.043068034591947058908889545136844,
                   0.1307444996557916849777569723301,
                   0.0073124100132336252236275875304727,
                   0.0076438703775169331705585662461999,
                   0.0021394075786459413462958778495704;
        Qc_inv_.setZero();
        Qc_inv_.diagonal() = 1.0/Qc_diag;
        // Initialize trajectory
        resetTraj();
    }
    // initialization
    void resetTraj();
    void setQcInv(const np::ndarray& Qc_inv_diag);
    void setMeas(const p::object& p2_list, const p::object& p1_list, const p::object& weight_list);
    // solve
    void optimize();
    // output
    void getPoses(np::ndarray& poses);
    void getVelocities(np::ndarray& vels);

private:
    // Solver
    typedef boost::shared_ptr<steam::SolverBase> SolverBasePtr;
    SolverBasePtr solver_;
    // States
    std::vector<TrajStateVar> states_;
    // Measurements
    std::vector<np::ndarray> p1_;  // reference
    std::vector<np::ndarray> p2_;  // frame points
    std::vector<np::ndarray> w_;   // weights
    // Constants
    double dt_;  // trajectory time step
    unsigned int window_size_;  // trajectory window size
    Eigen::Matrix<double, 6, 6> Qc_inv_;  // Motion prior inverse Qc
};

// boost wrapper
BOOST_PYTHON_MODULE(SteamSolver) {
    Py_Initialize();
    np::initialize();
    // p::def("run_simple", run_simple);
    p::class_<SteamSolver>("SteamSolver", p::init<const double&, const unsigned int&>())
        .def("resetTraj", &SteamSolver::resetTraj)
        .def("setQcInv", &SteamSolver::setQcInv)
        .def("setMeas", &SteamSolver::setMeas)
        .def("optimize", &SteamSolver::optimize)
        .def("getPoses", &SteamSolver::getPoses)
        .def("getVelocities", &SteamSolver::getVelocities);
}

#endif  // STEAMSOLVER_HPP