 #include "TXLib.h"
 #include <iostream>
 #include <conio.h>
 #include <time.h>
 #include "Yasuo.cpp"
 #include "ninja.cpp"

 const int TIME_FOR_MOCHILOVO = 2;
 const int TIME_FOR_USPOKOENIE = 4;


struct Product
{
    int x;
    int y;
    int width;
    int height;
    double nomer_kartinki;
    int kolich_kart;
    HDC icon;
    HDC movingPic;
    double speed_change;
    int speed;
    bool visible;
};

void obnovitNomerKartinkiTovara(Product* tovar)
{
    tovar->nomer_kartinki = tovar->nomer_kartinki + tovar->speed_change;

    if (round(tovar->nomer_kartinki) >= tovar->kolich_kart)
    {
        tovar->nomer_kartinki = 0;
    }
}





//Кибер
struct Cyber
{
    int x;
    int y;
    int speed;
    double kadr;
    bool visible;
    HDC pic;
    double opyanenie;
};

void drawCyberTime(Cyber cyber)
{
    txSetColor (TX_WHITE);
    txSetFillColor (TX_TRANSPARENT);
    txSelectFont("Segoe UI Black", 20);
    txDrawText (1261, 340,1276,500, "c\ny\nb\ne\nr");
    txRectangle(1279,311,1290,500);
    txSetFillColor(TX_BLUE);
    if (cyber.opyanenie > 10)
    {
        txRectangle(1279,311,1290,500);
    }
    else
    {
        txRectangle(1279,511 - 20 * cyber.opyanenie,1290,500);
    }
}

double getKadrCyber(Cyber cyber)
{
    if (cyber.x <= 400)
    {
        cyber.kadr = cyber.kadr + 0.09;
        if (round(cyber.kadr) >= 6 or round(cyber.kadr) <= 3)
        {
            cyber.kadr = 4;
        }
    }
    else
    {
        cyber.kadr = cyber.kadr + 0.3;
        if (round(cyber.kadr) >= 3 and cyber.x > 400)
        {
            cyber.kadr = 0;
        }
    }

    return cyber.kadr;
}

//Сфера
struct Sphere
{
    HDC pic;
    int x;
    int y;
    bool visibility;
    int speed_person;
    bool hide_person;
    int lives_person;
};

void drawSpheres(Sphere* spheres, int number_of_spheres)
{
    for (int nomer = 0; nomer < number_of_spheres; nomer = nomer + 1)
    {
        if (spheres[nomer].visibility == true)
        {
            //Сфера
            txTransparentBlt(txDC(), spheres[nomer].x, spheres[nomer].y,64,64,spheres[nomer].pic,0,0, TX_WHITE);
        }
    }
}

//Кунаи
void drawKYNAI     (Person kynai);
Person moveKynai   (Person kynai, Person ninja, bool person_alive);
//Ниндзя
int  getNewPersonY (int y, int speed);
int  getKadrPerson (int x);
Person moveNinja   (Person ninja);



//Пыль
HDC  getDust (bool alive, HDC up, HDC down, HDC left, HDC right);

void showTime(int startTime);

