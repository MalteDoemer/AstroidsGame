#include "ConsoleApplication.h"

class Astroids : public ConsoleApp
{
    struct SpaceShip
    {
        vd2d pos;
        vd2d vel;
        double heading;
        double scale;
    };

    struct Bullet
    {
        vd2d pos;
        vd2d vel;

        bool remove = false;
    };

    struct Astroid
    {
        vd2d pos;
        vd2d vel;
        double radius;
        bool remove = false;
    };

    wchar_t title[128];

    const double smallestSize = 4.0;
    const double spray = 6.0;

    std::vector<vd2d> SpaceShipModel;
    std::vector<vd2d> AstroidModel;
    Pixel BulletModel;

    std::vector<Astroid> astroids;
    std::vector<Astroid> toBeSpawned;

    std::vector<Bullet> bullets;
    SpaceShip player;

    double shootDelay;

    int round;
    int kills;
    bool gameOver;

    virtual vs2d GetPreferredSize() override
    {
        appName = L"Astroids";
        return {175, 120};
    }

    virtual vs2d GetPreferredFont() override
    {
        return {5, 5};
    }

    virtual void Draw(int x, int y, Pixel p) override
    {
        WrapAround(x, y, x, y);
        if (x >= 0 && x <= width && y >= 0 && y <= height)
            buffer[y * width + x] = p;
    }

    virtual bool Setup() override
    {
        EnableQuickEdit(false);
        EnableMouseEvents(true);
        SetResizable(false);
        SetCursorVisible(false);

        BulletModel = {L'\x25cf', FG_GREEN};
        SpaceShipModel = {
            {+0.0, -5.0},
            {-2.5, +2.5},
            {+2.5, +2.5},
        };

        int verts = 20;
        double a = 0;
        for (int i = 0; i < verts; i++)
        {
            AstroidModel.push_back({cos(a), sin(a)});
            a += 6.283185307179586 / verts;
        }

        ResetGame();
        return true;
    }

    virtual bool Update(float elapsedTime) override
    {
        if (keys[VK_ESCAPE].Held())
            return false;

        if (!gameOver)
        {
            // User input
            if (keys[VK_SPACE].Held())
                SpawnBullet(elapsedTime);
            if (keys[VK_LEFT].Held())
                player.heading -= 3 * elapsedTime;
            if (keys[VK_RIGHT].Held())
                player.heading += 3 * elapsedTime;
            if (keys[VK_UP].Held())
            {
                player.vel.x += cos(player.heading - HALF_PI) * 20 * elapsedTime;
                player.vel.y += sin(player.heading - HALF_PI) * 20 * elapsedTime;
            }

            //Clear the Screen
            Clear(Pixels::BLACK);

            // Update Player
            player.pos += player.vel * elapsedTime;
            WrapAround(player.pos.x, player.pos.y, player.pos.x, player.pos.y);
            FillWireFrame(SpaceShipModel, player.pos.x, player.pos.y, player.scale, player.heading, {PIXEL_HALF, (WORD)(BG_BLACK | FG_DARK_GREY)});

            //Update Astroids
            for (auto &a : astroids)
            {
                a.pos += a.vel * elapsedTime;
                WrapAround(a.pos.x, a.pos.y, a.pos.x, a.pos.y);
                FillCircle(a.pos.x, a.pos.y, a.radius, Pixels::GREY);
                DrawCircle(a.pos.x, a.pos.y, a.radius + 1, Pixels::DARK_GREY);

                if (CollideWithPlayer(a))
                    gameOver = true;

                // Check against every bullet if they collide
                for (auto &b : bullets)
                {
                    if ((b.pos - a.pos).mag() < a.radius)
                    {
                        a.remove = true;
                        b.remove = true;
                        kills++;
                        if (a.radius > smallestSize)
                        {
                            SpawnAstroid(a.pos.x, (a.pos.y - spray), a.radius / 2.0);
                            SpawnAstroid(a.pos.x, (a.pos.y + spray), a.radius / 2.0);
                            SpawnAstroid((a.pos.x - spray), a.pos.y, a.radius / 2.0);
                        }
                    }
                }
            }

            //Update Bullets
            for (auto &b : bullets)
            {
                b.pos += b.vel * elapsedTime;
                if (b.pos.x < 0 || b.pos.x > width || b.pos.y < 0 || b.pos.y > height)
                    b.remove = true;
                else
                    Draw(b.pos.x, b.pos.y, BulletModel);
            }

            // Delete everything thats dead
            for (int i = 0; i < astroids.size(); i++)
            {
                if (astroids[i].remove)
                {
                    astroids.erase(astroids.begin() + i);
                    i--;
                }
            }
            for (int i = 0; i < bullets.size(); i++)
            {
                if (bullets[i].remove)
                {
                    bullets.erase(bullets.begin() + i);
                    i--;
                }
            }

            //Add all spawned Astroids
            for (auto a : toBeSpawned)
                astroids.push_back(a);
            toBeSpawned.clear();

            if (astroids.size() == 0)
                SpawnRandomAstroids(++round + 2);


            swprintf_s(title, L"Console Game %s  -  Kills %i  -  Round %i  -  FPS %1.0f", appName.c_str(), kills, round, 1 / elapsedTime);
            SetConsoleTitleW(title);
        }
        else 
        {
            swprintf_s(title, L"Console Game %s  -  Kills %i  -  Round %i  -  Press ENTER to restart", appName.c_str(), kills, round);
            SetConsoleTitleW(title);

            for (int i = 0; i < bufferLen; i++)
			{
				buffer[i].Attributes |= 0x0080;
				buffer[i].Char.UnicodeChar = PIXEL_HALF;
			}

			if (keys[VK_RETURN].Held())
				ResetGame();
        }

        return true;
    }

