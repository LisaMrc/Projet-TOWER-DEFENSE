#include <iostream>
#include "App.hpp"
#include "code/draw/draw.hpp"
#include "code/entities/entities.hpp"
#include "code/ui/button.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>

#include "simpletext.h"
#include "utils.hpp"
#include "GLHelpers.hpp"

App::App() : _previousTime(0.0), _viewSize(2.0)
{
    img::Image test {img::load(make_absolute_path("images/level.png", true), 4, true)};

    // LAND TEXTURES
    img::Image grass {img::load(make_absolute_path("images/textures/land/grass.bmp", true), 4, true)};
    img::Image path {img::load(make_absolute_path("images/textures/land/path.bmp", true), 4, true)};
    img::Image in {img::load(make_absolute_path("images/textures/land/in.png", true), 4, true)};
    img::Image out {img::load(make_absolute_path("images/textures/land/out.png", true), 4, true)};

    map._grass = loadTexture(grass);
    map._path = loadTexture(path);
    map._in = loadTexture(in);
    map._out = loadTexture(out);
    
    // ENTITIES TEXTURES
    img::Image king {img::load(make_absolute_path("images/textures/entities/king.png", true), 4, true)};
    img::Image knight {img::load(make_absolute_path("images/textures/entities/knight.png", true), 4, true)};
    img::Image wizard {img::load(make_absolute_path("images/textures/entities/wizard.png", true), 4, true)};
    img::Image arrow_tex {img::load(make_absolute_path("images/textures/buttons/MEOWOLAS_Arrow.png", true), 4, true)};
    img::Image normal_tower {img::load(make_absolute_path("images/textures/entities/tower_1.png", true), 4, true)};
    img::Image elec_tower {img::load(make_absolute_path("images/textures/entities/tower_2.png", true), 4, true)};

    kinger._king = loadTexture(king);
    normal_arrow_tower._arrow = loadTexture(normal_tower);
    elec_arrow_tower._arrow = loadTexture(elec_tower);
    projectile_texture = loadTexture(arrow_tex);
    knight_enemy = loadTexture(knight);
    wizard_enemy = loadTexture(wizard);

    // BUTTONS TEXTURE
    img::Image start {img::load(make_absolute_path("images/textures/buttons/start_button.png", true), 4, true)};
    img::Image quit {img::load(make_absolute_path("images/textures/buttons/quit_button.png", true), 4, true)};
    img::Image pause {img::load(make_absolute_path("images/textures/buttons/pause_button.png", true), 4, true)};
    img::Image resume {img::load(make_absolute_path("images/textures/buttons/resume_button.png", true), 4, true)};
    img::Image retry {img::load(make_absolute_path("images/textures/buttons/retry_button.png", true), 4, true)};
    img::Image title {img::load(make_absolute_path("images/textures/buttons/title.png", true), 4, true)};
    img::Image victory {img::load(make_absolute_path("images/textures/buttons/victory_button.png", true), 4, true)};
    img::Image defeat {img::load(make_absolute_path("images/textures/buttons/defeat_button.png", true), 4, true)};

    start_button = loadTexture(start);
    quit_button = loadTexture(quit);
    pause_bouton = loadTexture(pause);
    title_button = loadTexture(title);
    resume_button = loadTexture(resume);
    retry_button = loadTexture(retry);
    victory_button = loadTexture(victory);
    defeat_button = loadTexture(defeat);
    wood_arrow_button = loadTexture(normal_tower);
    elec_arrow_button = loadTexture(elec_tower);

    // TOWER PLACEMENT TEXTURES
    img::Image free {img::load(make_absolute_path("images/textures/zones_tours/zone_verte.png", true), 4, true)};
    img::Image occupied {img::load(make_absolute_path("images/textures/zones_tours/zone_rouge.png", true), 4, true)};

    map._free = loadTexture(free);
    map._occupied = loadTexture(occupied);
}

