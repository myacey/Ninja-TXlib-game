const int NINJA_EXPLOSION = 0;
const int NINJA_LIGHTNING = 1;

struct Person
{
    int    x;
    int    y;
    HDC    pic;
    bool   visibility;
    int    speed;
    double kadr;
    bool   jump;
    int    speed_jump;
    int    time_jump;
    int    long_jump;
    int    time_hit;
    bool   gun_visible;
    bool   won;
};

void drawJumpTime(Person ninja)
{
    txSetColor (TX_WHITE);
    txSetFillColor (TX_TRANSPARENT);
    txSelectFont("Segoe UI Black", 20);
    txTextOut(0, 505, "jump");
    txRectangle(0,528,195,550);
    txSetFillColor(TX_GRAY);
    if (ninja.time_hit > 195)
    {
        txRectangle(0,528,195,550);
    }
    else
    {
        txRectangle(0,528,ninja.time_hit,550);
    }
}

int  getKadrPerson(int x)
{
    int kadr_ninja = 0;

    if (x < txGetExtentX() - 350)
    {
        kadr_ninja = 0;
    }
    else
    {
        kadr_ninja = 1;
    }

    return kadr_ninja;
}

Person moveNinja(Person ninja)
{
    //Прыжок
    if (GetAsyncKeyState('Q') && /*ninja.jump == false*/ninja.time_hit >= 2 * ninja.long_jump + 150)
    {
        ninja.jump = true;
        ninja.time_jump = 0;
        ninja.time_hit = 0;
    }
    else
    {
        //ninja.time_jump = ninja.time_jump + 1;
    }

    //Кадр (положение меча)
    ninja.kadr = getKadrPerson(ninja.x);

    //Если не прыгает
    if (ninja.jump == false)
    {
        //Движение по y
        if (GetAsyncKeyState('W') and ninja.y > 0)
        {
            ninja.y = ninja.y - ninja.speed;
        }
        else if (GetAsyncKeyState('S') and ninja.y < 332)
        {
            ninja.y = ninja.y + ninja.speed;
        }
    }
    else
    {
        //Прыжок
        if (ninja.time_jump < ninja.long_jump)
        {
            ninja.x = ninja.x + ninja.speed_jump;
        }
        else
        {
            ninja.x = ninja.x - ninja.speed_jump;
        }

        ninja.time_jump = ninja.time_jump + 1;
    }

    //Прыжок
    if (ninja.time_jump >= 2 * ninja.long_jump)
    {
        ninja.jump = false;
    }

    ninja.time_hit = ninja.time_hit + 1;

    return ninja;
}

int getNomerNinja(int nomer_vybora_ninja)
{
    if (GetAsyncKeyState('A'))
    {
        nomer_vybora_ninja = 0;
    }
    else if (GetAsyncKeyState('D'))
    {
        nomer_vybora_ninja = 1;
    }

    return nomer_vybora_ninja;
}

