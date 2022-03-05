#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include <queue>

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
    
    void AddBlock(class Actor* block);
    void RemoveBlock(class Actor* block);
    std::vector<class Actor*> GetBlocks() {return mBlocks;}
    
    void AddCone(class SecurityCone* cone);
    void RemoveCone(class SecurityCone* cone);
    std::vector<class SecurityCone*> GetCones() {return mCones;}
    
    void SetPlayer(class Player* ThePlayer) {mPlayer = ThePlayer;}

	Mix_Chunk* GetSound(const std::string& fileName);

	class Renderer* GetRenderer() {	return mRenderer; }
    class Player* GetPlayer() {return mPlayer;}
    int channel;
    
    void addCheckpoint(class Checkpoint* c);
    void removeCheckpoint();
    std::queue<class Checkpoint*> GetCheckpoint() {return mCheckpoints;}
    
    void setNextLevel (std::string nextLevel);
    void LoadNextLevel();

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, Mix_Chunk*> mSounds;
    std::queue<class Checkpoint*> mCheckpoints;

	// All the actors in the game
	std::vector<class Actor*> mActors;
    std::vector<class Actor*> mBlocks;
    std::vector<class SecurityCone*> mCones;
    class Player* mPlayer = nullptr;

	class Renderer* mRenderer = nullptr;
    Mix_Chunk* titleSong = nullptr;

	Uint32 mTicksCount = 0;
	bool mIsRunning;
    
    const Vector3 meye {-300.0f, 0.0f, 100.0f};
    const Vector3 mtarget {20.0f, 0.0f, 0.0f};

    Matrix4 proj = Matrix4::CreatePerspectiveFOV(1.22f, 1024.0f, 768.0f, 10.0f, 10000.0f);
    std::string mNextLevel = "";
    class Arrow* mArrow;
};