void App::setup()
{
    // Set the background color to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Set the text renderer with blending enabled and white text color
    TextRenderer.ResetFont();
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
    TextRenderer.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    TextRenderer.EnableBlending(true);

    // Set player_gold_text
    this->player_gold_text.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
    this->player_gold_text.SetTextSize(SimpleText::FontSize::SIZE_32);
    this->player_gold_text.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    this->player_gold_text.EnableBlending(true);

    // Set buttons list
    listeDeButton.push_back(Button{"Boutton_Start", false, 3, 4, 2, 1, start_button}); //0
    listeDeButton.push_back(Button{"Boutton_Quit", false, 3, 6, 2, 1, quit_button}); //1
    listeDeButton.push_back(Button{"Boutton_Pause", false, 8, 0, 2, 1, pause_bouton}); //2
    listeDeButton.push_back(Button{"Boutton_Titre", false, 3, 1, 6, 2, title_button}); //3
    listeDeButton.push_back(Button{"Boutton_Loose", false, 3, 1, 6, 2, defeat_button}); //4
    listeDeButton.push_back(Button{"Boutton_Win", false, 3, 1, 6, 2, victory_button}); //5
    listeDeButton.push_back(Button{"Boutton_Play_again", false, 3, 4, 2, 1, retry_button}); //6
    listeDeButton.push_back(Button{"Boutton_Play", false, 3, 4, 2, 1, resume_button}); //7
    listeDeButton.push_back(Button{"Boutton_wood_arrow", false, -3, 2, 1, 1, wood_arrow_button}); //8
    listeDeButton.push_back(Button{"Boutton_Elec_Arrow", false, -2, 2, 1, 1, elec_arrow_button}); //9
    listeDeButton.push_back(Button{"Boutton_croix", false, -2, 3, 1, 1, elec_arrow_button}); //10

    // Extract information from itd file
    std::vector<std::vector<std::string>> splitted_itd_file = split_itd_file();

    // Create the tools to print the map
    map.associate_RGB_to_CaseType(splitted_itd_file);
    map.associate_px_pos_to_CaseType();

    // Create graph for enemies from itd
    std::vector<std::vector<float>> adjacency_matrix {create_adjacency_matrix(splitted_itd_file)};
    Graph::WeightedGraph graph {Graph::build_from_adjacency_matrix(adjacency_matrix)};
    
    // Create path for enemies
    std::unordered_map<int, std::pair<float, int>> dij_map = graph.dijkstra(0, 7); 
    std::vector<node> vec_nodes = create_vect_nodes(splitted_itd_file);
    std::vector<int> shortest_path = get_shortest_path (dij_map, vec_nodes);
    std::vector<node> enemy_path = get_enemy_path (vec_nodes, shortest_path);

    // King gets enemy path
    kinger.enemy_path = enemy_path;

    // Set waves in waves list
    waves_list.push_back(Wave {1, {}, 0, 10, 2, 0});
    waves_list.push_back(Wave {2, {}, 12, 5, 3, 1});
    waves_list.push_back(Wave {3, {}, 31, 2, 4, 2});
    waves_list.push_back(Wave {4, {}, 43, 1, 6, 4});

    // Add enemies to each wave
    for (size_t i = 0; i < waves_list.size(); i++)
    {
        for (int j = 0; j < waves_list[i].nbr_knights; j++)
        {
            waves_list[i].enemies_in_wave.push_back(Enemy{j, false, false, 0, 0, 50, 1, 20, 20, EnemyType::KNIGHT, knight_enemy, enemy_path, 0, 1, 0});
        }

        for (int k = waves_list[i].nbr_knights; k < waves_list[i].nbr_knights + waves_list[i].nbr_wizards; k++)
        {
            waves_list[i].enemies_in_wave.push_back(Enemy{k, false, false, 0, 0, 20, 1.2, 40, 40, EnemyType::WIZARD, wizard_enemy, enemy_path, 0, 1, 0});
        }
    }
}

