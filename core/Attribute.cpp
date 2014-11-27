/*
 * 
 *  Tiny Multimedia Framework
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "core/Attribute.h"

using namespace tmf;

std::string Attribute::getProp(const std::string & key) {
  auto k = props.find(key);
  
  if (k == props.end())
    return "";
  
  return props[key];
}

void Attribute::setProp(const std::string & key, const std::string& val) {
  props.insert(std::make_pair(key, val));
}

void Attribute::setProp(const std::string & key, const char* val) {
  std::string valstr = std::string(val);
  props.insert(std::make_pair(key, valstr));
}