//**********************************
// Compiler
// 21/09/2012 - 21/09/2012
//**********************************
// Christophe Riccio
// ogl-samples@g-truc.net
//**********************************
// G-Truc Creation
// www.g-truc.net
//**********************************

#include "framework/opengl/compiler.h"
#include "qulkan/logger.h"

#include <glm/gtc/random.hpp>

#include <cstdarg>
#include <fstream>
#include <sstream>
#include <string>

#include <iostream>

std::string getDataDirectory() { return std::string("../data/shaders/"); }
std::string format(const char *Message, ...) {
    assert(Message);

    char Text[1024];
    std::memset(Text, 0, sizeof(Text));

    va_list ap;
    va_start(ap, Message);
    std::vsprintf(Text, Message, ap);
    va_end(ap);

    return Text;
}

Compiler::commandline::commandline(std::string const &Filename, std::string const &Arguments) : Profile("core"), Version(-1) {
    std::size_t PathOffset = Filename.find_last_of("/");
    std::string FilePath = Filename.substr(0, PathOffset + 1);

    this->Includes.push_back(FilePath);
    this->parseArguments(Arguments);
}

void Compiler::commandline::parseArguments(std::string const &Arguments) {
    std::stringstream Stream(Arguments);
    std::string Param;

    while (!Stream.eof()) {
        Stream >> Param;

        std::size_t FoundDefine = Param.find("-D");
        std::size_t FoundInclude = Param.find("-I");

        if (FoundDefine != std::string::npos)
            this->Defines.push_back(Param.substr(2, Param.size() - 2));
        else if (FoundInclude != std::string::npos)
            this->Includes.push_back(getDataDirectory() + Param.substr(2, Param.size() - 2));
        else if (Param == "--define") {
            std::string Define;
            Stream >> Define;
            this->Defines.push_back(Define);
        } else if ((Param == "--version") || (Param == "-v"))
            Stream >> Version;
        else if ((Param == "--profile") || (Param == "-p"))
            Stream >> Profile;
        else if (Param == "--include") {
            std::string Include;
            Stream >> Include;
            this->Includes.push_back(getDataDirectory() + Include);
        }
        /*
                        else
                        {
                                std::stringstream err;
                                err << "unknown parameter type: \"" << Param << "\"";
                                glDebugMessageInsertARB(
                                        GL_DEBUG_SOURCE_APPLICATION_ARB, GL_DEBUG_TYPE_OTHER_ARB, 1, GL_DEBUG_SEVERITY_LOW_ARB,
                                        -1, std::string(std::string("unknown parameter type: \"") << Param << std::string("\"")).c_str());
                        }
                        if(!Stream)
                        {
                                glDebugMessageInsertARB(
                                        GL_DEBUG_SOURCE_APPLICATION_ARB, GL_DEBUG_TYPE_OTHER_ARB, 1, GL_DEBUG_SEVERITY_LOW_ARB,
                                        -1, std::string(std::string("error parsing parameter: \"") << Param << std::string("\"")).c_str());
                        }
        */
    }
}

std::string Compiler::commandline::getDefines() const {
    std::string Result;
    for (std::size_t i = 0; i < this->Defines.size(); ++i)
        Result += std::string("#define ") + this->Defines[i] + std::string("\n");
    return Result;
}

// compiler::parser

std::string Compiler::parser::operator()(commandline const &CommandLine, std::string const &Filename) const {
    std::string Source = load_file(Filename);
    assert(!Source.empty());

    std::stringstream Stream;
    Stream << Source;
    std::string Line, Text;

    // Handle command line version and profile arguments
    if (CommandLine.getVersion() != -1)
        Text += format("#version %d %s\n", CommandLine.getVersion(), CommandLine.getProfile().c_str());

    // Handle command line defines
    Text += CommandLine.getDefines();

    while (std::getline(Stream, Line)) {
        std::size_t Offset = 0;

        // Version
        Offset = Line.find("#version");
        if (Offset != std::string::npos) {
            std::size_t CommentOffset = Line.find("//");
            if (CommentOffset != std::string::npos && CommentOffset < Offset)
                continue;

            // Reorder so that the #version line is always the first of a shader text
            if (CommandLine.getVersion() == -1)
                Text = Line + std::string("\n") + Text;
            // else skip is version is only mentionned
            continue;
        }

        // Include
        Offset = Line.find("#include");
        if (Offset != std::string::npos) {
            std::size_t CommentOffset = Line.find("//");
            if (CommentOffset != std::string::npos && CommentOffset < Offset)
                continue;

            std::string Include = parseInclude(Line, Offset);

            std::vector<std::string> Includes = CommandLine.getIncludes();

            for (std::size_t i = 0; i < Includes.size(); ++i) {
                std::string PathName = Includes[i] + Include;
                std::string IncludeSource = load_file(PathName);
                if (!IncludeSource.empty()) {
                    Text += IncludeSource;
                    break;
                }
            }

            continue;
        }

        Text += Line + "\n";
    }

    // Text += glf::format("\nconst float G_TRUC_GNI = %f;\n", glm::linearRand(0.0f, 1.0f));

    return Text;
}

std::string Compiler::parser::parseInclude(std::string const &Line, std::size_t const &Offset) const {
    std::string Result;

    std::string::size_type IncludeFirstQuote = Line.find("\"", Offset);
    std::string::size_type IncludeSecondQuote = Line.find("\"", IncludeFirstQuote + 1);

    return Line.substr(IncludeFirstQuote + 1, IncludeSecondQuote - IncludeFirstQuote - 1);
}

// compiler
Compiler::~Compiler() { this->clear(); }

