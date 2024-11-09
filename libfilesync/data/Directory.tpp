namespace filesync::data {

    template<typename T>
    DirectoryBase<T>::DirectoryBase(const std::filesystem::path& path) :
        EntryBase<T>{path} {

    }

    template<typename T>
    void DirectoryBase<T>::addEntry(std::shared_ptr<T> entry) {
        components.push_back(entry);
    }

    template<typename T>
    void DirectoryBase<T>::removeEntry(const std::filesystem::path& path) {
        doRemoveEntry(path);
    }

    template<typename T>
    void DirectoryBase<T>::doRemoveEntry(const std::filesystem::path& path) {
        for(auto it = components.begin(); it != components.end();) {
            if (it->get()->getPath() == path) {
                it = components.erase(it);
            } else {
                it++;
            }            
        }
    }

    template<typename T>
    void DirectoryBase<T>::registerObserver(Observer<T>& observer) {
        EntryBase<T>::registerObserver(observer);
        for(auto it = components.begin(); it != components.end(); it++) {
            it->get()->registerObserver(observer);
        }
    }

    template<typename T>
    void DirectoryBase<T>::unregisterObserver(Observer<T>& observer) {
        EntryBase<T>::unregisterObserver(observer);
        for(auto it = components.begin(); it != components.end(); it++) {
            it->get()->unregisterObserver(observer);
        }
    }

    template<typename T>
    void DirectoryBase<T>::notify() {
        EntryBase<T>::notify();
        for(auto it = components.begin(); it != components.end(); it++) {
            it->get()->notify();
        }       
    }    

    /**
     * @brief Composite getEntry() implementation
     */
    template<typename T>
    T* DirectoryBase<T>::doGetEntry(const std::filesystem::path& path) {
        if (EntryBase<T>::doGetEntry(path)) return dynamic_cast<T*>(EntryBase<T>::doGetEntry(path));
        for(auto it = components.begin(); it != components.end(); it++) {
            T* entry = it->get()->getEntry(path);
            if (entry) {
                return entry;
            }           
        }
        return nullptr;
    }

    template<typename T>
    void DirectoryBase<T>::doPerformOnChange() {
        EntryBase<T>::doPerformOnChange();
        for(auto it = components.begin(); it != components.end(); it++) {
            it->get()->performOnChange();
        }
    }

    template<typename T>
    bool DirectoryBase<T>::doValidate() const {
        bool valid = EntryBase<T>::doValidate();
        for(auto it = components.begin(); it != components.end(); it++) {
            if (!it->get()->validate()) {
                valid = false;
            }              
        }
        return valid;    
    }

    template<typename T>
    void DirectoryBase<T>::doPrint() const {
        EntryBase<T>::doPrint();
        for(auto it = components.begin(); it != components.end(); it++) {
            it->get()->print();        
        }        
    }

}