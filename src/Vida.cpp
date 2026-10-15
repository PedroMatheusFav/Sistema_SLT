#include "Vida.hpp"

Vida::Vida(int maximo) {
    // A vida maxima nunca comeca invalida.
    this->maximo = (maximo > 0) ? maximo : 1;
    this->atual = this->maximo;
}

int Vida::getAtual() const {
    return atual;
}

int Vida::getMaximo() const {
    return maximo;
}

bool Vida::curar(int valor) {
    if (valor <= 0) {
        return false;
    }

    int anterior = atual;
    atual = atual + valor;

    // Regra: a vida nunca passa do maximo, o valor trava no teto.
    if (atual > maximo) {
        atual = maximo;
    }

    return atual > anterior;
}

bool Vida::dano(int valor) {
    if (valor <= 0) {
        return false;
    }

    atual = atual - valor;

    if (atual <= 0) {
        atual = 0;
        // Avisa quem chamou: a vida zerou, o personagem morreu.
        return true;
    }

    return false;
}

void Vida::aumentarMax(int valor) {
    if (valor <= 0) {
        return;
    }
    maximo = maximo + valor;
}

void Vida::diminuirMax(int valor) {
    if (valor <= 0) {
        return;
    }

    maximo = maximo - valor;
    if (maximo < 1) {
        maximo = 1;
    }

    // A vida atual nunca pode ficar acima do novo maximo.
    if (atual > maximo) {
        atual = maximo;
    }
}
