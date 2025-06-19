#include "trie.hpp"
//PARSER
static void clean(std::istream& in){
   char c = 0;
    do{ ///toglie tutti gli spazi e a capo
        in >> c;
    }while (c == ' ' || c == '\n');
    in.putback(c);
}

template <typename T>
T convert( std::istream& in) {
    clean(in);
    T res;
    if (!(in >> res)) {
        throw parser_exception(" convertion failed");
    }
    return res;
}

template<typename T>
void set_all_sons(trie<T>& padre) {
    auto& sons=padre.get_children();
    if(!sons.empty()) {
        auto son=sons.get_first();
        while(son!=nullptr) {
            auto& nodo=son->data;
            if(nodo.get_parent()!=&padre) {
                nodo.set_parent(&padre);
                set_all_sons(nodo);
            }
            son=son->next;
        }
    }
}


template <typename T>
void children(std::istream& is);

template <typename T>
trie<T> arc(std::istream& in) {
    trie<T> res;
    ///attenzione qua
    T* micio=new T(convert<T>(in));
    res.set_label(micio);
    delete micio;

    bool peso=false;

    clean(in);
    char c=in.peek();
    if(std::isdigit(c) || c=='+' || c=='-') {
        auto weight=convert<double>(in);
        c=in.peek();
        if(c==',') { //se al posto di un. c'è una virgola
            in >> c;
            std::string virgoletta;
            if(in.peek()!=' ') {
                if(weight>=0) {
                    virgoletta="+0."+convert<std::string>(in);
                }else {
                    virgoletta="-0."+convert<std::string>(in);
                }
                weight=weight+std::stod(virgoletta);
            }
        }
        res.set_weight(weight);
        peso=true;
    }
    children(in,res);
    if(peso==false && res.get_children().empty()) {
        throw parser_exception("Leaf without a weight.");
    }
    return res;
}

template <typename T>
void children(std::istream& is, trie<T>& padre){
    char c='x';
    auto child=convert<std::string>(is);
    if(child=="children" || child=="Children") {
        c=convert<char>(is);
        if(c=='=') {
            c=convert<char>(is);
            if(c=='{') {
                clean(is);
                c=is.peek(); ////CASINOOOO
                if(c=='}') {
                    is >> c;
                    return;
                }
                while(c!='}') {
                    trie<T> new_child=std::move(arc<T>(is));
                    padre.add_child(new_child);
                    set_all_sons(padre);

                    clean(is);
                    c=is.peek();

                    if(c==',') {
                        is >> c;
                        clean(is);
                        c=is.peek();
                        if(c=='}') {
                            throw parser_exception(" ',' and then a '}' ");
                        }
                    }else if(c!='}') {
                        throw parser_exception("Expected ',' ");
                    }
                }
                if(c=='}') {
                    is >> c;
                    return;
                }
                throw parser_exception("Expected '}'  not close {}");
            }
            throw parser_exception("Expected '{' after '='");
        }
        throw parser_exception("Expected '=' after space");
    }
    throw parser_exception("Expected Children or Children");
}


//COSTRUTTORI

//crea vuoto
template <typename T>
trie<T>::trie() : m_p(nullptr) , m_l(nullptr), m_c(),m_w(){}


//crea con valore
template <typename T>
trie<T>::trie(double x){
    m_p=nullptr; //padre
    m_l=nullptr; //etichetta
    m_w=x; //contenuto nodo
}

//crea copia
template <typename T>
trie<T>::trie(trie<T> const& x) {
    m_p=nullptr; //padre
    m_l=nullptr; //etichetta
    m_w=0; //contenuto nodo
    *this=x;
}

//move contructor
template <typename T>
trie<T>::trie(trie<T>&& x) {
    m_p=nullptr; //padre
    m_l=nullptr; //etichetta
    m_w=0; //contenuto nodo
    *this=std::move(x);
}

//distruttore
template <typename T>
trie<T>::~trie() {
    m_p=nullptr;
    if (m_l != nullptr) {
        delete m_l;
        m_l = nullptr;
    }
}

template <typename T>
trie<T>& trie<T>::operator=(trie<T> const& x) {
    if(this == &x) {
        return *this;
    }
    if (m_l != nullptr) {
        delete m_l;
        m_l = nullptr;
    }

    if(x.m_l != nullptr) {
        m_l= new T(*x.m_l);
    }else {
        m_l=nullptr;
    }

    m_w=x.m_w; //peso
    m_c=x.m_c; //se è vuoto non fa nulla che ridere
    set_all_sons(*this);

    return *this;
}

