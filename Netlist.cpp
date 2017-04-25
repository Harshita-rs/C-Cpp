#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <assert.h>
#include <map>

#include "Lexical.h"
#include "Syntactic.h"
#include "Netlist.h"


int main(int argc, char *argv[]) 
{
  if (argc < 2) 
  {
    std::cerr << "You should provide a file name." << std::endl;
    return -1;
  }
  	evl_wires wires;
	evl_components components;
	evl_ports ports;
	evl_statements statements;
	evl_modules_map modules_map;
	evl_modules modules;


	if (!parse_evl_file(argv[1], wires, components, ports,modules))
	{
		return false;
	}

  // create output file
  std::string output_file_name = argv[1];

  output_file_name += ".netlist";

  std::ofstream output_file(output_file_name.c_str());

  if (!output_file) 
  {
    std::cerr << "I can't write " << output_file_name << "." << std::endl;
    return -1;
  }
  
 netlist netlist_1;

  if (!netlist_1.create(wires,components))
  {
	  return -1;
  }

  netlist_1.netlist_write (output_file, components, modules);

  std::cout << argv[1] << ": Info" << ": Ports " << ports.size() 
	  << ", Components " << netlist_1.components_count << ", Assigns 0" << std::endl;

  std::cout << argv[1] << ": Info: build module " << ": " << components.size() 
	  << " gates, " << netlist_1.nets_list.size() << " nets, " << netlist_1.pin_count << " pins." << std::endl;
  
  std::cout << argv[1] << ": Info: Tokens have been written into file: " << output_file_name << std::endl;
}

