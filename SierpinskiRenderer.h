#ifndef SIERPINSKIRENDERER_H
#define SIERPINSKIRENDERER_H



#include "raylib.h"
#include <vector>
#include <cmath>
#include <random>

class SierpinskiRenderer {
    struct Square {
        Vector2 vertices[4];
        float phase;
        RenderTexture2D renderTexture;
        Shader shader;
    };

    std::vector<Square> squares;
    int maxIterations;
    float timeSpeed;
    bool initialized;

    const char* vertexShaderSource = R"(
        #version 330
        in vec3 vertexPosition;
        in vec2 vertexTexCoord;
        out vec2 fragTexCoord;
        uniform mat4 mvp;
        void main() {
            fragTexCoord = vertexTexCoord;
            gl_Position = mvp * vec4(vertexPosition, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330
        in vec2 fragTexCoord;
        out vec4 finalColor;
        uniform float time;
        uniform float phase;
        uniform int maxIter;
        uniform float timeSpeed;

        int sierpinski(vec2 uv, int maxIterLocal) {
            vec2 pos = uv;
            for (int i = 0; i < maxIterLocal; i++) {
                pos *= 3.0;
                vec2 cell = floor(pos);
                pos = fract(pos);
                if (cell.x == 1.0 && cell.y == 1.0) {
                    return 0;
                }
            }
            return 1;
        }

        void main() {
            float animTime = time * timeSpeed + phase;
            int currentIter = int(mod(animTime, float(maxIter))) + 1;
            int fractal = sierpinski(fragTexCoord, currentIter);

            float hue = mod(animTime * 0.1 + phase * 0.5, 1.0);
            vec3 color1 = vec3(0.8 + 0.2 * sin(hue * 6.28),
                              0.6 + 0.4 * sin(hue * 6.28 + 2.09),
                              0.9 + 0.1 * sin(hue * 6.28 + 4.19));
            vec3 color2 = vec3(0.1, 0.1, 0.2);

            finalColor = vec4(mix(color2, color1, float(fractal)), 1.0);
        }
    )";

public:
    explicit SierpinskiRenderer(int maxIter = 8, float speed = 1.0f)
        : maxIterations(maxIter), timeSpeed(speed), initialized(false) {}

    ~SierpinskiRenderer() {
        cleanup();
    }

    void initialize(const std::vector<std::vector<Vector2>>& loops) {
        cleanup();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> phaseDist(0.0f, 10.0f);

        squares.clear();
        squares.reserve(loops.size());

        for (const auto& loop : loops) {
            if (loop.size() != 4) continue;

            Square square;

            for (int i = 0; i < 4; i++) {
                square.vertices[i] = loop[i];
            }
            square.phase = phaseDist(gen);
            square.renderTexture = LoadRenderTexture(512, 512);
            square.shader = LoadShaderFromMemory(vertexShaderSource, fragmentShaderSource);

            squares.push_back(square);
        }

        initialized = true;
    }

    void update(float deltaTime) {
        if (!initialized) return;

        static float totalTime = 0.0f;
        totalTime += deltaTime;
        for (auto& square : squares) {
            BeginTextureMode(square.renderTexture);
            ClearBackground(BLACK);
            SetShaderValue(square.shader, GetShaderLocation(square.shader, "time"), &totalTime, SHADER_UNIFORM_FLOAT);
            SetShaderValue(square.shader, GetShaderLocation(square.shader, "phase"), &square.phase, SHADER_UNIFORM_FLOAT);
            SetShaderValue(square.shader, GetShaderLocation(square.shader, "maxIter"), &maxIterations, SHADER_UNIFORM_INT);
            SetShaderValue(square.shader, GetShaderLocation(square.shader, "timeSpeed"), &timeSpeed, SHADER_UNIFORM_FLOAT);
            BeginShaderMode(square.shader);
            DrawRectangle(0, 0, 512, 512, WHITE);
            EndShaderMode();

            EndTextureMode();
        }
    }

    void render() {
        if (!initialized) return;

        for (const auto& square : squares) {
            float minX = square.vertices[0].x, maxX = square.vertices[0].x;
            float minY = square.vertices[0].y, maxY = square.vertices[0].y;

            for (int i = 1; i < 4; i++) {
                minX = fminf(minX, square.vertices[i].x);
                maxX = fmaxf(maxX, square.vertices[i].x);
                minY = fminf(minY, square.vertices[i].y);
                maxY = fmaxf(maxY, square.vertices[i].y);
            }
            Rectangle dest = { minX, minY, maxX - minX, maxY - minY };
            Rectangle source = { 0, 0, 512, -512 };
            DrawTexturePro(square.renderTexture.texture, source, dest, {0, 0}, 0, WHITE);
        }
    }

    void setMaxIterations(int maxIter) {
        maxIterations = maxIter;
    }

    void setTimeSpeed(float speed) {
        timeSpeed = speed;
    }

    void cleanup() {
        for (auto& square : squares) {
            if (square.renderTexture.id != 0) {
                UnloadRenderTexture(square.renderTexture);
            }
            if (square.shader.id != 0) {
                UnloadShader(square.shader);
            }
        }
        squares.clear();
        initialized = false;
    }
};


#endif //SIERPINSKIRENDERER_H
