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
    return (agentesConfiables.find(opinion.emisor) != agentesConfiables.end()) && (opinion.receptor == actual) &&
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
    for(int i = 0; i < vectorcitoDeOpiniones.size(); i++)
    {
        if (habloMalDeUnBueno(agentesConfiables, vectorcitoDeOpiniones[i], actual, confio) ||
                unBuenoHabloMalDeEl(agentesConfiables, vectorcitoDeOpiniones[i], actual, confio) ||
                habloBienDeUnMalo(agentesConfiables, vectorcitoDeOpiniones[i], actual, confio) ||
                unBuenoHabloBienYNoConfio(agentesConfiables, vectorcitoDeOpiniones[i], actual, confio)) {
            return false;
        }
        //si no confio, cuento solo aquellos q el no hablo bien
        //si confio, solo aquellos q el no hablo mal
         /*   if (vectorcitoDeOpiniones[i].first.first == actual)
            {
                if (vectorcitoDeOpiniones[i].first.second > actual){
                    if (confio){
                        if (vectorcitoDeOpiniones[i].second == 0){
                            cantARestar++;
                        }
                    }
                    else
                    {
                        if (vectorcitoDeOpiniones[i].second == 1){
                            cantARestar++;
                        }
                    }
                }
            }*/
    }
    return true;
}
int generarSolucion( std::vector<Opinion> &vectorcitoDeOpiniones,
                     std::set<int> &agentesConfiables, int i, int n)
{
    if (0 > i) {
          if (agentesConfiables.size() > m) {
              m = agentesConfiables.size();
          }
          return agentesConfiables.size();
    }
    int sinElla = -1;
    int conElla = -1;

    if ((agentesConfiables.size() - i) < m){ //los q ya agregue + los q me faltan procesar
        return -1;
    } else {

    bool tieneSentidoAgregarlo = tieneSentido(agentesConfiables, vectorcitoDeOpiniones, i, 1);
    bool tieneSentidoNoAgregarlo = tieneSentido(agentesConfiables, vectorcitoDeOpiniones, i, 0);

    if (tieneSentidoAgregarlo){
        agentesConfiables.insert(i);
        conElla = generarSolucion(vectorcitoDeOpiniones, agentesConfiables, i-1, n);
        agentesConfiables.erase(i);
    }
     if (tieneSentidoNoAgregarlo) {
            sinElla = generarSolucion(vectorcitoDeOpiniones, agentesConfiables, i-1, n);
    }

        return std::max(sinElla, conElla);
    }
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

int main() {

    std::string nombreArchivo = "esidatos";
    std::stringstream ss;
    ss <<  "/Users/jessicasinger/Desktop/algo3/algo2Taller/" << nombreArchivo << ".csv";
//    ss <<  "/home/jscherman/tpesi/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "long,tiempo" << std::endl;
    int minN = 1, maxN = 40, saltarDeA = 1, cantInstanciasPorN = 3;
    for (int i = minN; i <= maxN; i+=saltarDeA) {
        long long tiempoTotal = 0;
        for (int j = 0; j < cantInstanciasPorN; ++j) {

            std::set<int> agentesConfiables;
            std::vector<Opinion> vectorcitoDeOpiniones = generarMejorInstancia(i);
//            std::cout << vectorcitoDeOpiniones;

            auto tpi = std::chrono::high_resolution_clock::now();
            int rta = generarSolucion(vectorcitoDeOpiniones, agentesConfiables, i-1, i);
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


