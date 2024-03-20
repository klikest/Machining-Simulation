#include "Command.h"


void Command::Parse_file(std::string path, Coordinates curret_coords)
{

    command_list.clear();

    std::string str;
    std::ifstream in(path);
    while (getline(in, str))
    {
        
        Coordinates coords;
        std::string number;

        for (int i = str.size() - 1; i >= 0 ; i--)
        {

            if (str[i] != 'X' && str[i] != 'Y' && str[i] != 'Z' && str[i] != 'A' && str[i] != 'C')
            {
                number.push_back(str[i]);
            }
            else
            {
                std::reverse(number.begin(), number.end());


                float value = std::stof(number);
                number.clear();

                if (str[i] == 'C')
                {
                    coords.C = value;
                    std::cout << "C = " << value << std::endl;
                }

                else if (str[i] == 'A')
                {
                    coords.A = value;
                    std::cout << "A = " << value << std::endl;
                }

                else if (str[i] == 'Z')
                {
                    coords.Z = value;
                    std::cout << "Z = " << value << std::endl;
                }

                else if (str[i] == 'Y')
                {
                    coords.Y = value;
                    std::cout << "Y = " << value << std::endl;
                }

                else if (str[i] == 'X')
                {
                    coords.X = value;
                    std::cout << "X = " << value << std::endl;
                }
            }

        }

        command_list.push_back(coords);

    }

}

Coordinates Command::RunCommands()
{
    return Coordinates();
}