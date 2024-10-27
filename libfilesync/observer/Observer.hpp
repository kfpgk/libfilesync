#ifndef LIBFILESYNC_OBSERVER_HPP
#define LIBFILESYNC_OBSERVER_HPP

namespace filesync {

    /**
     * @brief Generic base observer interface class
     * 
     * Non-virtual interface
     */
    template <class T>
    class Observer {

        public:
            virtual ~Observer() = default;
            void update(T* t = nullptr) { doUpdate(t); }
            [[nodiscard]] int getId() const { return id; }            

        protected:
            Observer() : id {Observer::globalInstCounter++} {}

        private:
            int id = 0;
            static int globalInstCounter;

            virtual void doUpdate(T* t = nullptr) = 0;

    };

    template <class T>
    int Observer<T>::globalInstCounter = 0;

}

#endif