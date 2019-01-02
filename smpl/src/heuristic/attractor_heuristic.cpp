////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017, Andrew Dornbush
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

#include <smpl/heuristic/attractor_heuristic.h>

// standard includes
#include <cmath>

// project includes
#include <smpl/graph/discrete_space.h>

namespace smpl {

bool AttractorHeuristic::Init(DiscreteSpace* space)
{
    auto* extract_state = space->GetExtension<IExtractRobotState>();
    if (extract_state == NULL) return false;

    if (!Heuristic::Init(space)) {
        return false;
    }

    m_extract_state = extract_state;
    return true;
}

int AttractorHeuristic::GetGoalHeuristic(int state_id)
{
    auto& state = m_extract_state->ExtractState(state_id);
    assert(state.size() == m_attractor.size());

    auto dsum = 0.0;
    for (auto i = 0; i < state.size(); ++i) {
        double dj = (state[i] - m_attractor[i]);
        dsum += dj * dj;
    }
    dsum = FIXED_POINT_RATIO * std::sqrt(dsum);
    return (int)dsum;
}

auto AttractorHeuristic::GetExtension(size_t class_code) -> Extension*
{
    if (class_code == GetClassCode<Heuristic>() ||
        class_code == GetClassCode<IGoalHeuristic>())
    {
        return this;
    }
    return NULL;
}

} // namespace smpl
