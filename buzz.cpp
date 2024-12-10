#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <string>

using namespace std;
using namespace sf;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.

const int resolutionX = 960;
const int resolutionY = 720;
const int boxPixelsX = 48;
const int boxPixelsY = 48;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite);
void moveBullet(int array[][3], Clock& bulletClock, float player_x, float player_y);
void drawBullet(RenderWindow& window, int array[][3], Sprite& bulletSprite);
void movePlayer(float& player_x, float& player_y, float& player_vel, int flower_map[][3]);
void drawCansLeft(RenderWindow& window, Sprite canSprite, int& spray_cans);
void fireBullet(bool& bullet_exists, int& sprays, int& spray_cans, int& lvl_sprays, int& current_can_lvl, Clock& clock, int array[][3], float player_x, float player_y);
void generate_map(int map[][20][6], int& max_reg_bees, int& max_honeycombs, int& max_fast_bees, Clock& gen_clock, int fast_map[][20][5], int& max_hives);
void draw_maps(int map[][20][6], Sprite& reg_bee_sprite, Sprite& reg_honeycomb_sprite, Sprite& fast_bee_sprite, RenderWindow& window, int fast_map[][20][5], Sprite& fast_honeycomb_sprite, Sprite& hive_sprite);
void update_map(int map[][20][6], Clock& clock, Clock& pause_clock, Clock& fast_clock, int fast_map[][20][5], int flower_map[][3]);
void animate_sprites(Clock& clock, IntRect& reg_bee_rect, IntRect& fast_bee_rect, IntRect& bird_sprite_rect, Sprite& reg_bee_sprite, Sprite& fast_bee_sprite, Sprite& bird_sprite);
void check_collision(int map[][20][6], Music& hit_music, int bullet_array[][3], int fast_map[][20][5], int& score, int power_ups[][4], int& level);
void init_humming_bird(int humming_bird[]);
void draw_bird(int humming_bird[], Sprite& bird_sprite, RenderWindow& window);
void update_bird_target(int humming_bird[], Clock& clock, int map[][20][6], int& target_x, int& target_y);
void update_bird(int humming_bird[], int map[][20][6], int& target_x, int& target_y, int& score, int power_ups[][4], int& level);
void check_flower(int flower_map[][3], int map[][20][6]);
void draw_flowers(int flower_map[][3], RenderWindow& window, Sprite& obstacle_sprite);
bool flower_collision(int flower_map[][3], float& player_x, float& player_y);
void level_updater(int& max_reg_bees, int& max_fast_bees, int& max_honeycombs, int& level, int map[][20][6], Clock& clock, int fast_map[][20][5], int& sprays, int& spray_cans, int& lvl_sprays, int& current_can_lvl, bool reset, int& max_hives);
bool next_level(int map[][20][6], int fast_map[][20][5], int& score, int& level, Font& font, RenderWindow& window, bool& in_menu, int& max_reg_bees, int& max_fast_bees, int& max_honeycombs, int& selected_opt);
void reset_lvl(int map[][20][6], int fast_map[][20][5], int& sprays, int& spray_cans, int& lvl_sprays, int& current_can_lvl, int& max_reg_bees, int& max_fast_bees, int& max_honeycombs, int& level, Clock& clock, int flower_map[][3], int& score, RenderWindow& window, Font& font, bool& in_menu, int& selected_opt, int& max_hives);
void bird_collision(int humming_bird[], int bullet_array[][3], Clock& reset_clock);
void award_can(int score, int& spray_cans);
void display_scores(RenderWindow& window, Font font);
void feed_score(int& score, RenderWindow& window, Font font, bool& in_menu, int& level, int& max_reg_bees, int& max_fast_bees, int& max_honeycombs, int& selected_opt);
void update_power_ups(int power_ups[][4]);
void draw_power_ups(RenderWindow& window, Sprite& height_inc_sprite, Sprite& height_dec_sprite, Sprite& speed_inc_sprite, Sprite& speed_dec_sprite, int power_ups[][4]);
void update_active_power_ups(RenderWindow& window, int active_power_ups[][2], float& player_vel, Sprite& player_sprite, float& player_y);
void power_ups_collision(int power_ups[][4], float& player_x, float& player_y, int active_power_ups[][2]);
void draw_icons(RenderWindow& window, Sprite& height_inc_sprite, Sprite& height_dec_sprite, Sprite& speed_inc_sprite, Sprite& speed_dec_sprite, int active_power_ups[][2]);
int** init_infant_bees(int map[][20][6], int& bee_count);
void animate_infant_bee(Sprite& infant_bee_sprite, IntRect& infant_bee_rect, Clock& clock);
void draw_infant_bees(RenderWindow& window, int** infant_bees, Sprite& infant_bee_sprite, int& hive_count);
void update_infant_bees(int** infant_bees, int fast_map[][20][5], int map[][20][6], int count, Clock& clock);
void remove_infant_bees(int** infant_bees, int& count);
void infant_bees_collision(int bullet_array[][3], int** infant_bees, int& count, Music& music, int& score);

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Write your functions declarations here. Some have been written for you. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


