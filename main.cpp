#include <fstream>
#include <iostream>
#include "trie.cpp"
using TrieType = trie<std::string>;

int main() {
    try {
    TrieType a, b;
    // Test 1: Caricamento di Trie 1 e Trie 2 (con differenza nei sotto-alberi)
    std::string path1 = "trie.txt";
    std::string path2 = "trie2.txt";

    // Carica il primo trie (Trie 1)
    {
        std::ifstream ifs(path1);
        if (!ifs.is_open()) {
            std::cerr << "Error opening file: " << path1 << std::endl;
            return 1;
        }
        ifs >> a;
    }

    // Carica il secondo trie (Trie 2)
    {
        std::ifstream ifs(path2);
        if (!ifs.is_open()) {
            std::cerr << "Error opening file: " << path2 << std::endl;
            return 1;
        }
        ifs >> b;
    }

        std::cout<<"trie 1"<<std::endl<<a<<std::endl;
        std::cout<<"trie 2"<<std::endl<<b<<std::endl;

    // Test 2: Confronto di due trie con struttura identica
    std::cout << "Test 1 (Identici, a == b): " << std::boolalpha << (a == b) << std::endl;  // Dovrebbe essere false a causa di leaf7


        trie<int> original;
        original.add_child(trie<int>(2.71));

        // Perform copy assignment
        trie<int> copied = original;

        // Check structural equality
        assert(copied == original && "Copy assignment failed: tries are not structurally equal.");

        // Check parent pointer consistency (assuming parent() returns nullptr for root)
        assert(copied.get_parent() == nullptr && "Parent pointer of copied trie is not null.");

        // If all assertions pass
        std::cout << "Copy assignment test passed successfully!" << std::endl;



        //std::cout<<a<<std::endl;
/*
            // Test case for trie<int>
            {
                trie<int> t1;
                t1.set_label(new int(1));
                //t1.set_weight(10.5);

                trie<int> t2;
                t2.set_label(new int(2));
                t2.set_weight(20.5);

                t1.add_child(t2);

                trie<int> t3 = t1; // Copy constructor
                trie<int> t4;
                t4 = t1; // Copy assignment operator

                trie<int> t5 = std::move(t1); // Move constructor
                trie<int> t6;
                t6 = std::move(t2); // Move assignment operator

                std::cout << "t3 label: " << *t3.get_label() << ", weight: " << t3.get_weight() << std::endl;
                std::cout << "t4 label: " << *t4.get_label() << ", weight: " << t4.get_weight() << std::endl;
                std::cout << "t5 label: " << *t5.get_label() << ", weight: " << t5.get_weight() << std::endl;
                std::cout << "t6 label: " << *t6.get_label() << ", weight: " << t6.get_weight() << std::endl;
            }

            std::cout << "Tests completed." << std::endl;
            return 0;

*/

/*
        std::string path1 = "trie.txt";
        std::string path2 = "trie2.txt";

        TrieType a;
        TrieType b;

        std::ifstream ifs(path1);
        if (!ifs.is_open()) {
            std::cerr << "Error opening file: " << path1 << std::endl;
            return 1;
        }
        ifs >> a;

        std::ifstream ifss(path2);
        if (!ifss.is_open()) {
            std::cerr << "Error opening file: " << path2 << std::endl;
            return 1;
        }
        ifss >> b;

        b.get_children().get_first()->data.set_label(nullptr);

        std::cout<<a<<std::endl;
        std::cout<<b<<std::endl;

    std::cout << "sono uguali?? " << std::boolalpha << (a == b) << std::endl;





        // Load from path1 into a
        {
            std::ifstream ifs(path1);
            if (!ifs.is_open()) {
                std::cerr << "Error opening file: " << path1 << std::endl;
                return 1;
            }
            ifs >> a;
        }

        // Save a to path2
        {
            std::ofstream ofs(path2);
            if (!ofs.is_open()) {
                std::cerr << "Error opening file: " << path2 << std::endl;
                return 1;
            }
            ofs << a;
        }

        // Load from path2 into b
        {
            std::ifstream ifs(path2);
            if (!ifs.is_open()) {
                std::cerr << "Error opening file: " << path2 << std::endl;
                return 1;
            }
            ifs >> b;
        }

        // Compare a and b
        bool equal = (a == b);
        if (equal) {
            std::cout << "Test 1 passed: loaded/saved trie match." << std::endl;
        } else {
            std::cout << "Test 1 failed: loaded/saved trie do not match." << std::endl;
        }
    } catch (const parser_exception& e) {
        std::cerr << "ERROR: unexpected parser_exception thrown: " << e.what() << std::endl;
    }

    try {
        std::string path1 = "trie.txt";
        std::string path2 = "trie2.txt";

        TrieType a;
        TrieType b;

        // Load from path1 into a twice
        {
            std::ifstream ifs(path1);
            if (!ifs.is_open()) {
                std::cerr << "Error opening file: " << path1 << std::endl;
                return 1;
            }
            ifs >> a;
        }

        {
            std::ifstream ifs(path1);
            if (!ifs.is_open()) {
                std::cerr << "Error opening file: " << path1 << std::endl;
                return 1;
            }
            ifs >> a;
        }

        // Load from path1 into b
        {
            std::ifstream ifs(path1);
            if (!ifs.is_open()) {
                std::cerr << "Error opening file: " << path1 << std::endl;
                return 1;
            }
            ifs >> b;
        }


        std::ifstream file("trie.txt");
        // Verifica se il file è stato aperto correttamente
        if (!file.is_open()) {
            std::cerr << "Errore nell'aprire il file!" << std::endl;
            return 1;
        }
        //trie<std::string> myTrie;
        std::cout << "trie 1"<< std::endl;
        TrieType myTrie;
        file>>myTrie;
        std::cout<<myTrie;


        {
            std::cout <<"move assignment 1"<<std::endl;
            std::string path = "trie.txt";
            TrieType a, b, c;
            {
                std::ifstream ifs(path);
                ifs >> a;
            }

            {
                std::ifstream ifs(path);
                ifs >> c;
            }

            b = std::move(a);

            bool equal = (b == c);

            if(equal == true) {
                std::cout<<"move assignment 1 OK"<<std::endl;
            }else {
                std::cout<<"error: move assignment (or loading from file) failed."<<std::endl;
            }
        }

        {
            std::cout<<"move constructor"<<std::endl;
            std::string path = "trie.txt";
            TrieType a, c;
            {
                std::ifstream ifs(path);
                ifs >> a;
            }
            {
                std::ifstream ifs(path);
                ifs >> c;
            }

            TrieType b(std::move(a));

            bool equal = (b == c);

            if(equal == true) {
                std::cout<<"move constructor ok"<<std::endl;
            }else {
               std::cout<<"error: move constructor (or loading from file) failed."<<std::endl;
            }


}

            std::cout<<"copy constructor";
            {
                std::string path = "trie.txt";
                TrieType a, c;
                {
                    std::ifstream ifs(path);
                    ifs >> a;
                }
                {
                    std::ifstream ifs(path);
                    ifs >> c;
                }

                TrieType b(a);

                bool equal_ab = (a == b);
                bool equal_bc = (b == c);


                if(equal_ab == true) {
                    std::cout<<"copy const 1 ok"<<std::endl;
                }else {
                    std::cout<<"error: copy constructor (or loading from file) failed."<<std::endl;
                }
                if(equal_bc == true) {
                    std::cout<<"copy const 2 ok"<<std::endl;
                }else {
                    std::cout<<"error: copy constructor (or loading from file) failed."<<std::endl;
                }
            }
*/

        /*
        //std::ifstream file("trie.txt");
        // Verifica se il file è stato aperto correttamente
        if (!file.is_open()) {
            std::cerr << "Errore nell'aprire il file!" << std::endl;
            return 1;
        }
        //trie<std::string> myTrie;
        std::cout << "trie 1"<< std::endl;
        //TrieType myTrie;

        file>>myTrie;
        std::cout<<myTrie;

        TrieType myTrie2=myTrie;
        myTrie2.get_children().get_first()->data.get_children().remove_first();
        std::cout<<myTrie2;
        std::cout<<myTrie;
        */

        /*
        std::cout << "Test degli iteratori costanti:\n";
        for (trie<std::string>::const_node_iterator it = &const_myTrie.begin().get_leaf(); it != const_myTrie.end(); ++it) {
            std::cout << "Nodo: " << *it<< std::endl;
        }



        std::cout << "la foglia massima è: " << myTrie.max().get_weight()<< "\n";

        // Creiamo una trie con alcuni nodi per il test

            trie<std::string> rootTrie;
            trie<std::string> child1(3.5);
            trie<std::string> child2(5.0);
            trie<std::string> child3(2.0);

            std::string rootLabel = "root";
            std::string childLabel1 = "child1";
            std::string childLabel2 = "child2";
            std::string childLabel3 = "child3";

            rootTrie.set_label(&rootLabel);
            child1.set_label(&childLabel1);
            child2.set_label(&childLabel2);
            child3.set_label(&childLabel3);

            // Aggiungiamo i figli alla radice
            rootTrie.add_child(child1);
            rootTrie.add_child(child2);
            rootTrie.add_child(child3);
            std::cout<<rootTrie;
            // Test degli operatori di uguaglianza (operator== e operator!=)
            std::cout << "Test degli operatori == e !=\n";
            assert(rootTrie == rootTrie);   // Radice uguale a sé stessa
            assert(child1 != child2);       // Nodi differenti

            // Creare un nodo identico a child1 per verificare operator==
            trie<std::string> child1Clone = child1;
            assert(child1 == child1Clone);  // I nodi devono essere uguali

            std::cout << "Test degli operatori == e != superato.\n";

            // Test degli iteratori di nodi (node_iterator)
            std::cout << "Test degli iteratori di nodi:\n";
            for (auto it = rootTrie.root(); it != rootTrie.end(); ++it) {
                std::cout << "Nodo: " << *it << ", Peso: "<< std::endl;
            }

            // Test degli iteratori costanti (const_node_iterator)
            const trie<std::string>& constTrie = rootTrie;
            std::cout << "Test degli iteratori costanti:\n";
            for (auto it = constTrie.root(); it != constTrie.end(); ++it) {
                std::cout << "Nodo: " << *it<< std::endl;
            }
            std::cout << "Test completato con successo.\n";
            */

        return 0;
    } catch (const parser_exception& e) {
        std::cerr << "ERROR: unexpected parser_exception thrown: " << e.what() << std::endl;
    }
    return 0;
}