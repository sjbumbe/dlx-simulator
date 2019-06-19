
•	Load_Program_Code(string filename)

o	The purpose of this function is to load the hex values that were created in Project 1 into Project 2.

o	The file is read line by line

o	A stringstream is created and the hex version of the string is pushed inside of it

o	That stringstream is then pushed into an unsigned number

o	A bitset of length 32 is created and takes the value of the unsigned number and puts it in the bitset

o	Then this is converted back into a string and pushed into the vector MAIN_MEMORY.

o	At the end of getline all the hex values are stored in the vector MAIN_MEMORY as strings that appear as the binary value

•	Read_Memory(int address, int word_byte_half)

o	The purpose of this function is to read from MAIN_MEMORY at a specified address for a specified amount, either word, byte, 
or half.

o	An empty string value is created to store whatever it is we are trying to load.

o	A switch case is performed to determine if it is word byte or half.

o	If it’s a word it will simply load that address in memory into the empty string and return the string.

o	If it is a half or byte, a substring is performed on the given address in memory.

•	Write_Memory()
 
 o	The purpose of this function is to write to memory the data that is collected from a different function and place it into memory based on MAR.

o	What gets replaced is based on the memory op code.

o	If it’s a word, it simply puts the data into the memory at MAR

o	If it’s a half world or byte, the data is substringed based on which one and the actual location in MAR is replaced depending on if it is hw or byte.

•	Run()

o	The purpose of this function is to be the runner, meaning it begins and runs the program

o	It runs forever or until IR is a nullptr (being all 0’s)

o	It runs the first 2 states of the state machine

o	It also runs the function which checks the op codes of the data

o	Finally it prints the information to the screen after each state

•	checkOP(string op)

o	The purpose of this function is to check the first 6 digits of the data in memory and do something depending on what it is.

o	The first thing that is checked is if the op code = “000000”, if it is then it runs the getRFunc(op) function. Which 
handles R-Types.

o	Otherwise it checks the opcode and runs the appropriate function depending on what it is. 

o	If it is not an R-Type, then the possible function codes are Branch, Load, Store, and IType. 

o	Each of these receives the ALU op

•	getRFunc(string op)

o	The purpose of this function is similar to checkOP() except it needs to look at the last 11 digits for func in R-type. 

o	So, the first thing that happens is the data is substringed starting at 21 going for a length of 11 to get the last 11 
digits and store it as func.

o	Then the func is checked and depending on what that is will determine what is passed into the RType(int aluop) function.

•	getData()

o	The purpose of this function is to get all of the data, regardless of what type it is. With all of the data collected any 
operation can be performed and data collection need only be done once.

o	The first thing it does is set IR equal to the data.

o	Then a series of substrings are performed based on the data being acquired. 

o	For instance, to get the op code we would want the first 6 digits so a substring would be performed starting at 0 going for 
a length of 6.

o	This process is continued for rs1, rs2, imm, rd, func, and offset.

•	incPC()

o	The purpose of this function is to increment the program counter by 4. 

o	PC starts at 0 and the first thing that happens is PC is loaded onto the s1_bus

o	The ALU op is set to 0 for addition

o	The S2 op is set to 7 for a constant of 4

o	Then finally, PC is set equal to the destination

•	PC_MAR_SELECT(bool mop)

o	The purpose of this function is to flag the pc_mar_select with a 1 or a 0.

o	If it is a 1, then the address is set equal to the MAR

o	If it is a 0, then the address is set equal to the program counter

•	printReg()

o	This function is poorly named because it doesn’t only print the registers, but it prints all of the information required in 
the output. 

o	This function is called several times throughout the program to ensure a proper print is occurring for each state.

•	setMemOp(int op)

o	The purpose of this function is for setting the mem_op correctly for stores and loads depending on if they are word, half, 
or byte.

o	It takes in the op code and performs a switch on the op code for 6 conditions: LW, LH, LB, SW, SH, SB.

o	The result will simply determine the case and set the mem_op accordingly.

•	getMemOp()

o	The purpose of this function is to return the current mem_op which is used throughout the program

•	setAOE(bool t)

o	The purpose of this function is to be a flag to set the AOE. 

o	If it is flagged, then the s1 bus receives rs1 from the reg file.

•	setBOE(bool t)

