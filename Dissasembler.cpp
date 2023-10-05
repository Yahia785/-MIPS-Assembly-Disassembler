#include "functions.h"

#include <cstdint>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

using namespace std;
std::vector<std::string> lines;
std::map<int, string> labels;

/////////////////////////////////////////////////////////////////////////////////
/*  Implementations of functions declared in header file. Please check header file
/   "functions.h" for high-level description of each function implemented here.
*//////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////          General helper functions below       ///////////////////////////////////////////////////////////////

//function for extracting specified bits
int bit_range(uint32_t num, int start_pos, int end_pos)
{
    // Calculate the number of bits to extract
    int num_bits = end_pos - start_pos + 1;

    // Calculate a mask with the desired bits set to 1
    uint32_t mask = (1 << num_bits) - 1;
    mask <<= start_pos;

    // Extract the desired bits using bitwise AND
    uint32_t result = num & mask;

    // Shift the result right by start_pos to remove any trailing 0s
    result >>= start_pos;

    return result;
}

signed int bit_range_signed(int32_t num, int start_pos, int end_pos)
{

    // Calculate the number of bits to extract
    int num_bits = end_pos - start_pos + 1;

    // Calculate a mask with the desired bits set to 1
    uint32_t mask = (1 << num_bits) - 1;
    mask <<= start_pos;

    // Extract the desired bits using bitwise AND
    uint32_t result = num & mask;

    // Cast the result to a signed integer type before shifting right
    int32_t result_signed = static_cast<int32_t>(result) << (31 - end_pos);

    // Shift the result left to restore the sign bit
    result_signed >>= (31 - end_pos);

    return static_cast<signed int>(result_signed);
}

int errors_check(ifstream& infile)
{
    std::string line;
    int num_chars = 0;
    int errors = 0;
    uint32_t num = 0;
    int line_num = 0;
    if (infile.is_open())
    {
        while (getline(infile, line))
        {
            num = std::stoll(line, 0, 16);
            if (line.length() != 8)
            {
                errors++;
                cout << "Can't dissasemble line " << errors << endl;

            }
            else if (bit_range(num, 26, 31) == 0)
            {
                if (R_inst_lookup(bit_range(num, 0, 5)) == "-1")
                {
                    errors++;
                    cout << "can't dissasemble line " << errors << endl;
                }
            }
            else if (bit_range(num, 26, 31) != 0)
            {
                if (I_inst_lookup(bit_range(num, 26, 31)) == "-1")
                {
                    errors++;
                    cout << "can't dissasemble line " << errors << endl;
                }
            }
            line_num++;
        }
    }
    return errors;
}


//takes reg number in decimal
//returns register name
std::string reg_lookup(int reg_num)
{
    std::string arr[32] = { "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5",
                            "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8","$t9",
                            "$k0", "$k1", "$gp", "$sp", "$fp", "$ra" };

    return arr[reg_num];
}

////////////////////////////////////////////////////////////////          R-Type functions below        ////////////////////////////////////////////////////////////////////

//main function for R instruction type. 
int R_main(uint32_t num, std::ofstream& outfile)
{
    int op = bit_range(num, 26, 31);

    //getting instruction name
    int funct = bit_range(num, 0, 5);
    std::string inst_name = R_inst_lookup(funct);

    //getting register
    if (funct == 0 || funct == 2)     //if shifting operation
    {
        std::string rd = reg_lookup(bit_range(num, 11, 15));
        std::string rt = reg_lookup(bit_range(num, 16, 20));
        int shamt = bit_range(num, 6, 10);
        string inst = inst_name + " " + rd + ", " + rt + ", " + to_string(shamt);
        lines.push_back(inst);
        // outfile << inst_name << " " << rd << ", " << rt << ", " << shamt << endl;
    }
    else //if arithmetic or logical operation
    {
        std::string rd = reg_lookup(bit_range(num, 11, 15));
        std::string rs = reg_lookup(bit_range(num, 21, 25));
        std::string rt = reg_lookup(bit_range(num, 16, 20));
        string inst = inst_name + " " + rd + ", " + rs + ", " + rt;
        lines.push_back(inst);
        // outfile << inst_name << " " << rd << ", " << rs << ", " << rt << endl;
    }
    return 0;
}

