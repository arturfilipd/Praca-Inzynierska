#include "Entity.h"
#include "Behaviour.h"
#include <vector>
Entity::Entity(){     
    ID = n++;
}
Entity::Entity(float x, float y){
    this->x = x;
    this->y = y;
    ID = n++;
    name = "Generic Entity";
}
Entity::Entity(float x, float y, Model* m){
    this->x = x;
    this->y = y;
    model = m;
    ID = n++;
    name = "Generic Entity";
}
int Entity::n = 0;
Entity::~Entity() {
    for (Behaviour* b : behaviours) {
        delete b;
    }
}

void Entity::Update(){ 
    pathingRequested = false;
    if (timeToPathing > 0) timeToPathing -= game->deltaTime();
    destiPriority = -1;
    for (Behaviour* b : behaviours)
        b->Update();       
    if (path.size() != 0) {  
        LookAt(path.front().x, path.front().y);
        float distanceToGo = game->deltaTime() * speed;
        while (distanceToGo > 0 && !path.empty()) {
            if (distanceToGo < GetDistance(path.front())) {
                float dX = (distanceToGo / GetDistance(path.front())) * (path.front().x - x);
                float dY = (distanceToGo / GetDistance(path.front())) * (path.front().y - y);
                x += dX;
                y += dY;
                distanceToGo = 0;
            }
            else {
                distanceToGo -= GetDistance(path.front());
                x = path.front().x;
                y = path.front().y;                
                path.pop_front();
            }
        }        
    }    
    if (pathingRequested)timeToPathing = pathingIncrement;
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
    if (timeToPathing > 0) return;
    if (prio >= destiPriority) {        
        if (!(round(dest.x) == round(destx) && round(dest.y) == round(desty))) {            
                destx = dest.x;
                desty = dest.y;
                path.clear();
                destiPriority = prio;
                GetPath(dest);                      
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
    pathingRequested = true;
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
