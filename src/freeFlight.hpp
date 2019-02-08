//        Rockable, 3D-DEM with sphero-polyhedra
//        Copyright (C) 2016-2019  <vincent.richefeu@3sr-grenoble.fr>
//        
//        This program is free software: you can redistribute it and/or modify
//        it under the terms of the GNU General Public License as published by
//        the Free Software Foundation, either version 3 of the License, or
//        (at your option) any later version.
//        
//        This program is distributed in the hope that it will be useful,
//        but WITHOUT ANY WARRANTY; without even the implied warranty of
//        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//        GNU General Public License for more details.
//        
//        You should have received a copy of the GNU General Public License
//        along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef FREEFLIGHT_HPP
#define FREEFLIGHT_HPP

#include "vec3.hpp"
#include "quat.hpp"

// A free flight is an initial configuration (pos, vel, Q, vrot),
// a given time ti and a duration.
struct freeFlight {
  double ti;  // Initial time of the free-flight
  double duration;

  // kinematic-values at initial time ti
  vec3r pos;
  vec3r vel;
  quat Q;
  vec3r vrot;
};

#endif /* end of include guard: FREEFLIGHT_HPP */
