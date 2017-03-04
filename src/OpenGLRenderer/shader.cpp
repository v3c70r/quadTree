#include <common/log.hpp>
#include "shader.hpp"
#include <iostream>
using namespace LOG;
bool Shader::parseFileToStr_(const char* file_name, char* shader_str,
                             int max_len)
{
    shader_str[0] = '\0';  // reset string
    FILE* file = fopen(file_name, "r");
    if (!file) {
        writeLogErr("Unable to open file: %s\n", file_name);
        return false;
    }
    int current_len = 0;
    char line[2048];
    strcpy(line, "");  // remember to clean up before using for first time!
    while (!feof(file)) {
        if (NULL != fgets(line, 2048, file)) {
            current_len += strlen(line);  // +1 for \n at end
            if (current_len >= max_len) {
                writeLogErr(
                    "ERROR: shader length is longer than string buffer length "
                    "%i\n",
                    max_len);
            }
            strcat(shader_str, line);
        }
    }
    if (EOF == fclose(file)) {  // probably unnecesssary validation
        writeLogErr("ERROR: closing file from reading %s\n", file_name);
        return false;
    }
    return true;
}

void Shader::printShaderInfoLog_(GLuint shader_index)
{
    int max_length = 2048;
    int actual_length = 0;
    char log[2048];
    glGetShaderInfoLog(shader_index, max_length, &actual_length, log);
    printf("shader info log for GL index %i:\n%s\n", shader_index, log);
    writeLog("shader info log for GL index %i:\n%s\n", shader_index, log);
}
bool Shader::createShader_(const char* file_name, GLuint* shader, GLenum type)
{
    writeLog("creating shader from %s...\n", file_name);
    char shader_string[MAX_SHADER_LENGTH];
    assert(parseFileToStr_(file_name, shader_string, MAX_SHADER_LENGTH));
    *shader = glCreateShader(type);
    const GLchar* p = (const GLchar*)shader_string;
    glShaderSource(*shader, 1, &p, NULL);
    glCompileShader(*shader);
    // check for compile errors
    int params = -1;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params) {
        writeLogErr("ERROR: GL shader index %i did not compile\n", *shader);
        printShaderInfoLog_(*shader);
        return false;  // or exit or something
    }
    writeLog("shader compiled. index %i\n", *shader);
    return true;
}
bool Shader::isProgrammeValid_(GLuint sp)
{
    glValidateProgram(sp);
    GLint params = -1;
    glGetProgramiv(sp, GL_VALIDATE_STATUS, &params);
    if (GL_TRUE != params) {
        writeLogErr("program %i GL_VALIDATE_STATUS = GL_FALSE\n", sp);
        printProgrammeInfoLog_(sp);
        return false;
    }
    writeLog("program %i GL_VALIDATE_STATUS = GL_TRUE\n", sp);
    return true;
}
void Shader::printProgrammeInfoLog_(GLuint sp)
{
    int max_length = 2048;
    int actual_length = 0;
    char log[2048];
    glGetProgramInfoLog(sp, max_length, &actual_length, log);
    printf("program info log for GL index %u:\n%s", sp, log);
    writeLog("program info log for GL index %u:\n%s", sp, log);
}
bool Shader::createProgramme_(GLuint vert, GLuint geom, GLuint frag,
                              GLuint* programme)
{
    *programme = glCreateProgram();
    writeLog("created programme %u. attaching shaders %u, %u and %u...\n",
             *programme, vert, geom, frag);
    glAttachShader(*programme, vert);
    glAttachShader(*programme, geom);
    glAttachShader(*programme, frag);
    // link the shader programme. if binding input attributes do that before
    // link
    glLinkProgram(*programme);
    GLint params = -1;
    glGetProgramiv(*programme, GL_LINK_STATUS, &params);
    if (GL_TRUE != params) {
        writeLogErr("ERROR: could not link shader programme GL index %u\n",
                    *programme);
        printProgrammeInfoLog_(*programme);
        return false;
    }
    assert(isProgrammeValid_(*programme));
    // delete shaders here to free memory
    glDeleteShader(vert);
    glDeleteShader(geom);
    glDeleteShader(frag);
    return true;
}
bool Shader::createProgramme_(GLuint compute, GLuint* programme)
{
    *programme = glCreateProgram();
    writeLog("created compute programme %u. attaching shaders %u ...\n",
             *programme, compute);
    glAttachShader(*programme, compute);
    glLinkProgram(*programme);
    GLint params = -1;
    glGetProgramiv(*programme, GL_LINK_STATUS, &params);
    glGetProgramiv(*programme, GL_LINK_STATUS, &params);
    if (GL_TRUE != params) {
        writeLogErr("ERROR: could not link shader programme GL index %u\n",
                    *programme);
        printProgrammeInfoLog_(*programme);
        return false;
    }
    glDeleteShader(compute);
    return true;
}
bool Shader::createProgramme_(GLuint vert, GLuint frag, GLuint* programme)
{
    *programme = glCreateProgram();
    writeLog("created programme %u. attaching shaders %u and %u...\n",
             *programme, vert, frag);
    glAttachShader(*programme, vert);
    glAttachShader(*programme, frag);
    // link the shader programme. if binding input attributes do that before
    // link
    glLinkProgram(*programme);
    GLint params = -1;
    glGetProgramiv(*programme, GL_LINK_STATUS, &params);
    if (GL_TRUE != params) {
        writeLogErr("ERROR: could not link shader programme GL index %u\n",
                    *programme);
        printProgrammeInfoLog_(*programme);
        return false;
    }
    assert(isProgrammeValid_(*programme));
    // delete shaders here to free memory
    glDeleteShader(vert);
    glDeleteShader(frag);
    return true;
}
bool Shader::createProgrammeFromFiles(const char* kernel_file_name)
{
    GLuint computeShader;
    assert(createShader_(kernel_file_name, &computeShader, GL_COMPUTE_SHADER));
    assert(createProgramme_(computeShader, &programme_));
    return true;
}