template <typename T>
trie<T>& trie<T>::operator=(trie<T>&& x) {
    if(this == &x) {
        return *this;
    }
    if (m_l != nullptr) {
        delete m_l;
        m_l = nullptr;
    }

    m_l=x.m_l;
    m_w=x.m_w;
    m_c=std::move(x.m_c);
    set_all_sons(*this);

    x.m_l=nullptr;
    x.m_p=nullptr;
    x.m_w=0;
    //forse da togliere dalla bag del padre ?? vediamo come vanno i test
    return *this;
};

template <typename T>
void trie<T>::set_weight(double w) {
    if(this->m_c.empty()) {
        m_w=w;
    }else {
        throw parser_exception("not possible to assign a weight the node has children");
    }
};

template <typename T>
void trie<T>::set_label(T* l) {
    if (m_l != nullptr) {
        delete m_l;
        m_l = nullptr;
    }
    if (l != nullptr) {
        m_l = new T(*l); // Copia il valore puntato
    } else {
        m_l = nullptr;
    }
}

template <typename T>
void trie<T>::set_parent(trie<T>* p) {
    m_p=p;
}

template <typename T>
void trie<T>:: add_child(trie<T> const& c) {
    try {
        if(m_w) {
            m_w=0; //non più foglia
            throw parser_exception("The node has weight and is not a leaf");
        }
        m_c.append(c);
    }catch(std::runtime_error& e) {
        throw parser_exception(e.what());
    }
}

template <typename T>
double trie<T>::get_weight() const {
    return m_w;
};

template <typename T>
T const* trie<T>::get_label() const {
    return m_l;
}

template <typename T>
T* trie<T>::get_label() {
    return m_l;
}

template <typename T>
trie<T>* trie<T>::get_parent() {
    return m_p;
}

template <typename T>
trie<T> const* trie<T>::get_parent() const {
    return m_p;
}

template <typename T>
bag<trie<T>> const& trie<T>::get_children() const {
    return m_c;
};

template <typename T>
bag<trie<T>>& trie<T>::get_children() {
    return m_c;
};



template <typename T>
bool trie<T>::operator==(trie<T> const& in) const{
    if (this == &in) {
        return true;
    }
    if((m_l==nullptr && in.m_l==nullptr) || (m_l != nullptr && in.m_l != nullptr && *m_l == *in.m_l)) { //se lable è uguale
        if(m_w == in.m_w) { //se peso è ugulae
            if(m_c == in.m_c) { //se bag è uguale
                return true;
            }
        }
    }
    return false;
};

/*
template <typename T>
bool trie<T>::operator==(trie<T> const& in) const{
    if (this == &in) {
        return true;
    }
    if(compare_children(*this,in))
        return true;
    return false;
};

template <typename T>
bool compare_children(trie<T> const& ori, trie<T> const& in){
    auto sup1 = ori.get_children().begin();
    auto sup2 = in.get_children().begin();
    while (sup1!=ori.get_children().end() && sup2!=in.get_children().end()) {
        if(sup1->get_weight()!=sup2->get_weight()) {
            return false;
        }else if(sup1->get_label()==nullptr && sup2->get_label()!=nullptr || sup1->get_label()!=nullptr && sup2->get_label()==nullptr) {
            return false;
        }else if(sup1->get_label()!=nullptr && sup2->get_label()!=nullptr && *(sup1->get_label()) != *(sup2->get_label())) {
            return false;
        }else if (sup1->get_children().empty() != sup2->get_children().empty()) {
            return false;
        }else if(!compare_children(*sup1, *sup2)) {
            return false;
        }
            ++sup1;
            ++sup2;
    }
    if(sup1!=sup2)
        return false;
    return true;
}
*/


template <typename T>
bool trie<T>::operator!=(trie<T> const& in) const {
    return !(*this==in);
};

template <typename T>
trie<T>& trie<T>::operator[](std::vector<T> const& in){
    if(this->get_children().empty() || in.empty()) {
        return *this;
    }else{
        auto sup=m_c.get_first();
        while(sup!=nullptr && *sup->data.get_label() != in.at(0)){
            sup=sup->next;
        }
        if(sup==nullptr) {
            return *this;
        }else{
            std::vector<T> sub(in.begin()+1, in.end());
            return sup->data[sub];
        }
    }
};

template <typename T>
trie<T> const& trie<T>::operator[](std::vector<T> const& in) const {
    if(this->get_children().empty()) {
        return *this;
    }else{
        auto sup=m_c.get_first();
        while(sup!=nullptr && *sup->data.get_label()!=in.at(0)){
            sup=sup->next;
        }

        if(sup==nullptr) {
            return *this;
        }else{
            std::vector<T> sub(in.begin()+1, in.end());
            return sup->data[sub];
        }
    }
}

template<typename T>
trie<T> & trie<T>::max() {
    leaf_iterator it = this->begin();
    trie<T>* maximum=&it.get_leaf();
    double max_num=maximum->get_weight();
    ++it;
    for (; it != this->end(); ++it) {
        trie<T>* temp=&it.get_leaf();
        double temp_max=temp->get_weight();

        if(max_num < temp_max) {
            max_num = temp_max;
            maximum=temp;
        }
    }
    return *maximum;
}

