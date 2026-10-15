#include "Catalogo.hpp"

#include <map>
#include <set>
#include <string>

#include "Efeito.hpp"
#include "Habilidade.hpp"
#include "Item.hpp"

Catalogo::Catalogo() : finalizado(false) {
}

// O map ja esta em ordem de chave, entao basta comparar com 1, 2, 3...
// Na primeira chave diferente do esperado, achamos a tumba: esse e o id livre.
int Catalogo::proximoIdH() const {
    int esperado = 1;
    std::map<int, Habilidade>::const_iterator it = listaH.begin();
    while (it != listaH.end()) {
        if (it->first != esperado) {
            break;
        }
        ++esperado;
        ++it;
    }
    return esperado;
}

int Catalogo::proximoIdI() const {
    int esperado = 1;
    std::map<int, Item>::const_iterator it = listaI.begin();
    while (it != listaI.end()) {
        if (it->first != esperado) {
            break;
        }
        ++esperado;
        ++it;
    }
    return esperado;
}

bool Catalogo::nomeHabilidadeEmUso(const std::string& nome) const {
    return buscarH(nome) != 0;
}

bool Catalogo::nomeItemEmUso(const std::string& nome) const {
    return buscarI(nome) != 0;
}

bool Catalogo::addH(const std::string& nome, const std::string& areaAfetada,
                    const std::string& efeitoCausado,
                    const std::string& descricao, int custo) {
    if (finalizado) {
        return false;
    }
    if (nomeHabilidadeEmUso(nome)) {
        return false;
    }

    int id = proximoIdH();
    listaH.insert(std::make_pair(
        id, Habilidade(id, nome, areaAfetada, efeitoCausado, descricao, custo)));
    return true;
}

bool Catalogo::removerH(const std::string& nome) {
    if (finalizado) {
        return false;
    }

    const Habilidade* achou = buscarH(nome);
    if (achou == 0) {
        return false;
    }

    // Apago pela chave (o id). Depois disso o id vira uma tumba.
    listaH.erase(achou->getId());
    return true;
}

bool Catalogo::addI(const std::string& nome, int preco, int durabMax,
                    const std::string& descricao,
                    const std::string& efeitoCausado, bool remendavel) {
    if (finalizado) {
        return false;
    }
    if (nomeItemEmUso(nome)) {
        return false;
    }

    int id = proximoIdI();
    listaI.insert(std::make_pair(
        id, Item(id, nome, preco, durabMax, descricao, efeitoCausado,
                 remendavel)));
    return true;
}

bool Catalogo::removerI(const std::string& nome) {
    if (finalizado) {
        return false;
    }

    const Item* achou = buscarI(nome);
    if (achou == 0) {
        return false;
    }

    listaI.erase(achou->getId());
    return true;
}

bool Catalogo::addE(const std::string& nome) {
    if (finalizado) {
        return false;
    }

    // O set ja recusa repetido; insert avisa pelo segundo do par.
    std::pair<std::set<Efeito>::iterator, bool> resultado =
        listaE.insert(Efeito(nome));
    return resultado.second;
}

bool Catalogo::removerE(const std::string& nome) {
    if (finalizado) {
        return false;
    }

    // Para achar no set eu procuro um Efeito com esse nome.
    std::set<Efeito>::iterator achou = listaE.find(Efeito(nome));
    if (achou == listaE.end()) {
        return false;
    }

    listaE.erase(achou);
    return true;
}

const Habilidade* Catalogo::buscarH(int id) const {
    std::map<int, Habilidade>::const_iterator it = listaH.find(id);
    if (it == listaH.end()) {
        return 0;
    }
    // &it->second e o endereco do objeto guardado dentro do map.
    return &it->second;
}

const Habilidade* Catalogo::buscarH(const std::string& nome) const {
    std::map<int, Habilidade>::const_iterator it = listaH.begin();
    while (it != listaH.end()) {
        // operator== compara so o nome, entao o "#x " nao atrapalha.
        if (it->second == Habilidade(0, nome, "", "", "", 0)) {
            return &it->second;
        }
        ++it;
    }
    return 0;
}

const Item* Catalogo::buscarI(int id) const {
    std::map<int, Item>::const_iterator it = listaI.find(id);
    if (it == listaI.end()) {
        return 0;
    }
    return &it->second;
}

const Item* Catalogo::buscarI(const std::string& nome) const {
    std::map<int, Item>::const_iterator it = listaI.begin();
    while (it != listaI.end()) {
        if (it->second == Item(0, nome, 0, 0, "", "", false)) {
            return &it->second;
        }
        ++it;
    }
    return 0;
}

const std::map<int, Habilidade>& Catalogo::getListaH() const {
    return listaH;
}

const std::map<int, Item>& Catalogo::getListaI() const {
    return listaI;
}

const std::set<Efeito>& Catalogo::getListaE() const {
    return listaE;
}

void Catalogo::finalizar() {
    finalizado = true;
}

bool Catalogo::estaFinalizado() const {
    return finalizado;
}
