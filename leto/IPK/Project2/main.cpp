#include <iostream>
#include <cstring>
#include <netdb.h>

bool scanUdpPorts(std::string ports, std::string address){
    using namespace std;

    
    //cout << "21/udp   closed" << endl;
    return true;
}

bool scanTcpPorts(std::string ports, std::string address){
    using namespace std;

    //cout << "42/tcp   opened" << endl;
    return true;
}

int main(int argc, char* argv[]) {
    using namespace std;

    string tcpPorts;
    string udpPorts;
    string address;
    string helper;

    bool tcpScan;
    bool udpScan;

    if (argc != 6){
        cerr << "Wrong number of argumets!" << endl;
        return -1;
    }

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-pt") == 0) tcpPorts = argv[i+1];
        else if (strcmp(argv[i], "-pu") == 0) udpPorts = argv[i+1];
        else if (strcmp(argv[i], "-i") == 0) {
            cerr << "Interface selection not implemented!" << endl;
            return -1;
        }
        else {
            helper = argv[i];
            if (helper.find(",") == string::npos and helper.find("-") == string::npos) {
                address = helper;
            }
        }
    }

    cout << "Interesting ports on " << address << " :" << endl;
    cout << "PORT     STATE" << endl;
    tcpScan = scanTcpPorts(tcpPorts, address);
    udpScan = scanUdpPorts(udpPorts, address);

    return 0;
}