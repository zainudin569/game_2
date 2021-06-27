#include "TXLib.h"
const int W = 1070;
const int H = 650;
struct Ball;
struct Key;

void MoveBall();
void CollisionBall (Ball* ball_p, Ball* ball_b);
void ScoreDraw (int score1, int score2);

//---------------------------------------------------------------------------------

int main()
    {
    txCreateWindow (W, H);

    MoveBall();

    return 0;
    }

//---------------------------------------------------------------------------------

struct Key
    {
    int  key_left, key_right , key_up, key_down;

    void Control (Ball* ball, int* F4_Col);
    };

struct Ball
    {
    int  x, y , vx, vy;

    int r;

    int pl;

    COLORREF Color;
    COLORREF FillColor;

    void Physics (int* score1, int* score2, double ax, double ay, int dt);
    void Drow();
    };

//---------------------------------------------------------------------------------

void MoveBall()
    {
    //srand(time(NULL));

    Ball ball1 = { W/2 + rand() % W/2 ,  rand() % H, 0, 0,
                   40,
                   1,
                   RGB(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200),
                   RGB(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200) };

    Ball ball2 = { rand() % W/2,  rand() % H, 0, 0,
                   40,
                   2,
                   RGB(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200),
                   RGB(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200) };

    Ball ball3 = { 900,  500, 0, 0,
                   25,
                   0,
                   RGB(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200),
                   RGB(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200) };

    Key plaer1 = { VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN };
    Key plaer2 = { 'A', 'D', 'W', 'S' };

    int dt = 1;
    int F4_Col = 0;
    double ax = 0, ay = 0.9;
    int score1 = 0, score2 = 0;

    HDC Fon = txLoadImage ("Pictures\\Fon.bmp");

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        txBegin();
        txSetFillColor (TX_BLACK);
        txClear();

        txBitBlt (txDC(), 0, 50, 0, 0, Fon);

        //printf ("In CraziBall(): x  = %d and y  = %d\n", x2, y2);
        //printf ("In CraziBall(): vx = %d and vy = %d\n", vx2, vy2);

        plaer1 .Control (&ball3, &F4_Col);
        //plaer2 .Control (&ball2, &F4_Col);

        //ball1 .Physics (&score1, &score2, ax, ay, dt);
        //ball2 .Physics (&score1, &score2, ax, ay, dt);
        ball3 .Physics (&score1, &score2, ax, ay, dt);

        //CollisionBall (&ball1, &ball3);//проверка столкновения
        //CollisionBall (&ball2, &ball3);

        //ball1.Drow();
        //ball2.Drow();
        ball3.Drow();

        ScoreDraw (score1, score2);

        txEnd();
        txSleep (1);
        }

    txDeleteDC (Fon);
    }

//---------------------------------------------------------------------------------

void Ball::Drow()
    {
    txSetColor ((Color), 2);
    txSetFillColor (FillColor);

    txCircle (x, y, r);
    txLine (x, y, x + vx*5, y + vy*5);
    txCircle (x + vx*5, y + vy*5, 3);

    if ( pl == 1 or pl == 2 )
        {
        vx = 0;
        vy = 0;
        }
    }

//---------------------------------------------------------------------------------

void Key::Control (Ball* ball, int* F4_Col)
    {
    if (txGetAsyncKeyState (key_left))  (*ball) .vx = (*ball) .vx - 15;
    if (txGetAsyncKeyState (key_right)) (*ball) .vx = (*ball) .vx + 15;
    if (txGetAsyncKeyState (key_up))    (*ball) .vy = (*ball) .vy - 15;;
    if (txGetAsyncKeyState (key_down))  (*ball) .vy = (*ball) .vy + 15;;

    if (txGetAsyncKeyState (VK_SPACE)) (*ball) .vy = (*ball) .vx = 0;

    if (txGetAsyncKeyState (VK_F1))
        {
        (*ball) .Color = TX_LIGHTRED;
        (*ball) .FillColor = TX_RED;
        *F4_Col = 0;
        }

    if (txGetAsyncKeyState (VK_F2))
        {
        (*ball) .Color = TX_LIGHTBLUE;
        (*ball) .FillColor = TX_BLUE;
        *F4_Col = 0;
        }

    if (txGetAsyncKeyState (VK_F3))
        {
        (*ball) .Color = TX_LIGHTGREEN;
        (*ball) .FillColor = TX_GREEN;
        *F4_Col = 0;
        }

    if (txGetAsyncKeyState (VK_F4))
        {
        *F4_Col = 1;
        }

    if (*F4_Col == 1)
        {
        (*ball) .Color     = RGB((*ball) .x,   (*ball) .y,   150);
        (*ball) .FillColor = RGB((*ball) .x/2, (*ball) .y/2, 150);
        }
    }

//---------------------------------------------------------------------------------

