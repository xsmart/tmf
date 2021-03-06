/*
 * 
 *  Tiny Multimedia Framework (tmf)
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This file is part of tmf.
 *
 *  tmf is free software; you can redistribute it and/or 
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  tmf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with tmf.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "core/tmf.h"

using namespace tmf;

int main(int argc, char** argv) {
  
  Pipeline* pipe = Factory::createPipeline("E-Health Sensor Sender");
  
  Filter* sensor = Factory::createFilter("ehealthsensor", "ehealthsensor");
  Filter* sender = Factory::createFilter("ehealthsender", "ehealthsender");
  
  ConfigurationManager conf("sensorsender.json", "file");
  
  sender->setProp("host", conf.getValue("host"));
  sender->setProp("userid", conf.getValue("userid"));
  sender->setProp("mode", conf.getValue("mode"));

  pipe->addFilters(sensor, sender, nullptr);

  pipe->connectFilters(sensor, sender);

  pipe->init();
  
  pipe->run();
  
  Factory::destroyPipeline(pipe);
  
  return 0;
}
