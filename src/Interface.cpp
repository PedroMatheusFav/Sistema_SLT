#include "Interface.hpp"

#include <cctype>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Catalogo.hpp"
#include "Efeito.hpp"
#include "Habilidade.hpp"
#include "Item.hpp"
#include "Personagem.hpp"
#include "Save.hpp"
#include "Saves.hpp"

// ---------------------------------------------------------------------------
// Funcoes de apoio (so o texto da tela)
// ---------------------------------------------------------------------------

static void aviso(const std::string& texto) {
    std::cout << "[!] " << texto << "\n";
}

static void erro(const std::string& texto) {
    std::cout << "[X] " << texto << "\n";
}

static std::string semEspacos(const std::string& texto) {
    std::string::size_type inicio = texto.find_first_not_of(" \t\r");
    if (inicio == std::string::npos) {
        return "";
    }
    std::string::size_type fim = texto.find_last_not_of(" \t\r");
    return texto.substr(inicio, fim - inicio + 1);
}

static std::string minusculas(const std::string& texto) {
    std::string resultado = texto;
    for (std::string::size_type i = 0; i < resultado.size(); ++i) {
        resultado[i] = static_cast<char>(
            std::tolower(static_cast<unsigned char>(resultado[i])));
    }
    return resultado;
}

// Tenta ler um numero inteiro do texto todo. false = nao e numero.
static bool textoParaInteiro(const std::string& texto, int& valor) {
    std::string t = semEspacos(texto);
    if (t.empty()) {
        return false;
    }

    std::string::size_type i = 0;
    bool negativo = false;
    if (t[0] == '+' || t[0] == '-') {
        negativo = (t[0] == '-');
        i = 1;
    }
    if (i >= t.size()) {
        return false;
    }

    int resultado = 0;
    while (i < t.size()) {
        if (t[i] < '0' || t[i] > '9') {
            return false;
        }
        resultado = resultado * 10 + (t[i] - '0');
        ++i;
    }

    valor = negativo ? -resultado : resultado;
    return true;
}

// O jogador pode digitar "#7 maca" ou so "maca". Isto tira o "#x ".
static std::string nomeSemId(const std::string& entrada) {
    std::string t = semEspacos(entrada);
    if (t.size() > 1 && t[0] == '#') {
        std::string::size_type i = 1;
        while (i < t.size() && t[i] >= '0' && t[i] <= '9') {
            ++i;
        }
        t = t.substr(i);
    }
    return semEspacos(t);
}

static const Item* itemDaBolsa(const Personagem& p, const std::string& nome) {
    const std::vector<Item>& itens = p.getItens();
    for (std::vector<Item>::size_type i = 0; i < itens.size(); ++i) {
        if (itens[i].getNome() == nome) {
            return &itens[i];
        }
    }
    return 0;
}

static const Habilidade* habilidadeDaFicha(const Personagem& p,
                                           const std::string& nome) {
    const std::vector<Habilidade>& lista = p.getHabilidades();
    for (std::vector<Habilidade>::size_type i = 0; i < lista.size(); ++i) {
        if (lista[i].getNome() == nome) {
            return &lista[i];
        }
    }
    return 0;
}

// ---------------------------------------------------------------------------
// Listagens
// ---------------------------------------------------------------------------

static void mostrarCatalogoH(const Catalogo& c) {
    const std::map<int, Habilidade>& lista = c.getListaH();
    if (lista.empty()) {
        std::cout << "  (vazio)\n";
        return;
    }
    std::map<int, Habilidade>::const_iterator it = lista.begin();
    while (it != lista.end()) {
        const Habilidade& h = it->second;
        std::cout << "  " << h.getEtiqueta() << " | area "
                  << h.getAreaAfetada() << " | custo " << h.getCusto()
                  << " MP | efeito: " << h.getEfeitoCausado() << " | "
                  << h.getDescricao() << "\n";
        ++it;
    }
}

