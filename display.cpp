#ifndef DISPLAY_H
#define DISPLAY_H

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <iostream>
#include <sstream>

#include "tag.h"

/*** LINE LIST FUNCTIONS
 *   Functions that relate to the linked list structure that represents the lines we print out to the consoel
 *   Each entry in the list relate to the line in the print out, and contains tracking info such as which tag "owns" that printout
 *   The position in the list of each node is the line position - 1, so the 1st node is line 1, the 2nd node is line 2
 * ****/

struct Line{
	std::string line_string;		
	Tag *tag_owner;	

};

std::vector<struct Line> line_vector = std::vector<struct Line>();
int indentation = 0; //The amount of tabs to at the start of each line list, representing the depth in the tree

/*NBT TREE FUNCTIONS 
* Functions that relate to the taking a nbt tree and generating a our line list from it
*/

//Function prototyping
void read_nbt_tree(TagCompound* root_tag);

void  add_indent(std::string & line_string)
{
	for(int i = 0; i < indentation; i++){
		line_string += "\t";	
	}
}

void get_value_string(Tag* tag,std::string & value_string) //Returns the value string for all type of tags except 9,10 (list, compound)
{
	 value_string = tag->value_string;
}

void read_nbt_tag(Tag* tag)
{

	//Make sure we have a valid tag info to use
	if(tag->tagId == 0){
		return;
	}

	line_vector.push_back(Line());	
	struct Line &line = line_vector.back();

	//Set the owner of the tag, so later we know what tag to change the value/name of
	line.tag_owner = tag;

	//Fill out all the string information for the tag
	line.line_string = std::string();
	add_indent(line.line_string);	

	//It is possible for the name to be blank, if so then we can't evaluate the pointer, now can we
	if(tag->name != 0){
		line.line_string += *(tag->name);	
	}	

	//Do the tag specific formatting
	switch(tag->tagId){
		//All these type of tags have a very similar type of print out, formated as "name" : "value"
		case 0 ... 8:
		case 11 ... 12:
		{		
			//Set the string value pointed to by line string as that string + the closing quotes for the name + the opening quotes for the next value
			line.line_string += " : \"";	

			std::string value_string;
			get_value_string(tag, value_string);
			line.line_string += value_string + "\"";

			return;
		}
		//9,10 are the compound and list tags, they have a unique format of "name", with the values being lines under it with a higher indent
		case 9:
		{
			line.line_string += " : Tag List";	
			TagList* tag_list = static_cast<TagList*>(tag);
			std::vector<Tag*> *tag_list_vector = tag_list->tag_list;
			indentation++;	
			for(int i = 0; i < tag_list_vector->size();i++){
				Tag* tag_in_list = (*(tag_list_vector))[i];
				read_nbt_tag(tag_in_list);
			}
			indentation--;
			return;
		}	
		case 10:
		{		
			line.line_string += " : Tag Compound";	
			TagCompound* tag_compound = static_cast<TagCompound*>(tag);	
			indentation++;
			read_nbt_tree(tag_compound);
			indentation--;
			return;
		}
	}	
}

void read_nbt_tree(TagCompound* root_tag)
{
	if(line_vector.size() == 0){
		//This will be our root node 
		line_vector.push_back(Line());
		struct Line & first_line = line_vector.back();

		//Set the string value
		first_line.line_string = std::string("ROOT COMPOUND");
	}
	
	std::vector<Tag*> *tag_list = root_tag->tag_list;
	for(int i = 0;i < tag_list->size();i++){
		read_nbt_tag((*tag_list)[i]);
	}
}
void print_out_lines(int start, int end = -1)
{
	if( end == - 1){
		end =  line_vector.size();
	}	
	if(end > line_vector.size()){
		end = line_vector.size();
	}
	for(int i = start; i < end;i++){
		std::cout << i << " - " << line_vector[i].line_string << "\n\r";	
	}
}

/**
 * COMMAND FUNCTIONS
 * Functions that call each command
 */
