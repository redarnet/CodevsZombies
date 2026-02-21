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
        return false;
    if (c == "TOP")
    {
        if (i == 1 || i == 3 || i == 4 || i == 5 || 7 || i == 9 || i == 10 || i == 11)
            return true;
    }
    else if (c =="RIGHT")
    {
        if (i == 1 ||i == 4 || i == 2 || i == 8 || i == 12 || i == 7)
            return true;
    }
    else if (c == "LEFT")
    {
        if (i == 1 ||i == 5 || i == 2 || i == 8 || i == 13 || i == 9)
            return true;
    }
    return false;
}



Position next_pos(char c, int yi, int xi, string posi)
{
        int xp = xi;
        int yp = yi;
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
        if (yi ==  yp - 1)
            posp = "BOT";
        else if (yi == yp +1)
            posp = "TOP";
        else if (xi ==  xp - 1)
            posp = "LEFT";
        else if (xi == xp + 1)
            posp = "RIGHT";
    // next_pos == pos + posp 
    Position p(xi, yi, posp);
    return p;
    
}
vector<string> sol;

vector<string> simu(vector<vector<int>> grille, int yi, int xi, string pos)
{

    Position p(xi, yi, pos);
    cout << "POS = " << p.x << endl;
    cout << "POS = " << p.y << endl;
    int x =  xi;
    int y = yi;
    bool verif = true;
    while ( p.y  != grille.size())
    {
        cout << " POS =" <<  p.y << " " << p.x << endl;
        cout << " POS =" <<  grille[p.y][p.x] << endl;
        x =  p.x;
        y = p.y;
        p = next_pos(grille[p.y][p.x], p.y, p.x, pos); 
        if ( p.y != grille.size() && !verif_pos(grille[p.y][p.x], p.pos))
        {
            cout << " ERROR" << endl;
            verif = false;
            break;
            // return false;
        }
    }
    if (!verif)
    {
        // verif if grille valide > si oui ok si nn test - 1  sinon test + 2 etc 
        if (grille[y][x] <= 12)
            grille[y][x] += 1;
        else if (grille[y][x] == 13)
            grille[y][x] = 1;
        cout << " One  loop" << endl;
        // cout << " POS =" <<  y << " " << p.x << endl;
        // cout << " GRILLE[]" << grille[y][x] <<  endl;
        string s = to_string(x) + " " + to_string(y) + " RIGHT";  
        sol.push_back(s);
        simu(grille, yi, xi, pos);
    }
    //     relance simu avec backtracing et piece fausse + 1;nb par piece == pos ? 
    // if (simu == true good)
    //     get all change  
    return sol;

}


// 0 0 -3 0 0
// 0 0 2 0 0
// 0 0 -3 0 0
// 0 0 -27 0 0 
// 0 0 2 0 0 
// 0 0 -27 0 0 


// 0 0 0 0 0 3 
// 8 3 3 2 2 10 
// 2 0 0 0 10 13 
// 11 3 2 3 1 13 
// 3 10 0 0 2 0 
// 0 6 3 3 4 13 
// 0 3 0 13 4 10 
// 0 13 2 4 10 0 
// 0 0 0 3 0 0 

int main()
{
    // int w; // number of columns.
    // int h; // number of rows.
    // cin >> w >> h; cin.ignore();
    vector<vector<int>> grille;
    vector<int> v1= {0,0,0,0,0,3};
    vector<int> v2= {8,3,3,2,2,10};
    vector<int> v3= {2,0,0,0,10,13};
    vector<int> v4= {11,3,2,3,1,13};
    vector<int> v5= {3,10,0,0,2,0};
    vector<int> v6= {0,6,3,3,4,13};
    vector<int> v7= {0,3,0,1 ,3,4,10};
    vector<int> v8= {0,13,2,4, 10,0};
    vector<int> v9= {0,0,0,3, 0,0};
    grille.push_back(v1);
    grille.push_back(v2);
    grille.push_back(v3);
    grille.push_back(v4);
    grille.push_back(v5);
    grille.push_back(v6);
    grille.push_back(v7);
    grille.push_back(v8);
    grille.push_back(v9);
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
    vector<string> sol;
    sol = simu(grille, 0, 5, "TOP");
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

