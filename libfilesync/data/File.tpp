namespace filesync::data {

    template<typename T>
    FileBase<T>::FileBase(const std::filesystem::path& path) :
        EntryBase<T>{path} {

    }

}