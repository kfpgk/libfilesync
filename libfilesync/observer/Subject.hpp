#ifndef LIBFILESYNC_SUBJECT_HPP
#define LIBFILESYNC_SUBJECT_HPP

#include <libfilesync/observer/Observer.hpp>

#include <list>
#include <functional>

namespace filesync {

    /**
     * @brief Generic base subject interface class
     */
    template <class T>
    class Subject {

        public:
            virtual ~Subject() = default;
            Subject(const Subject&) = delete;
            Subject& operator=(const Subject&) = delete;
            virtual void registerObserver(Observer<T>& observer) {
                observers.push_back(observer);
            }
            virtual void unregisterObserver(Observer<T>& observer) {
                observers.remove(observer);
            }
            virtual void notify() {
                for (auto const& i : observers) {
                    i.get().update(static_cast<T*>(this));
                }
            }

        protected:
            Subject() = default;

        private:
            std::list<std::reference_wrapper<Observer<T>>> observers;

    };

}

#endif