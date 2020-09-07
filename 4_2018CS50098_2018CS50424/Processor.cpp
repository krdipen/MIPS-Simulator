#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<map>
#include<vector>
#include<math.h>

using namespace std;

int main(int argc,char *argv[]){
    int i,num;
    int cycle=0;
    int inst=0;
    string cmd;
    ifstream infile;
    ofstream outfile;
    ofstream outfile2;
    //cycle per instrction (cpi) as the function of instruction
    map<string,int> cpi;
    infile.open(argv[3]);
    while(infile>>cmd){
        infile>>num;
        cpi[cmd]=num;
    }
    infile.close();
    //register file all initialized to zero except stack pointer pointing to 4096
    vector<int> reg(32,0);
    reg[29]=4096;
    //text memory
    vector<string> text;
    infile.open(argv[1]);
    while(getline(infile,cmd)){
        text.push_back(cmd);
    }
    infile.close();
    //data memory with default value equals to zero
    vector<int> data(4096,0);
    infile.open(argv[2]);
    i=0;
    while(infile>>num){
        data[i]=num;
        i++;
    }
    infile.close();
    //processor processing text
    i=0;
    outfile.open("register.txt");
    outfile2.open("memory.txt");
    for(int j=0;j<reg.size();j++){
        outfile<<reg[j]<<" ";
    }
    for(int j=0;j<data.size();j++){
        outfile2<<data[j]<<" ";
    }
    while(i<text.size()){
        int rs,rt,rd,shamt,offset,target;
        istringstream ss;
        ss.str(text[i]);
        ss>>cmd;
        if(cmd=="add"){
            ss>>rd>>rs>>rt;
            reg[rd]=reg[rs]+reg[rt];
            cycle=cycle+cpi["add"];
            inst++;
        }
        else if(cmd=="sub"){
            ss>>rd>>rs>>rt;
            reg[rd]=reg[rs]-reg[rt];
            cycle=cycle+cpi["sub"];
            inst++;
        }
        else if(cmd=="sll"){
            ss>>rd>>rt>>shamt;
            reg[rd]=reg[rt]*pow(2,shamt);
            cycle=cycle+cpi["sll"];
            inst++;
        }
        else if(cmd=="srl"){
            ss>>rd>>rt>>shamt;
            reg[rd]=reg[rt]/pow(2,shamt);
            cycle=cycle+cpi["srl"];
            inst++;
        }
        else if(cmd=="jr"){
            ss>>rs;
            i=reg[rs]-1;
            cycle=cycle+cpi["jr"];
            inst++;
        }
        else if(cmd=="lw"){
            ss>>rt>>rs>>offset;
            reg[rt]=data[reg[rs]+offset];
            cycle=cycle+cpi["lw"];
            inst++;
        }
        else if(cmd=="sw"){
            ss>>rt>>rs>>offset;
            data[reg[rs]+offset]=reg[rt];
            cycle=cycle+cpi["sw"];
            inst++;
        }
        else if(cmd=="j"){
            ss>>target;
            i=target-1;
            cycle=cycle+cpi["j"];
            inst++;
        }
        else if(cmd=="jal"){
            ss>>target;
            reg[31]=i+1;
            i=target-1;
            cycle=cycle+cpi["jal"];
            inst++;
        }
        else if(cmd=="beq"){
            ss>>rs>>rt>>offset;
            if(reg[rs]==reg[rt]){
                i=i+offset;
            }
            cycle=cycle+cpi["beq"];
            inst++;
        }
        else if(cmd=="bne"){
            ss>>rs>>rt>>offset;
            if(reg[rs]!=reg[rt]){
                i=i+offset;
            }
            cycle=cycle+cpi["bne"];
            inst++;
        }
        else if(cmd=="blez"){
            ss>>rs>>offset;
            if(reg[rs]<=0){
                i=i+offset;
            }
            cycle=cycle+cpi["blez"];
            inst++;
        }
        else if(cmd=="bgtz"){
            ss>>rs>>offset;
            if(reg[rs]>0){
                i=i+offset;
            }
            cycle=cycle+cpi["bgtz"];
            inst++;
        }
        outfile<<endl<<endl;
        for(int j=0;j<reg.size();j++){
            outfile<<reg[j]<<" ";
        }
        outfile2<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
        for(int j=0;j<data.size();j++){
            outfile2<<data[j]<<" ";
        }
        i++;
    }
    outfile.close();
    outfile2.close();
    cout<<endl;
    cout<<"instruction counts: "<<inst<<endl;
    cout<<"clock cycle counts: "<<cycle<<endl;
    cout<<"average Instructions Per Cycle (IPC): "<<inst*1.0/cycle<<endl;
    cout<<endl;
    return 0;
}
