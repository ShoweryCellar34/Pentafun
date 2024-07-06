#include <Pentagram.hpp>
#include <glad.h>

PNT::image image;

void eventCallback(PNT::Window* window, PNT::windowEvent event) {
    if(event.type == PNT_EVENT_TYPE_DROP) {
        image.load(event.dropFiles.paths[0]);
        window->setDimentions(image.getWidth(), image.getHeight());
        image.loadOnGPU();
    }
}

int main(int argc, char *argv[]) {
    if(!PNT::init()) {
        return 1;
    }

    image.load("res\\textures\\logo\\ghoul.png");
    PNT::Window window("Drag'n Drop", image.getWidth(), image.getHeight(), 500, 500, ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_DockingEnable);
    image.loadOnGPU();
    window.setEventCallback(eventCallback);

    // Vertex shader.
    PNT::file vertexFile("res\\shaders\\vertex.glsl");
    PNT::shader vertexShader(vertexFile.getContents().c_str(), GL_VERTEX_SHADER);
    vertexShader.compile();

    // Fragment shader.
    PNT::file fragmentFile("res\\shaders\\fragment.glsl");
    PNT::shader fragmentShader(fragmentFile.getContents().c_str(), GL_FRAGMENT_SHADER);
    fragmentShader.compile();

    // Shader program.
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader.getID());
    glAttachShader(shaderProgram, fragmentShader.getID());
    glLinkProgram(shaderProgram);

    while(!window.shouldClose()) {
        PNT::processEvents();

        window.startFrame();
        image.ImGuiDraw(ImVec2(500, 500));

        window.endFrame();
    }

    PNT::deinit();
    return 0;
}
