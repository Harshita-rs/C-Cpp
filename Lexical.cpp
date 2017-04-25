#include <iostream>
#include <string>
#include <list>
#include <assert.h>

#include "Lexical.h"

bool is_character_a_space(char ch)
{
	return (ch == ' ') || (ch == '\t')|| (ch == '\r') || (ch == '\n');
}

bool is_character_a_letter(char ch)
{
	return (((ch >= 'a') && (ch <= 'z'))       // a to z
				|| ((ch >= 'A') && (ch <= 'Z')));    // A to Z
}

bool is_character_a_digit(char ch)
{
	return ((ch >= '0') && (ch <= '9'));
}

// Extracting tokens from a line.
bool extract_tokens_from_line(std::string line, int line_no, evl_tokens &tokens) 
{ // use reference to modify it
  for (size_t i = 0; i < line.size();) 
  {
    // Skip the Comments
    if (line[i] == '/') 
	{ 
      ++i;
      if ((i == line.size()) || (line[i] != '/')) 
	  {
        std::cerr << "LINE " << line_no << ": a single / is not allowed" << std::endl;
        return false;
      }
      break; // skip the rest of the line by exiting the loop
    }
    
    // skip spaces
    else if (is_character_a_space(line[i])) 
	{
      ++i; // skip this space character
      continue; // skip the rest of the iteration
    } 
    // a SINGLE token

    else if ((line[i] == '(') || (line[i] == ')') || (line[i] == '=') || (line[i] == '[') || (line[i] == ']') || (line[i] == ':') || (line[i] == ';') || (line[i] == ',')) 
	{
      
	  evl_token token;
      token.line_no = line_no;
	  token.type = evl_token::SINGLE;
      token.str = std::string(1, line[i]);
      tokens.push_back(token);

//	  std::cout << line_no << ": " << token.type <<": " << token.str << std::endl; //Printing all the tokens found on each line

      ++i; // we consumed this character
      continue; // skip the rest of the iteration
    }

	// a NAME token
    else if (is_character_a_letter(line[i]) || (line[i] == '_') || (line[i] == '\\') || (line[i] == '.')) 
	{
      size_t name_begin = i;
      for (++i; i < line.size(); ++i) 
	  {
        if (!(is_character_a_letter(line[i]) || (is_character_a_digit(line[i])) || (line[i] == '_') || (line[i] == '\\') || (line[i] == '.'))) 
		{
          break; // [name_begin, i) is the range for the token
        }
      }

      evl_token token;
      token.type = evl_token::NAME;
      token.str = line.substr(name_begin, i-name_begin);
      token.line_no = line_no;
      tokens.push_back(token);

//	  std::cout << line_no << ": " << token.type <<": " << token.str << std::endl; //Printing all the tokens found on each line
	  
    }

	// a NUMBER token
    else if (is_character_a_digit(line[i])) 
	{
      size_t num_begin = i;
      for (++i; i < line.size(); ++i) 
	  {
        if (!(is_character_a_digit(line[i]))) 
		{
          break; // [name_begin, i) is the range for the token
        }
      }

      evl_token token;
      token.type = evl_token::NUMBER;
      token.str = line.substr(num_begin, i-num_begin);
      token.line_no = line_no;
      tokens.push_back(token);	 

//	  std::cout << line_no << ": " << token.type <<": " << token.str << std::endl; //Printing all the tokens found on each line
	   
    }
    else 
	{
      std::cerr << "LINE " << line_no << ": invalid character" << std::endl;
      return false;
    }
  }
  return true; // nothing left
}

// Extracting tokens from a File.
bool extract_tokens_from_file(std::string file_name, evl_tokens &tokens) 
{ // use reference to modify it
  std::ifstream input_file(file_name.c_str());
  
  if (!input_file) 
  {
    std::cerr << "I can't read " << file_name << "." << std::endl;
    return false;
  }
  tokens.clear(); // be defensive, make it empty
  std::string line;

  for (int line_no = 1; std::getline(input_file, line); ++line_no) 
  {
    if (!extract_tokens_from_line(line, line_no, tokens))
	{
      return false;
    }
  }
  return true;
}

void display_tokens(std::ostream &out, const evl_tokens &tokens) 
{
  assert(!tokens.empty());
  for (evl_tokens::const_iterator iterator = tokens.begin(); iterator != tokens.end(); ++iterator) 
  {
    if ((*iterator).type == evl_token::SINGLE) 
	{
      std::cout << "SINGLE " << (*iterator).str << std::endl; //If token type is SINGLE
    }
    else if ((*iterator).type == evl_token::NAME) 
    {
      std::cout << "NAME " << (*iterator).str << std::endl; //If token type is NAME
    }
    else 
    {
      std::cout << "NUMBER " << (*iterator).str << std::endl; // token type is NUMBER
    }
  }
}

