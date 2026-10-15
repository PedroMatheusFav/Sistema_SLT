#ifndef SAVES_HPP
#define SAVES_HPP

#include <vector>

#include "Save.hpp"

// Organiza os 3 espacos de salvamento e marca qual esta ativo.
class Saves {
private:
    std::vector<Save> saves;  // 3 posicoes fixas, acessadas por indice
    int ativo;                // 0, 1 ou 2

public:
    Saves();

    int getQuantidade() const;
    int getAtivoIndice() const;

    Save& getAtivo();
    const Save& getAtivo() const;

    // Acesso por indice (0, 1, 2).
    Save& get(int indice);
    const Save& get(int indice) const;

    // Muda o espaco ativo. Falha com indice fora de 0..2.
    bool trocar(int indice);

    // Diz se o espaco ainda nao tem personagem.
    bool slotVazio(int indice) const;

    // Deseja apagar? Apaga o espaco inteiro: o catalogo volta ao padrao e o
    // personagem some. Quem pergunta "vivo/morto?" e a interface.
    bool excluir(int indice);
};

#endif