o	The purpose of this function is to be a flag to set the BOE.

o	If it is flagged, then the s2 bus receives rs2 from the reg file.

•	setS2op(int op)

o	The purpose of this function is to set the s2 op for the correct conditions.

o	The only s2 ops for this program that were needed were 0, 3, 5, 6, and 7.

o	For cases 0, 6, and 7 the s2 op is either passed or set to the appropriate value.

o	Cases 3 and 5 created an interesting issue in the case of negative numbers.

o	A temporary imm and offset were created accordingly.

o	The concept was the same just with different numbers but the purpose was to correct negative numbers. Essentially if the 
temporary number anded with the appropriate shifted ranged evaluated to be true, then it would be or’d with the temp variable, otherwise it would essentially flip the digits in the case of negative setting it to 0 and performed the same or.

o	With the final results after dealing with negatives the new value can be put on the s2_bus.

•	setALUop(int op)

o	The purpose of this function is to set the ALU op according to the op code and then perform the appropriate arithmetic on 
the contents of each bus.

o	Whatever the result is, the destination will be set equal to it.

o	A check is done at the end of this function to see if dest = 0, if it is then the zflag is set to 1 otherwise it is set to 
0. This flag is for branches and will be discussed later.

•	setRegSelect(int reg)

o	The purpose of this function is to set regSelect equal to whatever reg is passed into it, it is used to update the 
registers.

o	It simply returns regSelect = reg

•	updateReg()

o	The purpose of this function is to update the registers when adjustments are made based on an instruction.

o	regSelect is switched in this function and depending on the case, different things happen.

o	For instance, if regSelect = 0, then whatever is in C at that point will be stored in the regfile at rd

o	If regSelect = 1, then C will be stored in the reg file at rs1

o	And if regSelect = 2, then C will be stored in the 31 slot of the reg file, ie at the end.

•	S1()

o	The purpose of this function is the first state of the state machine.

o	PC_MAR_SELECT is set to 0

o	Data is set to Read_Memory(PC/4, 0). Meaning it is stored every 4th block at the size of a word based on the program 
counter

o	getData() is called to assign all of the potential use cases

o	Then all of the information is printed to show the first state

•	S2()

o	The purpose of this function is the second state of the state machine.

o	First, the program counter is incremented properly by calling the function incPC()

o	Rs1 is set equal to A

o	Rs2 is set equal to B

o	Then all of the information is printed to show the second state

•	IType(int aluop)

o	The purpose of this function is to handle IType’s that don’t require any sort of special attention. Therefore, most of 
these following functions will appear to be how a state machine operates but with the function calls that were listed above.

o	First, AOE is set to 1.

o	Then, the s2_bus is to IR

o	The s2 op is set to 3

o	setALUop() is called taking in the appropriate alu op

o	C is set to dest

o	Then setRegSelect is set to 1

o	Finally, the registers are updated and IType has concluded

•	RType(int aluop)

o	The purpose of this function is to handle RType instructions. Therefore, most of these following functions will appear to 
be how a state machine operates but with the function calls that were listed above.

o	AOE is set to 1 using setAOE()

o	BOE is set to 1 using setBOE()

o	S2 op is set to 0 using setS2op()

o	ALU up is set by taking a parameter using setALUop()

o	C is set to dest

o	regSelect is set to 0 using setRegSelect()

o	Finally, the registers are updated using updateReg() and RType has concluded

•	Branch(int aluop)

o	The purpose of this function is to handle Branch instructions regardless of which branch they are. Therefore, most of these 
following functions will appear to be how a state machine operates but with the function calls that were listed above.

o	AOE is set to 1 using setAOE()

o	S2 op is set to 0 using setS2op()

o	ALU op is set by taking a parameter using setALUop()

o	Now, for the zflag that was mentioned before. A check is made to see if the zflag is on or off.

o	If it is on and the op code is 4 or if its off and the op code is 5 then the following conditions occur:

o	S1_bus is set to PC

o	S2_bus is set to IR

o	S2 op is set to 3 using setS2op()

o	ALU op is set to 0 using setALUop()

o	PC is set to dest

o	At this point, whether or not those previous conditions were met, Branch has concluded

•	Store(int aluop)

o	The purpose of this function is to handle the Store instructions regardless of them being word, half or byte. Therefore, 
most of these following functions will appear to be how a state machine operates but with the function calls that were listed above.

