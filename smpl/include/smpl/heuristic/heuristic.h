////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2016, Andrew Dornbush
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     1. Redistributions of source code must retain the above copyright notice
//        this list of conditions and the following disclaimer.
//     2. Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//     3. Neither the name of the copyright holder nor the names of its
//        contributors may be used to endorse or promote products derived from
//        this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////////////////////////

/// \author Andrew Dornbush

#ifndef SMPL_ROBOT_HEURISTIC_H
#define SMPL_ROBOT_HEURISTIC_H

// project includes
#include <smpl/extension.h>
#include <smpl/types.h>

namespace smpl {

class GoalConstraint;
class DiscreteSpace;

class Heuristic : public virtual Extension
{
public:

    virtual ~Heuristic();

    bool Init(DiscreteSpace* space);

    auto GetPlanningSpace() -> DiscreteSpace*;
    auto GetPlanningSpace() const -> const DiscreteSpace*;

    virtual bool UpdateStart(int state_id);
    virtual bool UpdateGoal(GoalConstraint* goal);

public:

    DiscreteSpace* m_space = NULL;
};

class IGoalHeuristic : public virtual Extension
{
public:

    virtual ~IGoalHeuristic();
    virtual int GetGoalHeuristic(int state_id) = 0;
};

class IStartHeuristic : public virtual Extension
{
public:

    virtual ~IStartHeuristic();
    virtual int GetStartHeuristic(int state_id) = 0;
};

class IPairwiseHeuristic : public virtual Extension
{
public:

    virtual ~IPairwiseHeuristic();
    virtual int GetPairwiseHeuristic(int a_id, int b_id) = 0;
};

class IMetricGoalHeuristic : public virtual Extension
{
public:

    virtual ~IMetricGoalHeuristic();

    /// \brief Return the heuristic distance of the planning link to the start.
    ///
    /// This distance is used by the manipulation lattice to determine whether
    /// to activate context-aware actions.
    virtual auto GetMetricGoalDistance(double x, double y, double z) -> double = 0;
};

class IMetricStartHeuristic : public virtual Extension
{
public:

    virtual ~IMetricStartHeuristic();

    /// \brief Return the heuristic distance of the planning link to the goal.
    ///
    /// This distance is used by the manipulation lattice to determine whether
    /// to activate context-aware actions.
    virtual auto GetMetricStartDistance(double x, double y, double z) -> double = 0;
};

} // namespace smpl

#endif
