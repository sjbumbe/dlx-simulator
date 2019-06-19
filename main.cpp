//
//  main.cpp
//  compOrg_Project2
//
//  Created by Steven Bumbera on 5/9/19.
//  Copyright © 2019 Steven Bumbera. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Main_Memory{
public:
    string func, IR, imm, offset, op, addr, dest, rs1, rs2, rd, A, B, C, data;
    bool zflag = 0;
    int PC = 0;
    int regSelect;
    long s1_bus = 0;
    long s2_bus = 0;
    int MAR = 0;
    int MDR = 0;
    int mem_op = 0;
    vector<string> MAIN_MEMORY;
    vector<long> REG_FILE {
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,
        27,28,29,30,31
    };
    Main_Memory()=default;
    int Load_Program_Code(string filename);
    void checkOP(string op);
    void getRFunc(string op);
    void doRType(int aluop);
    void S1();
    void S2();
    void PC_MAR_SELECT(bool mop);
    string Read_Memory(int address, int word_byte_half);
    void run();
    void getData();
    void IType(int aluop);
    void setAOE(bool t);
    void setBOE(bool t);
    void setS2op(int op);
    void setALUop(int op);
    void setRegSelect(int reg){regSelect = reg;};
    void updateReg();
    void printReg();
    void incPC();
    void RType(int aluop);
    void Branch(int aluop);
    void Store(int aluop);
    void setMemOp(int op);
    int getMemOp();
    int Write_Memory();
    void Load(int aluop);
    void Jump(int aluop);
    
};

int Read_Memory(int address, int word_byte_half);
int Write_Memory(int address, int data, int word_byte_half);
string checkOP(string op);
string getRFunc(string op);
//------------------------------------------------------------------------------
int main(int argc, const char * argv[]){
    Main_Memory m;
    m.Load_Program_Code("input.txt");
    m.run();
  
    
    
    return 0;
}
//------------------------------------------------------------------------------
int Main_Memory::Load_Program_Code(string filename){
    string line;
    unsigned num;
    ifstream myfile (filename);
    if (myfile.is_open())
    {
        while ( getline (myfile,line, '\n') )
        {
            stringstream ss;
            ss << hex << line;
            ss >> num;
            
            bitset<32> bit(num);
            MAIN_MEMORY.push_back(bit.to_string());
        }
        myfile.close();
        return 1;
    }
    return 0;
}
//------------------------------------------------------------------------------
string Main_Memory::Read_Memory(int address, int word_byte_half){
    string val = "";
    switch(word_byte_half){
        case 0:{
            val = MAIN_MEMORY[address];
            break;}
        case 1:
            //hw 16 bits
            val = MAIN_MEMORY[address].substr(0,16);
            break;
        case 2:
            //byte 8 bits
            val = MAIN_MEMORY[address].substr(0,8);
            break;
    }
    return val;
}
//------------------------------------------------------------------------------
int Main_Memory::Write_Memory(){
    
    switch(getMemOp()){
        case 0: MAIN_MEMORY[MAR] = data; break;
        case 1: MAIN_MEMORY[MAR].replace(16,16, data.substr(16,16)); break;
        case 2: MAIN_MEMORY[MAR].replace(24,8, data.substr(24,8)); break;
        default: break;
            
    }
    return 0;
}

void Main_Memory::run() {
    for(;;) {
        S1();
        S2();
        long exit = stoul(IR, nullptr, 2);
        if(exit == 0) break;
        checkOP(op);
        printReg();
    }
    
}
//------------------------------------------------------------------------------
void Main_Memory::checkOP(string op){
    //check for R-TYPE

    if(op == "000000"){
        getRFunc(op);
    }
    else if(op == "000100")Branch(2);    //BEQZ
    else if(op == "100001")Load(1);      //LH
    else if(op == "101000")Store(2);     //SB
    else if(op == "001000")IType(0);     //ADDI
    else if(op == "000101")Branch(2);    //BNEZ
    else if(op == "100011")Load(0);      //LW
    else if(op == "001101")IType(5);
    else if(op == "101001")Store(1);     //SH
    else if(op == "000010")Jump(2);      //J
    else if(op == "101011")Store(0);     //SW
    else if(op == "100000")Load(2);      //LB
    
}

void Main_Memory::getRFunc(string op){
    string func;
    for(auto i : MAIN_MEMORY){
        string firstValue = i;
        func = firstValue.substr(21,11);
        if(func == "00000100000")RType(0);       //ADD
        else if(func == "00000100101")RType(5);  //OR
        else if(func == "00000100100")RType(4);  //AND
        else if(func == "00000000100")RType(8);  //SLL
        else if(func == "00000000110")RType(10); //SRL
        else if(func == "00000100010")RType(1);  //SUB
    }
}

void Main_Memory::S1(){
    PC_MAR_SELECT(0);
    data = Read_Memory(PC/4, 0);
    getData();
    printReg();
    
}

void Main_Memory::S2(){
    incPC();
    A = rs1;
    B = rs2;
    printReg();
    
}

void Main_Memory::incPC(){
    s1_bus = PC;
    setS2op(7);
    setALUop(0);
    PC = stoi(dest);
}

void Main_Memory::PC_MAR_SELECT(bool mop){
    if(mop) addr = MAR;
    else
        addr = to_string(PC);
}

void Main_Memory::getData(){
    IR = data;
    op = IR.substr(0,6);
    rs1 = IR.substr(6,5);
    rs2 = IR.substr(11,5);
    imm = IR.substr(16,16);
    rd = IR.substr(16,5);
    func = IR.substr(21,11);
    offset = IR.substr(6,26);
}

