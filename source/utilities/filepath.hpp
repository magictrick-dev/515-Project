#ifndef SOURCE_UTILITIES_FILEPATH_HPP
#define SOURCE_UTILITIES_FILEPATH_HPP
#include <definitions.hpp>

class filepath
{

    public:
                    filepath();
                    filepath(ccptr path);
                    filepath(string path);
                    filepath(const filepath& path);
        virtual    ~filepath();

        filepath&   operator=(const filepath& path);
        filepath&   operator=(ccptr path);
        filepath&   operator=(string path);

        bool        operator==(const filepath& path) const;
        bool        operator==(ccptr path) const;
        bool        operator==(string path) const;  
        
        bool        operator!=(const filepath& path) const;
        bool        operator!=(ccptr path) const;
        bool        operator!=(string path) const;

        filepath&   operator+=(ccptr path);
        filepath&   operator+=(string path);
        filepath&   operator+=(const filepath& path);

        bool        is_empty() const;
        bool        is_valid() const;
        bool        is_file() const;
        bool        is_directory() const;

        string      str() const;
        ccptr       c_str() const;

        string      get_directory() const;
        string      get_filename() const;
        string      get_extension() const;

        void        append(ccptr path);
        void        append(string path);
        void        append(const filepath& path);

        filepath    canonicalize() const;

        u64         get_file_size() const;

    protected:
        string      file_path;

};

#endif
