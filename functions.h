#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include <cstdint>
#include<string>
#include <vector>
#include<map>



enum _InstructionType
{
    I_TYPE, R_TYPE
};
typedef enum _InstructionType InstructionType;

//functions declarations


///////////////////////////////////////////////////////////////          General helper functions below       ///////////////////////////////////////////////////////////////

/*  This function is used to extract number of bits specified from the 32-bit hex number specified
*   @param num which is the 32-bit hex number, start_pos the iniial position of bits to be extracted
*   and end_pos which is the final position of bits to be extracted
*   @post this functions returns a 32-bit integer of the new number with extracted bits only
*/
int bit_range(uint32_t num, int start_pos, int end_pos);
int errors_check(std::ifstream& infile);
void skip_lines(std::ofstream& outfile, int num_lines);



////////////////////////////////////////////////////////////////          R-Type functions below        ////////////////////////////////////////////////////////////////////

int R_main(uint32_t num, std::ofstream& outfile);

std::string reg_lookup(int reg_num);

/*  This function takes opcode and returns the name of instruction corresponding to the opcode
*   @param op - takes opcode as a decimal integer
*   @post returns instruction name as a string
*/
std::string R_inst_lookup(int op);


////////////////////////////////////////////////////////////////          I-Type functions below        ////////////////////////////////////////////////////////////////////

int I_main(uint32_t num, std::ofstream& outfile);

std::string I_inst_lookup(int op);

int bit_range_signed(int32_t num, int start_pos, int end_pos);

int handle_label(std::ifstream& infile, std::ofstream& outfile);
std::vector<std::string> get_vector();
std::map<int, std::string> get_map();
#endif