void Ball::Physics (int* score1, int* score2, double ax, double ay, int dt)
    {
    int W_org   = W;
    int W_org_2 = 0;

    if (vy >  25) vy =  25;//ограничение на скорость движения
    if (vx >  25) vx =  25;
    if (vy < -25) vy = -25;
    if (vx < -25) vx = -25;

    vx = vx + ROUND (ax * dt);
    vy = vy + ROUND (ay * dt);


    (*this) .x += (*this) .vx * dt;
    (*this) .y += (*this) .vy * dt;

    if (pl == 1) // ограничения для pl = 1
        {
        W_org   = W;
        W_org_2 = W/2;
        }

    if (pl == 2) // ограничения для pl = 2
        {
        W_org   = W/2;
        W_org_2 = 0;
        }

    if (x > W_org - r)
        {
        vx = - vx;
         x = W_org - r;

        /*if (pl == 0)
            {
            ++(*score1);
            x = W/2; vx = 1 + rand() % 9; vy = 1 + rand() % 9;
            txPlaySound ("sounds/zvuk-krika-gol.wav");
            Sleep (200);
            }*/
        }

    if (y > H - r)
        {
        vy = - vy;
         y = H - r;
        }

    if (x < W_org_2 + r)
        {
        vx = - vx;
         x = W_org_2 + r;

        /*if ( pl == 0)
            {
            ++(*score2);
            x = W/2; vx = 1 + rand() % 9; vy = 1 + rand() % 9;
            txPlaySound ("sounds/zvuk-krika-gol.wav");
            Sleep (200);
            } */
        }

    if (y < 50 + r)
        {
        vy = - vy;
         y = 50 + r;
        }
    }

//---------------------------------------------------------------------------------

void CollisionBall (Ball* ball_p, Ball* ball_b)
    {
     double Dx = (*ball_b) .x - (*ball_p) .x; //стороны треугольника
     double Dy = (*ball_b) .y - (*ball_p) .y; //стороны треугольника
     double d = sqrt (Dx*Dx + Dy*Dy); if (d == 0) d = 0.01; // гипотенуза
	 double sin = Dx/d; //sin угла треугольника
	 double cos = Dy/d; //cos угла треугольника

	 if (d < (*ball_b) .r + (*ball_p) .r) //проверка столкновения
        {
        txPlaySound ("sounds/Zvuk_Ball.wav");

        double Vn1 = (*ball_p) .vx*sin + (*ball_p) .vy*cos; //поворот системы координат шар1
        double Vn2 = (*ball_b) .vx*sin + (*ball_b) .vy*cos; //поворот системы координат шар2

        if ((Vn1 - Vn2) == 0) Vn1 = Vn1 + 0.01;

        double dt = ((*ball_b) .r + (*ball_p) .r - d)/(Vn1 - Vn2); //удаление залипания

        if (dt >  0.6) dt =  0.6; //ограничение на dt, чтобы мяч не отскакивал далеко
        if (dt < -0.6) dt = -0.6;

        (*ball_b) .x -= ROUND ((*ball_b) .vx*dt);
        (*ball_b) .y -= ROUND ((*ball_b) .vy*dt);
        (*ball_p) .x -= ROUND ((*ball_p) .vx*dt);
        (*ball_p) .y -= ROUND ((*ball_p) .vy*dt);

        Dx = (*ball_b) .x - (*ball_p) .x;
        Dy = (*ball_b) .y - (*ball_p) .y;
        d = sqrt (Dx*Dx + Dy*Dy); if (d == 0) d=0.01;
        sin = Dx/d;
        cos = Dy/d;

        Vn1 = (*ball_p) .vx*sin + (*ball_p) .vy*cos;
        Vn2 = (*ball_b) .vx*sin + (*ball_b) .vy*cos;

        double Vt2 = -(*ball_b) .vx*cos + (*ball_b) .vy*sin; //поворот системы координат шар2

        Vn2 = Vn1 - Vn2;

        (*ball_b) .vx = ROUND (Vn2*sin - Vt2*cos);
        (*ball_b) .vy = ROUND (Vn2*cos + Vt2*sin);

        (*ball_b) .x +=ROUND ((*ball_b) .vx*dt);
        (*ball_b) .y +=ROUND ((*ball_b) .vy*dt);
        (*ball_p) .x +=ROUND ((*ball_p) .vx*dt);
        (*ball_p) .y +=ROUND ((*ball_p) .vy*dt);
        //printf (" dt  = %f\n", dt);
        //txSleep (3000);
	 }

}

//---------------------------------------------------------------------------------

void ScoreDraw (int score1, int score2)
    {
    txSelectFont ("TimesNewRoman", 50);
    char str[12] = "";
    sprintf (str, "%d : %d", score1, score2);

    int textSizeX = txGetTextExtentX (str)/2 + 10,
        textSizeY = txGetTextExtentY (str);

    txSetColor ((TX_WHITE), 2);
    txSetFillColor (TX_BLACK);
    txRectangle (W/2 - textSizeX, 2,
                 W/2 + textSizeX, textSizeY);

    txSetTextAlign (TA_CENTER);
    txTextOut (txGetExtentX() / 2, 2, str);
    }


