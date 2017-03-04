#pragma once
#include <GL/glew.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <vector>

#define MAX_SHADER_LENGTH (262144)

//! OpenGL Shader library
class Shader {
private:
    GLuint programme_;

    bool parseFileToStr_(const char* file_name, char* shader_str, int max_len);
    void printShaderInfoLog_(GLuint shader_index);
    bool createShader_(const char* file_name, GLuint* shader, GLenum type);
    bool isProgrammeValid_(GLuint sp);
    void printProgrammeInfoLog_(GLuint sp);
    bool createProgramme_(GLuint compute, GLuint* programme);
    bool createProgramme_(GLuint vert, GLuint geom, GLuint frag,
                          GLuint* programme);
    bool createProgramme_(GLuint vert, GLuint frag, GLuint* programme);

public:
    Shader() : programme_(0) { ; }
    bool createProgrammeFromFiles(const char* kernel_file_name);
    bool createProgrammeFromFiles(const char* vert_file_name,
                                  const char* geometry_file_name,
                                  const char* frag_file_name);
    bool createProgrammeFromFiles(const char* vert_file_name,
                                  const char* frag_file_name);
    bool use() const;
    GLuint programme() const {return programme_;}

    ~Shader() { if (programme_ != 0) glDeleteProgram(programme_); }
};
