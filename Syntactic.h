#ifndef SYNTACTIC_H
#define SYNTACTIC_H

#include <iostream>
#include <string>
#include <list>
#include <map>

#include "Lexical.h"

struct evl_statement
{
enum statement_type { MODULE, WIRE, COMPONENT, ENDMODULE};
statement_type type;
evl_tokens tokens;
}; // struct evl_statement

typedef std::list<evl_statement> evl_statements;


struct evl_wire
{
std::string name;
int width;
int line_no;
}; // struct evl_wire

typedef std::list<evl_wire> evl_wires;


typedef std::map<std::string, int> evl_wires_map;

struct evl_port
{
std::string dir;
std::string name;
int width;
int line_no;
}; // struct evl_port

typedef std::list<evl_port> evl_ports;

typedef std::map<std::string, int> evl_ports_map;

struct evl_pin
{
std::string name;
int bus_msb;
int bus_lsb;
}; // struct evl_pin

typedef std::list<evl_pin> evl_pins;


struct evl_component
{
std::string type;
std::string name;
int pin_count;
evl_pins pins;
}; // struct evl_comp

typedef std::list<evl_component> evl_components;

struct evl_module 
{
std::string name;
evl_tokens tokens;  
}; // struct evl_module

typedef std::map<std::string, int> map_evl_modules_line;

typedef std::map<std::string, evl_tokens> evl_modules_map;

typedef std::list<evl_module> evl_modules;

bool extract_tokens_from_file(std::string file_name, evl_tokens &tokens);

bool group_tokens_into_statements(evl_statements &statements, evl_tokens &tokens);

bool process_all_statements(evl_statements &statements, evl_wires &wires, evl_components &components, evl_ports &ports, evl_wires_map &wires_map, evl_ports_map &ports_map, evl_modules &modules);

void display_syntactic(std::ostream &out, const evl_statements &statements, evl_wires &wires, const evl_components &components);

bool map_wires(evl_wires &wires, evl_wires_map &wires_map);

bool map_ports(evl_ports &ports, evl_ports_map &ports_map);

bool match_pins_and_wires(evl_pin &pin, evl_wires_map &wires_map, evl_ports_map &ports_map, const evl_token &t);

bool move_tokens_to_module(evl_tokens &tokens, evl_modules_map &modules_map, map_evl_modules_line &modules_line_map);

#endif

