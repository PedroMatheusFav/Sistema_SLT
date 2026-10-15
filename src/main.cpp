#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Catalogo.hpp"
#include "Dinheiro.hpp"
#include "Efeito.hpp"
#include "Habilidade.hpp"
#include "Item.hpp"
#include "Mana.hpp"
#include "Personagem.hpp"
#include "Save.hpp"
#include "Saves.hpp"
#include "Vida.hpp"

// main TEMPORARIO: so serve para testar as classes das etapas 1 e 2.
// Na etapa 6 ele vira o main final, que so cria a Interface.
// Aqui pode usar cout porque este e o ponto de entrada, nao uma classe.

static void mostrarVida(const std::string& rotulo, const Vida& v) {
    std::cout << rotulo << ": " << v.getAtual() << "/" << v.getMaximo()
              << "\n";
}

static void testarVida() {
    std::cout << "=== VIDA ===\n";

    Vida v(100);

    std::cout << "dano 30  -> zerou? " << (v.dano(30) ? "sim" : "nao") << "\n";
    mostrarVida("depois do dano", v);

    v.curar(500);
    std::cout << "curar 500 (trava no maximo)\n";
    mostrarVida("depois da cura", v);

    v.aumentarMax(20);
    mostrarVida("aumentarMax 20", v);

    v.diminuirMax(400);
    mostrarVida("diminuirMax 400 (maximo trava em 1)", v);

    std::cout << "dano 99  -> zerou? " << (v.dano(99) ? "sim" : "nao") << "\n";
    mostrarVida("depois do dano", v);
}

static void testarMana() {
    std::cout << "\n=== MANA ===\n";

    Mana m(50);

    std::cout << "temMana(15)? " << (m.temMana(15) ? "sim" : "nao") << "\n";
    std::cout << "gastar 15 -> " << (m.gastar(15) ? "ok" : "falhou") << "\n";
    std::cout << "mana: " << m.getAtual() << "/" << m.getMaximo() << "\n";

    std::cout << "gastar 100 -> " << (m.gastar(100) ? "ok" : "falhou")
              << " (falta mana)\n";
    std::cout << "mana: " << m.getAtual() << "/" << m.getMaximo() << "\n";

    m.recuperar(1000);
    std::cout << "recuperar 1000 (trava no maximo)\n";
    std::cout << "mana: " << m.getAtual() << "/" << m.getMaximo() << "\n";
}

static void testarDinheiro() {
    std::cout << "\n=== DINHEIRO ===\n";

    Dinheiro d(200);

    std::cout << "receber 50\n";
    d.receber(50);
    std::cout << "  dinheiro: " << d.getQuantidade() << "\n";

    std::cout << "gastar 300 -> " << (d.gastar(300) ? "ok" : "falhou")
              << " (falta dinheiro)\n";
    std::cout << "  dinheiro: " << d.getQuantidade() << "\n";

    std::cout << "gastar 120 -> " << (d.gastar(120) ? "ok" : "falhou") << "\n";
    std::cout << "  dinheiro: " << d.getQuantidade() << "\n";

    std::cout << "extrato:\n";
    const std::vector<std::string>& e = d.getExtrato();
    for (std::vector<std::string>::size_type i = 0; i < e.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << e[i] << "\n";
    }
}

static void testarItens() {
    std::cout << "\n=== ITENS (sort e find) ===\n";

    std::vector<Item> bolsa;
    bolsa.push_back(Item(3, "pocao de vida", 30, 1, "recupera vida", "curar", false));
    bolsa.push_back(Item(1, "espada", 50, 10, "arma branca", "", true));
    bolsa.push_back(Item(2, "maca", 5, 1, "comida", "curar", false));
    bolsa.push_back(Item(4, "armadura", 80, 20, "defesa", "", true));

    // operator< compara pelo nome: o sort deixa em ordem alfabetica.
    std::sort(bolsa.begin(), bolsa.end());

    std::cout << "bolsa ordenada por nome:\n";
    for (std::vector<Item>::size_type i = 0; i < bolsa.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << bolsa[i].getEtiqueta() << " ($"
                  << bolsa[i].getPreco() << ")\n";
    }

    // operator== compara pelo nome: acho o item sem saber o id.
    Item alvo(0, "espada", 0, 0, "", "", false);
    std::vector<Item>::iterator achou =
        std::find(bolsa.begin(), bolsa.end(), alvo);

    if (achou != bolsa.end()) {
        std::cout << "find por nome 'espada' achou: " << achou->getEtiqueta()
                  << " | durabilidade " << achou->getDurabAtual() << "/"
                  << achou->getDurabMax() << "\n";
    } else {
        std::cout << "find por nome 'espada' nao achou nada\n";
    }

    // usarDurabilidade avisa quando o item acaba.
    Item maca(2, "maca", 5, 1, "comida", "curar", false);
    std::cout << "usar a maca -> ainda serve? "
              << (maca.usarDurabilidade() ? "sim" : "nao") << " (durabilidade "
              << maca.getDurabAtual() << ")\n";
}

