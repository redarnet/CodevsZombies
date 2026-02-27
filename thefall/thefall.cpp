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
        if (i == 1 ||i == 4 || i == 2 || i == 8 || i == 12 || i == 7 || i == 6)
            return true;
    }
    else if (c == "LEFT")
    {
        if (i == 6 || i == 1 ||i == 5 ||  i == 2 || i == 8 || i == 13 || i == 9)
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

int rotate_right(int cell)
{
    switch(cell) {
        case 2:  return 3;
        case 3:  return 2;
        case 4:  return 5;
        case 5:  return 4;
        case 6:  return 7;
        case 7:  return 8;
        case 8:  return 9;
        case 9:  return 6;
        case 10: return 11;
        case 11: return 12;
        case 12: return 13;
        case 13: return 10;
        default: return cell; // 1 ne tourne pas
    }
}

int rotate_left(int cell)
{
    switch(cell) {
        case 2:  return 3;
        case 3:  return 2;
        case 4:  return 5;
        case 5:  return 4;
        case 6:  return 9;
        case 7:  return 6;
        case 8:  return 7;
        case 9:  return 8;
        case 10: return 13;
        case 11: return 10;
        case 12: return 11;
        case 13: return 12;
        default: return cell; // 1 ne tourne pas
    }
}

bool isValid(vector<vector<int>> grille, int yi, int xi, string pos)
{
    // cerr << "VISIT (" << xi << "," << yi << ") pos=" << pos << " cell=" << abs(grille[yi][xi]) << endl;
    if (yi == grille.size())
    {
           cerr << "EXIT at xi=" << xi << endl;
        return true;
    }
    if (xi < 0 || xi >= grille[0].size() || yi < 0)
        return false;

    int cell = grille[yi][xi];
    // if (cell < 0)
    //     return false;
    bool locked = (cell < 0);
    cell = abs(cell);  
    if (cell == 0)
        return false;

        if (!locked)
        {
            int prev_cell = rotate_left(cell);
            if (verif_pos(prev_cell, pos))
            {
                grille[yi][xi] = prev_cell;
                sol.push_back(to_string(xi) + " " + to_string(yi) + " LEFT");
                Position p = next_pos(prev_cell, yi, xi, pos);
                if (isValid(grille, p.y, p.x, p.pos))
                    return true;
                sol.pop_back();
                grille[yi][xi] = -cell;
            }
            int next_cell = rotate_right(cell);
            if (verif_pos(next_cell, pos))
            {
                grille[yi][xi] = next_cell;
                sol.push_back(to_string(xi) + " " + to_string(yi) + " RIGHT");
                Position p = next_pos(next_cell, yi, xi, pos);
                if (isValid(grille, p.y, p.x, p.pos))
                    return true;
                sol.pop_back();
                grille[yi][xi] = -cell;
            }
            int r2 = rotate_right(rotate_right(cell));
            if (r2 != cell && verif_pos(r2, pos))
            {
                grille[yi][xi] = r2;
                sol.push_back(to_string(xi) + " " + to_string(yi) + " RIGHT");
                sol.push_back(to_string(xi) + " " + to_string(yi) + " RIGHT");
                Position p = next_pos(r2, yi, xi, pos);
                if (isValid(grille, p.y, p.x, p.pos))
                    return true;
                sol.pop_back();
                sol.pop_back();
                grille[yi][xi] = -cell;
            }
        }
        
        sol.push_back("WAIT");
        if (!verif_pos(cell, pos))
        {
            sol.pop_back();
            return false;
        }

    Position p = next_pos(cell, yi, xi, pos);
    // Pièce OK
    cerr << "OK (" << xi << "," << yi << ") → going to (" << p.x << "," << p.y << ")" << endl;
    return isValid(grille, p.y, p.x, p.pos);
}

int main()
{
    // int w; // number of columns.
    // int h; // number of rows.
    // cin >> w >> h; cin.ignore();
    vector<vector<int>> grille;
    // vector<int> v1= {0, 0, 0,  0,  0,  -3};
    // vector<int> v2= {10, 3, 3,  2,  2,  10};
    // vector<int> v3= {2, 0,  0,  0,  10, 13};
    // vector<int> v4= {11, 3, -2, 3,  1, 13};
    // vector<int> v5= {-3, 10,  0,  0,  2,  0};
    // vector<int> v6= {0, 6, 3,  3,  2,  13};
    // vector<int> v7= {0, 3,  0,  13,  4,  10};
    // vector<int> v8= {0, 13, 2,  4,  10,  0};
    // vector<int> v9= {0, 0, 0,  -3,  0, 0};
    // vector<int> v10= {3, 3, 3,  3,  3, 3};
    // vector<int> v10= {3, 0, 0,  3,  0, 0};
    vector<int> v1= {0, 0, 0,  0,  0,  -3};
    vector<int> v4= {11, 3, -2, 3,  1, 4};
    vector<int> v5= {-3, 10,  0,  0,  2,  0};
    vector<int> v6= {0, 6, 3,  3,  10,  13};
    vector<int> v7= {9, 4,  0,  13,  4,  10};
    vector<int> v10= {3, 0, 0,  0,  0, 0};
    // 0 0 0 0 0 -3
    // 8 3 3 2 2 10
    // 2 0 0 0 10 13
    // 11 3 -2 3 1 13
    // -3 10 0 0 2 0
    // 0 6 3 3 4 13
    // 0 3 0 13 -4 10
    // 0 13 2 4 10 0
    // 0 0 0 -3 0 0
     
    grille.push_back(v1);
    // grille.push_back(v2);
    // grille.push_back(v3);
    grille.push_back(v4);
    grille.push_back(v5);
    grille.push_back(v6);
    grille.push_back(v7);
    // grille.push_back(v8);
    // grille.push_back(v9);
    grille.push_back(v10);

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
    isValid(grille, 0, 5, "TOP");
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


// -3 12 8 6 3 2 7 2 7 0 0 0 0
// 11 5 13 0 0 0 3 0 3 0 0 0 0
// 0 11 2 2 3 3 8 2 -9 2 3 13 0
// 0 0 0 0 0 12 8 3 1 3 2 7 0
// 0 0 11 2 3 1 5 2 10 0 0 11 13
// 0 0 3 0 0 6 8 0 0 0 0 0 2
// 0 0 11 3 3 10 11 2 3 2 3 2 8
// 0 12 6 3 2 3 3 6 3 3 2 3 12
// 0 11 4 2 3 2 2 11 12 13 13 13 0
// 0 0 -3 12 7 8 13 13 4 5 4 10 0