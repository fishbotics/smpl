#include <memory>

#include <ros/node_handle.h>

namespace smpl {
class RobotModel;
class CollisionChecker;
class DiscreteSpace;
class GoalConstraint;
class Heuristic;
class Search;
} // namespace smpl

auto MakeManipLattice(
    smpl::RobotModel* model,
    smpl::CollisionChecker* checker,
    const ros::NodeHandle& nh)
    -> std::unique_ptr<smpl::DiscreteSpace>;

auto MakeWorkspaceLattice(
    smpl::RobotModel* model,
    smpl::CollisionChecker* checker,
    const ros::NodeHandle& nh)
    -> std::unique_ptr<smpl::DiscreteSpace>;

auto MakeManipLatticeEGraph(
    smpl::RobotModel* model,
    smpl::CollisionChecker* checker,
    const ros::NodeHandle& nh)
    -> std::unique_ptr<smpl::DiscreteSpace>;

auto MakeJointDistHeuristic(
    smpl::DiscreteSpace* graph,
    const ros::NodeHandle& nh)
    -> std::unique_ptr<smpl::Heuristic>;

auto MakeBFSHeuristic(
    smpl::DiscreteSpace* graph,
    const ros::NodeHandle& nh)
    -> std::unique_ptr<smpl::Heuristic>;

auto MakeMultiFrameBFSHeuristic(
    smpl::DiscreteSpace* graph,
    const ros::NodeHandle& nh)
    -> std::unique_ptr<smpl::Heuristic>;

auto MakeEuclidDistHeuristic(
    smpl::DiscreteSpace* graph,
    const ros::NodeHandle& nh)
    -> std::unique_ptr<smpl::Heuristic>;

auto MakeDijkstraEGraph3DHeuristic(
    smpl::DiscreteSpace* graph,
    const ros::NodeHandle& nh)
    -> std::unique_ptr<smpl::Heuristic>;

auto MakeARAStar(
    smpl::DiscreteSpace* graph,
    smpl::Heuristic* heuristic,
    const ros::NodeHandle& nh)
    -> std::unique_ptr<smpl::Search>;

auto MakeSMHAStar(
    smpl::DiscreteSpace* graph,
    smpl::Heuristic* anchor,
    smpl::Heuristic** heurs,
    int num_heuristics,
    const ros::NodeHandle& nh)
    -> std::unique_ptr<smpl::Search>;

auto MakePoseGoal(
    smpl::DiscreteSpace* graph,
    const ros::NodeHandle& nh)
    -> std::unique_ptr<smpl::GoalConstraint>;

auto MakeJointStateGoal(
    smpl::DiscreteSpace* graph,
    const ros::NodeHandle& nh)
    -> std::unique_ptr<smpl::GoalConstraint>;