static void testarHabilidades() {
    std::cout << "\n=== HABILIDADES (sort e find) ===\n";

    std::vector<Habilidade> ficha;
    ficha.push_back(Habilidade(2, "bola de fogo", "3x1", "dano", "joga fogo", 15));
    ficha.push_back(Habilidade(5, "impulso", "1x1", "", "empurra", 5));
    ficha.push_back(Habilidade(1, "gelo", "2x2", "envenenado", "congela", 10));

    std::sort(ficha.begin(), ficha.end());

    std::cout << "habilidades ordenadas por nome:\n";
    for (std::vector<Habilidade>::size_type i = 0; i < ficha.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << ficha[i].getEtiqueta()
                  << " (custo " << ficha[i].getCusto() << " MP | area "
                  << ficha[i].getAreaAfetada() << ")\n";
    }

    Habilidade alvo(0, "bola de fogo", "", "", "", 0);
    std::vector<Habilidade>::iterator achou =
        std::find(ficha.begin(), ficha.end(), alvo);

    if (achou != ficha.end()) {
        std::cout << "find por nome 'bola de fogo' achou: "
                  << achou->getEtiqueta() << " | efeito: "
                  << achou->getEfeitoCausado() << "\n";
    } else {
        std::cout << "find por nome 'bola de fogo' nao achou nada\n";
    }
}

static void testarEfeitos() {
    std::cout << "\n=== EFEITOS (set) ===\n";

    // O set usa operator< e ja guarda sem repetir e em ordem.
    std::set<Efeito> listaE;
    listaE.insert(Efeito("envenenado"));
    listaE.insert(Efeito("curar"));
    listaE.insert(Efeito("dano"));
    listaE.insert(Efeito("envenenado"));  // repetido: o set ignora

    std::cout << "efeitos no set (" << listaE.size() << " unicos):\n";
    std::set<Efeito>::iterator it = listaE.begin();
    while (it != listaE.end()) {
        std::cout << "  - " << it->getNome() << "\n";
        ++it;
    }
}

static void mostrarCatalogo(const Catalogo& c) {
    std::cout << "  [H] ";
    const std::map<int, Habilidade>& listaH = c.getListaH();
    std::map<int, Habilidade>::const_iterator h = listaH.begin();
    while (h != listaH.end()) {
        std::cout << h->second.getEtiqueta() << " | ";
        ++h;
    }
    std::cout << "\n";

    std::cout << "  [I] ";
    const std::map<int, Item>& listaI = c.getListaI();
    std::map<int, Item>::const_iterator i = listaI.begin();
    while (i != listaI.end()) {
        std::cout << i->second.getEtiqueta() << " | ";
        ++i;
    }
    std::cout << "\n";

    std::cout << "  [E] ";
    const std::set<Efeito>& listaE = c.getListaE();
    std::set<Efeito>::const_iterator e = listaE.begin();
    while (e != listaE.end()) {
        std::cout << e->getNome() << " | ";
        ++e;
    }
    std::cout << "\n";
}