int main()
{
    txCreateWindow (1309,550);
    txDisableAutoPause();

    bool yasuo_won = false;
    bool nadoelo = false;

    //Время1
    int time1 = 0;
    int timeWhenpersonHide = -10000;
    int timeExplosion = 0;
    bool ninja_jump = false;


    //Ниндзя
    Person ninja =       {0, 0, txLoadImage("ninja3.bmp"), true, 10, 0, false, 40, 600, 50, 0};
    ninja.long_jump =    (txGetExtentX() - 200) / ninja.speed_jump;
    HDC ninja_jump_pic = txLoadImage("Все ниндзя.bmp");

    //Пыль
    HDC Dust_down  = txLoadImage("Пыль\\Пыль вниз.bmp");
    HDC Dust_left  = txLoadImage("Пыль\\Пыль влево.bmp");
    HDC Dust_right = txLoadImage("Пыль\\Пыль вправо.bmp");
    HDC Dust_pic   = Dust_down;

    //Кунаи
    Person kynai =       {185, 20, nullptr, false, 40};

    //Ясуо
    Yasuo yasuo =       {846, 353, txLoadImage("Ясуо.bmp"), 10, 10, true, false, Dust_pic, false};


    //Сфера
    int number_of_spheres = 3;
    Sphere spheres[10];
    spheres[0] = {txLoadImage("Сферы\\сфера.bmp"),         5700, random(500), true,  0, true,   0};
    spheres[1] = {txLoadImage("Сферы\\Сфера голубя.bmp"),  3900, random(500), true, 10, false,  0};
    spheres[2] = {txLoadImage("Сферы\\Сфера красная.bmp"), 2600, random(500), true,  0, false, -5};


    //Землятресение
    HDC Fon = txLoadImage("ФОН.bmp");
    int earthquake_start = -100000;

    //Кровь БРЫгЖаЕТ
    HDC blood = txLoadImage("бризжет кровь.bmp");

    //Время2
    time_t t;
    struct tm *tm;

    //Магазин
    HDC magazin = txLoadImage("Магазин\\Иконка магазина.bmp");
    int kolich_tovarov = 4;
    int nomer_vybora_yasuo = YASUO_PIXELAXE;
    int nomer_vybora_ninja = NINJA_EXPLOSION;
    Product tovary[kolich_tovarov];
    tovary[NINJA_EXPLOSION] = { 10, 200, 256, 267, 0, 15, txLoadImage("Иконки магазина\\Взрыв.bmp"),     txLoadImage("Магазин\\Взрыв.bmp")    ,0.5, -10, true};
    tovary[NINJA_LIGHTNING] = {200, 250, 400, 150, 0,  5, txLoadImage("Иконки магазина\\lightning.bmp"), txLoadImage("Магазин\\lightning.bmp"),0.2, -10, true};
    tovary[YASUO_PIXELAXE] =  {900, 200, 100, 100, 0,  4, txLoadImage("Иконки магазина\\pixelaxe.bmp"),  txLoadImage("Магазин\\pixelaxe.bmp") ,0.6,  10, true};
    tovary[YASUO_FIREBALL] = {1100, 200, 212, 161, 0, 12, txLoadImage("Иконки магазина\\FireBall.bmp"),  txLoadImage("Магазин\\FireBall.bmp") ,0.3,  10, true};

    //Меню
    HDC menu = txLoadImage("Меню\\menu.bmp");

    //Музыка
    txPlaySound("Звуки\\CCC.wav");

    //КОД
    txSetFillColor(TX_WHITE);
    txSetColor    (TX_BLACK,5);
    txClear();
    txSelectFont("Segoe UI Black", 200);
    txDrawText  (0, 0, txGetExtentX(), txGetExtentY(), "Press F to start");

    //Кибер
    Cyber cyber = {1400, 300, 10, 0, false, txLoadImage("megaman.bmp"), 0};

    while(!GetAsyncKeyState('F'))
    {
        txSleep(10);
    }

    //Игра пашет
    t=time(NULL);
    tm=localtime(&t);
    int startTime = 3600 * tm->tm_hour + 60 * tm->tm_min + tm->tm_sec;

    while ((yasuo.alive == true or time1 - earthquake_start < 1000) and !yasuo_won and !ninja.won and !nadoelo)
    {
        txBegin();

        txSetFillColor(TX_WHITE);
        txClear();
        txSetColor(TX_BLACK,5);


        //Нет землятрясения
        if (time1 - earthquake_start > 500)
        {
            txBitBlt(txDC(), 0, 0, 1309,550, Fon);
        }

        //есть землятрясение
        else
        {
            txBitBlt(txDC(), random(-20, 20), random(-20,20),1309,550, Fon);
            //Рисуем кровищу
            txTransparentBlt(txDC(), yasuo.x + 100, yasuo.y + 65, 130, 60, blood,0,0, TX_WHITE);
        }

        //Жизни
        txSetFillColor(TX_GRAY);
        for (int nomer_rect = 0; nomer_rect < yasuo.lifes; nomer_rect = nomer_rect + 1)
        {
            txRectangle(txGetExtentX() - 30 * (nomer_rect + 1), txGetExtentY() - 30,
                        txGetExtentX() - 30 *  nomer_rect,      txGetExtentY());
        }

        //Время
        showTime(startTime);
        time_t t=time(NULL);
        struct tm *tm;
        tm=localtime(&t);
        int currentTime = 3600 * tm->tm_hour + 60 * tm->tm_min + tm->tm_sec;

        //Ясуо
        yasuo.stolknulsya = true;
        if (time1 - timeWhenpersonHide > 2000)
        {
            txTransparentBlt(txDC(), yasuo.x, yasuo.y,134,133,yasuo.pic,0,0, TX_WHITE);
            if (yasuo.gun_visible)
            {
                txTransparentBlt(txDC(),
                    tovary[nomer_vybora_yasuo].x,
                    tovary[nomer_vybora_yasuo].y,
                    tovary[nomer_vybora_yasuo].width,
                    tovary[nomer_vybora_yasuo].height,
                    tovary[nomer_vybora_yasuo].movingPic,
                    0, tovary[nomer_vybora_yasuo].height * round(tovary[nomer_vybora_yasuo].nomer_kartinki), RGB(163, 73, 164));
            }
            yasuo.stolknulsya = false;
        }

        //Рисование сфера
        drawSpheres(spheres, number_of_spheres);

        //Ninja
        if (ninja.jump == false)
        {
            txTransparentBlt(txDC(), ninja.x, ninja.y, 190,218,ninja.pic,0,0, TX_WHITE);
        }
        else
        {
            txTransparentBlt(txDC(), ninja.x, ninja.y, 316,218,ninja_jump_pic, 316 * round(ninja.kadr),0, TX_WHITE);
        }

        if (ninja.gun_visible and
            currentTime - timeExplosion > TIME_FOR_MOCHILOVO and
            currentTime - timeExplosion < TIME_FOR_USPOKOENIE)
        {
            txTransparentBlt(txDC(),
                tovary[nomer_vybora_ninja].x,
                tovary[nomer_vybora_ninja].y,
                tovary[nomer_vybora_ninja].width,
                tovary[nomer_vybora_ninja].height,
                tovary[nomer_vybora_ninja].movingPic,
                0, tovary[nomer_vybora_ninja].height * round(tovary[nomer_vybora_ninja].nomer_kartinki), RGB(163, 73, 164));
        }

        //КУНАИ
        if (kynai.visibility == true)
        {
            drawKYNAI(kynai);
        }

        //Кунаи улетел. Перезарядка
        if (kynai.x > txGetExtentX() + 1200)
        {
            kynai.visibility = false;
            yasuo.zastryal_nozh = false;
            kynai.x = 30;
            kynai.speed = 40.0;
        }

        //Прыжок кончился
        if (ninja.jump == true and ninja.time_jump >= 1.5 * ninja.long_jump)
        {
            yasuo.zastryal_nozh = false;
        }


        //Кибер
        if (cyber.visible)
        {
            txTransparentBlt(txDC(), cyber.x, cyber.y, 250, 230, cyber.pic, 250 * round(cyber.kadr), 230, TX_WHITE);
        }
        drawCyberTime(cyber);

        if (cyber.visible)
        {
            cyber.x = cyber.x - 20;
        }
        cyber.kadr = getKadrCyber(cyber);
        if (cyber.x < ninja.x + 230)
        {
            cyber.kadr = 6;
        }


        if (cyber.x < -100)
        {
            cyber.visible = false;
            cyber.opyanenie = 0;
            cyber.x = 1400;
        }


        if (currentTime - startTime > 40)
        {
            while(currentTime - startTime < 50)
            {
                txBegin();
                txBitBlt(txDC(), 0, 0, 1309,550, Fon);
                txTransparentBlt(txDC(), txGetExtentX()/2 - 135, 50, 270,237,magazin,0,0, RGB(163, 73, 164));
                txSelectFont("Segoe UI Black", 50);
                txDrawText(0,  0, txGetExtentX(), 50, "Зачехлите мне много денег, и тогда я закрою рекламу");
                txTextOut(100, 50, "Ninja's Gun");
                txTextOut(1000,50, "Yasuo's Gun");
                for (int nomer = 0; nomer < kolich_tovarov; nomer = nomer + 1)
                {
                    int nomer_tekuschei_kartinki = tovary[nomer].nomer_kartinki;

                    if (nomer == nomer_vybora_ninja ||
                        nomer == nomer_vybora_yasuo)
                    {
                        txTransparentBlt(txDC(),
                            tovary[nomer].x,
                            tovary[nomer].y,
                            tovary[nomer].width,
                            tovary[nomer].height,
                            tovary[nomer].movingPic,
                            0, tovary[nomer].height * round(tovary[nomer].nomer_kartinki), RGB(163, 73, 164));
                    }
                    else
                    {
                        txTransparentBlt(txDC(),
                            tovary[nomer].x,
                            tovary[nomer].y,
                            tovary[nomer].width,
                            tovary[nomer].height,
                            tovary[nomer].icon,
                            0, 0,
                            RGB(163, 73, 164));
                    }

                    if (GetAsyncKeyState('A'))
                    {
                        nomer_vybora_ninja = 0;
                    }
                    else if (GetAsyncKeyState('D'))
                    {
                        nomer_vybora_ninja = 1;
                    }

                    nomer_vybora_yasuo = getNomerYasuo(nomer_vybora_yasuo);

                    obnovitNomerKartinkiTovara(&tovary[nomer]);
                }

                txSleep(10);
                txEnd();

                t=time(NULL);
                tm=localtime(&t);
                currentTime = 3600 * tm->tm_hour + 60 * tm->tm_min + tm->tm_sec;
            }

        }


        int ninja_width = 316;
        if (ninja.jump == false)
        {
            ninja_width = 160;
        }

        if (ninja.x + ninja_width   > cyber.x          and
            ninja.x                 < cyber.x + 250    and
            ninja.y + 218           > cyber.y          and
            ninja.y                 < cyber.y + 230       )
        {
            yasuo_won = true;
        }

        if (GetAsyncKeyState(VK_RSHIFT) and yasuo.alive == true and cyber.visible == false and cyber.opyanenie >= 10)
        {
            cyber.visible = true;
            cyber.y = yasuo.y;
        }




        //ЯСУО
        yasuo = moveYasuo(yasuo);

        if (GetAsyncKeyState(VK_CONTROL) && currentTime - startTime > 50 and yasuo.gun_visible == false)
        {
            yasuo.gun_visible = true;
            tovary[nomer_vybora_yasuo].x = yasuo.x - 100;
            tovary[nomer_vybora_yasuo].y = yasuo.y + 5;
        }

        if (currentTime - startTime > 50 && yasuo.gun_visible)
        {
            tovary[nomer_vybora_yasuo].x =  tovary[nomer_vybora_yasuo].x - tovary[nomer_vybora_yasuo].speed;
        }





        //Шкала прыжка
        drawJumpTime(ninja);



        //Пыль
        yasuo.dust_pic = getDust(yasuo.alive, Dust_down, Dust_down, Dust_left, Dust_right);
        obnovitNomerKartinkiTovara(&tovary[nomer_vybora_yasuo]);

        //КУНАИ
        if (
            kynai.x + 23  > yasuo.x          and
            kynai.x       < yasuo.x + 120    and
            kynai.y       > yasuo.y          and
            kynai.y -  6  < yasuo.y + 120
            )
        {
            earthquake_start = time1;
            yasuo.lifes = yasuo.lifes - 1;
            yasuo.zastryal_nozh = true;
        }

        if (
            ninja.x + 316 > yasuo.x          and
            ninja.x       < yasuo.x + 120    and
            ninja.y + 218 > yasuo.y          and
            ninja.y       < yasuo.y + 120    and
            yasuo.zastryal_nozh == false
            )
        {
            earthquake_start = time1;
            yasuo.lifes = yasuo.lifes - 3;
            yasuo.zastryal_nozh = true;
        }

        //Выйгрыш
        if (yasuo.lifes <= 0)
        {
            yasuo.alive = false;
            ninja.won = true;
        }
        if (
            ninja.jump == false              and
            ninja.x + 120 > tovary[nomer_vybora_yasuo].x          and
            ninja.x       < tovary[nomer_vybora_yasuo].x + 130    and
            ninja.y + 200 > tovary[nomer_vybora_yasuo].y          and
            ninja.y       < tovary[nomer_vybora_yasuo].y + 100    and
            nomer_vybora_yasuo == YASUO_PIXELAXE
           )
            {
                yasuo_won = true;
            }

        kynai = moveKynai(kynai, ninja, yasuo.alive);

        if (GetAsyncKeyState(VK_SPACE) and kynai.x < 1200 and kynai.visibility == false and ninja.jump == false)
        {
            kynai.visibility = true;
            kynai.y = ninja.y + 103;
        }

        if (kynai.visibility == true and yasuo.alive == true)
        {
            kynai.x = kynai.x + kynai.speed;
            kynai.speed = kynai.speed + 2;
        }


        //Движение ниндзи
        ninja = moveNinja(ninja);


        if (currentTime - timeExplosion > TIME_FOR_MOCHILOVO            and
            currentTime - timeExplosion < TIME_FOR_USPOKOENIE             and
            ninja.gun_visible == true and
            tovary[nomer_vybora_ninja].nomer_kartinki < tovary[nomer_vybora_ninja].kolich_kart - 1)
        {
            obnovitNomerKartinkiTovara(&tovary[nomer_vybora_ninja]);
        }
        else
        {
            tovary[nomer_vybora_ninja].nomer_kartinki = 0;
        }

        //По нажатию на Е пускаем молнию / взрыв
        if (GetAsyncKeyState('E') && currentTime - startTime > 10 and ninja.gun_visible == false)
        {
            ninja.gun_visible = true;
            tovary[nomer_vybora_ninja].x = ninja.x + 100;
            tovary[nomer_vybora_ninja].y = ninja.y + 5;
            tovary[nomer_vybora_ninja].nomer_kartinki = 0;

            if (nomer_vybora_ninja == NINJA_LIGHTNING)
            {
                tovary[nomer_vybora_ninja].speed = 0;
                tovary[nomer_vybora_ninja].width = txGetExtentX();
                timeExplosion = currentTime;
            }
            else if (nomer_vybora_ninja == NINJA_EXPLOSION)
            {
                tovary[nomer_vybora_ninja].speed = 0;
                tovary[nomer_vybora_ninja].x = yasuo.x - 100;
                tovary[nomer_vybora_ninja].y = yasuo.y + 5;
                timeExplosion = currentTime;
            }
        }

            //Отнимаем жизни при попадании взрыва / молнии
            if (
                tovary[nomer_vybora_ninja].x + tovary[nomer_vybora_ninja].width     > yasuo.x          and
                tovary[nomer_vybora_ninja].x                                        < yasuo.x + 115    and
                tovary[nomer_vybora_ninja].y + tovary[nomer_vybora_ninja].height    > yasuo.y          and
                tovary[nomer_vybora_ninja].y                                        < yasuo.y + 115
                )
            {
                if (
                    nomer_vybora_ninja == NINJA_EXPLOSION                               and
                    currentTime - timeExplosion > TIME_FOR_MOCHILOVO                    and
                    currentTime - timeExplosion < TIME_FOR_USPOKOENIE
                    )
                {
                    yasuo.lifes = yasuo.lifes - 1;
                }

                if (
                    nomer_vybora_ninja == NINJA_LIGHTNING                  and
                    currentTime - timeExplosion > TIME_FOR_MOCHILOVO and
                    currentTime - timeExplosion < TIME_FOR_USPOKOENIE       and
                    ninja.gun_visible == true
                    )
                {
                    yasuo.lifes = yasuo.lifes - 0.4;
                    ninja.speed = 0;
                }
            }



        if (currentTime - startTime > 50 && ninja.gun_visible)
        {
            tovary[nomer_vybora_ninja].x =  tovary[nomer_vybora_ninja].x - tovary[nomer_vybora_ninja].speed;
        }


        for (int nomer = 0; nomer < number_of_spheres; nomer = nomer + 1)
        {
            spheres[nomer].x = spheres[nomer].x - 10;
            spheres[nomer].y = spheres[nomer].y + random(-10, 10);
        }

        for (int nomer = 0; nomer < number_of_spheres; nomer = nomer + 1)
        {
            if (spheres[nomer].visibility == true        and
                spheres[nomer].x + 64 > yasuo.x          and
                spheres[nomer].x      < yasuo.x + 120    and
                spheres[nomer].y + 64 > yasuo.y          and
                spheres[nomer].y      < yasuo.y + 120)
            {
                if (spheres[nomer].speed_person != 0)
                {
                    yasuo.speed = yasuo.speed + spheres[nomer].speed_person;
                }
                if (spheres[nomer].lives_person != 0)
                {
                    yasuo.lifes = yasuo.lifes + spheres[nomer].lives_person;
                }
                if (spheres[nomer].hide_person != false)
                {
                    timeWhenpersonHide = time1;
                }

                spheres[nomer].visibility = false;
            }
        }

        //Меню в игре
        if (GetAsyncKeyState(VK_ESCAPE) and yasuo.alive == true)
        {
            bool returnToGame = false;
            const int WIDTH_MENU = 640;
            const int HEIGHT_MENU = 364;

            RECT rectReturn = {
                (txGetExtentX() - WIDTH_MENU) / 2 + 120,
                (txGetExtentY() - HEIGHT_MENU) / 2 + 190,
                (txGetExtentX() - WIDTH_MENU) / 2 + 500,
                (txGetExtentY() - HEIGHT_MENU) / 2 + 220};

            RECT rectExit = {
                (txGetExtentX() - WIDTH_MENU) / 2 + 120,
                (txGetExtentY() - HEIGHT_MENU) / 2 + 250,
                (txGetExtentX() - WIDTH_MENU) / 2 + 500,
                (txGetExtentY() - HEIGHT_MENU) / 2 + 280};

            while (!returnToGame)
            {
                txTransparentBlt(txDC(),
                    (txGetExtentX() - WIDTH_MENU) / 2,
                    (txGetExtentY() - HEIGHT_MENU) / 2,
                    WIDTH_MENU,HEIGHT_MENU, menu, 0, 0, TX_WHITE);

                if (In (txMousePos(), rectReturn) && txMouseButtons() == 1)
                {
                    returnToGame = true;
                }
                if (In (txMousePos(), rectExit) && txMouseButtons() == 1)
                {
                    returnToGame = true;
                    nadoelo = true;
                }
                txSleep(10);
            }

        }

        txEnd();

        txSleep(10);
        time1 = time1 + 10;
        cyber.opyanenie = cyber.opyanenie + 0.03;
    }




    if( ninja.won == true )
    {
        txSetFillColor(TX_WHITE);
        txSetColor    (TX_BLACK,5);
        txSelectFont("Segoe UI Black", 150);
        txClear();
        txPlaySound(NULL);
        txPlaySound("Звуки\\FATALITY.wav");
        txDrawText  (0, 0, txGetExtentX(), txGetExtentY(), "Ninja Won!!!");
    }
    else if (yasuo_won == true)
    {
        txSetFillColor(TX_WHITE);
        txSetColor    (TX_BLUE,5);
        txSelectFont("Segoe UI Black", 150);
        txClear();
        txPlaySound(NULL);
        txPlaySound("Звуки\\FATALITY.wav");
        txDrawText  (0, 0, txGetExtentX(), txGetExtentY(), "Yasuo Won!!!");
    }

    txDeleteDC (yasuo.pic);
    txDeleteDC (ninja.pic);
    txDeleteDC (ninja_jump_pic);
    for (int nomer = 0; nomer < number_of_spheres; nomer = nomer + 1)
    {
        txDeleteDC (spheres[nomer].pic);
    }
    txDeleteDC (Fon);
    txDeleteDC (blood);
    txDeleteDC (Dust_down);
    txDeleteDC (Dust_left);
    txDeleteDC (Dust_right);
    txDeleteDC (yasuo.dust_pic);

    return 0;
}