static void mostrarCatalogoI(const Catalogo& c) {
    const std::map<int, Item>& lista = c.getListaI();
    if (lista.empty()) {
        std::cout << "  (vazio)\n";
        return;
    }
    std::map<int, Item>::const_iterator it = lista.begin();
    while (it != lista.end()) {
        const Item& item = it->second;
        std::cout << "  " << item.getEtiqueta() << " | $" << item.getPreco()
                  << " | durab " << item.getDurabMax()
                  << " | efeito: " << item.getEfeitoCausado() << " | "
                  << (item.getRemendavel() ? "remendavel" : "nao remendavel")
                  << " | " << item.getDescricao() << "\n";
        ++it;
    }
}

static void mostrarCatalogoE(const Catalogo& c) {
    const std::set<Efeito>& lista = c.getListaE();
    if (lista.empty()) {
        std::cout << "  (vazio)\n";
        return;
    }
    std::set<Efeito>::const_iterator it = lista.begin();
    while (it != lista.end()) {
        std::cout << "  - " << it->getNome() << "\n";
        ++it;
    }
}

static void mostrarBolsa(const Personagem& p) {
    const std::vector<Item>& itens = p.getItens();
    if (itens.empty()) {
        std::cout << "  (bolsa vazia)\n";
        return;
    }
    for (std::vector<Item>::size_type i = 0; i < itens.size(); ++i) {
        std::cout << "  " << itens[i].getEtiqueta() << " | durab "
                  << itens[i].getDurabAtual() << "/" << itens[i].getDurabMax()
                  << " | $" << itens[i].getPreco() << " | efeito: "
                  << itens[i].getEfeitoCausado() << "\n";
    }
}

static void mostrarFicha(const Personagem& p) {
    std::cout << "\n=== STATUS DA FICHA ===\n";
    std::cout << "Nome: " << p.getNome() << " ("
              << (p.estaVivo() ? "vivo" : "morto") << ")\n";
    std::cout << "Vida (HP): " << p.getVidaAtual() << "/" << p.getVidaMaxima()
              << "  |  Mana (MP): " << p.getManaAtual() << "/"
              << p.getManaMaxima() << "  |  Dinheiro: $"
              << p.getDinheiro().getQuantidade() << "\n";

    std::cout << "\n[ ITENS ]\n";
    const std::vector<Item>& itens = p.getItens();
    if (itens.empty()) {
        std::cout << "- (nenhum)\n";
    }
    for (std::vector<Item>::size_type i = 0; i < itens.size(); ++i) {
        std::cout << "- " << itens[i].getNome() << " (Durabilidade "
                  << itens[i].getDurabAtual() << "/" << itens[i].getDurabMax()
                  << ")\n";
    }

    std::cout << "\n[ HABILIDADES ]\n";
    const std::vector<Habilidade>& hs = p.getHabilidades();
    if (hs.empty()) {
        std::cout << "- (nenhuma)\n";
    }
    for (std::vector<Habilidade>::size_type i = 0; i < hs.size(); ++i) {
        std::cout << "- " << hs[i].getEtiqueta() << " (Custo: "
                  << hs[i].getCusto() << " MP | Area Afetada: "
                  << hs[i].getAreaAfetada() << ")\n";
    }

    std::cout << "\n[ EFEITOS ]\n";
    const std::list<Efeito>& ef = p.getEfeitos();
    if (ef.empty()) {
        std::cout << "- (nenhum)\n";
    }
    std::list<Efeito>::const_iterator it = ef.begin();
    while (it != ef.end()) {
        std::cout << "- " << it->getNome() << "\n";
        ++it;
    }
    std::cout << "=======================\n";
}

// ---------------------------------------------------------------------------
// Interface
// ---------------------------------------------------------------------------

Interface::Interface() : entradaAcabou(false) {
}

std::string Interface::lerLinha(const std::string& rotulo) {
    std::cout << rotulo;
    std::string linha;
    if (!std::getline(std::cin, linha)) {
        entradaAcabou = true;
        std::cout << "\n[!] fim da entrada. Encerrando.\n";
        return "";
    }
    return linha;
}

int Interface::lerInteiro(const std::string& rotulo) {
    while (!entradaAcabou) {
        std::string linha = lerLinha(rotulo);
        if (entradaAcabou) {
            return 0;
        }
        int valor;
        if (textoParaInteiro(linha, valor)) {
            return valor;
        }
        erro("'" + semEspacos(linha) + "' nao e um numero. Tente de novo.");
    }
    return 0;
}

