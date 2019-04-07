#ifndef CONFIGREADER_H
#define CONFIGREADER_H
#include<string>
#include<unordered_map>
#include<vector>
#include<tuple>
#include<fstream>
/*
template <class T1, class T2>
struct SameType
{
    static const bool value = false;
};

template<class T>
struct SameType<T, T>
{
    static const bool value = true;
};

template <typename... Types>
class VectorHolder
{
private:
    typedef std::tuple<std::vector<Types>...> vtype;
    vtype vectors;

    template<int N, typename T>
    struct VectorOfType: SameType<T,
        typename std::tuple_element<N, vtype>::type::value_type>
    { };

    template <int N, class T, class Tuple,
              bool Match = false> // this =false is only for clarity
    struct MatchingField
    {
        static std::vector<T>& get(Tuple& tp)
        {
            // The "non-matching" version
            return MatchingField<N+1, T, Tuple,
                   VectorOfType<N+1, T>::value>::get(tp);
        }
    };

    template <int N, class T, class Tuple>
    struct MatchingField<N, T, Tuple, true>
    {
        static std::vector<T>& get(Tuple& tp)
        {
            return std::get<N>(tp);
        }
    };
public:
    template <typename T>
    std::vector<T>& access()
    {
        return MatchingField<0, T, vtype,
               VectorOfType<0, T>::value>::get(vectors);
    }
};*/


class ConfigReader
{
    public:
        ConfigReader(const std::string& file_location);
        bool open(const std::string& file_location);
        void elaborate();
        template <typename T>
        std::vector<T> get(const std::string& name);
        template <typename T>
        std::vector<T> get(char* name);
        bool close();
        void reload();
        void print();

    private:
        std::vector<std::string> line_vector;
        std::unordered_map<std::string, std::vector<std::string> > name_map;
        std::ifstream file_data;
        bool is_open=false;
        bool is_loaded=false;
};

//https://stackoverflow.com/questions/27941661/generating-one-class-member-per-variadic-template-argument
#endif // CONFIGREADER_H
