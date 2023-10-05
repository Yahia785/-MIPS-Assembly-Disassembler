#include "functions.h"

#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstdio>

using namespace std;

//main function
int main(int argc, char* argv[])
{
	ifstream Infile(argv[1]);
	ofstream Outfile("mips_dissasembler.asm");
	string line;
	errors_check(Infile);
	if (errors_check == 0)
	{
		if (Infile.is_open())
		{
			//check to see if there are errors in the file before dissasembling
			Infile.clear();
			Infile.seekg(0, ios::beg);

			while (getline(Infile, line))
			{
				//variable that stores the 32-bit code for each instruction line
				uint32_t num = std::stoll(line, 0, 16);

				//variable to store instruction name (eg add, addi)
				std::string inst_name;

				//variable that stroes the opcode value for each instruction
				//extracting bits 26 >> 31 to get opcode
				int op = bit_range(num, 26, 31);

				//initialize state to a default value
				InstructionType state = R_TYPE;

				//statement to determine state and instruction type
				if (op == 0)state = R_TYPE;
				else state = I_TYPE;

				//main switch statement which drives the code
				switch (state)
				{
				case R_TYPE:
					R_main(num, Outfile);
					break;

				case I_TYPE:
					I_main(num, Outfile);
					break;

				}
			}

			Infile.close();

			auto lines = get_vector();
			auto labels = get_map();

			for (int i = 0; i < lines.size(); i++)
			{
				if (labels[i] != "")
				{
					Outfile << labels[i] << std::endl;

				}

				Outfile << "\t" << lines[i] << std::endl;

			}
			Outfile.close();
		}
		else
		{
			cout << "unable to open file!" << endl;
		}
	}


}