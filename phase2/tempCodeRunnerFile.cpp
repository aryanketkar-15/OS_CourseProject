
        cout << "\n--- Memory  (300 words) ---" << endl;
        for (int i = 0; i < MEM_SIZE; ++i) {
            cout << setw(3) << setfill('0') << i << ": ";
            for (int j = 0; j < WORD_SIZE; ++j)
                cout << "[" << memory[i][j] << "]";
            cout << endl;
        }
        cout << "-------------------------------\n" << endl;
    }
};

Memory M;
bool blockUsed[30]; // To track allocated blocks (30 blocks of 10 words each)
ifstream inputFile;
ofstream outputFile;
bool debug = true;

// Prototypes
void LOAD();
void STARTEXECUTION();
void EXECUTEUSERPROGRAM();
void MOS();