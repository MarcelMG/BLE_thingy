#include <iostream>
#include <string>
using namespace std;

int main()
{
    string strbuf, tmpstr, minor_str, major_str, txpwr_str;
    const string UUID = "123456789ABCDEF123456789ABCDEF12";
    unsigned int minor, major, txpwr;

    while(1){
        strbuf = "";
        size_t pos = string::npos;
        while(pos == string::npos){
            cin >> tmpstr;
            strbuf += tmpstr;
            pos = strbuf.find(">");
        }
        pos = strbuf.find(UUID);
        if(pos != string::npos){
            printf("\033c"); // clear console
            pos += UUID.size();
            minor_str = strbuf.substr(pos, 4);
            minor = stoul(minor_str, nullptr, 16);
            pos += 4;
            major_str = strbuf.substr(pos, 4);
            major = stoul(major_str, nullptr, 16);
            pos += 6;
            txpwr_str = strbuf.substr(pos, 2);
            txpwr = stoul(txpwr_str, nullptr, 16);
            if( major==0xFFFF && minor==0xFFFF ){
               cout<<"MOTION DETECTED!"<<endl;
            }else if( major==0xEEEE && minor==0xEEEE ){
               cout<<"BUTTON PRESSED!"<<endl;
            }else{
               cout<<"battery voltage: "<<(0.001*minor)<<"V\t\ttemperature: "<<(int16_t)major<<"°C\t\tttx_power= "<<txpwr<<endl;
            }

        }
    }

    return 0;
}
