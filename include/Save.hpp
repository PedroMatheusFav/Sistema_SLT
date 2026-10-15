#ifndef SAVE_HPP
#define SAVE_HPP

#include <string>

#include "Catalogo.hpp"
#include "Personagem.hpp"

// Um dos 3 espacos de salvamento. Enquanto o mestre nao cria o personagem,
// o espaco esta vazio (configurado = false) e o catalogo comeca do zero.
class Save {
private:
    bool configurado;
    Catalogo catalogo;
    Personagem personagem;

public:
    // Espaco vazio. O personagem aqui e so um lugar reservado; some quando
    // criarPersonagem roda.
    Save();

    bool foiConfigurado() const;

    // Cria o unico personagem deste save. Falha se o save ja tem um.
    bool criarPersonagem(const std::string& nome, int vidaMax, int manaMax,
                         int dinheiroInicial);

    // O mestre usa o catalogo; o jogador usa o personagem.
    Catalogo& getCatalogo();
    const Catalogo& getCatalogo() const;

    Personagem& getPersonagem();
    const Personagem& getPersonagem() const;
};

#endif
