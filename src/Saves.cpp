#include "Saves.hpp"

#include "Save.hpp"

Saves::Saves() : saves(3), ativo(0) {
}

int Saves::getQuantidade() const {
    return static_cast<int>(saves.size());
}

int Saves::getAtivoIndice() const {
    return ativo;
}

Save& Saves::getAtivo() {
    return saves[ativo];
}

const Save& Saves::getAtivo() const {
    return saves[ativo];
}

Save& Saves::get(int indice) {
    return saves[indice];
}

const Save& Saves::get(int indice) const {
    return saves[indice];
}

bool Saves::trocar(int indice) {
    if (indice < 0 || indice >= getQuantidade()) {
        return false;
    }

    ativo = indice;
    return true;
}

bool Saves::slotVazio(int indice) const {
    if (indice < 0 || indice >= getQuantidade()) {
        return true;
    }

    return !saves[indice].foiConfigurado();
}

bool Saves::excluir(int indice) {
    if (indice < 0 || indice >= getQuantidade()) {
        return false;
    }

    // Reconstroi o espaco do zero: catalogo padrao e sem personagem.
    saves[indice] = Save();
    return true;
}