bool Interface::lerSimNao(const std::string& rotulo) {
    while (!entradaAcabou) {
        std::string linha = lerLinha(rotulo);
        if (entradaAcabou) {
            return false;
        }
        std::string t = minusculas(semEspacos(linha));
        if (t == "s" || t == "sim") {
            return true;
        }
        if (t == "n" || t == "nao") {
            return false;
        }
        erro("responda 's' ou 'n'.");
    }
    return false;
}

int Interface::perguntarValorSePreciso(const std::string& textoDeEfeitos) {
    if (Personagem::contemEfeito(textoDeEfeitos, "curar") ||
        Personagem::contemEfeito(textoDeEfeitos, "dano") ||
        Personagem::contemEfeito(textoDeEfeitos, "recuperar")) {
        return lerInteiro("Valor do efeito: ");
    }
    return 0;
}

void Interface::executar() {
    std::cout << "\n=== INVENTARIO DE RPG ===\n";

    while (!entradaAcabou) {
        mostrarSaves();
        int op = lerInteiro("Escolha: ");
        if (entradaAcabou) {
            break;
        }

        if (op == 0) {
            break;
        }
        if (op == 4) {
            excluirFicha();
            continue;
        }
        if (op >= 1 && op <= saves.getQuantidade()) {
            saves.trocar(op - 1);
            abrirSaveAtivo();
        } else {
            erro("opcao invalida.");
        }
    }

    std::cout << "\nAte a proxima!\n";
}

void Interface::mostrarSaves() const {
    std::cout << "\n=== SALVAMENTO ===\n";
    for (int i = 0; i < saves.getQuantidade(); ++i) {
        std::cout << (i + 1) << ". Save " << (i + 1) << " - ";
        if (saves.slotVazio(i)) {
            std::cout << "vazio\n";
        } else {
            const Personagem& p = saves.get(i).getPersonagem();
            std::cout << p.getNome() << " ("
                      << (p.estaVivo() ? "vivo" : "morto") << " | "
                      << p.getVidaAtual() << "/" << p.getVidaMaxima() << ")\n";
        }
    }
    std::cout << "4. Excluir uma ficha\n";
    std::cout << "0. Sair\n";
}

void Interface::excluirFicha() {
    int numero = lerInteiro("Numero do save (1 a 3): ");
    if (entradaAcabou) {
        return;
    }
    if (numero < 1 || numero > saves.getQuantidade()) {
        erro("save invalido.");
        return;
    }

    int indice = numero - 1;
    if (saves.slotVazio(indice)) {
        aviso("esse save ja esta vazio.");
        return;
    }

    const Personagem& p = saves.get(indice).getPersonagem();
    std::string pergunta = "deseja apagar a ficha de " + p.getNome() + " " +
                           (p.estaVivo() ? "vivo?" : "morto?") + " (s/n): ";
    if (!lerSimNao(pergunta)) {
        aviso("nada foi apagado.");
        return;
    }

    saves.excluir(indice);
    aviso("ficha apagada. O mundo daquele save voltou ao padrao.");
}

void Interface::abrirSaveAtivo() {
    Save& save = saves.getAtivo();
    // Se o mundo ainda nao foi finalizado, o mestre continua montando.
    if (!save.foiConfigurado() || !save.getCatalogo().estaFinalizado()) {
        painelDoMestre();
    }
    if (entradaAcabou) {
        return;
    }
    // Se o mestre acabou de finalizar, ja cai no menu do jogador.
    if (save.foiConfigurado() && save.getCatalogo().estaFinalizado()) {
        menuDoJogador();
    }
}

// ---------------------------------------------------------------------------
// Mestre
// ---------------------------------------------------------------------------