bool Shader::createProgrammeFromFiles(const char* vert_file_name,
                                      const char* geometry_file_name,
                                      const char* frag_file_name)
{
    GLuint vertShader;
    GLuint geometryShader;
    GLuint fragShader;
    assert(createShader_(vert_file_name, &vertShader, GL_VERTEX_SHADER));
    assert(
        createShader_(geometry_file_name, &geometryShader, GL_GEOMETRY_SHADER));
    assert(createShader_(frag_file_name, &fragShader, GL_FRAGMENT_SHADER));
    assert(
        createProgramme_(vertShader, geometryShader, fragShader, &programme_));
    return true;
}
bool Shader::createProgrammeFromFiles(const char* vert_file_name,
                                      const char* frag_file_name)
{
    GLuint dummyVAO;
    glGenVertexArrays(1, &dummyVAO);
    glBindVertexArray(dummyVAO);
    GLuint vertShader;
    GLuint fragShader;
    assert(createShader_(vert_file_name, &vertShader, GL_VERTEX_SHADER));
    assert(createShader_(frag_file_name, &fragShader, GL_FRAGMENT_SHADER));
    assert(createProgramme_(vertShader, fragShader, &programme_));
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &dummyVAO);
    return true;
}

void Shader::getAttributes()
{
    GLint count;
    glGetProgramiv(programme_, GL_ACTIVE_UNIFORMS, &count);
    printf("Active Attributes: %d\n", count);
    char buffer[100];
    GLint length;
    GLenum type;
    GLint size = 100;

    for (GLint i = 0; i < count; i++) {
        glGetActiveAttrib(programme_, (GLuint)i, 100, &length, &size, &type,
                           buffer);
        GLint loc = glGetAttribLocation(programme_, buffer);

        printf("getAttributes #%d Type: %u Name: %s\n", loc, type, buffer);
    }
}
void Shader::getUniforms()
{
    GLint count;
    glGetProgramiv(programme_, GL_ACTIVE_UNIFORMS, &count);
    printf("Active Uniforms: %d\n", count);
    char buffer[100];
    GLint length;
    GLenum type;
    GLint size = 100;

    for (GLint i = 0; i < count; i++) {
        glGetActiveUniform(programme_, (GLuint)i, 100, &length, &size, &type,
                           buffer);

        printf("Uniform #%d Type: %u Name: %s\n", i, type, buffer);
    }
}

bool Shader::use() const
{
    glUseProgram(programme_);
    return true;
}
