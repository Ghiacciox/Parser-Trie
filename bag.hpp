template <typename T>
struct bag {
private:
    struct node {
        node() : data(), next(nullptr), prev(nullptr) {};
        node(T const& d, node* n, node* p) : data(d), next(n), prev(p) {};

        ~node() {
            next=nullptr;
            prev=nullptr;
        }

        T data; //int data
        node* next;
        node* prev;
    };
    node* first;
    node* last;

public:

    bag() : first(nullptr), last(nullptr) {}

    bag(bag const& in) : first(nullptr), last(nullptr) { ///modificata ultimo
        node* sup=in.first;
        while(sup != nullptr) {
            append(sup->data);
            sup = sup->next;
        }
    }

    bag(bag&& in){
        first = in.first;
        last = in.last;

        in.last=nullptr;
        in.first=nullptr;
    }

    void remove_first() {
        if(first!=nullptr) {
            node* supp=first;
            if(supp==last) {
                last=nullptr; //teoricamenete non leak
            }
            first=first->next;
            delete(supp);
        }
    }

    ~bag() {
        while(first!=nullptr){
            remove_first();
        }
    };

    bag& operator=(bag const& in){
        if(this == &in)
            return *this;

        while(first!=nullptr) {
            remove_first();
        }

        first=nullptr;
        last=nullptr;

        node* supp=in.first;
        while(supp!=nullptr) {
            append(supp->data);
            supp=supp->next;
        }
        return *this;

    };

    bag& operator=(bag&& in){
        if(this == &in)
            return *this;
        while(first!=nullptr) {
            remove_first();
        }
        first=in.first;
        last=in.last;
        in.first=nullptr;
        in.last=nullptr;
        return *this;
    };


    bool operator==(const bag<T>& in) const {
        auto sup1 = begin();
        auto sup2 = in.begin();
        while (sup1!=end() && sup2!=in.end()) {
            if (*sup1 == *sup2) {
                ++sup1;
                ++sup2;
            }else {
                return false;
            }
        }
        if(sup1!=sup2) {
            return false;
        }
        return true;
    }

    bool operator!=(bag& in) {
         return !(*this==in);
    };

    bool operator!=(bag const& in) {
        return !(*this==in);
    };

    bool operator!=(const bag<T>& in) const {
        return !(*this==in);
    };

    bool empty() const {
        if(first==nullptr)
            return true;
        return false;
    }

    node* get_first(){
        return first;
    }

    node* get_first() const{
        return first;
    }

    node* get_last(){
        return last;
    }

    node* get_last() const{
        return last;
    }

    void append(T const& d) {
        if(first==nullptr) {
            first=new node();
            first->data=d;
            last=first;
        }else {
            node* supp=this->first;
            bool control=true;
            while(supp!=nullptr && control){
                if(*d.get_label()==*supp->data.get_label()) {
                    throw std::runtime_error("Label already present ");
                }
                if(*d.get_label()<*supp->data.get_label()) {
                    node* insert=new node();
                    insert->data=d;
                    if(supp==first) {
                        first->prev=insert;
                        insert->next=first;
                        first=insert;
                        control=false;
                    }else{
                        insert->next=supp;
                        insert->prev=supp->prev;
                        supp->prev->next=insert;
                        supp->prev=insert;
                        control=false;
                    }
                }else if(supp==last){
                    node* insert=new node();
                    insert->data=d;
                    last->next=insert;
                    insert->prev=last;
                    last=insert;
                    control=false;
                }
                supp=supp->next;
            }
        }
    };


    ::bag<T>::node * first1() const {
        return first;
    }

    void set_first(::bag<T>::node *first) {
        this->first = first;
    }

    ::bag<T>::node * last1() const {
        return last;
    }

    void set_last(::bag<T>::node *last) {
        this->last = last;
    }

    /// ITERATORIIII BAG

    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        iterator(node* ptr): m_ptr(ptr) {};

        reference operator*() const {
            return m_ptr->data;
        };

        pointer operator->() const {
            return &(m_ptr->data);
        };

        iterator& operator++() {
            m_ptr = m_ptr->next;
            return *this;
        };

        iterator operator++(int /* dummy */) {
            auto copy = *this;
            m_ptr = m_ptr->next;
            return copy;
        };

        bool operator==(iterator const& rhs) const {
            return m_ptr == rhs.m_ptr;
        };
        bool operator!=(iterator const& rhs) const {
            return !(*this == rhs);
        };

        operator bool() const {
            return m_ptr != nullptr;
        };

    private:
        node* m_ptr;
    };

    struct const_iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = const T;
        using pointer = T const*;
        using reference = T const&;

        const_iterator(node* ptr) : m_ptr(ptr) {};

        reference operator*() const {
            return m_ptr->data;
        };

        pointer operator->() const {
            return &(m_ptr->data);
        };

        const_iterator& operator++() {
            m_ptr = m_ptr->next;
            return *this;
        };

        const_iterator operator++(int /* dummy */) {
            auto copy = *this;
            m_ptr = m_ptr->next;
            return copy;
        };

        bool operator==(const_iterator const& rhs) const {
            return m_ptr == rhs.m_ptr;
        };

        bool operator!=(const_iterator const& rhs) const {
            return !(*this == rhs);
        };

        operator bool() const {
            return m_ptr != nullptr;
        };

    private:
        node const* m_ptr;
    };


    iterator begin() {
        return {first};
    };

    iterator end() {
        return {nullptr};
    };

    const_iterator begin() const {
        return {first};
    };

    const_iterator end() const {
        return {nullptr};
    };


     std::size_t size() const {
        std::size_t count = 0;
        node* current = first;
        while (current != nullptr) {
            ++count;
            current = current->next;
        }
        return count;
    }

};

    template<typename T>
    std::ostream & operator<<(std::ostream & os, bag <T>const & tr) {
        auto* supp=tr.get_first();
        for (auto it = tr.begin(); it !=tr.end(); ++it) {
            os<<supp->data;
            if(supp!=tr.get_last())
                os << ",";
            supp=supp->next;
            os<< "\n";
        }
        return os;
    }


/*
 * Your implementation of the container bag<Val>.
 * Write both the declaration and definition of the
 * container in this file.
 */

