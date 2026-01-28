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


public:
    // Ajoute ou remplace un état (move détruit automatiquement un état)
    void addState(StatesNames id, std::unique_ptr<State> newState) {
        states[id] = std::move(newState); 
    }

    // Change l'état actif avec vérification de sécurité
    void setCurrentState(StatesNames id) {
            currentState = id;
            hasCurrentState = true;
    }

    // Accesseur rapide pour l'état actuel
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

    // Supprimer un état pour libérer de la mémoire
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
};