void print(std::stringstream & sstream) 
{
	//Check if there is argument present{{{{{{
	if(sstream.str().empty()){
		print_out_lines(0);	
		return;	
	}
	std::string argument;
	sstream >> argument;
	
	if(argument.empty()){
		print_out_lines(0);
		return;
	}
	
	//Check we have a range, like 10-20
	int occurence = argument.find_first_of('-');
	if(occurence != -1){
		std::string string_number_1 = argument.substr(0, occurence);
		std::string string_number_2 = argument.substr(occurence + 1);

		int int_number_1 = atoi(string_number_1.c_str());
		int int_number_2 = atoi(string_number_2.c_str()) + 1; //We add one to make it an inclusive range

		print_out_lines(int_number_1, int_number_2);		

		return;
	}

	//Check if we have a operation, like 10+5
	occurence = argument.find_first_of('+');
	if(occurence != -1){
		std::string string_number_1 = argument.substr(0, occurence);
		std::string string_number_2 = argument.substr(occurence + 1);

		int int_number_1 = atoi(string_number_1.c_str());
		int int_number_2 = atoi(string_number_2.c_str()) + 1; //Add one to make it an inclusive rangee
		
		int_number_2 += int_number_1; //Add the two numbers we parsed together

		print_out_lines(int_number_1, int_number_2);		

		return;
	}	

	//If we are left with anything else, we most likely only have a single number
	//Print around that number
	int print_start = atoi(argument.c_str());
	print_out_lines(print_start-5, print_start+5);

}
void quit(std::stringstream & sstream)
{
	exit(0);
}
//Searchs for line by word
void search(std::stringstream & sstream)
{
	std::string search_term = sstream.str();
	
	//Find out where our search term ends
	search_term = search_term.substr(search_term.find_first_of(' ') + 1);
	if(search_term.empty()){
		return;
	}	

	for(int i = 0; i < line_vector.size();i++){
		if(line_vector[i].line_string.find(search_term) != -1){
			std::cout << i << " - " << line_vector[i].line_string << "\n\r";	
		}
	}
}
//edit a line, given a number
void edit(std::stringstream & sstream)
{
	std::string string_of_line_number;
	sstream >> string_of_line_number;

	int line_number = atoi(string_of_line_number.c_str());
	if(line_number == 0){
		return;
	}

	//Check that the this is a valid line
	if(line_number >= line_vector.size() || line_number < 0){
		std::cout << "Invalid line of tag to edit" << std::endl;
	}
	Line &line = line_vector[line_number];
	Tag* tag = line.tag_owner;

	//Check that this is a valid tag that can be edited
	if(tag->tagId == 10){
		std::cout << "Cannot edit the direct value of a compound tag" << std::endl;
		return;
	}
	if(tag->tagId == 9){
		std::cout << "Cannot edit the direct value of a compound list" << std::endl;
		return;
	}	

	std::cout << "Current Value : \"" << tag->value_string << "\"" << std::endl;
	std::cout << "New Value? : ";
	
	std::string input; 
	std::cin >> input;

	std::cout << input << std::endl;
	
}


typedef void (*CommandFunction) (std::stringstream &);

#define NUMBEROFCOMMANDS 8
const char *commandNames[NUMBEROFCOMMANDS] = {
	"print",
	"p",
	"quit",
	"q",
	"search",
	"s",
	"edit",
	"e"
};
const CommandFunction commandFunctions[NUMBEROFCOMMANDS] = {
	&print,
	&print,
	&quit,
	&quit,
	&search,
	&search,
	&edit,
	&edit
};

/**
 * INTERACTIVE MODE FUNCTIONS
 * functions that relate to the interative mode, using the cursor to delete and modify nodes
 */

//Instead of manually programming in each command, we wil just call the command function

void process_command(std::string & command_line)
{
	std::stringstream sstream = std::stringstream(command_line);	
	
	std::string command_word;
	sstream >> command_word;
	
	for(int i = 0; i < NUMBEROFCOMMANDS;i++){
		if(std::string(commandNames[i]) == command_word){
			commandFunctions[i](sstream);	
			return;
		}	
	}
}

void enter_interactive_mode(TagCompound* root_compound)
{
	//Generate our line vector	
	read_nbt_tree(root_compound);

	while(true){
		std::cout << "nbteditor> ";
		
		std::string input;
		std::getline(std::cin, input);
		
		process_command(input);

	}		
}

#endif