void Interface::painelDoMestre() {
    Save& save = saves.getAtivo();

    while (!entradaAcabou) {
        std::cout << "\n--- PAINEL DO MESTRE (mundo novo) ---\n";
        std::cout << "1. Habilidades\n";
        std::cout << "2. Itens\n";
        std::cout << "3. Efeitos\n";
        std::cout << "4. Criar personagem\n";
        std::cout << "5. Finalizar mundo\n";
        std::cout << "0. Voltar\n";

        int op = lerInteiro("Escolha: ");
        if (entradaAcabou) {
            return;
        }

        if (op == 0) {
            return;
        } else if (op == 1) {
            painelHabilidades();
        } else if (op == 2) {
            painelItens();
        } else if (op == 3) {
            painelEfeitos();
        } else if (op == 4) {
            criarPersonagemNoSave();
        } else if (op == 5) {
            if (!save.foiConfigurado()) {
                erro("crie um personagem antes de finalizar o mundo.");
                continue;
            }
            save.getCatalogo().finalizar();
            aviso("Mundo finalizado. Bem-vindo, " +
                  save.getPersonagem().getNome() + "!");
            return;
        } else {
            erro("opcao invalida.");
        }
    }
}

void Interface::criarPersonagemNoSave() {
    Save& save = saves.getAtivo();
    if (save.foiConfigurado()) {
        erro("esse save ja tem um personagem.");
        return;
    }

    std::string nome = lerLinha("Nome: ");
    int vidaMax = lerInteiro("Vida MAX: ");
    int manaMax = lerInteiro("Mana MAX: ");
    int dinheiro = lerInteiro("Dinheiro inicial: ");
    if (entradaAcabou) {
        return;
    }

    if (semEspacos(nome).empty()) {
        erro("o nome nao pode ficar vazio.");
        return;
    }
    if (vidaMax < 1 || manaMax < 1 || dinheiro < 0) {
        erro("valores invalidos (vida e mana >= 1, dinheiro >= 0).");
        return;
    }

    save.criarPersonagem(semEspacos(nome), vidaMax, manaMax, dinheiro);
    aviso("Personagem '" + semEspacos(nome) + "' criado.");
}

void Interface::painelHabilidades() {
    Catalogo& c = saves.getAtivo().getCatalogo();

    while (!entradaAcabou) {
        std::cout << "\n--- HABILIDADES (mestre) ---\n";
        std::cout << "1. Adicionar\n2. Remover\n3. Listar\n0. Voltar\n";
        int op = lerInteiro("Escolha: ");
        if (entradaAcabou) {
            return;
        }

        if (op == 0) {
            return;
        } else if (op == 1) {
            std::string nome = lerLinha("Nome: ");
            std::string area = lerLinha("Area afetada: ");
            std::string efeito =
                lerLinha("Efeito causado (varios separados por ';'): ");
            std::string descricao = lerLinha("Descricao: ");
            int custo = lerInteiro("Custo (mana): ");
            if (entradaAcabou) {
                return;
            }
            if (c.addH(nome, area, efeito, descricao, custo)) {
                aviso("habilidade adicionada.");
            } else {
                erro("nome repetido ou mundo ja finalizado.");
            }
        } else if (op == 2) {
            std::string nome = lerLinha("Nome: ");
            if (entradaAcabou) {
                return;
            }
            if (c.removerH(nome)) {
                aviso("habilidade removida (virou tumba).");
            } else {
                erro("nao existe habilidade com esse nome.");
            }
        } else if (op == 3) {
            mostrarCatalogoH(c);
        } else {
            erro("opcao invalida.");
        }
    }
}

void Interface::painelItens() {
    Catalogo& c = saves.getAtivo().getCatalogo();

    while (!entradaAcabou) {
        std::cout << "\n--- ITENS (mestre) ---\n";
        std::cout << "1. Adicionar\n2. Remover\n3. Listar\n0. Voltar\n";
        int op = lerInteiro("Escolha: ");
        if (entradaAcabou) {
            return;
        }

        if (op == 0) {
            return;
        } else if (op == 1) {
            std::string nome = lerLinha("Nome: ");
            int preco = lerInteiro("Preco: ");
            int durabMax = lerInteiro("Durabilidade maxima: ");
            std::string descricao = lerLinha("Descricao: ");
            std::string efeito =
                lerLinha("Efeito causado (varios separados por ';'): ");
            bool remendavel = lerSimNao("Remendavel? (s/n): ");
            if (entradaAcabou) {
                return;
            }
            if (c.addI(nome, preco, durabMax, descricao, efeito, remendavel)) {
                aviso("item adicionado.");
            } else {
                erro("nome repetido ou mundo ja finalizado.");
            }
        } else if (op == 2) {
            std::string nome = lerLinha("Nome: ");
            if (entradaAcabou) {
                return;
            }
            if (c.removerI(nome)) {
                aviso("item removido (virou tumba).");
            } else {
                erro("nao existe item com esse nome.");
            }
        } else if (op == 3) {
            mostrarCatalogoI(c);
        } else {
            erro("opcao invalida.");
        }
    }
}