static void testarCatalogo() {
    std::cout << "\n=== CATALOGO ===\n";

    Catalogo c;

    c.addH("bola de fogo", "3x1", "dano", "joga fogo", 15);
    c.addH("gelo", "2x2", "envenenado", "congela", 10);
    c.addH("impulso", "1x1", "", "empurra", 5);
    std::cout << "depois de add 3 habilidades:\n";
    mostrarCatalogo(c);

    std::cout << "add 'gelo' de novo -> "
              << (c.addH("gelo", "9x9", "x", "y", 1) ? "entrou" : "recusado")
              << " (nome repetido)\n";

    std::cout << "remover 'gelo' -> "
              << (c.removerH("gelo") ? "ok" : "falhou") << "\n";

    // 'gelo' era o #2, agora e uma tumba. O proximo add deve pegar o #2.
    c.addH("lamina de vento", "3x1", "dano", "corta", 12);
    std::cout << "add 'lamina de vento' reaproveitou o id:\n";
    mostrarCatalogo(c);

    const Habilidade* porNome = c.buscarH("bola de fogo");
    std::cout << "buscarH por nome 'bola de fogo' -> "
              << (porNome != 0 ? porNome->getEtiqueta() : "nao achou") << "\n";

    const Habilidade* porId = c.buscarH(3);
    std::cout << "buscarH por id #3 -> "
              << (porId != 0 ? porId->getEtiqueta() : "nao achou") << "\n";

    const Habilidade* tumba = c.buscarH(4);
    std::cout << "buscarH pelo id livre #4 -> "
              << (tumba != 0 ? tumba->getEtiqueta() : "nao achou") << "\n";

    c.addI("espada", 50, 10, "arma branca", "", true);
    c.addI("maca", 5, 1, "comida", "curar", false);
    c.removerI("espada");
    c.addI("pocao de vida", 30, 1, "recupera vida", "curar", false);
    std::cout << "itens (removi o #1 'espada' e adicionei outra):\n";
    mostrarCatalogo(c);

    c.addE("envenenado");
    c.addE("curar");
    c.addE("envenenado");
    std::cout << "addE 'envenenado' 2x -> "
              << (c.addE("envenenado") ? "entrou" : "recusado") << "\n";

    c.finalizar();
    std::cout << "finalizado? " << (c.estaFinalizado() ? "sim" : "nao")
              << "\n";
    std::cout << "add depois de finalizar -> "
              << (c.addH("defesa magica", "1x1", "", "escudo", 8) ? "entrou"
                                                                 : "recusado")
              << "\n";
    std::cout << "remover depois de finalizar -> "
              << (c.removerH("impulso") ? "ok" : "recusado") << "\n";
}

static void mostrarFicha(const Personagem& p) {
    std::cout << "  Nome: " << p.getNome() << " ("
              << (p.estaVivo() ? "vivo" : "morto") << ")\n";
    std::cout << "  HP " << p.getVidaAtual() << "/" << p.getVidaMaxima()
              << " | MP " << p.getManaAtual() << "/" << p.getManaMaxima()
              << " | $" << p.getDinheiro().getQuantidade() << "\n";

    std::cout << "  [ ITENS ]\n";
    const std::vector<Item>& itens = p.getItens();
    for (std::vector<Item>::size_type i = 0; i < itens.size(); ++i) {
        std::cout << "    - " << itens[i].getEtiqueta() << " (durab "
                  << itens[i].getDurabAtual() << "/" << itens[i].getDurabMax()
                  << ", $" << itens[i].getPreco() << ")\n";
    }

    std::cout << "  [ HABILIDADES ]\n";
    const std::vector<Habilidade>& hs = p.getHabilidades();
    for (std::vector<Habilidade>::size_type i = 0; i < hs.size(); ++i) {
        std::cout << "    - " << hs[i].getEtiqueta() << " (custo "
                  << hs[i].getCusto() << " MP | efeito: "
                  << hs[i].getEfeitoCausado() << ")\n";
    }

    std::cout << "  [ EFEITOS ]\n";
    const std::list<Efeito>& ef = p.getEfeitos();
    int n = 1;
    std::list<Efeito>::const_iterator it = ef.begin();
    while (it != ef.end()) {
        std::cout << "    " << n << ". " << it->getNome() << "\n";
        ++n;
        ++it;
    }
}

