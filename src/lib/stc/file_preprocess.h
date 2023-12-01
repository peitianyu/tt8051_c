#ifndef __FILE_PREPROCESS_H__
#define __FILE_PREPROCESS_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include <map>

class FilePreprocess
{
public:
    FilePreprocess() : m_curr_path("") {}

    void run(const std::string& file_in)
    {   
        if(m_curr_path.empty()) m_curr_path = file_in.substr(0, file_in.find_last_of('/') + 1);

        parse_include(file_in);

        remove_comment();
        tidy_code();
        concat_space();
        concat_line();
        parse_define();
    }

    void output(const std::string& file_out)
    {
        std::ofstream ofs(file_out);
        while(!m_ss.eof())
        {
            char c = m_ss.get();
            if(c != EOF) ofs << c, std::cout << c;
        }
        ofs.close();
    }
private:
    void remove_comment()
    {
        std::string str;
        std::stringstream ss;
        while(!m_ss.eof())
        {
            char c = m_ss.get();
            if(c == '/') {
                char c2 = m_ss.get();
                if(c2 == '/') while(m_ss.get() != '\n');
                else if(c2 == '*') {
                    while(m_ss.get() != '*' && m_ss.get() != '/');
                    m_ss.get();
                }
                else if(c != EOF) ss << c << c2;
            }
            else
            {
                if(c != EOF) ss << c;
            }
        }
        m_ss = std::stringstream();
        m_ss << ss.str();
    }

    // 整理代码
    void tidy_code()
    {
        std::string str;
        std::stringstream ss;
        while(!m_ss.eof())
        {
            std::getline(m_ss, str);
            if(str.size() == 0) continue;

            // 若是'/t', 替换成' '
            for(uint i = 0; i < str.size(); i++) {
                if(str[i] == '\t') str[i] = ' ';
            }
            
            // 若全为' '，则删除
            bool is_empty = true;
            for(uint i = 0; i < str.size(); i++) {
                if(str[i] != ' ') {
                    is_empty = false;
                    break;
                }
            }

            // 删除字母前的空格
            if(!is_empty) {
                int i = 0;
                while(str[i] == ' ') i++;
                ss << str.substr(i) << std::endl;
            }
        }
        m_ss = std::stringstream();
        m_ss << ss.str();
    }

    // 拼接空格
    void concat_space()
    {
        std::string str;
        std::stringstream ss;
        while(std::getline(m_ss, str))
        {
            std::vector<std::string> vec = split(str);
            for(uint i = 0; i < vec.size(); i++) {
                ss << vec[i];
                if(i != vec.size() - 1) ss << ' ';
            }
            ss << std::endl;
        }
        m_ss = std::stringstream();
        m_ss << ss.str();
    }

    // 拼接换行符
    void concat_line(char c = '\\')
    {
        std::string str;
        std::stringstream ss;
        while(!m_ss.eof())
        {
            char ch = m_ss.get();
            if(ch == c) {
                std::getline(m_ss, str);
                ss << str;
            }
            else ss << ch;
        }
        m_ss = std::stringstream();
        m_ss << ss.str();
    }

    // #define, 并替换
    void parse_define()
    {
        std::string line;
        std::stringstream ss;
        while(!m_ss.eof())
        {
            std::getline(m_ss, line);
            ss << line << std::endl;
            // 检查#define
            if(line.size() > 7 && line.substr(0, 7) == "#define") 
            {
                std::vector<std::string> define_vec = split(line.substr(7));
            }
        }
        m_ss = std::stringstream();
        m_ss << ss.str();
    }

    void parse_include(const std::string& file_in)
    {
        
        // std::cout << "file_in: " << file_in << std::endl;
        if(m_include_set.find(file_in) != m_include_set.end()) return;

        // FIXME: 有可能会导致一些不可预料的错误, 之后一定加上判断
        m_ss << file_in << "{\n";

        std::ifstream ifs(file_in);

        if(!ifs.is_open()) {
            std::cout << "file open failed: " << file_in << std::endl;
            return;
        }

        while(!ifs.eof())
        {
            std::string line;
            std::getline(ifs, line);
            std::vector<std::string> vec = split(line);
            if(vec.size() > 0 && vec[0] == "#include") {
                std::string include_file = m_curr_path+find_middle_str(line);
                std::cout << "include_file: " << include_file << std::endl;
                run(include_file);
                m_include_set.insert(include_file);
            }else{
                m_ss << line << std::endl;
            }
        }
        ifs.close();

        m_ss << "}";
    }
private:
    // 通过' '分割字符串 
    std::vector<std::string> split(const std::string& str)
    {
        std::vector<std::string> vec;
        std::string tmp;
        for(uint i = 0; i < str.size(); i++)
        {
            if(str[i] == ' ' || str[i] == '\t') {
                if(tmp.size() > 0) {
                    vec.push_back(tmp);
                    tmp.clear();
                }
            }
            else tmp += str[i];
        }
        if(tmp.size() > 0) vec.push_back(tmp);
        return vec;
    }

    // 找出中间的字符串
    std::string find_middle_str(const std::string& str, char c1 = '"', char c2 = '"')
    {
        std::string tmp;
        bool is_find = false;
        for(uint i = 0; i < str.size(); i++)
        {
            if(str[i] == c1) is_find = true;
            else if(str[i] == c2) break;
            else if(is_find) tmp += str[i];
        }
        return tmp;
    }
private:
    std::stringstream m_ss;
    std::string m_curr_path;

    std::set<std::string> m_include_set;
};




#endif // __FILE_PREPROCESS_H__