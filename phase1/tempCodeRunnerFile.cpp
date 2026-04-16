#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class OperatingSystem {
private:
    char M[100][4]; 
    char IR[4];     
    char R[4];      
    int IC;         
    bool C;         
    int SI;         
    
    ifstream inFile;
    ofstream outFile;

    void INIT() {
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 4; j++) {
                M[i][j] = '\0';
            }
        }
        for(int i = 0; i < 4; i++) {
            IR[i] = '\0';
            R[i] = '\0';
        }
        IC = 0;
        C = false;
        SI = 0;
    }

    void printMemory() {
        cout << "---------------------------------" << endl;
        for (int i = 0; i < 100; i++) {
            if (i < 10) cout << i << "  |";
            else cout << i << " |";
            for (int j = 0; j < 4; j++) {
                if (M[i][j] == '\0') cout << " |";
                else cout << M[i][j] << "|";
            }
            cout << endl;
        }
        cout << "---------------------------------" << endl;
    }

    void READ() {
        IR[3] = '0'; 
        int address = (IR[2] - '0') * 10 + (IR[3] - '0'); 

        string dataLine;
        if (getline(inFile, dataLine)) {
            if (dataLine.substr(0, 4) == "$END") {
                cout << "Abort: Out of Data" << endl;
                return; 
            }

            int k = 0;
            for (int i = 0; i < 10 && k < dataLine.length(); i++) {
                for (int j = 0; j < 4 && k < dataLine.length(); j++) {
                    M[address + i][j] = dataLine[k++];
                }
            }
        }
    }

    void WRITE() {
        IR[3] = '0'; 
        int address = (IR[2] - '0') * 10 + (IR[3] - '0'); 
        
        string output = "";
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 4; j++) {
                if (M[address + i][j] != '\0') {
                    output += M[address + i][j];
                }
            }
        }
        outFile << output << endl;
    }

    void TERMINATE() {
        outFile << endl << endl; 
    }

    void MOS() {
        switch (SI) {
            case 1: 
                READ(); 
                break;
            case 2: 
                WRITE(); 
                break;
            case 3: 
                TERMINATE(); 
                break;
        }
        SI = 0; 
    }

    void EXECUTEUSERPROGRAM() {
        while (true) { 
            for (int i = 0; i < 4; i++) {
                IR[i] = M[IC][i];
            }
            IC++; 

            string opcode = "";
            opcode += IR[0];
            opcode += IR[1];

            int address = (IR[2] - '0') * 10 + (IR[3] - '0');

            if (opcode == "LR") {
                for (int i = 0; i < 4; i++) R[i] = M[address][i];
            } 
            else if (opcode == "SR") {
                for (int i = 0; i < 4; i++) M[address][i] = R[i];
            } 
            else if (opcode == "CR") {
                C = true;
                for (int i = 0; i < 4; i++) {
                    if (R[i] != M[address][i]) {
                        C = false;
                        break;
                    }
                }
            } 
            else if (opcode == "BT") {
                if (C == true) IC = address;
            } 
            else if (opcode == "GD") {
                SI = 1;
                MOS();
            } 
            else if (opcode == "PD") {
                SI = 2;
                MOS();
            } 
            else if (opcode[0] == 'H') { 
                SI = 3;
                MOS();
                break; 
            }
        }
    }

    void STARTEXECUTION() {
        IC = 0;
        EXECUTEUSERPROGRAM();
    }

public:
    OperatingSystem() {
        inFile.open("input.txt");
        outFile.open("output.txt");
        if (!inFile) {
            cerr << "Error: input.txt file not found!" << endl;
            exit(1);
        }
    }

    ~OperatingSystem() {
        inFile.close();
        outFile.close();
    }

    void LOAD() {
        string line;
        int m = 0; 
        
        cout << "Load is called next" << endl << endl;

        while (getline(inFile, line)) {
            if (line.substr(0, 4) == "$AMJ") {
                cout << "AMJ instruction found. Memory Flushed." << endl << endl;
                INIT(); 
                m = 0; 
            } 
            else if (line.substr(0, 4) == "$DTA") {
                cout << "Loading is completed. Memory state BEFORE execution:" << endl;
                printMemory();
                
                STARTEXECUTION(); 
            } 
            else if (line.substr(0, 4) == "$END") {
                // --> Added this block to print memory at the end of the job! <--
                cout << endl << "Job execution finished. Memory state AFTER execution:" << endl;
                printMemory();
                cout << "Moving to next job..." << endl << endl;
                continue; 
            } 
            else {
                if (m >= 100) {
                    cout << "Abort: Memory Exceeded" << endl;
                    break;
                }
                
                int k = 0;
                for (int i = 0; i < 10 && k < line.length(); i++) {
                    for (int j = 0; j < 4 && k < line.length(); j++) {
                        M[m + i][j] = line[k++];
                    }
                }
                m += 10; 
            }
        } 
    }
};

int main() {
    OperatingSystem os;
    os.LOAD(); 
    cout << "Execution completed! Check output.txt for the final outputs." << endl;
    return 0;
}