int main()
{
	srand(time(0));

	// Declaring RenderWindow.
	RenderWindow window(VideoMode(resolutionX, resolutionY), "Buzz Bombers", Style::Close | Style::Titlebar);

	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(Vector2i(500, 200));
	window.setFramerateLimit(30);

	//most imp variables
	int score = 0;
	int level = 1;
	int infant_bee_count = 0;

	// Initializing Background Music.
	Music lvl1_music;
	lvl1_music.openFromFile("Music/Forest.ogg");
	lvl1_music.setLoop(true);
	lvl1_music.setVolume(10);

	Music title_music;
	title_music.openFromFile("Music/Title.ogg");
	title_music.setLoop(true);
	title_music.setVolume(10);
	title_music.play();

	Music lvl2_music;
	lvl2_music.openFromFile("Music/lvl2.ogg");
	lvl2_music.setLoop(true);
	lvl2_music.setVolume(10);
	bool lvl2_play = false;

	Music lvl3_music;
	lvl3_music.openFromFile("Music/lvl3.ogg");
	lvl3_music.setLoop(true);
	lvl3_music.setVolume(10);
	bool lvl3_play = false;

	Music boss_music;
	boss_music.openFromFile("Music/risk.ogg");
	boss_music.setLoop(true);
	boss_music.setVolume(10);
	bool boss_play = false;

	Music hit_music;
	hit_music.openFromFile("Sound Effects/hit.wav");
	hit_music.setVolume(100);

	// Initializing Player and Player Sprites.
	float player_x = resolutionX / 2 - 32;
	float player_y = resolutionY - 112;
	float player_vel = 10;
	bool moved_down = false;

	Texture playerTextureArray[7];
	Sprite playerSprite;
	Sprite canSprite;
	for(int i = 0; i < 7; i++) {
		playerTextureArray[i].loadFromFile("Textures/spray_" + to_string(i + 1) + ".png");
	}
	playerSprite.setTexture(playerTextureArray[0]);
	canSprite.setTexture(playerTextureArray[0]);
	//	playerSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

		// Initializing Bullet and Bullet Sprites
		// Data for bullet / Spray pellet

	float bullet_x = player_x + 48;
	float bullet_y = player_y;
	bool bullet_exists = false;

    //bool to check if infant bees are summoned
	bool infant_bees_summoned = false;

    //bullet initializations
	Clock bulletClock;
	Texture bulletTexture;
	Sprite bulletSprite;
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	int bullet_array[10][3] = {};
	for(int i = 0; i < 10; i++) {
		bullet_array[i][0] = bullet_x;  //x_pos
		bullet_array[i][1] = bullet_y;  // y_pos
		bullet_array[i][2] = 0;  //inactive/ active
	}

	// ammunition
	int spray_cans = 3;
	int sprays = 56;
	int lvl_sprays = 0;
	int current_can_lvl = 1;
	
	//initial values for levels
	int max_reg_bees = 20;
	int max_honeycombs = 3;
	int max_fast_bees = 0;
	int max_hives = 0;

    // clocks for different events
	Clock map_clock;
	Clock gen_clock;
	Clock pause_clock;
	Clock fire_clock;
	Clock fast_clock;
	Clock bird_clock;
	Clock lvl_clock;
	Clock reset_clock;
	Clock boss_clock;
	Clock inf_anim_clock;
	Clock infant_move_clock;
	
	//maps I am using to store everything
	int reg_map[gameColumns][gameRows][6] = {};
	int fast_map[gameColumns][gameRows][5] = {};
	int flower_map[gameRows][3] = {};
	int power_ups[10][4];
	int active_power_ups[2][2];
	int** infant_bees = nullptr;

	int humming_bird[5] = {};
	int target_x = -80, target_y = resolutionY-128; // initial pos of humming bird
	init_humming_bird(humming_bird);
	
	//generate_map(reg_map, max_reg_bees, max_honeycombs, max_fast_bees, gen_clock, fast_map);

	//loaing in textures and animating some as well
	Texture bird_texture;
	bird_texture.loadFromFile("Textures/bird_sheet.png");
	IntRect bird_sprite_rect(0, 0, 48, 48);
	Sprite bird_sprite(bird_texture, bird_sprite_rect);
	
	Texture reg_bee_texture;
	reg_bee_texture.loadFromFile("Textures/reg_bee_sheet.png");
	IntRect reg_bee_sprite_rect(0, 0, 48, 48);
	Sprite reg_bee_sprite(reg_bee_texture, reg_bee_sprite_rect);

	Clock animation_clock;

	Texture fast_bee_texture;
	fast_bee_texture.loadFromFile("Textures/fast_bee_sheet.png");
	IntRect fast_bee_sprite_rect(0, 0, 48, 48);
	Sprite fast_bee_sprite(fast_bee_texture, fast_bee_sprite_rect);

	Texture reg_honeycomb_texture;
	reg_honeycomb_texture.loadFromFile("Textures/reg_honeycomb.png");
	Sprite reg_honeycomb_sprite;
	reg_honeycomb_sprite.setTexture(reg_honeycomb_texture);

	Texture fast_honeycomb_texture;
	fast_honeycomb_texture.loadFromFile("Textures/fast_honeycomb.png");
	Sprite fast_honeycomb_sprite;
	fast_honeycomb_sprite.setTexture(fast_honeycomb_texture);

	Texture obstacle_texture;
	obstacle_texture.loadFromFile("Textures/obstacles.png");
	Sprite obstacle_sprite;
	obstacle_sprite.setTexture(obstacle_texture);

	Texture hive_texture;
	hive_texture.loadFromFile("Textures/hive.png");
	Sprite hive_sprite;
	hive_sprite.setTexture(hive_texture);

	Texture height_dec_texture;
	height_dec_texture.loadFromFile("Textures/Height_Dec.png");
	Sprite height_dec_sprite;
	height_dec_sprite.setTexture(height_dec_texture);
	
	Texture height_inc_texture;
	height_inc_texture.loadFromFile("Textures/Height_Inc.png");
	Sprite height_inc_sprite;
	height_inc_sprite.setTexture(height_inc_texture);

	Texture speed_dec_texture;
	speed_dec_texture.loadFromFile("Textures/Speed_Dec.png");
	Sprite speed_dec_sprite;
	speed_dec_sprite.setTexture(speed_dec_texture);

	Texture speed_inc_texture;
	speed_inc_texture.loadFromFile("Textures/Speed_Inc.png");
	Sprite speed_inc_sprite;
	speed_inc_sprite.setTexture(speed_inc_texture);

	Texture infant_bee_texture;
	infant_bee_texture.loadFromFile("Textures/infant_bee_sheet.png");
	IntRect infant_bee_sprite_rect(0, 0, 32, 32);
	Sprite infant_bee_sprite(infant_bee_texture, infant_bee_sprite_rect);

	// The ground on which player moves

	RectangleShape groundRectangle(Vector2f(960, 48));
	groundRectangle.setPosition(0, resolutionY-48);
	groundRectangle.setFillColor(Color::Green);

	//Text
	Font font;
	font.loadFromFile("Fonts/retro-pixel.regular.otf");

	Text score_text;
	score_text.setFont(font);
	score_text.setCharacterSize(30);
	score_text.setFillColor(Color::Black);
	score_text.setPosition(resolutionX-120, resolutionY-48);
	
	Text menu_title("Buzz Bombers", font, 75);
	menu_title.setFillColor(Color::Yellow);
	menu_title.setPosition(resolutionX/2 - menu_title.getGlobalBounds().width/2, 100);

	Text start_opt("Start Game", font, 50);
	start_opt.setFillColor(Color::Red);
	start_opt.setPosition(resolutionX/2 - start_opt.getGlobalBounds().width/2, 200);

	Text boss_lvl_text("Boss Level", font, 50);
	boss_lvl_text.setFillColor(Color::Red);
	boss_lvl_text.setPosition(resolutionX/2 - boss_lvl_text.getGlobalBounds().width/2, 250);

	Text high_score_text("High Scores", font, 50);
	high_score_text.setFillColor(Color::Red);
	high_score_text.setPosition(resolutionX/2 - high_score_text.getGlobalBounds().width/2, 300);

	Text quit_opt("Quit Option", font, 50);
	quit_opt.setFillColor(Color::Red);
	quit_opt.setPosition(resolutionX/2 - start_opt.getGlobalBounds().width/2, 350);

	int selected_opt = 0;  //for main menu

	bool in_menu = true;   //for main menu

	while (window.isOpen()) {
		if(in_menu) {
			while(in_menu && window.isOpen()) {
				Event e;
				while(window.pollEvent(e)) {
					if(e.type == Event::Closed) {
						window.close();
					}

					if (e.type == Event::KeyPressed || selected_opt == 5) {
						if (e.key.code == Keyboard::Up || e.key.code == Keyboard::W) {
							selected_opt -= 1;
							if (selected_opt < 0) selected_opt = 3; // Loop back to the last option
						} 
						else if (e.key.code == Keyboard::Down || e.key.code == Keyboard::S) {
							selected_opt += 1;
							if (selected_opt > 3) selected_opt = 0; // Loop back to the first option
						}

						// Reset all options' colors
						start_opt.setFillColor(Color::Red);
						boss_lvl_text.setFillColor(Color::Red);
						high_score_text.setFillColor(Color::Red);
						quit_opt.setFillColor(Color::Red);

						// Highlight the selected option
						switch (selected_opt) {
							case 0: start_opt.setFillColor(Color::Green); break;
							case 1: boss_lvl_text.setFillColor(Color::Green); break;
							case 2: high_score_text.setFillColor(Color::Green); break;
							case 3: quit_opt.setFillColor(Color::Green); break;
						}

						// Handle selection with Enter key
						if (e.key.code == Keyboard::Enter || selected_opt == 5) {
							if(selected_opt == 5) {
								selected_opt = 2;
							}
							switch (selected_opt) {
								case 0: in_menu = false; title_music.stop(); lvl1_music.play(); break;       // Start game
								case 1: 
									max_reg_bees = 20;
									max_fast_bees = 15;
									max_honeycombs = 15;
									max_hives = 5;
									in_menu = false;
									level = 4;
									title_music.stop();
									boss_music.play();
									boss_clock.restart();
									break;
								case 2: display_scores(window, font); break;
								case 3: window.close(); return 0;    // Quit game
							}
						}
					}
				}

				lvl2_play = (lvl2_play)? false : true;
				lvl3_play = (lvl3_play)? false : true;
				boss_play = (boss_play)? false : true;

				window.clear(Color::Black);
				window.draw(menu_title);
				window.draw(start_opt);
				window.draw(boss_lvl_text);
				window.draw(high_score_text);
				window.draw(quit_opt);
				window.display();
			}
		}

		else {
			while (!in_menu && window.isOpen()) {
			
				Event e;
				while (window.pollEvent(e)) {
					if (e.type == Event::Closed) {
						return 0;
					}
				}

				//update music for every level
				if(level == 2 && !lvl2_play) {
					lvl1_music.stop();
					lvl2_music.play();
					lvl2_play = true;
					// cout<<"Yess";
				}
				else if(level == 3 && !lvl3_play) {
					lvl2_music.stop();
					lvl3_music.play();
					lvl3_play = true;
					// cout<<"yess";
				}
				else if(level == 4 && !boss_play) {
					lvl3_music.stop();
					boss_music.play();
					boss_clock.restart();
					boss_play = true;
				}

				///////////////////////////////////////////////////////////////
				//                                                           //
				// Call Your Functions Here. Some have been written for you. //
				// Be vary of the order you call them, SFML draws in order.  //
				//                                                           //
				///////////////////////////////////////////////////////////////
				//cout<<max_reg_bees<<"\t"<<max_fast_bees<<endl;

				// after 60secs infant bees will appear
				if(boss_clock.getElapsedTime().asSeconds() > 60 && level == 4 && !infant_bees_summoned) {
					infant_bees = init_infant_bees(reg_map, infant_bee_count);
					infant_bees_summoned = true;

					// for(int i = 0; i < gameColumns; i++) {
					// 	for(int j = 0; j < gameRows; j++) {
					// 		if(reg_map[i][j][2] == 5) {
					// 			hive_count++;

					// 			reg_map[i][j][0] = 0;
					// 			reg_map[i][j][1] = 0;
					// 			reg_map[i][j][2] = 0;
					// 			reg_map[i][j][3] = 0;
					// 			reg_map[i][j][4] = 0;
					// 			reg_map[i][j][5] = 0;
					// 		}
					// 	}
					// }
					if(infant_bees == nullptr)
						cout<<infant_bees<<endl;
					cout<<infant_bee_count<<endl;
				}
				
				if(max_reg_bees != 0 || max_fast_bees != 0) {
					generate_map(reg_map, max_reg_bees, max_honeycombs, max_fast_bees, gen_clock, fast_map, max_hives);
				}

				movePlayer(player_x, player_y, player_vel, flower_map);

				//moves objects on the map
				update_map(reg_map, map_clock, pause_clock, fast_clock, fast_map, flower_map);

				//gameover cond
				check_flower(flower_map, reg_map);
				
				playerSprite.setTexture(playerTextureArray[current_can_lvl - 1]);

				if(next_level(reg_map, fast_map, score, level, font, window, in_menu, max_reg_bees, max_fast_bees, max_honeycombs, selected_opt)) {
					level_updater(max_reg_bees, max_fast_bees, max_honeycombs, level, reg_map, gen_clock, fast_map, sprays, spray_cans, lvl_sprays, current_can_lvl, false, max_hives);
				}

				if(spray_cans <= 0) {
					exit(0);
				}

				reset_lvl(reg_map, fast_map, sprays, spray_cans, lvl_sprays, current_can_lvl, max_reg_bees, max_fast_bees, max_honeycombs, level, gen_clock, flower_map, score, window, font, in_menu, selected_opt, max_hives);

				//Bullet funcs
				moveBullet(bullet_array, bulletClock, player_x, player_y);
				check_collision(reg_map, hit_music, bullet_array, fast_map, score, power_ups, level);
				drawBullet(window, bullet_array, bulletSprite);
				fireBullet(bullet_exists, sprays, spray_cans, lvl_sprays, current_can_lvl, fire_clock, bullet_array, player_x, player_y);

				//bird funcs
				update_bird_target(humming_bird, bird_clock, reg_map, target_x, target_y);
				update_bird(humming_bird, reg_map, target_x, target_y, score, power_ups, level);
				bird_collision(humming_bird, bullet_array, reset_clock);

				//power ups
				update_power_ups(power_ups);
				power_ups_collision(power_ups, player_x, player_y, active_power_ups);


				//all the animation and drawing
				drawPlayer(window, player_x, player_y, playerSprite);
				window.draw(groundRectangle);
				animate_sprites(animation_clock, reg_bee_sprite_rect, fast_bee_sprite_rect, bird_sprite_rect, reg_bee_sprite, fast_bee_sprite, bird_sprite);
				animate_infant_bee(infant_bee_sprite, infant_bee_sprite_rect, inf_anim_clock);
				draw_maps(reg_map, reg_bee_sprite, reg_honeycomb_sprite, fast_bee_sprite, window, fast_map, fast_honeycomb_sprite, hive_sprite);
				drawCansLeft(window, canSprite, spray_cans);
				award_can(score, spray_cans);
				draw_flowers(flower_map, window, obstacle_sprite);
				draw_bird(humming_bird, bird_sprite, window);
				draw_power_ups(window, height_inc_sprite, height_dec_sprite, speed_inc_sprite, speed_dec_sprite, power_ups);
				update_active_power_ups(window, active_power_ups, player_vel, playerSprite, player_y);
				draw_icons(window, height_inc_sprite, height_dec_sprite, speed_inc_sprite, speed_dec_sprite, active_power_ups);

				//updates and drawing of infant bees
				if(level == 4 && infant_bees_summoned && infant_bees != nullptr && infant_bee_count > 0) {
					draw_infant_bees(window, infant_bees, infant_bee_sprite, infant_bee_count);
					update_infant_bees(infant_bees, fast_map, reg_map, infant_bee_count, infant_move_clock);
					infant_bees_collision(bullet_array, infant_bees, infant_bee_count, hit_music, score);
					remove_infant_bees(infant_bees, infant_bee_count);
				}

				score_text.setString(to_string(score));
				window.draw(score_text);

				window.display();
				if(level == 4) {
					window.clear(Color(75, 75, 75, 255));
					groundRectangle.setFillColor(Color(1, 75, 50, 255));
				}

				else {
					window.clear(Color(150, 220, 255, 255));
					groundRectangle.setFillColor(Color::Green);
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite) {
	playerSprite.setPosition(player_x, player_y);
	window.draw(playerSprite);
}
void moveBullet(int array[][3], Clock& bulletClock, float player_x, float player_y) {
	if (bulletClock.getElapsedTime().asMilliseconds() < 20)
		return;

	bulletClock.restart();
	for(int i = 0; i < 1; i++) {
		if(array[i][2] == 1) {
			array[i][1] -= 20;
			if(array[i][1] < -32) {
				array[i][2] = 0;
				array[i][0] = player_x + 16;
				array[i][1] = player_y;
			}
		}
	}
}
void drawBullet(sf::RenderWindow& window, int array[][3], Sprite& bulletSprite) {
	for(int i = 0; i < 1; i++) {
		if(array[i][2] == 1) {
			bulletSprite.setPosition(array[i][0], array[i][1]);
			window.draw(bulletSprite);
		}
	}
}

void movePlayer(float& player_x, float& player_y, float& player_vel, int flower_map[][3]) {
    bool isStuck = false;  // Variable to check if the player is stuck

    // Move left
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player_x >= 0) {
        // Check if there is no collision on the left
        float x = player_x - player_vel;
        if (!flower_collision(flower_map, x, player_y)) {
            player_x -= player_vel;  // Move player left
        } else {
            isStuck = true;  // Player is stuck, can't move left
        }
    }

    // Move right
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player_x < resolutionX - 64) {
        // Check if there is no collision on the right
        float x = player_x + player_vel;
        if (!flower_collision(flower_map, x, player_y)) {
            player_x += player_vel;  // Move player right
        } else {
            isStuck = true;  // Player is stuck, can't move right
        }
    }

    // If the player is stuck, try to "push" them out slightly in the opposite direction
    if (isStuck) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player_x += 5;  // Try pushing the player right if stuck on the left
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player_x -= 5;  // Try pushing the player left if stuck on the right
        }
    }
}


