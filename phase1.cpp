#include <iostream>
#include <fstream>
using namespace std;

class OS
{
private:
    char M[100][4]; // Physical Memory
    char IR[4];     // Instruction Register (4 bytes)
    char R[4];      // General Purpose Register (4 bytes)
    int IC = 0;     // Instruction Counter Register (2 bytes)
    int SI;         // Interrupt
    bool C = false; // Toggle (1 byte)
    char buffer[80];

public:
    void init();
    void load();
    void execute();
    void MOS();

    void printOS();

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
        for (int i = 0; i < 80; i++)
            buffer[i] = '\0';

        infile.getline(buffer, 80);

        int k = 0;
        int i = IR[2] - 48;
        int j = IR[3] - 48;
        i = i * 10;
        i += j;

        for (int l = 0; l < 10; ++l)
        {
            for (int j = 0; j < 4; ++j)
            {
                if (buffer[k] != '\0')
                    M[i][j] = buffer[k];
                k++;
            }
            if (k == 80)
            {
                break;
            }
            i++;
        }

        cout << endl;
        // printOS();
    }
    else if (SI == 2) // Write
    {
        for (int i = 0; i < 80; i++)
            buffer[i] = '\0';

        int k = 0;
        int i = IR[2] - 48;
        int j = IR[3] - 48;
        i = i * 10;
        i += j;

        for (int l = 0; l < 10; ++l)
        {
            for (int j = 0; j < 4; ++j)
            {
                buffer[k] = M[i][j];
                outfile << buffer[k];

                k++;
            }
            if (k == 80)
            {
                break;
            }
            i++;
        }
        // printOS();

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
        else if (IR[0] == 'L' && IR[1] == 'R')
        {
            int i = IR[2] - 48;
            int j = IR[3] - 48;
            i = i * 10;
            i += j;

            for (int l = 0; l < 4; l++)
            {
                R[l] = M[i][l];
            }
        }

        else if (IR[0] == 'S' && IR[1] == 'R')
        {
            int i = IR[2] - 48;
            int j = IR[3] - 48;
            i = i * 10;
            i += j;

            for (int l = 0; l < 4; l++)
            {
                M[i][l] = R[l];
            }
        }
        else if (IR[0] == 'C' && IR[1] == 'R')
        {
            int i = IR[2] - 48;
            int j = IR[3] - 48;
            i = i * 10;
            i += j;
            int flag = 1;
            for (int l = 0; l < 4; l++)
            {
                if (M[i][l] != R[l])
                    flag = 0;
            }
            if (flag == 0)
                C = false;
            else
                C = true;
        }
        else if (IR[0] == 'B' && IR[1] == 'T')
        {
            int i = IR[2] - 48;
            int j = IR[3] - 48;
            i = i * 10;
            i += j;
            if (C == true)
            {
                IC = i;
            }
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

        infile.getline(buffer, 80);

        cout << endl
             << "Buffer: ";

        for (int k = 0; k <= 39; k++)
            cout << buffer[k];
        cout << endl;

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

                    if (buffer[k] == 'H')
                    {
                        j++;
                        M[x][j++] = ' ';
                        M[x][j++] = ' ';
                        M[x][j++] = ' ';
                    }
                    k++;
                }

                if (k == 80 || buffer[k] == '\0' || buffer[k] == '\n')
                {
                    break;
                }
            }
        }

        printOS();

    } while (!infile.eof());
}

void OS::printOS()
{
    for (int i = 0; i < 100; i++)
    {
        cout << "M[" << i << "]\t";
        for (int j = 0; j < 4; j++)
        {
            cout << M[i][j];
        }
        cout << endl;
    }

    cout << "R: ";
    for (int i = 0; i < 4; i++)
    {
        cout << R[i];
    }
    cout << endl;

    cout << "IC: " << IC << endl;
    cout << "C: " << C << endl;
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