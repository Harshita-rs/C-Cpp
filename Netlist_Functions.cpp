#include <iostream>
#include <string>
#include <list>
#include <map>
#include <stddef.h>
#include <sstream>
#include <assert.h>
#include <fstream>

#include "Lexical.h"
#include "Syntactic.h"
#include "Netlist.h"

void netlist::create_net(std::string net_name) 
{
	nets_list.push_back(net_name);
}

bool netlist::create_nets(const evl_wires &wires) 
{
	
	for (evl_wires::const_iterator iter = wires.begin(); iter != wires.end(); ++iter)
	{
		if (iter->width == 1) 
		{
			create_net(iter->name);
		}
		else 
		{
			for (int i = 0; i < iter->width ; ++i) 
			{
				create_net(make_net_name(iter->name,i)); 			
			}
		}
	}
	return true;
}

bool netlist::create_gates(const evl_components &components)
{
	for (evl_components::const_iterator iter_c = components.begin(); iter_c != components.end(); ++iter_c)
	{
		gates_.push_back (new gate);
		gates_.back()->create(*iter_c, nets_);
	}
	return true;
}

bool netlist::create(const evl_wires &wires, const evl_components &comps) 
{
	bool ret_value;

	create_nets(wires); 
	create_gates(comps); 

	wires_count = 0;

	ret_value = create_connects(comps, nets_list, gate_pin_map_, pins_list_, plist);

	wires_count = nets_list.size();

	return ret_value;
}

bool netlist::semantics_error (const std::string &type_, const std::string &name_)
{
	std::cerr << "Error: Number of pins for " << type_ << " gate: " << name_ << " is invalid!" << std::endl;

	return false;
}

bool netlist::structural_semantics (const std::string &type_, const std::string &name_, const evl_pins &pins_, string_2_list &predef_gates_type)  
{
  if (type_ == "and") 
  {
    if (pins_.size() < 3) 
	{
  	  return semantics_error (type_, name_);
	}  

	evl_pins::const_iterator iter_p = pins_.begin(); // The first pin is an output pin

	predef_gates_type[(*iter_p).name] = "output";

	for (evl_pins::const_iterator iter_p = pins_.begin()++; iter_p != pins_.end(); ++iter_p) // All pins except the first are input pins
	{
	  predef_gates_type[(*iter_p).name] = "input";
    }
  }  
  
  else if (type_ == "or") 
  {
    if (pins_.size() < 3) 
	{
      return semantics_error (type_, name_);
	} 

	evl_pins::const_iterator iter_p = pins_.begin(); // The first pin is an output pin

	predef_gates_type[(*iter_p).name] = "output";

	for (evl_pins::const_iterator iter_p = pins_.begin()++; iter_p != pins_.end(); ++iter_p) // All pins except the first are input pins
	{
	  predef_gates_type[(*iter_p).name] = "input";
    }
  }  
  
  else if (type_ == "xor") 
  {
    if (pins_.size() < 3) 
	{
      return semantics_error (type_, name_);
	}  

	evl_pins::const_iterator iter_p = pins_.begin(); // The first pin is an output pin

	predef_gates_type[(*iter_p).name] = "output";

	for (evl_pins::const_iterator iter_p = pins_.begin()++; iter_p != pins_.end(); ++iter_p) // All pins except the first are input pins
	{
	  predef_gates_type[(*iter_p).name] = "input";
    }
  }  

  else if (type_ == "not") 
  {
    if (pins_.size() != 2) 
	{
      return semantics_error (type_, name_);
	}  
	evl_pins::const_iterator iter_p = pins_.begin(); // The first pin is an output pin

	predef_gates_type[(*iter_p).name] = "output";

	iter_p = pins_.begin()++;	// The second pin is an input pin

	predef_gates_type[(*iter_p).name] = "input";
  }
  
  else if (type_ == "buf") 
  {
    if (pins_.size() != 2) 
	{
      return semantics_error (type_, name_);
	}  
	evl_pins::const_iterator iter_p = pins_.begin(); // The first pin is an output pin

	predef_gates_type[(*iter_p).name] = "output";

	iter_p = pins_.begin()++;	// The second pin is an input pin

	predef_gates_type[(*iter_p).name] = "input";
  }
  
  else if (type_ == "dff") 
  {
    if (pins_.size() != 2) 
	{
      return semantics_error (type_, name_);
	}  

	evl_pins::const_iterator iter_p = pins_.begin(); // The first pin is an output pin

	predef_gates_type[(*iter_p).name] = "output";

	iter_p = pins_.begin()++;	// The second pin is an input pin

	predef_gates_type[(*iter_p).name] = "input";
  }
  
  else if (type_ == "one") 
  {
    if (pins_.size() < 1) 
	{
      return semantics_error (type_, name_); 
	}  

	for (evl_pins::const_iterator iter_p = pins_.begin(); iter_p != pins_.end(); ++iter_p) // All pins are output pins
	{
	  predef_gates_type[(*iter_p).name] = "output";
    }
  }  
  
  else if (type_ == "zero") 
  {
    if (pins_.size() < 1) 
	{
      return semantics_error (type_, name_);
	}  
	
	for (evl_pins::const_iterator iter_p = pins_.begin(); iter_p != pins_.end(); ++iter_p)	// All pins are output pins
	{
	  predef_gates_type[(*iter_p).name] = "output";
    }
  }  
  
  else if (type_ == "input") 
  {
    if (pins_.size() < 1) 
	{
      return semantics_error (type_, name_);
	}  

	for (evl_pins::const_iterator iter_p = pins_.begin(); iter_p != pins_.end(); ++iter_p) // All pins are output pins
	{
	  predef_gates_type[(*iter_p).name] = "output";
    }
  }  
 
  else if (type_ == "output") 
  {
    if (pins_.size() < 1) 
	{
      return semantics_error (type_, name_);
	}  
	
	for (evl_pins::const_iterator iter_p = pins_.begin(); iter_p != pins_.end(); ++iter_p)  // All pins are input pins
	{
	  predef_gates_type[(*iter_p).name] = "input";
    }
  }  
  return true;
}

