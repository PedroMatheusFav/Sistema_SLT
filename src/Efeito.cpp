#include "Efeito.hpp"

Efeito::Efeito(const std::string& nome) : nome(nome) {
}

std::string Efeito::getNome() const {
    return nome;
}

bool Efeito::operator<(const Efeito& outro) const {
    return nome < outro.nome;
}

bool Efeito::operator==(const Efeito& outro) const {
    return nome == outro.nome;
}
