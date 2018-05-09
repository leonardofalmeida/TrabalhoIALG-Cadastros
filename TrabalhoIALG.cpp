#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

//------------------------------------ESTRUTURA------------------------------------------------------------------------------//
struct Desenho
{
    char nome[50];
    char criador[50]; 
    char titulo[50];
    char relacao[50];
};

//---------------------------------PROTOTIPAÇÃO--------------------------------------------------------------------------//
int Menu();
void Cadastro (Desenho *personagem);
void Atualizar(fstream &dadosDesenhos, Desenho dados, char *nome_personagem);
void Procura (char procurado[], ifstream &leitura, Desenho dados);
void Imprime_tela (ifstream &leitura, Desenho dados);
void ordenacao(int quantidade, Desenho vetor[]);

//-------------------------------------------FUNÇÃO PRINCIPAL--------------------------------------------------------------------//
int main()
{
    int escolha;
    Desenho *personagem=new Desenho;
    while((escolha = Menu()) != 0)
    {
        switch(escolha)
        {
            //Cadastro dos personagens
            case 1:
            {
                Cadastro(personagem);

                ofstream escrita("Dados_dos_Desenhos.bin", ios::binary | ios::app | ios::ate);
                escrita.write(reinterpret_cast<char*>(personagem), sizeof(Desenho));

                escrita.close();
                
                break;
            }

            //Atualizar dados dos registros
            case 2:
            {
                Desenho dados;

                char nome_personagem[50];
                cout << "\nDigite o NOME DO PERSONAGEM que deseja atualizar os dados: ";

                cin.ignore();
                cin.getline (nome_personagem, 50);

                fstream dadosDesenhos("Dados_dos_Desenhos.bin", ios::in | ios::out);

                if (dadosDesenhos.good())
                {
                    Atualizar(dadosDesenhos, dados, nome_personagem);
                }
                else
                {
                    cout << "\n#ATENÇÃO#: Esse arquivo não contem nenhum registro, por favor cadastre um desenho primeiro!\n" << endl;
                }

                dadosDesenhos.close();
                break;
                
            }

            //Buscar registros 
            case 3:
            {
                Desenho dados;
                
                ifstream leitura("Dados_dos_Desenhos.bin", ios::in);
                leitura.read(reinterpret_cast<char*>(&dados),  sizeof(Desenho));
                
                char procurado[50];

                cout << "Veja as opções abaixo e veja o que deseja buscar: ";
                cout << "\n\tNome do personagem;" << endl
                     << "\tNome do criador;" << endl
                     << "\tNome do do desenho;" << endl
                     << "\tRealacao do personagem;" << endl;
                cout << "\nEscolha: ";
                cin.ignore();
                cin.getline (procurado, 50);
                cout << "\n";

                if (leitura.good())
                {
                    Procura(procurado, leitura, dados);
                }
                else
                {
                    cout << "#ATENÇÃO#: Esse arquivo não contem nenhum registro, por favor cadastre um desenho primeiro!\n" << endl;
                }

                leitura.close();
                break;
            }

            //Imprime os registros
            case 4:
            {
                Desenho dados;

                ifstream leitura ("Dados_dos_Desenhos.bin", ios::in);
                leitura.read(reinterpret_cast<char*>(&dados), sizeof(Desenho));
                if (leitura.good())
                {
                    Imprime_tela(leitura, dados);
                }
                else
                {
                    cout << "#ATENÇÃO#: Esse arquivo não contem nenhum registro, por favor cadastre um desenho primeiro!\n" << endl;
                }

                leitura.close();
                break;
            }
        }
    }

    delete[] personagem;
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------//
int Menu()
{
    int escolha;
    cout << "********************************************************************************" << endl;
    cout << "\n\t\t   SEJA BEM-VINDO AO CADASTRO DE PERSONAGENS!\t\t\t\t"                      << endl;
    cout << "\n\tAqui você pode cadastrar todos seus desenhos animados favoritos!\n"           << endl;
    cout << "********************************************************************************" << endl;
    cout << "Digite uma opção abaixo [0 ~ 4] e dê [enter]:\n"                                  << endl;
    cout << "   [ 1 ] Cadastrar novo desenho;"                                                 << endl//OK
         << "   [ 2 ] Atualizar desenho;"                                                      << endl //
         << "   [ 3 ] Buscar desenho animado;"                                                 << endl //Ok
         << "   [ 4 ] Listar personagens em ordem alfabética;\n"                               << endl //Falta a ordenação
         << "   < 0 > Sair;\n"                                                                 << endl;
    cout << "Escolha: "                                                                        ;
    cin >> escolha;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << endl;
    
    return escolha;
}

//--------------------------------------------CADASTRO DOS PERSONAGENS-----------------------------------------------------------------//
void Cadastro(Desenho *personagem)
{
    cin.ignore();
    cout << "#### Cadastrar novo desenho animado ####\n" << endl;

    cout << "-> Digite o nome do personagem do desenho: ";
    cin.getline (personagem->nome, 50);

    cout << "-> Digite o criador dele: ";
    cin.getline (personagem->criador, 50);
    
    cout << "-> Digite o nome do desenho: ";
    cin.getline (personagem->titulo, 50);
    
    cout << "-> Digite uma relação dele(ex: Personagem: Tom - Relação: Jerry): ";
    cin.getline (personagem->relacao, 50);

    cout << "\n\n";
    cout << "\t\t~~~~ Operação realizada com sucesso! ~~~~";
    cout << "\n\n";
    
    cout << "\n--------------------------------------------------------------------------------" << endl;
    cout << "Digite qualquer numero e aperte [enter] para voltar ao menu." << endl;

    //Chama o menu de novo, esse codigo abaixo está em todas funções, menos na de ordenação
    int opcao;
    cin >> opcao;
}

//-------------------------------------ATUALIZAR DADOS-------------------------------------------------------------------//
void Atualizar(fstream &dadosDesenhos, Desenho dados, char *nome_personagem)
{
    bool encontrou = false;
    int contador = 0;
    dadosDesenhos.seekg(contador * sizeof(Desenho));//Pega a posição de leitura no começo do arquivo
    while (dadosDesenhos && dadosDesenhos.read(reinterpret_cast<char*>(&dados), sizeof(Desenho)) && !encontrou)
    {
        if(strcmp(nome_personagem, dados.nome) == 0)
        {
            cout << "\nEncontramos, digite os campos atualizados abaixo:" << endl;

            cout << "\nNome do personagem: ";
            cin.getline(dados.nome, 50);

            cout << "Nome do criador: ";
            cin.getline(dados.criador, 50);

            cout << "Nome do desenho: ";
            cin.getline(dados.titulo, 50);

            cout << "Relações: ";
            cin.getline(dados.relacao, 50);

            dadosDesenhos.seekp(contador * sizeof(Desenho));//Posiciona a cabeça de escrita no registro que será atualizado e sobreescreve ele
            dadosDesenhos.write(reinterpret_cast<char*>(&dados), sizeof(Desenho));
            encontrou = true;
        }
        ++contador;//Usado para posicionar a escrita
    }
            
    if (encontrou == false)
    {
        cout << "\n\tNão conseguimos atualizar, porque não achamos esse desenho :(" << endl;
        cout << "\n\t\tTalvez você tenha errado a grafia, tente novamente!" << endl;
    }
    cout << "\n--------------------------------------------------------------------------------" << endl;
    cout << "Digite qualquer numero e aperte [enter] para voltar ao menu." << endl;
    int opcao;
    cin >> opcao;
}


//---------------------------------------PROCURA PERSONAGENS----------------------------------------------------------------//
void Procura (char procurado[], ifstream &leitura, Desenho dados)
{
    bool encontrou = false;
    
    
    while (leitura && !leitura.eof() and !encontrou)
    {
        if (strcmp(procurado, dados.nome) == 0)
        {
            cout << "Personagem: " << dados.nome << endl
                 << "Criador: " << dados.criador << endl
                 << "Titulo: " << dados.titulo << endl
                 << "Principal relação: " << dados.relacao << endl;
            cout << '\n';
            encontrou = true;
            
        }
        if(strcmp(procurado, dados.criador) == 0)
        {
            cout << "Personagem: " << dados.nome << endl
                 << "Criador: " << dados.criador << endl
                 << "Titulo: " << dados.titulo << endl
                 << "Principal relação: " << dados.relacao << endl;
            cout << '\n';
            encontrou = true;
        }
        if(strcmp(procurado, dados.titulo) == 0)
        {
            cout << "Personagem: " << dados.nome << endl
                 << "Criador: " << dados.criador << endl
                 << "Titulo: " << dados.titulo << endl
                 << "Principal relação: " << dados.relacao << endl;
            cout << '\n';
            encontrou = true;
        }
        if(strcmp(procurado, dados.relacao) == 0)
        {
            cout << "Personagem: " << dados.nome << endl
                 << "Criador: " << dados.criador << endl
                 << "Titulo: " << dados.titulo << endl
                 << "Principal relação: " << dados.relacao << endl;
            cout << '\n';
            encontrou = true;
        }
        
        leitura.read(reinterpret_cast<char*>(&dados),  sizeof(Desenho));
    }
    if (encontrou == false)
    {
        cout << "\n\t\tSeu personagem não foi encontrado :(" << endl;
        cout << "\n\tTalvez você tenha errado a grafia, tente novamente!" << endl;
    }
    
    cout << "\n--------------------------------------------------------------------------------" << endl;
    cout << "Digite qualquer numero e aperte [enter] para voltar ao menu." << endl;
    int opcao;
    cin >> opcao;
    
}

//-----------------------------------------IMPRIME TODOS PERSONAGENS---------------------------------------------------------------//
void Imprime_tela(ifstream &leitura, Desenho dados)
{
    //Tamanho do arquivo e quantos registros existem no arquivo
    int tamanho, quantidade, i = 0;

    leitura.seekg(0, ios::end);//Le o arquivo ate o fim
    tamanho = leitura.tellg();//Pega o tamanho dele em bytes
    quantidade = tamanho / sizeof(Desenho);//Divide pelo tamanho em bytes da estrutura, e com isso conseguimos quantos registros ha no arquivo

    Desenho vetor[quantidade];
    leitura.seekg(0 * sizeof(Desenho));
    while(leitura.read(reinterpret_cast<char*>(&vetor[i]), sizeof(Desenho)))
    {
        ++i;
    }

    ordenacao(quantidade, vetor);
    i = 0;
    while(i < quantidade)
    {
        cout << "Personagem: " << vetor[i].nome << endl
             << "Criador: " << vetor[i].criador << endl
             << "Titulo: " << vetor[i].titulo << endl
             << "Principal relação: " << vetor[i].relacao << endl;
        cout << '\n';
        ++i;
    }
    cout << "\n--------------------------------------------------------------------------------" << endl;
    cout << "Digite qualquer numero e aperte [enter] para voltar ao menu." << endl;
    int opcao;
    cin >> opcao;
}

//----------------------------------------------ORDENÇÃO DOS REGISTROS-----------------------------------------//
void ordenacao(int quantidade, Desenho *vetor)
{
    int min;
    Desenho aux;
    for (int i = 0; i < quantidade - 1; ++i)
    {
        min = i;
        for (int j = i+1; j < quantidade; ++j)
        {
            if (strcmp(vetor[j].nome, vetor[min].nome) < 0)
            {
                min = j;
            }
        }
        aux = vetor[min];
        vetor[min] = vetor[i];
        vetor[i] = aux;
    }
}

