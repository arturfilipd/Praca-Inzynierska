#include "Entity.h"
#include "Behaviour.h"
#include <vector>
Entity::Entity(){  
    rotation = glm::vec2(0.f, 1.f);
    ID = n++;
}
Entity::Entity(float x, float y){
    this->x = x;
    this->y = y;
}
Entity::Entity(float x, float y, Model* m){
    this->x = x;
    this->y = y;
    model = m;
}
int Entity::n = 0;
Entity::~Entity() {
    for (Behaviour* b : behaviours) {
        delete b;
    }
}

void Entity::Update(){ 
    pathingRequesred = false;
    if (framesToPathing > 0) framesToPathing--;
    destiPriority = -1;
    for (Behaviour* b : behaviours)
        b->Update();       
    if (path.size() != 0) {       
        float m = 1.4142f;
        LookAt(path.front().x, path.front().y);
        if (path.front().x == x || path.front().y == y) m = 1.f;
        float distance = speed / 60.f;
        float xD = abs(path.front().x - x);
        float yD = abs(path.front().y - y);   
        float xx = xD / (xD + yD); if (xx < 0.01) xx = 0;
        float yy = yD / (xD + yD); if (yy < 0.01) yy = 0;        
        (path.front().x > x) ? (x += distance * xx) : (x -= distance * xx);
        (path.front().y > y) ? (y += distance * yy) : (y -= distance * yy);   
        float minDist = (speed/60.f)* (speed / 60.f);
        if (minDist < 0.04f) minDist = 0.02f;
        if ((xD * xD) + (yD * yD) < (speed / 60.f) * (speed / 60.f))
            path.pop_front();
    }    
    if (pathingRequesred) framesToPathing = pathingIncrement;
}

Model* Entity::GetModel(){
    return model;
}

float Entity::GetX(){
    return  x;
}

float Entity::GetY(){
    return  y;
}

void Entity::SetGame(Game* g){
    this->game = g;
    for (Behaviour* b : behaviours)
        b->SetGame(g);
}

void Entity::SetDestination(Node dest, int prio) {
    if (prio >= destiPriority) {        
        if (!(dest.x == destx && dest.y == desty)) {
            if (framesToPathing == 0) {
                destx = dest.x;
                desty = dest.y;
                path.clear();
                destiPriority = prio;
                GetPath(dest);
            }            
        }        
    }
}

void Entity::AddBehaviour(Behaviour* b){
    b->SetGame(this->game);
    b->SetParent(this);
    behaviours.push_back(b);
}

void Entity::MarkForDeletion(){    
      toBeDeleted = true;
}

void Entity::GetPath(Node dest){    
    pathingRequesred = true;
    float ns = game->GetMap()->GetNodeSize();
    path.clear();
    Node pn;
    //Translating world space coordinates to A* nodes
    dest.x = floorf(dest.x / ns + 2);
    dest.y = floorf(dest.y / ns + 2);
    pn.x = floorf(x/ns+2);
    pn.y = floorf(y/ns+2);        
    vector<Node> p = game->GetMap()->FindPath(pn, dest);
    if (p.size() == 0) return;    
    for (Node n : p) {
        n.x = (n.x -2) * ns + ns/2;
        n.y = (n.y - 2) * ns + ns/2;
        path.push_back(n);      
    }
}

Node Entity::GetNode() {
    Node n;
    float ns = game->GetMap()->GetNodeSize();
    n.x = roundf(x/ns+2);
    n.y = roundf(y/ns+2);
    return n;
}
Node Entity::GetPosition() {
    Node n;
    n.x =(x);
    n.y =(y);
    return n;
}


void Entity::SetPosition(float x, float y){
    this->x = x;
    this->y = y;
}

void Entity::LookAt(float targetX, float targetY){    
    glm::vec2 r = { targetX - x, targetY - y };
    if (r.x == 0 && r.y == 0) {
        rotation = { 0.f, 0.f };
        return;
    }
    rotation = glm::normalize(r);    
}

void Entity::SetRotation(glm::vec2 rotation) {
    this->rotation = rotation;
}

void Entity::SetModel(Model* model){
    this->model = model;
}

void Entity::LookAt(Entity* target){
    LookAt(target->GetX(), target->GetY());
}

void Entity::OnDeath(){}

float Entity::GetDistance(Entity * e) {
    return sqrtf((x - e->GetX()) * (x - e->GetX()) + (y - e->GetY()) * (y - e->GetY()));
}

float Entity::GetDistance(Node n){
    return sqrtf((x - n.x) * (x - n.x) + (y - n.y) * (y - n.y));
}
