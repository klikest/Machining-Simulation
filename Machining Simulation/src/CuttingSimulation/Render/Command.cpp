#include "Command.h"


void Command::Parse_file(std::string path, Coordinates curret_coords)
{

    command_list.clear();

    std::string str;
    std::ifstream in(path);

    Coordinates coords;
    coords = curret_coords;

    while (getline(in, str))
    {
        std::string number;

        for (int i = str.size() - 1; i >= 0; i--)
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
                    //std::cout << "C = " << value << std::endl;
                }

                else if (str[i] == 'A')
                {
                    coords.A = value;
                    //std::cout << "A = " << value << std::endl;
                }

                else if (str[i] == 'Z')
                {
                    coords.Z = value;
                    //std::cout << "Z = " << value << std::endl;
                }

                else if (str[i] == 'Y')
                {
                    coords.Y = value;
                    //std::cout << "Y = " << value << std::endl;
                }

                else if (str[i] == 'X')
                {
                    coords.X = value;
                    //std::cout << "X = " << value << std::endl;
                }
            }

        }

        command_list.push_back(coords);

    }

}

void Command::Zero_time()
{
    t = 0;
    step = 0;
}

void Command::RunCommands(Coordinates& curret_coords, bool& run)
{

    Coordinates new_coords = command_list[step];

    Coordinates delta_vector = Coordinates(
        new_coords.X - curret_coords.X,
        new_coords.Y - curret_coords.Y,
        new_coords.Z - curret_coords.Z,
        new_coords.A - curret_coords.A,
        new_coords.C - curret_coords.C);

    curret_coords.X = curret_coords.X + delta_vector.X * t;
    curret_coords.Y = curret_coords.Y + delta_vector.Y * t;
    curret_coords.Z = curret_coords.Z + delta_vector.Z * t;
    curret_coords.A = curret_coords.A + delta_vector.A * t;
    curret_coords.C = curret_coords.C + delta_vector.C * t;


    float N = fabs(delta_vector.X*100 + delta_vector.Y*100 + delta_vector.Z*100 + delta_vector.A*10 + delta_vector.C*10) *( 1/((speed_of_sim* speed_of_sim)/1000)) ;

    t += 1.0f / N;
    if (t > 1)
    {
        if (step < command_list.size() - 1)
        {
            step += 1;
            t = 0;
        }
        else
        {
            t = 0;
            step = 0;
            run = false;
        }
    }
}