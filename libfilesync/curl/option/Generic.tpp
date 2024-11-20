#include <utility>

namespace filesync::curl::option {

    /**
     * @brief Constructor that saves the variadic parameter pack
     * to a tuple for later usage.
     */
    template<typename... Args> 
    Generic<Args...>::Generic(filesync::curl::wrapper::Easy& curlEasy,
        CURLoption curlOption, Args&&... args) :
            Option(curlEasy),
            curlOption{curlOption},
            args{std::make_tuple(std::forward<Args>(args)...)} {

    }

    /**
     * @brief Calls generic setOption of a curl interface
     * 
     * Combine the previously saved variadic parameter pack
     * which is now available as a tuple with a lambda to
     * call the template setOption method of the interface.
     */
    template<typename... Args> 
    void Generic<Args...>::doSet() {
        auto optionAndArgs = std::tuple_cat(std::make_tuple(curlOption), args);
        std::apply([this](auto &&... args) 
            -> decltype(curlEasy.get().setOption(std::forward<decltype(args)>(args)...))
            { 
                curlEasy.get().setOption(std::forward<decltype(args)>(args)...); 
            },
            optionAndArgs);
    }

}