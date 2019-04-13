#include <iostream>
#include <stdlib.h>
#include <time.h>
#define TAM 10

	
using namespace std;

template <typename T>
class Node{

        public:
            T info;
            Node* next;


            Node(T info, Node* next){	
                this->info = info;
                this->next = next;
            }

            Node(T info){
                this->info = info;
                this->next = NULL;
            }
            Node(){
                this->next = NULL;
            }
};

template <class T>
class Fila{

	private:
		Node<T>* primeiro;
		Node<T>* ultimo;
		int tam;

	public:
		Fila();
		void Insere(T, bool&);
		void Retira(T&,bool&);
		bool Vazia() const;
		bool Cheia() const;
		Fila* embaralha();
		int getTam() const;
		Node<T>* getPrimeiro()const;

};

template <class T>
int Fila<T>::getTam() const{
	return this->tam;
}

template <class T>
Fila<T>::Fila(){
	this->primeiro = NULL;
	this->ultimo = NULL;
	this->tam = 0;
}

template <class T>
bool Fila<T>::Vazia() const{
	if(this->primeiro == NULL){
		return true;
	}else{
		return false;
	}
}

template <class T>
bool Fila<T>::Cheia() const{
	Node<T> *aux = new Node<T>();

    if(aux == NULL){
        delete(aux);
        return true;
    }else{
        delete(aux);
        return false;
    }
}

template <class T>
void Fila<T>::Insere(T elemento, bool& DeuCerto){
	Node<T> *aux;

    if(this->Cheia()){
    	DeuCerto = false;
	}else{
		 DeuCerto = true;
		 this->tam++;
		 aux = new Node<T>(elemento);
		 
		 if(this->Vazia()){
		 	this->primeiro = aux;
		 }else{
		 	this->ultimo->next = aux;
		 }
		 this->ultimo = aux;
	}
	
}

template <class T>
void Fila<T>::Retira(T& elemento, bool& DeuCerto){
	Node<T> *aux;

    if(this->Vazia()){
    	DeuCerto = false;
	}else{
		DeuCerto = true;
		this->tam--;
		elemento = this->primeiro->info;
		aux = this->primeiro;
		this->primeiro = this->primeiro->next;
		
		if(this->primeiro == NULL){
			this->ultimo = NULL;
			delete(aux);
		}
	}
	
}

template <class T>
Node<T>* Fila<T>::getPrimeiro() const{
	return this->primeiro;
}
template <class T>
Fila<T>* Fila<T>::embaralha(){
	
	int j = this->getTam();
	
    T vetor [j] ;
    int l = j;
    T temp;
    Fila <T> * aux = new Fila();

    int i=0;
    bool DeuCerto;

    while (!this->Vazia()){
    	
        this->Retira(temp,DeuCerto);
        vetor [i] = temp;
        i++;
    }
	
	srand(time(NULL));
	
    for(int i=0; i < j; i++){
    	int r = rand() % l;
    	
    	
    	aux->Insere(vetor[r],DeuCerto);
    	
    	vetor[r] = vetor[l-1];
    	l--;
	}

	return aux;
}