void Interface::painelEfeitos() {
    Catalogo& c = saves.getAtivo().getCatalogo();

    while (!entradaAcabou) {
        std::cout << "\n--- EFEITOS (mestre) ---\n";
        std::cout << "1. Adicionar\n2. Remover\n3. Listar\n0. Voltar\n";
        int op = lerInteiro("Escolha: ");
        if (entradaAcabou) {
            return;
        }

        if (op == 0) {
            return;
        } else if (op == 1) {
            std::string nome = lerLinha("Nome: ");
            if (entradaAcabou) {
                return;
            }
            if (c.addE(nome)) {
                aviso("efeito adicionado.");
            } else {
                erro("efeito repetido ou mundo ja finalizado.");
            }
        } else if (op == 2) {
            std::string nome = lerLinha("Nome: ");
            if (entradaAcabou) {
                return;
            }
            if (c.removerE(nome)) {
                aviso("efeito removido.");
            } else {
                erro("nao existe efeito com esse nome.");
            }
        } else if (op == 3) {
            mostrarCatalogoE(c);
        } else {
            erro("opcao invalida.");
        }
    }
}

// ---------------------------------------------------------------------------
// Jogador
// ---------------------------------------------------------------------------

void Interface::menuDoJogador() {
    while (!entradaAcabou) {
        // Regra 9: morto so tem a opcao de reviver.
        if (!saves.getAtivo().getPersonagem().estaVivo()) {
            menuMorto();
            if (!saves.getAtivo().getPersonagem().estaVivo()) {
                return;  // desistiu de reviver: volta aos saves
            }
            continue;  // revivido
        }

        std::cout << "\n=== MENU DO JOGADOR ===\n";
        std::cout << "1. Status do personagem\n";
        std::cout << "2. Inventario e Loja\n";
        std::cout << "3. Habilidades\n";
        std::cout << "4. Efeitos\n";
        std::cout << "5. Dinheiro\n";
        std::cout << "6. Trocar de personagem\n";
        std::cout << "0. Voltar aos saves\n";

        int op = lerInteiro("Escolha: ");
        if (entradaAcabou) {
            return;
        }

        if (op == 0 || op == 6) {
            return;  // a tela de salvamento e quem troca de personagem
        } else if (op == 1) {
            mostrarStatus();
        } else if (op == 2) {
            menuInventario();
        } else if (op == 3) {
            menuHabilidades();
        } else if (op == 4) {
            menuEfeitos();
        } else if (op == 5) {
            menuDinheiro();
        } else {
            erro("opcao invalida.");
        }
    }
}

void Interface::menuMorto() {
    Save& save = saves.getAtivo();
    Personagem& p = save.getPersonagem();

    while (!entradaAcabou && !p.estaVivo()) {
        std::cout << "\n--- " << p.getNome() << " ESTA MORTO ---\n";
        std::cout << "1. Usar item de reviver\n";
        std::cout << "0. Voltar aos saves\n";

        int op = lerInteiro("Escolha: ");
        if (entradaAcabou) {
            return;
        }

        if (op == 0) {
            return;
        }
        if (op != 1) {
            erro("opcao invalida.");
            continue;
        }

        // Mostra so os itens de reviver da bolsa.
        const std::vector<Item>& itens = p.getItens();
        bool temReviver = false;
        for (std::vector<Item>::size_type i = 0; i < itens.size(); ++i) {
            if (Personagem::contemEfeito(itens[i].getEfeitoCausado(),
                                         "reviver")) {
                std::cout << "  " << itens[i].getEtiqueta() << "\n";
                temReviver = true;
            }
        }
        if (!temReviver) {
            erro("voce nao tem item de reviver.");
            continue;
        }

        std::string nome = lerLinha("Item de reviver: ");
        if (entradaAcabou) {
            return;
        }

        std::string mensagem;
        if (p.usarItem(nomeSemId(nome), 0, save.getCatalogo(), mensagem)) {
            aviso(mensagem);
        } else {
            erro(mensagem);
        }
    }
}

