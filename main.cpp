#include <bits/stdc++.h>
int m = 0;
bool habloMalDeUnBueno(std::set<int> agentesConfiables, std::pair< std::pair< int, int>, int> opinion, int actual, bool confio)
{
    return (opinion.first.first == actual) && (agentesConfiables.find(opinion.first.second) != agentesConfiables.end()) &&
           (opinion.second == 0) && confio;
}
bool unBuenoHabloMalDeEl(std::set<int> agentesConfiables, std::pair< std::pair< int, int>, int> opinion, int actual, bool confio)
{
    return (agentesConfiables.find(opinion.first.first) != agentesConfiables.end()) && (opinion.first.second == actual) &&
           (opinion.second == 0) && confio;
}
bool habloBienDeUnMalo(std::set<int> agentesConfiables, std::pair< std::pair< int, int>, int> opinion, int actual, bool confio)
{
    return (opinion.first.first == actual) && (opinion.first.second < actual) &&
        (agentesConfiables.find(opinion.first.second) == agentesConfiables.end()) &&
           (opinion.second == 1) && confio;
}
bool unBuenoHabloBienYNoConfio(std::set<int> agentesConfiables, std::pair< std::pair< int, int>, int> opinion, int actual, bool confio)
{
    return !confio && (agentesConfiables.find(opinion.first.first) != agentesConfiables.end()) &&
           (opinion.first.second == actual) && (opinion.second == 1);
}
bool tieneSentido(std::set<int> agentesConfiables, std::vector<std::pair< std::pair< int, int>, int>> vectorcitoDeOpiniones,
    int actual, bool confio)
{
    for(int i = 0; i < vectorcitoDeOpiniones.size(); i++)
    {
        if (habloMalDeUnBueno(agentesConfiables, vectorcitoDeOpiniones[i], actual, confio) ||
                unBuenoHabloMalDeEl(agentesConfiables, vectorcitoDeOpiniones[i], actual, confio) ||
                habloBienDeUnMalo(agentesConfiables, vectorcitoDeOpiniones[i], actual, confio) ||
                unBuenoHabloBienYNoConfio(agentesConfiables, vectorcitoDeOpiniones[i], actual, confio))
        {
            return 0;
        }
    }
    return 1;
}
int generarSolucion( std::vector<std::pair< std::pair< int, int>, int>> vectorcitoDeOpiniones,
                     std::set<int> agentesConfiables, int i, int n)
{
    if (n == i) {
          if (agentesConfiables.size() > m) {
              m = agentesConfiables.size();
          }
          return agentesConfiables.size();
    }
    int sinElla = -1;
    int conElla = -1;
    if ((agentesConfiables.size() + n - i) <= m){
        return -1;
    } else {
        if (tieneSentido(agentesConfiables, vectorcitoDeOpiniones, i, 1)){
            agentesConfiables.insert(i);
            conElla = generarSolucion(vectorcitoDeOpiniones, agentesConfiables, i+1, n);
            //agentesConfiables.erase(i);
        }
        agentesConfiables.erase(i);
        if (tieneSentido(agentesConfiables, vectorcitoDeOpiniones, i, 0)) {
            sinElla = generarSolucion(vectorcitoDeOpiniones, agentesConfiables, i+1, n);
        }
        return std::max(sinElla, conElla);
    }
}

void generarMejorInstancia( std::vector<std::pair< std::pair< int, int>, int>> &vectorcitoDeOpiniones, int n)
{
    for (int j = 0; j < n; j++)
    {
        for (int i = 0; i < n; i++){
            vectorcitoDeOpiniones.push_back( std::make_pair(std::make_pair(j, i), 1));
        }
    }
}
std::vector<std::pair< std::pair< int, int>, int>> generarPeorInstancia( std::vector<std::pair< std::pair< int, int>, int>> &vectorcitoDeOpiniones, int n)
{
    for (int j = 0; j < n/2; j++)
    {
        for (int i = n/2; i < n; i++){
            vectorcitoDeOpiniones.push_back( std::make_pair(std::make_pair(j, i), 0));
            vectorcitoDeOpiniones.push_back( std::make_pair(std::make_pair(i, j), 0));
        }
    }
    return vectorcitoDeOpiniones;
}

int main() {

    std::string nombreArchivo = "esidatos";

    std::stringstream ss;
    ss <<  "/home/jscherman/tpesi/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "long,tiempo" << std::endl;
    int minM = 1, maxM = 30, saltarDeA = 1, cantInstanciasPorM = 3;
    for (int i = minM; i <= maxM; i+=saltarDeA) {
        long long tiempoTotal = 0;
        for (int j = 0; j < cantInstanciasPorM; ++j) {
            std::vector<std::pair< std::pair< int, int>, int>> vectorcitoDeOpiniones;
            std::set<int> agentesConfiables;
            vectorcitoDeOpiniones = generarPeorInstancia(vectorcitoDeOpiniones, i);
            auto tpi = std::chrono::high_resolution_clock::now();
            int rta = generarSolucion(vectorcitoDeOpiniones, agentesConfiables, 0, i);
            auto tpf = std::chrono::high_resolution_clock::now();
            auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
            tiempoTotal+= tiempo;
        }

        tiempoTotal = tiempoTotal/ cantInstanciasPorM;
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
//        std::vector<std::pair< std::pair< int, int>, int>> vectorcitoDeOpiniones;
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