void drawCansLeft(RenderWindow& window, Sprite canSprite, int& spray_cans) {
	for(int i = 1; i < spray_cans; i++) {
		canSprite.setScale(0.75f, 0.75f);
		canSprite.setPosition((i-1) * 48 + 2, resolutionY - 50);
		window.draw(canSprite);
	}
}

void fireBullet(bool& bullet_exists, int& sprays, int& spray_cans, int& lvl_sprays, int& current_can_lvl, Clock& clock, int array[][3], float player_x, float player_y) {
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && clock.getElapsedTime().asSeconds() > 0.25f) {
		for(int i = 0; i < 1; i++) {
			if(array[i][2] == 0) {
				array[i][2] = 1;
				array[i][0] = player_x + 16;
				array[i][1] = player_y;
				sprays--;
				lvl_sprays++;
				clock.restart();
				if(lvl_sprays == 8) {
					lvl_sprays = 0;
					current_can_lvl++;
				}
				break;
			}
		}
	}
	if(sprays <= 0) {
		sprays = 56;
		spray_cans--;
		lvl_sprays = 0;
		current_can_lvl = 1;
	}
}

void generate_map(int map[][20][6], int& max_reg_bees, int& max_honeycombs, int& max_fast_bees, Clock& gen_clock, int fast_map[][20][5], int& max_hives) {
	if(gen_clock.getElapsedTime().asSeconds() >= 2) {
		gen_clock.restart();
		if(max_reg_bees != 0) {
			int x_pos = 9;
			int y_pos = 0;
			int dir = rand() % 2 + 1;
			int type = 1;
			int pol = 0;
			int loop = 0;
			if(map[y_pos][x_pos][2] == 0) {
				map[y_pos][x_pos][0] = x_pos;
				map[y_pos][x_pos][1] = y_pos;
				map[y_pos][x_pos][2] = type;
				map[y_pos][x_pos][3] = dir;
				map[y_pos][x_pos][4] = pol;
				map[y_pos][x_pos][5] = loop;
				max_reg_bees--;
			}
		}

		if(max_fast_bees != 0) {
			int x_pos = rand() % 20;
			int y_pos = 0;
			int dir = rand() % 2 + 1;
			int type = 2;
			int pol = 0;
			if(fast_map[y_pos][x_pos][2] == 0 && map[y_pos][x_pos][2] == 0) {
				fast_map[y_pos][x_pos][0] = x_pos;
				fast_map[y_pos][x_pos][1] = y_pos;
				fast_map[y_pos][x_pos][2] = type;
				fast_map[y_pos][x_pos][3] = dir;
				fast_map[y_pos][x_pos][4] = pol;
				max_fast_bees--;
			}
		}
	}
	int i;
	for(i = 0; i < max_honeycombs; ) {
		int x_pos = rand() % 20;
		int y_pos = rand() % 10 + 1;
		int dir = 0;
		int type = 3;
		if(map[y_pos][x_pos][2] == 0 && map[y_pos-1][x_pos][2] == 0) {	
			map[y_pos][x_pos][0] = x_pos;
			map[y_pos][x_pos][1] = y_pos;
			map[y_pos][x_pos][2] = type;
			map[y_pos][x_pos][3] = dir;
			i++;
			//cout<<"placed at: "<<x_pos<<"\t"<<y_pos<<endl;
		}
	}

	for(int j = 0; j < max_hives; ) {
		int x_pos = rand() % 20;
		int y_pos = rand() % 9 + 2;
		int dir = 0;
		int type = 5;
		if(map[y_pos][x_pos][2] == 0) {
			map[y_pos][x_pos][0] = x_pos;
			map[y_pos][x_pos][1] = y_pos;
			map[y_pos][x_pos][2] = type;
			map[y_pos][x_pos][3] = dir;
			j++;
		}
	}
	max_hives = 0;
	// if (i < max_honeycombs) {
    // 	cout << "Warning: Unable to place all honeycombs. Placed: " << i << "\n";
	// }

	max_honeycombs = 0;
}

void draw_maps(int map[][20][6], Sprite& reg_bee_sprite, Sprite& reg_honeycomb_sprite, Sprite& fast_bee_sprite, RenderWindow& window, int fast_map[][20][5], Sprite& fast_honeycomb_sprite, Sprite& hive_sprite) {
	for(int i = 0; i < gameColumns; i++) {
		for(int j = 0; j < gameRows; j++) {
			if(map[i][j][2] == 1) {
				reg_bee_sprite.setPosition(map[i][j][0] * 48, map[i][j][1] * 48);
				if(map[i][j][3] == 1) {
					reg_bee_sprite.setScale(1.0f, 1.0f);
					reg_bee_sprite.setOrigin(0, 0);
				}
				else if(map[i][j][3] == 2) {
					reg_bee_sprite.setScale(-1.0f, 1.0f);
					reg_bee_sprite.setOrigin(48, 0);
				}
				window.draw(reg_bee_sprite);
			}
			if(fast_map[i][j][2] == 2) {
				fast_bee_sprite.setPosition(fast_map[i][j][0] * 48, fast_map[i][j][1] * 48);
				if(fast_map[i][j][3] == 1) {
					fast_bee_sprite.setScale(1.0f, 1.0f);
					fast_bee_sprite.setOrigin(0, 0);
				}
				else if(fast_map[i][j][3] == 2) {
					fast_bee_sprite.setScale(-1.0f, 1.0f);
					fast_bee_sprite.setOrigin(48, 0);
				}
				window.draw(fast_bee_sprite);
			}
			if(map[i][j][2] == 3) {
				reg_honeycomb_sprite.setPosition(map[i][j][0] * 48, map[i][j][1] * 48);
				window.draw(reg_honeycomb_sprite);
			}
			if(map[i][j][2] == 4) {
				fast_honeycomb_sprite.setPosition(map[i][j][0] * 48, map[i][j][1] * 48);
				window.draw(fast_honeycomb_sprite);
			}
			if(map[i][j][2] == 5) {
				hive_sprite.setPosition(map[i][j][0] * 48, map[i][j][1] * 48);
				window.draw(hive_sprite);
			}
		}
	}
}

