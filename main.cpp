#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class obiekt {
public:
    sf::RectangleShape obj;
    float pozycja[2];
    float rozmiar[2];
    obiekt(float position[2], float size[2], int kolor[3]) {
        obj.setPosition(position[0], position[1]);
        obj.setSize(sf::Vector2f(size[0], size[1]));
        obj.setFillColor(sf::Color(kolor[0], kolor[1], kolor[2]));
        pozycja[0] = position[0];
        pozycja[1] = position[1];
        rozmiar[0] = size[0];
        rozmiar[1] = size[1];
    };
    inline void updatePozycja(int y) {
        pozycja[1] += y;
        obj.setPosition(pozycja[0], pozycja[1]);
    }
    ~obiekt() {};
};

class auta
{
public:
    sf::Sprite sprite;
    float boki[4];
    float pozycja[2];
    float wsp_srodka[2];
    float speed;
    auta(sf::Texture& textura, float srodek[2]) {
        sprite.setTexture(textura);
        sprite.setOrigin(srodek[0], srodek[1]);
        sprite.setScale(0.99, 0.99);
        wsp_srodka[0] = srodek[0];
        wsp_srodka[1] = srodek[1];
        pozycja[0] = 1;
        pozycja[1] = -100;
        speed = (std::rand() % 300) + 50;
    };
    void updatePozycja(float x, float y) {
        boki[0] = x - 25;
        boki[1] = x + 25;
        boki[2] = y - 41;
        boki[3] = y + 41;
        pozycja[0] = x;
        pozycja[1] = y;
        sprite.setPosition(x, y);
    }
    ~auta() {};
};

class jelen {
public:
    sf::Sprite sprite;
    float boki[4];
    float srodek[2];
    float pozycja[2];
    float gdzie;
    float los;
    int zyje;
    int odlicz;
    int na_ekranie;
    jelen(sf::Texture& textura) {
        sprite.setTexture(textura);
        gdzie = 1;
        zyje = 0;
        na_ekranie = 0;
        odlicz = 720;
        srodek[0] = 6;
        srodek[1] = 68;
        sprite.setOrigin(srodek[0], srodek[1]);
    };
    void updatePozycja(float x, float y) {
        boki[0] = x;
        boki[1] = x + 55;
        boki[2] = y - 62;
        boki[3] = y;
        pozycja[0] = x;
        pozycja[1] = y;
        sprite.setPosition(x, y);
    }
    ~jelen() {};
};

sf::Font utworzCzcionke(std::string sciezka)
{
    sf::Font font;
    font.loadFromFile(sciezka);
    return font;
}

sf::Text utworzText(int font_size, std::string tresc, sf::Font& font, int x, int y)
{
    sf::Text text;
    text.setCharacterSize(font_size);
    text.setFont(font);
    text.setString(tresc);
    text.setFillColor(sf::Color::Red);
    text.setPosition(x, y);
    return text;
}

sf::Sprite utworzSprite(sf::Texture& textura, int x, int y)
{
    sf::Sprite sprite;
    sprite.setTexture(textura);
    sprite.setPosition(x, y);
    return sprite;
}

bool kolizja(float boki1[4], float boki2[4]) {
    if ((boki1[0] <= boki2[1] && boki1[0] >= boki2[0]) || (boki1[1] >= boki2[0] && boki1[1] <= boki2[1])) {
        if ((boki1[2] <= boki2[3] && boki1[2] >= boki2[2]) || (boki1[3] >= boki2[2] && boki1[3] <= boki2[3])) {
            return true;
        }
    }
    return false;
}