HDC  getDust(bool alive, HDC up, HDC down, HDC left, HDC right)
{
    HDC Dust_pic = up;
    if (GetAsyncKeyState(VK_LEFT) and alive == true)
    {
        Dust_pic = right;
    }
    else if (GetAsyncKeyState(VK_RIGHT) and alive == true)
    {
        Dust_pic = left;
    }
    else if (GetAsyncKeyState(VK_UP) and  alive == true)
    {
        Dust_pic = down;
    }
    else if (GetAsyncKeyState(VK_DOWN) and alive == true)
    {
        Dust_pic = down;
    }

    return Dust_pic;
}

void showTime(int startTime)
{
    txSetColor(TX_WHITE);
    txSelectFont("Segoe UI Black", 40);
    time_t t=time(NULL);
    struct tm *tm;
    tm=localtime(&t);
    int currentTime = 3600 * tm->tm_hour + 60 * tm->tm_min + tm->tm_sec;

    char str[100];
    sprintf(str, "Sec %d", currentTime - startTime);
    txTextOut(txGetExtentX() - 60, 490, str);
}


void drawKYNAI (Person kynai)
{
    txSetColor (TX_BLACK,2);
    txCircle   (     kynai.x,-2 + kynai.y,3);
    txLine     (     kynai.x,-3 + kynai.y,12 + kynai.x,-6 + kynai.y);
    txLine     (     kynai.x,-3 + kynai.y,12 + kynai.x,     kynai.y);
    txLine     (12 + kynai.x,-6 + kynai.y,23 + kynai.x,-3 + kynai.y);
    txLine     (12 + kynai.x,kynai.y,23 + kynai.x,-3 + kynai.y);
}

Person moveKynai   (Person kynai, Person ninja, bool person_alive)
{


    if (kynai.visibility == true and person_alive == true)
    {
        kynai.x = kynai.x + kynai.speed;
        kynai.speed = kynai.speed + 2;
    }

    return kynai;
}