void update_map(int map[][20][6], Clock& clock, Clock& pause_clock, Clock& fast_clock, int fast_map[][20][5], int flower_map[][3]) {
	static int pause_bee_x = -1, pause_bee_y = -1;
    // if (clock.getElapsedTime().asSeconds() < 0.5f) {
    //     return;
    // }
    // clock.restart();

	if(pause_clock.getElapsedTime().asSeconds() > 2) {
		pause_bee_x = -1;
		pause_bee_y = -1;

		int bee_count = 0;
		for(int i = 0; i < gameColumns; i++) {
			for(int j = 0; j < gameRows; j++) {
				if(map[i][j][2] == 1 || map[i][j][2] == 2) {
					bee_count++;
				}
			}
		}

		if(bee_count > 3) {
			int random_idx = rand() % bee_count;
			int current_bee_count = 0;
			for(int i = 0; i < gameColumns; i++) {
				for(int j = 0; j < gameRows; j++) {
					if(map[i][j][2] == 1 || map[i][j][2] == 2) {
						if(current_bee_count == random_idx) {
							pause_bee_x = j;
							pause_bee_y = i;
							pause_clock.restart();
							break;
						}
						current_bee_count++;
					}
				}
				if(pause_bee_x != -1) break;
			}
		}
	}
	if(fast_clock.getElapsedTime().asSeconds() >= 0.25f) {
		fast_clock.restart();

		int fast_temp[15][20][4] = {};

		for (int i = 0; i < gameColumns; i++) {
			for (int j = 0; j < gameRows; j++) {
				if (fast_map[i][j][2] != 0) { // Entity exists
					int type = fast_map[i][j][2];
					int dir = fast_map[i][j][3];
					int new_x = j;
					int new_y = i;
					int pollinated = fast_map[i][j][4];

					/*if(i == pause_bee_y && j == pause_bee_x) {
						temp[i][j][0] = j;
						temp[i][j][1] = i;
						temp[i][j][2] = type;
						temp[i][j][3] = dir;
						continue;
					}*/

					// Determine movement based on type
					if (type == 1 || type == 2) { // Bees
						if (dir == 1) { // Move left
							new_x = j - 1;
						} else if (dir == 2) { // Move right
							new_x = j + 1;
						}
					}

					// Handle wall collisions
					if (new_x < 0 || new_x >= gameRows) {
						new_y = i + 1; // Move down
						dir = (dir == 1) ? 2 : 1; // Reverse direction
						new_x = j; // Stay in the same column
					}

					if(new_y == 13) {
						new_y -= 1;
					}

					/*if (map[new_y][new_x][2] == 3) { // Honeycomb exists
						if((new_x == 0 || new_x == 19) && (j == 0 || j == 19)) {
							new_y -= 1;
						}
						else {
							new_y = i + 1; // Move down
							dir = (dir == 1) ? 2 : 1; // Reverse direction
							new_x = j; // Stay in the current column
						}
					}*/
					if(fast_map[new_y][new_x][2] == 3 && !(j == 0 || j == 19)) {
						new_x = j;
						new_y -= 1;
					}
					// Handle bee collisions (prevent overwriting)
					if (fast_temp[new_y][new_x][2] != 0 && fast_temp[new_y][new_x][2] != 3) {
						new_y = i;
						new_x = j;
					}

					if(type == 3) {
						new_x = j;
						new_y = i;
					}
					if(new_x == 0 && pollinated == 1) {
						new_x = 0;
						new_y = 0;
						pollinated = 0;
						dir = 2;
					}
					if(new_x == 19 && pollinated == 1) {
						new_x = 0;
						new_y = 0;
						pollinated = 0;
						dir = 2;
					}

					// Update temporary map (do not overwrite honeycomb)
					if (fast_temp[new_y][new_x][2] == 0 || fast_temp[new_y][new_x][2] == 3) {
						fast_temp[new_y][new_x][0] = new_x;
						fast_temp[new_y][new_x][1] = new_y;
						fast_temp[new_y][new_x][2] = type;
						fast_temp[new_y][new_x][3] = dir;

					}
				}
			}
		}

		// Copy temp map back to main map
		for (int i = 0; i < gameColumns; i++) {
			for (int j = 0; j < gameRows; j++) {
				fast_map[i][j][0] = fast_temp[i][j][0];
				fast_map[i][j][1] = fast_temp[i][j][1];
				fast_map[i][j][2] = fast_temp[i][j][2];
				fast_map[i][j][3] = fast_temp[i][j][3];
				fast_map[i][j][4] = fast_temp[i][j][4];
			}
		}
	}

	if (clock.getElapsedTime().asSeconds() < 0.5f) {
        return;
    }

	clock.restart();

    int temp[15][20][6] = {}; // Temporary map to store updates

    for (int i = 0; i < gameColumns; i++) {
        for (int j = 0; j < gameRows; j++) {
            if (map[i][j][2] != 0) { // Entity exists
                int type = map[i][j][2];
                int dir = map[i][j][3];
                int new_x = j;
                int new_y = i;
                int pollinated = map[i][j][4];
                int loop = map[i][j][5]; // Counter for blocked movements

                if (type == 1) { // Bee movement logic
					if(i == pause_bee_y && j == pause_bee_x && i != 0 && (j != 0 || j != 19)) {
						temp[i][j][0] = j;
						temp[i][j][1] = i;
						temp[i][j][2] = type;
						temp[i][j][3] = dir;
						temp[i][j][4] = pollinated;
						temp[i][j][5] = loop;
						continue;
					}

                    // Calculate new position
                    if (dir == 1) { // Moving left
                        new_x -= 1;
                    } else if (dir == 2) { // Moving right
                        new_x += 1;
                    }

                    // Handle wall collisions
                    if (new_x < 0 || new_x >= gameRows || 
                        map[new_y][new_x][2] == 3 || 
                        map[new_y][new_x][2] == 4 || 
                        map[new_y][new_x][2] == 5) {

                        // Attempt to move down
                        new_x = j; // Reset x to current position
                        new_y += 1; // Move down a tier
						dir = (dir == 1) ? 2 : 1; // Reverse direction

                        // Check if the downward position is blocked
                        if (new_y >= gameColumns || map[new_y][new_x][2] == 3 || map[new_y][new_x][2] == 4 || map[new_y][new_x][2] == 5) {
                            // If downward movement is blocked, reverse direction and stay in the same tier
                            new_y = i; // Reset to current tier
                            //dir = (dir == 1) ? 2 : 1; // Reverse direction
							loop++;
                        }
                    }

                    // Prevent bees from overwriting occupied spaces
                    if (temp[new_y][new_x][2] != 0 && temp[new_y][new_x][2] != 3 && temp[new_y][new_x][2] != 4) {
                        if(temp[new_y][new_x][0] == pause_bee_x && temp[new_y][new_x][1] == pause_bee_y) {
							new_y += 1;
						}
						else {
							new_x = j;
							new_y = i;
						}
                    }

					if (temp[new_y][new_x][2] != 0 && temp[new_y][new_x][2] != 3 && temp[new_y][new_x][2] != 4) {
                        if(temp[new_y][new_x][0] == pause_bee_x && temp[new_y][new_x][1] == pause_bee_y) {
							new_y += 1;
							new_x += 1;
						}
						else {
							new_x = j;
							new_y = i;
						}
                    }

					if (new_x < 0 || new_x >= gameRows || 
                        map[new_y][new_x][2] == 3 || 
                        map[new_y][new_x][2] == 4 || 
                        map[new_y][new_x][2] == 5) {

                        // Attempt to move down
                        new_x = j; // Reset x to current position
                        new_y += 1; // Move down a tier
						dir = (dir == 1) ? 2 : 1; // Reverse direction

                        // Check if the downward position is blocked
                        if (new_y >= gameColumns || map[new_y][new_x][2] == 3 || map[new_y][new_x][2] == 4 || map[new_y][new_x][2] == 5) {
                            // If downward movement is blocked, reverse direction and stay in the same tier
                            new_y = i; // Reset to current tier
                            //dir = (dir == 1) ? 2 : 1; // Reverse direction
							loop++;
                        }
                    }

                    // Special handling for pollinated bees at boundaries
                    if (pollinated == 1 && (new_x == 0 || new_x == 19)) {
                        new_x = (new_x == 0) ? 0 : 19; // Stay on boundary
                        new_y = 0; // Move to top row
                        pollinated = 0; // Reset pollination
                        dir = (new_x == 0) ? 2 : 1; // Reverse direction
                    }

					if(new_y != i) {
						loop = 0;
					}

					if(loop >= 5) {
						type = 5;
						loop = 0;
						dir = 0;
						pollinated = 0;
					}
                }

                // Update the temporary map
                temp[new_y][new_x][0] = new_x;
                temp[new_y][new_x][1] = new_y;
                temp[new_y][new_x][2] = type;
                temp[new_y][new_x][3] = dir;
                temp[new_y][new_x][4] = pollinated;
                temp[new_y][new_x][5] = loop;
            }
        }
    }

	// int count = 0;
    // Copy temporary map back to the main map
    for (int i = 0; i < gameColumns; i++) {
        for (int j = 0; j < gameRows; j++) {
            map[i][j][0] = temp[i][j][0];
            map[i][j][1] = temp[i][j][1];
            map[i][j][2] = temp[i][j][2];
            map[i][j][3] = temp[i][j][3];
            map[i][j][4] = temp[i][j][4];
            map[i][j][5] = temp[i][j][5];
			// if(map[i][j][2] == 1) {
			// 	count++;
			// }
        }
    }
	// cout<<count<<endl;
}


