#include <iostream>
#include <vector>
#include <windows.h>
#include <cmath>
#include <string>
#include <typeinfo>

const int Xcenter = 960;
const int Ycenter = 540;
float dx = 0;
float dy = 0;
const double pi = acos(-1);
const float G = 6.6743 * pow(10,-11);

class figure
{
public:
    float xcord;
    float ycord;
    float angle;
    float m;
    float v;
    float vx;
    float vy;
    float a;
    float ax;
    float ay;
    float dx;
    float dy;

    std::vector<float> xpts, ypts;

    void move(float _xcord, float _ycord)
    {
        xcord = _xcord + Xcenter;
        ycord = _ycord + Ycenter;
        calculate();
    }

    virtual void show(HDC& _hdc) = 0;

    virtual void calculate() = 0;

    void rotate(float _angle)
    {
        angle += _angle;
        calculate();
    }

    void virtual SizeChange(float _value1, float _value2 = 0) = 0;
};

class circle : public figure
{
    float r;

public:

    circle(float _m, float _radius, float _x, float _y, float _vx, float _vy)
    {
        m = _m;
        r = _radius;
        xcord = _x + Xcenter;
        ycord = _y + Ycenter;
        vx = _vx;
        vy = _vy;
        calculate();
    }

    void calculate() override
    {
        xpts.clear();
        ypts.clear();
        float xpoint = -r;
        while (true)
        {
            ypts.push_back(dy + Ycenter + sqrt(r * r - xpoint * xpoint));
            xpts.push_back(dx + Xcenter + xpoint);
            ypts.push_back(dy + Ycenter - sqrt(r * r - xpoint * xpoint));
            xpts.push_back(dx + Xcenter + xpoint);
            xpts.push_back(dx + Xcenter + sqrt(r * r - xpoint * xpoint));
            ypts.push_back(dy + Ycenter + xpoint);
            xpts.push_back(dx + Xcenter - sqrt(r * r - xpoint * xpoint));
            ypts.push_back(dy + Ycenter + xpoint);
            xpoint += 1;
            if (xpoint > r)
            {
                break;
            }
        }

        for (int i = 0; i < xpts.size(); i++)
        {
            float tpts = xpts[i];
            xpts[i] = cos(angle * pi / 180) * (xpts[i] - xcord) + sin(angle * pi / 180) * (ypts[i] - ycord) + xcord;
            ypts[i] = -sin(angle * pi / 180) * (tpts - xcord) + cos(angle * pi / 180) * (ypts[i] - ycord) + ycord;
        }
    }

    void show(HDC& _hdc) override
    {
        for (int j = 0; j < xpts.size(); j++)
        {
            SetPixel(_hdc, xpts[j], ypts[j], RGB(255, 255, 255));
        }
    }

    void SizeChange(float _value1, float _value2 = 0) override
    {
        r = _value1;
        calculate();
    }

};

class map
{
public:

    std::vector<figure*> fig;

    void print(HDC& _hdc)
    {
        float r, xr, yr;
        for (int i = 0; i < fig.size(); i++)
        {
            for (int j = 0; j < fig.size(); j++)
            {
                if (i != j)
                {
                    xr = fig[i]->xcord - fig[j]->xcord;
                    yr = fig[i]->ycord - fig[j]->ycord;
                    r = xr * xr + yr * yr;
                    fig[i]->a = G * fig[j]->m / r;
                    fig[i]->ax = fig[i]->a * xr / r;
                    fig[i]->ay = fig[i]->a * yr / r;
                    fig[i]->vx += -fig[i]->ax;
                    fig[i]->vy += -fig[i]->ay;
                }
                else
                {

                }
            }
        }

        for (int i = 0; i < fig.size(); i++)
        {
            fig[i]->xcord += fig[i]->vx;
            fig[i]->ycord += fig[i]->vy;
            fig[i]->calculate();
        }

        float DXcenter=0;
        float DYcenter=0;

        for (int i = 0; i < fig.size(); i++)
        {
            DXcenter += fig[i]->xcord;
            DYcenter += fig[i]->ycord;
        }

        DXcenter = DXcenter / fig.size();
        DYcenter = DYcenter / fig.size();

        for (int i = 0; i < fig.size(); i++)
        {
            fig[i]->dx = fig[i]->xcord - DXcenter;
            fig[i]->dy = fig[i]->ycord - DYcenter;
        }

        for (int i = 0; i < fig.size(); i++)
        {
            fig[i]->show(_hdc);
        }
    }
};

int main()
{
    HWND hwnd = GetConsoleWindow();
    HDC hdc = GetDC(hwnd);
    HPEN hMyPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hdc, hMyPen);
    map m;
    figure* fig = nullptr;
    std::string cmd;
    std::vector<int> typefig;

    float v1, v2, x, y, a;
    int n1 = 0, n2 = 0;
    bool ch = false;
    circle c1(38000000000000, 1, 0, 0, 0, 0), c2(10, 20, 250, 0, 0, -0.2), c3(1, 20, -250, 0, 0, 0.2);
    m.fig.push_back(&c1);
    m.fig.push_back(&c2);
    m.fig.push_back(&c3);
  

    while (true)
    {
        m.print(hdc);
        InvalidateRect(hwnd, 0, true);
    }

    ReleaseDC(hwnd, hdc);
}