static void testarPersonagem() {
    std::cout << "\n=== PERSONAGEM (roteiro completo) ===\n";

    // --- o mestre monta o mundo ---
    Catalogo c;
    c.addE("envenenado");
    c.addE("curar");
    c.addE("dano");
    c.addE("reviver");
    c.addE("fadiga");
    c.addH("bola de fogo", "3x1", "dano; envenenado", "joga uma bola de fogo",
           15);
    c.addI("espada", 50, 10, "uma espada curta", "", true);
    c.addI("maca", 5, 1, "uma maca", "curar", false);
    c.addI("bola de fogo", 20, 1, "livro que ensina bola de fogo",
           "aprender", false);  // livro: nome = habilidade que ensina
    c.addI("pocao de reviver", 30, 1, "traz de volta a vida", "reviver",
           false);
    c.addI("armadura real", 999, 20, "set completo", "", true);
    c.finalizar();

    // --- cria o personagem ---
    Personagem p("Bianca", 100, 50, 200);
    std::cout << "personagem criado:\n";
    mostrarFicha(p);

    // --- comprar ---
    std::cout << "\ncomprar espada ($50) -> "
              << (p.comprar(*c.buscarI("espada")) ? "ok" : "falhou") << "\n";
    p.comprar(*c.buscarI("maca"));
    p.comprar(*c.buscarI("bola de fogo"));    // o livro
    p.comprar(*c.buscarI("pocao de reviver"));
    std::cout << "comprar armadura real ($999) -> "
              << (p.comprar(*c.buscarI("armadura real")) ? "ok" : "falhou")
              << " (falta dinheiro)\n";
    mostrarFicha(p);

    // --- usar item e remendar ---
    std::string msg;
    p.usarItem("espada", 0, c, msg);
    std::cout << "\n" << msg << "\n";

    std::cout << "remendar espada pagando $10 -> "
              << (p.remendar("espada", 10) ? "ok" : "falhou") << "\n";
    std::cout << "remendar maca (nao remendavel) -> "
              << (p.remendar("maca", 10) ? "ok" : "falhou") << "\n";

    // --- livro ensina habilidade ---
    p.usarItem("bola de fogo", 0, c, msg);
    std::cout << msg << "\n";
    mostrarFicha(p);

    // --- usar habilidade: valor 40 no "dano" ---
    std::cout << "\nusarHabilidade 'bola de fogo' (valor 40) -> "
              << (p.usarHabilidade("bola de fogo", 40, msg) ? "ok" : "falhou")
              << "\n";
    std::cout << msg << "\n";

    // --- efeito manual e limpeza ---
    p.aplicarEfeito(Efeito("fadiga"));
    std::cout << "tem o item 'espada'? " << (p.temItem("espada") ? "sim" : "nao")
              << " | tem 'machado'? " << (p.temItem("machado") ? "sim" : "nao")
              << "\n";
    std::cout << "tem a habilidade 'bola de fogo'? "
              << (p.temHabilidade("bola de fogo") ? "sim" : "nao") << "\n";

    std::vector<int> tirar;
    tirar.push_back(1);  // o 1o da lista de efeitos
    tirar.push_back(1);  // repetido de proposito: nao pode tirar 2 efeitos
    std::cout << "limpar o efeito numero 1 (pedido 2x) -> "
              << (p.limparEfeitos(tirar) ? "ok" : "falhou") << "\n";
    std::cout << "ficha depois da limpeza:\n";
    mostrarFicha(p);

    // --- sem mana ---
    p.gastarMana(30);
    std::cout << "\ngastar mana ate sobrar " << p.getManaAtual() << " -> usar "
              << "'bola de fogo' (custa 15):\n  ";
    if (p.usarHabilidade("bola de fogo", 40, msg)) {
        std::cout << msg << "\n";
    } else {
        std::cout << "[erro] " << msg << "\n";
    }

    // --- morte e reviver ---
    p.dano(999);
    std::cout << "\ndano 999 -> vivo? " << (p.estaVivo() ? "sim" : "nao")
              << " (HP " << p.getVidaAtual() << ")\n";

    std::cout << "morto usar habilidade:\n  ";
    if (p.usarHabilidade("bola de fogo", 40, msg)) {
        std::cout << msg << "\n";
    } else {
        std::cout << "[erro] " << msg << "\n";
    }

    std::cout << "morto usar espada:\n  ";
    if (p.usarItem("espada", 0, c, msg)) {
        std::cout << msg << "\n";
    } else {
        std::cout << "[erro] " << msg << "\n";
    }

    std::cout << "usar item que nao existe:\n  ";
    if (p.usarItem("machado", 0, c, msg)) {
        std::cout << msg << "\n";
    } else {
        std::cout << "[erro] " << msg << "\n";
    }

    p.usarItem("pocao de reviver", 0, c, msg);
    std::cout << msg << "\n";

    // --- vender e jogar no lixo ---
    std::cout << "\nvender maca -> " << (p.vender("maca") ? "ok" : "falhou")
              << "\n";
    std::cout << "jogarNoLixo espada -> "
              << (p.jogarNoLixo("espada") ? "ok" : "falhou") << "\n";

    // --- habilidade: modificar e remover ---
    // Para modificar, mantenho o id que a copia ja tinha (a interface faz
    // isso na etapa 6).
    int idBolaDeFogo = 0;
    const std::vector<Habilidade>& listaH = p.getHabilidades();
    for (std::vector<Habilidade>::size_type i = 0; i < listaH.size(); ++i) {
        if (listaH[i].getNome() == "bola de fogo") {
            idBolaDeFogo = listaH[i].getId();
        }
    }
    p.modificarHabilidade(
        "bola de fogo",
        Habilidade(idBolaDeFogo, "bola de fogo", "3x1", "dano", "muito forte",
                   25));
    std::cout << "modificar habilidade (custo agora 25):\n";
    mostrarFicha(p);
    std::cout << "removerHabilidade -> "
              << (p.removerHabilidade("bola de fogo") ? "ok" : "falhou") << "\n";

    std::cout << "\nficha final:\n";
    mostrarFicha(p);
    std::cout << "  [ EXTRATO ]\n";
    const std::vector<std::string>& ex = p.getDinheiro().getExtrato();
    for (std::vector<std::string>::size_type i = 0; i < ex.size(); ++i) {
        std::cout << "    " << (i + 1) << ". " << ex[i] << "\n";
    }

    // --- morrer() direto tambem zera a vida ---
    Personagem q("Teste", 30, 10, 0);
    q.morrer();
    std::cout << "\nmorrer() direto -> vivo? " << (q.estaVivo() ? "sim" : "nao")
              << " | HP " << q.getVidaAtual() << "/" << q.getVidaMaxima() << "\n";
    std::cout << "dano 10 num morto -> "
              << (q.dano(10) ? "zerou" : "nao fez nada") << "\n";
}