GLuint Compiler::create(GLenum Type, std::string const &Filename, std::string const &Arguments) {
    assert(!Filename.empty());

    commandline CommandLine(Filename, Arguments);

    std::string PreprocessedSource = parser()(CommandLine, Filename);
    assert(!PreprocessedSource.empty());
    char const *PreprocessedSourcePointer = PreprocessedSource.c_str();

    // Qulkan::Logger::Info("%s\n", PreprocessedSource.c_str());

    GLuint Name = glCreateShader(Type);
    glShaderSource(Name, 1, &PreprocessedSourcePointer, NULL);
    glCompileShader(Name);

    std::pair<files_map::iterator, bool> ResultFiles = this->ShaderFiles.insert(std::make_pair(Name, Filename));
    assert(ResultFiles.second);
    std::pair<names_map::iterator, bool> ResultNames = this->ShaderNames.insert(std::make_pair(Filename, Name));
    assert(ResultNames.second);
    std::pair<names_map::iterator, bool> ResultChecks = this->PendingChecks.insert(std::make_pair(Filename, Name));
    assert(ResultChecks.second);

    return Name;
}

bool Compiler::destroy(GLuint const &Name) {
    files_map::iterator NameIterator = this->ShaderFiles.find(Name);
    if (NameIterator == this->ShaderFiles.end())
        return false; // Shader name not found
    std::string File = NameIterator->second;
    this->ShaderFiles.erase(NameIterator);

    // Remove from the pending checks list
    names_map::iterator PendingIterator = this->PendingChecks.find(File);
    if (PendingIterator != this->PendingChecks.end())
        this->PendingChecks.erase(PendingIterator);

    // Remove from the pending checks list
    names_map::iterator FileIterator = this->ShaderNames.find(File);
    assert(FileIterator != this->ShaderNames.end());
    this->ShaderNames.erase(FileIterator);

    return true;
}

bool Compiler::validate_program(GLuint ProgramName) const {
    if (!ProgramName)
        return false;

    glValidateProgram(ProgramName);
    GLint Result = GL_FALSE;
    glGetProgramiv(ProgramName, GL_VALIDATE_STATUS, &Result);

    if (Result == GL_TRUE)
        return true;

    Qulkan::Logger::Info("Validate program\n");
    // fprintf(stdout, "Validate program\n");
    int InfoLogLength;
    glGetProgramiv(ProgramName, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> Buffer(InfoLogLength);
        glGetProgramInfoLog(ProgramName, InfoLogLength, NULL, &Buffer[0]);
        Qulkan::Logger::Error("%s\n", &Buffer[0]);
    }

    return Result == GL_TRUE;
}

bool Compiler::check_program(GLuint ProgramName) const {
    if (!ProgramName)
        return false;

    GLint Result = GL_FALSE;
    glGetProgramiv(ProgramName, GL_LINK_STATUS, &Result);

    if (Result == GL_TRUE)
        return true;

    Qulkan::Logger::Info("Linking Program\n");
    int InfoLogLength;
    glGetProgramiv(ProgramName, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> Buffer(glm::max(InfoLogLength, int(1)));
        glGetProgramInfoLog(ProgramName, InfoLogLength, NULL, &Buffer[0]);
        Qulkan::Logger::Error("%s\n", &Buffer[0]);
    }

    return Result == GL_TRUE;
}

// TODO Interaction with KHR_debug
bool Compiler::check() {
    bool Success(true);

    for (names_map::iterator ShaderIterator = PendingChecks.begin(); ShaderIterator != PendingChecks.end(); ++ShaderIterator) {
        GLuint ShaderName = ShaderIterator->second;
        GLint Result = GL_FALSE;
        glGetShaderiv(ShaderName, GL_COMPILE_STATUS, &Result);

        if (Result == GL_TRUE)
            continue;

        int InfoLogLength;
        glGetShaderiv(ShaderName, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> Buffer(InfoLogLength);
            glGetShaderInfoLog(ShaderName, InfoLogLength, NULL, &Buffer[0]);
            Qulkan::Logger::Error("%s\n", &Buffer[0]);
        }

        Success = Success && Result == GL_TRUE;
    }

    return Success;
}

void Compiler::clear() {
    for (names_map::iterator ShaderNameIterator = this->ShaderNames.begin(); ShaderNameIterator != this->ShaderNames.end(); ++ShaderNameIterator)
        glDeleteShader(ShaderNameIterator->second);

    this->ShaderNames.clear();
    this->ShaderFiles.clear();
    this->PendingChecks.clear();
}

std::string load_file(std::string const &Filename) {
    std::string Result;

    std::ifstream Stream(Filename.c_str());
    if (!Stream.is_open())
        return Result;

    Stream.seekg(0, std::ios::end);
    Result.reserve(Stream.tellg());
    Stream.seekg(0, std::ios::beg);

    Result.assign((std::istreambuf_iterator<char>(Stream)), std::istreambuf_iterator<char>());

    return Result;
}

bool load_binary(std::string const &Filename, GLenum &Format, std::vector<glm::uint8> &Data, GLint &Size) {
    FILE *File = fopen(Filename.c_str(), "rb");

    if (File) {
        fread(&Format, sizeof(GLenum), 1, File);
        fread(&Size, sizeof(Size), 1, File);
        if (Size > 0) {
            Data.resize(Size);
            fread(&Data[0], Size, 1, File);
            return true;
        }
        fclose(File);
    }
    return false;
}

bool save_binary(std::string const &Filename, GLenum const &Format, std::vector<glm::uint8> const &Data, GLint const &Size) {
    FILE *File = fopen(Filename.c_str(), "wb");

    if (File) {
        fwrite(&Format, sizeof(GLenum), 1, File);
        fwrite(&Size, sizeof(Size), 1, File);
        fwrite(&Data[0], Size, 1, File);
        fclose(File);
        return true;
    }
    return false;
}
