# 🎮 Jogo do Simon com Arduino (Máquina de Estados)

Este projeto foi desenvolvido como um desafio para a unidade curricular de **Sistemas Embebidos**. Este é o meu primeiro projeto em Arduino onde apliquei total dedicação, empenho e estudo autónomo para ir além do básico, resultando numa recriação profissional e elástica do clássico jogo de memória **Simon** (Genius).

O código foi estruturado utilizando as melhores convenções de mercado em C++, focando-se em arquitetura de software, programação defensiva, organização de hardware e engenharia de som retro.

## 🚀 Funcionalidades e Diferenciais Técnicos

*   **Máquina de Estados Finita (FSM):** O fluxo principal do `void loop()` é gerido por estados (`STATE_START_GAME`, `STATE_PLAYER_TURN`, etc.). Isto elimina loops infinitos que travam o processador, melhora a fluidez e permite uma leitura ultra-rápida de comandos.
*   **Reset Inteligente de Jogo:** Graças à arquitetura FSM e à variável de controlo `gameState`, o botão físico de Reset (Pino 12) funciona de forma segura. Quando o jogo termina ou a meio de uma jogada, ele limpa as variáveis de forma controlada e reinicia o sistema sem risco de sobrecarga de memória (*stack overflow*).
*   **Programação Defensiva (Escudo Anti-Crash):** A função de exibição de LEDs conta com um bloco `default` de segurança. Caso ocorra uma corrupção de memória ou um valor numérico inválido, o Arduino não bloqueia em silêncio; ele reporta um Erro Crítico a 9600 bits/s no Monitor Serial, avisa no LCD e congela o sistema de forma controlada.
*   **Identidade Sonora Customizada (Easter Eggs):**
    *   **Início:** Som do Anel (Ring) do *Sonic the Hedgehog* (Ideal para as ondas quadradas digitais do buzzer).
    *   **Reset:** Efeito de Paragem de Tempo do **Za Warudo** (*JoJo's Bizarre Adventure*), emulado acusticamente através de um choque violento de frequências graves/agudas opostas e ticks de relógio congelado.
    *   **Derrota:** Efeito de Susto/Alerta inspirado no *Metal Gear Solid* (Arpejo rápido em staccato de 4 notas agudas).
    *   **Vitória Máxima (Ronda 20):** O clássico som cadenciado do Pager de missões concluídas do *Grand Theft Auto III*.
*   **Uso Eficiente de Hardware:** Os botões utilizam os resistores internos do chip através do modo `INPUT_PULLUP`, reduzindo o ruído elétrico (*debounce*) e poupando cablagem física na breadboard.

## 🛠️ Componentes Utilizados (Simulação Tinkercad)

*   1x Arduino Uno R3
*   1x Ecrã LCD 16x2 com Módulo de Comunicação I2C (Endereço `0x27`, Placa PCF8574)
*   4x LEDs (Verde, Vermelho, Amarelo, Azul)
*   5x Botões de Pressão (4 de Cores + 1 de Reset)
*   1x Buzzer Passivo (Pino 13)
*   Resistores de Proteção para os LEDs

## 📂 Estrutura e Fluxo do Código

O projeto foi escrito num único ficheiro `.ino`, respeitando a convenção de organização vertical padrão da comunidade:
1.  **Diretivas de Pré-processador:** Configuração das constantes, `#define` dos pinos e mapeamento dos estados da FSM.
2.  **Variáveis Globais Estritas:** Definição do array da sequência (`gameSequence`) e controle de rondas (`currentRound`).
3.  **`void setup()`:** Inicialização dos pinos, ativação do LCD, ligação do canal USB (`Serial.begin(9600)`) e baralhamento elétrico da aleatoriedade captada pelo ruído do pino vazio `analogRead(A0)`).
4.  **`void loop()`:** O coração do programa, que verifica constantemente o botão de Reset e gerencia o Switch-Case de estados do jogo.
5.  **Funções Auxiliares Modulares:** Funções limpas com responsabilidade única para feedback físico de cliques