void animate_sprites(Clock& clock, IntRect& reg_bee_rect, IntRect& fast_bee_rect, IntRect& bird_sprite_rect, Sprite& reg_bee_sprite, Sprite& fast_bee_sprite, Sprite& bird_sprite) {
	if(clock.getElapsedTime().asSeconds() >= 0.05f) {
		if(reg_bee_rect.left == 144) {
			reg_bee_rect.left = 0;
		}
		else {
			reg_bee_rect.left += 48;
		}
		reg_bee_sprite.setTextureRect(reg_bee_rect);

		if(fast_bee_rect.left == 144) {
			fast_bee_rect.left = 0;
		}
		else {
			fast_bee_rect.left += 48;
		}
		fast_bee_sprite.setTextureRect(fast_bee_rect);

		if(bird_sprite_rect.left == 48) {
			bird_sprite_rect.left = 0;
		}
		else {
			bird_sprite_rect.left += 48;
		}
		bird_sprite.setTextureRect(bird_sprite_rect);

		clock.restart();
	}
}

void check_collision(int map[][20][6], Music& hit_music, int bullet_array[][3], int fast_map[][20][5], int& score, int power_ups[][4], int& level) {
	int bullet_width = 32;
	int bullet_height = 32;

	for(int k = 0; k < 1; k++) {
		for(int i = 0; i < gameColumns; i++) {
			for(int j = 0; j < gameRows; j++) {
				if(map[i][j][2] != 0 && bullet_array[k][2] == 1) {
					float obj_x = map[i][j][0] * 48;
					float obj_y = map[i][j][1] * 48;

					if(bullet_array[k][0] < obj_x + 48 && bullet_array[k][0] + bullet_width > obj_x && bullet_array[k][1] < obj_y + 48 && bullet_array[k][1] + bullet_height > obj_y) {
						if(map[i][j][2] == 1) {
							map[i][j][2] = 3;
							score += 100;
						}
						else if(map[i][j][2] == 2) {
							map[i][j][2] = 4;
						}
						else if(map[i][j][2] == 3) {
							map[i][j][2] = 0;
						}
						else if(map[i][j][2] == 4) {
							map[i][j][2] = 0;
							if((rand() % 2) == 1 && level == 4) {
								int type = rand() % 4 + 1;
								int x_pos = map[i][j][0] * 48;
								int y_pos = map[i][j][1] * 48;
								int timer = 300;

								for(int i = 0; i < 10; i++) {
									if(power_ups[i][2] == 0) {
										power_ups[i][0] = x_pos;
										power_ups[i][1] = y_pos;
										power_ups[i][2] = type;
										power_ups[i][3] = timer;
										// cout<<"added"<<endl;
										break;
									}
								}
							}
						}
						else if(map[i][j][2] == 5) {
							map[i][j][2] = 0;
						}
						
						map[i][j][3] = 0;
						bullet_array[k][2] = 0;
						hit_music.play();
						return;
					}
				}
				if(fast_map[i][j][2] != 0 && bullet_array[k][2] == 1) {
					float obj_x = fast_map[i][j][0] * 48;
					float obj_y = fast_map[i][j][1] * 48;

					if(bullet_array[k][0] < obj_x + 48 && bullet_array[k][0] + bullet_width > obj_x && bullet_array[k][1] < obj_y + 48 && bullet_array[k][1] + bullet_height > obj_y) {
						if(fast_map[i][j][2] == 1) fast_map[i][j][2] = 3;
						else if(fast_map[i][j][2] == 2) {
							map[i][j][2] = 4; 
							map[i][j][0] = fast_map[i][j][0];
							map[i][j][1] = fast_map[i][j][1];
							//map[i][j][2] = fast_map[i][j][2];
							map[i][j][3] = 0;
							fast_map[i][j][0] = 0;
							fast_map[i][j][1] = 0;
							fast_map[i][j][2] = 0;
							fast_map[i][j][3] = 0;

							score += 1000;
						}
						else if(fast_map[i][j][2] == 3) fast_map[i][j][2] = 0;
						else if(fast_map[i][j][2] == 4) fast_map[i][j][2] = 0;
						map[i][j][3] = 0;
						bullet_array[k][2] = 0;
						hit_music.play();
						return;
					}
				}
			}
		}
	}
}

void init_humming_bird(int humming_bird[]) {
	humming_bird[0] = 0;
	humming_bird[1] = resolutionY - 128;
	humming_bird[2] = 0;  //sick
	humming_bird[3] = 0;  //idle
	humming_bird[4] = 2;  //1 == left, 2 == right
}

void draw_bird(int humming_bird[], Sprite& bird_sprite, RenderWindow& window) {
	bird_sprite.setPosition(humming_bird[0], humming_bird[1]);
	if(humming_bird[4] == 1) {
		bird_sprite.setScale(-1.5f, 1.5f);
		bird_sprite.setOrigin(48, 0);
	}
	else if(humming_bird[4] == 2) {
		bird_sprite.setScale(1.5f, 1.5f);
		bird_sprite.setOrigin(0, 0);
	}
	window.draw(bird_sprite);
}

void update_bird_target(int humming_bird[], Clock& clock, int map[][20][6], int& target_x, int& target_y) {
	if(humming_bird[3] == 1)
		return;
	if(clock.getElapsedTime().asSeconds() < 3.0f)
		return;

	clock.restart();

	int total_combs = 0;
	for(int i = 0; i < gameColumns; i++) {
		for(int j = 0; j < gameRows; j++) {
			if(map[i][j][2] == 3 || map[i][j][2] == 4) {
				total_combs++;
			}
		}
	}
	//cout<<"Total: "<<total_combs<<endl;

	if(total_combs > 0 && humming_bird[2] == 0) {
		int random_idx = rand() % total_combs + 1;
		//cout<<"rdm: "<<random_idx<<endl;
		int comb_counter = 0;
		for(int i = 0; i < gameColumns; i++) {
			for(int j = 0; j < gameRows; j++) {
				if(map[i][j][2] == 3 || map[i][j][2] == 4)
					comb_counter++;
				if(comb_counter == random_idx) {
					target_x = map[i][j][0] * 48;
					target_y = map[i][j][1] * 48;
					//cout<<target_x<<"\t"<<target_y<<endl;
					
					return;
				}
			}
		}
	}
	else {
		target_x = -80;
		target_y = resolutionY - 128;
	}
}

void update_bird(int humming_bird[], int map[][20][6], int& target_x, int& target_y, int& score, int power_ups[][4], int& level) {
	float speed = 300.0f;
	float delta = 1.0f/30.0f;

	static Clock idle_clock;
	if(humming_bird[3] == 1) {
		if(idle_clock.getElapsedTime().asSeconds() >= 3.0f) {
			humming_bird[3] = 0;
			int i = target_y / 48;
			int j = target_x / 48;
			if(i >= 0 && i < gameColumns && j >= 0 && j < gameRows) {
				if(map[i][j][2] == 3) {
					if(map[i][j][1] == 0 || map[i][j][1] == 1) score += 1000;
					else if(map[i][j][1] == 3 || map[i][j][1] == 4 || map[i][j][1] == 5) score += 800;
					else score += 500;

					if((rand() % 2) == 1 && level == 4) {
						int type = rand() % 4 + 1;
						int x_pos = map[i][j][0] * 48;
						int y_pos = map[i][j][1] * 48;
						int timer = 300;

						for(int i = 0; i < 10; i++) {
							if(power_ups[i][2] == 0) {
								power_ups[i][0] = x_pos;
								power_ups[i][1] = y_pos;
								power_ups[i][2] = type;
								power_ups[i][3] = timer;
								// cout<<"added"<<endl;
								break;
							}
						}
					}
				}
				else if(map[i][j][2] == 4) {
					if(map[i][j][1] == 0 || map[i][j][1] == 1) score += 2000;
					else if(map[i][j][1] == 3 || map[i][j][1] == 4 || map[i][j][1] == 5) score += 1800;
					else score += 1500;

					if((rand() % 2) == 1 && level == 4) {
						int type = rand() % 4 + 1;
						int x_pos = map[i][j][0] * 48;
						int y_pos = map[i][j][1] * 48;
						int timer = 300;

						for(int i = 0; i < 10; i++) {
							if(power_ups[i][2] == 0) {
								power_ups[i][0] = x_pos;
								power_ups[i][1] = y_pos;
								power_ups[i][2] = type;
								power_ups[i][3] = timer;
								// cout<<"added"<<endl;
								break;
							}
						}
					}
				}
				map[i][j][2] = 0;

			}
		}
		return;
	}

	float dx = target_x - humming_bird[0];
	float dy = target_y - humming_bird[1];
	float distance = sqrt(dx * dx + dy * dy);

	if(distance > 5.0f) {
		dx /= distance;
		dy /= distance;

		//cout<<target_x<<"\t"<<target_y<<"\n";

		humming_bird[0] += dx * speed * delta;
		humming_bird[1] += dy * speed * delta;

		if(dx < 0) humming_bird[4] = 1;
		else humming_bird[4] = 2;
	}
	else {
		humming_bird[0] = target_x;
		humming_bird[1] = target_y;
		humming_bird[3] = 1;
		idle_clock.restart();
	}
}

