#include <iostream>
#include <fstream>
using namespace std;

class OS
{
private:
    char M[100][4]; // Physical Memory
    char IR[4];     // Instruction Register (4 bytes)
    char R[4];      // General Purpose Register (4 bytes)
    int IC;         // Instruction Counter Register (2 bytes)
    int SI;         // Interrupt
    bool C;         // Toggle (1 byte)
    char buffer[40];

public:
    void init();
    void load();
    void execute();
    void MOS();

    fstream infile;
    fstream outfile;
};

void OS::init()
{

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            M[i][j] = ' ';
        }
    }

    for (int i = 0; i < 4; i++)
    {
        R[i] = ' ';
        IR[i] = ' ';
    }
}

void OS::MOS()
{

    if (SI == 1) // Read
    {
        for (int i = 0; i < 40; i++)
            buffer[i] = '\0';

        infile.getline(buffer, 40);

        int k = 0;
        int i = IR[2] - 48;
        i = i * 10;

        for (int l = 0; l < 10; ++l)
        {
            for (int j = 0; j < 4; ++j)
            {
                if (buffer[k] != '\0')
                    M[i][j] = buffer[k];
                k++;
            }
            if (k == 40)
            {
                break;
            }
            i++;
        }

        cout << endl;
        for (int i = 0; i < 100; i++)
        {
            cout << "M[" << i << "]\t";
            for (int j = 0; j < 4; j++)
            {
                cout << M[i][j];
            }
            cout << endl;
        }
    }
    else if (SI == 2) // Write
    {
        for (int i = 0; i < 40; i++)
            buffer[i] = '\0';

        int k = 0;
        int i = IR[2] - 48;
        i = i * 10;

        for (int l = 0; l < 10; ++l)
        {
            for (int j = 0; j < 4; ++j)
            {
                buffer[k] = M[i][j];
                outfile << buffer[k];

                k++;
            }
            if (k == 40)
            {
                break;
            }
            i++;
        }
        for (int i = 0; i < 100; i++)
        {
            cout << "M[" << i << "]\t";
            for (int j = 0; j < 4; j++)
            {
                cout << M[i][j];
            }
            cout << endl;
        }

        outfile << "\n";
    }
    else if (SI == 3) // Terminate
    {

        outfile << "\n";
        outfile << "\n";
    }
}

void OS::execute()
{
    while (true)
    {
        for (int i = 0; i < 4; i++)
        {
            IR[i] = M[IC][i];
        }
        IC++;

        if (IR[0] == 'G' && IR[1] == 'D')
        {
            SI = 1;
            MOS();
        }
        else if (IR[0] == 'P' && IR[1] == 'D')
        {
            SI = 2;
            MOS();
        }
        else if (IR[0] == 'H')
        {
            SI = 3;
            MOS();
            break;
        }
    }
}

void OS::load()
{

    cout << "Reading Data..." << endl;
    int x = 0;
    do
    {
        for (int i = 0; i <= 39; i++)
            buffer[i] = '\0';

        infile.getline(buffer, 40);

        for (int k = 0; k <= 39; k++)
            cout << buffer[k];

        if (buffer[0] == '$' && buffer[1] == 'A' && buffer[2] == 'M' && buffer[3] == 'J')
        {
            init();
        }
        else if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'T' && buffer[3] == 'A')
        {
            IC = 00;
            execute();
        }
        else if (buffer[0] == '$' && buffer[1] == 'E' && buffer[2] == 'N' && buffer[3] == 'D')
        {
            x = 0;
            continue;
        }
        else
        {
            int k = 0;

            for (; x < 100; ++x)
            {
                for (int j = 0; j < 4; ++j)
                {

                    M[x][j] = buffer[k];
                    k++;
                }

                if (k == 40 || buffer[k] == '\0' || buffer[k] == '\n')
                {
                    break;
                }
            }
        }

    } while (!infile.eof());
}

int main()
{
    OS os;

    os.infile.open("input.txt", ios::in);
    os.outfile.open("output.txt", ios::out);

    if (!os.infile)
    {
        cout << "Input file not found." << endl;
    }
    else
    {
        cout << "Input file found." << endl;
    }

    os.load();

    return 0;
}