void netlist::create_pins_list(const evl_pins &pins, vector_string &comp_pins_, vector_int &pin_position)
{
  size_t index = 0;

  for (evl_pins::const_iterator iter_p = pins.begin(); iter_p != pins.end(); ++iter_p) 
  {
    if ((*iter_p).bus_msb == -1)		// a 1-bit wire
	{ 
      comp_pins_.push_back((*iter_p).name);
	  pin_position.push_back(index);
    }
    else if ((*iter_p).bus_msb == (*iter_p).bus_lsb)	// a 1-bit wire
	{ 
      comp_pins_.push_back((make_net_name((*iter_p).name,(*iter_p).bus_msb)));
	  pin_position.push_back(index);
    }
    else	// a bus
	{
      for (int i = (*iter_p).bus_lsb; i <= (*iter_p).bus_msb; ++i) 
	  {
        comp_pins_.push_back((make_net_name((*iter_p).name,i)));
	    pin_position.push_back(index);
      }
    }
	index++;
  }
}

bool netlist::create_connects(const evl_components &components, const string_list &nets_list_, gate_pin_map &gate_pin_map_, pins_list &pins_list_, pin_lit_  &pllist)
{
	gate_pin gate_pin_;
	pin_list pl;
	
	
for (string_list::const_iterator iter_n = nets_list_.begin(); iter_n != nets_list_.end(); ++iter_n) 
  {
	string_list evl_pins_n_;
    for (evl_components::const_iterator iter_c = components.begin(); iter_c != components.end(); ++iter_c) 
	{
		vector_int pin_position;
		vector_string comp_pins_;
		
		create_pins_list((*iter_c).pins, comp_pins_, pin_position);

		string_2_list predef_gates_type;
	   
		if(!structural_semantics((*iter_c).type, (*iter_c).name, (*iter_c).pins, predef_gates_type)) //Check for semantics
		{
			return false;
		}

	   for (size_t i = 0; i < comp_pins_.size(); ++i) 
	   {
	    if(*iter_n == comp_pins_[i]) 
		{
	 	  
		  gate_pin_.name = (*iter_c).name;
		  gate_pin_.type = (*iter_c).type;

		  gate_pin_.pin_position = pin_position[i];

		  std::ostringstream pin_index_str;

          pin_index_str << pin_position[i];

		  std::string pin_net_map = *iter_n + (*iter_c).name + (*iter_c).type + pin_index_str.str();
		  
		  evl_pins_n_.push_back(pin_net_map);

		  gate_pin_map_.insert(std::make_pair(pin_net_map, gate_pin_));
		}
      }
    }
	pl.name = *iter_n;
	pl.value = evl_pins_n_;
	pllist.push_back(pl);
    //pins_list_.insert(std::make_pair(*iter_n, evl_pins_n_));	
  }

  return 1;
}