void check_flower(int flower_map[][3], int map[][20][6]) {
	for(int i = 0; i < gameColumns; i++) {
		for(int j = 0; j < gameRows; j++) {
			int x_pos = map[i][j][0];
			int y_pos = map[i][j][1];
			int dir = map[i][j][3];
			int pol = map[i][j][4];
			int type = map[i][j][2];
			
			if(type == 1 && y_pos == 12 && pol == 0) {
				if(flower_map[0][2] == 0 || flower_map[19][2] == 0) {
					flower_map[0][2] = 1;
					flower_map[0][0] = 0;
					flower_map[0][1] = 13;

					flower_map[19][2] = 1;
					flower_map[19][0] = 19;
					flower_map[19][1] = 13;
					//cout<<flower_map[19][0]<<endl;
					//cout<<"Cond1"<<endl;

					pol = 1;
					dir = (dir == 1)? 2 : 1;
				}
				else {
					if(flower_map[x_pos][2] == 0 && map[i][j][4] == 0) {
						flower_map[x_pos][2] = 1;
						flower_map[x_pos][0] = x_pos;
						flower_map[x_pos][1] = 13; 
						//cout<<flower_map[19][2]<<endl;
						//cout<<"Cond2"<<endl;

						pol = 1;
						dir = (dir == 1)? 2 : 1;
					}
				}
			}

			map[i][j][3] = dir;
			map[i][j][4] = pol;
		}
	}
}

void draw_flowers(int flower_map[][3], RenderWindow& window, Sprite& obstacle_sprite) {
	int count = 0;
	for(int i = 0; i < gameRows; i++) {
		if(flower_map[i][2] == 1) {
			obstacle_sprite.setPosition(flower_map[i][0] * 48, flower_map[i][1] * 48);
			window.draw(obstacle_sprite);
			count++;
		}
	}
}

bool flower_collision(int flower_map[][3], float& player_x, float& player_y) {
    const int player_w = 64; // Player width
    const int player_h = 64; // Player height
    const int flower_w = 48; // Flower width
    const int flower_h = 48; // Flower height

    for (int i = 0; i < gameRows; i++) {
        // Check if the flower is active
        if (flower_map[i][2] == 0) {
            continue;
        }

        int flower_x = flower_map[i][0] * 48;
        int flower_y = flower_map[i][1] * 48;

        // AABB collision detection
        if (player_x < flower_x + flower_w && player_x + player_w > flower_x &&
            player_y < flower_y + flower_h && player_y + player_h > flower_y) {
            //cout << "Collision detected!" << endl;
            return true;
        }
    }
    //cout << "No collision detected." << endl;
    return false;
}

void level_updater(int& max_reg_bees, int& max_fast_bees, int& max_honeycombs, int& level, int map[][20][6], Clock& clock, int fast_map[][20][5], int& sprays, int& spray_cans, int& lvl_sprays, int& current_can_lvl, bool reset, int& max_hives) {
	switch (level) {
		case 1:
			max_reg_bees = 20;
			max_fast_bees = 0;
			max_honeycombs = 3;
			max_hives = 0;
			generate_map(map, max_reg_bees, max_honeycombs, max_fast_bees, clock, fast_map, max_hives);
			sprays = 56;
			if(!reset && spray_cans <= 3)
				spray_cans = 3;
			lvl_sprays = 0;
			current_can_lvl = 1;
			break;
		
		case 2:
			max_reg_bees = 15;
			max_fast_bees = 5;
			max_honeycombs = 9;
			max_hives = 0;
			generate_map(map, max_reg_bees, max_honeycombs, max_fast_bees, clock, fast_map, max_hives);
			sprays = 56;
			if(!reset && spray_cans <= 3)
				spray_cans = 3;
			lvl_sprays = 0;
			current_can_lvl = 1;
			break;

		case 3:
			max_reg_bees = 20;
			max_fast_bees = 10;
			max_honeycombs = 15;
			max_hives = 0;
			generate_map(map, max_reg_bees, max_honeycombs, max_fast_bees, clock, fast_map, max_hives);
			sprays = 56;
			if(!reset && spray_cans <= 3)
				spray_cans = 3;
			lvl_sprays = 0;
			current_can_lvl = 1;
			break;

		case 4:
			max_reg_bees = 20;
			max_fast_bees = 15;
			max_honeycombs = 15;
			max_hives = 5;
			generate_map(map, max_reg_bees, max_honeycombs, max_fast_bees, clock, fast_map, max_hives);
			sprays = 56;
			if(!reset && spray_cans <= 3)
				spray_cans = 3;
			lvl_sprays = 0;
			current_can_lvl = 1;
			break;

		default:
			break;
	}
}

bool next_level(int map[][20][6], int fast_map[][20][5], int& score, int& level, Font& font, RenderWindow& window, bool& in_menu, int& max_reg_bees, int& max_fast_bees, int& max_honeycombs, int& selected_opt) {
	int bee_count = 0, hive_count = 0, comb_count = 0;
	for(int i = 0; i < gameColumns; i++) {
		for(int j = 0; j < gameRows; j++) {
			int type = map[i][j][2];
			if(type == 1 || fast_map[i][j][2] == 2) {
				bee_count++;
			}
			if(type == 5) {
				hive_count++;
			}
			if(type == 3 || type == 4) {
				comb_count++;
			}
		}
	}

	if(bee_count == 0) {
		/*if(clock.getElapsedTime().asSeconds() < 3.0f) {
			return false;
		}*/
		for(int i = 0; i < gameColumns; i++) {
			for(int j = 0; j < gameRows; j++) {
				if(map[i][j][2] == 5) {
					if(map[i][j][1] == 0 || map[i][j][1] == 1) {
						score += 2000;
					}
					else if(map[i][j][1] == 3 || map[i][j][1] == 4 || map[i][j][1] == 5) {
						score += 1600;
					}
					else {
						score += 1000;
					}
				}
				else if(map[i][j][2] == 3) {
					if(map[i][j][1] == 0 || map[i][j][1] == 1) {
						score += 1000;
					}
					else if(map[i][j][1] == 2 || map[i][j][1] == 3 || map[i][j][1] == 5) {
						score += 800;
					}
					else {
						score += 500;
					}
				}
				else if(map[i][j][2] == 4) {
					if(map[i][j][1] == 0 || map[i][j][1] == 1) {
						score += 2000;
					}
					else if(map[i][j][1] == 2 || map[i][j][1] == 3 || map[i][j][1] == 5) {
						score += 1800;
					}
					else {
						score += 1500;
					}
				}

				map[i][j][0] = 0;
				map[i][j][1] = 0;
				map[i][j][2] = 0;
				map[i][j][3] = 0;
				map[i][j][4] = 0;
				map[i][j][5] = 0;
			}
		}
		level++;
		cout<<level<<endl;
		if(level > 4) feed_score(score, window, font, in_menu, level, max_reg_bees, max_fast_bees, max_honeycombs, selected_opt);
		return true;
	}
	return false;
}

void reset_lvl(int map[][20][6], int fast_map[][20][5], int& sprays, int& spray_cans, int& lvl_sprays, int& current_can_lvl, int& max_reg_bees, int& max_fast_bees, int& max_honeycombs, int& level, Clock& clock, int flower_map[][3], int& score, RenderWindow& window, Font& font, bool& in_menu, int& selected_opt, int& max_hives) {
	int flower_count = 0;
	for(int k = 0; k < gameRows; k++) {
		if(flower_map[k][2] == 1) {
			flower_count++;
		}
	}

	//cout<<flower_count<<endl;

	if(flower_count >= 19) {
		flower_count = 0;
		for(int i = 0; i < gameColumns; i++) {
			for(int j = 0; j < gameRows; j++) {
				map[i][j][0] = 0;
				map[i][j][1] = 0;
				map[i][j][2] = 0;
				map[i][j][3] = 0;
				map[i][j][4] = 0;
			}
		}

		for(int k = 0; k < gameRows; k++) {
			flower_map[k][0] = 0;
			flower_map[k][1] = 0;
			flower_map[k][2] = 0;
		}

		sprays = 56;
		spray_cans--;
		lvl_sprays = 0;
		current_can_lvl = 1;

		if(spray_cans <= 0) 
			feed_score(score, window, font, in_menu, level, max_reg_bees, max_fast_bees, max_honeycombs, selected_opt);
		else 
			level_updater(max_reg_bees, max_fast_bees, max_honeycombs, level, map, clock, fast_map, sprays, spray_cans, lvl_sprays, current_can_lvl, true, max_hives);
	}
}

void bird_collision(int humming_bird[], int bullet_array[][3], Clock& reset_clock) {
	if(reset_clock.getElapsedTime().asSeconds() < 8.0f && humming_bird[2] == 1) {
		return;
	}

	reset_clock.restart();
	humming_bird[2] = 0;

	int bullet_w = 32;
	int bullet_h = 32;
	int bird_w = 48;
	int bird_h = 48;
	int bird_x = humming_bird[0];
	int bird_y = humming_bird[1];
	static int hit_count = 0;

	for(int i = 0; i < 1; i++) {
		int bullet_x = bullet_array[i][0];
		int bullet_y = bullet_array[i][1];

		if(bullet_x < bird_x + bird_w && bullet_x + bullet_w > bird_x
		&& bullet_y < bird_y + bird_h && bullet_y + bullet_h > bird_y) {
			hit_count++;
			bullet_array[i][2] = 0;
			bullet_array[i][1] = 0;
			bullet_array[i][0] = 0;
			//cout<<hit_count<<endl;
		}
	}

	if(hit_count >= 3) {
		humming_bird[2] = 1;
		hit_count = 0;
	}
}

