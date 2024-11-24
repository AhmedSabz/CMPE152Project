#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <cctype>
using namespace std;
class ProgramTerminatedException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Program terminated";
    }
};

class AssemblyInterpreter {
private:
	std::unordered_map<std::string, int> registers;
	string g = "main";
	string exit="exit";
	std::unordered_set<std::string> valid_instructions = {"mov","MOV", "add","ADD" ,"sub","SUB", "mul","MUL", "addi","ADDI", "subi","SUBI", "li"};
	void validate_instruction(string& instruction, int line_num){
		string part;
		istringstream iss(instruction);
		vector<string>parts;
		while(iss >>part){
			parts.push_back(part);
		}
		string op= parts[0];

		if(!valid_instructions.count(op)){
			throw std::invalid_argument("Error on line " + std::to_string(line_num) + " Invalid operation " + "'");

		}
		if (op == "MOV" || op == "mov") {
			// Check for too many operands
			if (parts.size() != 3) {
				throw std::invalid_argument("Error on line " + std::to_string(line_num) + ": " + op + " requires exactly 2 operands");

			}
			if(!registers.count(parts[1])){
				throw std::invalid_argument("Error on line " + std::to_string(line_num) + ": Invalid register '" + parts[1] + "'");

			}

			if(!registers.count(parts[2])){
				throw std::invalid_argument("Error on line " + std::to_string(line_num) + ": Invalid register '" + parts[1] + "'");

			}
			registers[parts[1]]= registers[parts[2]];
		}

		else if (op == "li") {
			// Check for too many operands
			if (parts.size() != 3) {
				throw std::invalid_argument("Error on line " + std::to_string(line_num) + ": " + op + " requires exactly 2 operands");

			}
			if(!registers.count(parts[1])){
				throw std::invalid_argument("Error on line " + std::to_string(line_num) + ": Invalid register '" + parts[1] + "'");

			}

			if(!stoi(parts[2])){
				throw std::invalid_argument("Error on line " + std::to_string(line_num) + ": Invalid immediate value '" + parts[1] + "'");

			}
			registers[parts[1]]=stoi(parts[2]);
		}
		else if (op == "add" ||op =="ADD" || op == "sub" || op=="SUB" || op == "mul" || op == "MUL") {
			if (parts.size() != 4) {
				throw std::invalid_argument("Error on line " + std::to_string(line_num) + ": " + op + " requires exactly 3 operands");
			}
			if(!registers.count(parts[1])){
				throw std::invalid_argument("Error on line " + std::to_string(line_num) + ": Invalid register '" + parts[1] + "'");

			}
			if(!registers.count(parts[2])){
				throw std::invalid_argument("Error on line " + std::to_string(line_num) + ": Invalid register '" + parts[2] + "'");

			}
			if(!registers.count(parts[3])){
				throw std::invalid_argument("Error on line " + std::to_string(line_num) + ": Invalid register '" + parts[3] + "'");

			}
			int val1 = registers[parts[2]];
			int val2 = registers[parts[3]];

			if (op == "add" || op == "ADD") {
				registers[parts[1]] = val1 + val2;
			} else if (op == "sub" || op == "SUB") {
				registers[parts[1]] = val1 - val2;
			} else if (op == "mul" || op == "MUL") {
				registers[parts[1]] = val1 * val2;
			}

		}
		else if(op == "addi" || "ADDI" || "subi" || "SUBI"){
			if (parts.size() != 4) {
				throw std::invalid_argument("Error on line " + std::to_string(line_num) + ": " + op + " requires exactly 3 operands");
			}
			if(!registers.count(parts[1])){
				throw std::invalid_argument("Error on line " + std::to_string(line_num) + ": Invalid register '" + parts[1] + "'");

			}
			if(!registers.count(parts[2])){
				throw std::invalid_argument("Error on line " + std::to_string(line_num) + ": Invalid register '" + parts[2] + "'");

			}
			if(!stoi(parts[3])){
				throw std::invalid_argument("Error on line " + std::to_string(line_num) + ": Invalid immediate value '" + parts[3] + "'");

			}
			int val1= registers[parts[2]];
			int val2=stoi(parts[3]);
			if (op == "addi" || op == "ADDI") {
						registers[parts[1]] = val1 + val2;
					} else if (op == "subi" || op == "SUBI") {
						registers[parts[1]] = val1 - val2;
					}
		}

	}
public:
	void run_program(string& program){
		std::string line;
		int lineNum=0;
		istringstream iss(program);
		while(getline(iss,line)){
			lineNum++;
			if(!line.empty()){
				validate_instruction(line,lineNum);

			}
		}
		cout<<"final register values"<<endl;
		for(auto it= registers.begin(); it != registers.end(); it++){
			cout<<it->first <<": "<< it->second<<endl;
		}
	}
	AssemblyInterpreter() {
		registers = {
				{"p1", 0}, {"p2", 0}, {"p3", 0}, {"p4", 0}, {"p5", 0}
		};
	}


};

int main() {
	std::cout << "Enter your assembly program line by line. Type 'exit' to finish input." << std::endl;
	std::string line;
	std::string program;

	while (std::getline(std::cin, line)) {
		if (line == "exit") break;
		program += line + "\n";
	}

	AssemblyInterpreter interpreter;
	interpreter.run_program(program);

	return 0;
}
