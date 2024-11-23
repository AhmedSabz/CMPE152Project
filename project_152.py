class AssemblyInterpreter:
    def __init__(self):
        # Initialize all registers to 7
        self.registers = {
            'p1': 7, 'p2': 7, 'p3': 7, 'p4': 7, 'p5': 7
        }
        self.valid_instructions = {'mov','MOV', 'add','ADD', 'sub', 'SUB', 'mul', 'MUL','addi','ADDI', 'subi','SUBI', 'li'}
        self.valid_registers = set(self.registers.keys())

    def validate_register(self, register, line_num):
        if register not in self.valid_registers:
            raise ValueError(f"Error on line {line_num}: Invalid register '{register}'")

    def get_value(self, operand, line_num):
        # If operand is a register, return its value
        if operand in self.valid_registers:
            return self.registers[operand]
        # If operand is a number, convert and return it
        try:
            return int(operand)
        except ValueError:
            raise ValueError(f"Error on line {line_num}: Invalid operand '{operand}'")

    def execute_instruction(self, instruction, line_num):
        parts = instruction.lower().split()

        # Check basic formatting
        if len(parts) < 2:
            raise ValueError(f"Error on line {line_num}: Incomplete instruction")

        op = parts[0]
        if op not in self.valid_instructions:
            raise ValueError(f"Error on line {line_num}: Invalid instruction '{op}'")

        # Handle the 'mov' instruction
        if op == 'mov':
            if len(parts) != 3:
                raise ValueError(f"Error on line {line_num}: MOV requires exactly 2 operands")
            dest, src = parts[1:]
            self.validate_register(dest, line_num)
            # Check if the source is a register or immediate value
            if src in self.valid_registers:
                self.registers[dest] = self.registers[src]  # Copy value from register to register
            else:
                self.registers[dest] = self.get_value(src, line_num)  # Copy immediate value

        # Handle arithmetic operations (add, sub, mul)
        elif op in {'add', 'sub', 'mul'}:
            if len(parts) != 4:
                raise ValueError(f"Error on line {line_num}: {op.upper()} requires exactly 3 operands")
            dest, src1, src2 = parts[1:]
            self.validate_register(dest, line_num)
            self.validate_register(src1, line_num)  # Ensure src1 is a register
            self.validate_register(src2, line_num)  # Ensure src2 is a register

            val1 = self.registers[src1]
            val2 = self.registers[src2]

            if op == 'add':
                self.registers[dest] = val1 + val2
            elif op == 'sub':
                self.registers[dest] = val1 - val2
            elif op == 'mul':
                self.registers[dest] = val1 * val2

        # Handle immediate operations (addi, subi)
        elif op in {'addi', 'subi'}:
            if len(parts) != 4:
                raise ValueError(f"Error on line {line_num}: {op.upper()} requires exactly 3 operands")
            dest, src, imm = parts[1:]
            self.validate_register(dest, line_num)
            self.validate_register(src, line_num)

            val = self.registers[src]
            try:
                imm_val = int(imm)
            except ValueError:
                raise ValueError(f"Error on line {line_num}: Immediate value '{imm}' is not a valid integer")

            if op == 'addi':
                self.registers[dest] = val + imm_val
            elif op == 'subi':
                self.registers[dest] = val - imm_val

        # Handle 'li' (Load Immediate) instruction
        elif op == 'li':
            if len(parts) != 3:
                raise ValueError(f"Error on line {line_num}: LI requires exactly 2 operands")
            dest, imm = parts[1:]
            self.validate_register(dest, line_num)
            try:
                imm_val = int(imm)
            except ValueError:
                raise ValueError(f"Error on line {line_num}: Immediate value '{imm}' is not a valid integer")
            self.registers[dest] = imm_val  # Load immediate value into register

    def run_program(self, program):
        lines = program.strip().split('\n')
        try:
            for i, line in enumerate(lines, 1):
                line = line.strip()
                if line:  # Skip empty lines
                    self.execute_instruction(line, i)
            print("Final register values:", self.registers)
        except ValueError as e:
            print(e)

# Testing the code
def run_test_cases():
    # Test Case 1: Basic operations with 'mov' and 'li'
    test1 = """
    li p1 10
    MOV p2 p1
    addi p3 p2 5
    """
    AssemblyInterpreter().run_program(test1)

    # Test Case 2: Error handling for invalid register
    test2 = """
    li p1 10
    mov p5 p1
    """
    AssemblyInterpreter().run_program(test2)

    # Test Case 3: Error handling for invalid instruction
    test3 = """
    li p1 10
    mul p3 p2 p1
    """
    AssemblyInterpreter().run_program(test3)

# Run the test cases
run_test_cases()
