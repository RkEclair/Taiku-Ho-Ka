#include <OpenGL/gl.h>
#include <SDL.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <SDL_ttf.h>

#include <random>
#include <vector>
#include <cmath>
#include <chrono>

struct Data {
public:
	bool quit = false;
	bool fail = false;
	bool isPressedSpace = false;
	bool awaiting = false;
	std::chrono::system_clock::time_point lastShootTime;
	std::chrono::system_clock::time_point lastSpawnTime;
	int enemySpawnInterval = 3000;
	double rotation = 0.0;
	std::vector<struct Enemy*> enemyPool{};
	std::vector<struct Bullet*> bulletPool{};
	std::vector<struct Explode*> explodePool{};
	unsigned int score = 0;
	unsigned int highscore;
	unsigned int lives = 3;

	SDL_Window *window{};
	SDL_GLContext context = NULL;
	SDL_Renderer *renderer = nullptr;
	SDL_Surface *playerTex[3] = {};
	SDL_Surface *enemy1Tex = nullptr;
	SDL_Surface *enemy2Tex = nullptr;
	SDL_Surface *enemy3Tex = nullptr;
	SDL_Surface *explodeTex[11] = {};

	TTF_Font *font;

	std::random_device rd;
	std::mt19937 mt;

	Data() : mt(rd()) { ; }
};

struct Ridgebody {
	double posX = 0, posY = 0;
	bool isCollision(Ridgebody const& a) const {
		using namespace std;
		return (sqrt(pow(abs(posX - a.posX), 2) + pow(abs(posY - a.posY), 2))) <= 50;
	}
};

struct Enemy {
public:
	Ridgebody body{};
	bool moveToRight = false;
	enum struct Type {
		helicopter,
		bomber,
		ufo,
	};
	Type type = Type::helicopter;
	void update() {
		body.posX += (moveToRight ? 1 : -1);
	}
};

struct Bullet {
	Ridgebody body;
	int tag = 0;
	virtual void update() = 0;
	virtual ~Bullet() = default;
};

struct MyBullet : Bullet {
public:
	double rotate = 0;
	double speed = 0;
	void update() override {
		Bullet::body.posX += (std::cos(rotate) * 180.0 / M_PI) * speed;
		Bullet::body.posY += (std::sin(rotate) * 180.0 / M_PI) * speed;
	}
	virtual ~MyBullet() = default;
};

struct EnemyBullet : Bullet {
	void update() override {
		Bullet::body.posY += 2;
	}
	virtual ~EnemyBullet() = default;
};

struct Explode {
	int posX = 0, posY = 0;
	int subTexStep = 0;
};
