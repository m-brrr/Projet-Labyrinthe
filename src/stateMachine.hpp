#pragma once

class StateMachine {
private:
    // Utiliser une map permet d'accéder à l'état par son ID sans utiliser de chiffres (un peu comme un dictionnaire python)
    std::map<StatesNames, std::unique_ptr<State>> states;
    StatesNames currentState;
	StatesNames newState;
    bool hasCurrentState = false;
	bool changeStateFlag = false;
	std::string persoChoisi="Child";
	float tileWidth = 150.f;
	int nbEnemis=5;


public:
    // Ajoute ou remplace un état (move détruit automatiquement un état)
    void addState(StatesNames id, std::unique_ptr<State> newState) {
        states[id] = std::move(newState); 
    }

    void setCurrentState(StatesNames id) {
            currentState = id;
            hasCurrentState = true;
    }

    State* getCurrent() {
        return hasCurrentState ? states[currentState].get() : nullptr;
    }
    
    void handleEvent() {
        if (auto* state = getCurrent()) {
            state->handleEvent();
        }
    }

    void update(float dt) {
        if (auto* state = getCurrent()) {
            state->update(dt);
        }
    }

    void render() {
        if (auto* state = getCurrent()) {
            state->render();
        }
    }

    void removeState(StatesNames id) {
        states.erase(id);
        if (currentState == id) hasCurrentState = false;
    }

	void changeStateRequest(StatesNames id){
		newState= id;
		changeStateFlag=true;
	}

	void changeStateApply(){
		if (changeStateFlag){
			setCurrentState(newState);
			changeStateFlag=false;
		}
	}

	void setPersoName(const std::string& name) { persoChoisi = name; }

	std::string getPersoName(){return persoChoisi;}

	void setNbEnemis(int nb){nbEnemis=nb;}

	int getNbEnemis(){return nbEnemis;}
	
	float getTileWidth(){return tileWidth;}
};