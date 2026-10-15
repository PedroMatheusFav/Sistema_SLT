#include "Mana.hpp"

Mana::Mana(int maximo) {
    this->maximo = (maximo > 0) ? maximo : 1;
    this->atual = this->maximo;
}

int Mana::getAtual() const {
    return atual;
}

int Mana::getMaximo() const {
    return maximo;
}

bool Mana::temMana(int custo) const {
    return atual >= custo;
}

bool Mana::recuperar(int valor) {
    if (valor <= 0) {
        return false;
    }

    int anterior = atual;
    atual = atual + valor;

    // Regra: a mana nunca passa do maximo.
    if (atual > maximo) {
        atual = maximo;
    }

    return atual > anterior;
}

bool Mana::gastar(int valor) {
    if (valor < 0) {
        return false;
    }

    if (!temMana(valor)) {
        // Falta mana: nao gasta nada e avisa quem chamou.
        return false;
    }

    atual = atual - valor;
    return true;
}

void Mana::aumentarMax(int valor) {
    if (valor <= 0) {
        return;
    }
    maximo = maximo + valor;
}

void Mana::diminuirMax(int valor) {
    if (valor <= 0) {
        return;
    }

    maximo = maximo - valor;
    if (maximo < 1) {
        maximo = 1;
    }

    if (atual > maximo) {
        atual = maximo;
    }
}