o	First, the third state of the total state machine begins.

o	AOE is set to 1 using setAOE()

o	S2_bus is set to IR

o	S2 op is set to 3 using setS2op()

o	ALU op is set to 0 using setALUop()

o	MAR is set to dest

o	printReg() is called to display the information to show the end of this state.

o	State 4 begins

o	BOE is set to 1 using setBOE()

o	S2 op is set to 0 using setS2op()

o	ALU op is set to 3 using setALUop()

o	MDR is set to dest

o	printReg() is called to display the information to show the end of this state.

o	State 5 begins

o	Op code is converted from a string to an integer

o	The mem op is then set by passing the op code into setMemOp()

o	The address is set to MAR using PC_MAR_SELECT()

o	Finally, Write_Memory() is called to store this information in memory and the Store instructions concluded

•	Load(int aluop)

o	The purpose of the function is to handle Load instructions regardless of them being word, half, or byte. Therefore, most of 
these following functions will appear to be how a state machine operates but with the function calls that were listed above.

o	First, the third state of the total state machine begins.

o	AOE is set to 1 using setAOE()

o	S2_bus is set to IR

o	S2 op is set to 3 using setS2op()

o	ALU op is set to 0 using setALUop()

o	MAR is set to dest

o	printReg() is called to display the information to show the end of this state.

o	State 4 begins

o	Op code is converted from a string to an integer

o	The mem op is then set by passing the op code into setMemOp()

o	Read_Memory() is then called taking the arguments MAR/4 for the address location and getMemOp() as the second parameter to 
pass in the appropriate size (word, half, byte)

o	MDR is set to dest

o	printReg() is called to display the information to show the end of this state.

o	S2 op is set to 1 using setS2op()

o	ALU op is set to 3 using setALUop()

o	MDR is set to the s2_bus

o	After this, the load is successful and the Load instruction has concluded

•	Jump(int aluop)

o	The purpose of this function to handle Jump instructions, we were only given one jump instruction for this program, but the 
function was still designed to take any jump argument.  Therefore, most of these following functions will appear to be how a 
state machine operates but with the function calls that were listed above.

o	S2 op is set to 3 using setS2op()

o	The ALU op is set by passing in op code to setALUop()

o	S2_bus is set to IR

o	S1_bus is set to PC

o	Finally, the Jump instruction has concluded

This program only uses one class (Main_Memory)which handles all of the functions, the purpose of using the class was to have 
access to the REG_FILE and MAIN_MEMORY
1.	The first thing that happens is an object called m is created from the class Main_Memory and the default constructor is called.
2.	All variables are initialized
3.	The function Load_Program_Code is called and the input file is parsed turning the hex values into binary strings and then pushing them into MAIN_MEMORY
4.	Next the run() function is called.
a.	First S1() is called running the first state of the machine
b.	Next S2() is called running the second state of the machine
c.	The opcode is checked by passing the op code gained from getData() in S1() in checkOp()
5.	In checkOp() the first thing that is checked is to see if the op code is 0, if it is then it passes the op code in getRfunc() see we know it’s an R-Type
a.	If it is an R-Type the op code is passed into getRfunc
b.	The func is then grabbed via a substring
c.	RType(aluop) is run based on the resulting func variable
d.	If it is not an R-Type, then the following functions can be ran depending on the op code; Branch, Load, Store, IType, Jump.
e.	Each of which takes the appropriate alu op code to begin executing the instruction
6.	If it is an IType, the instruction is complete and the next one is moved onto once the registers are updated.
7.	If it is an RType, the instruction is complete and the next one is moved onto once the registers are updated.
8.	If it is a Branch, the instruction is complete and the next one is moved onto once the variables are all correctly assigned and the zflag concern has been addressed.
9.	If it is a Jump, the instruction is complete and the next one is moved onto once the s1 and s2 buses are set accordingly.
10.	If it is a Store, the instruction is complete and the next one is moved onto once it has been written back to memory using Write_Memory
11.	If it is a Load, the instruction is complete and the next one is moved onto once it has been read using Read_Memory() and the MDR is set to the s2 bus.
12.	In between each state, printReg() is called to display the full results of each state to the consol. 
13.	These processes are repeated until EOF







