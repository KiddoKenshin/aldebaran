#ifndef DEFAULTSHADER_HPP
#define DEFAULTSHADER_HPP

#include <stdio.h>
#include <stdlib.h>
#include "opengl.hpp"
#include "printlog.hpp"
#include "shader_interface.hpp"

class DefaultShader : public ShaderInterface
{
    GLuint shader;
    
    GLint aPosition;
    GLint aTexCoord;
    GLint aColor;
    GLint aTile;
    GLint aTileNum;
    
    GLint uHasTexture;
    GLint uTexture;
    GLint uTransparency;
    GLint uActiveTile;
    
    static void printShaderInfoLog(GLuint obj)
    {
        int infologLength = 0;
        int charsWritten  = 0;
        char *infoLog;
        
        glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
        if (infologLength > 0)
        {
            infoLog = (char *)malloc(infologLength);
            glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
            printlog("%s\n",infoLog);
            free(infoLog);
        }
    }
    
    static GLuint SetUpShader()
    {
        const char* vertexshader =
        STRINGIFY(
            attribute vec3 aPosition;
            attribute vec2 aTexCoord;
            attribute vec4 aColor;
            attribute float aTile;
            attribute float aTileNum;
            
            varying vec2 vTexCoord;
            varying vec4 vColor;
            
            uniform float uActiveTile;
            
            void main()
            {
                // Transforming The Vertex
                gl_Position = vec4(aPosition, 1.0);
            
                // Pass the rest to fragment shader
                vTexCoord = aTexCoord;
                vColor = aColor;
                vTexCoord.x += mod(uActiveTile, aTileNum) * aTile;
            }
        );
        
        
        const char* fragmentshader =
#ifdef HAVE_OPENGLES
        STRINGIFY(
            precision mediump float;
        )
#endif
        
        STRINGIFY(
            varying vec2 vTexCoord;
            varying vec4 vColor;
            
            uniform float uHasTexture;
            uniform sampler2D uTexture;
            uniform float uTransparency;
            
            void main()
            {
                vec4 color = (1.0 - uHasTexture) * vColor;
                vec4 texcolor = uHasTexture * texture2D(uTexture, vTexCoord);
                gl_FragColor = color + texcolor;
                gl_FragColor.a *= uTransparency;
            }
        );
        
        GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
        
        glShaderSource(vshader, 1, &vertexshader, NULL);
        glShaderSource(fshader, 1, &fragmentshader, NULL);
        
        printf("Compiling Vertex Shader...\n");
        glCompileShader(vshader);
        printShaderInfoLog(vshader);
        
        printf("Compiling Fragment Shader...\n");
        glCompileShader(fshader);
        printShaderInfoLog(fshader);
        
        GLuint prog = glCreateProgram();
        glAttachShader(prog, fshader);
        glAttachShader(prog, vshader);
        
        glLinkProgram(prog);
        
        glDeleteShader(vshader);
        glDeleteShader(fshader);
        
        return prog;
    }
public:
    DefaultShader()
    {
        shader = SetUpShader();
        
        aPosition = glGetAttribLocation(shader, "aPosition");
        aTexCoord = glGetAttribLocation(shader, "aTexCoord");
        aColor = glGetAttribLocation(shader, "aColor");
        aTile = glGetAttribLocation(shader, "aTile");
        aTileNum = glGetAttribLocation(shader, "aTileNum");
        
        uActiveTile = glGetUniformLocation(shader, "uActiveTile");
        uHasTexture = glGetUniformLocation(shader, "uHasTexture");
        uTexture = glGetUniformLocation(shader, "uTexture");
        uTransparency = glGetUniformLocation(shader, "uTransparency");
    }
    
    ~DefaultShader()
    {
        glDeleteProgram(shader);
    }
    
    virtual GLuint GetProgram()
    {
        return shader;
    }
    
    virtual GLuint GetPositionAttribute()
    {
        return aPosition;
    }
    
    virtual GLuint GetColorAttribute()
    {
        return aColor;
    }
    
    virtual GLuint GetTileAttribute()
    {
        return aTile;
    }
    
    virtual GLuint GetTilenumAttribute()
    {
        return aTileNum;
    }
    
    virtual GLuint GetTexCoordAttribute()
    {
        return aTexCoord;
    }
    
    
    virtual void SetTransparencyUniform(GLfloat transp)
    {
        glUniform1f(uTransparency, transp);
    }
    
    virtual void SetActiveTileUniform(GLfloat activetile)
    {
        glUniform1f(uActiveTile, activetile);
    }
    
    virtual void SetTexturePresentUniform(bool hastexture)
    {
        glUniform1f(uHasTexture, hastexture ? 1.0f : 0.0f);
    }
    
    virtual void SetTextureUniform(GLuint tex)
    {
        glUniform1i(uTexture, tex);
    }
    
    virtual bool HasColorAttribute()
    {
        return true;
    }
    
    virtual bool HasTileAttribute()
    {
        return true;
    }

    virtual bool HasTileNumAttribute()
    {
        return true;
    }

    virtual bool HasTexCoordAttribute()
    {
        return true;
    }

    
    virtual bool HasTransparencyUniform()
    {
        return true;
    }

    virtual bool HasTextureUniform()
    {
        return true;
    }

    virtual bool HasActiveTileUniform()
    {
        return true;
    }

};

#endif // DEFAULTSHADER_HPP