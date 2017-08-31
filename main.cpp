#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <fstream>

struct Opinion {
    int emisor;
    int receptor;
    bool confioEnReceptor;

    friend std::ostream &operator<<(std::ostream &os, const Opinion &p) {
        os << "(" << p.emisor << "/" << p.receptor << " : " << (p.confioEnReceptor ? "SI" : "NO") << ")";
        return os;
    };
};

inline std::ostream &operator<<(std::ostream &os, const std::vector<Opinion> &p) {
    std::cout << "Opiniones: ";
    for (int k = 1; k < p.size(); ++k) {
        std::cout << p[k] << " ";
    }
    std::cout << std::endl;
    return os;
};

int m = 0;
bool habloMalDeUnBueno(std::set<int> &agentesConfiables, Opinion &opinion, int actual, bool confio)
{
    return (opinion.emisor == actual) && (agentesConfiables.find(opinion.receptor) != agentesConfiables.end()) &&
           (opinion.confioEnReceptor == 0) && confio;
}
bool unBuenoHabloMalDeEl(std::set<int> &agentesConfiables, Opinion& opinion, int actual, bool confio)
{
    return ((agentesConfiables.find(opinion.emisor) != agentesConfiables.end()) || opinion.emisor == actual) && (opinion.receptor == actual) &&
           (opinion.confioEnReceptor == 0) && confio;
}
bool habloBienDeUnMalo(std::set<int> &agentesConfiables, Opinion& opinion, int actual, bool confio)
{
    return (opinion.emisor == actual) && (opinion.receptor > actual) &&
        (agentesConfiables.find(opinion.receptor) == agentesConfiables.end()) &&
           (opinion.confioEnReceptor == 1) && confio;
}
bool unBuenoHabloBienYNoConfio(std::set<int> &agentesConfiables, Opinion& opinion, int actual, bool confio)
{
    return !confio && (agentesConfiables.find(opinion.emisor) != agentesConfiables.end()) &&
           (opinion.receptor == actual) && (opinion.confioEnReceptor == 1);
}
bool tieneSentido(std::set<int> &agentesConfiables, std::vector<Opinion> &vectorcitoDeOpiniones,
    int actual, bool confio)
{
    std::set<int> MalosPosta;
    std::set<int> MalosParaElActual;
    std::set<int> BuenosPosta;
    std::set<int> BuenosParaElActual;
    for(int i = 0; i < vectorcitoDeOpiniones.size(); i++)
    {
        if (habloMalDeUnBueno(agentesConfiables, vectorcitoDeOpiniones[i], actual, confio) ||
                unBuenoHabloMalDeEl(agentesConfiables, vectorcitoDeOpiniones[i], actual, confio) ||
                habloBienDeUnMalo(agentesConfiables, vectorcitoDeOpiniones[i], actual, confio) ||
                unBuenoHabloBienYNoConfio(agentesConfiables, vectorcitoDeOpiniones[i], actual, confio)) {
            //y no hablo mal de alguien q un bueno hablo bien, no hablo bien si un bueno hablo mal Y CONFIO
            //y no confio y un bueno hablo bien de alguien q hbalo bien de el!!
            return false;
        }
        if (vectorcitoDeOpiniones[i].emisor == actual) {
            if (vectorcitoDeOpiniones[i].confioEnReceptor){
                BuenosParaElActual.insert(vectorcitoDeOpiniones[i].receptor);
            } else {
                MalosParaElActual.insert(vectorcitoDeOpiniones[i].receptor);
            }
        } else {
            if (agentesConfiables.find(vectorcitoDeOpiniones[i].emisor) != agentesConfiables.end()){
                if(vectorcitoDeOpiniones[i].confioEnReceptor) {
                    BuenosPosta.insert(vectorcitoDeOpiniones[i].receptor);
                }
                else {
                    MalosPosta.insert(vectorcitoDeOpiniones[i].receptor);
                }
            }
        }

    }
    if (confio) {
        auto iterator = MalosParaElActual.begin();
        while (iterator != MalosParaElActual.end())
        {
            if (BuenosPosta.find(*iterator) != BuenosPosta.end()){
                return false;
            }
            iterator++;
        }
        iterator = BuenosParaElActual.begin();
        while (iterator != BuenosParaElActual.end())
        {
            if (MalosPosta.find(*iterator) != MalosPosta.end()){
                return false;
            }
            iterator++;
        }
    }
    return true;
}
int generarSolucion( std::vector<Opinion> &vectorcitoDeOpiniones,
                     std::set<int> &agentesConfiables, int i, int n, int &maximoParcial)
{
    if (0 > i) {
          if (agentesConfiables.size() > m) {
              maximoParcial = agentesConfiables.size();
          }
          return agentesConfiables.size();
    }
    int sinElla = -1;
    int conElla = -1;


    bool tieneSentidoAgregarlo = tieneSentido(agentesConfiables, vectorcitoDeOpiniones, i, 1);
    bool tieneSentidoNoAgregarlo = tieneSentido(agentesConfiables, vectorcitoDeOpiniones, i, 0);

    if (tieneSentidoAgregarlo && ((agentesConfiables.size() + i) >= maximoParcial)){
        agentesConfiables.insert(i);
        conElla = generarSolucion(vectorcitoDeOpiniones, agentesConfiables, i-1, n, maximoParcial);
        agentesConfiables.erase(i);
    }
     if (tieneSentidoNoAgregarlo && ((agentesConfiables.size() + i - 1) >= maximoParcial)) {
            sinElla = generarSolucion(vectorcitoDeOpiniones, agentesConfiables, i-1, n, maximoParcial);
    }

        return std::max(sinElla, conElla);
    }