void netlist::netlist_write(std::ostream &out, const evl_components &components,  const evl_modules &modules) // Writing into output file
 { 
	//module_nm  mnm;
	/*for (evl_statements::const_iterator iter = statements.begin(); iter != statements.end(); ++iter)
	{
		if ((*iter).type == evl_statement::MODULE)
		{
			for (evl_tokens::const_iterator iter1 = (*iter).tokens.begin(); iter1 != (*iter).tokens.end(); ++iter1)
			{
					out << (*iter1).str << " ";
				
			}
			//out << wires.size() << " " << components.size() << std::endl;
		}
	}*/

   components_count = components.size();
   for (evl_modules::const_iterator iter_mod = modules.begin(); iter_mod != modules.end(); ++iter_mod)
   {
	   out << "module " << iter_mod->name << std::endl;
   }
   
   //out << "module " << mnm << std::endl;

   out << "nets" << " " << nets_list.size() << std::endl;
   for (pin_lit_::const_iterator iter_p = plist.begin(); iter_p != plist.end(); ++iter_p)
   {
		out << "  net " << iter_p->name << " " << iter_p->value.size() << std::endl;

		for (string_list::const_iterator iter_s = iter_p->value.begin(); iter_s != iter_p->value.end(); ++iter_s)
		{
			// out << "    " << gate_pin_map_[*iter_s].type << " "<< gate_pin_map_[*iter_s].pin_position << std::endl;
	    	
		out << "    " << gate_pin_map_[*iter_s].type << " " << gate_pin_map_[*iter_s].name << " "<< gate_pin_map_[*iter_s].pin_position << std::endl;
		}
   }
    
    std::string pin_name;
	std::list<std::string> pin_names;
	pin_count = 0;
 out << "components" << " " << components.size() << std::endl;
  for (evl_components::const_iterator iter_c = components.begin(); iter_c != components.end(); ++iter_c) 
  {
         if((*iter_c).name == "NONE")
            {
          out << "  " <<"component " << (*iter_c).type << " " << (*iter_c).pins.size() << std::endl;
              }
         else
           out << "  " <<"component " << (*iter_c).type << " " << (*iter_c).name << " " << (*iter_c).pins.size() << std::endl;

	pin_count += (*iter_c).pins.size();
	
    for (evl_pins::const_iterator iter_p = (*iter_c).pins.begin() ; iter_p != (*iter_c).pins.end(); ++iter_p) 
	{
	  int pin_width;

	  if((*iter_p).bus_msb == -1) 
	  {
	    pin_width = 1;

	    out<< "    pin " << pin_width << " " << (*iter_p).name << std::endl;
	  }
	  else if((*iter_p).bus_msb == (*iter_p).bus_lsb) 
	  {
	    pin_width = 1;
      
		pin_name = make_net_name((*iter_p).name, (*iter_p).bus_msb);

        out << "    pin " << pin_width << " " << pin_name << std::endl;
	  }
	  else 
	  {
	    pin_width = (*iter_p).bus_msb - (*iter_p).bus_lsb + 1;

        out << "    pin " << pin_width;

		for (int i = (*iter_p).bus_lsb; i <= (*iter_p).bus_msb; ++i) 
		{
			std::string pin_names_temp;

			pin_names_temp = make_net_name((*iter_p).name,i); 
			
			pin_names.push_back(pin_names_temp);

			out << " " << pin_names_temp;

		}

	    out << std::endl;
				
	  } 
    } 
  }	
}

bool gate::create(const evl_component &component, std::map<std::string, net *> &netlist_nets) 
{
	// store gate type and name;
	size_t pin_index = 0;

	for (evl_pins::const_iterator iter_p = component.pins.begin(); iter_p != component.pins.end(); ++iter_p)
	{
		create_pin(*iter_p, pin_index, netlist_nets);
		++pin_index;
	}
	return 1;
}

bool gate::create_pin(const evl_pin &p, size_t pin_index, std::map<std::string, net *> &netlist_nets) 
{
	pins_.push_back(new pin);

	return pins_.back()->create(this, pin_index, p, netlist_nets);
}

std::string make_net_name(std::string wire_name, int i) 
{
  assert(i >= 0);

  std::ostringstream oss;

  oss << wire_name << "[" << i << "]";

  return oss.str();
}

bool pin::create(gate *g, size_t pin_index, const evl_pin &p, std::map<std::string, net *> &netlist_nets) 
{
	//  store g and pin_index;

	if (p.bus_msb == -1) // a 1-bit wire
	{ 
		nets_.push_back(netlist_nets[p.name]); 
	}
	else   // a bus
	{ 
		//size_t index = 0;

		for(int i = p.bus_lsb; i <= p.bus_msb; ++i)
		{
			nets_.push_back(netlist_nets[make_net_name(p.name,i)]);
		}
	}
	return true;
}

void net::append_pin(pin *p) 
{
	connections_.push_back(p);
	
}

bool parse_evl_file(std::string evl_file, evl_wires &wires, evl_components &comps, evl_ports &ports, evl_modules &modules)
{
	evl_tokens tokens;
	evl_statements statements;
	evl_wires_map wires_map;
	evl_ports_map ports_map;
	evl_modules_map modules_map;
	map_evl_modules_line modules_line_map;
	

	if (!extract_tokens_from_file(evl_file, tokens)) 
	{
		return false;
	}
  
  int token_count = tokens.size();

  if (!move_tokens_to_module(tokens, modules_map, modules_line_map)) 
  {
		 return false;
  }
 
  std::cout << std::endl; //Displaying summary of results
  std::cout << evl_file << ": Info: Total token count = " << token_count << "." << std::endl;
  std::cout << evl_file << ": Info: Module(s) count = " << modules_map.size() << "." << std::endl << std::endl;

  for (evl_modules_map::iterator iter = modules_map.begin(); iter != modules_map.end(); ++iter) 
  {
    statements.clear();
	wires.clear();
	comps.clear();
	ports.clear();
	wires_map.clear();
	ports_map.clear();

    if (!group_tokens_into_statements(statements, iter->second)) 
	{
      return false;
    }
    evl_statements statements1(statements.begin(), statements.end());
  
    if (!process_all_statements(statements, wires, comps, ports, wires_map, ports_map,modules)) 
	{
      return false;
	}
  }
  return 1;
}