void Interface::mostrarStatus() {
    mostrarFicha(saves.getAtivo().getPersonagem());
    lerLinha("Pressione ENTER para voltar ao menu...");
}

void Interface::menuInventario() {
    Save& save = saves.getAtivo();
    Personagem& p = save.getPersonagem();

    while (!entradaAcabou) {
        std::cout << "\n--- INVENTARIO E LOJA ---\n";
        std::cout << "1. Comprar\n2. Vender\n3. Usar item\n";
        std::cout << "4. Remendar\n5. Jogar no lixo\n6. Ver bolsa\n0. Voltar\n";

        int op = lerInteiro("Escolha: ");
        if (entradaAcabou) {
            return;
        }

        if (op == 0) {
            return;
        } else if (op == 1) {
            mostrarCatalogoI(save.getCatalogo());
            std::string entrada = lerLinha("Item (ex.: #7 maca): ");
            if (entradaAcabou) {
                return;
            }
            const Item* item =
                save.getCatalogo().buscarI(nomeSemId(entrada));
            if (item == 0) {
                erro("esse item nao existe no catalogo.");
            } else if (p.comprar(*item)) {
                aviso("comprou " + item->getEtiqueta() + " por $" +
                      std::to_string(item->getPreco()) + ".");
            } else {
                erro("falta dinheiro para comprar " + item->getEtiqueta() +
                     " ($" + std::to_string(item->getPreco()) + ").");
            }
        } else if (op == 2) {
            mostrarBolsa(p);
            std::string entrada = lerLinha("Item: ");
            if (entradaAcabou) {
                return;
            }
            if (p.vender(nomeSemId(entrada))) {
                aviso("item vendido.");
            } else {
                erro("voce nao tem esse item.");
            }
        } else if (op == 3) {
            mostrarBolsa(p);
            std::string entrada = lerLinha("Item: ");
            if (entradaAcabou) {
                return;
            }
            std::string nome = nomeSemId(entrada);
            const Item* naBolsa = itemDaBolsa(p, nome);
            int valor = 0;
            if (naBolsa != 0) {
                valor = perguntarValorSePreciso(naBolsa->getEfeitoCausado());
            }
            if (entradaAcabou) {
                return;
            }
            std::string mensagem;
            if (p.usarItem(nome, valor, save.getCatalogo(), mensagem)) {
                aviso(mensagem);
            } else {
                erro(mensagem);
            }
        } else if (op == 4) {
            mostrarBolsa(p);
            std::string entrada = lerLinha("Item: ");
            int valorPago = lerInteiro("Valor pago: ");
            if (entradaAcabou) {
                return;
            }
            if (p.remendar(nomeSemId(entrada), valorPago)) {
                aviso("item remendado.");
            } else {
                erro("nao deu: item inexistente, nao remendavel ou "
                     "faltou dinheiro.");
            }
        } else if (op == 5) {
            mostrarBolsa(p);
            std::string entrada = lerLinha("Item: ");
            if (entradaAcabou) {
                return;
            }
            if (p.jogarNoLixo(nomeSemId(entrada))) {
                aviso("item jogado no lixo.");
            } else {
                erro("voce nao tem esse item.");
            }
        } else if (op == 6) {
            mostrarBolsa(p);
        } else {
            erro("opcao invalida.");
        }
    }
}

