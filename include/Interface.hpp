#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>

#include "Saves.hpp"

// Toda a leitura e escrita na tela fica aqui.
// As classes do sistema (Vida, Personagem, Catalogo...) nao conhecem
// cin/cout: esta classe le o que o usuario digita, chama a logica e mostra
// o resultado.
class Interface {
private:
    Saves saves;
    bool entradaAcabou;  // liga quando o getline falha (fim da entrada)

    // --- leitura robusta ---
    std::string lerLinha(const std::string& rotulo);
    int lerInteiro(const std::string& rotulo);  // so para os menus (0 = voltar)
    // Campos de uma acao. Devolvem false quando o usuario digita "0"
    // (cancela a acao) ou quando a entrada acaba.
    bool lerTexto(const std::string& rotulo, std::string& saida);
    bool lerNumero(const std::string& rotulo, int& saida);
    bool lerConfirmacao(const std::string& rotulo, bool& saida);
    // Se o texto de efeitos tiver curar/dano/recuperar, pergunta o valor.
    // Devolve false se o usuario cancelar; senao poe o valor em "saida".
    bool perguntarValorSePreciso(const std::string& textoDeEfeitos, int& saida);

    // --- telas ---
    void mostrarSaves() const;
    void excluirFicha();
    void abrirSaveAtivo();

    void painelDoMestre();
    void painelHabilidades();
    void painelItens();
    void painelEfeitos();
    void criarPersonagemNoSave();

    void menuDoJogador();
    void menuMorto();
    void mostrarStatus();
    void menuInventario();
    void menuVida();
    void menuMana();
    void menuHabilidades();
    void mostrarStatusHabilidade();
    void menuEfeitos();
    void menuDinheiro();

public:
    Interface();
    void executar();
};

#endif
