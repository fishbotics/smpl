#include <smpl/search/egraph_planner.h>

// standard includes
#include <chrono>

// system includes
#include <ros/console.h>
#include <sbpl/utils/key.h>

namespace sbpl {
namespace motion {

ExperienceGraphPlanner::ExperienceGraphPlanner(
    const RobotPlanningSpacePtr& pspace,
    const RobotHeuristicPtr& heur)
:
    SBPLPlanner(),
    m_pspace(pspace),
    m_heur(heur),
    m_states(),
    m_start_state(nullptr),
    m_goal_state(nullptr),
    m_graph_to_search_map(),
    m_open(),
    m_call_number(0),
    m_eps(5.0)
{
    environment_ = pspace.get();
}

ExperienceGraphPlanner::~ExperienceGraphPlanner()
{
    for (SearchState* s : m_states) {
        delete s;
    }
}

int ExperienceGraphPlanner::replan(
    double allowed_time,
    std::vector<int>* solution)
{
    int cost;
    return replan(allowed_time, solution, &cost);
}

int ExperienceGraphPlanner::replan(
    double allowed_time,
    std::vector<int>* solution,
    int* cost)
{
    ++m_call_number;
    m_expand_count = 0;

    ROS_INFO("Find path to goal");

    if (!m_start_state || !m_goal_state) {
        ROS_ERROR("Start or goal state not set");
        return 1;
    }

    m_open.clear();

    reinitSearchState(m_start_state);
    reinitSearchState(m_goal_state);

    m_goal_state->f = INFINITECOST;

    m_start_state->g = 0;
    m_start_state->f = /*m_start_state->g +*/ (unsigned int)(m_eps * m_start_state->h);
    m_open.push(m_start_state);

    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<int> succs;
    std::vector<int> costs;

    unsigned int& fgoal = m_goal_state->f;
    while (!m_open.empty()) {
        auto now = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(now - start_time).count();
        if (elapsed >= allowed_time) {
            ROS_INFO("Ran out of time");
            break;
        }

        SearchState* min_state = m_open.min();
        m_open.pop();
        ++m_expand_count;

        ROS_DEBUG("Expand state %d", min_state->state_id);

        min_state->iteration_closed = 1;

        // path to goal found
        if (min_state->f >= fgoal || min_state == m_goal_state) {
            ROS_INFO("Found path to goal");
            break;
        }

        succs.clear();
        costs.clear();
        m_pspace->GetSuccs(min_state->state_id, &succs, &costs);

        ROS_DEBUG("  %zu successors", succs.size());

        for (size_t sidx = 0; sidx < succs.size(); ++sidx) {
            int succ_state_id = succs[sidx];
            int cost = costs[sidx];

            SearchState* succ_state = getSearchState(succ_state_id);
            reinitSearchState(succ_state);

            if (succ_state->iteration_closed != 0) {
                continue;
            }

            int new_cost = min_state->g + cost;
            ROS_DEBUG("Compare new cost %d vs old cost %d", new_cost, succ_state->g);
            if (new_cost < succ_state->g) {
                succ_state->g = new_cost;
                succ_state->f = succ_state->g + m_eps * succ_state->h;
                succ_state->bp = min_state;
                if (m_open.contains(succ_state)) {
                    m_open.decrease(succ_state);
                } else {
                    m_open.push(succ_state);
                }
            }
        }
    }

    if (m_open.empty()) {
        ROS_INFO("Heap exhausted");
    }

    return 0;
}

int ExperienceGraphPlanner::replan(
    std::vector<int>* solution,
    ReplanParams params)
{
    int cost;
    return replan(params.max_time, solution, &cost);
}

int ExperienceGraphPlanner::replan(
    std::vector<int>* solution,
    ReplanParams params,
    int* cost)
{
    return replan(params.max_time, solution, cost);
}

int ExperienceGraphPlanner::force_planning_from_scratch_and_free_memory()
{
    return 0;
}

double ExperienceGraphPlanner::get_solution_eps() const
{
    return 1.0;
}

int ExperienceGraphPlanner::get_n_expands() const
{
    return m_expand_count;
}

double ExperienceGraphPlanner::get_initial_eps()
{
    return 1.0;
}

double ExperienceGraphPlanner::get_initial_eps_planning_time()
{
    return 1.0;
}

double ExperienceGraphPlanner::get_final_eps_planning_time()
{
    return 1.0;
}

int ExperienceGraphPlanner::get_n_expands_init_solution()
{
    return 0;
}

double ExperienceGraphPlanner::get_final_epsilon()
{
    return 1.0;
}

void ExperienceGraphPlanner::get_search_stats(std::vector<PlannerStats>* s)
{
}

void ExperienceGraphPlanner::set_initialsolution_eps(double initialsolution_eps)
{
}

int ExperienceGraphPlanner::set_goal(int goal_state_id)
{
    m_goal_state = getSearchState(goal_state_id);
    return 1;
}

int ExperienceGraphPlanner::set_start(int start_state_id)
{
    m_start_state = getSearchState(start_state_id);
    return 1;
}

int ExperienceGraphPlanner::force_planning_from_scratch()
{
    return 0;
}

int ExperienceGraphPlanner::set_search_mode(bool first_solution_unbounded)
{
    return 0;
}

void ExperienceGraphPlanner::costs_changed(const StateChangeQuery& state_change)
{

}

ExperienceGraphPlanner::SearchState*
ExperienceGraphPlanner::getSearchState(int state_id)
{
    if (m_graph_to_search_map.size() <= state_id) {
        m_graph_to_search_map.resize(state_id + 1, -1);
    }

    if (m_graph_to_search_map[state_id] == -1) {
        return createState(state_id);
    } else {
        return m_states[m_graph_to_search_map[state_id]];
    }
}

ExperienceGraphPlanner::SearchState*
ExperienceGraphPlanner::createState(int state_id)
{
    assert(state_id < m_graph_to_search_map.size());

    m_graph_to_search_map[state_id] = (int)m_states.size();

    SearchState* ss = new SearchState;
    ss->state_id = state_id;
    ss->call_number = 0;
    m_states.push_back(ss);

    return ss;
}

void ExperienceGraphPlanner::reinitSearchState(SearchState* state)
{
    if (state->call_number != m_call_number) {
        ROS_DEBUG("Reinitialize state %d", state->state_id);
        state->g = INFINITECOST;
        state->h = m_heur->GetGoalHeuristic(state->state_id);
        state->f = INFINITECOST;
        state->iteration_closed = 0;
        state->call_number = m_call_number;
        state->bp = nullptr;
    }
}

} // namespace motion
} // namespace sbpl