void Interface::menuHabilidades() {
    Save& save = saves.getAtivo();
    Personagem& p = save.getPersonagem();

    while (!entradaAcabou) {
        std::cout << "\n--- HABILIDADES ---\n";
        std::cout << "1. Usar habilidade\n2. Modificar habilidade\n";
        std::cout << "3. Remover habilidade\n4. Status de habilidade\n";
        std::cout << "5. Aprender habilidade\n0. Voltar\n";

        int op = lerInteiro("Escolha: ");
        if (entradaAcabou) {
            return;
        }

        if (op == 0) {
            return;
        } else if (op == 1) {
            const std::vector<Habilidade>& lista = p.getHabilidades();
            if (lista.empty()) {
                aviso("voce ainda nao tem habilidades.");
                continue;
            }
            for (std::vector<Habilidade>::size_type i = 0; i < lista.size();
                 ++i) {
                std::cout << "  " << lista[i].getEtiqueta() << " (custo "
                          << lista[i].getCusto() << " MP | efeito: "
                          << lista[i].getEfeitoCausado() << ")\n";
            }
            std::string entrada = lerLinha("Habilidade: ");
            if (entradaAcabou) {
                return;
            }
            std::string nome = nomeSemId(entrada);
            const Habilidade* naFicha = habilidadeDaFicha(p, nome);
            int valor = 0;
            if (naFicha != 0) {
                valor = perguntarValorSePreciso(naFicha->getEfeitoCausado());
            }
            if (entradaAcabou) {
                return;
            }
            std::string mensagem;
            if (p.usarHabilidade(nome, valor, mensagem)) {
                aviso(mensagem);
            } else {
                erro(mensagem);
            }
        } else if (op == 2) {
            const std::vector<Habilidade>& lista = p.getHabilidades();
            for (std::vector<Habilidade>::size_type i = 0; i < lista.size();
                 ++i) {
                std::cout << "  " << lista[i].getEtiqueta() << "\n";
            }
            std::string entrada = lerLinha("Habilidade a modificar: ");
            if (entradaAcabou) {
                return;
            }
            std::string nome = nomeSemId(entrada);
            const Habilidade* antiga = habilidadeDaFicha(p, nome);
            if (antiga == 0) {
                erro("voce nao tem essa habilidade.");
                continue;
            }
            int id = antiga->getId();

            std::string novoNome = lerLinha("Novo nome: ");
            std::string novaArea = lerLinha("Nova area afetada: ");
            std::string novoEfeito =
                lerLinha("Novo efeito causado (separe por ';'): ");
            std::string novaDescricao = lerLinha("Nova descricao: ");
            int novoCusto = lerInteiro("Novo custo (mana): ");
            if (entradaAcabou) {
                return;
            }

            if (p.modificarHabilidade(
                    nome, Habilidade(id, novoNome, novaArea, novoEfeito,
                                     novaDescricao, novoCusto))) {
                aviso("habilidade modificada (so na sua ficha).");
            } else {
                erro("nao deu para modificar.");
            }
        } else if (op == 3) {
            std::string entrada = lerLinha("Habilidade a remover: ");
            if (entradaAcabou) {
                return;
            }
            if (p.removerHabilidade(nomeSemId(entrada))) {
                aviso("habilidade removida da ficha.");
            } else {
                erro("voce nao tem essa habilidade.");
            }
        } else if (op == 4) {
            mostrarStatusHabilidade();
        } else if (op == 5) {
            mostrarCatalogoH(save.getCatalogo());
            std::string entrada =
                lerLinha("Habilidade (ex.: #3 envenenamento em area): ");
            if (entradaAcabou) {
                return;
            }
            const Habilidade* doCatalogo =
                save.getCatalogo().buscarH(nomeSemId(entrada));
            if (doCatalogo == 0) {
                erro("essa habilidade nao existe no catalogo.");
            } else if (p.aprender(*doCatalogo)) {
                aviso("aprendeu " + doCatalogo->getEtiqueta() + ".");
            } else {
                erro("voce ja conhece essa habilidade.");
            }
        } else {
            erro("opcao invalida.");
        }
    }
}