void App::update() {
    const double currentTime {glfwGetTime()};
    const double elapsedTime { currentTime - _previousTime};
    _previousTime = currentTime;

    // if start is pressed
    if (listeDeButton[0].isPressed)
    {
        //réinitizzliser le tableau towers
        normal_towers.clear();
        elec_towers.clear();
        normal_towers_already_builds.clear();
        elec_towers_already_builds.clear();
       
        // Réinitialise le roi (Kinger) 
        kinger.reset();

        // Réinitialise tous les ennemis de toutes les vagues
        for (size_t m = 0; m < waves_list.size(); m++)
        {      
            for (int i = 0; i < waves_list[m].enemies_in_wave.size(); i++)
            {
                waves_list[m].enemies_in_wave[i].reset();
            }
        }
    
        _state = state_screen::screen_LEVEL;

        // Render tower placement
        //draw_quad_with_texture(case_color, xBuild, yBuild, map);

        time_open_window = {glfwGetTime()};
    }

    if (listeDeButton[1].isPressed) // if quit is pressed
    {
        window_close = true;
    }
    
    if (listeDeButton[2].isPressed) // if pause is pressed
    {
        _state = state_screen::screen_PAUSE;
    }

    if (listeDeButton[7].isPressed) // if resume play is pressed
    {
        _state = state_screen::screen_LEVEL;
    }

    if(listeDeButton[6].isPressed) // if play again is pressed
    {
        _state = state_screen::MENU;
    }

    if (_state == state_screen::screen_LEVEL)
    {
        // KING UPDATE
        kinger.ko();

        // ENEMY UPDATE
            for (int m = 0; m < waves_list.size(); m++)
            {
                for (int i = 0; i < waves_list[m].enemies_in_wave.size(); i++)
                {
                    waves_list[m].enemies_in_wave[i].get_elapsedTime(elapsedTime);
                    waves_list[m].enemies_in_wave[i].ko();

                    if (waves_list[m].enemies_in_wave[i].current_node_id == waves_list[m].enemies_in_wave[i].enemy_path.back().node_id)
                    {
                        kinger.health -= waves_list[m].enemies_in_wave[i].damage;
                        std::cout << kinger.health << std::endl;
                    }
                }
            }
        // 

        // TOWERS UPDATE
            for (tower& tower : normal_towers)
            {
                // Détection des ennemis à portée
                for (Wave &w : waves_list)
                {
                    for (Enemy &e : w.enemies_in_wave)
                    {
                        if (e.is_on_stage && isWithinRange(tower, e))
                        {
                            // Vérifie si assez de temps s'est écoulé depuis le dernier tir
                            if (currentTime - tower.lastShotTime >= 1.0 / tower.rate) 
                            {
                                // Création d'un projectile
                                tower.projectiles.push_back(createProjectile(tower, e));
                                tower.lastShotTime = currentTime; // Met à jour le temps du dernier tir
                                break; // Une tourelle ne tire qu'un projectile par mise à jour
                            }
                        }
                    }
                }

                // Mise à jour des projectiles
                for (auto& projectile : tower.projectiles)
                {
                    projectile.update(elapsedTime);
                    if (projectile.hasHitTarget())
                    {
                        projectile.target->takeDamage(projectile.damages);
                        projectile.target->ko();
                        
                        // Incrémenter l'or si l'ennemi est mort
                        if (projectile.target->is_dead)
                        {
                            // player.gold += projectile.target.gold;
                            std::cout << "Bye looser";
                        }
                    }
                }

                // Supprimer les projectiles arrivés à destination
                tower.projectiles.erase(std::remove_if(tower.projectiles.begin(), tower.projectiles.end(), [](const Projectile& projectile) { return projectile.hasHitTarget(); }), tower.projectiles.end());
            }

            // // Supprimer les ennemis morts
            // all_enemies.erase(std::remove_if(all_enemies.begin(), all_enemies.end(), [](const Enemy& enemy) { return enemy.is_dead;}), all_enemies.end());
        // 
    }
    render();
}