template<typename T>
trie<T> const & trie<T>::max() const {
    const_leaf_iterator it = this->begin();
    const trie<T>* maximum=&it.get_leaf();
    double max_num=maximum->get_weight();
    ++it;
    for (; it != this->end(); ++it) {
        const trie<T>* temp=&it.get_leaf();
        double temp_max=temp->get_weight();

        if(max_num < temp_max) {
            max_num = temp_max;
            maximum=temp;
        }
    }
    return *maximum;
}


///iteratori

template<typename T>
trie<T>::node_iterator::node_iterator(trie<T>* in) {
    m_ptr=in;
}

template<typename T>
trie<T>::const_node_iterator::const_node_iterator(trie<T> const *in) {
    m_ptr=in;
}

template<typename T>
typename trie<T>::node_iterator::reference trie<T>::node_iterator::operator*() const {
    return *(m_ptr->get_label());
}

template<typename T>
typename trie<T>::const_node_iterator::reference trie<T>::const_node_iterator::operator*() const {
    return *(m_ptr->get_label());
}

template<typename T>
typename trie<T>::node_iterator::pointer trie<T>::node_iterator::operator->() const {
    return m_ptr->get_label();
}

template<typename T>
typename trie<T>::const_node_iterator::pointer trie<T>::const_node_iterator::operator->() const {
    return m_ptr->get_label();
}

template<typename T>
typename trie<T>::node_iterator & trie<T>::node_iterator::operator++() {
    m_ptr=m_ptr->m_p;
    return *this;
}

template<typename T>
typename trie<T>::const_node_iterator & trie<T>::const_node_iterator::operator++() {
    m_ptr=m_ptr->m_p;
    return *this;
}

template<typename T>
typename trie<T>::node_iterator trie<T>::node_iterator::operator++(int) {
    auto copy = *this;
    ++(*this);
    return copy;
}

template<typename T>
typename trie<T>::const_node_iterator trie<T>::const_node_iterator::operator++(int) {
    auto copy = *this;
    ++(*this);
    return copy;
}

template<typename T>
bool trie<T>::node_iterator::operator==(node_iterator const & in) const {
    return m_ptr==in.m_ptr;
}

template<typename T>
bool trie<T>::const_node_iterator::operator==(const_node_iterator const & in) const {
    return m_ptr==in.m_ptr;
}

template<typename T>
bool trie<T>::node_iterator::operator!=(node_iterator const & in) const {
    return m_ptr!=in.m_ptr;
}


template<typename T>
bool trie<T>::const_node_iterator::operator!=(const_node_iterator const & in) const {
    return m_ptr!=in.m_ptr;
}

template<typename T>
typename trie<T>::node_iterator trie<T>::root() {
    trie<T>* start=this;
    while(start->get_parent()!=nullptr) {
        start=start->get_parent();//primo andiamo a sx
    }
    node_iterator ret(start);
    return ret;
}

template<typename T>
typename trie<T>::const_node_iterator trie<T>::root() const {
    auto start=this;
    while(start->get_parent()!=nullptr) {
        start=start->get_parent();//primo andiamo a sx
    }
    const_node_iterator ret(start);
    return ret; //toericamente sale tutto l'alberozzz
}

///leaf

template<typename T>
typename trie<T>::leaf_iterator trie<T>::begin() { //ritorna iteratore
    trie<T>* start=this;
    while(!start->get_children().empty()) {
        start=&(start->get_children().get_first()->data);//primo andiamo a sx
    }
    leaf_iterator ret(start);
    return ret;
}

template<typename T>
typename trie<T>::const_leaf_iterator trie<T>::begin() const {
    const trie<T>* start=this;
    while(!start->get_children().empty()) {
        start=&(start->get_children().get_first()->data);//primo andiamo a sx
    }
    const_leaf_iterator ret(start);
    return ret;
}

template<typename T>
typename trie<T>::leaf_iterator trie<T>::end() {
    leaf_iterator ret(nullptr);
    return ret;
}

template<typename T>
typename trie<T>::const_leaf_iterator trie<T>::end() const {
    const_leaf_iterator ret(nullptr);
    return ret;
}

template<typename T>
trie<T>::leaf_iterator::leaf_iterator(trie<T> *in) {
    m_ptr=in;
}

template<typename T>
trie<T>::const_leaf_iterator::const_leaf_iterator(trie<T> const *in) {
    m_ptr=in;
}

template<typename T>
typename trie<T>::leaf_iterator::reference trie<T>::leaf_iterator::operator*() const {
    return *(m_ptr->get_label());
}

