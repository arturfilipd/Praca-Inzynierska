#include "Player.h"
#include "Scene.h"


Player::Player(float x, float y, const char* modelPath) {
    this->x = x;
    this->y = y;
    speed = 3;
    model = new Model(modelPath);
    rotation = glm::vec2(1.f, 0.5f);
    type = PLAYER;
    health = 10;
    armor = 0;
}

void Player::Update(){ 

    Entity::Update();  
   

    if (hasControl) {
        float xDir = 0;
        float yDir = 0;
        float m = 1;
        if (glfwGetKey(game->GetWindow(), GLFW_KEY_1) == GLFW_PRESS) SetWeapon(1);
        if (glfwGetKey(game->GetWindow(), GLFW_KEY_2) == GLFW_PRESS) SetWeapon(2);
        if (glfwGetKey(game->GetWindow(), GLFW_KEY_3) == GLFW_PRESS) SetWeapon(3);
        if (glfwGetKey(game->GetWindow(), GLFW_KEY_4) == GLFW_PRESS) SetWeapon(4);

        if (glfwGetKey(game->GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) game->GetScene()->Pause();

        if (glfwGetKey(game->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
            yDir = -1;
        if (glfwGetKey(game->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
            yDir = 1;
        if (glfwGetKey(game->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
            xDir = -1;
        if (glfwGetKey(game->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
            xDir = 1;
        if (xDir != 0 && yDir != 00) m = 1.414f;
        float xD = x + (speed * xDir / m / 60.f);
        float yD = y + (speed * yDir / m / 60.f);
        if (game->GetMap()->isValid(roundf(xD), roundf(yD))) {
            x = xD;
            y = yD;
        }
        double* mPos = game->GetWorldMousePosition();
        LookAt(mPos[0], mPos[2]);
    }

    if(currentWeapon)currentWeapon->Update();
    if (weaponSwapFrames > 0) weaponSwapFrames--;
    
}

void Player::SetCurrentWeapon(Weapon* w){
    currentWeapon = w;
}

void Player::SetWeapon(int group){
    if (weaponSwapFrames != 0) return;
    Weapon* target = NULL;
    for (Weapon* weapon : weapons) {
        if (weapon->GetGroup() == group) {
            if (currentWeapon == NULL || currentWeapon->GetGroup() != group) {
                if (target == NULL || target->GetPriority() > weapon->GetPriority()) 
                    target = weapon;             
            }
            else {
                if (target == NULL || (target->GetPriority() > weapon->GetPriority() && weapon->GetPriority() > currentWeapon->GetPriority()))
                    target = weapon;
            }
        }
    }
    if (target == currentWeapon) {
        for (Weapon* weapon : weapons) {
            if (weapon->GetGroup() == group) {
                if (target == NULL) { target = weapon; }
                else if (target->GetPriority() > weapon->GetPriority()) target = weapon;
            }
        }
    }
    if (target != NULL) {
        weaponSwapFrames = 10;
        currentWeapon = target;
    }
}

void Player::AddWeapon(Weapon* weapon){
    if (hasWeapon(weapon)) return;
    weapons.push_back(weapon);
    currentWeapon = weapon;
}

void Player::TakeDamage(int damage){
    if (armor > 0) {
        damage -= armor;
        if (damage < 0) armor = -damage;
        else {
            health -= damage;
            armor = 0;
        }
    }
    else health -= damage;
}

bool Player::hasWeapon(Weapon* weapon){
    for (Weapon* w : weapons) {
        if (w->GetName() == weapon->GetName()) return true;
    }
    return false;
}

Weapon* Player::GetWeapon(Weapon* w){
    for (Weapon* e : weapons) {
        if (e->GetName() == w->GetName()) {
            return e;
        }
    }
    return nullptr;
}

void Player::SetHealth(int h){
    health = h;
}

int Player::AddHealth(int amount){
    health += amount;
    if (health > maxHealth) {
        int r = health - maxHealth + 1;
        health = maxHealth;
        return r;
    }
    else return 0;
}

int Player::AddArmor(int amount) {
    armor += amount;
    if (armor > maxArmor) {        
        armor = maxArmor;
        return 1;
    }
    else return 0;
}

void Player::SetArmor(int a){
    armor = a;
}

void Player::SetControl(bool c){
    hasControl = c;
}


void Player::OnLeftMouseClick(){
    if(currentWeapon && currentWeapon->canFire())
        currentWeapon->Shoot();
}

void Player::OnLeftMouseRelease(){
    if(currentWeapon) currentWeapon->Release();
}

void Player::OnRightMouseClick(){}

void Player::OnRightMouseRelease(){}