/*  This function has the hard-coded values of opcodes and their
*   instructions. It is going to take the opcode as decimal and
*   look through the if statements and print the string of instruction
*   name
*/
std::string R_inst_lookup(int op)
{
    std::string inst_name;

    if (op == 32)
    {
        return inst_name = "add ";
    }
    else if (op == 33)
    {
        return inst_name = "addu ";
    }
    else if (op == 36)
    {
        return inst_name = "and ";
    }
    else if (op == 39)
    {
        return inst_name = "nor ";
    }
    else if (op == 37)
    {
        return inst_name = "or ";
    }
    else if (op == 42)
    {
        return inst_name = "slt ";
    }
    else if (op == 43)
    {
        return inst_name = "sltu ";
    }
    else if (op == 0)
    {
        return inst_name = "sll ";
    }
    else if (op == 2)
    {
        return inst_name = "srl ";
    }
    else if (op == 34)
    {
        return inst_name = "sub ";
    }
    else if (op == 35)
    {
        return inst_name = "subu ";
    }
    else
    {
        return "-1";
    }
    return inst_name;
}

////////////////////////////////////////////////////////////////          I-Type functions below        ////////////////////////////////////////////////////////////////////

int I_main(uint32_t num, std::ofstream& outfile)
{
    int op = bit_range(num, 26, 31);

    //getting instruction name
    std::string inst_name = I_inst_lookup(op);

    //getting register
    if (op >= 8 && op <= 15)     //if shifting operation
    {
        if (op != 14)
        {
            std::string rt = reg_lookup(bit_range(num, 16, 20));
            std::string rs = reg_lookup(bit_range(num, 21, 25));
            signed int immediate = bit_range_signed(num, 0, 15);
            string inst = inst_name + " " + rt + ", " + rs + ", " + to_string(immediate);
            lines.push_back(inst);
            // outfile << inst_name << " " << rt << ", " << rs << ", " << immediate << endl;
        }
    }
    else if (op == 4 || op == 5) //if arithmetic or logical operation
    {
        std::string rs = reg_lookup(bit_range(num, 21, 25));
        std::string rt = reg_lookup(bit_range(num, 16, 20));
        signed int  offset = bit_range_signed(num, 0, 15);
        char hex_string[20];
        snprintf(hex_string, 10, "Addr_%04X", (lines.size() + 1 + offset) * 4);
        string inst = inst_name + " " + rs + ", " + rt + ", " + hex_string;
        lines.push_back(inst);
        //convert number to hex
        labels[lines.size() + offset] = string(hex_string);
        // lines.insert(lines.begin()+ (lines.size()+offset), to_string((lines.size() +offset)*4));
        // outfile << inst_name << " " << rs << ", " << rt << ", " << offset << endl;
    }
    else
    {
        std::string rs = reg_lookup(bit_range(num, 21, 25));
        std::string rt = reg_lookup(bit_range(num, 16, 20));
        signed int offset = bit_range_signed(num, 0, 15);
        string inst = inst_name + " " + rt + ", " + to_string(offset) + "(" + rs + ")";
        lines.push_back(inst);
        // outfile << inst_name << " " << rt << ", " << offset << "(" << rs << ")" << endl;
    }
    return 0;

}


std::string I_inst_lookup(int op)
{
    std::string inst_name;

    if (op == 8)
    {
        return inst_name = "addi ";
    }
    else if (op == 9)
    {
        return inst_name = "addiu ";
    }
    else if (op == 12)
    {
        return inst_name = "andi ";
    }
    else if (op == 4)
    {
        return inst_name = "beq ";
    }
    else if (op == 5)
    {
        return inst_name = "bne ";
    }
    else if (op == 36)
    {
        return inst_name = "lbu ";
    }
    else if (op == 37)
    {
        return inst_name = "lhu ";
    }
    else if (op == 48)
    {
        return inst_name = "ll ";
    }
    else if (op == 15)
    {
        return inst_name = "lui ";
    }
    else if (op == 35)
    {
        return inst_name = "lw ";
    }
    else if (op == 13)
    {
        return inst_name = "ori ";
    }
    else if (op == 10)
    {
        return inst_name = "slti ";
    }
    else if (op == 11)
    {
        return inst_name = "sltiu ";
    }
    else if (op == 40)
    {
        return inst_name = "sb ";
    }
    else if (op == 56)
    {
        return inst_name = "sc ";
    }
    else if (op == 41)
    {
        return inst_name = "sh ";
    }
    else if (op == 43)
    {
        return inst_name = "sw ";
    }
    else
    {
        return "-1";
    }
    return inst_name;
}
std::vector<std::string> get_vector()
{
    return lines;
}
std::map<int, string> get_map()
{
    return labels;
}