template<typename T>
typename trie<T>::const_leaf_iterator::reference trie<T>::const_leaf_iterator::operator*() const {
    return *(m_ptr->get_label());
}

template<typename T>
typename trie<T>::leaf_iterator::pointer trie<T>::leaf_iterator::operator->() const {
    return m_ptr->get_label();
}

template<typename T>
typename trie<T>::const_leaf_iterator::pointer trie<T>::const_leaf_iterator::operator->() const {
    return m_ptr->get_label();
}

template<typename T>
trie <T>* trova_foglia(const trie<T>*  in){ //primo nodo dei fratm
    if(in->get_parent() != nullptr) {
        auto supp=in->get_parent()->get_children().get_first(); //trovo bag dove è dentro il trie
        while(in != &supp->data) {
            supp=supp->next; //itero
        }
        supp=supp->next; //scorro a bag+1
        if(supp==nullptr) {
            return trova_foglia(in->get_parent()); //se ho finito i nodi della bag salgo
        }else {
            while(!supp->data.get_children().empty()) { //guardo se il nodo è foglia
                supp=supp->data.get_children().get_first(); //se non è foglia scendo al primo suo nodo figlio, scendo e scendo
            }
            return &supp->data; //ritorno quello che c'è dentro il nodo ovvero la foglia
        }
    }else{ //se radice
        return nullptr;
    }
}

template<typename T>
typename trie<T>::leaf_iterator & trie<T>::leaf_iterator::operator++() {
    auto supp=trova_foglia<T>(m_ptr); //trovo la prossima foglia
    if(supp==nullptr) {
        m_ptr=nullptr; //se torno alla radice è null
    }else if(supp->get_children().empty()) {
        m_ptr=supp;
    }
    return *this;
}

template<typename T>
typename trie<T>::const_leaf_iterator & trie<T>::const_leaf_iterator::operator++() {
    auto supp=trova_foglia<T>(m_ptr); //trovo la prossima foglia
    if(supp==nullptr) {
        m_ptr=nullptr; //se torno alla radice è null
    }else if(supp->get_children().empty()) {
        m_ptr=supp;
    }
    return *this;
}

template<typename T>
typename trie<T>::leaf_iterator trie<T>::leaf_iterator::operator++(int) {
    auto copy = *this;
    ++(*this);
    return copy;
}

template<typename T>
typename trie<T>::const_leaf_iterator trie<T>::const_leaf_iterator::operator++(int) {
   auto copy = *this;
    ++(*this);
    return copy;
}

template<typename T>
bool trie<T>::leaf_iterator::operator==(leaf_iterator const & in) const {
    return m_ptr==in.m_ptr;
}

template<typename T>
bool trie<T>::const_leaf_iterator::operator==(const_leaf_iterator const & in) const {
    return m_ptr==in.m_ptr;
}

template<typename T>
bool trie<T>::leaf_iterator::operator!=(leaf_iterator const &in) const {
    return m_ptr!=in.m_ptr;
}

template<typename T>
bool trie<T>::const_leaf_iterator::operator!=(const_leaf_iterator const & in) const {
    return m_ptr!=in.m_ptr;
}


template<typename T>
trie<T>::leaf_iterator::operator node_iterator() const {
    return node_iterator(m_ptr);
}

template<typename T>
trie<T>::const_leaf_iterator::operator const_node_iterator() const {
    return const_node_iterator(m_ptr);
}

template<typename T>
trie<T> & trie<T>::leaf_iterator::get_leaf() const {
    return *m_ptr;
}
template<typename T>
trie<T> const & trie<T>::const_leaf_iterator::get_leaf() const {
    return *m_ptr;
}

template <typename T>
int dept(trie<T> const &  a) {
    int supp=0;
    const trie <T>* dummy=a.get_parent();
    while(dummy!=nullptr) {
        supp++;
        dummy=dummy->get_parent();
    }
    return supp;
}

template<typename T>
std::istream & operator>>(std::istream & is, trie<T>& in) {
    trie<T> a;
    in=a;//svuota l'oggetto prima di incollarci cdentro cose

    children(is,in);
    return is;
}

template<typename T>
std::ostream & operator<<(std::ostream & os, trie<T> const & tr) {
    for(int i=0;i <dept(tr);i++) {
        os<<"\t";
    }
    if(tr.get_label()!=nullptr)
        os<<*tr.get_label()<<" ";
    if(tr.get_weight() != 0)
        os<<tr.get_weight()<<" ";
    os<<"children = { ";
    if(tr.get_children().empty() == false ) {
        os<<"\n";
        os<<tr.get_children();
        for(int i=0;i <dept(tr);i++) {
            os<<"\t";
        }
        os<<"}";
    }else {
        os<<"}";
    }
    //os<<"\n";
    return os;
}