void Interface::mostrarStatusHabilidade() {
    Personagem& p = saves.getAtivo().getPersonagem();

    std::string entrada = lerLinha("Habilidade: ");
    if (entradaAcabou) {
        return;
    }

    const Habilidade* h = habilidadeDaFicha(p, nomeSemId(entrada));
    if (h == 0) {
        erro("voce nao tem essa habilidade.");
        return;
    }

    std::cout << "Nome: " << h->getEtiqueta() << "\n";
    std::cout << "Area afetada: " << h->getAreaAfetada() << "\n";
    std::cout << "Efeito causado: " << h->getEfeitoCausado() << "\n";
    std::cout << "Descricao: " << h->getDescricao() << "\n";
    std::cout << "Custo: " << h->getCusto() << " MP\n";
}

void Interface::menuEfeitos() {
    Personagem& p = saves.getAtivo().getPersonagem();

    while (!entradaAcabou) {
        std::cout << "\n--- EFEITOS ---\n";
        std::cout << "1. Ver efeitos\n2. Limpar efeitos\n0. Voltar\n";

        int op = lerInteiro("Escolha: ");
        if (entradaAcabou) {
            return;
        }

        if (op == 0) {
            return;
        } else if (op == 1) {
            const std::list<Efeito>& ef = p.getEfeitos();
            if (ef.empty()) {
                aviso("nenhum efeito ativo.");
                continue;
            }
            int n = 1;
            std::list<Efeito>::const_iterator it = ef.begin();
            while (it != ef.end()) {
                std::cout << n << ". " << it->getNome() << "\n";
                ++n;
                ++it;
            }
        } else if (op == 2) {
            const std::list<Efeito>& ef = p.getEfeitos();
            if (ef.empty()) {
                aviso("nenhum efeito ativo.");
                continue;
            }
            int total = static_cast<int>(ef.size());
            int n = 1;
            std::list<Efeito>::const_iterator it = ef.begin();
            while (it != ef.end()) {
                std::cout << n << ". " << it->getNome() << "\n";
                ++n;
                ++it;
            }

            // Le os numeros ate digitar 'n' ou acabar a lista.
            std::vector<int> tirar;
            while (!entradaAcabou &&
                   static_cast<int>(tirar.size()) < total) {
                std::string linha = lerLinha(
                    "Numero para tirar (ou 'n' para parar): ");
                if (entradaAcabou) {
                    return;
                }
                std::string t = minusculas(semEspacos(linha));
                if (t == "n" || t == "nao") {
                    break;
                }
                int pos;
                if (textoParaInteiro(linha, pos)) {
                    tirar.push_back(pos);
                } else {
                    erro("digite um numero ou 'n'.");
                }
            }

            if (tirar.empty()) {
                aviso("nenhum efeito tirado.");
            } else if (p.limparEfeitos(tirar)) {
                aviso("efeitos removidos.");
            } else {
                erro("nenhum numero valido.");
            }
        } else {
            erro("opcao invalida.");
        }
    }
}

void Interface::menuDinheiro() {
    Personagem& p = saves.getAtivo().getPersonagem();

    while (!entradaAcabou) {
        std::cout << "\n--- DINHEIRO ---\n";
        std::cout << "1. Gastar\n2. Receber\n3. Ver extrato\n0. Voltar\n";

        int op = lerInteiro("Escolha: ");
        if (entradaAcabou) {
            return;
        }

        if (op == 0) {
            return;
        } else if (op == 1) {
            int valor = lerInteiro("Valor: ");
            if (entradaAcabou) {
                return;
            }
            if (p.gastarDinheiro(valor)) {
                aviso("gastou $" + std::to_string(valor) + ".");
            } else {
                erro("nao tem esse dinheiro.");
            }
        } else if (op == 2) {
            int valor = lerInteiro("Valor: ");
            if (entradaAcabou) {
                return;
            }
            p.receberDinheiro(valor);
            aviso("recebeu $" + std::to_string(valor) + ".");
        } else if (op == 3) {
            const std::vector<std::string>& extrato =
                p.getDinheiro().getExtrato();
            if (extrato.empty()) {
                aviso("extrato vazio.");
                continue;
            }
            for (std::vector<std::string>::size_type i = 0; i < extrato.size();
                 ++i) {
                std::cout << "  " << (i + 1) << ". " << extrato[i] << "\n";
            }
        } else {
            erro("opcao invalida.");
        }
    }
}