static void mostrarSaves(const Saves& s) {
    for (int i = 0; i < s.getQuantidade(); ++i) {
        std::cout << "  " << (i + 1) << ". Save " << (i + 1) << " - ";
        if (s.slotVazio(i)) {
            std::cout << "vazio\n";
        } else {
            const Personagem& p = s.get(i).getPersonagem();
            std::cout << p.getNome() << " ("
                      << (p.estaVivo() ? "vivo" : "morto") << " | "
                      << p.getVidaAtual() << "/" << p.getVidaMaxima() << ")\n";
        }
    }
    std::cout << "  (ativo: save " << (s.getAtivoIndice() + 1) << ")\n";
}

static void mostrarContagem(const Catalogo& c) {
    std::cout << c.getListaH().size() << " habilidades, "
              << c.getListaI().size() << " itens, " << c.getListaE().size()
              << " efeitos";
}

static void testarCatalogoPadrao() {
    std::cout << "\n=== CATALOGO PADRAO ===\n";

    Catalogo c;
    c.carregarPadrao();
    std::cout << "carregarPadrao -> ";
    mostrarContagem(c);
    std::cout << "\n";
    mostrarCatalogo(c);
    std::cout << "finalizado? " << (c.estaFinalizado() ? "sim" : "nao")
              << " (o mestre ainda pode mexer)\n";

    // Carregar de novo nao duplica nada.
    c.carregarPadrao();
    std::cout << "carregarPadrao de novo -> ";
    mostrarContagem(c);
    std::cout << "\n";
}