void award_can(int score, int& spray_cans) {
	static int first_award = 0;
	static int second_award = 0;
	static int last_milestone = 0;
	int milestone = 80000;

	if(score >= 20000 && first_award == 0) {
		spray_cans += 1;
		first_award = 1;
		//cout<<"awarded"<<endl;
	}
	else if(score >= 40000 && second_award == 0) {
		spray_cans += 1;
		second_award = 1;
		//cout<<"awarded"<<endl;
	}

	else if(score >= last_milestone + milestone) {
		spray_cans += 1;
		//cout<<"awarded"<<endl;
		last_milestone  += milestone;
	}
}

void display_scores(RenderWindow& window, Font font) {
	int scores[10] = {};
	char names[10][20] = {};

	ifstream score_file("high_scores.txt");
	if(!score_file.is_open()) {
		cout<<"Error opening file...\n";
		return;
	}

	for(int i = 0; i < 10; i++) {
		score_file>>names[i];
		score_file>>scores[i];
	}

	Text title("High Scores", font, 50);
	title.setFillColor(Color::Yellow);
	title.setPosition(resolutionX/2 - title.getGlobalBounds().width/2, 50);

	Text header("Rank       Name                          Score     ", font, 30);   //15 30 10
	header.setFillColor(Color::Green);
	header.setPosition(180, 150);

	Text rank_entry("", font, 30);
	rank_entry.setFillColor(Color::Red);

	Text name_entry("", font, 30);
	name_entry.setFillColor(Color::Red);

	Text score_entry("", font, 30);
	score_entry.setFillColor(Color::Red);

	while(window.isOpen()) {
		Event e;
		while(window.pollEvent(e)) {
			if(e.type == Event::Closed) {
				window.close();
				return;
			}

			if(e.type == Event::KeyPressed && e.key.code == Keyboard::Escape) {
				return;
			}
		}

		window.clear(Color::Black);
		window.draw(title);
		window.draw(header);

		for(int i = 0; i < 10; i++) {
			if(strlen(names[i]) == 0) {score_file.close(); break;}

			string rank = to_string(i+1);
			string name = names[i];
			string score = to_string(scores[i]);

			rank_entry.setString(rank);
			rank_entry.setPosition(180, 200 + i * 40);

			name_entry.setString(name);
			name_entry.setPosition(337, 200 + i * 40);

			score_entry.setString(score);
			score_entry.setPosition(690, 200 + i * 40);

			window.draw(rank_entry);
			window.draw(name_entry);
			window.draw(score_entry);
		}

		window.display();
	}
	
}

void feed_score(int& score, RenderWindow& window, Font font, bool& in_menu, int& level, int& max_reg_bees, int& max_fast_bees, int& max_honeycombs, int& selected_opt) {
    int scores[10] = {};        // Stores the top 10 scores
    char names[10][20];         // Stores the names corresponding to the top 10 scores

    ifstream score_file("high_scores.txt");
    if (!score_file.is_open()) {
        cout << "Error opening file...\n";
        return;
    }

    for (int i = 0; i < 10; i++) {
        score_file >> names[i] >> scores[i];
    }

	score_file.close();

    // Set up the text box for name input
    RectangleShape text_rect(Vector2f(500, 140));
    text_rect.setPosition(resolutionX / 2 - 250, resolutionY / 2 - 70);
    Color gray(100, 100, 100, 50);
    text_rect.setFillColor(gray);

    RectangleShape text_box(Vector2f(490, 64));
    text_box.setPosition(resolutionX / 2 - 245, resolutionY / 2);
    text_box.setFillColor(Color::White);

    // Variables to handle player input
    bool input_done = false;
    char player_input[20] = "";  // Start with an empty string for input
    int input_index = 0;       // Track the current index of the input string
    Text player_text;
    player_text.setFont(font);   
    player_text.setCharacterSize(30);
    player_text.setFillColor(Color::Black);
    player_text.setPosition(resolutionX / 2 - 245, resolutionY / 2 + 20);

	Text prompt("Enter your name", font, 30);
	prompt.setFillColor(Color::White);
	prompt.setPosition(resolutionX/2 - prompt.getGlobalBounds().width/2, resolutionY/2 - 50);

    // Main game loop for handling input and updating the display
    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
            }

            // Handle text input (typing in the text box)
            if (e.type == Event::TextEntered) {
                if(e.text.unicode == '\r') {
					cout<<"Name Entered: "<< player_input;
					cout<<endl;

					bool inserted = false;
					for(int i = 0; i < 10; i++) {
						if(score > scores[i]) {
							// cout<<"Less foundd"<<endl;
							for(int j = 9; j > i; j--) {
								scores[j] = scores[j - 1];
								strcpy(names[j], names[j - 1]);
							}

							scores[i] = score;
							strcpy(names[i], player_input);
							inserted = true;
							break;
						}
					}

					if(inserted) {
						ofstream out_score("high_scores.txt");
						for(int i = 0; i < 10; i++) {
							out_score<<names[i]<<" "<<scores[i]<<endl;
							// cout<<i<<" Inserted"<<endl;
						}
						out_score.close();
					}
					score = 0;
					in_menu = true;
					level = 1;
					max_reg_bees = 20;
					max_fast_bees = 0;
					max_honeycombs = 3;
					selected_opt = 5;

					return;
			    }
				else if(e.text.unicode == '\b') {
					if(input_index > 0) {
						player_input[--input_index] = '\0';	
					}
				}
				else if(e.text.unicode < 128 && input_index < 20) {
					player_input[input_index++] = static_cast<char>(e.text.unicode);
					player_input[input_index] = '\0';
				}
				player_text.setString(player_input);
            }
        }

        window.clear(Color::Black);  // Clear the window each frame

        // Draw the UI
        window.draw(text_rect);
        window.draw(text_box);
		window.draw(prompt);
        window.draw(player_text);  // Display the player input text

        window.display();  // Display the updated window content
    }
}

void update_power_ups(int power_ups[][4]) {
	for(int i = 0; i < 10; i++) {
		if(power_ups[i][2] != 0) {
			if(power_ups[i][1] < resolutionY - 96) {
				power_ups[i][1] += 5;
			}
			power_ups[i][3] -= 1;

			if(power_ups[i][3] <= 0) {
				power_ups[i][0] = 0;
				power_ups[i][1] = 0;
				power_ups[i][2] = 0;
				power_ups[i][3] = 0;
			}
		}
	}
}

void draw_power_ups(RenderWindow& window, Sprite& height_inc_sprite, Sprite& height_dec_sprite, Sprite& speed_inc_sprite, Sprite& speed_dec_sprite, int power_ups[][4]) {
	for(int i = 0; i < 10; i++) {
		if(power_ups[i][2] != 0) {
			int type = power_ups[i][2];
			int x_pos = power_ups[i][0];
			int y_pos = power_ups[i][1];

			if(type == 1) { //height inc
				height_inc_sprite.setPosition(x_pos, y_pos);
				height_inc_sprite.setScale(1.5f, 1.5f);
				window.draw(height_inc_sprite);
			}
			else if(type == 2) { //height dec
				height_dec_sprite.setPosition(x_pos, y_pos);
				height_dec_sprite.setScale(1.5f, 1.5f);
				window.draw(height_dec_sprite);
			}
			else if(type == 3) { //speed inc
				speed_inc_sprite.setPosition(x_pos, y_pos);
				speed_inc_sprite.setScale(1.5f, 1.5f);
				window.draw(speed_inc_sprite);
			}
			else if(type == 4) { // speed dec
				speed_dec_sprite.setPosition(x_pos, y_pos);
				speed_dec_sprite.setScale(1.5f, 1.5f);
				window.draw(speed_dec_sprite);
			}
		}
	}
}

void power_ups_collision(int power_ups[][4], float& player_x, float& player_y, int active_power_ups[][2]) {
    int player_w = 64;
    int player_h = 64;

    for (int i = 0; i < 10; i++) {
        if (power_ups[i][2] != 0) { // Check if power-up is active
            int power_x = power_ups[i][0];
            int power_y = power_ups[i][1];
            int power_w = 48;
            int power_h = 48;

            // Check for collision
            if (player_x < power_x + power_w && player_x + player_w > power_x &&
                player_y < power_y + power_h && player_y + player_h > power_y) {
                
                // Handle power-up activation or cancellation
                for (int j = 0; j < 2; j++) {
                    // If the slot is empty or the same power-up type is already active
                    if (active_power_ups[j][0] == 0 || active_power_ups[j][0] == power_ups[i][2]) {
                        active_power_ups[j][0] = power_ups[i][2];
                        active_power_ups[j][1] = 300; // Reset duration
                        break;
                    }

                    // Handle cancellation for conflicting power-ups
                    if ((power_ups[i][2] == 1 && active_power_ups[j][0] == 2) || 
                        (power_ups[i][2] == 2 && active_power_ups[j][0] == 1) || 
                        (power_ups[i][2] == 3 && active_power_ups[j][0] == 4) || 
                        (power_ups[i][2] == 4 && active_power_ups[j][0] == 3)) {
                        
                        active_power_ups[j][0] = 0; // Cancel the conflicting power-up
                        active_power_ups[j][1] = 0;
                        break;
                    }
                }

                // Remove the power-up from the game world
                power_ups[i][0] = 0;
                power_ups[i][1] = 0;
                power_ups[i][2] = 0;
                power_ups[i][3] = 0;
            }
        }
    }
}