void Main_Memory::IType(int aluop){
    setAOE(1);
    s2_bus = stol(IR, nullptr, 2);
    setS2op(3);
    setALUop(aluop);
    C = dest;
    
    setRegSelect(1);
    updateReg();

}

void Main_Memory::RType(int aluop){
    setAOE(1);
    setBOE(1);
    setS2op(0);
    setALUop(aluop);
    C = dest;
    
    setRegSelect(0);
    updateReg();
}

void Main_Memory::Branch(int aluop){
    setAOE(1);
    setS2op(0);
    setALUop(aluop);
    
    if((zflag == 1 && op == "00100") || (zflag == 0 && op == "00101")){
        s1_bus = PC;
        s2_bus = stol(IR, nullptr, 2);
        setS2op(3);
        setALUop(0);
        PC = stoi(dest);
    }
}

void Main_Memory::Store(int aluop){
    setAOE(1);
    s2_bus = stol(IR, nullptr, 2);
    setS2op(3);
    setALUop(0);
    MAR = stoi(dest);
    
    printReg();
    
    setBOE(1);
    setS2op(0);
    setALUop(3);
    MDR = stoi(dest,nullptr,2);
    
    printReg();
    
    int op_int = stoi(op, nullptr, 2);
    setMemOp(op_int);
    PC_MAR_SELECT(1);
    Write_Memory();
}

void Main_Memory::Load(int aluop){
    setAOE(1);
    s2_bus = stol(IR, nullptr, 2);
    setS2op(3);
    setALUop(0);
    MAR = stoi(dest);
    
    printReg();
    
    PC_MAR_SELECT(1);
    int op_int = stoi(op, nullptr, 2);
    setMemOp(op_int);
    Read_Memory(MAR/4, getMemOp());
    MDR = stoi(dest);
    
    printReg();
    
    setS2op(1);
    setALUop(3);
    MDR = s2_bus;
}

void Main_Memory::Jump(int aluop){
    setS2op(3);
    setALUop(aluop);
    s2_bus = stol(IR, nullptr, 2);
    s1_bus = PC;
    
}
void Main_Memory::setMemOp(int op){
    
    switch(op){
        case 43: mem_op = 2; break; //SB
        case 41: mem_op = 1; break; //SH
        case 40: mem_op = 0; break; //SW
        case 35: mem_op = 0; break; //LW
        case 33: mem_op = 1; break; //LH
        case 32: mem_op = 2; break; //LB
        default: break;
    }
}

int Main_Memory::getMemOp(){
    return mem_op;
}
void Main_Memory::setAOE(bool t){
    if(t)s1_bus = REG_FILE[stoi(rs1, nullptr, 2)];
}

void Main_Memory::setBOE(bool t){
    if(t)s2_bus = REG_FILE[stoi(rs2, nullptr, 2)];
}

void Main_Memory::setS2op(int op){
    switch(op){
        case 0: break;
        case 3:{
            long tempimm = stol(imm, nullptr, 2);
            tempimm |= tempimm & (1 << 15) ? (-(1 << 16)) : 0;
            s2_bus = tempimm;
            break;}
        case 5:{
            long tempoff = stol(offset, nullptr, 2);
            tempoff |= tempoff & (1 << 25) ? (-(1 << 26)) : 0;
            s2_bus = tempoff;
            break;}
        case 6: {s2_bus = 16; break;}
        case 7: {s2_bus = 4; break;}
        default: break;
    }
}

void Main_Memory::setALUop(int op){
    switch(op){
        case 0: dest = to_string(s1_bus + s2_bus); break;
        case 1: dest = to_string(s1_bus - s2_bus); break;
        case 2: dest = to_string(s1_bus); break;
        case 3: dest = to_string(s2_bus); break;
        case 5: dest = to_string(s1_bus | s2_bus); break;
        case 8: dest = to_string(s1_bus << s2_bus); break;
        case 10: dest = to_string(s1_bus >> s2_bus); break;
        default: break;
    }
    if(dest == "0") zflag = 1;
    else zflag = 0;
}

void Main_Memory::updateReg(){
    switch(regSelect){
        case 0: REG_FILE[stoi(rd,nullptr,2)] = stol(C); break;
        case 1: REG_FILE[stoi(rs2,nullptr,2)] = stol(C); break;
        case 2: REG_FILE[31] = stol(C); break;
    }
}

void Main_Memory::printReg(){
    int cnt = 0;
    cout << "=======================================";
    
    cout << endl << "REGISTER FILE:" << endl;
    for(auto i : REG_FILE){
        cout << "r" << cnt << ": " << i << ' ';
        cnt++;
    }
    
    cout << endl << endl << "MEMORY REGISTERS:" << endl;
    cout << "MDR: " << MDR << ' ' << "MAR: " << MAR << endl;
    
    cout << endl << endl << "HARDWARE REGISTERS:" << endl;
    cout << "IR: " << IR << ' ' << "PC: " << PC << endl;
    
    cout << endl << endl << "DATA BUS:" << endl;
    cout << "S1: " << s1_bus << ' ' << "S2: " << s2_bus << ' ' << "Dest: " <<
    dest << ' ' << "Address: " << addr << ' ' << "Data: " << IR << endl;
    
    cout << endl << endl << "REGISTER BUFFERS:" << endl;
    cout << "A: " << A << ' ' << "B: " << B << ' ' << "C: " << C << endl;
    cout << endl << endl;
    cout << "=======================================";
    cout << endl << endl;
}
