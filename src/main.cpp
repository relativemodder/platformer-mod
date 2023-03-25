#include <matdash.hpp>
#include <matdash/minhook.hpp>
#include <matdash/boilerplate.hpp>
#include <matdash/console.hpp>
#include <gd.h>

float new_speed = 0.0f;
float new_position = 0.0f;
float speed_modificator = 397.5f;

bool flipped = false;
bool is_platformer_active;

matdash::cc::thiscall<void> PlayLayer_reset(gd::PlayLayer* self) {
    matdash::orig<&PlayLayer_reset>(self);

    new_position = 0.0f;

    return {};
}


void executePlatformer(gd::PlayLayer* playLayer, float deltaTime) {
    if (playLayer->m_isDead) return;

    if ((GetKeyState('A') & 0x8000) || (GetKeyState(VK_LEFT) & 0x8000))
    {
        flipped = true;
        new_position -= playLayer->m_player1->m_playerSpeed * deltaTime * 0.9f * speed_modificator;
    }

    else if ((GetKeyState('D') & 0x8000) || (GetKeyState(VK_RIGHT) & 0x8000))
    {
        flipped = false;
        new_position += playLayer->m_player1->m_playerSpeed * deltaTime * 0.9f * speed_modificator;
    }

    playLayer->m_player1->m_position.x = new_position;
    playLayer->m_player1->setFlipX(flipped);
    playLayer->m_player2->m_position.x = new_position;
    playLayer->m_player2->setFlipX(flipped);
}


matdash::cc::thiscall<void> PlayLayer_update(gd::PlayLayer* self, float dt) {
    matdash::orig<&PlayLayer_update>(self, dt);

    if (is_platformer_active)
        executePlatformer(self, dt);

    return {};
}


bool PlayLayer_init(gd::PlayLayer* self, gd::GJGameLevel* level) {
    auto result = matdash::orig<&PlayLayer_init>(self, level);
    is_platformer_active = gd::GameManager::sharedState()->getGameVariable("platformerOn");

    return result;
}



bool MoreOptionsLayer_init(gd::MoreOptionsLayer* self) {
    if (!matdash::orig<&MoreOptionsLayer_init>(self)) return false;

    self->addToggle(
        "Platformer by RLT",
        "platformerOn",
        "Enables platformer mode while playing levels.");

    return true;
}


void mod_main(HMODULE) {
    matdash::create_console();

    matdash::add_hook<&PlayLayer_update>(gd::base + 0x2029c0);
    matdash::add_hook<&PlayLayer_init>(gd::base + 0x1FB780);

    matdash::add_hook<&PlayLayer_reset>(gd::base + 0x20BF00);
    matdash::add_hook<&MoreOptionsLayer_init>(gd::base + 0x1DE8F0);
}