int main()
{
    srand(time(NULL));

    //fonts
    sf::Font royal_font = utworzCzcionke("game/font/Royal_Fighter.otf");
    sf::Font nightmare_font = utworzCzcionke("game/font/Nightmare.ttf");
    sf::Font hardboard_font = utworzCzcionke("game/font/Hardboard.otf");


    //text
    sf::Text tytul[2];
    sf::Text opcje[3];
    sf::Text info;

    std::string tytul_text[] = { "jelen", "killer" };
    int tytul_text_pozycja[2][2] = { { 200, 80 }, { 900, 80 } };
    for (int i = 0; i < 2; i++) tytul[i] = utworzText(100, tytul_text[i], royal_font, tytul_text_pozycja[i][0], tytul_text_pozycja[i][1]);

    std::string opcje_text[] = { "I - Jak grac", "U - Ustawienia", "Esc - Wyjscie" };
    for (int i = 0; i < 3; i++) opcje[i] = utworzText(38, opcje_text[i], nightmare_font, 250 + (322 * i), 715);

    std::string info_text = "Ruch myszy - poruszanie pojazdem                                    \n\n";
    info_text += "Spacja - spowolnienie czasu - 1O sec odnowienia                                \n\n";
    info_text += "Esc podczas gry - pauza                                                        \n\n";
    info_text += "Esc podczas pauzy - menu                                                       \n\n\n";

    info_text += "Celem gry jest zdobycie jak najwiekszej ilosci punktow, punkty zdobywa sie za  \n";
    info_text += "jazde: +240 na sekunde oraz za przejezdzanie jeleni: +1000 za jelenia          \n\n";
    info_text += "Gra konczy sie gdy gracz zetknie sie z wrogim pojazdem                         \n\n\n\n\n\n\n\n";

    info_text += "ESC - powrot";

    info = utworzText(32, info_text, hardboard_font, 40, 40);


    //sound & music
    sf::SoundBuffer buffer;
    sf::Music menu_music;
    sf::Music game_music;
    sf::Music koniec_music;
    sf::Sound jelen_glos[2];
    menu_music.openFromFile("game/sound/menu.flac");
    game_music.openFromFile("game/sound/muzyka.wav");
    koniec_music.openFromFile("game/sound/smierc.flac");
    buffer.loadFromFile("game/sound/jelen.wav");

    jelen_glos[0].setBuffer(buffer);
    jelen_glos[1].setBuffer(buffer);

    menu_music.setLoop(true);
    game_music.setLoop(true);


    //textures
    sf::Texture logo_menu;
    sf::Texture blood_menu;
    sf::Texture gracz_tx;
    sf::Texture wrogowie_tx;
    sf::Texture jelenie_tx[3];

    logo_menu.loadFromFile("game/images/menu_logo.png");
    blood_menu.loadFromFile("game/images/menu_blood.png");
    gracz_tx.loadFromFile("game/images/gracz.png");
    wrogowie_tx.loadFromFile("game/images/wrog.png");
    jelenie_tx[0].loadFromFile("game/images/jelenL.png");
    jelenie_tx[1].loadFromFile("game/images/jelenP.png");
    jelenie_tx[2].loadFromFile("game/images/krew.png");

    logo_menu.setSmooth(true);
    blood_menu.setSmooth(true);


    //sprites
    sf::Sprite logo = utworzSprite(logo_menu, 383, 10);
    sf::Sprite blood = utworzSprite(blood_menu, 320, 30);


    //ulica
    float pozycja_x_obiektow[] = { 385, 483, 678, 0, 1008, 678 };
    float pozycja_y_obiektow[] = { 0, 0, 0, 0, 0, 0 };
    float rozmiar_x_obiektow[] = { 650, 400, 10, 358, 358, 10 };
    float rozmiar_y_obiektow[] = { 768, 768, 768, 768, 768, 15 };
    int kolor_r_obiektow[] = { 50, 36, 255, 0, 0, 36 };
    int kolor_g_obiektow[] = { 161, 36, 255, 0, 0, 36 };
    int kolor_b_obiektow[] = { 12, 36, 255, 0, 0, 36 };

    std::vector <obiekt> tlo;
    float pozycja_obiektu[2];
    float rozmiar_obiektu[2];
    int kolor_obiektu[3];

    for (int i = 0; i < 25; i++) {
        if (i < 5) {
            pozycja_obiektu[0] = pozycja_x_obiektow[i];
            pozycja_obiektu[1] = pozycja_y_obiektow[i];
            rozmiar_obiektu[0] = rozmiar_x_obiektow[i];
            rozmiar_obiektu[1] = rozmiar_y_obiektow[i];
            kolor_obiektu[0] = kolor_r_obiektow[i];
            kolor_obiektu[1] = kolor_g_obiektow[i];
            kolor_obiektu[2] = kolor_b_obiektow[i];
        }
        else {
            pozycja_obiektu[0] = pozycja_x_obiektow[5];
            pozycja_obiektu[1] = 48 * (i - 7);
            rozmiar_obiektu[0] = rozmiar_x_obiektow[5];
            rozmiar_obiektu[1] = rozmiar_y_obiektow[5];
            kolor_obiektu[0] = kolor_r_obiektow[5];
            kolor_obiektu[1] = kolor_g_obiektow[5];
            kolor_obiektu[2] = kolor_b_obiektow[5];
        }

        tlo.push_back(obiekt(pozycja_obiektu, rozmiar_obiektu, kolor_obiektu));
    }


    //gracz, wrogowie, jelenie
    int liczba_wrogow = 5;

    float srodek_auta[2] = { 25, 41 };
    auta player = auta(gracz_tx, srodek_auta);

    std::vector <auta> enemies;
    for (int i = 0; i < liczba_wrogow; i++) enemies.push_back(auta(wrogowie_tx, srodek_auta));

    std::vector <float> pozycje_wrogow;
    float pozycja_wrogow;
    for (int i = 0; i < liczba_wrogow; i++) {
        if (i == 0) {
            pozycja_wrogow = (std::rand() % (350 / liczba_wrogow)) + 483 + 25;
            pozycje_wrogow.push_back(pozycja_wrogow);
        }
        else {
            pozycja_wrogow = ((std::rand() % (350 / liczba_wrogow)) + (350 / liczba_wrogow) * i) + 483 + 25;
            pozycje_wrogow.push_back(pozycja_wrogow);
        }
        enemies[i].pozycja[1] = (std::rand() % 100) - 80;
        enemies[i].pozycja[0] = pozycje_wrogow[i];
    }

    jelen jelenie[2] = { jelen(jelenie_tx[0]), jelen(jelenie_tx[1]) };

    float granice[4] = {
        tlo[1].pozycja[0] + player.wsp_srodka[0],
        tlo[1].pozycja[0] + tlo[1].rozmiar[0] - player.wsp_srodka[0],
        tlo[1].pozycja[1] + player.wsp_srodka[1],
        tlo[1].rozmiar[1] - player.wsp_srodka[1]
    };

    float dol_wrogowie = 0;

    sf::RectangleShape fade_out;
    fade_out.setSize(sf::Vector2f(1366, 768));
    fade_out.setFillColor(sf::Color(255, 255, 255, 255));

    int pozycja_ok = 0;
    float droga = 0;
    float droga2 = 0;
    float roznica = 0;

    int fade_out_time = 255;
    int ok = 0;
    int odliczanie = 765;
    int stop = 1;
    int pauza = 1;
    int fps = 2400;
    int fps_spadek = 0;
    int cool_down = 2400;
    int przegrana = 0;


    int jak_grac = 0;
    int ustawienia = 0;

    sf::RenderWindow okno(sf::VideoMode(1366, 768, 32), "Gra jelenie - Artur Pas", sf::Style::Fullscreen);

    okno.setFramerateLimit(240);
    okno.setMouseCursorVisible(false);

    sf::Vector2i myszka = sf::Mouse::getPosition(okno);

    sf::Clock zegar;
    sf::Time tick;

    menu_music.play();

    while (okno.isOpen())
    {
        sf::Event zdarzenie;
        if (okno.pollEvent(zdarzenie))
        {
            if (zdarzenie.type == sf::Event::Closed) okno.close();
            if (zdarzenie.type == sf::Event::KeyPressed) {
                if (zdarzenie.key.code == sf::Keyboard::Escape) {
                    if (pauza == 0) {
                        game_music.pause();
                        pauza = 1;
                    }
                    else if (jak_grac == 1) {
                        jak_grac = 0;
                    }
                    else if (pauza == 1 && jak_grac == 0 && stop == 0) {
                        stop = 1;
                        koniec_music.stop();
                        przegrana = 0;
                        menu_music.play();
                    }
                    else okno.close();
                }
                else if (zdarzenie.key.code == sf::Keyboard::Enter) {
                    if (stop == 1 && jak_grac == 0) {
                        menu_music.pause();
                        stop = 0;
                    }
                    else if (pauza == 1 && jak_grac == 0) {
                        pauza = 0;
                        koniec_music.stop();
                        przegrana = 0;
                        game_music.play();
                    }
                }
                else if (zdarzenie.key.code == sf::Keyboard::Space) {
                    if (cool_down <= 2400) {
                        fps_spadek = 2;
                        cool_down = 5520;
                    }
                }
                else if (zdarzenie.key.code == sf::Keyboard::I) {
                    if (stop == 1) jak_grac = 1;
                }
            }
        }

        if (stop == 1) {
            if (jak_grac == 0) {
                if (ok == 0) {
                    if (fade_out_time > 0) {
                        okno.clear(sf::Color(0, 0, 0, 255));
                        fade_out.setFillColor(sf::Color(255, 255, 255, fade_out_time));
                        fade_out_time -= 0.1;
                        okno.draw(fade_out);
                    }
                    else {
                        fade_out_time = 255;
                        ok = 1;
                    }
                }
                else if (ok == 1) {
                    if (fade_out_time > 5) {
                        okno.clear(sf::Color(0, 0, 0, 255));
                        okno.draw(tytul[0]);
                        okno.draw(tytul[1]);
                        okno.draw(logo);
                        okno.draw(fade_out);
                        fade_out.setFillColor(sf::Color(0, 0, 0, fade_out_time));
                        if (fade_out_time > 190) odliczanie -= 0.05;
                        else odliczanie -= 1;
                        fade_out_time = odliczanie / 3;
                    }
                    else ok = 2;
                }
                else {
                    okno.clear(sf::Color(0, 0, 0, 255));
                    okno.draw(tytul[0]);
                    okno.draw(tytul[1]);
                    okno.draw(logo);
                    okno.draw(blood);
                    for (int i = 0; i < 3; i++) okno.draw(opcje[i]);
                    okno.draw(fade_out);
                }
            }
            else {
                okno.clear(sf::Color(0, 0, 0, 255));
                okno.draw(info);
            }
        }
        else {
            okno.clear(sf::Color::Black);
            //rysowanie tla
            tick = zegar.restart();
            for (int i = 0; i < 25; i++) {
                if (i != 4 && i != 5) {
                    okno.draw(tlo[i].obj);
                    if (i > 4 && pauza == 0) {
                        tlo[i].updatePozycja(2);
                        if (tlo[i].pozycja[1] > 768) tlo[i].updatePozycja(-tlo[1].rozmiar[1] - 96);
                    }
                }
            }

            //spowolnienie czasu
            if (pauza == 0) {
                cool_down--;
                if (cool_down < 0) cool_down = 0;
                if (fps_spadek == 2) {
                    okno.setFramerateLimit(fps / 10);
                    fps -= 20;

                    if (fps < 600) {
                        fps = 600;
                        fps_spadek = 1;
                    }
                }
                else if (fps_spadek == 1) {
                    okno.setFramerateLimit(fps / 10);
                    fps += 5;

                    if (fps > 2400) {
                        fps = 2400;
                        fps_spadek = 0;
                    }
                }

                //granice ulicy
                myszka = sf::Mouse::getPosition(okno);

                if (myszka.x < granice[0]) {
                    myszka.x = granice[0];
                    sf::Mouse::setPosition(sf::Vector2i(granice[0], myszka.y));
                }
                else if (myszka.x > granice[1]) {
                    myszka.x = granice[1];
                    sf::Mouse::setPosition(sf::Vector2i(granice[1], myszka.y));
                }

                if (myszka.y < granice[2]) {
                    myszka.y = granice[2];
                    sf::Mouse::setPosition(sf::Vector2i(myszka.x, granice[2]));
                }
                else if (myszka.y > granice[3]) {
                    myszka.y = granice[3];
                    sf::Mouse::setPosition(sf::Vector2i(myszka.x, granice[3]));
                }

                player.updatePozycja(myszka.x, myszka.y);

                //wrogowie
                dol_wrogowie = tlo[1].rozmiar[1] + enemies[0].wsp_srodka[1];

                for (int i = 0; i < liczba_wrogow; i++) {

                    enemies[i].pozycja[1] += enemies[i].speed / 100;
                    enemies[i].updatePozycja(enemies[i].pozycja[0], enemies[i].pozycja[1]);

                    if (enemies[i].pozycja[1] > dol_wrogowie) {
                        enemies[i].pozycja[1] = ((std::rand() % 100) + 80) * -1;
                        enemies[i].speed = (std::rand() % 300) + 50;
                        droga = enemies[i].speed * 240;

                        pozycje_wrogow[i] = (std::rand() % 350) + 508;

                        pozycja_ok = 0;
                        while (pozycja_ok != liczba_wrogow - 1) {
                            pozycje_wrogow[i] = (std::rand() % 350) + 508;
                            for (int j = 0; j < liczba_wrogow; j++) {
                                if (j != i) {
                                    if (pozycje_wrogow[i] < pozycje_wrogow[j] - 50 || pozycje_wrogow[i] > pozycje_wrogow[j] + 50) {
                                        pozycja_ok++;
                                        continue;
                                    }
                                    else {
                                        if (enemies[i].pozycja[1] > enemies[j].pozycja[1] + 82) {
                                            if (enemies[i].speed > enemies[j].speed) {
                                                pozycja_ok++;
                                                continue;
                                            }
                                        }
                                        else if (enemies[i].pozycja[1] < enemies[j].pozycja[1] - 82) {
                                            droga2 = enemies[j].speed * 240;
                                            roznica = droga - droga2;
                                            if (roznica < 0) roznica = -roznica;

                                            if (enemies[i].pozycja[1] < enemies[j].pozycja[1] - (roznica - 60)) {
                                                pozycja_ok++;
                                                continue;
                                            }
                                            else if (enemies[i].speed < enemies[j].speed) {
                                                pozycja_ok++;
                                                continue;
                                            }
                                        }
                                    }
                                    pozycja_ok = 0;
                                    break;
                                }
                            }
                        }
                        enemies[i].pozycja[0] = pozycje_wrogow[i];
                    }
                }

                for (int i = 0; i < liczba_wrogow; i++) {
                    if (kolizja(player.boki, enemies[i].boki)) {
                        pauza = 1;
                        game_music.pause();
                        przegrana = 1;
                        koniec_music.play();
                    }
                }

                for (int i = 0; i < 2; i++) {
                    if (jelenie[i].na_ekranie == 1) {

                        if (i % 2 == 0) {
                            if (jelenie[i].pozycja[0] <= 100 || jelenie[i].pozycja[1] > 868 || jelenie[i].pozycja[1] < -100) jelenie[i].na_ekranie = 0;
                            else {
                                if (jelenie[i].zyje == 1) jelenie[i].updatePozycja(jelenie[i].pozycja[0] - 1, jelenie[i].pozycja[1] + (jelenie[i].gdzie / 10));
                                else jelenie[i].updatePozycja(jelenie[i].pozycja[0], jelenie[i].pozycja[1] + 2);
                            }
                        }
                        else if (i % 2 == 1) {
                            if (jelenie[i].pozycja[0] > 1466 || jelenie[i].pozycja[1] > 868 || jelenie[i].pozycja[1] < -100) jelenie[i].na_ekranie = 0;
                            else {
                                if (jelenie[i].zyje == 1) jelenie[i].updatePozycja(jelenie[i].pozycja[0] + 1, jelenie[i].pozycja[1] + (jelenie[i].gdzie / 10));
                                else jelenie[i].updatePozycja(jelenie[i].pozycja[0], jelenie[i].pozycja[1] + 2);
                            }
                        }
                    }
                    else {
                        if (jelenie[i].odlicz > 0) jelenie[i].odlicz--;
                        else {
                            if (jelenie[i].zyje == 0) jelenie[i].sprite.setTexture(jelenie_tx[i]);

                            jelenie[i].pozycja[1] = (std::rand() % 500) - 200;

                            if (i % 2 == 0) jelenie[i].pozycja[0] = 1083;
                            else if (i % 2 == 1) jelenie[i].pozycja[0] = 233;

                            jelenie[i].updatePozycja(jelenie[i].pozycja[0], jelenie[i].pozycja[1]);
                            jelenie[i].los = round((std::rand() % 10) - 2);
                            while (jelenie[i].los == 0) jelenie[i].los = round((std::rand() % 10) - 5);
                            jelenie[i].gdzie = ((std::rand() % 5) + 1) * jelenie[i].los;
                            jelenie[i].na_ekranie = 1;
                            jelenie[i].zyje = 1;
                            jelenie[i].odlicz = (std::rand() % 500) + 500;
                        }
                    }
                }

                for (int i = 0; i < 2; i++) {
                    if (jelenie[i].zyje == 1) {
                        if (kolizja(player.boki, jelenie[i].boki)) {
                            jelen_glos[i].play();
                            jelenie[i].sprite.setTexture(jelenie_tx[2]);
                            jelenie[i].zyje = 0;
                        }

                        for (int j = 0; j < liczba_wrogow; j++) {
                            if (kolizja(enemies[j].boki, jelenie[i].boki)) {
                                jelenie[i].sprite.setTexture(jelenie_tx[2]);
                                jelenie[i].zyje = 0;
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < 2; i++) okno.draw(jelenie[i].sprite);
            for (int i = 0; i < liczba_wrogow; i++) okno.draw(enemies[i].sprite);
            okno.draw(player.sprite);
            okno.draw(tlo[3].obj);
            okno.draw(tlo[4].obj);

        }

        okno.display();
    }

    return 0;
}