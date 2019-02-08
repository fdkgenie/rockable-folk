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

#include <iostream>
#include <utility>

#include "message.hpp"

#include "ContactPartnership.hpp"
#include "Rockable.hpp"

ContactPartnership::ContactPartnership() : name("None"), update(nullptr), getWeight(nullptr) {}
ContactPartnership::~ContactPartnership() {}

void ContactPartnership::setModel(std::string& modelName) {
  if (modelName == "NumberWeight") {
    name = modelName;
    std::cout << "ContactPartnership has been set to 'NumberWeight'" << std::endl;

    update = [this](Rockable& box) -> void {
      weightMap.clear();

      // FIXME: The following cannot be easily parallelized because of
      //        concurrent access to 'weightMap'

      //#pragma omp parallel for default(shared)
      for (size_t k = 0; k < box.Interactions.size(); ++k) {
        for (auto it = box.Interactions[k].begin(); it != box.Interactions[k].end(); ++it) {
          Interaction* I = const_cast<Interaction*>(std::addressof(*it));
          if (I->stick != nullptr || I->dn >= 0.0) continue;
          size_t i = I->i;
          size_t j = I->j;
          if (j < i) std::swap(i, j);
          std::pair<size_t, size_t> p(i, j);

          auto ref = weightMap.find(p);
          if (ref != weightMap.end()) {
            //#pragma omp atomic
            ref->second += 1.0;
          } else {
            //#pragma omp critical(addInMap)
            weightMap[p] = 1.0;
          }
        }
      }

      // todo: replace by inverse so that the getWeight is faster
    };

    getWeight = [this](Interaction& I) -> double {
      size_t i = I.i;
      size_t j = I.j;
      if (j < i) std::swap(i, j);
      std::pair<size_t, size_t> p(i, j);
      auto ref = weightMap.find(p);
      if (ref != weightMap.end()) {
        return (1.0 / ref->second);
      }
      return 1.0;
    };

  } else if (modelName == "OverlapWeight") {
    name = modelName;
    std::cout << "ContactPartnership has been set to 'OverlapWeight'" << std::endl;

    update = [this](Rockable& box) -> void {
      weightMap.clear();

      //#pragma omp parallel for default(shared)
      for (size_t k = 0; k < box.Interactions.size(); ++k) {
        for (auto it = box.Interactions[k].begin(); it != box.Interactions[k].end(); ++it) {
          Interaction* I = const_cast<Interaction*>(std::addressof(*it));
          if (I->stick != nullptr || I->dn >= 0.0) continue;
          size_t i = I->i;
          size_t j = I->j;
          if (j < i) std::swap(i, j);
          std::pair<size_t, size_t> p(i, j);

          auto ref = weightMap.find(p);
          if (ref != weightMap.end()) {
            //#pragma omp atomic
            ref->second += I->dn;
          } else {
            //#pragma omp critical(addInMap)
            weightMap[p] = I->dn;
          }
        }
      }

      // todo: replace by inverse so that the getWeight is faster
    };

    getWeight = [this](Interaction& I) -> double {
      size_t i = I.i;
      size_t j = I.j;
      if (j < i) std::swap(i, j);
      std::pair<size_t, size_t> p(i, j);
      auto ref = weightMap.find(p);
      if (ref != weightMap.end()) {
        return (I.dn / ref->second);
      }
      return 1.0;
    };

  } else if (modelName == "SurfaceWeight") {
    name = modelName;
    std::cout << "ContactPartnership has been set to 'SurfaceWeight'" << std::endl;
    ///////////// THAT ONE IS NOT YET IMPLEMENTED ////////////
    ///////////// and maybe it will never be!     ////////////
  } else if (modelName == "None") {
    name = modelName;
    std::cout << "No Partnership has been set" << std::endl;
    update = nullptr;
    getWeight = nullptr;
  } else {
    std::cout << "ContactPartnership '" << modelName << "' is unknown. So no Partnership has been set" << std::endl;
    modelName = "Unknown";
    update = nullptr;
    getWeight = nullptr;
  }
}
