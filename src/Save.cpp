#include "Save.hpp"

#include <string>

#include "Catalogo.hpp"
#include "Personagem.hpp"

Save::Save() : configurado(false), personagem("", 1, 1, 0) {
    // Todo save novo ja comeca com o mundo padrao da DESCRICAO.md.
    catalogo.carregarPadrao();
}

bool Save::foiConfigurado() const {
    return configurado;
}

bool Save::criarPersonagem(const std::string& nome, int vidaMax, int manaMax,
                           int dinheiroInicial) {
    if (configurado) {
        return false;  // 1 personagem por save
    }

    personagem = Personagem(nome, vidaMax, manaMax, dinheiroInicial);
    configurado = true;
    return true;
}

Catalogo& Save::getCatalogo() {
    return catalogo;
}

const Catalogo& Save::getCatalogo() const {
    return catalogo;
}

Personagem& Save::getPersonagem() {
    return personagem;
}

const Personagem& Save::getPersonagem() const {
    return personagem;
}