std::vector<Opinion>  generarMejorInstancia(int n) {
    std::vector<Opinion> opiniones;
    for (int j = 1; j <= n; j++)
    {
        // for (int i = 0; i < n; i++){
        Opinion opinion = {j-1, j, 0};
        opiniones.push_back(opinion);
        //}
    }
    return opiniones;
}
std::vector<Opinion>  generarOtraMejorInstancia(int n) {
    std::vector<Opinion> opiniones;
    for (int j = 0; j < n; j++)
    {
         for (int i = 0; i < n; i++){
        Opinion opinion = {i, j, 0};
        opiniones.push_back(opinion);
        }
    }
    return opiniones;
}
std::vector<Opinion> generarPeorInstancia(int n)
{
    std::vector<Opinion> opiniones;
    for (int j = 0; j <= n/2; j++)
    {
        for (int i = n/2; i < n; i++){
            opiniones.push_back({j, i, 0});
            opiniones.push_back({i, j, 0});
        }
    }
    return opiniones;
}
std::vector<Opinion> generarAlgunaInstancia()
{
    std::vector<Opinion> opiniones;
   // opiniones.push_back({9, 12, 1});
    opiniones.push_back({0, 5, 1});
    opiniones.push_back({0, 1, 1});
    opiniones.push_back({3, 2, 1});
    opiniones.push_back({4, 2, 1});
    opiniones.push_back({4, 0, 1});
    opiniones.push_back({5, 6, 0});
    opiniones.push_back({6, 3, 0});
    opiniones.push_back({7, 6, 1});
    opiniones.push_back({7, 0, 1});
    opiniones.push_back({8, 1, 0});
    opiniones.push_back({9, 8, 1});
    opiniones.push_back({9, 4, 1});
    opiniones.push_back({9, 2, 0});

  return opiniones;
}
int main() {

    std::string nombreArchivo = "esidatos";
    std::stringstream ss;
    ss <<  "/Users/jessicasinger/Desktop/algo3/algo2Taller/" << nombreArchivo << ".csv";
//    ss <<  "/home/jscherman/tpesi/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "long,tiempo" << std::endl;
    int minN = 1, maxN = 1000, saltarDeA = 3, cantInstanciasPorN = 5;
    for (int i = minN; i <= maxN; i+=saltarDeA) {
        long long tiempoTotal = 0;
        for (int j = 0; j < cantInstanciasPorN; ++j) {

            std::set<int> agentesConfiables;
            std::vector<Opinion> vectorcitoDeOpiniones = generarAlgunaInstancia();//= generarOtraMejorInstancia(i);
//            std::cout << vectorcitoDeOpiniones;

            auto tpi = std::chrono::high_resolution_clock::now();
            //int maximoParcial = -1;
            //int rta = generarSolucion(vectorcitoDeOpiniones, agentesConfiables, i-1, i, m);
            int rta = generarSolucion(vectorcitoDeOpiniones, agentesConfiables, 9, 10, m);
            m = 0;
            auto tpf = std::chrono::high_resolution_clock::now();

            auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
            tiempoTotal+= tiempo;
        }

        tiempoTotal = tiempoTotal/ cantInstanciasPorN;
        std::cout << i << "," << tiempoTotal << std::endl ;
        a_file << i << "," << tiempoTotal << std::endl;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;

//   // int m = -1;
//    std::vector<int> tiempos;
//    std::vector<int> resultados;
//    for (int j = 1; j < 30; j++)
//    {
//
//        std::vector<Opinion> vectorcitoDeOpiniones;
//       // generarMejorInstancia(vectorcitoDeOpiniones, j);
//        generarPeorInstancia(vectorcitoDeOpiniones, j);
//        std::set<int> agentesConfiables;
//
//        auto tpi = std::chrono::high_resolution_clock::now();
//
//        int hola = generarSolucion(vectorcitoDeOpiniones, agentesConfiables, 0, j);
//        hola = generarSolucion(vectorcitoDeOpiniones, agentesConfiables, 0, j);
//        hola = generarSolucion(vectorcitoDeOpiniones, agentesConfiables, 0, j);
//        resultados.push_back(hola);
//        m = 0;
//        auto tpf = std::chrono::high_resolution_clock::now();
//        auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
//        tiempos.push_back(tiempo / 3);
//        std::cout << j << "," << tiempo/3 << std::endl;
//    }

    return m;
}