static void testarSaves() {
    std::cout << "\n=== SAVES ===\n";

    Saves s;
    std::cout << "estado inicial (todo save ja vem com o mundo padrao):\n";
    mostrarSaves(s);
    for (int i = 0; i < s.getQuantidade(); ++i) {
        std::cout << "  catalogo do save " << (i + 1) << ": ";
        mostrarContagem(s.get(i).getCatalogo());
        std::cout << "\n";
    }

    // --- save 1: o mestre ajusta o mundo padrao e cria o personagem ---
    s.trocar(0);
    Save& um = s.getAtivo();
    um.getCatalogo().removerH("impulso");  // tira a #5
    um.getCatalogo().addH("cura em grupo", "3x3", "curar", "cura o time", 18);
    um.getCatalogo().finalizar();
    um.criarPersonagem("Cleber", 100, 50, 200);
    std::cout << "\nconfigurado o save 1 (tirei 'impulso' e pus 'cura em grupo'):\n";
    std::cout << "  ";
    mostrarContagem(um.getCatalogo());
    std::cout << "\n";
    mostrarSaves(s);

    // --- save 2 ---
    s.trocar(1);
    s.getAtivo().criarPersonagem("Bianca", 80, 40, 100);
    s.getAtivo().getPersonagem().dano(30);  // Bianca com 50/80
    std::cout << "\nconfigurado o save 2 (Bianca levou 30 de dano):\n";
    mostrarSaves(s);

    std::cout << "\ncriar um 2o personagem no save 2 -> "
              << (s.getAtivo().criarPersonagem("Outro", 10, 10, 0) ? "ok"
                                                                   : "recusado")
              << " (1 personagem por save)\n";

    std::cout << "trocar para o save 9 -> "
              << (s.trocar(8) ? "ok" : "recusado") << "\n";

    // --- excluir: a interface monta a pergunta com o status ---
    std::cout << "\napagar o save 1:\n";
    const Personagem& p1 = s.get(0).getPersonagem();
    std::cout << "  \"deseja apagar a ficha de " << p1.getNome() << " "
              << (p1.estaVivo() ? "vivo?" : "morto?") << "\" -> confirmado\n";
    s.excluir(0);

    const Catalogo& cat1 = s.get(0).getCatalogo();
    std::cout << "  catalogo do save 1: ";
    mostrarContagem(cat1);
    std::cout << "\n";
    std::cout << "  ganhou 'impulso' de volta? "
              << (cat1.buscarH("impulso") != 0 ? "sim" : "nao")
              << " | perdeu 'cura em grupo'? "
              << (cat1.buscarH("cura em grupo") == 0 ? "sim" : "nao")
              << " | finalizado? "
              << (cat1.estaFinalizado() ? "sim" : "nao") << "\n";
    std::cout << "  save 1 vazio de novo? "
              << (s.slotVazio(0) ? "sim" : "nao") << "\n";
    mostrarSaves(s);
}

static void testarPocaoDeMana() {
    std::cout << "\n=== POCAO DE MANA (efeito recuperar) ===\n";

    Catalogo c;
    c.addE("recuperar");
    c.addI("pocao de mana", 30, 1, "recupera mana", "recuperar", false);
    c.finalizar();

    Personagem p("Bianca", 100, 50, 100);
    std::string msg;

    p.comprar(*c.buscarI("pocao de mana"));
    p.gastarMana(45);  // mana 5/50
    std::cout << "mana antes de usar: " << p.getManaAtual() << "/"
              << p.getManaMaxima() << "\n";

    p.usarItem("pocao de mana", 20, c, msg);
    std::cout << msg << " | mana: " << p.getManaAtual() << "/"
              << p.getManaMaxima() << "\n";

    p.comprar(*c.buscarI("pocao de mana"));
    p.usarItem("pocao de mana", 999, c, msg);
    std::cout << msg << " | mana: " << p.getManaAtual() << "/"
              << p.getManaMaxima() << " (trava no maximo)\n";
}

int main() {
    testarVida();
    testarMana();
    testarDinheiro();
    testarItens();
    testarHabilidades();
    testarEfeitos();
    testarCatalogo();
    testarCatalogoPadrao();
    testarPersonagem();
    testarPocaoDeMana();
    testarSaves();
    return 0;
}