    void SpawnBullet(float elapsedTime)
    {
        shootDelay += elapsedTime;
        if (shootDelay > 0.2)
        {
            shootDelay = 0;
            Bullet b;
            vd2d offset = {0, -5};
            offset *= player.scale;
            offset = offset.rotate(player.heading);
            b.pos = player.pos + offset;
            double a = player.heading - HALF_PI;
            b.vel = {cos(a), sin(a)};

            b.vel *= (player.vel.mag() + 30);
            bullets.push_back(b);
        }
    }
    void SpawnAstroid(double x, double y, double radius)
    {
        Astroid astr;
        astr.pos.x = x;
        astr.pos.y = y;
        astr.vel = {1, 0};
        astr.vel = astr.vel.rotate(((double)rand() / (double)RAND_MAX) * 6.283185307179586);
        astr.vel *= 5;
        astr.radius = radius;
        toBeSpawned.push_back(astr);
    }
    void SpawnRandomAstroids(int amount)
    {
        double a = 0;
        for (int i = 0; i < amount; i++)
        {
            vd2d p = {cos(a) * 50.0, sin(a) * 50.0};
            p += player.pos;
            SpawnAstroid(p.x, p.y, 10);
            a += 6.283185307179586 / amount;
        }
    }
    bool CollideWithPlayer(Astroid a)
    {
        if ((player.pos - a.pos).mag() < a.radius)
            return true;

        int vertCount = SpaceShipModel.size();
        std::vector<vd2d> verts;
        verts.resize(vertCount);
        double angle = player.heading;
        for (int i = 0; i < vertCount; i++)
        {
            verts[i].x = SpaceShipModel[i].x * cos(angle) - SpaceShipModel[i].y * sin(angle);
            verts[i].y = SpaceShipModel[i].x * sin(angle) + SpaceShipModel[i].y * cos(angle);
            verts[i] *= player.scale;
            verts[i].x += player.pos.x;
            verts[i].y += player.pos.y;
        }

        for (auto &v : verts)
            if ((v - a.pos).mag() < a.radius)
                return true;

        return false;
    }

    void ResetGame()
    {
        astroids.clear();
        bullets.clear();
        toBeSpawned.clear();
        gameOver = false;
        kills = 0;
        round = 0;
        shootDelay = 0.0;

        player.heading = PI;
        player.pos = {(double)width / 2.0, (double)height / 2.0};
        player.scale = 2;
        player.vel = {0.0, 1.0};

        SpawnRandomAstroids(2);
    }
};

int main()
{
    Astroids game;
    game.Construct();
    game.Start();
    return 0;
}