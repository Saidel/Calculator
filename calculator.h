#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Brackets{
    double Result, Last_Num;
    char  Binary_Operation, Unary_Operation;

    Brackets(){Clear();}

    void Clear(){
        Result = 0;
        Last_Num = 0;
        Binary_Operation = '+';
        Unary_Operation = '+';
    }

    void Do_Operation(double d1){
        if(Unary_Operation == '-') d1 = -d1;
        Unary_Operation = '+';

        if(Binary_Operation == '+'){
            Result+=d1;
            Last_Num = d1;
        }else if(Binary_Operation == '-'){
            Result-=d1;
            Last_Num = -d1;
        }else if(Binary_Operation == '*'){
            Result -= Last_Num;
            Last_Num*=d1;
            Result += Last_Num;
        }else if(Binary_Operation == '/'){
            Result -= Last_Num;
            Last_Num/=d1;
            Result += Last_Num;
        }else if(Binary_Operation == '^'){
            Result -= Last_Num;
            Last_Num=pow(Last_Num, d1);
            Result += Last_Num;
        }
        Binary_Operation = 0;
    }
};

double Get_Number(string n){
    int AfterPoint = -1;
    double Num = 0;
    for(int i = 0; i<n.length(); i++){
        if((n[i] < '0' || n[i] > '9') && n[i]!='.' && n[i]!=',') return -1;
        if(n[i]=='.' || n[i]==','){
            if(AfterPoint != -1) return -1;
            else AfterPoint = (n.length()-i-1);
        }else
            Num = Num*10.0 + (n[i]-'0');
    }
    if(AfterPoint>2) return -1;
    else Num*=(AfterPoint==2)?0.01:(AfterPoint==1)?0.1:1;
    return Num;
}

bool Is_Special_Char(char c){
    if(c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '(' || c == ')') return true;
    else return false;
}


string Calculate(string exp_s){
    vector<Brackets> Br;
    Br.push_back(Brackets());
    int BrLevel = 0;
    for(int i = 0; i < exp_s.length(); i++){
        if(exp_s[i] != ' '){
            if(Is_Special_Char(exp_s[i])){

                if(Br[BrLevel].Binary_Operation != 0 && (exp_s[i]=='*' || exp_s[i]=='/' || exp_s[i]=='^'))
                    return "ошибка операции '"+string(1, exp_s[i])+"'";

                if(exp_s[i]=='+' || exp_s[i]=='-'){
                        if(Br[BrLevel].Binary_Operation == 0)
                            Br[BrLevel].Binary_Operation = exp_s[i];
                        else
                            Br[BrLevel].Unary_Operation = (Br[BrLevel].Unary_Operation == exp_s[i])?'+':'-';
                }
                else if(exp_s[i]=='*' || exp_s[i]=='/' || exp_s[i]=='^') Br[BrLevel].Binary_Operation = exp_s[i];
                else if(exp_s[i]=='('){
                    if(Br[BrLevel].Binary_Operation==0) return "пропушена операция перед '('";
                    BrLevel++;
                    if(BrLevel < Br.size()) Br[BrLevel].Clear();
                    else Br.push_back(Brackets());
                }else if(exp_s[i]==')'){
                    BrLevel--;
                    if(BrLevel < 0) return "неправильная расстановка скобок";
                    if(Br[BrLevel+1].Result >= 10000000 || Br[BrLevel+1].Result <= -10000000) return "по модулю не меньше 10000000";
                    if(Br[BrLevel].Binary_Operation == '/' && Br[BrLevel+1].Result == 0) return "ошибка деления на ноль";
                    Br[BrLevel].Do_Operation(Br[BrLevel+1].Result);
                }

            }else{
                int EndNum = i+1;
                while(!Is_Special_Char(exp_s[EndNum]) && exp_s[EndNum]!=' ' && exp_s[EndNum]!='\0') EndNum++;
                double Num = Get_Number(exp_s.substr(i, EndNum-i));

                if(Num == -1) return "строка содержит недопустимое выражение '"+exp_s.substr(i, EndNum-i)+"'";
                if(Num >= 10000000 || Num <= -10000000) return "по модулю не меньше 10000000";
                if(Br[BrLevel].Binary_Operation == '/' && Num == 0) return "ошибка деления на ноль";
                Br[BrLevel].Do_Operation(Num);

                i = EndNum-1;
            }
        }
    }

    if(BrLevel != 0) return "не все скобки закрыты";

    for(int i = 0; i<Br.size(); i++)
        if(Br[i].Binary_Operation != 0)
                return "ошибка операции '"+string(1, Br[i].Unary_Operation!='+'?Br[i].Unary_Operation:Br[i].Binary_Operation)+"'";


    if(Br[0].Result >= 10000000 || Br[0].Result <= -10000000) return "по модулю не меньше 10000000";

    string Res = to_string(roundf(Br[0].Result*100.0)/100.0);
    while(Res.back() == '0') Res.pop_back();
    if(Res.back() == '.') Res.pop_back();
    if(Res.compare("-0") == 0) Res = "0";

    Br.clear();

    return Res;
}

#endif
