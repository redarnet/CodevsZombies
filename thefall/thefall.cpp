#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
class Position{
    public :
        int x;
        int y;
        string  pos;
    
        Position(int x = 0, int y = 0, string pos = "TOP") : x(x), y(y), pos(pos)
        {}
};


bool verif_pos(int i, string c)
{
    if (i == 0)
    {
        return false;
    }
    if (c == "TOP")
    {
        if (i == 1 || i == 3 || i == 4 || i == 5 || i == 7 || i == 9 || i == 10 || i == 11)
            return true;
    }
    else if (c =="RIGHT")
    {
        if (i == 1 ||i == 4 ||i == 5 || i == 2 || i == 8 || i == 12 || i == 7)
            return true;
    }
    else if (c == "LEFT")
    {
        if (i == 1 ||i == 5 || i == 4 || i == 2 || i == 8 || i == 13 || i == 9)
            return true;
    }
    return false;
}


Position next_pos(int c, int yi, int xi, string posi)
{
        int xp = xi;
        int yp = yi;
        c = abs(c);
        switch(c){
            case 2:
                if(posi=="LEFT")
                    xi++;
                else 
                    xi--;
                break;
            case 4:
                if(posi=="TOP")
                    xi--;
                else 
                    yi++;
                break;
            case 5:
                if(posi=="TOP")
                    xi++;
                else 
                    yi++;
                break;
            case 6:
                if(posi=="LEFT")
                    xi++;
                else
                    xi--;
                break;
            case 10:
                xi--;
                break;
            case 11:
                xi++;
                break;
            default:
                yi++;
                break;  
        }
        string posp;
        if (yi == yp + 1)
               posp = "TOP";  
        else if (yi == yp - 1)
          posp = "BOT";   
        else if (xi == xp + 1)  
            posp = "LEFT";  
        else if (xi == xp - 1)  
            posp = "RIGHT"; 
    // next_pos == pos + posp 
    Position p(xi, yi, posp);
    return p;
    
}

vector<string> sol;


bool isValid(vector<vector<int>> grille, int yi, int xi, string pos)
{
    if (yi == grille.size())
        return true;
    if (xi < 0 || xi >= grille[0].size() || yi < 0)
        return false;

    int cell = grille[yi][xi];
    bool locked = (cell < 0);
    cell = abs(cell);  // travaille toujours avec la valeur absolue
    if (cell == 0)
        return false;

    if (!verif_pos(cell, pos))
    {
        if (!locked)
        {
            if (cell == 13 && verif_pos(10, pos))
            {
                cerr << " Par ici " << endl;
                grille[yi][xi] = 10;
                sol.push_back(to_string(xi) + " " + to_string(yi) + " RIGHT");
                Position p = next_pos(grille[yi][xi], yi, xi, pos);
                if (isValid(grille, p.y, p.x, p.pos))
                    return true;
                cerr << " Par ici " << endl;
                sol.pop_back();
                grille[yi][xi] = cell; 

            }
            if (cell < 13 && verif_pos(cell + 1, pos))
            {
                grille[yi][xi] = cell + 1;
                sol.push_back(to_string(xi) + " " + to_string(yi) + " RIGHT");
                Position p = next_pos(grille[yi][xi], yi, xi, pos);
                if (isValid(grille, p.y, p.x, p.pos))
                    return true;
                sol.pop_back();
                grille[yi][xi] = cell; 
            }
            if (cell > 1 && verif_pos(cell - 1, pos))
            {
                grille[yi][xi] = cell - 1;
                sol.push_back(to_string(xi) + " " + to_string(yi) + " LEFT");
                Position p = next_pos(grille[yi][xi], yi, xi, pos);
                if (isValid(grille, p.y, p.x, p.pos))
                    return true;
                sol.pop_back();
                grille[yi][xi] = cell; 
            }
        }
        if (grille[yi][xi] > 0)
            grille[yi][xi] = -grille[yi][xi]; 
        return false;
    }

    Position p = next_pos(cell, yi, xi, pos);
    return isValid(grille, p.y, p.x, p.pos);
}

int main()
{
    // int w; // number of columns.
    // int h; // number of rows.
    // cin >> w >> h; cin.ignore();
    vector<vector<int>> grille;
    vector<int> v1= {0, -3, 0,  0,  0,  0,  0,  0};
    vector<int> v2= {0, 12, 3,  3,  2,  3,  12, 0};
    vector<int> v3= {0, 0,  0,  0,  0,  0,  2,  0};
    vector<int> v4= {0, -12, 3,  2,  2,  3,  13, 0};
    vector<int> v5= {0, 3,  0,  0,  0,  0,  0,  0};
    vector<int> v6= {0, 12, 3,  3,  2,  3,  12, 0};
    vector<int> v7= {0, 0,  0,  0,  0,  0,  2,  0};
    vector<int> v8= {0, 12, 3,  2,  2,  3,  13, 0};
     
    grille.push_back(v1);
    grille.push_back(v2);
    grille.push_back(v3);
    grille.push_back(v4);
    grille.push_back(v5);
    grille.push_back(v6);
    grille.push_back(v7);
    grille.push_back(v8);

    for (int i = 0; i < grille.size(); i++)
    {
        for (int j = 0; j < grille[0].size(); j++)
        {
            cerr << grille[i][j] << " ";
        }
        cerr << endl;
    }
    
    int ex; // the coordinate along the X axis of the exit (not useful for this first mission, but must be read).
    ex = 2;
    // cin >> ex; cin.ignore();
    cerr << " Ex = " << ex << endl;
    // vector<string> sol;
    isValid(grille, 0, 1, "TOP");
    for (auto c : sol)
    {
        cout << c << endl;
    }
 
    // game loop
    // while (1) {
    //     int xi;
    //     int yi;
    //     string posi;
    //     cin >> xi >> yi >> posi; cin.ignore();
    //     int r; // the number of rocks currently in the grid.
    //     // cin >> r; cin.ignore();
    //     // for (int i = 0; i < r; i++) {
    //     //     int xr;
    //     //     int yr;
    //     //     string posr;
    //     //     cin >> xr >> yr >> posr; cin.ignore();
    //     // }
    //     // void simu(grille, posi,  )
    //     simu(grille, posi, xi, yi);
    //     // verif_grille 
        
    //     // if (verif_pos(posp, grille[xi][yi]))
    //         cout << "WAIT" << endl;
    //     // else
    //     // {
    //     //     cout << xi << " " << yi << " RIGHT" << endl;
    //     //     // string s =  find_pos(grille[xi][yi]);
    //     //     // cout << xi << " " << yi << s << endl;
    //     // }
    //     // Write an action using cout. DON'T FORGET THE "<< endl"
    //     // To debug: cerr << "Debug messages..." << endl;

    //     // One line containing on of three commands: 'X Y LEFT', 'X Y RIGHT' or 'WAIT'
    // }
}

