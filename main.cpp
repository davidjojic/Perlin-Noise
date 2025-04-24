#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib> 
using namespace std;

int width = 1200, height = 800, cellsize = 100;
#define M_PI 3.14159
class vectori {
private:
    float rot; 
public:
    vectori(float ro)
    {
        rot = ro;
    }
    float get_rot()
    {
        return rot;
    }
};

vector<vector<vectori>> grid(width / cellsize+1, vector<vectori>(height / cellsize+1, vectori(0)));
vector<vector<double>> grad(width, vector<double>(height, 0));
void initialize_grid()
{
    for (int i = 0; i < width / cellsize; i++)
    {
        for (int j = 0; j < height / cellsize; j++)
        {
            float angle = (rand() % 360) * M_PI / 180.0; 
            grid[i][j] = vectori(angle);
        }
    }
}
int a[] = { 0,1,0,1 };
int b[] = { 0,0,1,1 };
double fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

void dot_product(int pixx, int pixy) {
    int fx = pixx / cellsize;
    int fy = pixy / cellsize;
    double count[4];

    for (int i = 0; i < 4; i++) {
        int x = fx + a[i];
        int y = fy + b[i];
        float dx = (pixx - x * cellsize) / (float)cellsize;
        float dy = (pixy - y * cellsize) / (float)cellsize;

        vectori gradient = grid[x][y];
        float grad_x = cos(gradient.get_rot());
        float grad_y = sin(gradient.get_rot());

        count[i] = grad_x * dx + grad_y * dy;
    }

    double u = fade((double)(pixx % cellsize) / cellsize);
    double v = fade((double)(pixy % cellsize) / cellsize);

    double tr1 = count[0] + u * (count[1] - count[0]);
    double tr2 = count[2] + u * (count[3] - count[2]);
    grad[pixx][pixy] = tr1 + v * (tr2 - tr1);
}

sf::Color grad_to_color(double value)
{
    int color_value = static_cast<int>((value + 1.0) * 127.5);
    return sf::Color(color_value, color_value, color_value); 
}
int main()
{
    initialize_grid(); 
    sf::RenderWindow window(sf::VideoMode(width, height),"IGRA",sf::Style::Default);
    sf::Image image;
    image.create(width, height, sf::Color::Black);
    for (int i = 0;i < width;i++)
    {
        for (int j = 0;j < height;j++)
        {
            dot_product(i, j);
            sf::Color cl;
            cl = grad_to_color(grad[i][j]);
            image.setPixel(i, j, cl);
        }
    }
    sf::Texture tx;
    tx.loadFromImage(image);
    sf::Sprite sp(tx);
    while (window.isOpen())
    {
        window.clear();
        window.draw(sp);
        window.display();
   }
}
