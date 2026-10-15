#include "Habilidade.hpp"

#include <string>

Habilidade::Habilidade(int id, const std::string& nome,
                       const std::string& areaAfetada,
                       const std::string& efeitoCausado,
                       const std::string& descricao, int custo)
    : id(id),
      nome(nome),
      areaAfetada(areaAfetada),
      efeitoCausado(efeitoCausado),
      descricao(descricao),
      custo(custo) {
}

int Habilidade::getId() const {
    return id;
}

std::string Habilidade::getNome() const {
    return nome;
}

std::string Habilidade::getAreaAfetada() const {
    return areaAfetada;
}

std::string Habilidade::getEfeitoCausado() const {
    return efeitoCausado;
}

std::string Habilidade::getDescricao() const {
    return descricao;
}

int Habilidade::getCusto() const {
    return custo;
}

std::string Habilidade::getEtiqueta() const {
    return "#" + std::to_string(id) + " " + nome;
}

// Compara so o nome: assim o "#x " nao atrapalha a ordem nem a busca.
bool Habilidade::operator<(const Habilidade& outro) const {
    return nome < outro.nome;
}

bool Habilidade::operator==(const Habilidade& outro) const {
    return nome == outro.nome;
}