void App::render()
{
    // Clear the color and depth buffers of the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity();

    // Render the text          
    TextRenderer.Render();

    if(_state == state_screen::screen_LEVEL)
    {
        // Gets variables
        time_play = glfwGetTime();

        // Change buttons state
        listeDeButton[0].isPressed = false;
        listeDeButton[7].isPressed = false;

        // RENDERS IMMEDIATELY

            // Render pause button
            listeDeButton[2].draw_me();

            // Render types towers button
            listeDeButton[8].draw_me(); //normal tower
            listeDeButton[9].draw_me(); //electrical tower

            // Render the map
            map.draw_map(map);

            // Render the King
            draw_quad_with_texture(kinger._king, kinger.x, kinger.y, map);

            // Render player's gold number
            std::string GOLD_Label{" GOLD : " + std::to_string(kinger.player_gold) + " "};
            this->player_gold_text.Label(GOLD_Label.c_str() , _width / 80, 100, SimpleText::LEFT);
            this->player_gold_text.Render();

            // // Render tower placement
            draw_quad_with_texture(case_color, xBuild, yBuild, map);
        // 

        // TRIGGERS (awaits for...)

            if (kinger.is_dead == 1)
            {
                _state = state_screen::screen_LOOSE;
            }
            else if (time_play > 75 + time_open_window && kinger.health != 0)
            {
                _state = state_screen::screen_WIN;
            }

            if (time_play >= waves_list[0].start)
            {
                for (int i = 0; i < waves_list[0].enemies_in_wave.size(); i++)
                {
                    if (time_play >= waves_list[0].start + waves_list[0].freq_btw_ennemies_in_s*(i))
                    {
                        if (waves_list[0].enemies_in_wave[i].target_node_id < waves_list[0].enemies_in_wave[i].enemy_path.size())
                        {
                            waves_list[0].enemies_in_wave[i].enemy_move();
                        }
                        draw_quad_with_texture(waves_list[0].enemies_in_wave[i].texture, waves_list[0].enemies_in_wave[i].x, waves_list[0].enemies_in_wave[i].y, map);
                        waves_list[0].enemies_in_wave[i].is_on_stage = true;
                    }
                }
            }

            if (time_play >= waves_list[1].start)
            {
                for (int i = 0; i < waves_list[1].enemies_in_wave.size(); i++)
                {
                    if (time_play > waves_list[1].start + waves_list[1].freq_btw_ennemies_in_s*(i+1))
                    {
                        if (waves_list[1].enemies_in_wave[i].target_node_id < waves_list[1].enemies_in_wave[i].enemy_path.size())
                        {
                            waves_list[1].enemies_in_wave[i].enemy_move();
                        }
                        draw_quad_with_texture(waves_list[1].enemies_in_wave[i].texture, waves_list[1].enemies_in_wave[i].x, waves_list[1].enemies_in_wave[i].y, map);
                        waves_list[1].enemies_in_wave[i].is_on_stage = true;
                    }
                }
            }

            if (time_play >= waves_list[2].start)
            {
                for (int i = 0; i < waves_list[2].enemies_in_wave.size(); i++)
                {
                    if (time_play > waves_list[2].start + waves_list[2].freq_btw_ennemies_in_s*(i+1))
                    {
                        if (waves_list[2].enemies_in_wave[i].target_node_id < waves_list[2].enemies_in_wave[i].enemy_path.size())
                        {
                            waves_list[2].enemies_in_wave[i].enemy_move();
                        }
                        draw_quad_with_texture(waves_list[2].enemies_in_wave[i].texture, waves_list[2].enemies_in_wave[i].x, waves_list[2].enemies_in_wave[i].y, map);
                        waves_list[2].enemies_in_wave[i].is_on_stage = true;
                    }
                }
            }

            if (time_play >= waves_list[3].start)
            {
                for (int i = 0; i < waves_list[3].enemies_in_wave.size(); i++)
                {
                    if (time_play > waves_list[3].start + waves_list[3].freq_btw_ennemies_in_s*(i+1))
                    {
                        if (waves_list[3].enemies_in_wave[i].target_node_id < waves_list[3].enemies_in_wave[i].enemy_path.size())
                        {
                            waves_list[3].enemies_in_wave[i].enemy_move();
                        }
                        draw_quad_with_texture(waves_list[3].enemies_in_wave[i].texture, waves_list[3].enemies_in_wave[i].x, waves_list[3].enemies_in_wave[i].y, map);
                        waves_list[3].enemies_in_wave[i].is_on_stage = true;
                    }
                }
            }

            if (listeDeButton[9].isPressed){
                std::cout << "bouton 9 presse" << std::endl;
                //listeDeButton[9].isPressed = false;
                //listeDeButton[8].isPressed = false;
                listeDeButton[10].draw_me();
                for (const auto& tower : elec_towers){
                    create_tower(map, elec_arrow_tower, tower.x, tower.y);
                }
                for (const auto& tower : normal_towers){
                    create_tower(map, normal_arrow_tower, tower.x, tower.y);
                }
            }        
            
            if (listeDeButton[8].isPressed){ 
                std::cout << "bouton 8 presse" << std::endl;
                //listeDeButton[9].isPressed = false;
                listeDeButton[10].draw_me();
                for (const auto& tower : normal_towers){

            if (listeDeButton[8].isPressed)
            {    
                listeDeButton[9].isPressed = false;
                for (const auto& tower : normal_towers)
                {
                    create_tower(map, normal_arrow_tower, tower.x, tower.y);
                }
                for (const auto& tower : elec_towers){
                    create_tower(map, elec_arrow_tower, tower.x, tower.y);
                }
            }

            if(listeDeButton[10].isPressed){
                listeDeButton[9].isPressed = false;
                listeDeButton[8].isPressed = false;
                listeDeButton[10].isPressed = false;
                for (const auto& tower : normal_towers)
                {
                    create_tower(map, elec_arrow_tower, tower.x, tower.y);
                }
                for (const auto& tower : normal_towers){
                    create_tower(map, normal_arrow_tower, tower.x, tower.y);
                }
            }
        // 

        // Render towers
            for (const auto& tower : normal_towers)
            {
                float towerX = tower.x;
                float towerY = tower.y;
                draw_quad_with_texture(normal_arrow_tower._arrow, towerX, towerY, map);
                for (const auto& projectile : tower.projectiles) {
                    float projectileX = projectile.x;
                    float projectileY = projectile.y;
                    draw_quad_with_texture(projectile_texture, projectileX, projectileY, map);
                }
            }
        //
    }

    if (_state == state_screen::MENU)
    {
        listeDeButton[0].isPressed = false;
        listeDeButton[1].isPressed = false;
        listeDeButton[2].isPressed = false;
        listeDeButton[6].isPressed = false;
        listeDeButton[8].isPressed = false;
        listeDeButton[9].isPressed = false;
        listeDeButton[0].draw_me();
        listeDeButton[1].draw_me();
        listeDeButton[3].draw_me();
    }

    if (_state == state_screen::screen_PAUSE)
    {
        listeDeButton[8].isPressed = false;
        listeDeButton[9].isPressed = false;
        listeDeButton[2].isPressed = false;
        listeDeButton[1].draw_me();
        listeDeButton[7].draw_me();
    }

    if (_state == state_screen::screen_LOOSE)
    {
        listeDeButton[4].draw_me();
        listeDeButton[6].draw_me();
        listeDeButton[1].draw_me();
    }

    if (_state == state_screen::screen_WIN)
    {
        listeDeButton[5].draw_me();
        listeDeButton[6].draw_me();
        listeDeButton[1].draw_me();
    }
}

void App::key_callback(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/){}

void App::mouse_button_callback(int button, int action, int mods) {
    if(mouseXpos >= listeDeButton[0].posX && mouseXpos < listeDeButton[0].posX+listeDeButton[0].width && 
    mouseYpos >= listeDeButton[0].posY && mouseYpos < listeDeButton[0].posY + listeDeButton[0].height && _state == state_screen::MENU){
        listeDeButton[0].isPressed = true;
    }
    if(mouseXpos >= listeDeButton[1].posX && mouseXpos < listeDeButton[1].posX+listeDeButton[1].width && 
    mouseYpos >= listeDeButton[1].posY && mouseYpos < listeDeButton[1].posY + listeDeButton[1].height && (_state == state_screen::MENU||_state == state_screen::screen_PAUSE||_state == state_screen::screen_LOOSE||_state == state_screen::screen_WIN)){
        listeDeButton[1].isPressed = true;
    }
    if(mouseXpos >= listeDeButton[2].posX && mouseXpos < listeDeButton[2].posX+listeDeButton[2].width && 
    mouseYpos >= listeDeButton[2].posY && mouseYpos < listeDeButton[2].posY + listeDeButton[2].height && _state == state_screen::screen_LEVEL){
        listeDeButton[2].isPressed = true;
    }
    if(mouseXpos >= listeDeButton[6].posX && mouseXpos < listeDeButton[6].posX+listeDeButton[6].width && 
    mouseYpos >= listeDeButton[6].posY && mouseYpos < listeDeButton[6].posY + listeDeButton[6].height && (_state == state_screen::screen_LOOSE||_state == state_screen::screen_WIN)){
        listeDeButton[6].isPressed = true;
    }
    if(mouseXpos >= listeDeButton[7].posX && mouseXpos < listeDeButton[7].posX+listeDeButton[7].width && 
    mouseYpos >= listeDeButton[7].posY && mouseYpos < listeDeButton[7].posY + listeDeButton[7].height && (_state == state_screen::screen_PAUSE)){
        listeDeButton[7].isPressed = true;
    }
    if(mouseXpos >= listeDeButton[8].posX+1 && mouseXpos < listeDeButton[8].posX+listeDeButton[8].width+1 && 
    mouseYpos >= listeDeButton[8].posY && mouseYpos < listeDeButton[8].posY + listeDeButton[8].height && _state == state_screen::screen_LEVEL){
        listeDeButton[8].isPressed = true;
    }
    if(mouseXpos >= listeDeButton[9].posX+1 && mouseXpos < listeDeButton[9].posX+listeDeButton[9].width+1 && 
    mouseYpos >= listeDeButton[9].posY && mouseYpos < listeDeButton[9].posY + listeDeButton[9].height && _state == state_screen::screen_LEVEL){
        listeDeButton[9].isPressed = true;
    }
    if(mouseXpos >= listeDeButton[10].posX+1 && mouseXpos < listeDeButton[10].posX+listeDeButton[10].width+1 && 
    mouseYpos >= listeDeButton[10].posY && mouseYpos < listeDeButton[10].posY + listeDeButton[10].height && _state == state_screen::screen_LEVEL){
        listeDeButton[10].isPressed = true;
    }

}

void App::scroll_callback(double /*xoffset*/, double /*yoffset*/){}

void App::cursor_position_callback(double /*xpos*/, double /*ypos*/){}

void App::size_callback(int width, int height)
{
    _width  = width;
    _height = height;

    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, _width, _height);

    const float aspectRatio { _width / (float) _height };

    // Change the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(aspectRatio > 1.0f) {
        glOrtho(-_viewSize / 2.0f * aspectRatio, _viewSize / 2.0f * aspectRatio, -_viewSize / 2.0f, _viewSize / 2.0f, -1.0f, 1.0f);
    } else {
        glOrtho(-_viewSize / 2.0f, _viewSize / 2.0f, -_viewSize / 2.0f / aspectRatio, _viewSize / 2.0f / aspectRatio, -1.0f, 1.0f);
    }
}

bool App::isWithinRange(const tower& tour, const Enemy& enemy)
{
    float dx = tour.x - enemy.x;
    float dy = tour.y - enemy.y;
    float distance = sqrt(dx * dy + dy * dy);
    return distance <= tour.range;
}

Projectile App::createProjectile(const tower& tour, Enemy& enemy)
{
    Projectile projectile;
    projectile.x = tour.x;
    projectile.y = tour.y;
    projectile.target = &enemy;
    projectile.speed = 10.0f; // Vitesse du projectile
    projectile.damages = 10; // Dégâts infligés par le projectile
    return projectile;
}