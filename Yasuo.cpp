const int YASUO_PIXELAXE = 2;
const int YASUO_FIREBALL = 3;

//ясуо
struct Yasuo
{
    int x;
    int y;
    HDC pic;
    int speed;
    double lifes;
    bool alive;
    bool zastryal_nozh;
    HDC dust_pic;
    bool stolknulsya;
    bool gun_visible;
};

Yasuo moveYasuo(Yasuo yasuo);
Yasuo moveYasuo(Yasuo yasuo)
{
    if (GetAsyncKeyState(VK_UP) && yasuo.y > 0 and yasuo.alive == true)
    {
        yasuo.y = yasuo.y - yasuo.speed;

        //ѕыль
        if (yasuo.stolknulsya == false)
        {
            txTransparentBlt(txDC(), yasuo.x, yasuo.y + 75,100,100,yasuo.dust_pic,0,0, TX_WHITE);
        }
    }
    else if (GetAsyncKeyState(VK_DOWN) && yasuo.y < 550 - 170 and yasuo.alive == true)
    {
        yasuo.y = yasuo.y + yasuo.speed;
        //ѕыль
        if (yasuo.stolknulsya == false)
        {
            txTransparentBlt(txDC(), yasuo.x, yasuo.y + 75,100,100,yasuo.dust_pic,0,0, TX_WHITE);
        }
    }

    if (GetAsyncKeyState(VK_LEFT) and yasuo.x > 340    and yasuo.alive == true)
    {
        yasuo.x = yasuo.x - yasuo.speed;
        //ѕыль
        if (yasuo.stolknulsya == false)
        {
            txTransparentBlt(txDC(), yasuo.x, yasuo.y + 75,100,100,yasuo.dust_pic,0,0, TX_WHITE);
        }
    }

    else if (GetAsyncKeyState(VK_RIGHT)  && yasuo.x < 1190 and yasuo.alive == true)
    {
        yasuo.x = yasuo.x + yasuo.speed;
        //ѕыль
        if (yasuo.stolknulsya == false)
        {
            txTransparentBlt(txDC(), yasuo.x, yasuo.y + 75,100,100,yasuo.dust_pic,0,0, TX_WHITE);
        }
    }

    return yasuo;
}

int getNomerYasuo(int nomer_vybora_yasuo);
int getNomerYasuo(int nomer_vybora_yasuo)
{
    if (GetAsyncKeyState(VK_LEFT))
    {
        nomer_vybora_yasuo = YASUO_PIXELAXE;
    }
    else if (GetAsyncKeyState(VK_RIGHT))
    {
        nomer_vybora_yasuo = YASUO_FIREBALL;
    }

    return nomer_vybora_yasuo;
}