void update_active_power_ups(RenderWindow& window, int active_power_ups[][2], float& player_vel, Sprite& player_sprite, float& player_y) {
    float temp_vel = 10; // Default velocity if no power-ups are active
    float scale_x = 1.0f; // Default horizontal scale
    float scale_y = 1.0f; // Default vertical scale
	float temp_y = resolutionY - 112;

    for (int i = 0; i < 2; i++) {
        if (active_power_ups[i][0] != 0) {
            int width = active_power_ups[i][1];

            // Draw the power-up timer bar
            RectangleShape bar(Vector2f(width / 2.0f, 10.0f));
            bar.setPosition(resolutionX / 2 - 150, resolutionY - (15 + 25 * i));
            bar.setFillColor(Color::Blue);

            active_power_ups[i][1] -= 1; // Decrement timer
            if (active_power_ups[i][1] <= 0) {
                active_power_ups[i][0] = 0; // Deactivate expired power-up
                active_power_ups[i][1] = 0;
            }

            // Update behavior based on the active power-up type
            int type = active_power_ups[i][0];
            if (type == 1) {
                scale_x = 1.25f; // Increase size horizontally
                scale_y = 1.25f; // Increase size vertically
				temp_y = resolutionY - 128;
            } else if (type == 2) {
                scale_x = 0.75f; // Decrease size horizontally
                scale_y = 0.75f; // Decrease size vertically
				temp_y = resolutionY - 96;
            } else if (type == 3) {
                temp_vel = 15; // Speed increase
            } else if (type == 4) {
                temp_vel = 7; // Speed decrease
            }

            window.draw(bar);
        }
    }

    player_vel = temp_vel;
	player_y = temp_y;
    player_sprite.setScale(scale_x, scale_y); 
}



void draw_icons(RenderWindow& window, Sprite& height_inc_sprite, Sprite& height_dec_sprite, Sprite& speed_inc_sprite, Sprite& speed_dec_sprite, int active_power_ups[][2]) {
	for(int i = 0; i < 2; i++) {
		if(active_power_ups[i][0] != 0) {
			int type = active_power_ups[i][0];

			if(type == 1) { //height inc
				height_inc_sprite.setPosition(resolutionX/2 - 185, resolutionY - (22 + 25 * i));
				height_inc_sprite.setScale(0.75f, 0.75f);
				window.draw(height_inc_sprite);
			}
			else if(type == 2) { //height dec
				height_dec_sprite.setPosition(resolutionX/2 - 185, resolutionY - (22 + 25 * i));
				height_dec_sprite.setScale(0.75f, 0.75f);
				window.draw(height_dec_sprite);
			}
			else if(type == 3) { //speed inc
				speed_inc_sprite.setPosition(resolutionX/2 - 185, resolutionY - (22 + 25 * i));
				speed_inc_sprite.setScale(0.75f, 0.75f);
				window.draw(speed_inc_sprite);
				// cout<<"speed+"<<endl;
			}
			else if(type == 4) { // speed dec
				speed_dec_sprite.setPosition(resolutionX/2 - 185, resolutionY - (22 + 25 * i));
				speed_dec_sprite.setScale(0.75f, 0.75f);
				window.draw(speed_dec_sprite);
				// cout<<"speed-"<<endl;
			}
		}
	}
}

int** init_infant_bees(int map[][20][6], int& bee_count) {
	int** arr = nullptr;

	int hive_count = 0;
	for(int i = 0; i < gameColumns; i++) {
		for(int j = 0; j < gameRows; j++) {
			if(map[i][j][2] == 5 && map[i-1][j][2] != 3 && map[i-1][j][2] != 4 && map[i-1][j][2] != 5) {
				hive_count++;
			}
		}
	}

	arr = new int*[hive_count];
	int idx = 0;

	for(int i = 0; i < gameColumns; i++) {
		for(int j = 0; j < gameRows; j++) {
			if(map[i][j][2] == 5 && map[i-1][j][2] != 3 && map[i-1][j][2] != 4 && map[i-1][j][2] != 5) {
				arr[idx] = new int[3];
				arr[idx][0] = j;
				arr[idx][1] = i-1;
				arr[idx][2] = 0;
				idx++;

				map[i][j][0] = 0;
				map[i][j][1] = 0;
				map[i][j][2] = 0;
				map[i][j][3] = 0;
				map[i][j][4] = 0;
				map[i][j][5] = 0;
			}
		}
	}

	bee_count = idx;
	cout<<bee_count<<endl;

	return arr;
}

void animate_infant_bee(Sprite& infant_bee_sprite, IntRect& infant_bee_rect, Clock& clock) {
	if(clock.getElapsedTime().asSeconds() < 0.05f) {
		return;
	}

	clock.restart();

	if(infant_bee_rect.left == 128) {
		infant_bee_rect.left = 0;
	}
	else {
		infant_bee_rect.left += 32;
	}

	infant_bee_sprite.setTextureRect(infant_bee_rect);
}

void draw_infant_bees(RenderWindow& window, int** infant_bees, Sprite& infant_bee_sprite, int& hive_count) {
    // cout << "Entered draw func" << endl;

    // Check if the array is null
    if (infant_bees == nullptr) {
        cout << "infant_bees is nullptr" << endl;
        return; // Exit the function to avoid segmentation fault
    }

    // Check if hive_count is valid
    if (hive_count <= 0) {
        cout << "hive_count is invalid (<=0)" << endl;
        return; // Exit if there are no bees to draw
    }

    for (int i = 0; i < hive_count; i++) {
        // Check if the pointer to each bee is valid
        if (infant_bees[i] == nullptr) {
            cout << "Null pointer encountered at index " << i << endl;
            continue; // Skip this invalid entry
        }

        int x_pos = infant_bees[i][0] * 48;
        int y_pos = infant_bees[i][1] * 48;
        int dir = infant_bees[i][2];

        if (dir == 0 || dir == 1) {
            infant_bee_sprite.setScale(1.5f, 1.5f);
            infant_bee_sprite.setOrigin(0, 0);
        } else if (dir == 2) {
            infant_bee_sprite.setScale(-1.5f, 1.5f);
            infant_bee_sprite.setOrigin(48, 0);
        }

        infant_bee_sprite.setPosition(x_pos, y_pos);
		if(dir != -1)
        	window.draw(infant_bee_sprite);
    }
}

void update_infant_bees(int** infant_bees, int fast_map[][20][5], int map[][20][6], int count, Clock& clock) {
	if(clock.getElapsedTime().asSeconds() < 0.5f) {
		return;
	}
	clock.restart();
	const int size = count;
	int temp[count][3] = {};

	for(int i = 0; i < count; i++) {
		int x_pos = infant_bees[i][0];
		int y_pos = infant_bees[i][1];
		int dir = infant_bees[i][2];

		//cout<<"Bee"<<i+1<<" at "<<x_pos<<", "<<y_pos<<", dir: "<<dir<<endl;

		if(dir != -1) {
			if(dir == 0) {
				y_pos -= 1;
			}
			if(map[y_pos][x_pos][2] == 3 || map[y_pos][x_pos][2] == 4 || map[y_pos][x_pos][2] == 5) {
				y_pos += 1;
				dir = 1;
			}
			if(dir == 1) {
				x_pos -= 1;
				dir = 0;
			}
			if(map[y_pos][x_pos][2] == 3 || map[y_pos][x_pos][2] == 4 || map[y_pos][x_pos][2] == 5) {
				x_pos += 1;
				dir = 2;
			}
			if(dir == 2) {
				x_pos += 1;
				dir = 0;
			}
			if(map[y_pos][x_pos][2] == 3 || map[y_pos][x_pos][2] == 4 || map[y_pos][x_pos][2] == 5) {
				x_pos -= 1;
				dir = -1;
				map[y_pos][x_pos][0] = x_pos;
				map[y_pos][x_pos][1] = y_pos;
				map[y_pos][x_pos][2] = 5;
				map[y_pos][x_pos][3] = 0;
				map[y_pos][x_pos][4] = 0;
				map[y_pos][x_pos][5] = 0;
			}
			if(y_pos == 0) {
				dir = -1;
				fast_map[y_pos][x_pos][0] = x_pos;
				fast_map[y_pos][x_pos][1] = y_pos;
				fast_map[y_pos][x_pos][2] = 2;
				fast_map[y_pos][x_pos][3] = 1;
				fast_map[y_pos][x_pos][4] = 0;
				// cout<<"Red bee"<<endl;
			}

			temp[i][0] = x_pos;
			temp[i][1] = y_pos;
			temp[i][2] = dir;
		}
		else {
			temp[i][0] = -1;
			temp[i][1] = -1;
			temp[i][2] = -1;
		}
	}

	for(int i = 0; i < count; i++) {
		infant_bees[i][0] = temp[i][0];
		infant_bees[i][1] = temp[i][1];
		infant_bees[i][2] = temp[i][2];
	}
}

void remove_infant_bees(int** infant_bees, int& count) {
    int rem_count = 0;
	// cout<<"Count:"<<count<<endl;
    for(int i = 0; i < count; i++) {
        if(infant_bees[i][2] == -1) {
            rem_count++;
        }
    }

    if(rem_count == count) {
        for(int i = 0; i < count; i++) {
            // cout << "Deleting" << endl;
            delete[] infant_bees[i]; // Delete each individual array
        }
        delete[] infant_bees;  // Now delete the array of pointers
        infant_bees = nullptr;  // Set the pointer to null
        count = 0;  // Reset the count
    }
}

void infant_bees_collision(int bullet_array[][3], int** infant_bees, int& count, Music& music, int& score) {
	int inf_w = 48;
	int inf_h = 48;
	int bull_w = 48;
	int bull_h = 48;

	for(int k = 0; k < 1; k++) {
		if(bullet_array[k][2] == 1) {
			int bull_x = bullet_array[k][0];
			int bull_y = bullet_array[k][1];
			for(int i = 0; i < count; i++) {
				int inf_x = infant_bees[i][0] * 48;
				int inf_y = infant_bees[i][1] * 48;

				if(bull_x < inf_x + inf_w && bull_x + bull_w > inf_x && bull_y < inf_y + inf_h && bull_y + bull_h > inf_y) {
					infant_bees[i][2] = -1;
					bullet_array[k][2] = 0;
					music.play();
					score -= 500;
					break;
				}
			}
		}
	}
}
